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

#ifndef SLAP_VMATH_CONVERT_H_
#define SLAP_VMATH_CONVERT_H_

#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace vmath {  // NAMESPACE vmath

template <typename T>
void castString(const vector<string> &s, vector<T> &res) {
    res.resize(s.size());
    T x;
    for (int i = 0; i < s.size(); ++i) {
        istringstream ss(s[i]);
        if (!(ss >> x))
            res[i] = -99;
        res[i] = x;
    }
} 


template <typename T>
vector<T> castString(const vector<string> &s) {
    vector<T> res;
    castString<T>(s, res);
	return res;
} 


template <typename T>
void transpose(const vector< vector<T> > &data, vector< vector<T> > &cols) {
    int x = data.size();
    int y = data[0].size();
    cols.resize(y);
    for (int i = 0; i < y; ++i) {
        cols[i].resize(x);
    }

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            cols[j][i] = data[i][j];
        }
    }
}


template <typename T>
vector< vector<T> > transpose(const vector< vector<T> > &data) {
    vector< vector<T> > cols;
    transpose<T>(data, cols);
    return cols;
}


}  // NAMESPACE vmath


#endif