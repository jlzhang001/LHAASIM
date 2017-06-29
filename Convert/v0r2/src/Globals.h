//============================================================================ 
//
// Header-File : Globals.h (This file is part of the FDSim++ package)
//
// Date        : Thu May 18 2000
//
// Author      : Luis Prado Jr.
//               University of Campinas
//
// E-Mail      : pradojr@ifi.unicamp.br
//
// Description : This header file declares some constants used in the package
// 
//============================================================================

#ifndef __GLOBALS_H
#define __GLOBALS_H

#include <string>
//----------------------------------------------------------
// Global Variables:
//----------------------------------------------------------
const int    MOD_SIMULATION     = 1;
const int    MOD_RECONSTRUCTION = 2;
const int    MOD_ANALYSIS       = 3;

const double ALTITUDE_OBSERVATORY = 1400; //(m) reference altitude
const double LATITUDE_CENTER    = -35.25;//(deg) reference center of the Surface array
const double LONGITUDE_CENTER   = -69.25;  
const double SD_EAST_SIZE       = 35000.; //(m) width of surface array
const double SD_NORTH_SIZE       = 30000.; //(m) height of surface array
const int    UTM_ELLIPSE_NUM    = 23; // number of reference ellipsoid for earth 
#define UTM_ZONE            "19H"


const double EXP_MAX            = 57.6;
const double EPS                = 1e-14;
const double PI                 = 3.14159265358979323846;
const double FOURTHPI           = PI/4.;
const double TWO_PI             = 2*PI;
const double DEG2RAD            = PI/180;
const double RAD2DEG            = 180/PI;
const double SQRT3              = 1.732050808;
// Speed of light given in m / ns
const double C_LIGHT            = 0.299792458;
// 1.5 degrees in Radians
const double ETA0               = 0.0261799387;
const int N_COLS                = 20;
const int N_ROWS                = 22;
const int MAX_N_FRM             = 150;
const int NTSL                  = MAX_N_FRM-3;
const int MAX_N_PXL             = 80;
const int MAX_N_EYE             = 6;
const int MAX_N_MIRROR          = 6;
const double R_MIRROR            = 340;
const double EPSILON            = 4.3;
const double SIN_AX             = 0.28240;
const double SIN_AZ             = 0.25640; 
const double SIN_BX             = 0.26885;
const double SIN_BZ             = 0.24895;
const double BASE_MERC          = 0.46; 
const double HEIGHT_MERC        = 1.8;
// Horizontal width of a mirror for hexagonal pixels
const double W_PHI_MIRROR       = (N_COLS + 0.5)*ETA0; 
// Elevation width of a mirror for hexagonal pixels (betwen extremities)
const double W_OMEGA_MIRROR     = (3*N_ROWS+1)*SQRT3*ETA0/6; 
// Lower width
const double LOWER_WIDTH        = 17*ETA0/SQRT3;
const int MAX_N_SHW_PTS         = 200;

#endif // __GLOBALS_H

//==End of File=================================================================
