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

#include "Filters.h"

using namespace std;
using namespace vmath;


Filters::Filters(string path) : folderPath_(path) {
    readFolder();
    setFullRange();
}

void Filters::readFolder() {
    vector<string> list = dirlist(folderPath_);

    vector<string> temp;
    for (int i = 0; i < list.size(); ++i) {
        fileList_.push_back(list[i]);
        split(list[i],'.',temp);

        filterID_[temp[0]] = i;
        filterName_[i] = temp[0];
        
        loadFilter(i);
    }
}

void Filters::loadFilter(int ID) {
    FilterData filter;

    vector< vector<double> > data;
    string path = folderPath_+"/"+fileList_[ID];
    loadtxt<double>(path,2,data);

    filter.name_ = filterName_[ID];
    filter.inputWavelength_ = data[0];
    filter.inputBandpass_ = data[1];
    filter.area_ = trapz<double>(data[1],data[0][1]-data[0][0]);

    filters_.push_back(filter);
}

void Filters::rescale(const vector<double> &wavelength) {
    masterWavelength_ = wavelength;

    for (int i = 0; i < filters_.size(); ++i) {
        filters_[i].wavelength_ = masterWavelength_;
        filters_[i].bandpass_ = interp<double>(masterWavelength_,filters_[0].inputWavelength_,filters_[0].inputBandpass_);
    }
}

void Filters::rescale(double start, double end, double step) {
    masterWavelength_ = range<double>(start, end, step);

    for (int i = 0; i < filters_.size(); ++i) {
        filters_[i].wavelength_ = masterWavelength_;
        filters_[i].bandpass_ = interp<double>(masterWavelength_,filters_[i].inputWavelength_,filters_[i].inputBandpass_);
    }
}

void Filters::setFullRange() {
    double start = filters_[0].inputWavelength_.front();
    double end = filters_[0].inputWavelength_.back();
    double step = filters_[0].inputWavelength_[1] - filters_[0].inputWavelength_[0];

    for (int i = 1; i < filters_.size(); ++i) {
        if (filters_[i].inputWavelength_.front() < start) {
            start = filters_[i].inputWavelength_.front();
        }
        if (filters_[i].inputWavelength_.back() > end) {
            end = filters_[i].inputWavelength_.back();
        }
        if ((filters_[i].inputWavelength_[1] - filters_[i].inputWavelength_[0]) < step) {
            step = filters_[i].inputWavelength_[1] - filters_[i].inputWavelength_[0];
        }
    }

    rescale(start, end, step);
}

double Filters::flux(const vector<double>& SED, const string& filterName) {
    if (filterID_.find(filterName) == filterID_.end()) {
        return 0;
    } else {
        int ID = filterID_.at(filterName);
        vector<double> filteredSED = mult<double>(SED, filters_[ID].bandpass_);
        double integFlux = trapz<double>(filteredSED, filters_[ID].wavelength_[1] - filters_[ID].wavelength_[0]);
 
        return integFlux / filters_[ID].area_;
    }
}