	QUICK GUIDE TO START WITH EASYSIM


INSTALLATION

1. Other soft needed:
   a. ROOT : any version 
   b. CMT ( for example v1r12p20020606)
   c. CDAS :  the present version uses IoSd v2r6, but it can be updated
   d. (needed to conversion to CDAS format)
   e. Aires library (libAires.a)

2. Environment variables to be correctly defined
   a. ROOTSYS, PATH and  LD_LIBRARY_PATH as usual with ROOT
   b. AIRESLIB
   c. CMTPATH


3. Get EASYSIM  from Auger CVS in Lyon

4. Compilation

   "cd  EASYSIM/EasySim/v5r1/cmt"
   "cmt broadcast cmt config"
   "cmt broadcast gmake"

5.Running 
	   You will find an example of script in the folder 
	    "EASYSIM/EasySim/v5r1/data" the file "run0"
	o The first step is conversion CorsToRoot or AiresToRoot.
	   WARNING!!!! with the present version of CorsToRoot, ONLY 2
	   ARGUMENTS ARE NEEDED, the file for groundparticles, and the
	   output file . YOU DONT NEED THE .long FILE

	o second step is run EasySim
	  In the same folder you will need to update the file
	   "default.inp" according to your needs
	    it will be read by EasySim

	o You may need to run easysim2cdas to convert to CDAS format, 
	  or can run "anamuons.exe", define an object " Analyze" and 
	  use the  functions defined in Analyze.h. 




**************************************************************************


FILL INPUT FILE

The input  file is on /EASYSIM/EasySim/v5r1/data/default.inp

THE FIRST PART IS FOR ALL MODES OF SIMULATION

* mode: 
	o SHOWER  
	shower simulations reading a root file  "filename.root"
	the output file will be "Sim_filename.root"
      
	o CALIB
	 injection of particles in a single tank ,  there is one event
	 per particle injected
	 the output file name is given by the program

* simmode :
	o  DETAILED ( mode currently used )
	  the detail of the simulation of particle and follow up of photons is simulated
	o FAST  ( not anymore implemented in the present version)
	o SAMPLE 
	 stops the simulation at the stage of the sampling of particles in tanks

* muemmode
	o MUEM
	   give in addition to the usual traces, separate traces for muonic et electromagnetic components
	o DEFAULT
	  give only the usual traces

* elecmode
	o DEFAULT
	  give the time profile of particules for each PMT and the final traces for all channels
	o PM
	  give in addition the profiles in time after PM simulation for each channel
	o FULL
	   give all  profiles for the intermediate steps of the
	   electronic simulation, after PM and after Front End, in
	   addition of default mode
	o SHOWSAT
	  give additionnal FADC traces where the saturation due to coding is not applied.

* number of events
	o for showers, number of time the same shower is simulated 
	o for calib , number of particules entering a tank


THIS SECOND PART IS RELEVANT ONLY IN CALIB MODE BUT IS ALWAYS READ BY THE PROGRAM

* runnumber

* partmode	

	o VEM		
	  only vertical and central muons of 1 GeV are injected
	o SCINTILLATOR
	  position and size  of scintillators are read in Calib.h and
	  the trajectory of particles injected  are calculated 
	o FIXEDTHETA
	   a flux of particles is injected with a given theta chosen
	   below and with phi all around the tank
	o HORIZONTAL
	  flux of horizontal muons all around the tank, inject ad mid height
	o RANDOM
	  flux of muons follwing a sin2theta distribution

* partcode
	o 1 for photons
	o 2 for electrons
	o 3 for muons

* partenergy
	o energy in GeV , if equal to zero, it will be an energy
	distribution following a spectrum defined in "Calib.h"

* parttheta
	o theta of particles relevant only in FIXEDTHETA mode

* partaddmode
	o DEFAULT  
	   only per particle par event
	o DOUBLE
	  2 particles of the same kind an energy are simulated in a
	  event, the second particle is delayed of a random time between
	  0 and 3 microseconds ( the ToT window)
	o MULTIPLE
	  N particles of same energy and kind is simulated at the same 
	  time,the number of particles is read below
	
* npartmultiple
	o see above , relevant only in MULTIPLE mode



***************************************************************************
IN CALIB MODE, THE FILE IS NOT READ BELOW THIS
THIS THIRD PART IS RELEVANT ONLY IN SHOWER MODE 

o arraymode
	o READFILE
	   reads a file ( name given above ) with one line per station 
	   ( id,northing, easting)
	o DEFAULTSOUTH
	   creates a default file with the south grid
	   the number of stations is given below in the field arrayfilename
	o DEFAULTNORTH
	   creates a default file with the north grid
	   the number of stations is given below in the field arrayfilename
	o STARARRAY
	   the array is concentric crowns of stations in the shower plane
	   in that case, only 1 event can be simulated per file , 
	   and it has to be in readaverage position mode, centered at 0,0, with no spread.

o arrayfilename

	in READFILE mode, name of the file to read with stations id and positions
	in  DEFAULT mode, give an integer that will be the number of stations 

o eventmode
	choice of the shower core	
	o DEFAULT 
	  choose a random position in the array , on a range given by the spead (see below)
	o READAVERAGEPOS
	  an average position is read ( see below)

o acceptance mode 
       should be set to 0, mode base on hexagones for specifics studies

o easting(northing) average
       position  of the core, relevant only in READAVERAGEPOS mode

o easting(northing) spread
      in READAVERAGEPOS mode, spread in distance around the average
      in DEFAULT mode , value between 0 and 1, if it is one , the core
      can go all the way to the border of the array. 
      This area is reduced by the spread factor in easting and northing

o Rotation of the shower
     angle in degree if we want to rotate the shower , relevant only
     for not too inclined shower, where geomagnetic would not be taken
     into account properly.

o rmin	
    distance in meters below which the  stations are not taken into account

o rmax
     distance in meters above which the stations are not taken into account

o delta_r	
     radius for the definition of  the sampling zone
     usually 0.15 is used
o delta_azi
     azimuth for the definition of the sampling zone
     usually 20 is used



Most parameters that need currently to be changed are in this "default.inp" file. Meanwhile, some other parameters are in the other files are in header files and can be changed. In that case, the program will have to be compiled again.
Here is a summary of the header files where parameters are initialized

 Constants.h
	 o general universal constants
	 o Auger Constants  
	 o altitude Auger
	 o stations radius and size
	 o tanks Constants
	 o Number of PMTs
	 o radius of PMTs
	 o position of the core of the PMT sphere above water
	 o positions of the PMTs in the tanks
	 o Simulations constants
	 o Number of clones for sampling in tanks
	 o Number of minimal particles for resampling close to core
	 o Constants related to traces definition
	 o Bin numbers , sampling time
	 o Particles Constants
	 o mass and energy loss of particles
	 o cherenkov constants 
	 o delta rays constants

BuildProcessesTables.h
	o Water related constants (index, absorption)
	o Tyvek related constants
	o Tabulations for simulations of processes in water
	o PMT efficiencies


Calib.h
	o Positions of scintillators for calibration in SCINTILLATOR MODE
	o Arrays of different tabulations for energy spectrum

ElecConstants.h
	o Constants for PMT simulation	( gain, dynode/anode)
	o Constants for FE simulation


HERE IS A SUMMARY OF THE PARAMETERS WRITTEN IN ROOT FILES
Each root event is an object of the class event, that is defined with
the following attributes 
In SHOWERARRAY mode, objects written are HitStation , only one event
can be simulated each time. This allows to simulate a very large number of
stations (3000 works)


class Event : public TObject
{
 
 public:

  Int_t fId;		/ Id of event
  Int_t fPrimary;	/ code for primary
  Double_t fEnergy;	/ shower energy
  Double_t fTheta;	/ zenith angle of shower
  Double_t fAzim;	/ azimuth angle of shower
  Double_t fNorCore;	/ Core northing
  Double_t fEasCore;	/ Core Easting
  Double_t fPhiRot;	/ POssible rotation of the shower in EasySim
  Double_t fXCore;	/ Core X  coordinate in cartesian
  Double_t fYCore;	/ Core Y  coordinate in cartesian
  Double_t fXmax;	/ Xmax of shower
  Double_t fX0;		/ X0 of shower
  Int_t fNombTank;	/ Number of Tanks hit by showers
  Int_t fT3Algo;	/ Code of T3 algorithm
  Int_t fT4Algo;	/ code of T4 algo
  vector<HitStation> fHitStationList;	/List of stations with particles

}




Class Station
{
 
 public:
  
  Int_t fId;		
  string fName;
  Float_t fNorthing;
  Float_t fEasting;
  Float_t fAltitude; 
  Float_t fX_sf;     //coordinates of station is shower frame
  Float_t fY_sf;
  Float_t fZ_sf;
  Float_t fR_sf;	// distance to shower	 axis
  Float_t fAzim_sf;	// azimut in shower frame

}

class SampStation : public Station
{
	
 public:	
		
  Int_t fNmu;		// number of muons entering the tank
  Int_t fNph;		// number of photons entering the tank
  Int_t fNel;		// number of electrons entering the tank
  Int_t fNPartTot;	// number of particles entering the tank
  vector<Particle> fPartList;	  list of particles
  
 }

class HitStation : public TObject,public SampStation
{
 
 public:

  Int_t fT2ToT;			// flag for T2ToT
  Int_t fT1Threshold;		// flag for T1 threshold
  Int_t fT2Threshold;		// flag for T2 threshold
  Int_t fTriggerBin;		// Bin of trigger
  Int_t fFirstPelTime;		// Time of first phoelectron
  Int_t fFirstPelBin;		// Bin of first photoelectron
  Double_t fTime0;		// Start time
  Int_t fNtop;			// Number of particles entring by top
  Int_t fNside;			// Number of particles entring by side   
  Float_t fNpe;			// Number of photoelectrons in Station
  Float_t fNpe_direct;		// Number of photoelectrons from direct light
  Float_t fNpe_mu;		// number of pe from incident muons
  Float_t fNpe_ph;		// number of pe from incident  photons
  Float_t fNpe_el;		// number of pe from incident electrons
  Double_t fSignalInVem;	// signal in VEM
  Double_t fSampFact;		// sampling factor when tank close to core
  Int_t fNbOfBinsOverThres;	// Number of bins over threshold
  vector<Int_t> fMuTimes;	// Arrival time of muons in tank
  map<Int_t,Double_t> fTimeProfile;	   //time profile of pe
  map<Int_t,Double_t> fTimeProfile_mu;	   //time profile of pe from mu
  map<Int_t,Double_t> fTimeProfile_em;	   // time profile of pe from em
  short fADC[2][MAXNUMBEROFADCBINS];	   //ADC Trace 
  short fADC_ns[MAXNUMBEROFADCBINS];	   //ADC Trace without ADC saturation
  short fADC_mu[2][MAXNUMBEROFADCBINS];	   //ADC trace from mu
  short fADC_em[2][MAXNUMBEROFADCBINS];	   //ADC trace from el
  Double_t fDoubleADC[2][MAXNUMBEROFADCBINS];
  Double_t fDoubleADC_mu[2][MAXNUMBEROFADCBINS];
  Double_t fDoubleADC_em[2][MAXNUMBEROFADCBINS];
  PMT fPMT[NPM];			

}

The PM class has the same attributes as  the HitStation and also additionnal
intermediate containers for  steps in the electronics simulation

class PMT
{
 
 public:

  Float_t fNpe;
  Float_t fNpe_direct;
  Float_t fNpe_mu;
  Float_t fNpe_ph;
  Float_t fNpe_el;
  Int_t fFirstPelTime;
  Int_t fFirstPelBin;
  Int_t fFirstADCBin;
  Double_t fSampFact;
  map<Int_t,Double_t> fTimeProfile;	
  map<Int_t,Double_t> fTimeProfile_mu;
  map<Int_t,Double_t> fTimeProfile_em;
  map<Int_t,Double_t> fPMTSignal_hi;		//PMT signal high gain
  map<Int_t,Double_t> fPMTSignal_lo;		//PMT signal low gain
  Double_t fFEFilter_hi[MAXNUMBEROFTIMEBINS];	//signal after FE high
  Double_t fFEFilter_lo[MAXNUMBEROFTIMEBINS];	//signal after FE low
  short fADC[2][MAXNUMBEROFADCBINS];
  short fADC_ns[MAXNUMBEROFADCBINS];
  short fADC_mu[2][MAXNUMBEROFADCBINS];
  short fADC_em[2][MAXNUMBEROFADCBINS];
}


