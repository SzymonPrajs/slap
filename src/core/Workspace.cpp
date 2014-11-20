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


void Workspace::restoreDefault() {
    z_ = 0;
    filterFolder_ = "data/filters";
    model_ = "BB4";

    interactiveMode = false;
}


void Workspace::initCosmology() {
    shared_ptr<Cosmology> cosmology(new Cosmology(z_));
    cosmology_ = cosmology;
}


void Workspace::initFilters() {
    shared_ptr<Filters> filters(new Filters(filterFolder_));
    filters_ = filters;
}


void Workspace::initModel() {
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
}


void Workspace::init() {
    initCosmology();
    initFilters();
    initModel();
}

