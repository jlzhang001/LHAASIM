//=========================================================================== 
//
// Header-File  : FDSim_ROOT.h (This file is part of the FDSim++ package)
//
// Date         : Mon Nov 06 2000
//
// Author       : Luis Prado Jr.
//                State University at Campinas
//
// E-Mail       : pradojr@ifi.unicamp.br
//
// Description  : Header-file of the FDSim_ROOT. 
// 
//=========================================================================== 



// ROOT Description

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// FDSim_ROOT                                                           //
//                                                                      //
// Description of the FDSim_ROOT for ROOT-format I/O                    //
// manipulations.                                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef SHOWERPROF_ROOT_H
#define SHOWERPROF_ROOT_H

#include "TObject.h"
#include "TVector.h"

#include <iostream>


//! Class definition of the ShowerProf_ROOT object.
/*!
 *  This object is defined for I/O manipulations in ROOT(CERN) format.
 */
class ShowerProf_ROOT : public TObject {
  
 private:
  Float_t    fChi0;              // chi0  
  Float_t    fChimax;            // chimax
  Int_t      fNShwPts;           // number of shower points
  TVector   *fShwPts;            // shower levels (g/cm2)
  TVector   *fLogPts;            // log of number of charged particles
  TVector   *fLogEPts;           // log of energy deposit  
 public:
  //! Constructor of the ShowerProfile_ROOT class.
  ShowerProf_ROOT();
  //! Destructor of the ShowerProfile_ROOT class.
  virtual ~ShowerProf_ROOT();
  
  void      SetChi0(Float_t f)     { fChi0 = f; }
  void      SetChimax(Float_t f)   { fChimax = f; }
  void      SetNShwPts(Int_t n)    { fNShwPts = n; }
  
  void      SetShwPts(TVector *sp) 
                  { fShwPts->ResizeTo(sp->GetNrows()); *fShwPts = *sp; }
  void      SetLogPts(TVector *lp) 
                  { fLogPts->ResizeTo(lp->GetNrows()); *fLogPts = *lp; }
  void      SetLogEPts(TVector *sp) 
                  { fLogEPts->ResizeTo(sp->GetNrows()); *fLogEPts = *sp; }
  

  Float_t   GetChi0()     const { return fChi0; }
  Float_t   GetChimax()   const { return fChimax; }
  Float_t   GetNShwPts()  const { return fNShwPts; }
  
  TVector*  GetShwPts() const { return fShwPts; }
  TVector*  GetLogPts() const { return fLogPts; }
  TVector*  GetLogEPts() const { return fLogEPts; }
  
  ClassDef(ShowerProf_ROOT,1)  //A shower information
};
    


#endif


