//------------------------------------------------------------------------
/*! \file CorsToRoot.cc
    \brief Program which convert output of Corsika into a DPA standard ROOT format
    The input thus are  CORSIKA output files (.part and .long)

    This program was originally written by Gilles zalamansky
    Currently maintained by S. Dagoret-Campagne (LPNHE-Paris)  
 */
//------------------------------------------------------------------------


#include "IOCorsika.h"
#include "PartGrnd_ROOT.h"
#include "HeaderCorsika_ROOT.h"
#include "ShowerProf_ROOT.h"
// System Headers
#include <string>
#include <cmath>
#include <stdio.h>
#include <cstdlib>
#include <iostream>

// ROOT Headers
#include "root.h"

const unsigned int SZmax = MAXLONGI;
TROOT root(" ", " ");


double OBSERVATORYLEVEL; 
const double REARTH= 637131500; // cm REarth = 637131500 cm   


//-----------------------------------------------------------------------
int main(int argc, char *argv[])
{
	/*
  if (argc < 4) 
    {
      std::cerr << "Syntax: " << argv[0] 
	   << " file.part  file.long root_file  -option " << std::endl;
      std::cerr << "       -2 : if you want two different root files for Ground Particles and Longitudinal Profile " << std::endl;
      std::cerr << "            ( default: all in one file ) "
	   << std::endl;
      return 1;
    }
*/


  bloc_struct InputBuffer;
  bloc_type type;
  unsigned long Nblocs = 0;
  unsigned int MAXBUF = SubBlocSize;
  float *A;
  FILE *input = NULL;
  Int_t compress = 1;
  char *filename;
  char showername[10];


  double Nmax = 0;
  double X0 = 0;
  double Xmax = 0;
  double Chi2LDF = 0;



  /* ********************************************************************** */
  /* 1) process GROUND PARTICLE FILE  */
  /* ********************************************************************** */

  filename = argv[1];
  snprintf(showername,9,filename);
  //creation time
  time_t date = time(0);
  struct tm *ptrdate = localtime(&date);
  unsigned short enddetectflag = 1;
 
  //1) Opening binary file
  // ---------------------
  input = OpenInput(filename);


  unsigned int NEv = 0;	//Shower Id Number
  char PartNtplName[20] = "GrndPcles";
  char HeaderName[20] = "Header_ROOT";
  // char * ShowerName = argv[3]; // output file name // od
  char * ShowerName = argv[2]; // output file name
  int Nombrefile=0;
  if (argc ==5 )
    {
      Nombrefile  = (int) atoi(argv[4]+1);
      std::cout << " longitudinal Profile and Ground particles are in two different files " << std::endl;;
    }
  else 

    {
      //std::cout << " longitudinal Profile and Ground particles are in the same file " << std::endl;
    }



  float EVT[2 * SubBlocSize];	//contains EVTH and EVTE
  float PartCode = 0, Px = 0, Py = 0, Pz = 0, x = 0, y = 0,t = 0, w = 0;
                          	//particle in corsika format
  float W = 0;	//particle in aires format -> 1 pour LHAASO, 0 pour Auger
  unsigned long countpart = 0;	// nb of particles in a shower
  unsigned long countmuadd = 0, countEp = 0, countEm = 0, 
    countMUp = 0, countMUm = 0, countGAM = 0, countHad = 0, CCode = 0;	
  //particles of various type
  float countWpart = 0, countWEp = 0, countWEm = 0, 
    countWMUp = 0, countWMUm = 0, countWGAM = 0, countWHad = 0;
  float ZFi = 0, ThPr = 0, TravelLength = 0, TravelTime = 0;	//time of travel
  //corsika'version
  float CorsVers = 0;
  //Shower Number
  int ShNum = 0;

  //speed of light
  const float lightspeed = 29.9792458;	//c in cm/ns
  //(pointers to) objects to be stored in the root file

  TFile *EvTFile = 0;
  PartGrnd_ROOT *currentpart = 0;
  HeaderCorsika_ROOT *headerC = 0;
  Header_ROOT *header = 0;
  TTree *tree = 0;
  ShowerProf_ROOT *shwprof = new ShowerProf_ROOT();
  int count_sh=0;

  //2) Loop on blocks at input
  // --------------------------

  //receiving buffers
  while (enddetectflag) 
    {
      Nblocs++;
      // read a Corsika block
      NextBloc(input, &InputBuffer);

      A = &InputBuffer.bl[0];
      //cout<<"ici: "<<BlocType(A)<<endl; //pour vérifier fonctionnement
       switch (type = BlocType(A)) 
	{
    	  // read CORSIKA Header 
	case RUNH:
	  cout<<"header"<<endl;
	  CorsVers = A[3];
	  ShNum = int(A[1]);
#ifdef NODEBUGBQS
	  printf("run header found, CORSIKA version %f\n",CorsVers);
#endif
	  break;

	  // read CORSIKA End	  
	case RUNE:
	  cout<<"end"<<endl;
#ifdef NODEBUGBQS
	  std::cout << "run end found " << countpart <<" particles" << std::endl;
#endif
	  break;

	  // read CORSIKA Event Header	  	
	case EVTH:
	  cout<<"evt"<<MAXBUF<<endl;
	  for (unsigned int k = 0; k < MAXBUF; k++)
	    EVT[k] = A[k];             //event number

	  NEv = (unsigned int) A[1];   //Shower number
	  count_sh++;
	  if(count_sh>1) goto theend; 
	  //  create output root file (generic name 'Shower' is defined here)
	  EvTFile = new TFile(ShowerName, "RECREATE");
	  EvTFile->SetCompressionLevel(compress);
	  // create tree and the branch
	  tree = new TTree(PartNtplName, PartNtplName); //  particle ntuple
	  tree->Branch(PartNtplName,"PartGrnd_ROOT",&currentpart,128000,1);
	  //travel time:
	
	  if(A[78]==0)
	    {
#ifdef NODEBUGBQS
	    printf(" \t - Event with Uncurved version \n");
#endif
	    }
	  else if(A[78]==2)
	    {
#ifdef NODEBUGBQS
	    printf(" \t - Event with Curved version \n");
	    
#endif
	    }
	  else
	    {
#ifdef NODEBUGBQS
	    printf(" \t - Event Unknow curvature A[78]=%d\n",A[78]);
#endif	
	    }

	  ThPr = A[10] ;
	  //cout<<A[78]<<" "<< A[6]<< " "<<A[10]<<endl;
	
	  if((A[78]!=2))
	    {
	      ZFi  = A[6];
	    }
	  else 
	    {
	       ZFi  = A[6];
	      
	      // double ThePrbar=ThPr -asin(sin(ThPr)*(REARTH+A[47])/(REARTH+A[6]));
// 	      ZFi=A[47]+1/tan(ThPr)*(REARTH+A[6])*sin(ThePrbar);
// 	      cout<<A[47]<<" "<<REARTH<<" "<<sin(ThPr)<<" "<<sin(ThPr)*(REARTH+A[47])/(REARTH+A[6])<<" "<<ZFi< <" "<< ThePrbar<<endl;
	    }

	  TravelLength = (ZFi-A[47]) / cos(ThPr);//in cm at Auger ground level

	  OBSERVATORYLEVEL=A[47]/100; // altitude of the observator in meter
	  //cout << "observatory level= "<<OBSERVATORYLEVEL<<", ZFi=A[6]="<< ZFi <<", A[47]="<< A[47]<<endl;
	  TravelTime = TravelLength  / lightspeed;	//in ns
	  //reset particles counting in the shower
	  countpart = 0;
	  cout<<TravelLength<<"  "<<TravelTime<<endl;
	  break;

	  // read CORSIKA Event 	  	
	case EVTE:
	  // completes event info
	  for (unsigned int k = 0; k < MAXBUF; k++)
	    EVT[k + SubBlocSize] = A[k];
	  
	  //constructing shower header
	  headerC = new HeaderCorsika_ROOT(EVT);
	  strcpy(headerC->DateOfCreation, asctime(ptrdate));
	  headerC->TimeOffset = TravelTime;

	  //filling statistics
	  headerC->NbPart = countpart;
	  headerC->NbWPart = countWpart;	  
	  headerC->NbGAM = countGAM;
	  headerC->NbWGAM = countWGAM;
	  headerC->NbEm = countEm;
	  headerC->NbWEm = countWEm;
	  headerC->NbEp = countEp;
	  headerC->NbWEp = countWEp;
	  headerC->NbMUp = countMUp;
	  headerC->NbWMUp = countWMUp;
	  headerC->NbMUm = countMUm;
	  headerC->NbWMUm = countWMUm;
	  headerC->NbHad = countHad;
	  headerC->NbWHad = countWHad;
 
	  // LDF parameters
	  Nmax =  headerC->PARLGDC[0];
	  X0 =headerC->PARLGDC[1];
	  Xmax =  headerC->PARLGDC[2];
	  Chi2LDF =headerC->CHI2LD;

	    
	  //building header
	  header = headerC;
	  header->ProgFlag = CORSIKA;
	  sprintf(header->MotherProgram, "CORSIKA %f",CorsVers);

	  // xmax et x0
 	  shwprof->SetChimax(Xmax); // Gaisser Hillas fitted
  	  shwprof->SetChi0(X0); // Gaisser Hillas fitted
          shwprof->Write("ShowerProf_ROOT");
	  cout<<Xmax<<"  "<<X0<<endl;

	  header->Write(HeaderName);

	  //filling root file
	  EvTFile->Write(ShowerName);
	  break;

	  // read CORSIKA Particle 	  	
	case PART:
	  unsigned int curpos = 0;
	  //39 particles come through buffer, eight float each 
	  //for (unsigned int k = 0; k < (MAXBUF / 8); k++)  // changer 7 par 8 pour Auger
	  for (unsigned int k = 0; k < (MAXBUF / 7); k++)  // changer 7 par 8 pour Auger
	    {
	      // curpos = 8 * k;	// part nb k in the buffer - changer 7 par 8 pour Auger
	      curpos = 7 * k;	// part nb k in the buffer - changer 7 par 8 pour Auger
	      PartCode = *(A + curpos);
	      Px = *(A + curpos + 1);
	      Py = *(A + curpos + 2);
	      Pz = *(A + curpos + 3);
	      x = *(A + curpos + 4) * 1e-2;	//x, y in meters
	      y = *(A + curpos + 5) * 1e-2;
	      t = *(A + curpos + 6) - TravelTime;	//substration due to aires standard
	 
// 	    {
// 	      double ThePrbar=ThPr -asin(sin(ThPr)*(REARTH+A[47])/(REARTH+A[6]));ThePrbar
// 	      ZFi=A[47]+1/tan(ThPr)*(REARTH+A[6])*sin(ThePrbar);
// 	    }

	      // cout<<A[78]<<endl;
	      //     cout<<A[78]<<"  "<<A[6]<<" "<<A[47]<<" "<<cos(ThPr)<<" "<<ThPr<<*(A + curpos + 6)<<" "<<TravelTime<<endl;
	      w = *(A + curpos + 7);
	      CCode = (unsigned short) ((unsigned int) PartCode / 1000);
	      if (CCode == 75 || CCode == 76)	//muon add. info discarded
		countmuadd++;
	      else 
		{
		  if (Pz > 0)	//void particles and negative Pz discarded
		    {
		      w = W = 1; // Pour LHAASO, pas besoin de poids
		      currentpart = CorsikaPart((int)PartCode,Px, Py,Pz, x, y,t, w);
		      ///// W = currentpart->W; // Pour Auger

		      // count particles of various types		
		      if (CCode == 2) 
			{
			  countEp++;
			  countWEp += W;
			};
		      if (CCode == 3) 
			{
			  countEm++;
			  countWEm += W;
			};
		      if (CCode == 5) 
			{
			  countMUp++;
			  countWMUp += W;
			};
		      if (CCode == 6) 
			{
			  countMUm++;
			  countWMUm += W;
			};
		      if (CCode == 1) 
			{
			  countGAM++;
			  countWGAM += W;
			};
		      if (CCode > 6 && CCode < 66) 
			{
			  countHad++;
			  countWHad += W;
			};
		      // add the particle to the output tree
		      tree->Fill();
		      countpart++;
		      countWpart += W;
		      if (countpart % 1000000 == 0) 
			std::cout << ">>>> " << countpart << " particles written" << std::endl;    
		      // clean memory
		      delete currentpart;
		    };// end on if
		};// end on else
	    };	// end of loop on particles
	  break;
	}; // end of switch


      enddetectflag = EndDetect(A);
    };
 
 theend:
  std::cout << ">>>> " << countpart << " Particles written to file " << ">>>>" << std::endl ;
 
  //  fclose(input);

  return 1;

  /* ********************************************************************** */
  /* 2) LONGITUDINAL PROFILE CHARGED PARTICLES AND ENERGY EM DEPOSIT        */
  /* ********************************************************************** */


  /* Case : we separate on two different files longitudinal profile and particles */
  if ( Nombrefile == 2 )
    {
      EvTFile->Close();
      EvTFile = new TFile("ShowerProfile.root", "RECREATE");
    };
  
  ShowerProf_ROOT *shw_prof;
  shw_prof = new ShowerProf_ROOT();
  FILE * tfile = fopen(argv[2],"r");
  char Line[132];
  
  char auxLine[132];
  float xdepth=0.0 , chargedparticles=0.0 , sumenergy=0.0 , neutrino=0.0;
  float dum1 = 0.0 , dum2 = 0.0 , dum3 = 0.0 , dum4 = 0.0 ;
  float dum5 = 0.0 , dum6 = 0.0 , dum7 = 0.0 , dum8 = 0.0 ;
  char startstr0[256] = " LONGITUDINAL DISTRIBUTION IN";
  char endstr0[256] = " DEPTH     GAMMAS   POSITRONS";
  char endstr1[256] = " LONGITUDINAL ENERGY DEPOSIT";
  char endstr2[256] = " DEPTH       GAMMA    EM IONIZ     EM CUT";
  char endstr3[256] = " FIT OF THE HILLAS CURVE";
  char ShowerProfName[20] = "ShowerProf_ROOT";
  int i = 0;
  int j = 0;
  int nbq = 0;

  Float_t nbr = 0.;
  Float_t maxi= 0.;
  Float_t maxdepth=0.;
  char s1[50],s2[50],s3[50],s4[50];

  //  string *s1 = 0,*s2 = 0,*s3 = 0;
  fgets(Line,140,tfile);
  //std::cout <<"Line=" <<Line  << std::endl;
  //  sscanf(Line,"%s %s %s %i",&s1,&s2,&s3,&nbq);

  sscanf(Line,"%s %s %s %i %s",&s1,&s2,&s3,&nbq,&s4);
  //std::cout << " nbq = " << nbq << "  " << " s1 = " << s4 << std::endl;
  shw_prof->SetNShwPts(nbq);

  fgets(Line,140,tfile);
  fgets(Line,140,tfile);
  sscanf(Line,"%f",&nbr);
  shw_prof->SetChi0(nbr);
  //std::cout << " NBR " << nbr << std::endl;


  TVector aux_shw_pts(nbq);  // depth
  TVector aux_log_pts(nbq);  // log du nombre de part.
  TVector aux_logE_pts(nbq); // log energy deposit
  fclose(tfile);

  tfile = fopen(argv[2],"r");


  while ( fgets(Line,140,tfile) != NULL ) 
    {
      if ( strstr(Line,startstr0) != NULL )
	fgets(Line,140,tfile);
      if ( strstr(Line,endstr1) != NULL  || strstr(auxLine,endstr2)!=NULL )
	{
	  if (strstr(Line,endstr1) != NULL )
	    fgets(Line,140,tfile);
	  if ( strstr(Line,endstr2) != NULL ) 
	    {	      
	      //auxLine = Line;
	      for(int iLine=0; iLine<132;++iLine)
		auxLine[iLine]=Line[iLine];
	      fgets(Line,140,tfile);
	    }
	  if ( strstr(Line,endstr3) != NULL )
	      break;
	  if ( strstr(auxLine,endstr2) != NULL )
	    {
	      sscanf(Line,"%f %f %f %f %f %f %f %f %f %f",
		     &dum1,&dum2,&dum3,&dum4,&dum5,&dum6,
		     &dum7,&dum8,&neutrino,&sumenergy);
	      aux_logE_pts(j) = sumenergy-neutrino;
	      j++;
	    }
	}
      if ( strstr(Line,endstr1) == NULL || strstr(Line,endstr0) != NULL )
	{
	  if ( strstr(auxLine,endstr2)!=NULL ) 
	    continue;
	  sscanf(Line,"%f %f %f %f %f %f %f %f %f %f",
		 &xdepth,&dum1,&dum2,&dum3,&dum4,&dum5,&dum6,&chargedparticles,&dum7,&dum8);
	  if (xdepth >0 ) 
	    {
	      aux_shw_pts(i) = xdepth;
	      aux_log_pts(i) = chargedparticles;
	      if ( chargedparticles > maxi )
		{
		  maxi = chargedparticles;
		  maxdepth = xdepth;
		}
	      i++;
	    }
	}
    }


  fclose(tfile);




  // fabrice verctors
  shw_prof->SetShwPts(&aux_shw_pts);
  shw_prof->SetLogPts(&aux_log_pts);
  shw_prof->SetLogEPts(&aux_logE_pts);
  //shw_prof->SetChimax(maxdepth); //computed here
  shw_prof->SetChimax(Xmax); // Gaisser Hillas fitted
  //shw_prof->SetChi0(maxdepth); //computed here
  shw_prof->SetChi0(X0); // Gaisser Hillas fitted
  shw_prof->Write(ShowerProfName);


#ifdef NODEBUGBQS
  std::cout << shw_prof->Read(ShowerProfName) << std::endl;
#endif    

#ifdef NODEBUGBQS
  std::cout << " ********* " << std::endl;
  std::cout << " NShwPts " << shw_prof->GetNShwPts() << std::endl;
  std::cout << " Chi0    " << shw_prof->GetChi0()    << std::endl;
  std::cout << " Chimax  " << shw_prof->GetChimax()    << std::endl;
#endif    
#ifdef NODEBUGBQS
  std::cout << " ********* Print Longitudinal profile **********  " << std::endl;
  aux_logE_pts.Print();
  aux_log_pts.Print();
  aux_shw_pts.Print();
#endif    

  /* Print Longitudinal profile */
  /* *******************************************************************  */


  EvTFile->Close();

#ifdef NODEBUGBQS  
  std::cout << "file " << filename << " closed: I read " << Nblocs 
       << " blocs, " << std::endl;
  std::cout << "time offset aires/corsika " << TravelTime << " ns " << std::endl;
  std::cout << "\n\n counted: " << countpart << " particles, " 
       << countmuadd << " muon add. info. " << std::endl;
  std::cout << "\n MUp " << countMUp << " MUm " << countMUm << " MUONS "
       << countMUp + countMUm << std::endl;
  std::cout << "weighted MUp " << countWMUp << " weighted MUm " 
       << countWMUm << " weighted muons " << countWMUp + countWMUm << std::endl;
  std::cout << "\n gammas " << countGAM << " weighted :" << countWGAM 
       << std::endl;
  std::cout << "\n e+ (Ep) " << countEp << " e- (Em) " << countEm 
       << " e+e- (EpEm) " << countEm + countEp << std::endl;
  std::cout << "weighted Ep " << countWEp << " weighted Em " << countWEm
       << " weighted EpEm " << countWEp + countWEm << std::endl;
  std::cout << "\nhadrons " << countHad << " weighted " << countWHad 
       << std::endl;
  std::cout << "weighted NBPart " << countWpart << std::endl;
#endif

  std::cout << ">>>>  Root file " << ShowerName <<
    " has been created from original file " << filename << " and " << argv[2]<< std::endl;
  std::cout << "shower created at time " << header->GetDateOfCreation() 
       << std::endl;
  //std::cout << " HEADER MOI " << header->ProgFlag << std::endl;

  return 0;
};

//--------------------------------------------------------------------
// Configure (x)emacs for this file ...
// Local Variables:
// mode:c++
// compile-command: "cd ../cmt; make -k"
// End:

