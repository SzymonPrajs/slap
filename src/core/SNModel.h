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

#ifndef SLAP_CORE_SNMODEL_H_
#define SLAP_CORE_SNMODEL_H_

#include <math.h>
#include <vector>
#include <memory>
#include "Cosmology.h"
#include "Filters.h"
#include "../vmath/range.h"
#include "../vmath/algebra.h"
#include "../vmath/loadtxt.h"
#include "../vmath/interp.h"


using namespace std;
using namespace vmath;

class SNModel {
protected:
    vector<double> restWavelength_;
    vector<double> obsWavelength_;
    vector<double> absLines_;

public:
    int noModelParams_;
    int noSEDParams_;
    vector<double> SEDParams_;
    vector<double> modelParams_;
    vector<double> defaultParams_;
    vector<double> derivedParams_;
    vector<string> paramNames_;
    string absLineFile_;
    
    shared_ptr<Cosmology> cosmology_;
    shared_ptr<Filters> filters_;
    
    SNModel(shared_ptr<Cosmology> cosmology, shared_ptr<Filters>);    
    SNModel(shared_ptr<Filters> filters);
    virtual vector<double> calcSED(double) = 0;
    virtual void calcSEDParams(double) = 0;
    virtual void calcDerivedParams() {};
    virtual void printDerivedVariables() {};
    
    double flux(double, string);
    void setWavelength();
    void absFilter();
};

#endif
