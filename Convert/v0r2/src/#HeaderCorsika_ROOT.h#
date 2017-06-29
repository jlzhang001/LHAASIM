//-------------------------------------------------------------------
/*! \file HeaderCorsika_ROOT.h
    \brief Describe the inhalt of a Header of a shower produced by Corsika
    \ingroup SHOWER
 */
//-------------------------------------------------------------------

#ifndef HEADERCORSIKA_ROOT_H
#define HEADERCORSIKA_ROOT_H
#include "root.h"
#include "Header_ROOT.h"
#include "PartGrnd_ROOT.h"
//#include "IOCorsika.h"
const unsigned int MAXLONGI = 500;	//max number of steps for longitudinal dev.

//--------------------------------------------------------------
/*! \class  HeaderCorsika_ROOT
    \brief Header for shower produced by Corsika
    \ingroup SHOWER
 */
//--------------------------------------------------------------
class HeaderCorsika_ROOT:public Header_ROOT { private:
 public:
  char DateOfCreation[50];
  unsigned int NEv;	//Event number
  int PrimaryType;	//particle id
  float PrimEnGev;	// total energy (gev)
  float StartAlt;		//starting altitude (g/cm2)
  unsigned int NbOfFirstTarg;	//number of first target
  float ZcoordFI;		// z coordinate of first interaction
  float PrimPx;		// px (Gev)
  float PrimPy;		//py
  float PrimPz;		//pz
  float ZenAng;		// zen. angle (rad)
  float AzAng;		//azimuth angle(rad)
  unsigned int NDRNS;	//nb of different rand. seq
  unsigned int ISSI[10];	//integer seed of seq. i
  unsigned int NOFRCM[10];	//nb of offset rand. calls (mod 1e6) of sequence i
  unsigned int NOFRCO[10];	//nb of offset rand. calls (/1e6) of sequence i
  unsigned int NObsL;	//number of observation levels 
  float HObsL[10];	// height of lev. i(cm)
  
  float CHKG;		//cutoff for hadr. cin. en
  float CMKG;		//cutoff for muons cin. en
  float CEKG;		//cutoff for  el. cin. en
  float CPKG;		//cutoff for  phot. cin. en

  
  int NFLAIN;
  int NFLDIF;
  int NFLPI0;
  int NFLPIF;
  int NFLCHE;
  int NFRAGM;

  float EMFx;		//Earth's mag. field (microT), x cmp.
  float EMFz;		//Earth's mag. field (microT), z cmp.
  unsigned int EGS4;	//flag for act. EGS4
  unsigned int NKG;	//flag for NKG
  unsigned int GHEISHA;	//flag for Gheisha
  unsigned int VENUS;	//flag for venus(1)/nexux(2) 
  unsigned int CERENKOV;	//flag for cerenkov
  unsigned int NEUTRINO;	//flag for neutrino
  unsigned int CURVED;	//0=standard,1=horiz,2=curved

  unsigned int SIBYLL;	//sibyll int. flag
  unsigned int SIBYLLC;	//sibyll cross sect. flag
  unsigned int QGSJET;	//qetint. flag
  unsigned int QGSJETC;	//qgsjet cross sect. flag
  unsigned int DPMJET;	//dpmjet int. flag
  unsigned int DPMJETC;	//dpmjet cross sect. flag
  unsigned int VENUSC;	//venus/nex cross  sec flag (1=VENUSSIG,2=NEXUSSIG)
  unsigned int MUSF;	//muon multiple scat. flag (1=moliere, 0=gauss)
  float EFRCTHN;		//energy fraction of thinning level
  float NKGRD;		//rad distr. range cm


//copoied from EVTE
  float WNPh;		//weighted nb of photons 
  float WNEl;		//weighted nb of electrons
  float WNHad;		//weighted nb of hadrons
  float WNMu;		//weighted nb of muons
  float WNPart;		//weighted nb of part

//counted by CorsToRoot
  float NbPart;
  float NbWPart;

  float NbGAM;
  float NbWGAM;
  
  float NbEm;
  float NbWEm;

  float NbEp;
  float NbWEp;

  float NbMUp;
  float NbWMUp;

  float NbMUm;
  float NbWMUm;
  
  float NbHad;
  float NbWHad;
  

  float LDX1[21];		//lat. distr x level 1 (cm-2)
  float LDY1[21];		//lat. distr y level 1 (cm-2)
  float LDXY1[21];	//lat. distr xy level 1 (cm-2)
  float LDYX1[21];	//lat. distr yx level 1 (cm-2)


  float LDX2[21];		//lat. distr x level 2 (cm-2)
  float LDY2[21];		//lat. distr y level 2 (cm-2)
  float LDXY2[21];	//lat. distr xy level 2 (cm-2)
  float LDYX2[21];	//lat. distr yx level 2 (cm-2)
	
  float EN[10];		//el. numb. in steps of 100 g/cm2
  float Ag[10];		//age in steps of 100 g/cm2
  float DED[10];		// dist. for el. distr. i cm
  float LAg1[10];		// local age 1. lev

  float HLEg[10];		// height of lev for el nb g/cm2
  float HLEc[10];		// height of lev for el nb cm
  float DB[10];		//dist bins for local age in cm
  float LAg2[10];		// local age lev. 2
  
  float PARLGDC[6];	//param. of long. distr. for charged p.
  float CHI2LD;		//chi2 fit for long. dist

  HeaderCorsika_ROOT(float *A);	//constructor called by SplitBFile.cc and corsika
  float TimeOffset;
  float GetPrimaryType();
  float GetPrimaryEnGev();
  float GetPrimaryThin();
  float GetPrimaryZenAng();
  float GetPrimaryAzAng();
  float GetPrimaryPx();
  float GetPrimaryPy();
  float GetPrimaryPz();

  float GetTimeOffset();
  char *GetDateOfCreation();


  float GetNbGAM(char W);
  float GetNbEp(char W);
  float GetNbEm(char W);
  float GetNbMUp(char W);
  float GetNbMUm(char W);
  float GetNbHad(char W);
  float GetNbPart(char W);


  HeaderCorsika_ROOT() {
  };
  virtual ~HeaderCorsika_ROOT() {
  };

  ClassDef(HeaderCorsika_ROOT, 1)
};
//------------------------------------------------------------------
#endif
