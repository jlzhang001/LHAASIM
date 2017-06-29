//--------------------------------------------------------------------
/*! \file AiresToRootMultiple.cc
    \author Sylvie Dagoret-Campagne according original work of X. Bertou
    \brief Convert Aires file into SDSim input ROOT file
    \date january 2002
*/
//--------------------------------------------------------------------


// Local Headers
#include "AiresToRootMultiple.h"

//----------------------------------------------------------------------------
// Global variables

TROOT root("AiresToRoot", "Convert AIRES Grdpcles file into ROOT format");

char PartNtplName[20] = "GrndPcles";
char HeaderName[20]   = "Header_ROOT";
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
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
      CurrentFile=ProfileFile;
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
	  CurrentFile=OutputFile.substr(0,OutputFile.find("."));
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
      CurrentFile=OutputFile;
      OutputFile += ".root";
      cout << "Use [-o outputfile] to specify the output file name.\n";
      cout << "Using default output file: " << OutputFile << ".\n";
    }
  
  if (OutputFile == InputFile)
    {
      Error ("FATAL ERROR: The names of input and output file are identical!");
    }
 


  if(!IOAiresSimpleOpen(InputFile.c_str()))
    {
      cerr << " Input Aires File " << InputFile.c_str() << " Not open " << endl;
      exit(1);
    }
  else
    IOAiresSummary();

  /*
  f1 = new TFile(OutputFile.c_str(), "RECREATE");
  f1->SetCompressionLevel(compress);
  

  shA = IOAiresOpen(InputFile.c_str());
  sh = shA;
  sh->ProgFlag = AIRES;
  sh->Write(HeaderName);
  
  */
  
} // End of ATR_App::ATR_App


//============================================================================

ATR_App::~ATR_App ()
{
  
}// End of ATR_App::~ATR_App


//============================================================================


void ATR_App::Run ()
{  
  ConvertGrndPartnew();
  
  if (output_profile)
    ConvertProfile();
  
  cout << "Done!\n";
  
}// End of ATR_App::Run


//=============================================================================


void ATR_App::ConvertGrndPart()
{  
  HeaderAires_ROOT *sh;
  pcle = new PartGrnd_ROOT();
  int showernum=0;
  
  tree[0] = new TTree(PartNtplName, PartNtplName);
  tree[0]->Branch(PartNtplName,"PartGrnd_ROOT",&pcle,128000,1);

  
  
  int i = 0;
  int record;
  while (record=IOAiresNext(pcle)) 
    {
      if(record==PARTICLERECORDTYPE)
	{
	  tree[showernum]->Fill();
	  i++;
	  if (i % 10000 == 0)
	    cout << ">>>> " << i << " particles written" << endl;
	}
      else if (record==STARTOFSHOWERRECORDTYPE) // process the start Of shower
	{
	cout << " \t STARTOFSHOWERRECORDTYPE " << endl;
	showernum++;
	if(showernum>0)
	  {
	    // open a new file
	    OutputFile=CurrentFile;
	    char showerstring[10];
	    sprintf(showerstring,"%d",showernum);
	    OutputFile+="_";
	    OutputFile+=showerstring;
	    OutputFile+=".root";
	    //tree->Delete("");
	    // tree->Delete("all");
	    //delete tree;
	    tree[showernum] = new TTree(PartNtplName, PartNtplName);
	    tree[showernum]->Branch(PartNtplName,"PartGrnd_ROOT",&pcle,128000,1);

	    cout << " Open a new ROOT file " << OutputFile << endl;

	    char c; cin >> c;

	    f1[0]=new TFile(OutputFile.c_str(),"RECREATE");
	    sh= new HeaderAires_ROOT();
	    sh->ProgFlag= AIRES;
	    ReReadAiresBeginOfShower(sh);
	    
	  }
	}
      else if (record==ENDOFSHOWERRECORDTYPE) // process the end of shower
	{
	cout << " \t ENDOFSHOWERRECORDTYPE " << endl;
	if(showernum==0)
	  {
	    cout << ">>>> " << tree[0]->GetEntries() << " Particles written to file" << OutputFile.c_str() << endl << ">>>>" << endl << endl;
	    f1[0]->Write(OutputFile.c_str());
	    f1[0]->Close();
	    cout << OutputFile.c_str() <<" file closed " << endl; 
	    //tree->Delete("");
	    //tree->Delete("all");
	    //delete tree;
	   
	  }
	else
	  {
	    ReReadAiresEndOfShower(sh);
	    cout << ">>>> " << tree[0]->GetEntries() << " Particles written to file" << OutputFile.c_str() << endl << ">>>>" << endl << endl;
	    //sh->Write(HeaderName);
	    f1[0]->Write(OutputFile.c_str());
	    f1[0]->Close();
	    cout << OutputFile.c_str() <<" file closed " << endl; 
	    delete sh;
	    //tree->Delete("");
	    // tree->Delete("all");
	    //delete tree;

	  }
	
	
	}
      else
	cout << " \t OTHERRECORDTYPE " << endl;

    };

  cout << " End-Of File probably reached " << endl;

  cout << ">>>> " << tree[0]->GetEntries() << " Particles written to file" 
       << endl << ">>>>" << endl << endl;
  //sh->Write("Shower");

  // f1->Write(OutputFile.c_str());
  // f1->Close();
  
  IOAiresShutdown();
    
}// End of ATR_App::ConvertGrndPart()
//=============================================================================


//=============================================================================
void ATR_App::ConvertGrndPartnew()
{  

  int showernum=0;

  // dummy file
  OutputFile=CurrentFile;
  OutputFile+=".root";
  cout << " Open a new ROOT file " << OutputFile << endl;	    
  f1[showernum]=new TFile(OutputFile.c_str(),"RECREATE");
  f1[showernum]->SetCompressionLevel(compress);
 


  pcle = new PartGrnd_ROOT();
  
  
  int i = 0;
  int record;
  while (record=IOAiresNext(pcle)) 
    {
      if(record==PARTICLERECORDTYPE)
	{
	  f1[showernum]->cd();
	  tree[showernum]->Fill();
	  i++;
	  if (i % 5000 == 0)
	    cout << ">>>> " << i << " particles written in tree " << showernum<< endl;
	}
      else if (record==STARTOFSHOWERRECORDTYPE) // process the start Of shower
	{
	showernum++;
	cout << " \t STARTOFSHOWERRECORDTYPE , showernum=" <<showernum <<endl;

	 
	// open a new file
	OutputFile=CurrentFile;
	char showerstring[10];
	sprintf(showerstring,"%d",showernum);
	OutputFile+="_";
	OutputFile+=showerstring;
	OutputFile+=".root";
	char thefilename[20];
	strcpy(thefilename,OutputFile.c_str());

	cout << " Open a new ROOT file " << OutputFile << endl;
	//	cout << " Open a new ROOT file " << thefilename << endl;

	// open a new file    
	f1[showernum]=new TFile(OutputFile.c_str(),"RECREATE");

	//f1[showernum]=new TFile(thefilename,"RECREATE",thefilename);

	f1[showernum]->SetCompressionLevel(compress);
	f1[showernum]->cd();

	// create a new Header
	shA[showernum]= new HeaderAires_ROOT();
	sh[showernum]=shA[showernum];
	sh[showernum]->ProgFlag=AIRES;

	//read the shower header parameters
	ReReadAiresBeginOfShower(shA[showernum]);

	// book the tree
	tree[showernum] = new TTree(PartNtplName, PartNtplName);
	tree[showernum]->Branch(PartNtplName,"PartGrnd_ROOT",&pcle,128000,99);
	    
	
	}
      else if (record==ENDOFSHOWERRECORDTYPE) // process the end of shower
	{
	  cout << " \t ENDOFSHOWERRECORDTYPE , showernum=" << showernum<< endl;
	  ReReadAiresEndOfShower(shA[showernum]);
	  cout << ">>>> " << tree[showernum]->GetEntries() << " Particles written to file" << OutputFile.c_str() << endl << ">>>>" << endl << endl;
	  f1[showernum]->cd();

	  sh[showernum]->Write(HeaderName); // we want the generic header
	  tree[showernum]->Write();


	  f1[showernum]->ls();

	  f1[0]->cd();
	  f1[showernum]->Close();
	  cout << OutputFile.c_str() <<" file closed " << endl; 
	  //shA[showernum]->Delete(""); // these delete prduce crashed ion IO files
	  //tree[showernum]->Delete("");

	
	
	}
      else
	cout << " \t OTHERRECORDTYPE " << endl;

    };

  cout << " End-Of File probably reached " << endl;

  //  cout << ">>>> " << tree[showernum]->GetEntries() << " Particles written to file" 
  //       << endl << ">>>>" << endl << endl;
  //sh->Write("Shower");

  // f1->Write(OutputFile.c_str());
  // f1->Close();
  
  IOAiresShutdown();
  delete pcle;
}// End of ATR_App::ConvertGrndPart()


//=============================================================================

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


//==End of File==============================================================

















