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

    double janskyConst = 3631 * 1e-23 * 299792458 * 1e10;
    vector<double> waveSq = mult<double>(data[0], data[0]);
    vector<double> jansky = div<double>(janskyConst, waveSq);
    jansky = mult<double>(jansky, data[1]);
    double fluxZp = trapz<double>(jansky, data[0][1] - data[0][0]);

    filter.name_ = filterName_[ID];
    filter.inputWavelength_ = data[0];
    filter.inputBandpass_ = data[1];
    filter.wavelength_ = filter.inputWavelength_;
    filter.bandpass_ = filter.inputBandpass_;
    filter.restWavelength_ = filter.inputWavelength_;

    filter.area_ = trapz<double>(data[1], data[0][1] - data[0][0]);
    filter.zp_ = -2.5 * log10(fluxZp / filter.area_);

    filters_.push_back(filter);
}


void Filters::rescale(const vector<double> &wavelength) {
    for (int i = 0; i < filters_.size(); ++i) {
        filters_[i].wavelength_ = wavelength;
        filters_[i].bandpass_ = interp<double>(wavelength,filters_[i].inputWavelength_,filters_[i].inputBandpass_);
    }
}


void Filters::rescale(double start, double end, double step) {
    vector<double> wavelength = range<double>(start, end, step);
    for (int i = 0; i < filters_.size(); ++i) {
        filters_[i].wavelength_ = wavelength;
        filters_[i].bandpass_ = interp<double>(wavelength,filters_[i].inputWavelength_,filters_[i].inputBandpass_);
    }
}

void Filters::rescale(double step) {
    double start, end;
    for (int i = 0; i < filters_.size(); ++i) {
        start = filters_[i].inputWavelength_.front();
        start -= fmod(start, step);
        end = filters_[i].inputWavelength_.back();
        end -= fmod(end, step);
        end += step;

        filters_[i].wavelength_ = range<double>(start, end, step);
        filters_[i].bandpass_ = interp<double>(filters_[i].wavelength_,filters_[i].inputWavelength_,filters_[i].inputBandpass_);
    }
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