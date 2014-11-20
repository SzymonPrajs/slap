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
        sn->snmodel_->modelParam_[i] = p[i];
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


void fit (string file, double z, string model) {
    shared_ptr<Filters> filters(new Filters("data/filters"));
    shared_ptr<Cosmology> cosmology(new Cosmology(z));
    shared_ptr<SNModel> snmodel;
    vector<double> par;

    if (model == "BB4") {
        shared_ptr<BB4> bb4(new BB4(cosmology, filters));
        snmodel = bb4;
        par = {1.0, 10000, -100, 0};

    } else if (model == "BB6") {
        shared_ptr<BB6> bb6(new BB6(cosmology, filters));
        snmodel = bb6;
        par = {1.0, 0.1, 10000, -100, 10, 0};

    } else if (model == "Magnetar") {
        shared_ptr<Magnetar> magnetar(new Magnetar(cosmology, filters));
        snmodel = magnetar;
        par = {30.0, 7.0, 2.0, 20, 0};

    } else {
        cout << "Model '" << model << "' was found.\nExiting..." << endl;
        /*TODO - send kill signal*/
    }

    shared_ptr<SNEvent> sn(new SNEvent(file, snmodel));
    par.back() = sn->explosionMJD_ - 10;
    
    fitLC(sn, par);
    sn->snmodel_->printDerivedVariables();
}


void fitLC(shared_ptr<SNEvent> sn, vector<double> &par) {
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
}