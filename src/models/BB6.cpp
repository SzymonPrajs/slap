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


BB6::BB6(shared_ptr<Cosmology> cosmology, shared_ptr<Filters> filters) : SNModel(cosmology, filters) {
    SEDParams_.resize(2);
    modelParam_.resize(5);
}


void BB6::calcSEDParams(double t) { 
    SEDParams_[0] = (modelParam_[0] * 1e14 * t) + (modelParam_[1] * 1e14 * t * t);
    SEDParams_[1] = modelParam_[2] + (modelParam_[3] * t) + (modelParam_[4] * t * t);
}

    
vector<double> BB6::calcSED(double t) {
    calcSEDParams(t);
    vector<double> sed(restWavelength_.size(), 0);

    double res;
    for(int i = 0; i < restWavelength_.size (); ++i) {
        res = 2.0 * CGS_H * M_PI * pow(CGS_C,2) / pow(restWavelength_[i] * 1e-8,5);
        res /= exp(CGS_H * CGS_C / (restWavelength_[i] * 1e-8 * CGS_K * SEDParams_[1])) - 1.0;
        res *= 4 * M_PI * pow(SEDParams_[0], 2);
        res *= 1e-8;
        sed[i] = res;
    }

    return sed;
}
