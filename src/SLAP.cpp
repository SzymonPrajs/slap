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
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdio.h>

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
#include "func/fit2.h"
#include "func/plot.h"

using namespace std;


void getArgv(int argc, char **argv, vector<string> &options) {
    options.clear();

    for (int i = 1; i < argc; ++i) {
        options.push_back(argv[i]);
    }
}


void applyOptions(vector<string> &options, shared_ptr<Workspace> w) {
    vector<string> command;
    w->currentFunction_ = "interactive";

    for (int i = 0; i < options.size(); ++i) {
        split(options[i], '=', command);

        if (command.size() > 2) {
            cout << "'" << command[0] << "' is not a valid command." << endl;
            continue;

        } else if (command.size() == 1) {
            if (w->functionList_.find(command[0]) == w->functionList_.end()) {
                cout << "'" << command[0] << "' is not a valid function." << endl;
                if (w->interactiveMode_ == true) {
                    w->currentFunction_ = "interactive";

                } else {
                    w->currentFunction_ = "quit";
                }

            } else {
                w->currentFunction_ = command[0];
            }

        } else if (command[0] == "LC" || command[0] == "lc" || command[0] == "file") {
            w->LC_ = command[1];

        } else if (command[0] == "filter" || command[0] == "filters") {
            w->rawFilters_ = command[1];

        } else if (command[0] == "model") {
            w->model_ = command[1];
            w->rawParams_ = "default";

        } else if (command[0] == "param" || command[0] == "params") {
            w->rawParams_ = command[1];

        } else if (command[0] == "z") {
            w->z_ = atof(command[1].c_str());

        } else if (command[0] == "ptype") {
            w->plotType_ = command[1];

        } else if (command[0] == "snname") {
            w->SNName_ = command[1];

        } else if (command[0] == "expMJD" || command[0] == "MJD") {
            w->rawExplosionMJD_ = command[1];

        } else if (command[0] == "abs") {
            w->absLines_ = command[1];

        } else {
            cout << "'" << command[0] << "' is not a valid command." << endl;
        }
    }
}


void runCommand(shared_ptr<Workspace> w) {
    if (w->currentFunction_ == "fit") {
        fit2(w);

    } else if (w->currentFunction_ == "plot") {
        plotModel(w);    

    } else if (w->currentFunction_ == "addplot") {
        addplot(w);

    } else if (w->currentFunction_ == "makeplot") {
        makeplot(w);

    } else if (w->currentFunction_ == "clearplot") {
        clearplot(w);

    } else if (w->currentFunction_ == "exit") {
        w->currentFunction_ = "quit";
    }
}


int main(int argc, char *argv[]) {
    vector<string> options;
    getArgv(argc, argv, options);
    
    shared_ptr<Workspace> w(new Workspace());
    applyOptions(options, w);

    w->update();

    if (w->interactiveMode_ == false) {
        runCommand(w);
    
    } else {
        string sInput;
        char* input, shell_prompt[1000];
        while (w->currentFunction_ != "quit") {
            snprintf(shell_prompt, sizeof(shell_prompt), "SLAP> ");
            input = readline(shell_prompt);

            if (!input) {
                break;
            }
             
            sInput = input;    
            if (!sInput.empty()) {
                add_history(input);
            }
     
            split(sInput, ' ', options);
            applyOptions(options, w);

            w->update();
            runCommand(w);

            free(input);
        }
    }

    clearplot(w);
    return 0;
}