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
	int status = mkdir("plottmp", 0777);
	ofstream fhandle;
	if (status == 0) {
		fhandle.open("plottmp/info.dat");
		fhandle << "SNname=" << w->SNname_ << "\n";
		fhandle << "z=" << w->z_ << "\n";
		fhandle.close();
	}
}


void makeplot(shared_ptr<Workspace> &w) {
	
}


void plot(shared_ptr<Workspace> &w) {
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