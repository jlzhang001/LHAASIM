//=========================================================================== 
//
// Header-File: Parameters.h (This file is part of the FDSim++ package)
//
// Date       : Wed May 17 2000
//
// Last change: Thu Oct 10 2000
//
// Author     : Luis Prado Jr.
//              University of Campinas
//
// E-Mail     : pradojr@ifi.unicamp.br
//
// Description: Header-file of the Parameters class 
// 
//=========================================================================== 
/*! \file Parameters.h
    \author  Luis Prado Jr
    \date Thu Oct 10 2000
    \brief Header file for the Configuration of SDSim
    \ingroup CONFIGURATION 
*/
//-------------------------------------------------------------------------
//-----------------------------------------------------------------------
/*! \defgroup CONFIGURATION Configuration of SDSim by the external user
 */
//-----------------------------------------------------------------------
#ifndef __PARAMETERS_H
#define __PARAMETERS_H

// System Headers:
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

// Local Headers:
#include "Utils.h"
#include "Globals.h"

// Global Variables:
enum{ GENERATE, READ_FROM_AIRES, READ_FROM_CORSIKA };

//! Class definition of the Parameters object.
/*! \class Parameters
 *    This class managers all the parameters or variables used in the 
    simulations. It reads and checks the input file passed as argument by 
    FDSim++ (see the main page of this documentation).
 *   Parameters are classified as:
 *      - control variables\n
 *        Variables used to control: the number of events to be simulated,
          the seed for the random number generator, the type of output 
	  file and the error variable.        
 *	- shower variables\n
          Variables related to the shower instantiation and to the parameters 
	  used by the generation of the longitudinal profile. 
 *	- site variables\n
          Variables to set the number and positions of the eyes and the 
          geometry of the array.
 *	- detector variables\n
          Variables related to the detector.
 *	- variables for noise studies\n
          Variables related to noise parameters to be used in future studies.
 *	- light variables\n
          Variables related to the light properties. Most of them used by
          atmospheric attenuation models. 

	  \ingroup CONFIGURATION
 */
class Parameters
{
 private:
  //  Control variables
  int    n_max_events; /**< maximum number of events to be simulated */
  int    seed;         /**< seed for the random number generator. If seed = 0*/
                       /**< the program gets the seed from the CPU's clock. */
  bool   out_log;      /**< log output file */
  bool   out_root;     /**< ROOT/CERN output file */
  bool   out_ascii;    /**< Simple ASCII output file */
  bool   wrong_data;   /**< Status of the variables checkout methods */

  bool  _SD_SIM;      /**< flag to select simulation of surface array detector*/
  bool  _SD_DETAILEDSIM;      /**< flag to select the detailed simulation */
  bool  _SD_SIM_MUEM;  /**< flag to simulate electronic for mu and em component*/


  bool  _SD_ELECSIM;  /**< flag to simulate electronic */
  bool  _SD_ELECSIM_MUEM;  /**< flag to simulate electronic for mu and em component*/

  bool _SD_NEWELECSIM;
  bool _SD_TRIGSIM;
  bool _SD_CTSIM;



  bool  _FD_SIM;      /**< flag to select simulation of fluorescence detector */
  int   _DEBUG_LEVEL; /**< debug level to print */
  int   _SD_DEBUGLEVEL; /**< debug level to print */

  int   _SD_ELECSIMDEBUGLEVEL; /**< debug level to print */


  bool  _GB_READSHOWER;  /**< read the shower from input file */
  bool  _GB_WRITESHOWER;  /**< write the shower from input file */
  bool  _GB_READEVENT;   /**< read the event from input file */
  bool  _GB_WRITEEVENT;  /**< write the event in the output file */

  bool  _GB_READOBSERVATORY;
  bool  _GB_WRITEOBSERVATORY;

  bool  _SD_READPARTICLES; /**< read particle in hit stations */
  bool  _SD_READPHOTOEL;   /**< read photoelectrons in hit stations */
  bool  _SD_READFADCTRACES;      /**< read fadc traces */

  bool  _SD_PRODPARTICLES; /**< produce particle in hit stations */
  bool  _SD_PRODPHOTOEL;   /**< produce photoelectrons in hit stations */
  bool  _SD_PRODFADCTRACES;      /**< produce fadc traces */
 
  bool  _SD_WRITEPARTICLES; /**< write particle in hit stations */
  bool  _SD_WRITEPHOTOEL;   /**< write photoelectrons in hit stations */
  bool  _SD_WRITEFADCTRACES; /**< write fadc traces */

  bool _SD_SAVEMEMORY;/**< remove unnedded elements as soon as possible */

  bool _SD_RANDOMSEED;
  bool _SD_RANDOMPOSITION;

  // Shower Variables
  int    shower_type;
  bool   generate_energy;
  std::string task_name;
  double en_exp;
  double alpha;
  double radius;
  double chi_sig;
  double theta_max;
  int    n_cos;
    
  // surface array (_SD...)


  bool _SD_FARRAY;
  bool _SD_EARRAY;
  bool _SD_SHOWERARRAY;

  int    _SD_N_STAT_ARRAY;
  double _SD_EAST_MIN_ARRAY;
  double _SD_NORTH_MIN_ARRAY;

  double _GL_CORENORTHING;
  double _GL_COREEASTING;

  double _SD_DELT_DIST;
  double _SD_DELT_AZI;
  double _SD_DISTMAX_STATION;
  double _SD_DISTMIN_STATION;
  int    _SD_N_CLONE;
  int    _SD_PART_SAMPLING;
  
  double _SD_ENER_MUCALIB;
  double _SD_ENER_ELCALIB;
  double _SD_ENER_PHCALIB;

  double _SD_WEIGHT_MUCALIB;
  double _SD_WEIGHT_ELCALIB;
  double _SD_WEIGHT_PHCALIB;


  // event generator global variables (_GL...)
  double _GL_D_NORTH;
  double _GL_D_EAST;
  double _GL_NORTH_MIN;
  double _GL_EAST_MIN;
  int    _GL_N_EVENT;
  int    _GL_I_PRINT;
  double _GL_WEIGHT;

  // Site Variables
  int    n_eyes;
  double *x_eye;
  double *y_eye;
  double *z_eye;
  double *phi_eye;
  double *angle_eye;
  double *elev_eye;
  int    n_array_pts;
  double *x_pt;
  double *y_pt;
  
  // Detector Variables
  // Mirror actually here is the diameter of the diaphragm in Schmidt optics  
  double r_diaphr;
  double area_diaphr;
  double noise;
  double rp_min;
  double d_cut;
  double dt;
  double sig_noi;
  int    n_pxl_trigger;
  
  // Variables for noise studies
  double r_min;
  double r_max;
  double phi_min;
  double phi_max;
  
  // Light Variables
  bool   detail_light;
  double l_mie;
  double h_mie;
  double mean_path;
  double const_electron;
  double efficiency;
  double a_lambda;
  double x_rayleigh;
  double background;
  double fluoryield;
  
 public: 
  //! Constructor of the Parameters class.
  Parameters (const char* FileName);
  //! Destructor of the Parameters class.
  ~Parameters ();

  // Control functions
  //! Control function to set maximum number of the events to be simulated.
  void SetNMaxEvents( int nme ) { n_max_events = nme; }
  //! Control function to get maximum number of the events to be simulated.
  int GetNMaxEvents()     const { return n_max_events; }
  //! Control function to set the seed for the random-numbers generator. If seed = 0, the program gets the seed from the CPU's clock
  void SetSeed( int s ) { seed = s; }
  //! Control function to get the seed used in the random-numbers generator.
  int GetSeed()           const { return seed; }
  //! Use this control function passing TRUE as argument to get a logfile.
  void SetOutLog( bool sol ) { out_log = sol; }
  //! Control function to get the switch of logfile.
  bool GetOutLog()        const { return out_log; }
  //! Use this control function passing TRUE as argument to get a rootfile.
  void SetOutRoot( int sor ) { out_root = sor; }
  //! Control function to get the switch of rootfile.
  bool GetOutRoot()       const { return out_root; }
  //! Use this control function passing TRUE as argument to get an asciifile.
  void SetOutAscii( int soa ) { out_ascii = soa; }
  //! Control function to get the switch of asciifile.
  bool GetOutAscii()      const { return out_ascii; }
  //! Use this control function (with TRUE) to set the status of the parameters as wrong.
  void SetWrongData( int swd ) { wrong_data = swd; }
  //! Control function to get the result of the internal test of the parameters.
  bool GetWrongData()     const { return wrong_data; }

 
  int N_STAT_ARRAY() const {return _SD_N_STAT_ARRAY;}
  double EAST_MIN_ARRAY()const{return _SD_EAST_MIN_ARRAY;}
  double NORTH_MIN_ARRAY()const{return _SD_NORTH_MIN_ARRAY;}
  bool SD_FARRAY()const{ return _SD_FARRAY;}
  bool SD_EARRAY()const{return _SD_EARRAY;}
  bool SD_SHOWERARRAY()const{return _SD_SHOWERARRAY; }




  double D_NORTH() const {return _GL_D_NORTH;}
  double D_EAST() const {return _GL_D_EAST;}
  double NORTH_MIN() const {return _GL_NORTH_MIN;}
  double EAST_MIN() const {return _GL_EAST_MIN;}
  unsigned int N_EVENT() const {return _GL_N_EVENT;}
  unsigned int I_PRINT() const {return _GL_I_PRINT;}
  double GLOB_WEIGHT() const {return _GL_WEIGHT;}
  double DELT_DIST() const {return _SD_DELT_DIST;}
  double DELT_AZI() const {return _SD_DELT_AZI;}
  double DISTMAX_STATION() const {return _SD_DISTMAX_STATION;}
  double DISTMIN_STATION() const {return _SD_DISTMIN_STATION;}
  unsigned int N_CLONE() const {return _SD_N_CLONE;}
  unsigned int PART_SAMPLING() const {return _SD_PART_SAMPLING;}
  double ENER_MUCALIB() const {return _SD_ENER_MUCALIB;}
  double ENER_ELCALIB() const {return _SD_ENER_ELCALIB;}
  double ENER_PHCALIB() const {return _SD_ENER_PHCALIB;}
  double WEIGHT_MUCALIB() const {return _SD_WEIGHT_MUCALIB;}
  double WEIGHT_ELCALIB() const {return _SD_WEIGHT_ELCALIB;}
  double WEIGHT_PHCALIB() const {return _SD_WEIGHT_PHCALIB;}


  bool SD_SIM() const {return _SD_SIM;}
  bool SD_DETAILEDSIM() const {return _SD_DETAILEDSIM;}
  bool SD_SIM_MUEM() const {return _SD_SIM_MUEM;}

  bool SD_ELECSIM() const {return _SD_ELECSIM;}
  bool SD_ELECSIM_MUEM() const {return _SD_ELECSIM_MUEM;}

  bool SD_NEWELECSIM() const {return _SD_NEWELECSIM;}
  bool SD_TRIGSIM() const {return _SD_TRIGSIM;}
  bool SD_CTSIM() const {return _SD_CTSIM;}


  bool  GB_READSHOWER()const{return _GB_READSHOWER;}  
  bool  GB_WRITESHOWER()const{return _GB_WRITESHOWER;}  
  bool  GB_READEVENT()const{return _GB_READEVENT;}     
  bool  GB_WRITEEVENT()const{return _GB_WRITEEVENT;}    

  bool  GB_READOBSERVATORY()const{return _GB_READOBSERVATORY;}     
  bool  GB_WRITEOBSERVATORY()const{return _GB_WRITEOBSERVATORY;}     


  bool  SD_READPARTICLES()const{return _SD_READPARTICLES;}  
  bool  SD_READPHOTOEL()const{return _SD_READPHOTOEL;}
  bool  SD_READFADCTRACES()const{return _SD_READFADCTRACES;}

  bool  SD_PRODPARTICLES()const{return _SD_PRODPARTICLES;}
  bool  SD_PRODPHOTOEL()const{return _SD_PRODPHOTOEL;}
  bool  SD_PRODFADCTRACES()const{return _SD_PRODFADCTRACES;}  
 
  bool  SD_WRITEPARTICLES()const{return _SD_WRITEPARTICLES;}  
  bool  SD_WRITEPHOTOEL()const{return _SD_WRITEPHOTOEL;}
  bool  SD_WRITEFADCTRACES()const{return _SD_WRITEFADCTRACES;}  
  bool  SD_SAVEMEMORY() const{return _SD_SAVEMEMORY; } 


  bool SD_RANDOMSEED() const{ return _SD_RANDOMSEED;}
  bool SD_RANDOMPOSITION()const{return _SD_RANDOMPOSITION;}

  double GL_CORENORTHING() const{ return _GL_CORENORTHING;}
  double GL_COREEASTING() const{return _GL_COREEASTING; }



  bool FD_SIM() const {return _FD_SIM;}
  int DEBUG_LEVEL() const {return _DEBUG_LEVEL;}
  int SD_DEBUGLEVEL() const {return _SD_DEBUGLEVEL;}
  int SD_ELECSIMDEBUGLEVEL()const{ return _SD_ELECSIMDEBUGLEVEL;}  

    
  // Shower functions
  //! Function to set the type of the shower: 
  /*!
   *    Types are:
   *  - shower_type = 0\n
   *    shower profile will be generated
   *  - shower_type = 1\n
   *    shower profile will be read from AIRES
   *  - shower_type = 2\n
   *    shower profile will be read from CORSIKA (NOT IMPLEMENTED YET!) 
   */  
  void SetShowerType ( int st ) { shower_type = st; }
  //! Function to get the type of the shower: 
  /*!
   *    Types are:
   *  - shower_type = 0\n
   *    shower profile will be generated
   *  - shower_type = 1\n
   *    shower profile will be read from AIRES
   *  - shower_type = 2\n
   *    shower profile will be read from CORSIKA (NOT IMPLEMENTED YET!) 
   */  
  int GetShowerType() const { return shower_type; }
  //! Use this funtion to set the taskname of the AIRES/CORSIKA shower. 
  void SetTaskName( std::string tn ) { task_name = tn; }
  //! Use this funtion to get the taskname of the AIRES/CORSIKA shower. 
  std::string GetTaskName()    const { return task_name; }
  //! In the case of shower_type = 0(GENERATE), use this function to allow a fluctuation of the primary energy.
  void SetGenerateEnergy ( bool ge ) { generate_energy = ge; }
  //! In the case of shower_type = 0(GENERATE), use this function to get the information if fluctuation of the primary energy is allowed.
  bool GetGenerateEnergy() const { return generate_energy; }
  //! Use this function to set the exponent of the primary particle energy, where Energy = 10^en_exp (in eV).  
  void SetEnExp ( double eex ) { en_exp = eex; }
  //! Use this function to get the exponent of the primary particle energy, where Energy = 10^en_exp (in eV).  
  double GetEnExp() const { return en_exp; }
  //! Use this function to set the parameter constant to calculate the fluctuation in the value of the primary energy.
  void SetAlpha ( double a ) { alpha = a; }
  //! Use this function to get the parameter constant to calculate the fluctuation in the value of the primary energy.
  double GetAlpha() const { return alpha; }
  //! Use this function to set the parameter constant (in g/cm2) to calculate the distribution of chi_max. 
  void SetChiSig ( double cs ) { chi_sig = cs; }
  //! Use this function to get the parameter constant (in g/cm2) to calculate the distribution of chi_max. 
  double GetChiSig() const { return chi_sig; }
  //! This function set the maximum shower theta (in deg) to be simulated.
  void SetTetaMax ( double tm ) { theta_max = tm; } 
  //! This function gets the maximum shower theta (in deg) to be simulated.
  double GetThetaMax() const { return theta_max; }
  //! Use this function to set the parameter used in the distribution of the simulated theta shower.
  void SetNCos ( int nc ) { n_cos = nc; }
  //! Use this function to set the parameter used in the distribution of the simulated theta shower.
  int GetNCos() const { return n_cos; }
  //! Use this function to set the radius (in meters) to distribute the simulated showers from the barycenter of the site. 
  void SetRadius (double r) { radius = r; }
  //! This function gets the radius (in meters) to distribute the simulated showers from the barycenter of the site.
  double GetRadius () const { return radius; }

  
  // Site functions
  //! Site configuration function used to set the number of eyes.
  void SetNEyes(int ne) { n_eyes = ne; }
  //! Site configuration function used to get the number of eyes.
  int GetNEyes() const { return n_eyes; }
  //! Site configuration function used to set the x coordinate (meters) of the eyes.
  void SetXEye(double *xe) { *x_eye = *xe; }
  //! Site configuration function used to get the x coordinate (meters) of the eyes.
  double GetXEye(const int& i) { return x_eye [i]; }
  //! Site configuration function used to set the y coordinate (meters) of the eyes.
  void SetYEye(double *ye) { *y_eye = *ye; }
  //! Site configuration function used to get the y coordinate (meters) of the eyes.
  double GetYEye(const int& i) { return y_eye [i]; }
  //! Site configuration function used to set the z coordinate (meters) of the eyes.
  void SetZEye(double *ze) { *z_eye = *ze; }
  //! Site configuration function used to get the z coordinate (meters) of the eyes.
  double GetZEye(const int& i)  { return z_eye [i]; }
  //! Site configuration function used to set the azimuth angle (deg) of the eyes.
  void SetPhiEye(double *pe) { *phi_eye = *pe; }
  //! Site configuration function used to get the azimuth angle (deg) of the eyes.
  double GetPhiEye(const int& i) { return phi_eye [i]; }
  //! Site configuration function used to set the cover angle (deg) of the eyes.
  void SetAngleEye(double *ae) { *angle_eye = *ae; }
  //! Site configuration function used to get the cover angle (deg) of the eyes.
  double GetAngleEye(const int& i) { return angle_eye [i]; }
  //! Site configuration function used to set the elevation angle (deg) of the eyes.
  void SetElevEye(double *el) { *elev_eye = *el; }
  //! Site configuration function used to get the elevation angle (deg) of the eyes.
  double GetElevEye(const int& i) { return elev_eye [i]; }
  //! Site configuration function used to set the number of vertices of the border of the site.
  void SetNArrayPts(int nap) { n_array_pts = nap; }
  //! Site configuration function used to get the number of vertices of the border of the site.
  int GetNArrayPts() const { return n_array_pts; }
  //! Site configuration function used to set the x coordinate (meters) of the vertices. 
  void SetXPt(double *xp) { *x_pt = *xp; }
  //! Site configuration function used to get the x coordinate (meters) of the vertices. 
  double GetXPt(const int& i) { return x_pt [i]; }  
  //! Site configuration function used to set the y coordinate (meters) of the vertices. 
  void SetYPt(double *yp) { *y_pt = *yp; }
  //! Site configuration function used to get the y coordinate (meters) of the vertices. 
  double GetYPt(const int& i) { return y_pt [i]; }  

  
  // Detector functions

  void SetRDiaphr( double rd) { r_diaphr = rd; }

  double GetRDiaphr() const { return r_diaphr; }

  void SetAreaDiaphr( double ad) { area_diaphr = ad; }

  double GetAreaDiaphr() const { return area_diaphr; }

  void SetNoise( double no) { noise = no; }

  double GetNoise() const { return noise; }

  void SetRPMin( double rpm) { rp_min = rpm; }

  double GetRPMin() const { return rp_min; }

  void SetDCut( double dc) { d_cut = dc; }

  double GetDCut() const { return d_cut; }

  void SetDt( double d) { dt = d; }

  double GetDt() const { return dt; }
  //! Detector parameter function used to set the relation between signal and noise.
  void SetSigNoi( double sn ) { sig_noi = sn; }
  //! Detector parameter function used to get the relation between signal and noise.
  double GetSigNoi() const { return sig_noi; }
  //! Detector parameter function used to set the number of pixels for trigger.
  void SetNPxlTrigger( int npt ) { n_pxl_trigger = npt; }
  //! Detector parameter function used to get the number of pixels for trigger.
  int GetNPxlTrigger() const { return n_pxl_trigger; }

  
  // Noise functions

  void SetRMin ( double rm ) { r_min = rm; }

  double GetRMin() const { return r_min; }

  void SetRMax ( double rm ) { r_max = rm; }

  double GetRMax() const { return r_max; }

  void SetPhiMin ( double pm ) { phi_min = pm; }

  double GetPhiMin() const { return phi_min; }

  void SetPhiMax ( double pm ) { phi_max = pm; }

  double GetPhiMax() const { return phi_max; }

  
  // Light functions

  void SetDetailLight( bool dl ) { detail_light = dl; }

  bool GetDetailLight() const { return detail_light; }

  void SetLMie( double lm ) { l_mie = lm; }

  double GetLMie() const { return l_mie; }

  void SetHMie( double hm ) { h_mie = hm; }

  double GetHMie() const { return h_mie; }

  void SetMeanPath( double mp ) { mean_path = mp; }

  double GetMeanPath() const { return mean_path; }

  void SetConstElectron( double ce ) { const_electron = ce; }

  double GetConstElectron() const { return const_electron; }

  void SetEfficiency( double e ) { efficiency = e; }

  double GetEfficiency() const { return efficiency; }

  void SetALambda( double al ) { a_lambda = al; }

  double GetALambda() const { return a_lambda; }

  void SetXRayleigh( double xr ) { x_rayleigh = xr; }

  double GetXRayleigh() const { return x_rayleigh; }
  //! Light variable function used to set the mean background light.
  void SetBackground( double b ) { background = b; }
  //! Light variable function used to get the mean background light. 
  double GetBackground() const { return background; }
  //! Light variable function used to set the mean fluorescence yield.
  void SetFluoryield( double f ) { fluoryield = f; }
  //! Light variable function used to get the mean fluorescence yield.
  double GetFluoryield() const { return fluoryield; }  
  
  
  //! Function to output the parameters for log and test purposes.
  void Output(ostream& Out);
  
};

#endif // __PARAMETERS_H


//==End of File=============================================================























