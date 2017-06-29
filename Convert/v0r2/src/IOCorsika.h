//-------------------------------------------------------------------------
/*! \file IOCorsika.h
    \brief defines functions to read in Corsila files
    Presently maintained by Sylvie Dagoret-Campagne.
      
 */ 
//-------------------------------------------------------------------

#ifndef CORSIKAFUNC_H
#define CORSIKAFUNC_H
using namespace std;
#define NODEBUGBQS 1
#define SubBlocSize 273 //nouveau format fichier Corsika 273=7*39 (7mots)
//#define SubBlocSize 312 //ancien format fichier Corsika 312=8*39 (8mots)
#include "root.h"
#include "PartGrnd_ROOT.h"
#include <vector>
#include <cmath>
#include <ctime>
#include <set>
#include "CorsikaConst.h"
#include "RotToAugerFr.h"
// few physical definition 
//const double MAGDECLI = 4.233333; // magnetic declination in degree 
                                  // magnetic field is 4.2 degrees 
                                  // toward east with respect to 
                                  // geographic north
// rotation matrix elements from Corsika to Auger conventions
//const double COSROT=cos(M_PI_2*(-1.+MAGDECLI/90.));
//const double SINROT=sin(M_PI_2*(-1.+MAGDECLI/90.));
//const double COSD=cos(MAGDECLI/180.*M_PI);
//const double SIND=sin(MAGDECLI/180.*M_PI);


//------------------------------------------------------------------------
typedef struct 
{
float DEPTH, GAMMAS, POSITRONS, ELECTRONS, MUp, MUm, HADRONS,CHARGED, NUCLEI, CHERENKOV;
} longi_struct;
//------------------------------------------------------------------------

//------------------------------------------------------------------------
typedef struct {
	float bl[SubBlocSize];
} bloc_struct;
//------------------------------------------------------------------------

//------------------------------------------------------------------------
typedef enum { RUNH, RUNE, EVTH, EVTE, PART } bloc_type;
bool operator < (const longi_struct & a, const longi_struct & b);
//------------------------------------------------------------------------


//---------------------------------------------------------------------
/*! \function PartGrnd_ROOT* CorsikaPart( int PartCode, float Px, float Py, float Pz, float XX,
       float YY, float TT, float WW);
    \brief This function convert the Corsika particles variables into a
    ROOT format particle with the geometric conventions
 */
//------------------------------------------------------------------------
PartGrnd_ROOT *CorsikaPart( int PartCode, float Px, float Py, float Pz, float XX,float YY, float TT, float WW);
//------------------------------------------------------------------------



bloc_type BlocType(float *A0);

FILE *OpenInput(char *filename);

void NextBloc(FILE * Input, bloc_struct * InputBuffer);

unsigned short EndDetect(float *A0);

void filllongimat(char *filename, char *startstr0,char *endstr0);

unsigned short DetectSubStr(char *targ, char *tested);
int EofInLST(char *filename);

//reads directory in which root files are stored
unsigned short defineoutputdir(char *strout);


#endif
