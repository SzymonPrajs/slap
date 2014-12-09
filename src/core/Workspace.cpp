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
    /*Settings for a default SN*/
    LC_ = "data/sample/SN2010gx.dat";
    model_ = "Magnetar";
    rawParams_ = "default";
    rawFilters_ = "LC";
    rawStartMJD_ = "LC";
    rawExplosionMJD_ = "LC";
    rawEndMJD_ = "LC";
    z_ = 0.23;

    /*Default data locations*/
    filterFolder_ = "data/filters";
    plotDir_ = "/Users/szymon/Projects/slap/plot";
    plotType_ = "data";
    plotCount_ = 0;

    /*Default behaviour*/
    currentFunction_ = "quit";
    interactiveMode_ = false;
    /*TODO - Are these needed any more?*/
    updateParam_ = true;
    updateEvent_ = true;

    /*Default fit params*/
    fitParam_ = {};
    fitParamError_ = {};
    fitExplosionMJD_ = -1;
    fitExplosionMJDError_ = -1;
}


void Workspace::updatePaths() {
    currentDir_ = boost::filesystem::current_path();
    boost::filesystem::path path(currentDir_);
    path /= LC_;
    if (boost::filesystem::exists(LC_)) {
        LCPath_ = path;
    
    } else {
        cout << "Lightcurve file does not exist! Reverting to previous value." << endl;
    }
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

    } else if (model_ == "Magnetar" || model_ == "magnetar") {
        shared_ptr<Magnetar> magnetar(new Magnetar(cosmology_, filters_));
        snmodel_ = magnetar;

    } else if (model_ == "MagnetarK" || model_ == "magnetarK") {
        shared_ptr<MagnetarK> magnetarK(new MagnetarK(cosmology_, filters_));
        snmodel_ = magnetarK;

    } else {
        cout << model_ << " is not a recognised model. Restoring default (Magnetar)" << endl;
        shared_ptr<Magnetar> magnetar(new Magnetar(cosmology_, filters_));
        snmodel_ = magnetar;
    }
}


void Workspace::updateEvent() {
    shared_ptr<SNEvent> snevent(new SNEvent(LC_, snmodel_));
    snevent_ = snevent;
}


void Workspace::updateDates() {
    if (rawExplosionMJD_ == "previous") {
        /*No changes required*/

    } else if (rawExplosionMJD_ == "LC") {
        explosionMJD_ = min<double>(snevent_->mjd_);

    } else if (rawExplosionMJD_ == "fit") {
        if (fitExplosionMJD_ == -1) {
            cout << "No fits have been performed yet. Restoring default." << endl;
            explosionMJD_ = min<double>(snevent_->mjd_);

        } else {
            explosionMJD_ = fitExplosionMJD_;
        }

    } else {
        explosionMJD_ = atof(rawExplosionMJD_.c_str());
    }

    rawExplosionMJD_ = "previous";

    /*TODO: Implement the changes in start and end MJD - This includes resizing the LC*/
    startMJD_ = min<double>(snevent_->mjd_);
    endMJD_ = max<double>(snevent_->mjd_);
}


void Workspace::updateMode() {
    if (interactiveMode_ == false) {
        if (currentFunction_ == "interactive") {
            interactiveMode_ = true;
        }
    }
}


void Workspace::updateRawParams() {
    if (rawParams_ == "previous") {
        /*retain previous parameters with no changes*/

    } else if (rawParams_ == "default") {
        params_ = snmodel_->defaultParams_;

    } else if (rawParams_ == "fit") {
        if (fitParam_.size() == 0) {
            cout << "No fit parameters present. Restoring default." << endl;
            params_ = snmodel_->defaultParams_;
        
        } else {
            params_ = fitParam_;
        }

    } else {
        vector<string> params;
        split(rawParams_, ',', params);

        /*if the params are not correct set to default for the model*/
        if (params.size() != snmodel_->noModelParams_) {
            cout << "Wrong numbr of parameters. Restoring default." << endl;
            params_ = snmodel_->defaultParams_;
        
        } else {
            for (int i = 0; i < params.size(); ++i) {
                params_[i] = atof(params[i].c_str());
            }
        }
    }

    /*Once the parameters have been dealt with set to 0 to ignore in the future*/
    rawParams_ = "previous";
}


void Workspace::updateRawFilters() {
    if (rawFilters_ == "previous") {
        /*Retail previous settings*/

    } else if (rawFilters_ == "LC") {
        filterList_ = snevent_->filterList_;

    } else {
        filterList_ = split(rawFilters_, ',');
    }

    /*Once the filters have been dealt with set to 0 to ignore in the future*/
    rawFilters_ = "previous";
}


void Workspace::update() { 
    updatePaths();
    updateCosmology();
    updateFilters();
    updateModel();
    updateEvent();
    updateDates();
    updateMode();
    updateRawParams();
    updateRawFilters();
}

