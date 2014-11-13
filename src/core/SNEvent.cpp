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

#include "SNEvent.h"

using namespace std;
using namespace vmath;


SNEvent::SNEvent(string file, shared_ptr<SNModel> model) {
    snmodel_ = model;
    readData(file);
    setFilterList();
    setFilterRange();
}

void SNEvent::readData(string file) {
    vector< vector<string> > data;
    loadtxt<string>(file, 4, data);

    completeMJD_ = castString<double>(data[0]);
    completeFlux_ = castString<double>(data[1]);
    completeFluxErr_ = castString<double>(data[2]);
    completeFilter_ = data[3];

    restoreCompleteLC();
    explosionMJD_ = min<double>(mjd_);
}

void SNEvent::restoreCompleteLC() {
	mjd_ = completeMJD_;
    flux_ = completeFlux_; 
    fluxErr_ = completeFluxErr_; 
    filter_ = completeFilter_;
}

void SNEvent::setFilterList() {
    vector<string> list;
    unique_copy(filter_.begin(), filter_.end(), back_inserter(list), compareStrings);

    filterList_ = list;
}

void SNEvent::setFilterRange() {
    setFilterList();
    int ID;
    double start = 999999; 
    double end = 0;
    double step = 9999;


    for (int i = 0; i < filterList_.size(); ++i) {
        ID = snmodel_->filters_->filterID_[filterList_[i]];

        if (snmodel_->filters_->filters_[ID].inputWavelength_.front() < start) {
            start = snmodel_->filters_->filters_[ID].inputWavelength_.front();
        }
        if (snmodel_->filters_->filters_[ID].inputWavelength_.back() > end) {
            end = snmodel_->filters_->filters_[ID].inputWavelength_.back();
        }
        if ((snmodel_->filters_->filters_[ID].inputWavelength_[1] - snmodel_->filters_->filters_[ID].inputWavelength_[0]) < step) {
            step = snmodel_->filters_->filters_[ID].inputWavelength_[1] - snmodel_->filters_->filters_[ID].inputWavelength_[0];
        }
    }

    snmodel_->filters_->rescale(start, end, step);
    snmodel_->setWavelength();
    cout << snmodel_->filters_->masterWavelength_.front() << " " << snmodel_->filters_->masterWavelength_.back() << endl;
}