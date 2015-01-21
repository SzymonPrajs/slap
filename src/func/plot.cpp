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
		boost::filesystem::create_directories(w->plotDir_);
		fhandle.open(w->plotDir_.string() + "/info.dat");
		fhandle << "name=" << w->SNName_ << "\n";
		fhandle << "z=" << w->z_ << "\n";
        fhandle << "fig=" << w->fig_ << "\n";
		fhandle.close();

	} else if (w->plotCount_ < 0) {
		cout << "Something went wrong! Cannot create a temporary folder" << endl;
	}

	if (boost::filesystem::exists(w->plotDir_.string() + "/info.dat")) {
		fhandle.open(w->plotDir_.string() + "/info.dat", ios::app);
		
		if (w->plotType_ == "data") {
			boost::filesystem::path s = w->LC_;
			boost::filesystem::path d = w->plotDir_.string() + "/" + to_string(w->plotCount_) + ".dat";

			if (!boost::filesystem::exists(d)) {
				boost::filesystem::copy_file(s, d);
			}
		
		} else if (w->plotType_ == "model") {
			plotModel(w);
		
        } else if (w->plotType_ == "param") {
            plotSEDParam(w);
        
        } else if (w->plotType_ == "residual") {
            plotResidual(w);
        }

		fhandle << w->plotCount_;
		fhandle << " type=" + w->plotType_;
		fhandle << " filters=" << joinStrings<string>(w->filterList_, ',');
		fhandle << " file=" << w->plotDir_.string() + "/" + to_string(w->plotCount_) + ".dat";
		fhandle << "\n";

		fhandle.close();
		w->plotCount_++;

	} else {
		cout << "Something went wrong! Cannot create a new plot" << endl;
	}
}


void makeplot(shared_ptr<Workspace> &w) {
	system("python scripts/SLAPPlot.py /Users/szymon/Projects/slap/plot");
}


void clearplot(shared_ptr<Workspace> &w) {
	boost::filesystem::remove_all(w->plotDir_);
	w->plotCount_ = 0;
}


void plotModel(shared_ptr<Workspace> &w) {
    w->snmodel_->modelParams_ = w->params_;
    w->snmodel_->calcDerivedParams();
    double t = 0;

	ofstream plotFile;
	plotFile.open(w->plotDir_.string() + "/" + to_string(w->plotCount_) + ".dat");

    for (int j = 0; j < w->filterList_.size(); ++j) {
        for (double mjd = w->explosionMJD_ + 1; mjd < w->endMJD_; ++mjd) { 
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

    // ofstream plotFile;
    // plotFile.open(w->plotDir_.string() + "/" + to_string(w->plotCount_) + ".dat");

    for (double mjd = w->explosionMJD_ + 1; mjd < w->endMJD_; ++mjd) { 
        t = mjd - w->explosionMJD_;

        w->snmodel_->calcSEDParams(t);
        cout << w->snmodel_->SEDParams_[0] << " " << w->snmodel_->SEDParams_[1] << endl;

        // plotFile << mjd << " " << w->snmodel_->SEDParams_[0] << " " << w->snmodel_->SEDParams_[1] << "\n";
    }

    // plotFile.close();
}


void plotResidual(shared_ptr<Workspace> &w) {
    w->snmodel_->modelParams_ = w->params_;
    w->snmodel_->calcDerivedParams(); 

    double t = 0;

    ofstream plotFile;
    plotFile.open(w->plotDir_.string() + "/" + to_string(w->plotCount_) + ".dat");

    for (int i = 0; i < w->snevent_->mjd_.size(); ++i) { 
        t = w->snevent_->mjd_[i] - w->explosionMJD_;
        w->snmodel_->calcSEDParams(t);

        plotFile << w->snevent_->mjd_[i] << " " << w->snmodel_->flux(t, w->snevent_->filter_[i]) - w->snevent_->flux_[i] << " " << w->snevent_->fluxErr_[i] << " " << w->snevent_->filter_[i] << "\n";
    }

    plotFile.close();
}

