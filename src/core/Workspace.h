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

#ifndef SLAP_CORE_WORKSPACE_H_
#define SLAP_CORE_WORKSPACE_H_

#include <memory>
#include <string>
#include "Cosmology.h"
#include "SNModel.h"
#include "Filters.h"
#include "SNEvent.h"
#include "../models/BB4.h"
#include "../models/BB6.h"
#include "../models/Magnetar.h"
#include "../utils/utils.h"

using namespace std;


class Workspace {
public:
    /*SN and Model properties*/
    double z_;
    vector<double> params_;
    vector<double> fitParam_;
    vector<double> fitParamError_;
    double explosionMJD_;
    string model_;

    /*LC and Filter*/
    string filterFolder_;
    string LCFile_;

    /*Program state*/
    bool interactiveMode_;
    string rawParam_;
    string rawFilter_;
    string currentFunction_;
    map<string,int> functionList_ = {{"interactive", 0}, 
                                     {"fit", 1}, 
                                     {"plot", 2}, 
                                     {"addplot", 3}, 
                                     {"makeplot", 4}, 
                                     {"quit", 99}};

    /*Core classes*/
    shared_ptr<Cosmology> cosmology_;
    shared_ptr<Filters> filters_;
    shared_ptr<SNModel> snmodel_;
    shared_ptr<SNEvent> snevent_;
    
    /*Construtor*/
    Workspace();

    /*Functions*/
    void restoreDefault();
    void initCosmology();
    void initFilters();
    void initModel();
    void initEvent();
    void initMode();
    void initRawParams();
    void init();
};

#endif