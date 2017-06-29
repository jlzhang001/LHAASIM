/*---------------------------------------------------
-  LHAASIM Program - IPN Orsay -                    -
-                                                   - 
-  File ParticleED.h                                -
-                                                   -
---------------------------------------------------*/

#ifndef PARTICLE_H
#define PARTICLE_H

using namespace std;

#include "TObject.h"
#include <vector>
#include <string>

/*------------------------------------------------------------------
  class Particle (parent class)
  A particle at the ground level.
  This particle has be produced by an Air Shower simulation code.
------------------------------------------------------------------*/

class Particle : public TObject 
{
 private:
  
 public:
  
  Int_t fId;            /* Aires code */
  Float_t fUX;
  Float_t fUY;
  Float_t fUZ;
  Float_t fT;           /* T: time of arrival */
  Float_t fT_plane;     /* T: time of  */
  Double_t fE;           /* En: energy in Gev */
  Float_t fWtop;
  Float_t fWside;
  
  Particle();
  Particle(Int_t idpart,Int_t ntop,Int_t nside,Float_t time_plane,
	   Float_t timepart,Float_t energy,Float_t cx,Float_t cy, Float_t cz );
  ~Particle();

  ClassDef(Particle, 1)
    
    };

/*----------------------------------------------------------
  class ParticleInScint
  
  ----------------------------------------------------------*/

class ParticleInScint : public Particle
{
  
 public:

  /* step parameters for charged particles */
  Double_t ElectronEnergyLoss;//!
  Double_t MuonEnergyLoss;//!

  /* step parameters for gammas */
  Double_t GLength; //!
  Double_t GammaEnergyLoss;//!
  Double_t ProbNoInt;//!
  Double_t ProbPair;//!

  Int_t fISta;
  Int_t fMotherId;
  Double_t fX;
  Double_t fY;		/*X,Y : position in m */
  Double_t fZ;  
  Double_t fW;		/* W : weight */
  
  Int_t fNPhot;     /* Number of photons induced */
  Int_t fTPhot;     /* Time of photons induced */
  Int_t fIpm;       /* Number of PMT */
  vector<Double_t> fBinContent;//!

  ParticleInScint();
  ParticleInScint (Int_t ist,Int_t id,Double_t x,Double_t y,Double_t z, 
		   Double_t cx, Double_t cy, Double_t cz,Double_t tim,Double_t en,Double_t weight);
  ParticleInScint (Int_t ist,Int_t id,Int_t motherid,Double_t x,Double_t y,Double_t z, 
		  Double_t cx, Double_t cy, Double_t cz,Double_t tim,Double_t en,Double_t weight);  virtual ~ParticleInScint();
 
 
 void DoEDSim();
 void GeomLength();
 void InteractionProbability();
 void DoGammaProcesses();
 void PairProbability();
 void PairProduction();
 void ComptonScattering();
 void EnergyLoss();
 void GeneratePhoton();
 
  
  ClassDef(ParticleInScint, 1)
    
};
 

#endif
