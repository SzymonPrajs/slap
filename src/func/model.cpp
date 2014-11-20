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

#include "model.h"

using namespace std;


void modelLC (string model, double z, vector<double> &par, vector<string> &filterList) {
    shared_ptr<Filters> filters(new Filters("data/filters"));
    shared_ptr<Cosmology> cosmology(new Cosmology(z));
    shared_ptr<SNModel> snmodel;

    if (model == "BB4") {
        shared_ptr<BB4> bb4(new BB4(cosmology, filters));
        snmodel = bb4;

    } else if (model == "BB6") {
        shared_ptr<BB6> bb6(new BB6(cosmology, filters));
        snmodel = bb6;

    } else if (model == "Magnetar") {
        shared_ptr<Magnetar> magnetar(new Magnetar(cosmology, filters));
        snmodel = magnetar;

    } else {
        cout << "Model '" << model << "' was found.\nExiting..." << endl;
        /*TODO - send kill signal*/
    }

    snmodel->modelParam_ = par;
    snmodel->calcDerivedParams();

    for (int j = 0; j < filterList.size(); ++j) {
        for (int i = 0; i < 150; ++i) { 
            cout << i + 55653.6 << " " << snmodel->flux(i, filterList[j]) << " " << filterList[j] << endl;
        }
    }
}