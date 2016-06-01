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

#ifndef SLAP_VMATH_STAT_H_
#define SLAP_VMATH_STAT_H_

#include <math.h>
#include <vector>

using namespace std;

namespace vmath {  // NAMESPACE vmath

template<typename T>
T max(const vector<T> &vec) {
    return *max_element(vec.begin(), vec.end());
}



template <typename T>
T sum(vector<T> &vec) {
    return accumulate(vec.begin(), vec.end(), 0.0);
}


template <typename T>
T mean(const vector<T> &vec) {
    return sum<T>(vec) / vec.size();
}


template<typename T>
T min(const vector<T> &vec) {
    return *min_element(vec.begin(), vec.end());
}


template<typename T>
T standardDiv(const vector<T> &vec) {
    T res = 0.0;
    T m = mean<T>(vec);

    for_each(vec.begin(), vec.end(), [&](const T d) {
        res += pow(d - m, 2);
    });

    return sqrt(res / (vec.size() - 1));
}


} //NAMESPACE vmath

#endif
