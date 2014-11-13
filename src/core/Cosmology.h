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

#ifndef SLAP_CORE_COSMOLOGY_H_
#define SLAP_CORE_COSMOLOGY_H_

#include <math.h>
#include <vector>
#include <memory>
#include "../vmath/integrate.h"

using namespace std;

const double SI_C = 299792458;
const double CGS_C = 29979245800;
const double CGS_H = 6.62606957e-27;
const double CGS_K = 1.3806488e-16;

class Cosmology {
public:
    // input 
    double z_;
    double a_;

    // required variables 
    const double H0_ = 70;
    const double WM_ = 0.3;
    const double WV_ = 0.7;
    
    // results
    double comDis_;
    double comVol_;
    double lumDis_;
    double lumDisCGS_;

    // Constructor
    Cosmology(double z = 0);

    // Functions
    void set(double);
    double E(double);
    double findComDis(double);
    double findLumDis(double);
    double findComVol(double);
    vector<double> findDZ(double);
    vector<double> findDV(vector<double>&);
};

#endif