/*SLAP - Supernova Lightcurve Analysis Package
 *
 *Copyright (C) 2014-2016  Szymon Prajs
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

#ifndef SLAP_VMATH_RANGE_H_
#define SLAP_VMATH_RANGE_H_

#include <vector>
#include <math.h>

using namespace std;

namespace vmath {  // NAMESPACE vmath


template <typename T>
void range(T start, T end, T step, vector<T> &res) {
    res.clear();
    if (end < start) {
        step = -step;
    }
    for (T i = start; i < end; i+=step) {
        res.push_back(i);
    }
}


template <typename T>
vector<T> range(T start, T end, T step) {
    vector<T> res;
    range<T>(start, end, step, res) ;
    return res;
}


} // NAMESPACE vmath

#endif
