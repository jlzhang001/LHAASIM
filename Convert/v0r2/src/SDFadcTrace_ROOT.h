//-------------------------------------------
/*! \file SDFadcTrace_ROOT.h
    \brief Header file for a Local Station FADC
    \author Sylvie Dagoret-Campagne
    \date june 2001
    \ingroup DATA

 */
//-------------------------------------------

#ifndef SDFADCTRACE_ROOT_H
#define SDFADCTRACE_ROOT_H

#include "TObject.h"
#include <vector>
#include <map>
#include <iostream>

#define SDFADCBINSIZE 10  // FADC window size in ns

//-------------------------------------------
/*! \class SDFadcTrace_ROOT
    \brief Definition for a Flash ADC trace of a Local Station
    \ingroup DATA

 */
//-------------------------------------------
class SDFadcTrace_ROOT : public TObject
{
 public:
  SDFadcTrace_ROOT(){};
  virtual ~SDFadcTrace_ROOT(){};
  int Slot0_;
  map<int,int> trace_; /**< container of FADC time slot */
  map<int,int> mutrace_; /**< container of FADC time slot for muon component */
  map<int,int> emtrace_; /**< container of FADC time slot for em component */
  void setSlot0(int s0){Slot0_=s0;}
  void Clean();
  void Print(ostream& o=cout) const;
  ClassDef( SDFadcTrace_ROOT,1)
    };
//-------------------------------------------

//-------------------------------------------
ostream& operator<<(ostream& o,const SDFadcTrace_ROOT& f);
//-------------------------------------------
#endif
