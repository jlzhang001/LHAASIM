//----------------------------------------------------------------
/*! \file ShowerGrnd_ROOT.h
    \brief File describing a Ground Particle Shower
    \author Sylvie Dagoret-Campagne according X. Bertou and Gilles S original
    \date July 2001              
 */
//----------------------------------------------------------------

#ifndef SHOWERGRND_ROOT_H
#define SHOWERGRND_ROOT_H
#include "Header_ROOT.h"
#include "PartGrnd_ROOT.h"


//----------------------------------------------------------------
/*! \class ShowerGrnd_ROOT
    \brief Class describing a Ground particle Shower

 */
//----------------------------------------------------------------
class ShowerGrnd_ROOT:public TObject
{
 private:
  ShowerGrnd_ROOT ()
  {
    f_ = 0;
    Pcles_ = 0;
    Branch_ = 0;
    p_ = 0;
  };

public:

//internal parameters
  TFile * f_;
  TTree * Pcles_;
  TBranch * Branch_;
//  float Id, PX, PY, X, Y, T, W, E;
  PartGrnd_ROOT *p_;

  UInt_t NbPart_;		//Number of particles in the record

  void QuickLook ();

//particles and header
  PartGrnd_ROOT *GetPart (UInt_t k);
  Stat_t GetEntries ();


//constructor
  ShowerGrnd_ROOT (TFile *);


//destructor
  virtual ~ShowerGrnd_ROOT ()
  {
    //    f->Close ();
  };
ClassDef (ShowerGrnd_ROOT, 1)};
//----------------------------------------------------------------
#endif
