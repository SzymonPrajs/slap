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

#ifndef SLAP_FUNC_FIT_H_
#define SLAP_FUNC_FIT_H_

#include <memory>
#include <iomanip>
#include <cstring>
#include "../utils/mpfit.h"
#include "../core/SNEvent.h"

using namespace std;

int resFunc(int, int, double*, double*, double**, void*);
void fitLC(shared_ptr<SNEvent>, vector<double>&);

#endif