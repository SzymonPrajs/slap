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
		fhandle.open(w->plotDir_ + "/info.dat");
		fhandle << "name=" << w->SNName_ << "\n";
		fhandle << "z=" << w->z_ << "\n";
		fhandle.close();
	} else if (w->plotCount_ < 0) {
		cout << "Something went wrong! Cannot create a temporary folder" << endl;
	}

	if (boost::filesystem::exists(w->plotDir_ + "/info.dat")) {
		fhandle.open(w->plotDir_ + "/info.dat", ios::app);
		
		if (w->plotType_ == "data") {
			boost::filesystem::path s = w->currentDir_;
			boost::filesystem::path d = w->plotDir_;
			s.append(w->LCFile_);
			d.append("test.dat"); /*Make this a proper path and then tae the file name through Boost*/

			if (!boost::filesystem::exists(d)) {
				boost::filesystem::copy_file(s, d);
			}
			
			fhandle << w->plotCount_;
			fhandle << " type=data";
			fhandle << " filters=" << makeString<string>(w->filterList_, ',');
			fhandle << " file=" << (d.append("test.dat")).string();
			fhandle << "\n"; 
		}

		fhandle.close();
		w->plotCount_++;

	} else {
		cout << "Something went wrong! Cannot create a new plot" << endl;
	}
}


void makeplot(shared_ptr<Workspace> &w) {

}

void clearplot(shared_ptr<Workspace> &w) {
	boost::filesystem::remove_all(w->plotDir_);
}


void plotModel(shared_ptr<Workspace> &w) {
    w->snmodel_->modelParams_ = w->params_;
    w->snmodel_->calcDerivedParams();
    double t = 0;

    for (int j = 0; j < w->filterList_.size(); ++j) {
        for (double mjd = w->explosionMJD_; mjd < w->endMJD_; ++mjd) { 
            t = w->endMJD_ - w->explosionMJD_;
            cout << mjd << " " << w->snmodel_->flux(t, w->filterList_[j]) << " " << w->filterList_[j] << endl;
        }
    }
}