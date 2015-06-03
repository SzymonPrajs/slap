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

#include "BB4.h"

using namespace std;
using namespace vmath;


BB4::BB4(shared_ptr<Cosmology> cosmology, shared_ptr<Filters> filters, shared_ptr<Absorption> absorption) : SNModel(cosmology, filters, absorption) {
    noSEDParams_ = 2;
    noModelParams_ = 3;
    defaultParams_ = {1.0, 15000, 150};
    lParams_ = {0.01, 5000, 10};
    uParams_ = {3.0, 25000, 2000};
    paramNames_ = {"dR", "T0", "dT"};
    
    modelParams_.resize(noModelParams_);
    SEDParams_.resize(noSEDParams_);
}


void BB4::calcSEDParams(double t) {
    SEDParams_[0] = modelParams_[0] * 1e14 * t;
    SEDParams_[1] = modelParams_[1] - (modelParams_[2] * t);
}
   
 
double BB4::calcSED(double wav) {
    double res;
    res = 2.0 * CGS_H * M_PI * pow(CGS_C,2) / pow(wav * 1e-8, 5);
    res /= exp(CGS_H * CGS_C / (wav * 1e-8 * CGS_K * SEDParams_[1])) - 1.0;
    res *= 4 * M_PI * pow(SEDParams_[0],2);
    res *= 1e-8;

    return res;
}
