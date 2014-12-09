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

#include "fit2.h"

using namespace std;
using namespace ROOT::Minuit2;

Residual::Residual(shared_ptr<Workspace> &w) {
    w_ = w;
    errorDef_ = 100.0;
}


double Residual::operator()(const std::vector<double>& par) const {
    assert(par.size() == (w_->snmodel_->noModelParams_ + 1));
    double chi2 = 0.0;
    double residual = 0.0;
    double t = 0.0;

    w_->snevent_->explosionMJD_ = par.back();
    vector<double> tpar = par;
    tpar.pop_back();
    w_->snmodel_->modelParams_ = tpar;
    w_->snmodel_->calcDerivedParams();

    for (int i = 0; i < w_->snevent_->mjd_.size(); ++i) {
        if (w_->snevent_->mjd_[i] >= w_->snevent_->explosionMJD_) {
            t = w_->snevent_->mjd_[i] - w_->snevent_->explosionMJD_;
            residual = (w_->snevent_->flux_[i] - w_->snmodel_->flux(t, w_->snevent_->filter_[i])) / w_->snevent_->fluxErr_[i];

        } else {
            residual = w_->snevent_->flux_[i] / w_->snevent_->fluxErr_[i];
        }

        chi2 += pow(residual, 2.0);
    }

    for(int i = 0; i < par.size(); ++i) {
        cout << par[i] << " ";
    }
    cout << chi2 << endl;

    return chi2;
}


void fit2(shared_ptr<Workspace> &w) {
    Residual res(w);

    // setting up fit parameters 
    MnUserParameters upar;
    for (int i = 0; i < w->params_.size(); ++i) {
        upar.Add(w->snmodel_->paramNames_[i], w->params_[i], w->params_[i]);
        upar.SetLowerLimit(w->snmodel_->paramNames_[i], 0.0001);
    }
    upar.Add("t0", w->explosionMJD_, 100.0);
    upar.SetLowerLimit("t0", 0.0001);

    // create SIMPLEX minimizer
    MnSimplex simplex(res, upar);
    FunctionMinimum min = simplex();

    /*TODO: Try again to copy over the residual finction from fit.cpp but this time copy the SNEvent class like in that code*/
    
    // MnMigrad migrad(res, upar);
    // FunctionMinimum min = migrad();

    for(int i = 0; i < (w->params_.size() + 1); ++i) {
        cout << min.UserState().Value(i) << " ";
    }
    cout << endl;
}