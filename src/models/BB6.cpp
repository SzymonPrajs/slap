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

#include "BB6.h"

using namespace std;
using namespace vmath;


BB6::BB6(shared_ptr<Cosmology> cosmology, shared_ptr<Filters> filters, shared_ptr<Absorption> absorption) : SNModel(cosmology, filters, absorption) {
    noSEDParams_ = 2;
    noModelParams_ = 5;
    defaultParams_ = {1.0, 0.01, 15000, 150, 1.5};
    lParams_ = {0.1, 0.001, 5000, 10, 0.001};
    uParams_ = {3.0, 1, 25000, 2000, 20};
    paramNames_ = {"dR", "d2R", "T0", "dT", "d2T"};

    modelParams_.resize(noModelParams_);
    SEDParams_.resize(noSEDParams_);
}


void BB6::calcSEDParams(double t) { 
    SEDParams_[0] = (modelParams_[0] * 1e14 * t) + (modelParams_[1] * 1e14 * t * t);
    SEDParams_[1] = modelParams_[2] - (modelParams_[3] * t) + (modelParams_[4] * t * t);
}

    
double BB6::calcSED(double wav) {
    double res;
    res = 2.0 * CGS_H * M_PI * pow(CGS_C,2) / pow(wav * 1e-8, 5);
    res /= exp(CGS_H * CGS_C / (wav * 1e-8 * CGS_K * SEDParams_[1])) - 1.0;
    res *= 4 * M_PI * pow(SEDParams_[0],2);
    res *= 1e-8;

    return res;
}
