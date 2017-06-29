//----------------------------------------------------------------------
/*!
 \file PartGrnd_ROOT.h
 \brief Header file of Particles at the Ground level
 \author Sylvie Dagoret-Campagne
\date april 2001
 */
//----------------------------------------------------------------------

#ifndef PARTGRND_ROOT_H
#define PARTGRND_ROOT_H
#include "root.h"
#include "ShProperties.h"
#include <cmath>
using namespace std;
//list of known types
enum part_type { ALL=0, GAMMAS=1, POSITRONS=2, ELECTRONS=-2, MUp=3, MUm=-3, HADRONS=31, CHARGED=31, NUCLEI=31, CHERENKOV=1, MUONS=3, EpEm=2, OthMUONS=4 };

//----------------------------------------------------------------
/*!
  \class PartGrnd_ROOT
  \brief A particle at the ground level
  This particle has be produced by an Air Shower simulation code
 */
//----------------------------------------------------------------

class PartGrnd_ROOT:public TObject {
 private:

 public:


  Int_t Id;		/* Aires code */
  
  Float_t UX;
  Float_t UY;
  Float_t UZ;
  
  Float_t X;
  Float_t Y;		/*X,Y : position in m */
  Float_t Z;
  
  Float_t T;		/* T: time of arrival */
  
  Float_t W;		/* W : weight */
  
  Float_t E;		/* En: energy in Gev */


// those methods return data members by direct copy
  int   GetId();
  float GetUX();
  float GetUY();
  float GetUZ();
  float GetX();
  float GetY();
  float GetZ();
  float GetT();
  float GetW();
  float GetE();
// those methods require computations : outputs are derived from data members only
  float GetUR();	//direction projected on ground
  float Phi();
  float Theta();
  float Azim();
  float GetPhiDeg();
  float GetThetaDeg();
  float GetCosTheta();
  float GetAzimDeg();
  float R();
  float GetR();		//same as R()
  float LogR();
  float LogW();
  float LogE();
  float LogT();

  void Print(ostream& s = cout) const;

  ClassDef(PartGrnd_ROOT, 1)
};



ostream& operator<<( ostream& s, const PartGrnd_ROOT& p);



#endif
