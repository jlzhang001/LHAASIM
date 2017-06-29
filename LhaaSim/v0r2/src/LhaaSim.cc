//----------------------------------------------------
/*
  LHAASIM Program - IPN Orsay -
  
  

 */
//----------------------------------------------------------------------


#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <time.h>
#include <string>
#include <pthread.h>
#include "LhaaSim.h"
#include "Array.h"
#include "Station.h"
#include "Event.h"
#include "Calib.h"
#include "Utils.h"
#include "ManageEvent.h"
#include "BuildProcessesTables.h"
#include "ShowerParam.h"
#include "Constants.h"


//----------------------------------------------------------------
/*
  global variables
*/
//----------------------------------------------------------------
TROOT root ("Main", "Tank Simulation");	// ROOT workspace
//Array *gCoreArray;
HitStationED *gHStationED;
HitStationMD *gHStationMD;
extern double gMuonEnergyAngularSpectrum[ANGULAR_FLUX_SIZE];
extern double gMuonProbaAngularSpectrum[ANGULAR_FLUX_SIZE];

//----------------------------------------------------------------
/*
  reading config file
*/
void
read_config ()
{
  ifstream cfgfile;
  cfgfile.open ("./default.inp", ios::in);
  // Reading the parameters from default.inp file
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->Mode;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->SimMode;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->MuEmMode;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->ElecMode;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->EDMode;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->NEvents;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->ArrayMode;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->ArrayFileName;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->ArrayMDFileName;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->EventMode;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->Acceptance;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->EasAvrg;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->EasSpread;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->NorAvrg;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->NorSpread;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->PhiRotation;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->RMin;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->RMax;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->DeltaR;
  cfgfile.ignore (100, '\n');
  cfgfile.ignore (100, '\n');
  cfgfile >> theConfig ()->DeltaAzim;
  
  
  
  theConfig ()->DeltaAzim = theConfig ()->DeltaAzim * DEG2RAD;
  
  cfgfile.close ();
}


//----------------------------------------------------------------
/*
  parsing arguments
*/
void
parse_argv (int argc, char **argv)
{
  theConfig ()->NoFileName = theConfig ()->TwoFileName =
    theConfig ()->AsciiFile = 0;
  switch (argc) {
 
  case 2:
    cerr << "\tSelected input file : " << endl << "\t" << argv[1] << endl;
    theConfig ()->InRootFileName = argv[1];
    break;
  case 3:
    cerr << "\tSelected output file: " << endl << "\t" << argv[2] << endl;
    theConfig ()->InRootFileName = argv[1];
    theConfig ()->OutRootFileName = argv[2];
    theConfig ()->TwoFileName = 1;
    break;
  case 4:
    cerr << "\tSelected output file: " << endl << "\t" << argv[2] << endl;
    theConfig ()->InRootFileName = argv[1];
    theConfig ()->OutRootFileName = argv[2];
    theConfig ()->TwoFileName = 1;
    theConfig ()->AsciiFileName = argv[3];
    theConfig ()->AsciiFile = 1;
    break;
  default:
    cerr << " ++++ Too many arguments or too few ++++" << endl;
    cerr << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cerr << "Usage : " << argv[1] << " Grnd-Root-filename " << endl;
    cerr <<
      " \t where Grnt-Root-filename is the ROOT file containing the Ground particles  "
      << endl;
    cerr << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    exit (1);
  }

}


//----------------------------------------------------------------
/*
  Building out file names, ...
*/
void
build_outputs (Event * event)
{
  for (int ii = 0; ii < 80; ii++)
    cerr << "-";
  cerr << endl;
  cerr << "\t\tSHOWER MODE " << endl;
  if (!theConfig ()->TwoFileName) {
    if (theConfig ()->NoFileName == 1) {
      cerr <<
	"Please enter an ouput filename or change the mode of simulation" <<
	endl;
      exit (0);
    }
    theConfig ()->OutRootFileName = theConfig ()->InRootFileName;
    
    while (string_contains (theConfig ()->OutRootFileName, "/")) {
      theConfig ()->OutRootFileName =
	string_after (theConfig ()->OutRootFileName, "/");
    }
    
    if (theConfig ()->SimMode == "FAST")
      theConfig ()->OutRootFileName = "Sim_fast_" + theConfig ()->OutRootFileName;
    else if (theConfig ()->SimMode == "SAMPLE")
      theConfig ()->OutRootFileName = "Sample_" + theConfig ()->OutRootFileName;
    else {
      theConfig ()->OutRootFileName =
	"Sim_" + theConfig ()->OutRootFileName;
      
    }
    
  theConfig ()->OutRootFile =
    new TFile (theConfig ()->OutRootFileName.c_str (), "RECREATE");
  
  // TREE EVENT (OutTreeEvent) :  
  theConfig ()->OutTreeEvent = new TTree ("TreeEvent", "tree for Event objects");
  event = new Event;
  theConfig ()->OutBranchEvent =
    theConfig ()->OutTreeEvent->Branch ("event", "Event", &event, 16000, 0);

  theConfig ()->OutBranchEvent->SetAddress (&event);

  delete event;

  // TREE MD DETECTOR (OutTreeMD) :
  theConfig ()->OutTreeMD = new TTree ("TreeMD", "tree for MD detector");
  gHStationMD = new HitStationMD;
  theConfig ()->OutBranchMD =
    theConfig ()->OutTreeMD->Branch ("hstationMD", "HitStationMD", &gHStationMD, 16000, 0);
  delete gHStationMD;
  
  // TREE ED DETECTOR (OutTreeED) :
  theConfig ()->OutTreeED = new TTree ("TreeED", "tree for ED detector");
  gHStationED = new HitStationED;
  theConfig ()->OutBranchED =
    theConfig ()->OutTreeED->Branch ("hstationED", "HitStationED", &gHStationED, 16000, 0);
  delete gHStationED;
  
  }

}

//----------------------------------------------------------------
/*
  Building out file names, ...
*/

//----------------------------------------------------------------
/*
  main function
*/
int
main (int argc, char **argv)
{
  Event *event = NULL;
  int nt3 = 0, nt4 = 0;
  // initialisations
  cerr << endl;
  for (int ii = 0; ii < 80; ii++)
    cerr << "-";
  cerr << endl << endl;
  cerr << "\t\t\t\tPROGRAM LHAASIM" << endl;
  cerr << "\t\t\t\t+-+-+-+-+-+-+-+" << endl;
  cerr << "\t\tComplete simulation of the LHAASO KM2A detector" << endl;
  float vol = pow( STATION_RADIUS,2) * PI * STATION_HEIGHT;
  float surf = RAD_PM *(RAD_PM-HC_PM) *2 *PI *10000.;
  cerr << "\t\tVolume of each tank = " <<vol<<"  m3"<< endl;
  cerr << "\t\tEach tank has = " <<NPM<<"  PMT"<< endl;
  cerr << "\t\tSurface of each PMT = " <<surf <<"  cm2 " <<endl;
  cerr << endl;
  for (int ii = 0; ii < 80; ii++)
    cerr << "-";
  cerr << endl << endl;
  read_config ();
  parse_argv (argc, argv);
  build_outputs (event);


  
  // Choice of the simulation mode 
  // building some tables

  if (theConfig ()->SimMode == "DETAILED") {
    cerr << "\tDETAILED SIMULATION CHOSEN" << endl << endl;
    BuildProcessesTables ();
  } else if (theConfig ()->SimMode == "SAMPLE") {
    cerr << "\tSIMULATION WILL STOP AFTER SAMPLING" << endl;
  } else {
    cerr << " You have not chosen an existing mode of simulation" << endl;
    exit (0);
  }

  for (int ii = 0; ii < 80; ii++)
    cerr << "-";
  cerr << endl;

  
  
  // read simulated shower
  gShParamP = new ShowerParam (theConfig ()->InRootFileName);
  
  // define array
  cout << "ArrayMode = " << theConfig ()->ArrayMode << " "<< theConfig ()->EventMode << endl; 
  if(theConfig ()->ArrayMode == "KM2A") {  //LHAASO KM2
    gArrayED = new Array (4*atoi (theConfig ()->ArrayFileName.c_str ()),2);
    gArrayMD = new Array (atoi (theConfig ()->ArrayFileName.c_str ()),1);
  }
  else if(theConfig ()->ArrayMode == "READFILE") {
    gArrayED = new Array (theConfig ()->ArrayFileName.c_str ());
    gArrayMD = new Array (theConfig ()->ArrayMDFileName.c_str ());
  }
  else 
    cerr << "Please define an existing array mode" << endl; 
  
  //average position and spreading limits of the events
  if (theConfig ()->EventMode == "DEFAULT") {
    theConfig ()->EasAvrg = gArrayED->fEasMean;
    theConfig ()->NorAvrg = gArrayED->fNorMean;
    theConfig ()->EasSpread *= gArrayED->fEasWidth / 2.;
    theConfig ()->NorSpread *= gArrayED->fNorWidth / 2.;
  }
  
  for (Int_t n = 0; n < theConfig ()->NEvents;) {
    event = GenerateEvent (n);
    n++;
    gShParamP->XCore = event->fXCore;
    gShParamP->YCore = event->fYCore;
    gShParamP->EasCore = event->fEasCore;
    gShParamP->NorCore = event->fNorCore;
    
    vector < SampStation > samp_list_MD;
    vector < SampStation > samp_list_ED;
    
    DoSampling (event->fEasCore, event->fNorCore,1, &samp_list_MD);
    DoSampling (event->fEasCore, event->fNorCore,2, &samp_list_ED);
         
    //cout << "######### Dans LhaaSim, (fEasCore,fNorCore) = (" << event->fEasCore << "," << event->fNorCore << ")" << endl;


    UInt_t samp_size_MD = samp_list_MD.size ();
    for (UInt_t ist = 0; ist < samp_size_MD; ist++) {
      HitStationMD *hst = GenerateHitStationMD(samp_list_MD[ist]);
      if (!hst)
	continue; 
      gHStationMD = hst;
      //  theConfig ()->OutBranchMD->SetAddress (&gHStationMD);
      theConfig ()->OutTreeMD->Fill();
      delete hst;
    }

    UInt_t samp_size_ED = samp_list_ED.size ();
    for (UInt_t ist = 0; ist < samp_size_ED; ist++) {
      HitStationED *hst = GenerateHitStationED(samp_list_ED[ist]);
      if (!hst)
    	continue; 
      gHStationED = hst;
      //  theConfig ()->OutBranchED->SetAddress (&gHStationED);
      theConfig ()->OutTreeED->Fill();
      delete hst;    
    }

    theConfig ()->OutBranchEvent->SetAddress (&event);
    theConfig ()->OutTreeEvent->Fill ();
    cerr << endl << "\t---> Writing event in root output file" << endl;
  
     delete event;
      cerr << endl;
      for (int ii = 0; ii < 80; ii++)
	cerr << "-";
      cerr << endl;
  
  }	//end of loop on events
  
  delete gShParamP;
  
  
  
  // cleanings
  theConfig ()->OutRootFile->Write ();
  cerr << endl;
  for (int ii = 0; ii < 80; ii++)
  cerr << "-";
  cerr << endl;
  cerr << endl << "\t  ROOT FILE WRITTEN =  " << theConfig ()->
    OutRootFileName << endl;
  cerr << endl;
  for (int ii = 0; ii < 80; ii++)
    cerr << "-";
  cerr << endl;
  theConfig ()->OutRootFile->Close ();
  
  
  return 1;
}
