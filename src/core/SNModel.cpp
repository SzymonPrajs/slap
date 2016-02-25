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

#include "SNModel.h"

using namespace std;
using namespace vmath;


vector<double> SMCExtinctionF(vector<double> wavelength, double Rv) {
    vector<double> x(wavelength.size(), 1.0e4);
    x = div(x, wavelength);
    double C1 = -4.959;
    double C2 = 2.264;
    double C3 = 0.389;
    double C4 = 0.461;
    double x0 = 4.6;
    double g = 1.0;

    vector<double> D(x.size());
    for (int i = 0; i < x.size(); i++) {
        D[i] = x[i]*x[i]*pow(pow(x[i]*x[i] - x0*x0, 2) + x[i]*x[i]*g*g, -2);
        D[i] = C1+C2*x[i]+C3*D[i];

        if ( x[i] >= 5.9) {
            D[i] += C4 * (0.5392 * pow((x[i]-5.9),2) + 0.05644*pow((x[i]-5.9),3));
        }

        D[i] += Rv;
    }

    return D;
}

vector<double> SMCExtinction(shared_ptr<Filters> filters, vector<double> wavelength, double EBmV, double Rv) {
    int ID = filters->filterID_.at("V");
    vector<double> ext = SMCExtinctionF(filters->filters_[ID].restWavelength_, Rv);
    for (int i = 0; i < ext.size(); i++) {
        ext[i] = pow(10.0, ext[i] / 2.5);
    }
    double Av = -2.5 * log10(filters->flux(ext, "V"));
    double A = Rv * EBmV / Av;
    vector<double> D(wavelength.size(), 0);
    if (EBmV != 0) {
        vector<double> D = SMCExtinctionF(wavelength, Rv);
    }

    for (int i = 0; i < D.size(); i++) {
        D[i] *= A;
        D[i] = pow(10.0, D[i] / 2.5);
    }

    return D;
}

SNModel::SNModel(shared_ptr<Cosmology> cosmology, shared_ptr<Filters> filters, shared_ptr<Absorption> absorption) {
    cosmology_ = cosmology;
    filters_ = filters;
    absorption_ = absorption;
    absFile_ = "06D4eu"; /*TODO: Need to be globaly setup*/
    derivedParams_ = {}; /*TODO: Why is this here?*/
}


SNModel::SNModel(shared_ptr<Filters> filters) {
    cosmology_ = shared_ptr<Cosmology>(new Cosmology(0));
    filters_ = filters;
    derivedParams_ = {};
    
}

/* TODO: CHANGE SO THAT IT TAKES WAVELENGTH AS INPUT*/
vector<double> SNModel::SED(double t, string f) {
    calcSEDParams(t * cosmology_->a_);
    int ID = filters_->filterID_.at(f);
    int absID = absorption_->absID_.at(absFile_);
    vector<double> sed(filters_->filters_[ID].restWavelength_.size(), 0);
    for(int i = 0; i < filters_->filters_[ID].restWavelength_.size(); ++i) {
        sed[i] = calcSED(filters_->filters_[ID].restWavelength_[i]);
        sed[i] *= cosmology_->a_ / (4 * M_PI * pow(cosmology_->lumDisCGS_, 2));
        sed[i] *= absorption_->abs_[absID].filter_[ID].bandpass_[i];
    }

    return sed;
}

double SNModel::flux(double t, string f, double EBmV, double Rv) {
    calcSEDParams(t * cosmology_->a_);
    int ID = filters_->filterID_.at(f);
    int absID = absorption_->absID_.at(absFile_);
    vector<double> sed(filters_->filters_[ID].restWavelength_.size(), 0);
    vector<double> extinction = SMCExtinction(filters_, filters_->filters_[ID].restWavelength_, Rv, EBmV);

    for(int i = 0; i < filters_->filters_[ID].restWavelength_.size(); ++i) {
        sed[i] = calcSED(filters_->filters_[ID].restWavelength_[i]);
        sed[i] *= cosmology_->a_ / (4 * M_PI * pow(cosmology_->lumDisCGS_, 2));
        sed[i] *= absorption_->abs_[absID].filter_[ID].bandpass_[i];
        sed[i] *= extinction[i];
    }

    return filters_->flux(sed, f);
}


double SNModel::mag(double t, string filterName, double Rv, double EBmV) {
    int ID = filters_->filterID_.at(filterName);
    return -2.5 * log10(flux(t, filterName, Rv, EBmV)) - filters_->filters_[ID].zp_;
}
