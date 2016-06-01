#include <iostream>
#include <random>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#include "core/Cosmology.h"
#include "core/Filters.h"
#include "core/Absorption.h"
#include "core/SNModel.h"
#include "core/SNEvent.h"
#include "models/Magnetar.h"
#include "vmath/loadtxt.h"
#include "vmath/algebra.h"

using namespace std;


// Constants
const int CALCULATIONS = 10000;   // Number of iretations per Rate
const int BINS = 21;              // Max number of SLSN detected by the survey +1
const int MJD_START = 52660;      // Start date of the survey
const int MJD_END = 54267;        // End date of the survey
const double Z_MAX = 1.6;         // Redshift range for the survey
const double Z_MIN = 0.7;         // Redshift range for the survey

// Flat Rate
const double RateMin = 5;
const double RateMax = 600;
const double RateStep = 5;

// SFH Rate
// const double RateMin = 0.5;
// const double RateMax = 70;
// const double RateStep = 0.5;

// Cosmology variables
const double c  = 2.99792458e8;
const double h = 6.62606957e-34;
const double k = 1.3806488e-23;
const double pi = 3.14159265359;
const float H0 = 69.6;
const float WM = 0.286;
const float WV = 0.714;

struct MAGLIM 
{
    string Name;
    vector<double> Mag;
    vector<double> Prob;
};


void ReadLog(string FileName, vector <string> & First, vector <string> & Second, vector <int> & Third, vector <double> & Fourth) 
{
   ifstream file;
   file.open( FileName );
   
   string temp_1, temp_2;
   double temp_3, temp_4;
   
   while ( file >> temp_1 >> temp_2 >> temp_3 >> temp_4 ) {
      First.push_back( temp_1 );
      Second.push_back( temp_2 );
      Third.push_back( int(temp_3) );
      Fourth.push_back( temp_4 );
   }
   
   file.close();
}

void Read2Double (string FileName, vector <double> & First, vector <double> & Second) 
{
    ifstream file;
    file.open(FileName);
    double temp_1, temp_2;

    while (file >> temp_1 >> temp_2) 
    {
        First.push_back(temp_1);
        Second.push_back(temp_2);
    }
    file.close();
}

double trapz (vector <double> & vec, double w) 
{
    double res = 0;
    for (int i = 0 ; i < (vec.size() - 1) ; i++) 
    {
        res += (vec[i] * w) + (0.5 * (vec[i+1] - vec[i]) * w);
    }
    
    return res;
}

double interp (double org_x, vector<double> &temp_x, vector<double> &temp_y) 
{
    double org_y = 0;
    double m, c;
    
    for (int i = 0 ; i < (temp_x.size() - 1) ; i++) 
    {
        if (temp_x[i] <= org_x && temp_x[i+1] >= org_x) 
        {
            m = (temp_y[i+1] - temp_y[i])/(temp_x[i+1] - temp_x[i]);
            c = temp_y[i] - m*temp_x[i];
            org_y = m*org_x + c;
            break;
        }
    }

    return org_y;
}

/*****************/
/*** COSMOLOGY ***/
/*****************/

double E (double z1) 
{
    return 1.0 / sqrt(WV + WM * pow((1.0 + z1),3));
}

double FindDL(double z) 
{
    vector <double> Steps(10000);
    for (int i = 0 ; i <= 10000; ++i) 
    {
        Steps[i] = E(z * i / 10000.0);
    }
    
    double a = 1. / (1. + z);
    double DH = c / (1000.0 * H0);
    double DM = trapz(Steps,0.0001 * z);

    return DH * DM / a;
}

double FindVC(double z)
{
    return pow(FindDL(z)/(1000.*(1.+z)),3) * 4. * pi / 3;
}

vector<vector<double>> findN(double zMin, double zMax, double stp, double rate, double t)
{
    vector<vector<double>> DV(3);
    vector<double> delZ, delV, delN;
    for (double dz = zMin + stp ; dz < zMax+stp ; dz += stp)
    {
        delV.push_back(FindVC(dz) - FindVC(dz-stp));
        delZ.push_back(dz);
        delN.push_back(delV.back()*rate*t/(1.+dz));
    }
    DV[0] = delZ;
    DV[1] = delV;
    DV[2] = delN;
    return DV;
}

double SFH(double z, double a, double b, double c, double d) {
    return ((a+b*z) * 0.7) / (1.0 + pow((z/c),d));
}


int main() {
    shared_ptr<Cosmology> cosmology(new Cosmology(0.0));
    shared_ptr<Filters> filters(new Filters("/Users/szymon/Projects/slap/share/slap/filters"));
    shared_ptr<Absorption> absorption(new Absorption("/Users/szymon/Projects/slap/share/slap/absLines"));
    shared_ptr<Magnetar> magnetar(new Magnetar(cosmology, filters, absorption));
    const string absName = "NoAbs";
    magnetar->absFile_ = absName;
    int fID = filters->filterID_["SNLS_i"];
    int aID = absorption->absID_[absName];

    // Load parameters
    vector<vector<double>> param = loadtxt<double>("/Users/szymon/Desktop/Parameters.txt", 3);

    // Initialise the random number generator
    srand(time(NULL));
    random_device rd;
    mt19937_64 gen(rd());
    uniform_real_distribution<> posGen(0, 41253 - 1);
    uniform_int_distribution<> mjdGen(MJD_START, MJD_END);
    uniform_int_distribution<> parGen(0, param[0].size() - 1);

    // Variables  
    vector<vector<double>> dN;
    vector<double> SNbin(BINS);
    int SN_No;
    double SN_dec;
    double pos;
    vector<vector<int>> D;
    int fakeT;
    double int_y;
    double Mag;
    int fN;

    // SN Params
    int mjdExp;
    double TotalProbability;
    int TotalCount;

    // Load in the OBS LOGS
    vector<int> Temp_Date;
    vector<double> Temp_Limit;
    vector<string> Temp_Field;
    vector<string> Temp_File;
    vector<vector<int>> Date;
    vector<vector<double>> Limit;
    vector<vector<string>> Field;
    vector<vector<string>> File;
    for (int OL = 1 ; OL < 5 ; OL++)
    {
        Temp_Date.clear();
        Temp_Limit.clear();
        Temp_Field.clear();
        Temp_File.clear();
        ReadLog("/Users/szymon/Projects/MPhys/Git/Data/ObsLogs/D"+to_string(OL)+".txt", Temp_Field, Temp_File, Temp_Date, Temp_Limit);
        Date.push_back(Temp_Date);
        Limit.push_back(Temp_Limit);
        File.push_back(Temp_File);
        Field.push_back(Temp_Field);
    }

    // Load Mag Limits 
    MAGLIM temp;
    string temp_Name;
    vector<double> temp_Mag;
    vector<double> temp_Prob;
    vector<vector<MAGLIM>> mag_lim(4);
    for (int iM = 0 ; iM < 4 ; iM++) {
        for (int jM = 0; jM < File[iM].size() ; jM++) {
            temp_Mag.clear();
            temp_Prob.clear();
            temp_Name = Field[iM][jM]+"_"+File[iM][jM]+".dat";
            Read2Double("/Users/szymon/Projects/MPhys/Git/Data/MagLimits/"+temp_Name, temp_Mag, temp_Prob);
            temp.Name = temp_Name;
            temp.Mag = temp_Mag;
            temp.Prob = temp_Prob;
            mag_lim[iM].push_back(temp);
        } 
    }

    for (double Rate = RateMin ; Rate <= RateMax ; Rate += RateStep) {   
        // Number of supernovas that explode per redshift bin
        dN = findN(Z_MIN, Z_MAX, 0.01, Rate, (MJD_END - MJD_START)/365.25);

        // USE IF EVOLVING WITH SFH
        // for (int i = 0; i < dN[0].size(); ++i) {
        //     dN[2][i] = dN[2][i] * SFH(dN[0][i], 0.017, 0.13, 3.3, 5.3) / SFH(0.0, 0.017, 0.13, 3.3, 5.3);
        // }

        // Clear bins
        for (int i = 0; i < BINS; ++i) {
            SNbin[i] = 0;   
        }

        /*Repeat the simulation many times*/
        for (int i = 0 ; i < CALCULATIONS ; ++i) {
            // --> This is where a single simulation is starting <-- 
            D.clear();
            for (int iD = 0 ; iD < 5 ; ++iD) {
                D.push_back(vector<int>(dN[0].size(), 0));
            }

            // Find how many supernovae can be seen in each redshift bin and field
            for (int iD = 0 ; iD < D[0].size() ; iD++) {
                SN_No = int(dN[2][iD]);
                SN_dec = dN[2][iD] - SN_No;
                if (SN_dec <= generate_canonical<double, 10>(gen)) {
                    SN_No++;
                }
                
                for (int j = 0 ; j < SN_No ; j++) {
                    // Random position in the sky
                    pos = posGen(gen);

                    // If within the observed field
                    if (pos <= 0.88) {
                        D[0][iD]++;
                    } else if (0.88 < pos && pos <= 1.78) {
                        D[1][iD]++;
                    } else if (1.78 < pos && pos <= 2.68) {
                        D[2][iD]++;
                    } else if (2.68 < pos && pos <= 3.56) {
                        D[3][iD]++;
                    }                
                }
            }

            TotalCount = 0;
            /*For each field run the lightcurve simulation*/
            for (int j = 0 ; j < 4 ; j++) {
                for (int iZ = 0 ; iZ < D[j].size() ; iZ++) {
                    /*Check each SN in each field*/
                    for (int k = 0; k < D[j][iZ]; k++) {
                        /*Assign SN parameters*/
                        fN = parGen(gen);
                        mjdExp = mjdGen(gen);
                        cosmology->set(dN[0][iZ]);
                        filters->filters_[fID].restWavelength_ = mult<double>(filters->filters_[fID].inputWavelength_, cosmology->a_);
                        AbsFilter flt;
                        absorption->abs_[aID].filterID_ = filters->filterID_;
                        absorption->abs_[aID].filterName_ = filters->filterName_;
                        absorption->abs_[aID].filter_.clear();
                        
                        for (int f = 0; f < absorption->abs_[aID].filterID_.size(); ++f) {
                            if (f == fID) {
                                flt.restWavelength_ = filters->filters_[f].restWavelength_;
                                flt.bandpass_ = interp<double>(flt.restWavelength_,
                                                absorption->abs_[aID].inputWavelength_,
                                                absorption->abs_[aID].inputBandpass_,
                                                0.4,1.0);
                            }
                            absorption->abs_[aID].filter_.push_back(flt);  
                        }

                        magnetar->modelParams_[0] = param[0][fN];
                        magnetar->modelParams_[1] = param[1][fN];
                        magnetar->modelParams_[2] = param[2][fN];
                        magnetar->calcDerivedParams();

                        TotalProbability = 1;
                        /*Loop though every day of the fake lightcurve*/
                        Mag = magnetar->mag(20, "SNLS_i");
                        for (int m = 0 ; m < Date[j].size() ; m++) {
                            if (Date[j][m] > mjdExp && Date[j][m] < mjdExp + (60 * cosmology->a_)){
                                fakeT = Date[j][m] - mjdExp;
                                Mag = magnetar->mag(fakeT, "SNLS_i");
                                if (Mag <= 25.75 && Mag >= 18.55) {
                                    int_y = interp(Mag, mag_lim[j][m].Mag, mag_lim[j][m].Prob);
                                    TotalProbability *= (1.0-int_y);
                                }
                            }
                        }

                        if (generate_canonical<double, 10>(gen) >= TotalProbability) {
                            TotalCount++;
                        }
                    }
                }
            }
            
            if (TotalCount > BINS-1) {
                SNbin.back()++;
            } else {
                SNbin[TotalCount]++;
            }
        }
        
        // cout << Rate << " " << Rate * SFH(1.2, 0.017, 0.13, 3.3, 5.3) / SFH(0.0, 0.017, 0.13, 3.3, 5.3) << " ";
        cout << Rate << " ";
        for (int i = 0; i < BINS; i++) {
            cout << SNbin[i] << " ";
        }
        cout << endl;
    }

    return 0;
}