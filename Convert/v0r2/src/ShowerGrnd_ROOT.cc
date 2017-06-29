//----------------------------------------------------------------
/*! \file ShowerGrnd_ROOT.cc
    \brief Implementation file of ShowerGrnd_ROOT.h
    \author Sylvie Dagoret-Campagne according X. Bertou and Gilles S original
    \date July 2001         
 */
//----------------------------------------------------------------

	// all methods of class Shower


#include "ShowerGrnd_ROOT.h"
#include "Globals.h"
ClassImp (ShowerGrnd_ROOT);

//--------------------------------------------------------------------
/*! constructor */
ShowerGrnd_ROOT::ShowerGrnd_ROOT (TFile* f)
{
  f_=f;
//particles' ntuple
  Pcles_ = (TTree *) f_->Get ("GrndPcles");	//seting trees addresses
      Branch_ = Pcles_->GetBranch ("GrndPcles");
      NbPart_ = (UInt_t) Pcles_->GetEntries ();
      p_ = new PartGrnd_ROOT ();
      Branch_->SetAddress (&p_);

};
//--------------------------------------------------------------------



//--------------------------------------------------------------------

void ShowerGrnd_ROOT::QuickLook ()
{
  cerr << "\n	 Shower: number of particles in file: " << NbPart_ << endl;

//some info found in HeaderCorsika
/*
  cerr << "Number of weighted Part " << GetHeader ()->GetNbPart ('W') << endl;
  cerr << " Photons " << GetHeader ()->GetNbGAM ('W') << endl;
  cerr << " e+e- " << GetHeader ()->GetNbEp ('W') +
    GetHeader ()->GetNbEm ('W') << endl;
  cerr << " Hadrons " << GetHeader ()->GetNbHad ('W') << endl;
  cerr << " Muons " << GetHeader ()->GetNbMUm ('W') +
    GetHeader ()->GetNbMUm ('W') << endl;
*/

};
//--------------------------------------------------------------------



//--------------------------------------------------------------------
Stat_t ShowerGrnd_ROOT::GetEntries ()
{
  return this->Pcles_->GetEntries ();
};
//--------------------------------------------------------------------




/*
void
Shower::SetPart (UInt_t k)
{
  this->pntpl->GetEntry (k);
  this->p->Id = this->Id;
  this->p->PX = this->PX;
  this->p->PY = this->PY;
  this->p->X = this->X;
  this->p->Y = this->Y;
  this->p->T = this->T;
  this->p->W = this->W;
  this->p->E = this->E;
};
*/


//-----------------------------------------------------------------
PartGrnd_ROOT *ShowerGrnd_ROOT::GetPart (UInt_t k)
{
  //this->SetPart (k);
  this->Pcles_->GetEntry (k);
  //cout << "test id,x,y,W= " <<this->p_->Id<<", "<<this->p_->X<<", "<<this->p_->Y<<", "<<this->p_->W<<endl;
  return this->p_;
};
//--------------------------------------------------------------------




