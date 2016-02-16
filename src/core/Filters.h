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

#ifndef SLAP_CORE_FILTERS_H_
#define SLAP_CORE_FILTERS_H_

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
#include "../utils/utils.h"
#include "../vmath/loadtxt.h"
#include "../vmath/integrate.h"
#include "../vmath/interp.h"
#include "../vmath/range.h"
#include "../vmath/algebra.h"

using namespace std;
using namespace vmath;


struct FilterData {
    string name_;
    vector<double> inputWavelength_;  
    vector<double> inputBandpass_;
    vector<double> restWavelength_;
    vector<double> wavelength_;
    vector<double> bandpass_;
    double area_;
    double zp_;
    double min_;
    double max_;
    double centralWavelength_;
};

class Filters {
private:
    // private data members
    string folderPath_;    
    vector<string> fileList_;

    // private functions 
    void readFolder();
    void loadFilter(int);

public:
    // public data members 
    map<string,int> filterID_;
    map<int,string> filterName_;
    vector<FilterData> filters_;
    
    // constructor
    Filters(string);

    // public functions
    void rescale(double,double,double);   // start, end, step
    void rescale(const vector<double>&);  // wavelength range
    void rescale(double);         // step, start and end set automatically
    double flux(const vector<double>&, const string&);
};

#endif