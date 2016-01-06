/*SLAP - Supernova Lightcurve Analysis Package
 *
 *Copyright (C) 2014-2016  Szymon Prajs
 *
 *This program is free software; you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation; either version 2 of the License, or
 *(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License along
 *with this program; if not, write to the Free Software Foundation, Inc.,
 *51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. 

 Contact author: S.Prajs@soton.ac.uk
 */

#include "MagnetarPiro.h"

using namespace std;
using namespace vmath;


MagnetarPiro::MagnetarPiro(shared_ptr<Cosmology> cosmology, shared_ptr<Filters> filters, shared_ptr<Absorption> absorption) : SNModel(cosmology, filters, absorption) {
    noSEDParams_ = 2;
    noModelParams_ = 6;
    defaultParams_ = {2e13, 3.0, 10.0, 3.0, 1.4, 6e51};
    lParams_ = {1e12, 0.1, 1, 0.1, 0.1, 1e51};
    uParams_ = {5e14, 10.0, 100, 20, 20, 5e52};
    paramNames_ = {"Re", "Me", "Mc", "B", "P", "Ek"};

    modelParams_.resize(noModelParams_);
    SEDParams_.resize(noSEDParams_);
}


void MagnetarPiro::calcDerivedParams() {
    double temp;
    
    /* Constant quantities */
    opacity_ = 0.1;
    energyRadiation_ = 0;
    energySN_ = 1e51;
    alpha_ = 10;

    /* Magnetar spin down timescale */
    tauP_ = 4.7 * pow(modelParams_[3], -2.0) * pow(modelParams_[4], 2.0);
    
    /* Energy produced by the spin down of a magnetar */ 
    energyMagnetar_ = 4.9e46 * pow(modelParams_[3], 2.0) * pow(modelParams_[4], -4.0) * tauP_ * 86400;
    
    /* Kinetic energy of the system */
    energyKinetic_ = modelParams_[5] + 0.5 * (energyMagnetar_ - energyRadiation_);

    /* Piro derived parameters */
    velocityExtended_ = 2.0e9 * pow(modelParams_[5] / 1e51, 0.5) * pow(modelParams_[2], -0.35) * pow (modelParams_[1] / 0.01, -0.15); 
    timescaleExtended_ = modelParams_[0] / velocityExtended_;
    timescalePeak_ = (0.9 * pow(opacity_ / 0.34,0.5) * pow(modelParams_[5] / 1e51,0.25) * pow(modelParams_[2],0.17) * pow(modelParams_[1] / 0.01,0.57)) * 86400;
    energyExtended_ = 4e49 * (modelParams_[5] / 1e51) * pow(modelParams_[2],-0.7) * pow(modelParams_[1] / 0.01,0.7); 

    /* Diffusion timecale */
    tauM_ = pow(modelParams_[2] * pow(energyKinetic_ / 1.0e51, -1.0 / 3.0) * pow(opacity_ / 0.1, 2.0 / 3.0), 3.0 / 4.0) * 10;

    /* core velocity */
    temp = energyKinetic_ * 10.0 / (3.0 * modelParams_[2] * 2.0e33); 
    velocityCore_ = sqrt(temp) * 86400;

    /* Wang et. al 2014 late time corrction*/
    Wang14_ = 3.0 * opacity_ * modelParams_[2] * 1.989e33 / (4.0 * M_PI * pow(velocityCore_, 2.0));

    derivedParams_ = {energyMagnetar_, energyKinetic_, tauM_, velocityCore_};
}


double MagnetarPiro::lumMagnetar(double t) {
    return 4.9e46 * pow(modelParams_[3], 2.0) * pow(modelParams_[4], -4.0) / pow(1 + t / tauP_, 2.0);
}


double integralLumMag(double t, void *param) {
    double *par = (double *)param;
    double tauP = par[0];
    double modelParams0 = par[1];
    double modelParams1 = par[2];
    double modelParams2 = par[3];

    double f = 4.9e46 * pow(modelParams1, 2.0) * pow(modelParams2, -4.0) / pow(1 + t / tauP, 2.0);
    double res = 2 * f * t * exp(pow(t / modelParams0, 2)) / pow(modelParams0, 2);

    return res;
}


double MagnetarPiro::lumMag(double t) {
    double res = exp(-1 * pow(t / tauM_, 2.0));

    /*Factor based on Wang et. al. (2014) */
    res *= (1 - exp(-Wang14_ * pow(t, -2.0)));

    gsl_integration_workspace *w = gsl_integration_workspace_alloc(10000);
    double integ, error;
    size_t neval;
    double par[] = {tauP_, tauM_, modelParams_[3], modelParams_[4]};

    gsl_function F;
    F.function = &integralLumMag;
    F.params = &par;

    if (t > 200) {
        return 0;
    }

    int test = gsl_integration_qags(&F, 0, t, 1e39, 1e-7, 10000, w, &integ, &error);
    res *= integ;
    
    gsl_integration_workspace_free (w);
    return res;
}


double MagnetarPiro::lumPiro(double t) {
    t *= 86400;
    return (timescaleExtended_ * energyExtended_ / pow(timescalePeak_, 2)) * exp(- t * (t + 2*timescaleExtended_) / (2 * pow(timescalePeak_, 2)));
}


double MagnetarPiro::lumSN(double t) {
    return lumMag(t) + lumPiro(t);
}

double MagnetarPiro::temperature(double t) {
    return pow(lumSN(t) / (CGS_SIGMA * 4 * M_PI * pow(radius(t), 2)), 0.25);
}

double MagnetarPiro::radius(double t) {
    double radPiro = modelParams_[0] + velocityExtended_ * t * 86400;

    double radiusCore = velocityCore_ * t;
    double rhoCore = 3 * modelParams_[2] * 2e33 / (4 * M_PI * pow(velocityCore_ * t, 3));
    double tauCore = opacity_ * rhoCore * velocityCore_ * t;

    double rad19 = radiusCore * pow((alpha_ - 1) / tauCore, 1 / (1 - alpha_)); 
    double rad20 = radiusCore - (1 - tauCore / (alpha_ - 1)) / (opacity_ * rhoCore); 

    if (radPiro > rad19) {
        return rad19;
    } else if (radPiro < rad19 && rad19 > radiusCore) {
        return rad19;
    } else if (rad20 > 0) {
        return rad20;
    } else {
        return 0;
    }
}

void MagnetarPiro::printDerivedVariables() {}


void MagnetarPiro::calcSEDParams(double t) {
    SEDParams_[0] = radius(t);
    SEDParams_[1] = temperature(t);
}
   
 
double MagnetarPiro::calcSED(double wav) {
    double res;
    res = 2.0 * CGS_H * M_PI * pow(CGS_C,2) / pow(wav * 1e-8, 5);
    res /= exp(CGS_H * CGS_C / (wav * 1e-8 * CGS_K * SEDParams_[1])) - 1.0;
    res *= 4 * M_PI * pow(SEDParams_[0],2);
    res *= 1e-8;

    return res;
}
