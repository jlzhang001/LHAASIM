//---------------------------------------------------------------
/*! \file HeaderCorsika_ROOT.cc
     \brief Implementation file for HeaderCorsika_ROOT.h  
     \ingroup SHOWER
 */
//---------------------------------------------------------------



//  methods of class HeaderCorsika


#include "HeaderCorsika_ROOT.h"
#include "IOCorsika.h"
#include "CorsikaConst.h"
ClassImp(HeaderCorsika_ROOT);

extern RotToAuger rot;

//------------------------------------------------------------
//! constructor
HeaderCorsika_ROOT::HeaderCorsika_ROOT(float *A)
{
  unsigned int i = 0;
  NEv = (unsigned int) A[1];	//Event number

  //  PrimaryType = CorsToAiresTab[(int) A[2]];	//particle id (AIRES code)
  if ( A[2] < 77 ) {
    PrimaryType = CorsToAiresTab[(int) A[2]];	
  }
  else if (A[2] >=100) {
    PrimaryType = (108 + 30 * ( (int) A[2] %100) + ( (int) (A[2]/100)) ); 
  }
  else {
    PrimaryType = 0;
  }

  PrimEnGev = A[3];	// total energy (gev)
  StartAlt = A[4];	//starting altitude (g/cm2)
  NbOfFirstTarg = (unsigned int) A[5];	//number of first target
  ZcoordFI = A[6];	// z coordinate of first interaction

  // in Corsika frame :
  PrimPx = A[7];		// px (Gev)
  PrimPy = A[8];		//py
  PrimPz = A[9];		//pz
  ZenAng = A[10];		// zen. angle (rad)
  AzAng = A[11];		//azimuth angle(rad)

  // in Augerframe
  double corsPx = PrimPx;
  double corsPy = PrimPy;
  double corsPz = PrimPz;


  // rotation of momentum
  rot.rotMoment(corsPx,corsPy,corsPz);
  PrimPx=float(corsPx);
  PrimPy=float(corsPy);
  PrimPz=float(corsPz);

 
  // rotation of azimuth
   AzAng=rot.rotAzimuth(AzAng);
  
  NDRNS = (unsigned int) A[12];	//nb of different rand. seq
  for (i = 1; i < 11; i++) {
    ISSI[i - 1] = (unsigned int) A[i * 3 + 10];
    NOFRCM[i - 1] = (unsigned int) A[i * 3 + 11];
    NOFRCO[i - 1] = (unsigned int) A[i * 3 + 12];
  };
  
  NObsL = (unsigned int) A[46];
  for (i = 1; i < 11; i++)
    HObsL[i - 1] = A[46 + i];
  
  CHKG = A[60];
  CMKG = A[61];
  CEKG = A[62];
  CPKG = A[63];
  
  NFLAIN = (int) A[64];
  NFLDIF = (int) A[65];
  NFLPI0 = (int) A[66];
  NFLPIF = (int) A[67];
  NFLCHE = (int) A[68];
  NFRAGM = (int) A[69];
  
  EMFx = A[70];
  EMFz = A[71];
  EGS4 = (unsigned int) A[72];
  NKG = (unsigned int) A[73];
  GHEISHA = (unsigned int) A[74];
  VENUS = (unsigned int) A[75];
  CERENKOV = (unsigned int) A[76];
  NEUTRINO = (unsigned int) A[77];
  CURVED = (unsigned int) A[78];

  SIBYLL = (unsigned int) A[138];
  SIBYLLC = (unsigned int) A[139];
  QGSJET = (unsigned int) A[140];
  QGSJETC = (unsigned int) A[141];
  DPMJET = (unsigned int) A[142];
  DPMJETC = (unsigned int) A[143];
  VENUSC = (unsigned int) A[144];
  MUSF = (unsigned int) A[145];
  //  EFRCTHN = A[146];
  // NKGRD = A[147];
  EFRCTHN = A[147];
  NKGRD = A[146];
  float *B = A + 312;	// EVTE bloc

  WNPh = B[2];
  WNEl = B[3];
  WNHad = B[4];
  WNMu = B[5];
  WNPart = B[6];


  for (i = 1; i < 22; i++) {
    LDX1[i - 1] = B[i + 6];
    LDY1[i - 1] = B[i + 27];
    LDXY1[i - 1] = B[i + 48];
    LDYX1[i - 1] = B[i + 69];
    LDX2[i - 1] = B[i + 90];
    LDY2[i - 1] = B[i + 111];
    LDXY2[i - 1] = B[i + 132];
    LDYX2[i - 1] = B[i + 153];
  };

  for (i = 1; i < 11; i++) {
    EN[i - 1] = B[i + 174];
    Ag[i - 1] = B[i + 184];
    DED[i - 1] = B[i + 194];
    LAg1[i - 1] = B[i + 204];
    HLEg[i - 1] = B[i + 214];
    HLEc[i - 1] = B[i + 224];
    DB[i - 1] = B[i + 234];
    LAg2[i - 1] = B[i + 244];
  };
  for (i = 1; i < 7; i++)
    PARLGDC[i - 1] = B[i + 254];
  CHI2LD = B[261];
  
  TimeOffset = 0;
  NbPart = 0;
  NbWPart = 0;

  NbGAM = 0;
  NbWGAM = 0;

  NbEm = 0;
  NbWEm = 0;
  
  NbEp = 0;
  NbWEp = 0;

  NbMUp = 0;
  NbWMUp = 0;

  NbMUm = 0;
  NbWMUm = 0;

  NbHad = 0;
  NbWHad = 0;

}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetTimeOffset()
{
  return TimeOffset;
}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
char *HeaderCorsika_ROOT::GetDateOfCreation()
{
  return DateOfCreation;
}
//-----------------------------------------------------------------

//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetPrimaryType()
{
  return this->PrimaryType;
}
//-----------------------------------------------------------------

//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetPrimaryEnGev()
{
  return this->PrimEnGev;
}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetPrimaryThin()
{
  return this->EFRCTHN;
}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetPrimaryZenAng()
{
  return this->ZenAng;
}
//-----------------------------------------------------------------

//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetPrimaryAzAng()
{
  return this->AzAng;
}
//-----------------------------------------------------------------

//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetPrimaryPx()
{
  return this->PrimPx;
}
//-----------------------------------------------------------------

//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetPrimaryPy()
{
  return this->PrimPy;
}
//-----------------------------------------------------------------

//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetPrimaryPz()
{
  return this->PrimPz;
}
//-----------------------------------------------------------------

//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetNbGAM(char W)
{
  if (W == 'W')
    return this->NbWGAM;
  else
    return this->NbGAM;
}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetNbEm(char W)
{
  if (W == 'W')
    return this->NbWEm;
  else
    return this->NbEm;
}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetNbEp(char W)
{
  if (W == 'W')
    return this->NbWEp;
  else
    return this->NbEp;
}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetNbMUp(char W)
{
  if (W == 'W')
    return this->NbWMUp;
  else
    return this->NbMUp;
}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetNbMUm(char W)
{
  if (W == 'W')
    return this->NbWMUm;
  else
    return this->NbMUm;
}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetNbHad(char W)
{
  if (W == 'W')
    return this->NbWHad;
  else
    return this->NbHad;
}
//-----------------------------------------------------------------

//-----------------------------------------------------------------
float HeaderCorsika_ROOT::GetNbPart(char W)
{
  if (W == 'W')
    return this->NbWPart;
  else
    return this->NbPart;
}
//-----------------------------------------------------------------

