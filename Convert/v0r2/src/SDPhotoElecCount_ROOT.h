//-------------------------------------------
/*! \file SD_PhotoElecCount_ROOT.h
    \brief Header file for Photoelectrons of a Local Station Pm
    \author Sylvie Dagoret-Campagne
    \date june 2001
    \ingroup DATA
 */
//-------------------------------------------

#ifndef _SDPHOTOELCOUNT_H
#define _SDPHOTOELCOUNT_H

#include "TObject.h"
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <utility>

const int SDPHELBINSIZE = 1; // bin size of photoelectron histogram

/**
 * \defgroup DATA The Data structure
 * Structure in which the results of the simulation are written 
 */



//--------------------------------------------------------------
/*! \class SDPhotoElecCount_ROOT
  \brief Definition of the photoelectrons of a Local Station Pm

    \ingroup DATA

 */
//--------------------------------------------------------------
class SDPhotoElecCount_ROOT : public TObject
{
 public:
  SDPhotoElecCount_ROOT():Slot0_(0),total_(0),totalem_(0),totalmuon_(0){}
  SDPhotoElecCount_ROOT(int slot0):Slot0_(slot0),total_(0),totalem_(0),totalmuon_(0) {}
  virtual ~SDPhotoElecCount_ROOT(){};
  int Slot0_; /**< first timebin corresponding to the arrival of the wave front */
  int total_; /**< total number of photoelectrons */
  int totalem_;
  int totalmuon_;
  map<int,int> timepattern_; /**< container for photoelectrons */
  void setSlot0(int s0){Slot0_=s0;}
  void Print(ostream& o=cout) const;
  void addphotoelectron(int timslot);
  void addphotoelfrommuon(int timslot);
  void addphotoelfromem(int timslot);
  void addNphotoelectron(int timslot,unsigned int n);
  void addNphotoelfrommuon(int timslot,unsigned int n);
  void addNphotoelfromem(int timslot,unsigned int n);
  void Clean();
  map<int,int> mutimepattern_;/**< container for photoelectrons induced by muon component */
  map<int,int> emtimepattern_;/**< container for photoelectrons induced by gamma/e+e- */

ClassDef(SDPhotoElecCount_ROOT,1)
  };
//--------------------------------------------------------------

//--------------------------------------------------------------
ostream& operator<<(ostream& o,const SDPhotoElecCount_ROOT& phe);
//--------------------------------------------------------------

#endif
