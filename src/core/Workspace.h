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
#include <boost/filesystem.hpp>
#include "../models/BB4.h"
#include "../models/BB6.h"
#include "../models/Magnetar.h"
#include "../models/MagnetarR.h"
#include "../utils/utils.h"
#include "../vmath/stat.h"


using namespace std;


class Workspace {
public:
    /*SN and Model properties*/
    string SNName_;
    double z_;
    vector<double> params_;
    vector<double> fitParam_;
    vector<double> fitParamError_;
    double startMJD_;
    double explosionMJD_;
    double endMJD_;
    string model_;
    vector<string> filterList_;

    /*LC and Filter*/
    string filterFolder_;
    string LCFile_;
    string plotDir_;

    /*Program state*/
    boost::filesystem::path currentDir_;
    // boost::filesystem::path plotDir_;
    boost::filesystem::path LCPath_;
    bool interactiveMode_;
    bool updateParam_;
    bool updateEvent_;
    string rawParams_;
    string rawFilters_;
    string currentFunction_;
    int plotCount_;
    string plotType_;
    map<string,int> functionList_ = {{"interactive", 0}, 
                                     {"fit", 1}, 
                                     {"plot", 2}, 
                                     {"addplot", 3}, 
                                     {"makeplot", 4},
                                     {"clearplot", 5},
                                     {"exit", 98}, 
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
    void updatePaths();
    void updateCosmology();
    void updateFilters();
    void updateModel();
    void updateEvent();
    void updateMJDBounds();
    void updateMode();
    void updateRawParams();
    void updateRawFilters();
    void update();
};

#endif