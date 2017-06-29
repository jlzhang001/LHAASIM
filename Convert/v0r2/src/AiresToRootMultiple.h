#ifndef AIRESTOROOT_H
#define AIRESTOROOT_H

// System Headers
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>

// ROOT Headers
#include "root.h"

#include "IOAires.h"
#include "HeaderAires_ROOT.h"
#include "PartGrnd_ROOT.h"
#include "ShowerProf_ROOT.h"
#include "Utils.h"


const int MAXOBSLEVELS = 510;
const int MAXSHOWERS= 20;
// Classes
//! Definition of the AiresToRoot application class ATR_App.
/*!
 * This class defines the AiresToRoot application and it contains all the 
 methods used throughout this converter. Almost all the ATR_App methods  
 were adapteded from the first converter version (credits Xavier Bertou and Sylvie 
 Dagoret-Campagne). 
 */
class ATR_App
{
 private:
  string           CurrentFile;
  string           InputFile;
  string           OutputFile;
  string           ProfileFile;
  string           OutputProfileFile;
  Int_t            compress;
  bool             output_profile;
  PartGrnd_ROOT    *pcle;
  ShowerProf_ROOT  *shw_prof;
  TFile            *f1[MAXSHOWERS+1];
  TFile            *f2;
  HeaderAires_ROOT *shA[MAXSHOWERS+1];
  Header_ROOT      *sh[MAXSHOWERS+1];
  TTree            *tree[MAXSHOWERS+1]; 
  
  void  ShowOptions ();
  void  WriteHeader (ostream&);
  void  ConvertGrndPart( );
  void  ConvertGrndPartnew( );
  void  ConvertProfile( );

 public:
  // Constructor of the application 
  ATR_App (int argc, char *argv[]);
  // Destructor of the application
  ~ATR_App ( );
  // Function to run the program
  void Run ( );

};


#endif // __AIRESTOROOT_H






































