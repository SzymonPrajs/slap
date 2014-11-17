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

#include "Magnetar.h"

using namespace std;
using namespace vmath;


Magnetar::Magnetar(shared_ptr<Cosmology> cosmology, shared_ptr<Filters> filters) : SNModel(cosmology, filters) {
    SEDParams_.resize(2);
    modelParam_.resize(3);
}


void Magnetar::calcDerivedParams() {
    double temp;
    /* Constant quantities */
    opacity_ = 0.1;
    energyRadiation_ = 0;
    alpha_ = 10;

    /* Magnetar spin down timescale */
    tauP_ = 4.7 * pow(modelParam_[1], -2.0) * pow(modelParam_[2], 2.0);
    
    /* Energy produced by the spin down of a magnetar */ 
    energyMagnetar_ = 4.9e46 * pow(modelParam_[1], 2.0) * pow(modelParam_[2], -4.0) * tauP_ * 86400;
    
    /* Kinetic energy of the system */
    energyKinetic_ = 1.0e51 + 0.5 * (energyMagnetar_ - energyRadiation_);
    
    /* Mass ejected in the explosion */
    temp = 10 * pow(energyKinetic_ / 1.0e51, -0.25) * pow(opacity_ / 0.1, 0.5);
    ejectedMass_ = pow(modelParam_[0] / temp, 4.0/3.0);

    /* core velocity */
    temp = energyKinetic_ * 10.0 / (3.0 * ejectedMass_ * 2.0e33); 
    velocityCore_ = sqrt(temp) * 86400;
}


double Magnetar::lumMagnetar(double t) {
    return 4.9e46 * pow(modelParam_[1], 2.0) * pow(modelParam_[2], -4.0) / pow(1 + t / tauP_, 2.0);
}


double Magnetar::lumSN(double t) {
    double res = 2 * exp(-1 * pow(t / modelParam_[0], 2.0));

    vector<double> arr(int(t+1));
    for (int i = 0; i < int(t+1); ++i) {
        arr[i] = lumMagnetar(i) * i * exp(pow(i / modelParam_[0], 2)) / modelParam_[0];
    }
    res *= trapz<double>(arr,1);

    return res;
}


double Magnetar::energy(double t) {
    vector<double> arr(int(t+1));

    for (int i = 0; i < int(t+1); ++i) {
        arr[i] = lumSN(i);
    }
    
    return trapz<double>(arr, 86400); 
}

void Magnetar::calcSEDParams(double t) {
    calcDerivedParams();
    /*
     *Approximating Vphot as Vcore for the time being
     *TODO: Take into the account the expansion of the photosphere
     */
    SEDParams_[0] = velocityCore_ * t;
    /*
     *Approximating E ~ E(t)
     *TODO: Integrate L(t) to obtain correct E(t)
     */
    SEDParams_[1] = 1.0e5 * pow(energy(t) / 1e51, -0.125) * pow(ejectedMass_, 0.375) * pow(t / 10, -0.75);

    cout << SEDParams_[0] << " " << SEDParams_[1] << endl;
}
   
 
vector<double> Magnetar::calcSED(double t) {
    calcSEDParams(t);
    vector<double> sed(restWavelength_.size(), 0);

    double res;
    for(int i = 0; i < restWavelength_.size (); ++i) {
        res = 2.0 * CGS_H * M_PI * pow(CGS_C,2) / pow(restWavelength_[i] * 1e-8,5);
        res /= exp(CGS_H * CGS_C / (restWavelength_[i] * 1e-8 * CGS_K * SEDParams_[1])) - 1.0;
        res *= pow(SEDParams_[0]/cosmology_->lumDisCGS_,2);
        res *= 1e-8;
        sed[i] = res;
    }

    return sed;
}
