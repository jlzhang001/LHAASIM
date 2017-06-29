//-----------------------------------------------------------------
/*! \file LocalStationConst.h
    \brief File of constants used in SD detector simulation
    \ingroup SIMULATION
*/
//-----------------------------------------------------------------
#ifndef _LOCALSTATIONCONST_H_
#define _LOCALSTATIONCONST_H_
//height of one Water Tank in meters units
const double HEIGHT_STAT= 1.2;   
//radius of one water Tank  in meters units
const double RAD_STAT= 1.8;  

//Top surface of one water tank in meter-squared units
//#define TOP_AREA PI*RAD_STAT*RAD_STAT 
const double TOP_AREA = 10.1787601976; 
//Side surface of one water Tank in meter-squared units
//#define SIDE_AREA 2*RAD_STAT*HEIGHT_STAT
const double SIDE_AREA= 4.32;

const double EPSILON2 = 1e-6;

#define NBPM 3
#define NBFADC 6

//physical constants
#define MASS_ELECTRON .000511
#define MASS_MUON .1057
#define LIFE_MUON 2197.
#define DEDX_ELECTRON .2
#define DEDX_MUON .2
#define WATER_INDEX 1.33
#define EMIN_ELECTRON .000264  // threshold for Cherenkov radiation
#define EMIN_MUON .05455       //        idem
#define EMIN_PHOTON .0003      // lower energy cut for photons
//#define RATE_PHOTO_EL  27.     // nb of photo-el (per PMT) per meter of range
#define RATE_PHOTO_EL  40.     // nb of photo-el (per PMT) per meter of range
                               // (with beta=1)
#define TIME_SLOT 1.           // time slot for photo-electron counting (not for FADC !)
#define NSLOT_PHOTO_EL 20000 
// delta ray stuff
#define DELTARAYCONST 0.008445 // in Gev/m


// constants for sampling and detector simulation
#define TIMESMOOTH .1        


  // constants for detector simulation
#define WATER_INDEX 1.33
#define C_LIGHT_WATER (C_LIGHT/WATER_INDEX)
#define NSTEP_MUON 27
#define NSTEP_ELECTRON 32
#define N_WAVELENGTH 30
#define WAVELENGTH_MIN .000000300
#define D_WAVELENGTH   .000000010
#define CHERENKOV_CONST .0458506
/* #define COLL_EFF        0.8 */
/* #define ABS_LENGTH_MAX   30. // Original SDSIM    */
#define COLL_EFF       0.6
#define ABS_LENGTH_MAX  30. // Original SDSIM   
// stephane parameters
/* #define COLL_EFF       1 */
/* #define ABS_LENGTH_MAX  90. // Original SDSIM    */
//#define ABS_LENGTH_MAX  90. // Original SDSIM-Geant   
//#define ABS_LENGTH_MAX   192. // in www.sbu.ac.uk   

//-----------------------------------------------------------
/*! \var static const double ESTEP_MUON[NSTEP_MUON]
    \brief bins of energy for muons in GeV
 */
//-----------------------------------------------------------
static const double ESTEP_MUON[NSTEP_MUON] = 
{1.,.95,.9,.85,.8,.75,.7,.65,.6,.55,.5,.45,.4,.35,.3,.25,.2,
 .18,.16,.14,.12,.1,.09,.08,.07,.06,.05};



//-----------------------------------------------------------
/*! \var static const double ESTEP_ELECTRON[NSTEP_ELECTRON] 
    \brief bins of energy for electrons in GeV
 */
//-----------------------------------------------------------
static const double ESTEP_ELECTRON[NSTEP_ELECTRON] = 
{.01,.0095,.009,.0085,.008,.0075,.007,.0065,.006,.0055,.005,.0045,.004,
 .0035,.003,.0025,.002,.0018,.0016,.0014,.0012,.001,.0009,.0008,.0007,
 .0006,.0005,.00045,.0004,.00035,.0003,.00025};


//-----------------------------------------------------------
/*! \var static const float PMT_EFFIC[N_WAVELENGTH]
    \brief PMT quantum efficiency for various wavelength bins
 */
//-----------------------------------------------------------
static const float PMT_EFFIC[N_WAVELENGTH] =
{.04, .09, .14, .17, .21, .22, .23, .24, .24, .24,
 .23, .22, .22, .21, .20, .19, .18, .16, .15, .13,
 .11, .10, .09, .08, .07, .06, .05, .04, .03, .02}; 





//-----------------------------------------------------------
/*! \var static const float RELATIVE_ABS_LENGTH[N_WAVELENGTH]
    \brief relative absorption length in water according to the wavelength
 */
//-----------------------------------------------------------
static const float RELATIVE_ABS_LENGTH[N_WAVELENGTH] =
// original SDSIM
{.102, .137, .171, .212, .257, .311, .380, .480, .655, .754,
 .842, .889, .941, 1.00, .993, .993, .923, .923, .818, .735,
 .560, .403, .302, .284, .258, .226, .203, .180, .133, .092};
// from reference www.sbu.ac.uk :
//static const float  RELATIVE_ABS_LENGTH[N_WAVELENGTH] =
//{0.190564,0.225917,0.266385,0.312514,0.444116,0.636212,0.902213,0.919043,
//0.816853,0.728252,0.651152,0.583825,0.524838,0.472995,0.415073,0.341361,
//0.281948,0.233836,0.194699,0.162726,0.136497,0.114895,0.0970361,0.0822172,
//0.0698774,0.0595672,0.0509245,0.0436567,0.0375266,0.0323408};



//-----------------------------------------------------------
/*! \var static const float WALL_REFLECT[N_WAVELENGTH]
    \brief  reflection coefficient
 */
//-----------------------------------------------------------
static const float WALL_REFLECT[N_WAVELENGTH] =
//{.7107,.7494,.7827,.8109,.8347,.8544,.8706,.8836,.8938,.9017,
// .9075,.9116,.9143,.9158,.9164,.9164,.9158,.9150,.9141,.9131,
// .9122,.9114,.9109,.9106,.9106,.9108,.9111,.9116,.9121,.9125}; 
// original SDSIM
//{.90,.90,.90,.90,.90,.90,.90,.90,.90,.90,
// .90,.90,.90,.90,.90,.90,.90,.90,.90,.90,
// .90,.90,.90,.90,.90,.90,.90,.90,.90,.90};

 {0.77275,0.810871,0.843415,0.870915,0.894437,0.913449,0.928387,0.940657,0.95026,0.957535,0.962773,0.966363,0.968545,0.96966,0.97,0.96966,0.968933,0.96806,0.967041,0.966023,0.965101,0.964422,0.964035,0.963889,0.963986,0.964228,0.964665,0.965198,0.965635,0.965635}; // Geant version 

// geant4 version - from stephane
/*  {0.952*0.7755, 0.952*0.8178, 0.952*0.8541, 0.952*0.8849, 0.952*0.9108, 0.952*0.9334, 
   0.952*0.9500, 0.952*0.9642, 0.952*0.9753, 0.952*0.9840, 0.952*0.9903, 0.952*0.9948,
   0.952*0.9977, 0.952*0.9993, 0.952*1.000,  0.952*1.000,  0.952*0.9993, 0.952*0.9985,
   0.952*0.9975, 0.952*0.9964, 0.952*0.9954, 0.952*0.9945, 0.952*0.9940, 0.952*0.9937, 
   0.952*0.9937, 0.952*0.9939, 0.952*0.9942, 0.952*0.9948, 0.952*0.9953, 0.952*0.9957};*/

//-----------------------------------------------------------
/*! \var static const float WALL_SPECULAR[N_WAVELENGTH]
    \brief  
 */
//-----------------------------------------------------------
static const float WALL_SPECULAR[N_WAVELENGTH] =
{.07107,.07494,.07827,.08109,.08347,.08544,.08706,.08836,.08938,.09017,
 .09075,.09116,.09143,.09158,.09164,.09164,.09158,.09150,.09141,.09131,
 .09122,.09114,.09109,.09106,.09106,.09108,.09111,.09116,.09121,.09125};


//----------------------------------------------------------------
/*! \var enum TANK_WALL
    type of wall encoutered by a Cerenkov photon
 */
enum TANK_WALL {TOP,BOTTOM,SIDE};
//----------------------------------------------------------------


// PMT as a portion of a sphere
static const double RADIUS_PMT = 0.133;  // SDSim original value
//static const double RADIUS_PMT = 0.0918;  //Geant4 value
static const double Z_CENTER_PMT = 1.262; 
//static const double X_PMT[NBPM+1] = {0., 0.  ,-1.039, 1.039};
//static const double Y_PMT[NBPM+1] = {0., 1.200,-0.600,-0.600};
static const double X_PMT[NBPM+1] = {0.  ,-1.039, 1.039,0};
static const double Y_PMT[NBPM+1] = {1.200,-0.600,-0.600,0};

//----------------------------------------------------------------
/*  Calibration constants
 */

//----------------------------------------------------------------

const double VEMTOPHOTFAST=142.2;
const double VEMTOFADCHFAST=146.7;
const double VEMToFADCLFAST=4.025;
const double SIGNALOVERPEAKFAST=3.716; // from high gain channel
//const double VEMTOPHOTDETAILED=149.9;
//const double VEMTOFADCHDETAILED=147.4;
//const double VEMTOFADCHDETAILED=146.64;  // my parameter from calib muon
//const double VEMToFADCLDETAILED=4.16;
//const double SIGNALOVERPEAKDETAILED=3.001; // from high gain channel
//const double SIGNALOVERPEAKDETAILED=4.132; // my parameter from calib muon
//--- Estelle values
const double VEMTOPHOTDETAILED=57.77;
//const double VEMTOFADCHDETAILED=147.4;
const double VEMTOFADCHDETAILED=102.363;  // my parameter from calib muon
const double VEMToFADCLDETAILED=0.388;
//const double SIGNALOVERPEAKDETAILED=3.001; // from high gain channel
const double SIGNALOVERPEAKDETAILED=3.83; // my parameter from calib muon


#endif
