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

#include "fit3.h"

using namespace std;
using namespace vmath;

double flatPrior(double r, double x1, double x2) {
    return x1 + r * (x2 - x1);
}


void LogLike(double *cube, int &ndim, int &npars, double &lnew, void *context) {
    class SNEvent *sn = (struct SNEvent *) context;

    double * p = new double[npars];
    for (int i = 0; i < (npars-1); ++i) {
        p[i] = flatPrior(cube[i], sn->snmodel_->lParams_[i], sn->snmodel_->uParams_[i]);
        cube[i] = p[i];
    }
    p[npars-1] = flatPrior(cube[npars-1], sn->mjd_.front() - 100.0, sn->mjd_.back());
    cube[npars-1] = p[npars-1];

    double t;
    double chi2 = 0;
    double residual;
    sn->explosionMJD_ = p[npars-1];
    for (int i = 0; i < npars-1; ++i) {
        sn->snmodel_->modelParams_[i] = p[i];
    }

    sn->snmodel_->calcDerivedParams();
    for (int i = 0; i < sn->mjd_.size(); ++i) {
        if (sn->mjd_[i] >= sn->explosionMJD_) {
            t = sn->mjd_[i] - sn->explosionMJD_;
            residual = (sn->flux_[i] - sn->snmodel_->flux(t, sn->filter_[i])) / sn->fluxErr_[i];

        } else {
            residual = sn->flux_[i] / sn->fluxErr_[i];
        }
        chi2 += powf(residual, 2.0);
    }

    lnew = -2.0 * chi2;
    delete[] p;
}


void dumper(int &nSamples, int &nlive, int &nPar, double **physLive, double **posterior, double **paramConstr, double &maxLogLike, double &logZ, double &INSlogZ, double &logZerr, void *context)
{
    // convert the 2D Fortran arrays to C++ arrays
    
    // the posterior distribution
    // postdist will have nPar parameters in the first nPar columns & loglike value & the posterior probability in the last two columns
    int i, j;
    double postdist[nSamples][nPar + 2];
    for( i = 0; i < nPar + 2; i++ )
        for( j = 0; j < nSamples; j++ )
            postdist[j][i] = posterior[0][i * nSamples + j];
    
    // last set of live points
    // pLivePts will have nPar parameters in the first nPar columns & loglike value in the last column  
    double pLivePts[nlive][nPar + 1];
    for( i = 0; i < nPar + 1; i++ )
        for( j = 0; j < nlive; j++ )
            pLivePts[j][i] = physLive[0][i * nlive + j];
}


void createDirectory(shared_ptr<Workspace> &w) {
    boost::filesystem::path resultsDir = w->currentDir_;
    resultsDir /= "results";
    if (!boost::filesystem::exists(resultsDir)) {
        boost::filesystem::create_directory(resultsDir);
    }

    boost::filesystem::path dataDir = resultsDir;
    dataDir /= w->SNName_;
    if (!boost::filesystem::exists(dataDir)) {
        boost::filesystem::create_directory(dataDir);
    }
}


void runMultiNest(shared_ptr<Workspace> &w) {
     // set the MultiNest sampling parameters
    int IS = 0;                 // do Nested Importance Sampling?
    int mmodal = 1;                 // do mode separation?
    int ceff = 1;                   // run in constant efficiency mode?
    int nlive = 100 * (w->snmodel_->noModelParams_ + 1);               // number of live points
    double efr = 0.8;               // set the required efficiency
    double tol = 0.5;               // tol, defines the stopping criteria
    int ndims = w->snmodel_->noModelParams_ + 1;                  // dimensionality (no. of free parameters)
    int nPar = w->snmodel_->noModelParams_ + 1;                   // total no. of parameters including free & derived parameters
    int nClsPar = w->snmodel_->noModelParams_ + 1;                // no. of parameters to do mode separation on
    int updInt = 100;              // after how many iterations feedback is required & the output files should be updated  
    double Ztol = -1E20;                // all the modes with logZ < Ztol are ignored
    int maxModes = 100;             // expected max no. of modes (used only for memory allocation)
    int pWrap[ndims];               // which parameters to have periodic boundary conditions?
    for(int i = 0; i < ndims; i++) {
        pWrap[i] = 0;
    }
    string root = "results/"+ w->SNName_ + "/nest-";
    int seed = -1;                  // random no. generator seed, if < 0 then take the seed from system clock
    int fb = 1;                 // need feedback on standard output?
    int resume = 0;                 // resume from a previous job?
    int outfile = 1;                // write output files?
    int initMPI = 0;                // initialize MPI routines?, relevant only if compiling with MPI
    double logZero = -1E20;             // points with loglike < logZero will be ignored by MultiNest
    int maxiter = 0;                // max no. of iterations, a non-positive value means infinity. MultiNest will terminate if either it 

    shared_ptr<SNEvent> sn = w->snevent_;
    void *context = (void*) sn.get();              // not required by MultiNest, any additional information user wants to pass

    // calling MultiNest
    nested::run(IS, mmodal, ceff, nlive, tol, efr, ndims, nPar, nClsPar, maxModes, updInt, Ztol, root.c_str(), seed, pWrap, fb, resume, outfile, initMPI,
    logZero, maxiter, LogLike, dumper, context);
}


void readFitParam(shared_ptr<Workspace> &w) {
    string summaryPath = w->currentDir_.string() +  "/results/"+ w->SNName_ + "/nest-summary.txt";
    int npar = w->snmodel_->noModelParams_ + 1;
    vector< vector<double> > summary = loadtxt<double>(summaryPath, npar * 4 + 2);
    vector<double> logLike = summary[npar * 4 + 1];
    int indexMax = distance(logLike.begin(), max_element(logLike.begin()+1, logLike.end()));
    
    vector<double> param(npar, 0);
    vector<double> error(npar, 0);
    for (int i = 0; i < npar; ++i) {
        param[i] = summary[2*npar + i][indexMax];
        error[i] = summary[npar + i][indexMax];
    }  

    w->fitExplosionMJD_ = param.back();
    w->fitExplosionMJDError_ = error.back();
    param.pop_back();
    error.pop_back();
    w->fitParam_ = param;
    w->fitParamError_ = error;
    w->fitChi_ = logLike[indexMax] / -4.0;
    w->fitRedChi_ = w->fitChi_ / (w->snevent_->mjd_.size() - param.size());
}


void fit3(shared_ptr<Workspace> &w) {
    // Set the results directory based on the SN name
    createDirectory(w);

    // Run the Multinest fitter
    runMultiNest(w);

    // Read the results
    readFitParam(w);
}
