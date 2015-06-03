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
    absorption_ = shared_ptr<Absorption>(new Absorption("/Users/szymon/Projects/slap/data/absLines"));
    absFile_ = "06D4eu"; /*TODO: Need to be globaly setup*/
    derivedParams_ = {}; /*TODO: Why is this here?*/
}


SNModel::SNModel(shared_ptr<Filters> filters) {
    cosmology_ = shared_ptr<Cosmology>(new Cosmology(0));
    filters_ = filters;
    derivedParams_ = {};
    
}

/* TODO: CHANGE SO THAT IT TAKES WAVELENGTH AS INPUT*/
vector<double> SNModel::SED(double t, string f) {
    calcSEDParams(t);
    int ID = filters_->filterID_.at(f);
    int absID = absorption_->absID_.at(absFile_);
    vector<double> sed(filters_->filters_[ID].restWavelength_.size(), 0);
    for(int i = 0; i < filters_->filters_[ID].restWavelength_.size(); ++i) {
        sed[i] = calcSED(filters_->filters_[ID].restWavelength_[i]);
        sed[i] *= cosmology_->a_ / (4 * M_PI * pow(cosmology_->lumDisCGS_, 2));
        sed[i] *= absorption_->abs_[absID].filter_[ID].bandpass_[i];
    }

    return sed;
}

double SNModel::flux(double t, string f) {
    calcSEDParams(t);
    int ID = filters_->filterID_.at(f);
    int absID = absorption_->absID_.at(absFile_);
    vector<double> sed(filters_->filters_[ID].restWavelength_.size(), 0);

    for(int i = 0; i < filters_->filters_[ID].restWavelength_.size(); ++i) {
        sed[i] = calcSED(filters_->filters_[ID].restWavelength_[i]);
        sed[i] *= cosmology_->a_ / (4 * M_PI * pow(cosmology_->lumDisCGS_, 2));
        sed[i] *= absorption_->abs_[absID].filter_[ID].bandpass_[i];
    }

    return filters_->flux(sed, f);
}


double SNModel::mag(double t, string filterName) {
    int ID = filters_->filterID_.at(filterName);
    return -2.5 * log10(flux(t, filterName)) - filters_->filters_[ID].zp_;
}


// void SNModel::absFilter() {
//     vector <vector<double> > data;
//     loadtxt<double>("/Users/szymon/Projects/slap/data/absLines/" + absLineFile_ + ".abs", 2, data);
//     double min = data[0].front();
//     double max = data[0].back();
//     absLines_.resize(restWavelength_.size());

//     for (int i = 0; i < restWavelength_.size(); ++i) {
//         if (restWavelength_[i] < min) {
//             absLines_[i] = 0.4;
        
//         } else if (restWavelength_[i] > max) {
//             absLines_[i] = 1.0;
        
//         } else {
//             absLines_[i] = interp(restWavelength_[i], data[0], data[1]);
//         }
//     }
// }

