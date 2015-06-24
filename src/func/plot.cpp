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

#include "plot.h"

using namespace std;


void addplot(shared_ptr<Workspace> &w) {
	ofstream fhandle;

	if (w->plotCount_ == 0) {
        mkdir((w->plotDir_ + w->SNName_).c_str(), 0755);
		fhandle.open(w->plotDir_ + w->SNName_ + "/info.dat");
		fhandle << "name=" << w->SNName_ << "\n";
		fhandle << "z=" << w->z_ << "\n";
        fhandle << "fig=" << w->fig_ << "\n";
		fhandle.close();

	} else if (w->plotCount_ < 0) {
		cout << "Something went wrong! Cannot create a temporary folder" << endl;
	}

    if (access((w->plotDir_ + w->SNName_ + "/info.dat").c_str(), R_OK) != -1) {
		fhandle.open(w->plotDir_ + w->SNName_ + "/info.dat", ios::app);
		
		if (w->plotType_ == "data") {
			string dest = w->plotDir_ + w->SNName_ + "/" + to_string(w->plotCount_) + ".dat";

			if (access(w->LC_.c_str(), R_OK) != -1) {
				copyFile(w->LC_, dest);
			}
		
		} else if (w->plotType_ == "model") {
			plotModel(w);
		
        } else if (w->plotType_ == "param") {
            plotSEDParam(w);

        } else if (w->plotType_ == "SED") {
            plotSED(w);
        
        } else if (w->plotType_ == "residual") {
            plotResidual(w);
        }

		fhandle << w->plotCount_;
		fhandle << " type=" + w->plotType_;
		fhandle << " filters=" << joinStrings<string>(w->filterList_, ',');
		fhandle << " file=" << w->plotDir_ + w->SNName_ + "/" + to_string(w->plotCount_) + ".dat";
		fhandle << "\n";

		fhandle.close();
		w->plotCount_++;

	} else {
		cout << "Something went wrong! Cannot create a new plot" << endl;
	}
}


void makeplot(shared_ptr<Workspace> &w) {
    if (w->plotType_ == "model" || w->plotType_ == "data") {
        string command = "python ";
        command += DATA;
        command += "/scripts/plotLC.py ";
        command += w->plotDir_ + w->SNName_;
    	system(command.c_str());
    
    } else {
        cout << "This will be implemented soon!" << endl;
    }
}


void clearplot(shared_ptr<Workspace> &w) {
    /*TODO: This need to be done in a better way*/
    struct stat s;
    if (stat((w->plotDir_ + w->SNName_).c_str(), &s) == 0 && S_ISDIR(s.st_mode)) {
	   system(("rm -r " + w->plotDir_ + w->SNName_).c_str());
    }
	w->plotCount_ = 0;
}


void plotModel(shared_ptr<Workspace> &w) {
    w->snmodel_->modelParams_ = w->params_;
    w->snmodel_->calcDerivedParams();
    double t = 0;

	ofstream plotFile;
	plotFile.open(w->plotDir_ + w->SNName_ + "/" + to_string(w->plotCount_) + ".dat");

    for (int j = 0; j < w->filterList_.size(); ++j) {
        for (double mjd = w->explosionMJD_ + 1; mjd < w->endMJD_ + 50; ++mjd) { 
            t = mjd - w->explosionMJD_;

            plotFile << mjd << " " << w->snmodel_->flux(t, w->filterList_[j]) << " " << w->filterList_[j] << "\n";
        }
    }

    plotFile.close();
}


void plotSEDParam(shared_ptr<Workspace> &w) {
    w->snmodel_->modelParams_ = w->params_;
    w->snmodel_->calcDerivedParams(); 

    double t = 0;

    ofstream plotFile;
    plotFile.open(w->plotDir_ + w->SNName_ + "/" + to_string(w->plotCount_) + ".dat");

    for (double mjd = w->explosionMJD_ + 1; mjd < w->endMJD_; ++mjd) { 
        t = mjd - w->explosionMJD_;
        w->snmodel_->calcSEDParams(t);
        plotFile << mjd << " " << w->snmodel_->SEDParams_[0] << " " << w->snmodel_->SEDParams_[1] << "\n";
    }

    plotFile.close();
}

void plotSED(shared_ptr<Workspace> &w) {
    // w->snmodel_->modelParams_ = w->params_;
    // w->snmodel_->calcDerivedParams();
    // double t, phase;

    // ofstream plotFile;
    // plotFile.open(w->plotDir_ + w->SNName_ + "/" + to_string(w->plotCount_) + ".txt");

    // vector<double> sed;

    // for (int j = 1; j < 120; ++j) {
    //     t = j * (1 + w->z_);
    //     sed = w->snmodel_->SED(t, "SDSS_g"); /*TODO: NEEDS TO TAKE WAVELENGTH AS INPUT*/
    //     phase = t - 22;

    //     for (int i = 0; i < sed.size(); ++i) {
    //        // TODO: Deprecated when above is implemented
    //         if (w->filters_->masterWavelength_[i] > 500 && w->filters_->masterWavelength_[i] < 13000) {
    //             plotFile << phase << " " << w->filters_->masterWavelength_[i] << " " << sed[i] << "\n";
    //             cout << w->filters_->masterWavelength_[i] << " " << sed[i] << endl;
    //         }
    //     }
    //     cout << phase << " " << w->snmodel_->mag(t, "SDSS_u") - w->snmodel_->mag(22, "SDSS_u") << endl;
    // }

    // plotFile.close();
}

void plotResidual(shared_ptr<Workspace> &w) {
    w->snmodel_->modelParams_ = w->params_;
    w->snmodel_->calcDerivedParams(); 

    double t = 0;

    ofstream plotFile;
    plotFile.open(w->plotDir_ + w->SNName_ + "/" + to_string(w->plotCount_) + ".dat");

    for (int i = 0; i < w->snevent_->mjd_.size(); ++i) { 
        t = w->snevent_->mjd_[i] - w->explosionMJD_;
        w->snmodel_->calcSEDParams(t);

        plotFile << w->snevent_->mjd_[i] << " " << w->snmodel_->flux(t, w->snevent_->filter_[i]) - w->snevent_->flux_[i] << " " << w->snevent_->fluxErr_[i] << " " << w->snevent_->filter_[i] << "\n";
    }

    plotFile.close();
}

