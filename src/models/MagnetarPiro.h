/*SLAP - Supernova Lightcurve Analysis Package
 *
 *Copyright (C) 2014-2016  Szymon Prajs
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

#ifndef SLAP_MODEL_MAGNETARPIRO_H_
#define SLAP_MODEL_MAGNETARPIRO_H_

#include <vector>
#include <memory>
#include <math.h>
#include "../vmath/integrate.h"
#include "../core/Cosmology.h"
#include "../core/Filters.h"
#include "../core/SNModel.h"
#include <gsl/gsl_integration.h>

using namespace std;
using namespace vmath;

class MagnetarPiro : public SNModel {
/*
 * modelParam_ = {Re, Me, Mc, B, P}
 */
private:
    /* contants */
    double opacity_;
    double energyKinetic_;
    double tauP_;
    double energyMagnetar_;
    double energyRadiation_;
    double energySN_;
    double tauM_;
    double velocityCore_;
    double alpha_;
    double Wang14_;

    /* Piro derived variables */
    double timescalePeak_;
    double velocityExtended_;
    double timescaleExtended_;
    double energyExtended_;

public:
    /* Constructor */
    MagnetarPiro(shared_ptr<Cosmology> cosmology, shared_ptr<Filters>, shared_ptr<Absorption>);

    /* SNModel virtual classes */
    void calcDerivedParams();
    double calcSED(double);
    void calcSEDParams(double);
    void printDerivedVariables();

    /* Piro model */
    double lumMag(double);
    double lumPiro(double);
    double lumSN(double);
    double lumMagnetar(double);
    double radius(double);
    double temperature(double);
};

#endif