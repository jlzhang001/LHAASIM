//----------------------------------------------------------------
/*! \file Shower_ROOT.cc
    \brief Implementation file of Shower_ROOT.h
    \author Sylvie Dagoret-Campagne according X. Bertou and Gilles S original
    \date July 2001         
 */
//----------------------------------------------------------------


	// all methods of class Shower


#include "Shower_ROOT.h"
#include "Globals.h"
ClassImp (Shower_ROOT);
//--------------------------------------------------------------
/*! constructor */
Shower_ROOT::Shower_ROOT ()
{
    f_ = 0;
    hc_ = 0;
    //  ha_ = 0;
    h_ = 0;
    showerGrnd_=NULL;
    showerProf_=NULL;
    showerProps_=NULL;

}
//--------------------------------------------------------------

//--------------------------------------------------------------
Shower_ROOT::Shower_ROOT (char *ShowerFullName)
{
  f_ = new TFile (ShowerFullName);	//ShowerFullName: root file containing the Shower
  if (! f_->IsZombie())
    {
      exists_ = true;
      // f_->ls();


//header
      h_ = (Header_ROOT *) f_->Get ("Header_ROOT");
      ProgFlag_ = h_->ProgFlag;	//aires or corsika
      switch (ProgFlag_)
	{
	case CORSIKA:		//CORSIKA
	  hc_ = (HeaderCorsika_ROOT *) f_->Get ("Header_ROOT");
	  //	  ha_ = 0;
	  break;

	// case AIRES:		//Aires
	//   ha_ = (HeaderAires_ROOT *) f_->Get ("Header_ROOT");
	//   hc_ = 0;
	//   break;
	default:
	  cerr <<"shower type (aires/corsika) unrecognized. Check the header "<< endl;
	  break;
	};

      showerGrnd_=new ShowerGrnd_ROOT(f_);

    }
  else exists_ = false;
}
//--------------------------------------------------------------


//--------------------------------------------------------------
Shower_ROOT:: ~Shower_ROOT ()
  {
    f_->Close ();
    if(showerGrnd_!=NULL) delete showerGrnd_;
    if(showerProps_!=NULL) delete showerProps_;
  }
//--------------------------------------------------------------


//--------------------------------------------------------------
void Shower_ROOT::QuickLook ()
{
  cerr << " PrimaryType (Aires Code) " <<
    GetHeader ()->GetPrimaryType () << endl;
  cerr << " En " << GetHeader ()->GetPrimaryEnGev () << " Gev" << endl;
  cerr << "Thinning: " << GetHeader ()->GetPrimaryThin () << endl;
  cerr << " Primary's Theta " << GetHeader ()->GetPrimaryZenAng () <<
    " rad <=>" << RAD2DEG *
    GetHeader ()->GetPrimaryZenAng () << " degree " << endl;
  cerr << " Primary's Phi " << GetHeader ()->GetPrimaryAzAng () << " rad <=>"
    << RAD2DEG * GetHeader ()->GetPrimaryAzAng () << " degree " << endl;


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

cerr << "this shower was produced by " << h_->
    MotherProgram << " on " << h_->GetDateOfCreation () << endl;
}
//--------------------------------------------------------------



//--------------------------------------------------------------
Header_ROOT *Shower_ROOT::GetHeader ()
{
  return h_;
}

//--------------------------------------------------------------



