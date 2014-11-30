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


SNEvent::SNEvent(string file, shared_ptr<SNModel> model) : snmodel_(model) {
    cosmology_ = snmodel_->cosmology_;
    filters_ = snmodel_->filters_;
    readData(file);
    setFilterList();
    verifyFilters();
    // setFilterRange();
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

/*
 *Check entire filter array and select all unique filters. MUST BE SORTED BY FILTERS!s
 */
void SNEvent::setFilterList() {
    vector<string> list;
    unique_copy(filter_.begin(), filter_.end(), back_inserter(list), compareStrings);

    filterList_ = list;
}


void SNEvent::verifyFilters() {
    for (int i = 0; i < filterList_.size(); ++i) {
        if(filters_->filterID_.find(filterList_[i]) == filters_->filterID_.end()) {
            cout << "WARNING! No filter responce data found for filter '" << filterList_[i] << "'\n";
            cout << "Lightcurve points for this filter will now be removed and the program continues normal execution." << endl;

            removeData(filterList_[i]);
        }
    }
}


void SNEvent::setFilterRange() {
    int ID;
    double start = 9999999; 
    double end = 0;
    double step = 999999;


    for (int i = 0; i < filterList_.size(); ++i) {
        ID = filters_->filterID_[filterList_[i]];

        if (filters_->filters_[ID].inputWavelength_.front() < start) {
            start = filters_->filters_[ID].inputWavelength_.front();
        }
        if (filters_->filters_[ID].inputWavelength_.back() > end) {
            end = filters_->filters_[ID].inputWavelength_.back();
        }
        if ((filters_->filters_[ID].inputWavelength_[1] - filters_->filters_[ID].inputWavelength_[0]) < step) {
            step = filters_->filters_[ID].inputWavelength_[1] - filters_->filters_[ID].inputWavelength_[0];
        }
    }

    filters_->rescale(start, end, step);
    snmodel_->setWavelength();
}


/*
 *TODO - Test for real data
 */
void SNEvent::removeData(double start, double end) {
    vector<double>::iterator mjd_it = mjd_.begin();
    vector<double>::iterator flux_it = flux_.begin();
    vector<double>::iterator fluxErr_it = fluxErr_.begin();
    vector<string>::iterator filter_it = filter_.begin();

    for (; mjd_it != mjd_.end(); ) {
        if (*mjd_it < start || *mjd_it > end) {
            mjd_it = mjd_.erase(mjd_it);
            flux_it = flux_.erase(flux_it);
            fluxErr_it = fluxErr_.erase(fluxErr_it);
            filter_it = filter_.erase(filter_it);
        } else {
            mjd_it++;
            flux_it++;
            fluxErr_it++;
            filter_it++;
        }     
    }
}


void SNEvent::removeData(string filter) {
    vector<double>::iterator mjd_it = mjd_.begin();
    vector<double>::iterator flux_it = flux_.begin();
    vector<double>::iterator fluxErr_it = fluxErr_.begin();
    vector<string>::iterator filter_it = filter_.begin();

    for (; filter_it != filter_.end(); ) {
        if (*filter_it == filter) {
            mjd_it = mjd_.erase(mjd_it);
            flux_it = flux_.erase(flux_it);
            fluxErr_it = fluxErr_.erase(fluxErr_it);
            filter_it = filter_.erase(filter_it);
        } else {
            mjd_it++;
            flux_it++;
            fluxErr_it++;
            filter_it++;
        }     
    }
}