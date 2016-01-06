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

#include "Piro.h"

using namespace std;
using namespace vmath;


Piro::Piro(shared_ptr<Cosmology> cosmology, shared_ptr<Filters> filters, shared_ptr<Absorption> absorption) : SNModel(cosmology, filters, absorption) {
    noSEDParams_ = 2;
    noModelParams_ = 3;
    defaultParams_ = {2e13, 3.0, 10.0};
    lParams_ = {1e12, 0.1, 1};
    uParams_ = {5e14, 10.0, 100};
    paramNames_ = {"Re", "Me", "Mc"};

    modelParams_.resize(noModelParams_);
    SEDParams_.resize(noSEDParams_);
}


void Piro::calcDerivedParams() {
    double temp;
    
    /* Constant quantities */
    opacity_ = 0.1;
    energyKinetic_ = 6e51;

    /* Piro derived parameters */
    velocityExtended_ = 2.0e9 * pow(energyKinetic_ / 1e51, 0.5) * pow(modelParams_[2], -0.35) * pow (modelParams_[1] / 0.01, -0.15); 
    timescaleExtended_ = modelParams_[0] / velocityExtended_;
    timescalePeak_ = (0.9 * pow(opacity_ / 0.34,0.5) * pow(energyKinetic_ / 1e51,0.25) * pow(modelParams_[2],0.17) * pow(modelParams_[1] / 0.01,0.57)) * 86400;
    energyExtended_ = 4e49 * (energyKinetic_ / 1e51) * pow(modelParams_[2],-0.7) * pow(modelParams_[1] / 0.01,0.7); 

}


double Piro::lumSN(double t) {
    t *= 86400;
    return (timescaleExtended_ * energyExtended_ / pow(timescalePeak_, 2)) * exp(- t * (t + 2*timescaleExtended_) / (2 * pow(timescalePeak_, 2)));
}

double Piro::temperature(double t) {
    return pow(lumSN(t) / (CGS_SIGMA * 4 * M_PI * pow(radius(t), 2)), 0.25);
}

double Piro::radius(double t) {
    return modelParams_[0] + velocityExtended_ * t * 86400;
}

void Piro::printDerivedVariables() {}


void Piro::calcSEDParams(double t) {
    SEDParams_[0] = radius(t);
    SEDParams_[1] = temperature(t);
}
   
 
double Piro::calcSED(double wav) {
    double res;
    res = 2.0 * CGS_H * M_PI * pow(CGS_C,2) / pow(wav * 1e-8, 5);
    res /= exp(CGS_H * CGS_C / (wav * 1e-8 * CGS_K * SEDParams_[1])) - 1.0;
    res *= 4 * M_PI * pow(SEDParams_[0],2);
    res *= 1e-8;

    return res;
}
