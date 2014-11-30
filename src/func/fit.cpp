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

#include "fit.h"

using namespace std;


int resFunc(int m, int n, double *p, double *residual, double **dvec, void *vars) {
    class SNEvent *sn = (struct SNEvent *) vars;

    double t;
    sn->explosionMJD_ = p[n-1];
    for (int i = 0; i < n-1; ++i) {
        sn->snmodel_->modelParams_[i] = p[i];
    }

    sn->snmodel_->calcDerivedParams();
    for (int i = 0; i < sn->mjd_.size(); ++i) {
        if (sn->mjd_[i] >= sn->explosionMJD_) {
            t = sn->mjd_[i] - sn->explosionMJD_;
            residual[i] = (sn->flux_[i] - sn->snmodel_->flux(t, sn->filter_[i])) / sn->fluxErr_[i];

        } else {
            residual[i] = sn->flux_[i] / sn->fluxErr_[i];
        }
    }

    return 0;
}


void fit (shared_ptr<Workspace> &w) {
    shared_ptr<SNEvent> sn = w->snevent_;

    vector<double> par = w->params_;
    par.push_back(sn->explosionMJD_ - 10);
    int status;
    mp_result result;
    mp_config config;
    mp_par pars[par.size()];
    memset(&config, 0, sizeof(config));
    memset(&result, 0, sizeof(result));
    memset(&pars,0,sizeof(pars));
    vector<double> parErr(par.size());
    result.xerror = parErr.data();

    config.maxiter = 2000;
    status = mpfit(resFunc, sn->mjd_.size(), par.size(), par.data(), pars, &config, (void*) sn.get(), &result);

    for (int i = 0; i < par.size(); ++i) {
        cout << setw(10) << par[i] << setw(8) << " +/- " << setw(10) << parErr[i] << endl;
    }

    cout << setw(11) << "Chi^2 " << setw(17) << result.bestnorm <<  endl;
    cout << setw(11) << "RedChi^2 " << setw(17) << result.bestnorm / sn->mjd_.size() << endl;

    w->explosionMJD_ = par.back();
    par.pop_back();
    parErr.pop_back();
    w->fitParam_ = par;
    w->fitParamError_ = parErr;
}