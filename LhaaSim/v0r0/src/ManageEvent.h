//----------------------------------------------------------------------
/*
  LHAASIM Program - IPN Orsay -
  
  File ManageEvent.h 

 */
//----------------------------------------------------------------------

#ifndef MANAGEEVENT_H
#define MANAGEEVENT_H

#include <TROOT.h> 
#include "Event.h" 
#include "Station.h" 
#include "Constants.h" 

//----------------------------------------------------------------
/*
  Functions to manage a simulated event in mode SHOWER
*/
//----------------------------------------------------------------

Event *GenerateEvent(UInt_t);
void DoSampling(Double_t, Double_t,Int_t, vector<SampStation>*);
void AddRandomParticles(vector<SampStation>*);
HitStationMD *GenerateHitStationMD(const SampStation &);
HitStationED *GenerateHitStationED(const SampStation &);

//----------------------------------------------------------------
/*
  Functions to manage a simulated event in mode CALIB
*/
//----------------------------------------------------------------


#endif
