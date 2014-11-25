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

#include "Workspace.h"

using namespace std;


Workspace::Workspace() {
    restoreDefault();
}


/*
 *Set defult parameters for the program.
 *This is mainly to allow for initialisation to occur with no crashes
 */
void Workspace::restoreDefault() {
    SNname_ = "06D4eu";
    filterFolder_ = "data/filters";
    LCFile_ = "data/sample/06D4eu.dat";
    z_ = 1.588;
    model_ = "BB4";
    rawParam_ = "1.7,17000,-200";

    /*Set interactive mode as the default behaviour*/
    currentFunction_ = "quit";
    interactiveMode_ = false;
    updateParam_ = true;
}


void Workspace::updateCosmology() {
    shared_ptr<Cosmology> cosmology(new Cosmology(z_));
    cosmology_ = cosmology;
}


void Workspace::updateFilters() {
    shared_ptr<Filters> filters(new Filters(filterFolder_));
    filters_ = filters;
}


void Workspace::updateModel() {
    if (model_ == "BB4") {
        shared_ptr<BB4> bb4(new BB4(cosmology_, filters_));
        snmodel_ = bb4;

    } else if (model_ == "BB6") {
        shared_ptr<BB6> bb6(new BB6(cosmology_, filters_));
        snmodel_ = bb6;

    } else if (model_ == "Magnetar") {
        shared_ptr<Magnetar> magnetar(new Magnetar(cosmology_, filters_));
        snmodel_ = magnetar;

    } else {
        cout << model_ << " is not a recognised model. Restoring default (BB4)" << endl;
        shared_ptr<BB4> bb4(new BB4(cosmology_, filters_));
        snmodel_ = bb4;
    }
    if (updateParam_ == true) {
        params_ = snmodel_->defaultParams_;
        updateParam_ = false;
    }
}


void Workspace::updateEvent() {
    shared_ptr<SNEvent> snevent(new SNEvent(LCFile_, snmodel_));
    snevent_ = snevent;
}


void Workspace::updateMode() {
    if (interactiveMode_ == false) {
        if (currentFunction_ == "interactive") {
            interactiveMode_ = true;
        }
    }
}


void Workspace::updateRawParams() {
    vector<string> params;

    if (rawParam_ != "0") {
        split(rawParam_, ',', params);

        /*if the params are not correct set to default for the model*/
        if (params.size() != snmodel_->noModelParams_) {
            params_ = snmodel_->defaultParams_;
        
        } else {
            for (int i = 0; i < params.size(); ++i) {
                params_[i] = atof(params[i].c_str());
                
            }
        }

        /*Once the parameters have been dealt with set to 0 to ignore in the future*/
        rawParam_ = "0";
    }
}


void Workspace::updateRawFilters() {
    filterList_ = snevent_->filterList_;
}


void Workspace::update() {    
    updateCosmology();
    updateFilters();
    updateModel();
    updateEvent();
    updateMode();
    updateRawParams();
    updateRawFilters();
}

