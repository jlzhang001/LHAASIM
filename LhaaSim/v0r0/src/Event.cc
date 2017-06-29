
/*
  Event.cc

  implementation file for all class Event

*/

#include "Event.h"
#include "LhaaSim.h"
#include "Utils.h"

ClassImp(Event)

//----------------------------------------------------------------
/*
  class Event (parent class)
  A simulated event.
*/
//----------------------------------------------------------------

Event::Event(){};

Event::Event(Int_t primary,Double_t energy,Double_t theta ,Double_t azim)
{
  fPrimary = primary;
  fEnergy = energy;
  fTheta = theta;
  fAzim = azim ;
  fEasCore = 0;
  fNorCore = 0;
  fXCore = 0;
  fYCore = 0;
  fZCore = 0;
  fT3Algo=fT4Algo=0;
  fNombTank = 0;
  fNombScint = 0;
  fHitStationListMD.clear();
  fHitStationListED.clear();
  if(theConfig()->MuEmMode =="MUEM") theConfig()->MuEmFlag=1;
  else theConfig()->MuEmFlag=0;
}

Event::Event(Int_t primary,Double_t energy,Double_t theta ,Double_t azim,Double_t xcore ,Double_t ycore,Double_t zcore)
{
  fPrimary = primary;
  fEnergy = energy;
  fTheta = theta;
  fAzim = azim ;
  fEasCore = 0;
  fNorCore = 0;
  fXCore = xcore;
  fYCore = ycore;
  fZCore = zcore;
  fT3Algo=fT4Algo=0;
  fNombTank = 0 ;
  fNombScint = 0;
  fHitStationListMD.clear();
  fHitStationListED.clear();
  if(theConfig()->MuEmMode =="MUEM") theConfig()->MuEmFlag=1;
  else theConfig()->MuEmFlag=0;
}

Event::Event(int primary,double energy)
{
  fPrimary = primary;
  fEnergy = energy;
  fTheta = 0;
  fAzim = 0 ;
  fEasCore = 0;
  fNorCore = 0;
  fXCore = 0;
  fYCore = 0;
  fZCore = 0;
  fT3Algo=fT4Algo=0;
  fNombTank = 0 ;
  fNombScint = 0;
  fHitStationListMD.clear();
  fHitStationListED.clear();
  if(theConfig()->MuEmMode == "MUEM") theConfig()->MuEmFlag=1;
  else theConfig()->MuEmFlag=0;
}



Event::Event(Int_t primary,Double_t energy,Double_t theta ,Double_t azim,
             Double_t eas_avrg,Double_t eas_spread, Double_t nor_avrg,
             Double_t nor_spread)
{
  fEasCore = eas_avrg + eas_spread * (2*Rand()-1);
  fNorCore = nor_avrg + nor_spread * (2*Rand()-1);
  UTMToXY(fNorCore,fEasCore,&fXCore,&fYCore);
  fPrimary = primary;
  fEnergy = energy;
  fTheta = theta;
  fAzim = azim ;
  fT3Algo = fT4Algo=0;
  
  if(theConfig()->MuEmMode == "MUEM") theConfig()->MuEmFlag=1;
  else theConfig()->MuEmFlag=0;
}



Event::~Event()
{

}

void Event::SetDepth(Double_t xmax,Double_t x0)
{
  fXmax=xmax;
  fX0=x0;
}




void  Event::CountTanks()
{
  fNombTank = fHitStationListMD.size();
}


void  Event::CountScints()
{
  fNombScint = fHitStationListED.size();
}


void Event::AddTank(HitStationMD *hst)
{
  fHitStationListMD.push_back(*hst);
}

   
void Event::AddScint(HitStationED *hst2)
{
  fHitStationListED.push_back(*hst2);
}


int Event::DoT3Sim(){
  int ntrig=0;
  for(unsigned int nsta=0;nsta< fHitStationListMD.size();nsta++){
    if( fHitStationListMD[nsta].fT2ToT)ntrig++;
    if(ntrig>2){
      fT3Algo=1;
      return(1);
    }
  }
  ntrig=0;
  for(unsigned int nsta=0;nsta< fHitStationListMD.size();nsta++){
    if( fHitStationListMD[nsta].fT2ToT || fHitStationListMD[nsta].fT2Threshold)ntrig++;
    if(ntrig>3){
      fT3Algo=2;
      return(1);
    }
  }
  return (0);
}


int Event::DoT4Sim(){

#define IST4DMIN 2000
#define IST4DMAX 2800  
#define IST4DIST(i,j) sqrt(pow(fHitStationListMD[i].Y()-fHitStationListMD[j].Y(),2.)+pow(fHitStationListMD[i].X()-fHitStationListMD[j].X(),2.))  
  
  fT4Algo = 0;
  
  for ( int i = 0; i < fNombTank; i++) {
    if (!fHitStationListMD[i].fT2ToT) continue;
    
    for ( int j = 0; j <fNombTank; j++) {
      if (j == i) continue;
      if (IST4DIST(i, j) > IST4DMIN) continue;
      if (!fHitStationListMD[j].fT2ToT)continue;
      
      for ( int k = 0; k < fNombTank; k++) {
        if (k == j || k == i) continue;
        if (IST4DIST(i, k) > IST4DMIN) continue;
        if (IST4DIST(j, k) > IST4DMAX) continue;	
        if (!fHitStationListMD[k].fT2ToT)continue;
        // Found a 3TOT
        k = j = i = fNombTank;
        fT4Algo= 1;
      }
      
    }
  }

  if(fT4Algo) return(1);
  
  for (int i = 0; i <fNombTank; i++) {
    
    for ( int j = 0; j <fNombTank; j++) {
      if (j == i)continue;
      if (IST4DIST(i, j) > IST4DMIN) continue;
      if (!fHitStationListMD[j].fT2ToT && !fHitStationListMD[j].fT2Threshold) continue;
      
      for ( int k = 0; k <fNombTank; k++) {
        if (k == i || k == j) continue;
	if (IST4DIST(i, k) > IST4DMIN) continue;
	if (!fHitStationListMD[k].fT2ToT && !fHitStationListMD[k].fT2Threshold) continue;
	
        for (int l = 0; l <fNombTank; l++) {
          if (l == i || l == j || l == k) continue;
          
          if (IST4DIST(i, l) > IST4DMIN) continue;
	  if (!fHitStationListMD[l].fT2ToT && !fHitStationListMD[l].fT2Threshold) continue;
	  
          // Found a 4C1
          l = k = j = i = fNombTank;
          fT4Algo =2;
        }
      }
    }
  }
  
  if(fT4Algo) return(1);
  return(0);
}

