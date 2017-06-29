//==============================================================================
//
// Source-File  : AiresToRoot.cc 
//
// Date         : Mon May 28 2001
//
// Author	: Luis Prado Jr.
//                State University at Campinas.
//
// E-Mail       : pradojr@ifi.unicamp.br
//
// Description  : Root file for the AiresToRoot package. This package was 
//                written upon the first version from Xavier Bertou and Gilles
//                Zalamansky. For details type "AiresToRoot --help" and 
//                read the file README. 
//
//============================================================================== 


// Local Headers
#include "AiresToRoot.h"
#include <string>
#include "TROOT.h"
#include "TFile.h"
//----------------------------------------------------------------------------
// Global variables

TROOT root("sample grdpcles", "Auger sample Grdpcles file");

char PartNtplName[20] = "GrndPcles";
char HeaderName[20]   = "Header_ROOT";
//----------------------------------------------------------------------------


//============================================================================== 
//
// Function name: ATR_App::ATR_App
// Input        : argc, argv
// Output       : -
// Date         : Mon May 28 2001
// Author       : Luis Prado Jr.
//
// Last change  : Mon May 28 2001
// Done by      : Luis Prado Jr.
//
// Status       : Finished and tested
//
// Description  : Compute command line options of this program. 
//
//
//============================================================================== 
ATR_App::ATR_App (int argc, char *argv[])
{
  
  // Print the header
  WriteHeader (cout);

  // Set some default parameters
  compress = 1;
  output_profile = false;
  
  // Check the arguments argc and argv
  if (argc == 1)
    {
      ShowOptions ();
      exit (0);
    }
  
  if (argc == 2)
    {
      string Argument = argv[1];
      if (Argument == "-c")
        {
	  cout << "   Redistribution and use in source and binary forms, with or without" << endl << "   modification, are permitted but we doubt you would like to do so." << endl << endl;
	  exit (0);
        }
      if (Argument == "-w")
        {
	  cout << "   This program is not supposed to do anything wrong." << endl << "   If you find any bug, please, please contact the package owner." << endl << endl;
	  exit (0);
        }
      if (Argument == "--version" || Argument == "-v")
        {
	  cout << VERSION << '.' << REVISION << '\n';
	  exit (0);
        }
      if (Argument == "--help" || Argument == "-h")
        {
	  ShowOptions ();
	  exit (0);
        }
    }
  
  // Get the input filename and check if the file exists
  if (argc >= 2)
    InputFile = argv[1];
  
  //  ifstream Input (string_new_cstring(InputFile), ios::in|ios::nocreate);
  ifstream Input (InputFile.c_str(), ios::in);
  if (!Input)
    {
      cout << "FATAL ERROR: cannot find file: " << InputFile << ".\n";
      Error ("Choose an input file that exists and try again!");
    }

  
  // Compute the associated AIRES file with the longitudinal information
  ProfileFile = InputFile;
  if (string_contains(InputFile,"."))
    {
      ProfileFile=InputFile.substr(0,InputFile.find("."));
    }
  ProfileFile += ".t1291";

  //Compute the default output name for the profile file
  OutputProfileFile = ProfileFile;
  OutputProfileFile += ".root";

  ifstream InputProf (ProfileFile.c_str(), ios::in);
  if (!InputProf)
    {
      cout << "WARNING: cannot find file: " << ProfileFile << ".\n";
      cout << "WARNING: no shower profile information will be converted.\n";
    }
  else
    {
      cout << "AIRES shower profile file: " << ProfileFile << " found!\n";
      output_profile = true;
    }


  string Argument = "";
  // Look if the user specified an output filename and get it
  if (argc >= 4)
    Argument = argv[2];
  
  if (Argument == "-o")
    {
      OutputFile = argv[3];
      if (string_contains(OutputFile,"."))
	{
	  OutputProfileFile = OutputFile.substr(0,OutputFile.find("."));
	}
      OutputProfileFile += ".t1291.root";
    }
  else
    {
      // Compute a default output filename
      OutputFile = InputFile;
      if (string_contains(InputFile,"."))
	{
	  OutputFile=InputFile.substr(0,InputFile.find("."));
	}
      OutputFile += ".root";
      cout << "Use [-o outputfile] to specify the output file name.\n";
      cout << "Using default output file: " << OutputFile << ".\n";
    }
  
  if (OutputFile == InputFile)
    {
      Error ("FATAL ERROR: The names of input and output file are identical!");
    }

  cout << " output file will be " << OutputFile << endl;
  
  //  f1 = new TFile(string_new_cstring(OutputFile), "RECREATE");
  f1 = new TFile(OutputFile.c_str(), "RECREATE");
  f1->SetCompressionLevel(compress);
  
  //  shA = IOAiresOpen(string_new_cstring(InputFile));
  shA = IOAiresOpen(InputFile.c_str());
  sh = shA;
  sh->ProgFlag = AIRES;
  sh->Write(HeaderName);
  IOAiresSummary();

  
} // End of ATR_App::ATR_App


//============================================================================== 
//
// Functionname : ATR_App::~ATR_App
// Input        : -
// Output       : -
// Date         : Mon May 28 2001
// Author       : Luis Prado Jr. 
//
// Last change  : Mon May 18 2001
// Done by      : Luis Prado Jr.
//
// Status       : Finished and tested
//
// Description  : Destructor of ATR_App
//
//============================================================================== 
ATR_App::~ATR_App ()
{
  
}// End of ATR_App::~ATR_App


//============================================================================== 
//
// Function name: ATR_App::Run
// Input        : -
// Output       : -
// Date         : Mon May 28 2001
// Author       : Luis Prado Jr. 
//
// Last change  : Mon May 28 2001
// Done by      : Luis Prado Jr.
//
// Status       : Finished and tested
//
// Description  : Running the AiresToRoot application.
//
//============================================================================== 
void ATR_App::Run ()
{  
  ConvertGrndPart();
  
  if (output_profile)
    ConvertProfile();
  
  cout << "Done!\n";
  
}// End of ATR_App::Run


//============================================================================== 
//
// Function name: ATR_App::ConvertGrndPart()
// Input        : -
// Output       : -
// Date         : Mon May 28 2001
// Author       : Luis Prado Jr.
//
// Last change  : Mon May 28 2001
// Done by      : Luis Prado Jr.
//
// Status       : Finished and tested
//
// Description  : Convert shower profile in ROOT format.
//
//============================================================================== 
void ATR_App::ConvertGrndPart()
{  
  pcle = new PartGrnd_ROOT();
  
  tree = new TTree(PartNtplName, PartNtplName);
  tree->Branch(PartNtplName,"PartGrnd_ROOT",&pcle,128000,1);
  
  int i = 0;
  while (IOAiresNext(pcle)) 
    {
      tree->Fill();
      i++;
      if (i % 10000 == 0)
	cout << ">>>> " << i << " particles written" << endl;
    };
  cout << ">>>> " << tree->GetEntries() << " Particles written to file" 
       << endl << ">>>>" << endl << endl;
  //sh->Write("Shower");

  f1->Write(string_new_cstring(OutputFile));
  f1->Close();
  
  IOAiresShutdown();
    
}// End of ATR_App::ConvertGrndPart()


//============================================================================== 
//
// Function name: ATR_App::ConvertProfile()
// Input        : -
// Output       : -
// Date         : Mon May 28 2001
// Author       : Luis Prado Jr.
//
// Last change  : Mon May 28 2001
// Done by      : Luis Prado Jr.
//
// Status       : Finished and tested
//
// Description  : Convert shower profile in ROOT format.
//
//============================================================================== 
void ATR_App::ConvertProfile()
{
  f2 = new TFile(string_new_cstring(OutputProfileFile), "RECREATE");
  f2->SetCompressionLevel(compress);
  shw_prof = new ShowerProf_ROOT();
  
  
  //int       nobslevels;
  //double    thinegy;
  //double    x1;
  //double    t0;
  //int       fitrc;
  //double    xmax;
  //double    nmax;
  int     idummy;
  char    line[132];
  FILE    *tfile;
  double  x, nx;
    TVector aux_shw_pts(MAXOBSLEVELS);
    TVector aux_log_pts(MAXOBSLEVELS);

  // Reading in the longitudinal profile
  
  tfile = fopen(string_new_cstring(ProfileFile), "r");
  while (fgets(line, 132, tfile ) != NULL)
    {
      if (line[0] != '#')
	{
	  sscanf(line, "%d %lg %lg", &idummy, &x, &nx);
	   aux_log_pts(idummy - 1) = x;
	   aux_shw_pts(idummy - 1) = log(nx);
	}
    }
  fclose(tfile);
    
  
  // shw_prof->SetChi0(0);
  // shw_prof->SetChimax(0);
 
   shw_prof->SetShwPts(&aux_shw_pts);
   shw_prof->SetLogPts(&aux_log_pts);
  
  //  SDC suppress shw_prof->Write(string_new_cstring(OutputProfileFile));
  //  shw_prof->Write("ShowerProf_ROOT");
  shw_prof->Write();
  f2->Close();

#ifdef BETA_VERSION   
   aux_shw_pts.Print(); //Just to control the instantiation
  aux_log_pts.Print(); //Just to control the instantiation
#endif

}// End of ATR_App::ConvertProfile


//============================================================================== 
//
// Function name: ATR_App::ShowOptions
// Input        : -
// Output       : -
// Date         : Mon May 28 2001
// Author       : Luis Prado Jr.
//
// Last change  : Mon May 28 2001
// Done by      : Luis Prado Jr.
//
// Status       : Finished and tested
//
// Description  : Show command line options to run this program.
//
//============================================================================== 
void ATR_App::ShowOptions ()
{
  cout << "\n AiresToRoot Version " << VERSION << '.' << REVISION << '\n';
  cout << " Syntax is:  AiresToRoot inputfile [-o outputfile] \n";
  cout << " or          AiresToRoot [option] \n\n";
  
  cout << " where:\n";
  cout << " inputfile is the AIRES output file for the  ground  particles,\n";
  cout << " usually named as taskname.grdpcles. If the file taskname.t1291 \n";
  cout << " is also present in the  same  directory, AiresToRoot will also \n";
  cout << " convert the shower profile information in ROOT format.\n \n";  
  
  cout << " [option] can be:\n";
  cout << " -o outputfile    Place output in file outputfile\n";
  cout << "                  If the option [-o outputfile] is not \n";
  cout << "                  specified the default output file name\n"; 
  cout << "                  will be taskname.root \n \n";
  cout << " -c               Show copying conditions\n";
  cout << " -w               Show warranty conditions\n";
  cout << " -h or --help     Show this message\n";
  cout << " -v or --version  Show version\n";
  cout << '\n';
  
} // End of ATR_App::ShowOptions


//============================================================================== 
//
// functionname : ATR_App::WriteHeader
// input        : Output
// output       : -
// date         : Mon May 28 2001
// author       : Luis Prado Jr.
//
// last change  : Mon May 28 2001
// done by      : Luis Prado Jr.
//
// status       : Finished and tested
//
// description  : Write a header containing a copyright line and the basic idea
//                of this program. Then write a message that this program is
//                free software and so comes with absolutely no warranty.
//
//============================================================================== 
void ATR_App::WriteHeader (ostream& Output)
{
  Output << '\n';
  Output << "=====================================================================\n";
  Output << " AiresToRoot " << VERSION << '.' << REVISION << "\n";
  Output << " Copyright (C) 2001  Pierre Auger Observatory \n";
  Output << '\n';
  Output << " Converting AIRES showers in ROOT format\n";
  Output << "=====================================================================\n";

  Output << '\n';

#ifdef BETA_VERSION
  Output << "WARNING:  This is a BETA VERSION and you are welcome testing \n";
  Output << "WARNING:  this version. Please send a full report if you have\n";
  Output << "WARNING:  found a bug to: pradojr@ifi.unicamp.br\n";
  Output << "\n\n";
#endif


} // End of ATR_App::WriteHeader


//============================================================================== 
//
// Function name: main
// Input        : argc, argv
// Output       : -
// Date         : Mon May 28 2001
// Author       : Luis Prado Jr.
//
// Last change  : Mon May 28 2001
// Done by      : Luis Prado Jr.
//
// Status       : Finished and tested
//
// Description  : Given the command line parameters `argv[1..argc]' the main
//                function initializes the AiresToRoot application and starts
//                it. If everything works fine the return value is zero.
//                (Note: argv[0] = Name of program that is started.)
//
//============================================================================== 
int main (int argc, char* argv[])
{
  ATR_App Converter ( argc, argv );  
  Converter.Run ( );
  
  return (0);
  
} // End of main


//==End of File=================================================================

















