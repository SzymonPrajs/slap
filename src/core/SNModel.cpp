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

#include "SNModel.h"

using namespace std;
using namespace vmath;


SNModel::SNModel(shared_ptr<Cosmology> cosmology, shared_ptr<Filters> filters) {
    cosmology_ = cosmology;
    filters_ = filters;
    derivedParams_ = {};
    absLineFile_ = "06D4eu";
    
    setWavelength();
}


SNModel::SNModel(shared_ptr<Filters> filters) {
    cosmology_ = shared_ptr<Cosmology>(new Cosmology(0));
    filters_ = filters;
    derivedParams_ = {};
    absLineFile_ = "06D4eu";

    setWavelength();
}


double SNModel::flux(double t, string filterName) {
    vector<double> sed = calcSED(t * cosmology_->a_);
    sed = mult<double>(sed, cosmology_->a_ / (4 * M_PI * pow(cosmology_->lumDisCGS_, 2)));
    // sed = mult<double>(sed, absLines_);
    return filters_->flux(sed, filterName);
}


void SNModel::setWavelength() {
    obsWavelength_ = filters_->masterWavelength_;
    restWavelength_ = mult<double>(obsWavelength_,cosmology_->a_);
    absFilter();
}


void SNModel::absFilter() {
    vector <vector<double> > data;
    loadtxt<double>("/Users/szymon/Projects/slap/data/absLines/" + absLineFile_ + ".abs", 2, data);
    double min = data[0].front();
    double max = data[0].back();
    absLines_.resize(restWavelength_.size());

    for (int i = 0; i < restWavelength_.size(); ++i) {
        if (restWavelength_[i] < min) {
            absLines_[i] = 0.2;
        
        } else if (restWavelength_[i] > max) {
            absLines_[i] = 1.0;
        
        } else {
            absLines_[i] = interp(restWavelength_[i], data[0], data[1]);
        }
    }
}

