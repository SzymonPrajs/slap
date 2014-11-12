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

#ifndef SLAP_MODEL_BB4_H_
#define SLAP_MODEL_BB4_H_

#include <vector>
#include <memory>
#include <math.h>
#include "../core/Cosmology.h"
#include "../core/Filters.h"
#include "../core/SNModel.h"

using namespace std;
using namespace vmath;

class BB4 : public SNModel {
public:
    BB4(shared_ptr<Cosmology> cosmology, shared_ptr<Filters>);

    vector<double> calcSED(double);
    void calcSEDParams(double);

};

#endif