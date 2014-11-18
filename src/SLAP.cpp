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

#include <memory>
#include "utils/utils.h"
#include "core/Cosmology.h"
#include "core/Filters.h"
#include "core/SNModel.h"
#include "core/SNEvent.h"
#include "models/BB4.h"
#include "models/BB6.h"
#include "models/Magnetar.h"
#include "func/fit.h"

using namespace std;


int main(int argc, char *argv[]) {  
	string file = argv[1];
	double z = atof(argv[2]);
    string model = argv[3];

    shared_ptr<Filters> filters(new Filters("data/filters"));
    shared_ptr<Cosmology> cosmology(new Cosmology(z));
    shared_ptr<SNModel> snmodel;
    vector<double> par;

    if (model == "BB4") {
        shared_ptr<BB4> bb4(new BB4(cosmology, filters));
        snmodel = bb4;
        par = {1.0, 10000, -100, 55170};
    } else if (model == "BB6") {
        shared_ptr<BB6> bb6(new BB6(cosmology, filters));
        snmodel = bb6;
        par = {1.0, 0.1, 10000, -100, 10, 0};
    } else if (model == "Magnetar") {
        shared_ptr<Magnetar> magnetar(new Magnetar(cosmology, filters));
        snmodel = magnetar;
        par = {30.0, 7.0, 2.0, 55170};
    } else {
        cout << "No other model implemented yet. Using BB4 instead ..." << endl;
        shared_ptr<BB4> bb4(new BB4(cosmology, filters));
        snmodel = bb4;
        par = {1.0, 10000, -100, 55170};
    }

    shared_ptr<SNEvent> sn(new SNEvent(file, snmodel));
    
    // par.back() = sn->explosionmJD_;     
    fitLC(sn, par);
    
    return 0;
}