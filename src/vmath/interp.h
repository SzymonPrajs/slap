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

#ifndef SLAP_VMATH_INTERP_H_
#define SLAP_VMATH_INTERP_H_

#include <vector>
#include <math.h>

using namespace std;

namespace vmath {  // NAMESPACE vmath

template <typename T>
T interp(T org_x, const vector<T> &temp_x, const vector<T> &temp_y, T low = 0, T up = 0) {
    T org_y = low;
    T m;
    T c;

    if (org_x < temp_x.front()) {
        return low;
    } else if (org_x > temp_x.back()) {
        return up;
    }

    T stepTry = temp_x[1] - temp_x[0];
    T indexTry = int((org_x - temp_x[0]) / stepTry);
    T sizeTry = int((temp_x.back() - temp_x.front()) / stepTry) + 1;

    if (sizeTry == temp_x.size()) {
        if (org_x == temp_x[indexTry]) {
            org_y = temp_y[indexTry];

        } else if (org_x > temp_x[indexTry] && org_x < temp_x[indexTry+1]) {
            m = (temp_y[indexTry+1] - temp_y[indexTry]) / (temp_x[indexTry+1] - temp_x[indexTry]);
            c = temp_y[indexTry] - m * temp_x[indexTry];
            org_y = m * org_x + c;

        } else {
            sizeTry--;
        }
    }

    /*
     *TODO - Choose a better search algorithm
     */
    if (sizeTry != temp_x.size()) {
        for (int i = 0 ; i < (temp_x.size() - 1) ; i++) {
            if (temp_x[i] <= org_x && temp_x[i+1] >= org_x) {
                m = (temp_y[i+1] - temp_y[i])/(temp_x[i+1] - temp_x[i]);
                c = temp_y[i] - m*temp_x[i];
                org_y = m*org_x + c;
                break;
            }
        }
    }

    return org_y;
}


template <typename T>
vector<T> interp(const vector<T> &org_x, const vector<T> &temp_x, const vector<T> &temp_y, T low = 0, T up = 0) {
    vector<T> org_y(org_x.size(), low);

    for (int i = 0; i < org_x.size(); ++i) {
        org_y[i] = interp<T>(org_x[i], temp_x, temp_y, low, up);
    }

    return org_y;
}

} // NAMESPACE vmath

#endif
