/*SLAP - Supernova Lightcurve Analysis Package
 *
 *Copyright (C) 2014-2015  Szymon Prajs
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

#include "Magnetar.h"

using namespace std;
using namespace vmath;


Magnetar::Magnetar(shared_ptr<Cosmology> cosmology, shared_ptr<Filters> filters, shared_ptr<Absorption> absorption) : SNModel(cosmology, filters, absorption) {
    noSEDParams_ = 2;
    noModelParams_ = 3;
    defaultParams_ = {32.0, 3.0, 1.4};
    lParams_ = {10, 0.1, 0.1};
    uParams_ = {100, 20, 20};
    paramNames_ = {"tauM", "B", "P"};

    modelParams_.resize(noModelParams_);
    SEDParams_.resize(noSEDParams_);
}


void Magnetar::calcDerivedParams() {
    double temp;
    /* Constant quantities */
    opacity_ = 0.1;
    energyRadiation_ = 0;
    alpha_ = 10;

    /* Magnetar spin down timescale */
    tauP_ = 4.7 * pow(modelParams_[1], -2.0) * pow(modelParams_[2], 2.0);
    
    /* Energy produced by the spin down of a magnetar */ 
    energyMagnetar_ = 4.9e46 * pow(modelParams_[1], 2.0) * pow(modelParams_[2], -4.0) * tauP_ * 86400;
    
    /* Kinetic energy of the system */
    energyKinetic_ = 1.0e51 + 0.5 * (energyMagnetar_ - energyRadiation_);
    
    /* Mass ejected in the explosion */
    // temp = 10 * pow(energyKinetic_ / 1.0e51, -0.25) * pow(opacity_ / 0.1, 0.5);
    // ejectedMass_ = pow(modelParams_[0] / temp, 4.0/3.0);
    ejectedMass_ = pow(modelParams_[0] / 10, 4.0/3.0) * pow(energyKinetic_ / 1.0e51, 1.0 / 3.0) * pow(opacity_ / 0.1, -2.0 / 3.0);

    /* core velocity */
    temp = energyKinetic_ * 10.0 / (3.0 * ejectedMass_ * 2.0e33); 
    velocityCore_ = sqrt(temp) * 86400;

    derivedParams_ = {energyMagnetar_, energyKinetic_, ejectedMass_, velocityCore_};

    Wang14_ = 3.0 * opacity_ * ejectedMass_ * 1.989e33 / (4.0 * M_PI * pow(velocityCore_, 2.0));
}


double Magnetar::lumMagnetar(double t) {
    return 4.9e46 * pow(modelParams_[1], 2.0) * pow(modelParams_[2], -4.0) / pow(1 + t / tauP_, 2.0);
}


double integralLumSN(double t, void *param) {
    double *par = (double *)param;
    double tauP = par[0];
    double modelParams0 = par[1];
    double modelParams1 = par[2];
    double modelParams2 = par[3];

    double f = 4.9e46 * pow(modelParams1, 2.0) * pow(modelParams2, -4.0) / pow(1 + t / tauP, 2.0);
    double res = 2 * f * t * exp(pow(t / modelParams0, 2)) / pow(modelParams0, 2);

    return res;
}


double Magnetar::lumSN(double t) {
    double res = exp(-1 * pow(t / modelParams_[0], 2.0));

    /*Factor based on Wang et. al. (2014) */
    res *= (1 - exp(-Wang14_ * pow(t, -2.0)));

    gsl_integration_workspace *w = gsl_integration_workspace_alloc(10000);
    double integ, error;
    size_t neval;
    double par[] = {tauP_, modelParams_[0], modelParams_[1], modelParams_[2]};

    gsl_function F;
    F.function = &integralLumSN;
    F.params = &par;

    if (t > 200) {
        return 0;
    }

    int test = gsl_integration_qags(&F, 0, t, 1e39, 1e-7, 10000, w, &integ, &error);
    res *= integ;
    
    gsl_integration_workspace_free (w);
    return res;
}


double Magnetar::radius(double t) {
    double radiusCore = velocityCore_ * t;
    double rhoCore = 3 * ejectedMass_ * 2e33 / (4 * M_PI * pow(velocityCore_ * t, 3));
    double tauCore = opacity_ * rhoCore * velocityCore_ * t;

    double rad19 = radiusCore * pow((alpha_ - 1) / tauCore, 1 / (1 - alpha_)); 
    double rad20 = radiusCore - (1 - tauCore / (alpha_ - 1)) / (opacity_ * rhoCore); 

    if (rad19 > radiusCore) {
        return rad19;
    } else if (rad20 > 0) {
        return rad20;
    } else {
        return 0;
    }

}


double Magnetar::temperature(double t) {
    return pow(lumSN(t) / (CGS_SIGMA * 4 * M_PI * pow(radius(t), 2)), 0.25);
}


void Magnetar::printDerivedVariables() {
    cout << energyMagnetar_ << " ";
    cout << energyKinetic_ << " ";
    cout << ejectedMass_ << " ";
    cout << velocityCore_ << " ";
}


void Magnetar::calcSEDParams(double t) {
    SEDParams_[0] = radius(t);
    SEDParams_[1] = temperature(t);
}
   
 
double Magnetar::calcSED(double wav) {
    double res;
    res = 2.0 * CGS_H * M_PI * pow(CGS_C,2) / pow(wav * 1e-8, 5);
    res /= exp(CGS_H * CGS_C / (wav * 1e-8 * CGS_K * SEDParams_[1])) - 1.0;
    res *= 4 * M_PI * pow(SEDParams_[0],2);
    res *= 1e-8;

    return res;
}
