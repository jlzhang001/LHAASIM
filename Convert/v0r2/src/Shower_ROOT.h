//----------------------------------------------------------------
/*! \file Shower_ROOT.h
    \brief File describing a Shower
    \author Sylvie Dagoret-Campagne according X. Bertou and Gilles S original
    \date July 2001              
 */
//----------------------------------------------------------------

#ifndef SHOWER_ROOT_H
#define SHOWER_ROOT_H
#include "Header_ROOT.h"
#include "ShowerGrnd_ROOT.h"
#include "ShowerProf_ROOT.h"
#include "ShProperties.h"

//----------------------------------------------------------------
/*! \class Shower_ROOT
    \brief Class describing a Shower

 */
//----------------------------------------------------------------

class Shower_ROOT:public TObject
{
private:
  //  Shower_ROOT ();

public:
  
//internal parameters
  TFile * f_;

  HeaderCorsika_ROOT *hc_;
  // HeaderAires_ROOT *ha_;
  Header_ROOT *h_;
  ProgName ProgFlag_;  // 1 Aires, 0 Corsika

  Bool_t exists_;		//true if the shower exists. allows program to exit in the other case without seg. fault

  ShowerGrnd_ROOT* showerGrnd()const{return showerGrnd_; }
  ShowerProf_ROOT* showerProf()const{return showerProf_; }
  ShowerProperties* showerProps()const{return showerProps_;}
  void setshowerProps(ShowerProperties* p){showerProps_=p; }
  void QuickLook ();


  Header_ROOT *GetHeader ();

//constructor
  Shower_ROOT (char *ShowerFullName);
  Shower_ROOT ();


//destructor
  virtual ~Shower_ROOT ();

 private:
  ShowerGrnd_ROOT* showerGrnd_;
  ShowerProf_ROOT* showerProf_;
  ShowerProperties* showerProps_; //! not saved in root file

ClassDef (Shower_ROOT, 1)
};
//----------------------------------------------------------------------


#endif
