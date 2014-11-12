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