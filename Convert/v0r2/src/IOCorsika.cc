//---------------------------------------------------------------------------
/*! \file IOCorsika.cc
    \brief Implementation file of IOCorsila.h  

 */
//---------------------------------------------------------------------------

#include "IOCorsika.h"
#include "PartGrnd_ROOT.h"
#include <fstream>

extern RotToAuger rot;

extern double OBSERVATORYLEVEL;

//energy :below is the mass table
static float CorsCodeMass[NBPartTable] = {
    0,		//
      0.0e0,		//gamma;
      .51099906e-3,	//e+;
      .51099906e-3,	//e-;
      0.0e0,		//;
      .105658389e0,	//mu+;
      .105658389e0,	//mu-;
      .1349743e0,	//PI0;
      .1395679e0,	//PI+;
      .1395679e0,	//PI-;
      0.497671e0,	//K0_L;
      0.493646e0,	//K+;
      0.493646e0,	//K-;
      .93956563e0,	//n;
      .93827231e0,	//p;
      .93827231e0,	//pbar;
      0.497671e0,	//K0_s;
      0.54745e0,	//etha;
      1.11563e0,	//Lambda;
      1.18937e0,	//Sigma+;
      1.19255e0,	//Sigma0;
      1.197465e0,	//Sigma-;
      1.31485e0,	//Xhi0;
      1.32133e0,	//Xhi-;
      1.67243e0,	//Omega-;
      .93956563e0,	//nbar;
      1.11563e0,	//Lambda_bar;
      1.18937e0,	//Sigmabar-;
      1.19255e0,	//Sigmabar0;
      1.19743e0,	//Sigmabar+;
      1.31485e0,	//Xhibar0;
      1.32133e0,	//Xhibar+;
      1.67243e0,	//Omegabar+;
      1.7841e0,	//;
      1.7841e0,	//;
      1.8693e0,	//;
      1.8693e0,	//;
      1.8645e0,	//;
      1.8645e0,	//;
      1.9693e0,	//;
      1.9693e0,	//;
      2.2852e0,	//;
      80.6e0,		//;
      80.6e0,		//;
      91.161e0,	//;
      1.877e0,	//;
      2.817e0,	//;
      3.755e0,	//;
      0.0e0,		//;
      0.0e0,		//;
      0.78195e0,	//omega;
      0.7669e0,	//rho0;
      0.7681e0,	//rho+;
      0.7681e0,	//rho-;
      1.2309e0,	//Delta++;
      1.2323e0,	//Delta+;
      1.2336e0,	//Delta0;
      1.2349e0,	//Delta-;
      1.2309e0,	//Deltabar--;
      1.2323e0,	//Deltabar-;
      1.2336e0,	//Deltabar0;
      1.2349e0,	//Deltabar+;
      0.89624e0,//K*0;
      0.89209e0,//K*+;
      0.89209e0,//K*-;
      0.89624e0,//Kbar*0;
      0.0e0,	//neutrino e;
      0.0e0,	//neutrino bar e;
      0.0e0,	//neutrino mu ;
      0.0e0,    //neutrino bar mu ;
      0.0e0,    //;
      0.54745e0,//etha->gamma,gamma;
      0.54745e0,//etha->3.PI0;
      0.54745e0,//etha->PI+,PI-,PI0;
      0.54745e0,//etha->PI+,PI-,gamma;
      0.0e0,	//mu + add info;
      0.0e0	//mu- addd info;
      };


//----------------------------------------------------------------------
bool operator < (const longi_struct & a, const longi_struct & b) 
{
  return (a.DEPTH < b.DEPTH);
}
//----------------------------------------------------------------------



//------------------------------------------------------------------------
/*! \function PartGrnd_ROOT *CorsikaPart( int PartCode, float Px, float Py, float Pz, float XX,float YY, float TT, float WW)
    \brief This function translate particles variables which where in Corsika conventions into Auger conventions  
 */
//---------------------------------------------------------------------------
PartGrnd_ROOT *CorsikaPart( int PartCode, float Px, float Py, float Pz, float XX,float YY, float TT, float WW)
{
  float X,Y,Zp,UX,UY,UZ,T,W,E; // Corsika conventions
  float Xa,Ya,Za,UXa,UYa,UZa,Ta,Wa,Ea; // Auger Conventions

  int Id;
  unsigned int CHO = (unsigned int) PartCode;	//corsika's code+hadr gen.
  unsigned short CCode = (unsigned short) (CHO / 1000);	//corsika's code

  double norm = sqrt((double) Px * Px + (double) Py * Py + (double) Pz * Pz);

  UX = (float) ((double) Px / norm);
  UY = (float) ((double) Py / norm);
  UZ = (float) ((double) Pz / norm);

	

  /*  old rotation
  UXa=SIND*UX-COSD*UY;
  UYa=COSD*UX+SIND*UY;
  UZa=-UZ; // positive coordinates are toward earth center in Corsika
           // in auger positive coordinates are upward.
			 
  X = XX;
  Y = YY;
  Zp = 0.0;			//waiting further development
  
  Xa=COSROT*X+SINROT*Y; // Ok here it is at Corsika reference level
  Ya=-SINROT*X+COSROT*Y; // need to shift to Auger level at 1400
  Za=Zp;		 // shit is OBSERVATORYLEVEL

                         // dx=(Ux/Uz)*dz
                         // dy=(Uy/Uz)*dz
                         // Xa+=Ux/Uz*OBSERVATORYLEVEL
                         // Ya+=Uy/Uz*OBSERVATORYLEVEL

  */

  // new rotation

  double x,y,z,u,v,w;
  x=double(XX); y=double(YY); z=0;
  u=double(UX); v=double(UY); w=double(UZ);
  rot.rotPart(x,y,z,u,v,w);
  
  Xa=float(x);
  Ya=float(y);
  Za=float(z);
  UXa=float(u);
  UYa=float(v);
  UZa=float(w);

  T = TT;
  W = WW;


  float B, m, mMev, av = 15.56, as = 17.23, ac = 0.697, aa = 23.285, ap = 12;
// I use Bethe Weiszacker formula with those coefficients for nuclei masses

  unsigned short Z, A;

  if (CCode < 100)
    m = CorsCodeMass[CCode];
  else 
    {
      A = (unsigned short) CCode / 100;
      Z = CCode % 100;
      
      B = av * A - as * pow(A, 2. / 3.) - ac * Z * Z / pow(A,1. /3.) - aa * (A - 2 * Z) * (A - 2 * Z) / A;
      if (Z % 2 == 0 && (A - Z) % 2 == 0)
	B = B - ap * sqrt((double)A);
      if (Z % 2 == 1 && (A - Z) % 2 == 1)
	B = B + ap * sqrt((double)A);
      
      mMev = 939.56563 * (A - Z) + 938.27231 * Z - B;
      m = mMev / 1000.;
    }


  //compute the energy of the particle in GeV
  E =(Float_t) sqrt((double) m * m + (double) Px * Px +(double) Py * Py + (double) Pz * Pz);
  if(CCode == 5 || CCode ==6)
    cout << " CCode = " << CCode << " -> Energy = " << E << " = sqrt(" << m << "*" << m << " + " << Px << "*" << Px << " + " << Py << "*" << Py << " + " << Pz << "*" << Pz << ")" << endl; 
  


//Aires Code form CCode
//-------------------
  if (CCode < 100)
    Id = CorsToAiresTab[CCode]; // use table if not a nuclei
  else  // use the formula if a nuclei
    Id = (108 + 30 * (CCode % 100) + (Int_t) (CCode / 100));

  // create the class
  PartGrnd_ROOT *part=new PartGrnd_ROOT();

  // fill the variables which are now in Auger Conventions
  part->Id=Id;
  part->X=Xa;
  part->Y=Ya;
  part->Z=Za;
  part->UX=UXa;
  part->UY=UYa;
  part->UZ=UZa;
  part->E=E;
  part->T=T;
  part->W=W;


  return part;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
/*! \function bloc_type BlocType(float *A0)
    \brief decode the block type RUNH RUNE EVTH EVTE in corsika file

 */
//---------------------------------------------------------------------------
bloc_type BlocType(float *A0)
{
  char *word;
  word = (char *) A0;
  if (word[0] == 'R' && word[1] == 'U' && word[2] == 'N'
      && word[3] == 'H')
    return RUNH; 
  if (word[0] == 'R' && word[1] == 'U' && word[2] == 'N'
      && word[3] == 'E')
    return RUNE;
  if (word[0] == 'E' && word[1] == 'V' && word[2] == 'T'
      && word[3] == 'H')
    return EVTH;
  if (word[0] == 'E' && word[1] == 'V' && word[2] == 'T'
      && word[3] == 'E')
    return EVTE;
  return PART;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/*! \function FILE *OpenInput(char *filename)
    \brief open the corsika file to read it in a block mode 
 */
//---------------------------------------------------------------------------
FILE *OpenInput(char *filename)
{
  FILE *fi;
  float dummy;
  if ((fi = fopen(filename, "rb")) == NULL) 
    {
      printf("failed to open %s for reading\n", filename);
      exit(0);
    }
  printf("\nfile %s open for reading\n", filename);
  fread(&dummy, sizeof(float), 1, fi);
  return fi;
}
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/*! \function void NextBloc(FILE * Input, bloc_struct * InputBuffer)
    \brief seek to the next block in the file
 */
//---------------------------------------------------------------------------
void NextBloc(FILE * Input, bloc_struct * InputBuffer)
{
  static int count = 0;
  float dummy;
  fread(InputBuffer, sizeof(bloc_struct), 1, Input);
  count++;
  if (count % 21 == 0) 
    {
      fread(&dummy, sizeof(float), 1, Input);
      fread(&dummy, sizeof(float), 1, Input);
    }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/*! \function unsigned short EndDetect(float *A0)
    \brief detect the end of Corsika file
*/
//---------------------------------------------------------------------------
unsigned short EndDetect(float *A0)
{
  char *word;
  word = (char *) A0;
  if (word[0] == 'R' && word[1] == 'U' && word[2] == 'N'
      && word[3] == 'E')
    return 0;
  else
    return 1;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
unsigned short DetectSubStr(char *targ, char *tested)
{				
// check a substring 'target' inside  string 'tested'
//DetectSubStr("hello","****hello***") returns 1
//DetectSubStr("hullo","****hello***") returns 0
  unsigned short ltarg = strlen(targ), ltested = strlen(tested), k=0, res=1;
  char buff[512];
  while (k + ltarg < ltested && res) 
    {
      sprintf(buff, "%s", tested + k);
      buff[ltarg] = '\0';
      k++;
      res = strcmp(targ, buff);
    }
  return (res == 0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
int EofInLST(char *filename)	//return 1 if END OF RUN found,0 if not, -1 if file not found
{
  cout << "EofInLST " << filename << endl;
  char endstr[11] = "END OF RUN";
  unsigned short flagend0 = 1;
  char c = '.', buff[12] = "END OF RHUM";	//so that it doesn't match
  unsigned int i = 0, lb = strlen(endstr);
  unsigned long k = 0;
  int res = 0;
  
  FILE *fi;
  if ((fi = fopen(filename, "r")) == NULL) 
    {
    cout << "file " << filename << " could not be open " <<
      endl;
    res = -1;
    } 
  else 
    {
    while (flagend0 && feof(fi) == 0) 
      {
      k++;
      fscanf(fi, "%c", &c);
      for (i = 0; i < lb - 1; i++)
	buff[i] = buff[i + 1];
      buff[lb - 1] = c;
      if (DetectSubStr(endstr, buff)) 
	{
	  flagend0 = 0;
	  res = 1;
	}
      }
    fclose(fi);
    }
  return res;
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
void filllongimat(char *filename, char *startstr0,char *endstr0)
{
  char endstr[256] = "END OF RUN";
  unsigned short flagstart0 = 0, flagend0 = 1;
  char buff[256];
  unsigned int k = 0;
 

  ifstream fi(filename);
  
  if (fi) 
    {
      while (flagend0 && !DetectSubStr(endstr, buff)) 
	{
	  fi.getline(buff, 256);
	  if (DetectSubStr(startstr0, buff)) 
	    {
	      fi.getline(buff, 256);	//line feed
	      fi.getline(buff, 256);
	      flagstart0 = 1;
	    }
	  if (flagstart0 && DetectSubStr(endstr0, buff))
	    flagend0 = 0;
	  if (flagstart0 && flagend0) {
	    /*	sscanf(buff,
		"%f %f %f %f %f %f %f %f %f %f\n",
		&v.DEPTH, &v.GAMMAS, &v.POSITRONS,
		&v.ELECTRONS, &v.MUp, &v.MUm,
		&v.HADRONS, &v.CHARGED, &v.NUCLEI,
		&v.CHERENKOV);
		longi.push_back(v);
	    */	k++;
	  }
	}
      fi.close();
		
    } 
  else
    cout << "file " << filename << " could not be open " << endl;
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
unsigned short defineoutputdir(char *strout)
{
/* used by SplitBFile and corsika to know
where to search and write root files */

  char outdir[150] = "../outputdir";
  //  file /home/zala/corsika/outputdir contains the file and path
  // inside which root files are stored in our simulations
  
  unsigned short l = 0;
  ifstream fin(outdir);
  if (fin) 
    {
      fin.getline(strout, 256);
      l = strlen(strout);
      printf("root files  created and searched in dir:  %s\n",strout);
    } 
  else
    cout << "message from generalfunc.cc: file " << outdir <<
      " could not be open:this file should contain the default path for root files " << endl;
  return l;
}
//-----------------------------------------------------------------------
