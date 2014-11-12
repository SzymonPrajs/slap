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

#ifndef SLAP_CORE_SNEVENT_H_
#define SLAP_CORE_SNEVENT_H_

#include <memory>
#include "Cosmology.h"
#include "SNModel.h"
#include "Filters.h"
#include "../vmath/loadtxt.h"
#include "../vmath/convert.h"
#include "../vmath/stat.h"

using namespace std;
using namespace vmath;


class SNEvent {
private:
	vector<double> completeMJD_;
    vector<double> completeFlux_;
    vector<double> completeFluxErr_;
    vector<string> completeFilter_;

public:
    vector<double> mjd_;
    vector<double> flux_;    
    vector<double> fluxErr_;
    vector<string> filter_;
    double explosionMJD_;
    shared_ptr<SNModel> snmodel_;

    SNEvent(string, shared_ptr<SNModel>);
	void readData(string);
    void restoreCompleteLC();
};

#endif