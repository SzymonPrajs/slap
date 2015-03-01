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

#include "analyse.h"

using namespace std;

void maxMag(shared_ptr<Workspace> &w) {
    w->snmodel_->modelParams_ = w->params_;
    w->snmodel_->calcDerivedParams();
    
    double z = w->snmodel_->cosmology_->z_;
    w->snmodel_->cosmology_->set(0.0);
    w->snmodel_->setWavelength();

    vector<double> flux(150, 0);
    double tMax = 0;
    for (int t = 1; t < flux.size(); ++t) { 
		flux[t] = w->snmodel_->flux(t, "SDSS_u");
    }

    tMax = distance(flux.begin(), max_element(flux.begin(), flux.end()));
    double fluxTemp = 0;
    double fluxMax = 0;
    double peak;
    for (double t = (tMax - 1.0); t < (tMax + 1.0); t += 0.01) { 
		fluxTemp = w->snmodel_->flux(t, "SDSS_u");
		if (fluxTemp > fluxMax) {
			fluxMax = fluxTemp;
			peak = t;
		} 
    }

    w->maxMJD_ = w->explosionMJD_ + peak * (1 + z);
    w->MsdssU_ = w->snmodel_->mag(peak, "SDSS_u");

    w->snmodel_->cosmology_->set(z);
    w->snmodel_->setWavelength();
}

void display(shared_ptr<Workspace> &w) {
    for (int i = 0 ; i < w->snmodel_->noModelParams_; ++i) {
        cout << w->snmodel_->paramNames_[i] << ": " << w->fitParam_[i] << "  (" << w->fitParamError_[i] << ")\n";
    }
    cout << "Name: " << w->SNName_ << "\n";
    cout << "explosion MJD: " << w->fitExplosionMJD_ << "  (" << w->fitExplosionMJDError_ << ")\n";
    cout << "peak MJD (observer): " << w->maxMJD_ << "\n";
    cout << "rise time (rest): " << (w->maxMJD_ - w->explosionMJD_) / (1 + w->cosmology_->z_) << "\n";
    cout << "M_u Mag at max: " << w->MsdssU_ << endl;
    cout << endl;

    ofstream fitFile;
    fitFile.open(w->currentDir_.string() +  "/results/"+ w->SNName_ + "/fit.txt");

    fitFile << w->SNName_ << " " << w->cosmology_->z_ << " " << w->MsdssU_ << " " << w->maxMJD_ << " " << (w->maxMJD_ - w->explosionMJD_) / (1 + w->cosmology_->z_) << " ";
    for (int i = 0 ; i < w->snmodel_->noModelParams_; ++i) {
        fitFile << w->fitParam_[i] << " " << w->fitParamError_[i] << " ";
    }
    fitFile << w->fitExplosionMJD_ << " " << w->fitExplosionMJDError_ << " ";
    fitFile << w->fitChi_ << " " << w->fitRedChi_ << "\n";

    fitFile.close();
}
