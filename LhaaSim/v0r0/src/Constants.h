/*============================================================================= 
=                                                                             = 
=  Header-File : Constants.h                                                  =
=                                                                             =
=  Date        : Thu January 16 2002                                          =
=                                                                             =
=  Author      : I. Lhenry-Yvon                                               =
=                                                                             =
=  Description : This header file declares some constants used in the package =
=                                                                             =
===================================================================*/

#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#include <string>
/*-------------------------------------------------
-               Global Variables:                 -
-------------------------------------------------*/

// GENERAL CONSTANTS

const double EPSILON     = 1.E-6;
const double ELTCHARGE   = 1.6E-19; // (C) charge élémentaire
const double PI          = 3.14159265358979323846;
const double DEG2RAD     = PI/180;
const double RAD2DEG     = 180/PI;
const double GeV2J       = 1.60217646E-10; // (J) conversion GeV -> J
const double SQRT3       = 1.732050808;
const double CLIGHT      = 0.299792458;// Speed of light given in m/ns
const double s2ns        = 1e9;
const double CLIGHTsec   = CLIGHT*s2ns;// Speed of light given in m/ns
const double GFERMI      = 1.16639E-5; // GeV^-2
const double Na          = 6.0221415E23; // (/mol) Avogadro
const double elC         = 1.6E-19; // (C) elementary charge
const double h           = 6.58E-16*1E-9; // (GeV.s) Planck


// OBSERVATORY CONSTANT

const double ALTITUDE_OBSERVATORY = 4300; //(m) altitude 


// TANK CONSTANTS (MD)

const double STATION_RADIUS     = 3.60;  //(m) Lhaaso_MD : 3.6 m, Lhaaso_ED (scintillateur): 1 m
const double STATION_HEIGHT     = 1.20;  //(m) Lhaaso_MD & Auger : 1.2m
const double TOP_FACT           = 1.0;  //white top
const int  NPM                  = 1; // number of PMT in the tank
const double RAD_PM             = 0.131; //(m) Hamamatsu , 0.1477 Photonis
const double HC_PM              = 0.0902; //(m) Hamamatsu , 0.0776 Photonis

// 1 PMT (LHAASO)
const double X_PM[NPM]          = {0.};
const double Y_PM[NPM]          = {0.};

// SCINTILLATOR CONSTANTS (ED)
// Ex: Scintillateur composé d'Anthracène C14H10, rendement lumineux 55%,
// Au premier ordre, on considère une plaque de 1m², 1.5 cm d'épaisseur :

const double SCINT_LENGTH    = 1.00;  //(m)  Scintillateur 1 m²
const double SCINT_HEIGHT    = 0.015;  //(m) 1,5 cm thickness
const double A               = 178.;
const double Z               = 94.;
const double I               = 69.5E-6; //(MeV) ionization energy
const double density         = 1.283; //(g/cm³) scintillator density
const double factor          = 0.1535; //(MeV.cm²/V)
const double delta           = 0.; //first approximation (shell correction)
const double C               = 3.1514; // shell correction
const double M               = 105.7; // muon mass (in MeV)
const double me              = 0.511; // electron mass (in MeV)


// SIMULATION CONSTANTS

const int NCLONES = 10;
const int NMAXPARTINTANK = 5000;


// BINNING OF PE PROFILES AND ADC TRACES

const int  MAXNUMBEROFTIMEBINS  = 19200; // bins of 1ns (10000)
const int  SAMPLINGTIME         = 25; // (ns)
const int  MAXNUMBEROFADCBINS   = MAXNUMBEROFTIMEBINS/SAMPLINGTIME;
const int  TIMEOFFSET           = 100; //time offset of 100 ns ( to save very early particles) 
 
const double re                  = 2.817E-13; // electron radius (in m)
const double me_gr               = 9.11E-28; // (in g)
const double MASS_E              = 0.000511;  // (in GeV)
const double MASS_MU             = 0.1057;   // (in GeV)
const double LIFE_MUON           = 2197.0; // (in ns)
const double EMIN_E              = 0.00025; //
const double EMIN_MU             = 0.05 ;   //
const double DEDX_E              = 0.20 ;    // (in GeV/m)
const double DEDX_MU             = 0.20;     // (in GeV/m)
const double CHERCONST           = 0.0458506;
const double DELTARAYCONST       = 0.008445; // (in Gev/m)


// CONSTANTS FOR FAST SIMULATION BASED ON MUONS SHAPE

const double MUONPULSE_RISINGTIME       = 13.31;
const double MUONPULSE_DECAYTIME        = 54.72;
const double MUONPULSE_RISINGNORM       = 3.77;
const double MUONPULSE_DECAYNORM        = 2.45;
const double MUONPULSE_TOTALTIME_CALIB  = 400.0; 
const double MUONPULSE_TOTALTIME_SHOWER = 10000.0;
const double MUONPULSE_PROBLIMIT        = 0.75;
const double VEMNORMALIZATION           = 13.6; //to normalize the fast sim to the vem


#endif // __CONSTANTS_H

//==End of File=================================================================

