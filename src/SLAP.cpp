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
#include "core/Workspace.h"
#include "models/BB4.h"
#include "models/BB6.h"
#include "models/Magnetar.h"
#include "func/fit.h"
#include "func/model.h"

using namespace std;

void getArgv(int argc, char **argv, vector<string> &options) {
    options.clear();

    for (int i = 1; i < argc; ++i) {
        options.push_back(argv[i]);
    }
}


void applyOptions(vector<string> &options, shared_ptr<Workspace> w) {
    vector<string> command;
    for (int i = 0; i < options.size(); ++i) {
        split(options[i], '=', command);
        
        if (command.size() > 2) {
            cout << "'" << command[0] << "' is not a valid command." << endl;
            continue;
        
        } else if (command.size() == 1) {
            if (w->functionList_.find(command[0]) == w->functionList_.end()) {
                cout << "'" << command[0] << "' is not a valid function." << endl;

            } else {
                w->currentFunction_ = command[0];
            }

        } else if (command[0] == "file") {
            cout << command[1] << endl;

        } else if (command[0] == "z") {
            w->z_ = atof(command[1].c_str());
        
        } else if (command[0] == "model") {
            w->model_ = command[1];
        
        } else if (command[0] == "filter") {
            cout << command[1] << endl;

        } else if (command[0] == "param") {
            cout << command[1] << endl;

        } else {
            cout << "'" << command[0] << "' is not a valid command." << endl;
        }
    }
}


int main(int argc, char *argv[]) {
    vector<string> options;
    getArgv(argc, argv, options);
    
    shared_ptr<Workspace> w(new Workspace());
    applyOptions(options, w);

    w->init();
 
    // string function = argv[1];
    // argc--;

    // if (function == "fit") {
    //     if (argc == 3) {
    //         string file = argv[2];
    //         double z = atof(argv[3]);
    //         string model = argv[4];


    //         fit(file, z, model);

    //     } else {
    //         cout << "incorrect number of parameter" << endl;
    //     } 

    // } else if (function == "model") {
    //     if (argc >= 5) {
    //         vector<double> par;
    //         vector<string> flt = {"u", "g", "r", "i", "z"};
    //         string model = argv[2];
    //         double z = atof(argv[3]);
    //         argc -= 2;
    //         int pos = 4;

    //         while (argc > 0) {
    //             par.push_back(atof(argv[pos]));
    //             argc--;
    //             pos++;
    //         }

    //         modelLC(model, z, par, flt);

    //     } else {
    //         cout << "incorrect number of parameter" << endl;
    //     }
    
    // } else {
    //     cout << "No function '" << function << "' found" << endl;
    // } 

    return 0;
}