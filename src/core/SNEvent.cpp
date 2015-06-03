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
    absorption_ = snmodel_->absorption_;
    readData(file);
    setFilterList();
    verifyFilters();
    setRestWavelength();
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
 *Check entire filter array and select all unique filters. MUST BE SORTED BY FILTERS!
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

/*TODO: do this for ABSORPTION too*/
void SNEvent::setRestWavelength() {
    for (int i = 0; i < filters_->filters_.size(); ++i) {
        filters_->filters_[i].restWavelength_ = mult<double>(filters_->filters_[i].wavelength_, cosmology_->a_);  
   }

   AbsFilter flt;
   for (int i = 0; i < absorption_->abs_.size(); ++i) {
        absorption_->abs_[i].filterID_ = filters_->filterID_;
        absorption_->abs_[i].filterName_ = filters_->filterName_;
        
        for (int j = 0; j < absorption_->abs_[i].filterID_.size(); ++j) {
            flt.restWavelength_ = filters_->filters_[j].restWavelength_;
            flt.bandpass_ = interp<double>(flt.restWavelength_,
                            absorption_->abs_[i].inputWavelength_,
                            absorption_->abs_[i].inputBandpass_,
                            0.4,1.0);
            absorption_->abs_[i].filter_.push_back(flt);
        }
   }
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