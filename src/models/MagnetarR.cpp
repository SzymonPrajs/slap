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

#include "MagnetarR.h"

using namespace std;
using namespace vmath;


MagnetarR::MagnetarR(shared_ptr<Cosmology> cosmology, shared_ptr<Filters> filters) : SNModel(cosmology, filters) {
    noSEDParams_ = 2;
    noModelParams_ = 4;
    defaultParams_ = {30, 3, 1.5, 30};

    modelParams_.resize(noModelParams_);
    SEDParams_.resize(noSEDParams_);
}


void MagnetarR::calcDerivedParams() {
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
    temp = 10 * pow(energyKinetic_ / 1.0e51, -0.25) * pow(opacity_ / 0.1, 0.5);
    ejectedMass_ = pow(modelParams_[0] / temp, 4.0/3.0);

    /* core velocity */
    temp = energyKinetic_ * 10.0 / (3.0 * ejectedMass_ * 2.0e33); 
    velocityCore_ = sqrt(temp) * 86400;
}


double MagnetarR::lumMagnetar(double t) {
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


double MagnetarR::lumSN(double t) {
    double res = exp(-1 * pow(t / modelParams_[0], 2.0));

    gsl_integration_workspace *w = gsl_integration_workspace_alloc(1000);
    double integ, error;
    size_t neval;
    double par[] = {tauP_, modelParams_[0], modelParams_[1], modelParams_[2]};

    gsl_function F;
    F.function = &integralLumSNR;
    F.params = &par;

    gsl_integration_qags(&F, 0, t, 1e39, 1e-7, 1000, w, &integ, &error);
    res *= integ;
    
    gsl_integration_workspace_free (w);
    return res;
}


double MagnetarR::energy(double t) {
    vector<double> arr(int(t+1));

    for (int i = 0; i < int(t+1); ++i) {
        arr[i] = lumSN(i);
    }
    
    return trapz<double>(arr, 86400); 
}


double MagnetarR::radius(double t) {
    double radiusCore = modelParams_[3] * 1e14 + velocityCore_ * t;
    double rhoCore = 3 * ejectedMass_ * 2e33 / (4 * M_PI * pow(velocityCore_ * t, 3));
    double tauCore = opacity_ * rhoCore * velocityCore_ * t;

    double rad19 = radiusCore * pow((alpha_ - 1) / tauCore, 1 / (1 - alpha_)); 
    double rad20 = radiusCore - (1 - tauCore / (alpha_ - 1)) / (opacity_ * rhoCore); 

    if (rad19 > radiusCore) {
        return rad20;
    } else {
        return rad20;
    }

}


double MagnetarR::temperature(double t) {
    return pow(lumSN(t) / (CGS_SIGMA * 4 * M_PI * pow(radius(t), 2)), 0.25);
}


void MagnetarR::printDerivedVariables() {
    cout << "Magnetar: " << energyMagnetar_ << endl;
    cout << "Kinetic: " << energyKinetic_ << endl;
    cout << "Mass: " << ejectedMass_ << endl;
    cout << "Velocity: " << velocityCore_ << endl;
}


void MagnetarR::calcSEDParams(double t) {
    SEDParams_[0] = radius(t);
    SEDParams_[1] = temperature(t);
}
   
 
vector<double> MagnetarR::calcSED(double t) {
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