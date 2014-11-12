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

#ifndef SLAP_VMATH_INTERP_H_
#define SLAP_VMATH_INTERP_H_

#include <vector>
#include <math.h>

using namespace std;

namespace vmath {  // NAMESPACE vmath

template <typename T>
T interp(T org_x, const vector<T> &temp_x, const vector<T> &temp_y) {
    T org_y = 0;
    T m, c;
    
    for (int i = 0 ; i < (temp_x.size() - 1) ; i++) {
        if (temp_x[i] <= org_x && temp_x[i+1] >= org_x) {
            m = (temp_y[i+1] - temp_y[i])/(temp_x[i+1] - temp_x[i]);
            c = temp_y[i] - m*temp_x[i];
            org_y = m*org_x + c;
            break;
        }
    }
    return org_y;
}


template <typename T>
vector<T> interp(const vector<T> &org_x, const vector<T> &temp_x, const vector<T> &temp_y) {
    vector<T> org_y(org_x.size(),0);
    T m, c;
    
    for (int j = 0; j < org_x.size(); ++j) {
        for (int i = 0; i < (temp_x.size() - 1); i++) {
            if (temp_x[i] <= org_x[j] && temp_x[i+1] >= org_x[j]) {
                m = (temp_y[i+1] - temp_y[i])/(temp_x[i+1] - temp_x[i]);
                c = temp_y[i] - m*temp_x[i];
                org_y[j] = m*org_x[j] + c;
                break;
            }
        }
    }
    return org_y;
} 

} // NAMESPACE vmath

#endif