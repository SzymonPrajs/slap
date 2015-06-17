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

#include "Absorption.h"

using namespace std;
using namespace vmath;


Absorption::Absorption(string path) : folderPath_(path) {
    readFolder();
}


void Absorption::readFolder() {
    vector<string> list = loadtxt<string>(folderPath_ + "/list.txt", 1)[0];

    vector<string> temp;
    for (int i = 0; i < list.size(); ++i) {
        fileList_.push_back(list[i]);
        split(list[i],'.',temp);

        absID_[temp[0]] = i;
        absName_[i] = temp[0];
        
        loadAbs(i);
    }
}


void Absorption::loadAbs(int ID) {
    AbsData absFilter;

    vector< vector<double> > data;
    string path = folderPath_+"/"+fileList_[ID];
    loadtxt<double>(path,2,data);

    absFilter.name_ = absName_[ID];
    absFilter.inputWavelength_ = data[0];
    absFilter.inputBandpass_ = data[1];
    
    abs_.push_back(absFilter);
}