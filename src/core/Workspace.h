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
#include "../models/MagnetarK.h"
#include "../models/MagnetarR.h"
#include "../utils/utils.h"
#include "../vmath/stat.h"


using namespace std;


class Workspace {
public:
    /*Names*/
    string rawSNName_;
    string SNName_;
    string plotTitle_;

    /*Cosmology*/
    double z_;
    string cosmologyUsed_;

    /*Parameters*/
    double fitExplosionMJD_;
    double fitExplosionMJDError_;
    double fitChi_;
    double fitRedChi_;
    double MsdssU_;
    vector<double> params_;
    vector<double> fitParam_;
    vector<double> fitParamError_;
    vector<double> fitDerivedParam_;
    string rawParams_;
    string absLines_;


    /*Dates*/
    double startMJD_;
    double explosionMJD_;
    double endMJD_;
    double maxMJD_;
    string rawStartMJD_;
    string rawExplosionMJD_;
    string rawEndMJD_;

    /*Model*/
    string model_;
    
    /*Filter*/
    vector<string> filterList_;
    string filterFolder_;
    string rawFilters_;

    /*Directories*/
    string LC_;
    boost::filesystem::path currentDir_;
    boost::filesystem::path plotDir_;
    boost::filesystem::path LCPath_;
    
    /*Program state*/
    bool interactiveMode_;
    bool updateParam_;
    bool updateEvent_;
    int plotCount_;
    string currentFunction_;
    string plotType_;
    string fig_;
    string fitter_;


    /*Function list*/
    map<string,int> functionList_ = {{"interactive", 0}, 
                                     {"fit", 1}, 
                                     {"plot", 2}, 
                                     {"addplot", 3}, 
                                     {"makeplot", 4},
                                     {"clearplot", 5},
                                     {"analyse", 6},
                                     {"mag", 7},
                                     {"exit", 98}, 
                                     {"quit", 99}};

    /*Core classes*/
    shared_ptr<Cosmology> cosmology_;
    shared_ptr<Filters> filters_;
    shared_ptr<SNModel> snmodel_;
    shared_ptr<SNEvent> snevent_;
    

    /*Construtor*/
    Workspace();

    /*Update individual parameters*/
    void restoreDefault();
    void updatePaths();
    void updateCosmology();
    void updateFilters();
    void updateModel();
    void updateEvent();
    void updateDates();
    void updateMJDBounds();
    void updateMode();
    void updateRawParams();
    void updateRawFilters();
    void updateSNName();

    /*Perform a full update*/
    void update();
};

#endif