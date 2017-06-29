//=========================================================================== 
//
// Header-File  : FDSim_ROOT.cpp (This file is part of the FDSim++ package)
//
// Date         : Mon Oct 23 2000
//
// Author       : Luis Prado Jr.
//                State University at Campinas
//
// E-Mail       : pradojr@ifi.unicamp.br
//
// Description  : FDSim_ROOT methods. 
// 
//=========================================================================== 



// ROOT Description

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FDSim_ROOT                                                           //
//                                                                      //
// Description of the FDSim_ROOT object for ROOT-format I/O             //
// manipulations.                                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////



#include "ShowerProf_ROOT.h"

ClassImp(ShowerProf_ROOT)



ShowerProf_ROOT::ShowerProf_ROOT() : TObject()
{
  fShwPts  = new TVector();        
  fLogPts  = new TVector();        
  fLogEPts  = new TVector();        
}

ShowerProf_ROOT::~ShowerProf_ROOT()
{
  delete fShwPts;        
  delete fLogPts;        
  delete fLogEPts;        
}

void ShowerProf_ROOT::Streamer(TBuffer &R__b)
{
  // Stream an object of class ShowerProf.
  
  if (R__b.IsReading()) 
    {
      Version_t R__v = R__b.ReadVersion(); 
      if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> fChi0;
      R__b >> fChimax;
      R__b >> fNShwPts;
      R__b >> fShwPts;
      R__b >> fLogPts;
      R__b >> fLogEPts;
    } 
  else 
    {
      R__b.WriteVersion(ShowerProf_ROOT::IsA());
      TObject::Streamer(R__b);
      R__b << fChi0;
      R__b << fChimax;
      R__b << fNShwPts;
      R__b << fShwPts;
      R__b << fLogPts;
      R__b << fLogEPts;
    }
}



