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

#include "Cosmology.h"

using namespace std;
using namespace vmath;


Cosmology::Cosmology() {
    z_ = -1;
    a_ = -1;
    comDis_ = -1;
    comVol_ = -1;
    lumDis_ = -1;
}


Cosmology::Cosmology(double z, bool init) {
    if (init == true) { 
        set(z);
    } else {
        z_ = z;
        a_ = 1/(1+z);
        comDis_ = -1;
        comVol_ = -1;
        lumDis_ = -1;
        lumDisCGS_ = -1;
    }

}


void Cosmology::set(double z) {
    z_ = z;
    a_ = 1.0/(1.0+z);
    comDis_ = findComDis(z);
    lumDis_ = findLumDis(z);
    comVol_ = findComVol(z);
    lumDisCGS_ = lumDis_ * 3.08568e24;
}


double Cosmology::E(double z1) {
    return 1.0 / sqrt(WV_ + WM_ * pow((1.0 + z1),3));
}


double Cosmology::findComDis(double z) {
    vector<double> steps(10000);
    
    for (int i = 0; i <= 10000; ++i) {
        steps[i] = E(z * i / 10000.0);
    }
    
    double DH = SI_C/H0_;
    double DM = trapz<double>(steps,0.0001*z);
    return DH*DM/1000;
}


double Cosmology::findLumDis(double z) {
    return findComDis(z)*(1+z); 
}


double Cosmology::findComVol(double z) {
    return pow(findComDis(z) / 1000, 3) * 4 * M_PI /  3;
}


vector<double> Cosmology::findDZ(double step) {
    int dzSize = int(z_/step);
    vector<double> res(dzSize);

    for (int i = 0; i < dzSize; ++i) {
        res[i] = (i + 1) * step;
    }

    return res;
}


vector<double> Cosmology::findDV(vector<double> &dz) {
    int dzSize = dz.size();
    vector<double> res(dzSize);

    res[0] = findComVol(dz[1]);    
    for (int i = 1; i < dzSize; ++i) {
        res[i] = findComVol(dz[i]) - findComVol(dz[i-1]);
    }

    return res;
}