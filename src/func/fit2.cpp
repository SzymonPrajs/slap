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
    errorDef_ = 1.0;
}


double Residual::operator()(const std::vector<double>& par) const {
    shared_ptr<SNEvent> sn = w_->snevent_ ;
    vector<double> residual(sn->mjd_.size());
    double t;
    double chi2 = 0;
    sn->explosionMJD_ = par.back();
    for (int i = 0; i < (par.size() - 1); ++i) {
        sn->snmodel_->modelParams_[i] = par[i];
    }

    sn->snmodel_->calcDerivedParams();
    for (int i = 0; i < sn->mjd_.size(); ++i) {
        if (sn->mjd_[i] > sn->explosionMJD_) {
            t = sn->mjd_[i] - sn->explosionMJD_;
            // if (t < 200) {
            residual[i] = (sn->flux_[i] - sn->snmodel_->flux(t, sn->filter_[i])) / sn->fluxErr_[i];
            // }

        } else {
            residual[i] = sn->flux_[i] / sn->fluxErr_[i];
        }

        chi2 += pow(residual[i], 2.0);
    }

    return chi2;

}


void fit2(shared_ptr<Workspace> &w) {
    Residual res(w);

    // setting up fit parameters 
    MnUserParameters upar;
    for (int i = 0; i < w->params_.size(); ++i) {
        upar.Add(w->snmodel_->paramNames_[i], w->params_[i], 0.1 * w->params_[i]);
        upar.SetLowerLimit(w->snmodel_->paramNames_[i], 0.01);
    }
    upar.Add("t0", w->explosionMJD_, 100.0);
    upar.SetLowerLimit("t0", 50000);

    // create SIMPLEX minimizer
    MnSimplex simplex(res, upar);
    FunctionMinimum min = simplex();
    
    // MnMigrad migrad(res, upar);
    // FunctionMinimum min = migrad();

    vector<double> par = min.UserState().Params();
    for(int i = 0; i < (w->params_.size() + 1); ++i) {
        cout << min.UserState().Value(i) << " ";
    }
    w->snmodel_->printDerivedVariables();
    cout << res(par) << " " << res(par) / (w->snevent_->mjd_.size() - par.size()) << endl;

    cout << 1 << endl;

    w->fitExplosionMJD_ = par.back();
    // w->fitExplosionMJDError_ = parErr.back();
    par.pop_back();
    // parErr.pop_back();
    w->fitParam_ = par;
    // w->fitParamError_ = parErr;
    cout << 2 << endl;
    w->fitChi_ = res(par);
    cout << 3 << endl;
    w->fitRedChi_ = res(par) / (w->snevent_->mjd_.size() - par.size());
    cout << 4 << endl;
}