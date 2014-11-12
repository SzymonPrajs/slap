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

#ifndef SLAP_VMATH_ALGEBRA_H_
#define SLAP_VMATH_ALGEBRA_H_

#include <vector>
#include <math.h>

using namespace std;

namespace vmath {  // NAMESPACE vmath

template <typename T>
void add(const vector<T> &vec, T scalar, vector<T> &res) {
    res.resize(vec.size());

    for (int i = 0; i < vec.size(); ++i) {
        res[i] = vec[i] + scalar;
    }
}


template <typename T>
vector<T> add(vector<T> &vec, T scalar) {
    vector<T> res;
    add<T>(vec, scalar, res); 
    return res;
}


template <typename T>
void add(const vector<T> &vec1, const vector<T> &vec2, vector<T> &res) {
    if (vec1.size() <= vec2.size()) {
        res.resize(vec1.size());
    } else {
        res.resize(vec2.size());
    }

    for (int i = 0; i < res.size(); ++i) {
        res[i] = vec1[i] + vec2[i];
    }
}


template <typename T>
vector<T> add(const vector<T> &vec1, const vector<T> &vec2) {
    vector<T> res;
    add<T>(vec1, vec2, res);
    return res;
}


template <typename T>
void mult(const vector<T> &vec, T scalar, vector<T> &res) {
    res.resize(vec.size());

    for (int i = 0; i < vec.size(); ++i) {
        res[i] = vec[i]*scalar;
    }
}


template <typename T>
vector<T> mult(const vector<T> &vec, T scalar) {
    vector<T> res;
    mult<T>(vec, scalar, res);
    return res;
}


template <typename T>
void mult(const vector<T> &vec1, const vector<T> &vec2, vector<T> &res) {
    if (vec1.size() <= vec2.size()) {
        res.resize(vec1.size());
    } else {
        res.resize(vec2.size());
    }

    for (int i = 0; i < res.size(); ++i) {
        res[i] = vec1[i]*vec2[i];
    }
}


template <typename T>
vector<T> mult(const vector<T> &vec1, const vector<T> &vec2) {
    vector<T> res;
    mult<T>(vec1, vec2, res);
    return res;
}


} // NAMESPACE vmath

#endif