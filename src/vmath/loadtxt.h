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

#ifndef SLAP_VMATH_LOADTXT_H_
#define SLAP_VMATH_LOADTXT_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace vmath {  // NAMESPACE vmath

template <typename T>
void loadtxt(const string &fileName, int l, vector< vector<T> > &data) {
    data.resize(l);
    for (int i = 0; i < l; ++i) {
        data[i].clear();
    }

    ifstream file;
    file.open(fileName);
    T temp;
    string s;

    while (getline(file, s)) {
        istringstream iss(s);

        for (int i = 0; i < l; ++i) {
            iss >> temp;
            data[i].push_back(temp);
        }
    }

    file.close();
}


template <typename T>
vector< vector<T> > loadtxt(const string &fileName, int l) {
    vector< vector<T> > data;
    loadtxt<T>(fileName, l, data);
    return data;
}


} //NAMESPACE vmath

#endif
