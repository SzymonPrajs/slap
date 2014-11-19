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
#include "func/model.h"

using namespace std;


int main(int argc, char *argv[]) {  
    argc--;

    string function = argv[1];
    argc--;

    if (function == "fit") {
        if (argc == 3) {
            string file = argv[2];
            double z = atof(argv[3]);
            string model = argv[4];


            fit(file, z, model);

        } else {
            cout << "incorrect number of parameter" << endl;
        } 

    } else if (function == "model") {
        if (argc >= 5) {
            vector<double> par;
            vector<string> flt = {"g"};
            string model = argv[2];
            double z = atof(argv[3]);
            argc -= 2;
            int pos = 4;

            while (argc > 0) {
                par.push_back(atof(argv[pos]));
                argc--;
                pos++;
            }

            modelLC(model, z, par, flt);

        } else {
            cout << "incorrect number of parameter" << endl;
        }
    
    } else {
        cout << "No fucntion '" << function << "' found" << endl;
    } 

    return 0;
}