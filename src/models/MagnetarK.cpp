/*SLAP - Supernova Lightcurve Analysis Package
 *
 *Copyright (C) 2014  Szymon Prajs
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

#include "MagnetarK.h"

using namespace std;
using namespace vmath;


MagnetarK::MagnetarK(shared_ptr<Cosmology> cosmology, shared_ptr<Filters> filters) : SNModel(cosmology, filters) {
    noSEDParams_ = 2;
    noModelParams_ = 4;
    defaultParams_ = {5.0, 3, 1.5, 0.1};
    paramNames_ = {"Mej", "B", "P", "k"};

    modelParams_.resize(noModelParams_);
    SEDParams_.resize(noSEDParams_);
}


void MagnetarK::calcDerivedParams() {
    double temp;
    /* Constant quantities */
    energyRadiation_ = 0;
    alpha_ = 10;

    /* Magnetar spin down timescale */
    tauP_ = 4.7 * pow(modelParams_[1], -2.0) * pow(modelParams_[2], 2.0);
    
    /* Energy produced by the spin down of a magnetar */ 
    energyMagnetar_ = 4.9e46 * pow(modelParams_[1], 2.0) * pow(modelParams_[2], -4.0) * tauP_ * 86400;
    
    /* Kinetic energy of the system */
    energyKinetic_ = 1.0e51 + 0.5 * (energyMagnetar_ - energyRadiation_);
    
    /* Magnetar difusion timescale */
    tauM_ = 10 * pow(energyKinetic_ / 1.0e51, -0.25) * pow(modelParams_[3] / 0.1, 0.5) * pow(modelParams_[0], 0.75);

    /* core velocity */
    temp = energyKinetic_ * 10.0 / (3.0 * modelParams_[0] * 2.0e33); 
    velocityCore_ = sqrt(temp) * 86400;
}


double MagnetarK::lumMagnetar(double t) {
    return 4.9e46 * pow(modelParams_[1], 2.0) * pow(modelParams_[2], -4.0) / pow(1 + t / tauP_, 2.0);
}


double integralLumSNR(double t, void *param) {
    double *par = (double *)param;
    double tauP = par[0];
    double modelParams0 = par[1];
    double modelParams1 = par[2];
    double modelParams2 = par[3];

    double f = 4.9e46 * pow(modelParams1, 2.0) * pow(modelParams2, -4.0) / pow(1 + t / tauP, 2.0);
    double res = 2 * f * t * exp(pow(t / modelParams0, 2)) / pow(modelParams0, 2);

    return res;
}


double MagnetarK::lumSN(double t) {
    double res = exp(-1 * pow(t / tauM_, 2.0));

    gsl_integration_workspace *w = gsl_integration_workspace_alloc(10000);
    double integ, error;
    size_t neval;
    double par[] = {tauP_, tauM_, modelParams_[1], modelParams_[2]};

    gsl_function F;
    F.function = &integralLumSNR;
    F.params = &par;

    // for (int i = 0; i < 4; ++i) {
    //     cout << par[i] << " ";
    // }
    // cout << t << endl;
    int test = gsl_integration_qags(&F, 0, t, 1e39, 1e-7, 10000, w, &integ, &error);
    res *= integ;
    
    gsl_integration_workspace_free (w);
    return res;
}


double MagnetarK::radius(double t) {
    double radiusCore = velocityCore_ * t;
    double rhoCore = 3 * modelParams_[0] * 2e33 / (4 * M_PI * pow(velocityCore_ * t, 3));
    double tauCore = modelParams_[3] * rhoCore * velocityCore_ * t;

    double rad19 = radiusCore * pow((alpha_ - 1) / tauCore, 1 / (1 - alpha_)); 
    double rad20 = radiusCore - (1 - tauCore / (alpha_ - 1)) / (modelParams_[3] * rhoCore); 

    if (rad19 > radiusCore) {
        return rad19;
    } else if (rad20 > 0) {
        return rad20;
    } else {
        return 0;
    }

}


double MagnetarK::temperature(double t) {
    return pow(lumSN(t) / (CGS_SIGMA * 4 * M_PI * pow(radius(t), 2)), 0.25);
}


void MagnetarK::printDerivedVariables() {
    cout << tauM_ << " ";
    cout << velocityCore_ << " ";
    cout << energyMagnetar_ << " ";
    cout << energyKinetic_ << endl;
}


void MagnetarK::calcSEDParams(double t) {
    SEDParams_[0] = radius(t);
    SEDParams_[1] = temperature(t);
}
   
 
vector<double> MagnetarK::calcSED(double t) {
    calcSEDParams(t);
    vector<double> sed(restWavelength_.size(), 0);

    double res;
    for(int i = 0; i < restWavelength_.size (); ++i) {
        res = 2.0 * CGS_H * M_PI * pow(CGS_C,2) / pow(restWavelength_[i] * 1e-8,5);
        res /= exp(CGS_H * CGS_C / (restWavelength_[i] * 1e-8 * CGS_K * SEDParams_[1])) - 1.0;
        res *= 4 * M_PI * pow(SEDParams_[0],2);
        res *= 1e-8;
        sed[i] = res;
    }

    return sed;
}
