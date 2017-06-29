/*
 *              Aires <-> Root Very Basic Interface v0r1
 *
 * It currently only handles files with one shower. If there are more than one
 * shower, it will concatenate them in a single one, using the beginning of
 * the first shower and the end of the last to make the header. That's
 * definitely not what we want, therefore be careful if you use multiple
 * shower files
 *
 * Xavier Bertou <bertou@in2p3.fr> Fri May  5 14:51:51 CEST 2000
 *
 * Are provided by the library :
 * ---------------------------
 *  HeaderAires *IOAiresOpen(char filename[1024])
 *           creates a new header properly extracted
 *           from the .grdpcles Aires file
 *  void IOAiresSummary();
 *           dumps the airessry() to stdout
 *  Part *IOAiresNext();
 *           creates a new particle filled with the next particle in
 *           .grdpcles Aires file
 *  int IOAiresNext(Part *);
 *           fills the Part pointer with the next particle in
 *           .grdpcles Aires file (returns 0 if end of file)
 *  int IOAiresShutdown();
 *           close any opened files
 *
 */


#include <string.h>
#include <math.h>
#include <stdio.h>

#include <cstdlib>
#include <iostream>
//#include "root.h"

#include "IOAires.h"
#include "Globals.h"
#include "PartGrnd_ROOT.h"


extern RotToAuger rot;

// Some stuff to use libAires
// You may have to remove the '_' at the end of the names.

// Flushing out the arguments added by CJillings (jillings@physics.ucla.edu)
// on September 20, 2001 at LPNHEP at Paris VI. Left hardcoded as arrays below.
extern "C" { 
	int opencrofilec(char(*)[1024], char(*)[1024], int*, int*, int*, int*, int*);
	int ciorinit_(int*, int*, int*, int*);
	int crorewind_(int*, int*, int*);
	int crorecfind_(int*, int*, int*,int*,int*);
	int getcrorecord_(int*, int(*)[99],double(*)[99],int*,int*,int*);
	int regetcrorecord_(int*, int(*)[99],double(*)[99],int*,int*,int*);
	int inpsry_(int*);
	int croinputdata0_(int*,double*,int*,double*);
	int cioclose_();
	int ciorshutdown_();
	int crofileversion_(int*);
}

// End of the mess fortran/C++

int ciochann;	// The integer used by Aires as a file descriptor

int ReadAiresBeginOfShower(HeaderAires_ROOT * sh);	// Reads the begin of shower (internal)
int ReadAiresEndOfShower(HeaderAires_ROOT * sh);	// Updates with the end of shower
int ReReadAiresBeginOfShower(HeaderAires_ROOT * sh);	// Reads the begin of shower (internal)
int ReReadAiresEndOfShower(HeaderAires_ROOT * sh);	// Updates with the end of shower

//--------------------------------------------------------------------------
void Error(char *msg)
{
  cerr << "EEEE" << endl;
  cerr << "EEEE " << msg << endl;
  cerr << "EEEE" << endl;
  cerr << "EEEE Aborting." << endl;
  cerr << "EEEE" << endl;
  exit(1);
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
HeaderAires_ROOT *IOAiresOpen(const char filename[1024])
{
  char wdir[1024];
  char filename2[1024];
  int indata[99];
  double fldata[99];
  int altrec, irc;

  strncpy(wdir, "", 1024);
  /* necessary for opencrofile_ to work. Don't ask me why... */
  strncpy(filename2, filename, 1024);
  int i1 = 0;
  int i2 = 1;
  ciorinit_(&i1, &i2, &i1, &irc);
  if (irc)
    Error("Can't init Aires");
  i2 = 10;
  int verb = 3;
  opencrofilec(&wdir, &filename2, &i1, &i2, &verb, &ciochann, &irc);
  if (irc > 1)
    Error("Can't Open file");
  HeaderAires_ROOT *sh = new HeaderAires_ROOT();
  // the beginning of shower
  if (!ReadAiresBeginOfShower(sh))
    Error("Can't find begin of shower");
  if (!ReadAiresEndOfShower(sh))
    Error("Can't find end of shower");
  // We update the info
  // with the end of shower
  crorewind_(&ciochann, &verb, &irc);
  if (irc)
    Error("Can't Rewind file");
  getcrorecord_(&ciochann, &indata, &fldata, &altrec, &i1, &irc);
  if (irc != 1)
    Error("Can't Reread file");
  // The file is now just
  // after the beginning
  snprintf(sh->MotherProgram,50,"Aires version %d",crofileversion_(&ciochann));
  return sh;
}
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
bool IOAiresSimpleOpen(const char filename[1024])
{
  char wdir[1024];
  char filename2[1024];
  int indata[99];
  double fldata[99];
  int altrec, irc;

  strncpy(wdir, "", 1024);
  /* necessary for opencrofile_ to work. Don't ask me why... */
  strncpy(filename2, filename, 1024);
  int i1 = 0;
  int i2 = 1;
  ciorinit_(&i1, &i2, &i1, &irc);
  if (irc)
    {
    Error("Can't init Aires");
    return false;
    }
  i2 = 10;
  int verb = 3;
  opencrofilec(&wdir, &filename2, &i1, &i2, &verb, &ciochann, &irc);
  if (irc > 1)
    {
    Error("Can't Open file");
    return false;
    }
  return true;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void IOAiresSummary()
{
  int i1 = 0;
  inpsry_(&i1);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
int ReadAiresBeginOfShower(HeaderAires_ROOT * sh)
{
  int i1 = 0;
  int indata[99], intdata[99], shprimcode[10];
  double fldata[99], realdata[99], shprimwt[10];
  int altrec, irc;
  if (!getcrorecord_
      (&ciochann, &indata, &fldata, &altrec, &i1, &irc)) return 0;
  if (altrec && (irc == 1)) {
    // Get some more information...
    croinputdata0_(intdata, realdata, shprimcode, shprimwt);
    // We can now fill the header
    sh->PrimaryType = indata[0];
    sh->PrimaryEnergy = pow(10, fldata[0]);
    sh->PrimaryZenithAngle = fldata[1] * DEG2RAD;
    //sh->PrimaryAzimuthAngle = fldata[2] * DEG2RAD; without rotation
    // with rotation to Auger Frame
    sh->PrimaryAzimuthAngle=rot.rotAzimuth(fldata[2] * DEG2RAD+M_PI); 

    sh->GroundLevel = realdata[10];
    snprintf(sh->DateOfCreation,50,"%02d/%02d/%04d at %02dh%02d",
	     indata[3],indata[4],indata[2],indata[5],indata[6]);
    sh->ThinningEnergy = fldata[3];
    sh->FirstInteractionDepth = fldata[4];
    sh->TimeShift = fldata[6];
    snprintf(sh->MotherProgram,50,"Aires version %d",crofileversion_(&ciochann));

    cout << " Read begginning of the shower logE= " <<  fldata[0] ;
    cout << " Theta = " <<  fldata[1] << " Phi = " <<  fldata[2] << endl;
    return 1;
  }
  return 0;
}
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
int ReReadAiresBeginOfShower(HeaderAires_ROOT * sh)
{
  int i1 = 0;
  int indata[99], intdata[99], shprimcode[10];
  double fldata[99], realdata[99], shprimwt[10];
  int altrec, irc;
  if (!regetcrorecord_
      (&ciochann, &indata, &fldata, &altrec, &i1, &irc)) return 0;
  if (altrec && (irc == 1)) {
    // Get some more information...
    croinputdata0_(intdata, realdata, shprimcode, shprimwt);
    // We can now fill the header
    sh->PrimaryType = indata[0];
    sh->PrimaryEnergy = pow(10, fldata[0]);
    sh->PrimaryZenithAngle = fldata[1] * DEG2RAD;
    //sh->PrimaryAzimuthAngle = fldata[2] * DEG2RAD; without rotation
    // with rotation to Auger Frame
    sh->PrimaryAzimuthAngle=rot.rotAzimuth(fldata[2] * DEG2RAD+M_PI); 

    sh->GroundLevel = realdata[10];
    snprintf(sh->DateOfCreation,50,"%02d/%02d/%04d at %02dh%02d",
	     indata[3],indata[4],indata[2],indata[5],indata[6]);
    sh->ThinningEnergy = fldata[3];
    sh->FirstInteractionDepth = fldata[4];
    sh->TimeShift = fldata[6];
    cout << " ReRead begginning of the shower logE= " <<  fldata[0] ;
    cout << " Theta = " <<  fldata[1] << " Phi = " <<  fldata[2] << endl;
    snprintf(sh->MotherProgram,50,"Aires version %d",crofileversion_(&ciochann));

    return 1;
  }
  return 0;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
int ReadAiresEndOfShower(HeaderAires_ROOT * sh)
{
  int i1 = 0;
  int intype = 2;
  int vrb = 3;
  int indata[99];
  double fldata[99];
  int altrec, irc;
  
  if (!crorecfind_(&ciochann, &intype, &vrb, &i1, &irc))
    return 0;
  
  regetcrorecord_(&ciochann, &indata, &fldata, &altrec, &i1, &irc);
  if (altrec && (irc == 2)) 
    {
      sh->XMax = fldata[8];
      sh->NMax = fldata[9];
      cout << " Read end of the shower Xmax= " <<  fldata[8] ;
      cout << " NMax= " <<  fldata[9] << endl;

      return 1;
    } 
  else
    return 0;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
int ReReadAiresEndOfShower(HeaderAires_ROOT * sh)
{
  int i1 = 0;
  int intype = 2;
  int vrb = 3;
  int indata[99];
  double fldata[99];
  int altrec, irc;
  
  //	if (!crorecfind_(&ciochann, &intype, &vrb, &i1, &irc))
  //		return 0;
  
  regetcrorecord_(&ciochann, &indata, &fldata, &altrec, &i1, &irc);
  if (altrec && (irc == 2)) 
    {
      sh->XMax = fldata[8];
      sh->NMax = fldata[9];
      cout << " ReRead end of the shower Xmax= " <<  fldata[8] ;
      cout << " NMax= " <<  fldata[9] << endl;

      return 1;
    } 
  else
    return 0;
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
PartGrnd_ROOT * IOAiresNext()
{
  PartGrnd_ROOT *result=new PartGrnd_ROOT();
  if (IOAiresNext(result)) return result;
  delete result;
  return NULL;
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
int IOAiresNext(PartGrnd_ROOT *result)
{
  int i1 = 0;
  int indata[99];
  double fldata[99];
  int altrec, irc;
  
  if (!getcrorecord_
      (&ciochann, &indata, &fldata, &altrec, &i1, &irc)) return 0;
  
  if (altrec)
    {
      cerr << " Alternative Record :: irc=" << irc << endl;
      
      // return IOAiresNext(result);	 old statement
      // Do we want eventually to process
      // multiple showers ???
      // If yes, here we change shower...
      if(irc==1) return STARTOFSHOWERRECORDTYPE;
      else if(irc==2) return ENDOFSHOWERRECORDTYPE;
      else return OTHERRECORDTYPE;
    }
  else 
    {
      result->Id = indata[0];
      result->E = pow(10, fldata[0]);

      // without rotation:
      result->X = pow(10, fldata[1]) * cos(fldata[2]);
      result->Y = pow(10, fldata[1]) * sin(fldata[2]);
      result->Z = 0.0;  // FIXME Z is bad !!!!!
      result->UX = fldata[3];
      result->UY = fldata[4];
      if (pow(result->UX,2)+pow(result->UY,2)>1-0.000001) 
	{
	double norm=sqrt((1-0.000001)/(pow(result->UX,2)+pow(result->UY,2)));
	cerr << "PZ out of bonds : " << pow(result->UX,2)+pow(result->UY,2) << " > 1-PZMIN² = " << 1-0.000001 << endl;
	result->UX*=norm;
	result->UY*=norm;
	}
      result->UZ=-sqrt(1-(pow(result->UX,2)+pow(result->UY,2)));

      // now with rotation to Auger Frame
      double x= result->X;
      double y= result->Y;
      double z= result->Z;
      double u=	result->UX;
      double v=	result->UY;
      double w=	result->UZ;
      rot.rotPart(x,y,z,u,v,w);
      result->X=x;
      result->Y=y;
      result->Z=z;
      result->UX=u;
      result->UY=v;
      result->UZ=w;


      result->T = fldata[5];
      result->W = fldata[6];
      return PARTICLERECORDTYPE; // return 1 if particle
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
int IOAiresShutdown()
{
	cioclose_();
	return ciorshutdown_();
}
//--------------------------------------------------------------------------
