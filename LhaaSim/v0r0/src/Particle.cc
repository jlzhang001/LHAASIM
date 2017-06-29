/*------------------------------------------------
-               Particle.cc  (v6r0)              -
-                                                -
- implementation file for all classes Particle   -
------------------------------------------------*/



#include "Particle.h"
#include "LhaaSimConfig.h"
#include "BuildProcessesTables.h"
#include "Utils.h"
#include "Constants.h"
#include <math.h>

ClassImp(Particle)
ClassImp(ParticleInTank)
ClassImp(ParticleInScint)

extern double gCherFact;
extern double gCherPrb[NWAVEL];
extern double gNpesamples;
extern Int_t gNshapezones;
extern Int_t gNshape;
extern Int_t gShapezone[MAXNUMBEROFADCBINS];
extern double gIntshape[MAXNUMBEROFADCBINS];
extern Int_t gNbins;
extern double gTMinForDeltaRays;
extern double gBremTable[NSTEP_E][NDIV];

enum
  {
    STOP = 0,
    CONTINUE = 1,
    LAST
  };

// Affiche les "cout" si PrintCout = 1, et les masque si = 0
const Int_t PrintCout = 0;

/*-------------------------------------------------
  class Particle (parent class)                   -
  A particle at the ground level.                 -
  This particle has be produced                   -          
  by an Air Shower simulation code.               -
--------------------------------------------------*/

Particle::Particle()
{

}



Particle::Particle(Int_t idpart,Int_t ntop,Int_t nside,Float_t time_plane,
		   Float_t timepart,Float_t energy,Float_t cx,Float_t cy, Float_t cz )
{
  fId = idpart;
  fUX = cx;
  fUY = cy;
  fUZ = cz;
  fT = timepart;
  fT_plane = time_plane;
  fE = energy;
  fWtop = ntop;
  fWside = nside;
}

Particle::~Particle()
{

}


/*-------------------------------------------------                              
  class ParticleInTank                            -
  Last modification: July 2nd, 2004               -
  by Isabelle and Carla                           -
                                                  -
 Première approximation :                         -
 -> partie EM absorbée par Terre                  -
 -> muons non affectés                            -
                                                  -
  For Detailed Mode:                              -
  ------------------                              -
  * Muon Simulation:                              -
  Cherenkov production                            -
  Delta Rays                                      -
  Muon Decay                                      -
                                                  -
  * Electron Simulation:                          -
  Cherenkov production                            -
  MultipleScattering                              -
  Bremsstrahglung                                 -
  Ionization lost                                 -
                                                  -
  * Gamma Simulation:                             -
  Compton Scattering                              -
  Pair Production                                 -
                                                  -
  For Fast Mode:                                  -
  --------------                                  -
  Only Cherenkov production but NO WORKING !!!    - 
                                                  -
-------------------------------------------------*/


ParticleInTank::ParticleInTank()
{

}

ParticleInTank::ParticleInTank (Int_t ist,Int_t id,double x,double y,double z, 
				double cx,double cy,double cz,double tim,double en,double weight)
  //  : Particle(id, 0,0,0, tim, en, cx, cy, cz)
{
  fId = id;
  fUX = cx;
  fUY = cy;
  fUZ = cz;
  fT = tim;
  //fT_plane = time_plane;
  fE = en;

  fISta = ist;
  fX = x;
  fY = y;
  fZ = z;
  fW = weight;
  Beta = 0;
  Range = 0;
  DxStep = 0;
  IStep = 0;
  GLength = 0;
  MeanIntLength = 0;
  IntLength = 0;
  ProbPair = 0;
  fNCherPhot = 0;
  fTCherPhot.clear();
  fNReflexions.clear();
  fIpm.clear();
  fBinContent.clear();

}
ParticleInTank::ParticleInTank (Int_t ist,Int_t id,Int_t motherid,double x,double y,double z, 
				double cx,double cy,double cz,double tim,double en,double weight)
  //  : Particle(id, 0,0,0, tim, en, cx, cy, cz)
{
  fId = id;
  fMotherId =  motherid;
  fUX = cx;
  fUY = cy;
  fUZ = cz;
  fT = tim;
  //fT_plane = time_plane;
  fE = en;
  fISta = ist;
  fX = x;
  fY = y;
  fZ = z;
  fW = weight;
  Beta = 0;
  Range = 0;
  DxStep = 0;
  IStep = 0;
  GLength = 0;
  MeanIntLength = 0;
  IntLength = 0;
  ProbPair = 0;
  fNCherPhot = 0;
  fTCherPhot.clear();
  fNReflexions.clear();
  fIpm.clear();
  fBinContent.clear();

}

ParticleInTank::~ParticleInTank()
{

}


// -------------------------------------------------
// DETECTOR SIMULATION FOR EACH PARTICLE           -
// -------------------------------------------------



/* --------------------------------------------------
   Manager of the step by step propagation of each  -
   particle in a tank. In each step, determine the  -
   length and generate Cherenkov photons.           -        
   ------------------------------------------------*/
void ParticleInTank::DoDetSim()  
{
  //if(PrintCout == 1) cout<<"DoDetSim"<<endl;
  if(abs(fId) == 1) {GeomLength(); DoProcesses();}
 
  else if (abs(fId) == 2 || abs(fId) == 3) {
    GeomLength();
    Range=GLength;
    while ( DoNextStep() == CONTINUE ) {
      ComputeStepParameters();
      if ( DoProcesses() == STOP ) break;
      Propagate();
    }
  } else return;
}


/*  ------------------------------------
    Computes the distance between the  -
    particle and the tank.             -
    ----------------------------------*/
void ParticleInTank::GeomLength()
{
  //if(PrintCout == 1) cout<<"geomlength"<<endl;
  Double_t aa,bb,cc;
  aa = fUX*fUX+fUY*fUY;
  bb = fX*fUX+fY*fUY;
  cc = STATION_RADIUS*STATION_RADIUS-fX*fX-fY*fY;
  //   length up to the external cylinder
  if(aa!=0) GLength = (-bb+sqrt(bb*bb+cc*aa))/aa;
  else GLength = INFINITY;
  //   length up to the top or the bottom
  if(fUZ<0) GLength = min(GLength,-fZ/fUZ);
   else if(fUZ!=0)
     GLength = min(GLength,(STATION_HEIGHT-fZ)/fUZ);
   else  GLength = 2*(fX*fUX+fY*fUY);
 
  return;
}


/*-----------------------------------------
   Decides if the step by step tracking   -
   must be continued or stopped.          -
-----------------------------------------*/

 int ParticleInTank::DoNextStep()
{
  if(PrintCout == 1) cout<<"next step"<<endl;
  int ret = 0;
  IStep = 0;
 
  switch (abs(fId)) {

  case 1:
    
    break;

  case 2:
    //  /*
    while (fE <= ESTEP_E[IStep]) IStep++;
      ret = (IStep<NSTEP_E && Range>=0);
      //  */
    break;

  case 3:
    while (fE <= ESTEP_MU[IStep]) IStep++;
    ret = (IStep <= NSTEP_MU && Range>0);
    break;

  default:
    cerr << "Erreur dans ParticleInTank::DoNextStep..." << endl;

  }

  if (ret) ret = CONTINUE;
  else ret = STOP;
  return ret;
}

/* ----------------------------------------------

   --------------------------------------------*/

void ParticleInTank::ComputeStepParameters()
{
  double energy;
  //if(PrintCout == 1) cout<<"compute step"<<endl;
   switch (abs(fId)) {

  case 1:

    break;

  case 2:
    ///*    
    if (IStep == 0) { // case energy > 0.1 GeV      
      DxStep=0.2;
      energy=fE;
    } else { // case energy < 0.1 GeV
      DxStep = (fE - ESTEP_E[IStep]) / DEDX_E;
      energy = (ESTEP_E[IStep-1]+ESTEP_E[IStep])/2;
    }    
    GeomLength();
    Range = min(DxStep, GLength);
    Beta = sqrt(energy*(energy+2*MASS_E))/(energy+MASS_E);
    //*/
    break;

  case 3:
    if (IStep == NSTEP_MU) DxStep=fE/DEDX_MU;
    else DxStep = (fE - ESTEP_MU[IStep]) / DEDX_MU;
    GeomLength();
    Range = min(DxStep, GLength);

    if(IStep>0) energy = (ESTEP_MU[IStep-1]+ESTEP_MU[IStep])/2;
    else energy = (10.+ESTEP_MU[IStep])/2.;
    Beta = sqrt(energy*(energy+2*MASS_MU))/(energy+MASS_MU);
    break;

  default:
    cerr << "Erreur dans ParticleInTank::ComputeStepParameters..." << endl;                   

  }  
}


/*--------------------------------------------
-   Simulation of processes in each step.    -
-                                            -
-          For Detailed Mode:                -
-          ------------------                -
-          * Gamma Simulation:               -
-            Pair Production                 -
-            Compton Scattering              -
-                                            -  
-          * Electron Simulation:            -
-            Cherenkov production            -
-            MultipleScattering              -
-            Bremsstrahglung                 -
-            Ionization lost                 -
-                                            -
-          * Muon Simulation:                -
-            Cherenkov production            -
-            Delta Rays                      -
-            Muon Decay                      -   
--------------------------------------------*/

int ParticleInTank::DoProcesses()
{
  double probIntInOneStep;
  double random2011;
  if(PrintCout == 1) cout << "doprocesses" << endl;
  switch (abs(fId)) {

  case 1:
    ///*
    PairProbability();
    //if(PrintCout == 1) cout <<"Dans DoProcesses, "<<"GLength: "<<GLength<<" "<<"IntLength: "<<IntLength<<endl;
    if (GLength < IntLength) return STOP;
    random2011 = Rand();
    //if(PrintCout == 1) cout<<"random2011: "<<random2011<<" "<<"ProbPair: "<<ProbPair<<endl;
    if (random2011 <= ProbPair) {
      PairProduction();
      return STOP;
    } else {
      ComptonScattering();
    }
    //*/
    break;

  case 2:    
    ///*
    if (theConfig()->SimMode == "DETAILED") {
      if(Range>0) CherPhot(Beta,Range); //retire Cherenkov
      MultipleScattering(Beta,Range); //retire multiple scattering
      probIntInOneStep=gBremTable[IStep][0]*(Range*100); //retire Brem 1/2
      if(Rand()<probIntInOneStep) fE -= Bremstrahlung(IStep,Range,Beta); //retire Brem 2/2
      fE -= Range*DEDX_E;
      if (Range < DxStep || fE < EMIN_E) return STOP;
    } else { // fast simulation
      ;
    }
    //*/
    break;

  case 3:    
    if (theConfig()->SimMode == "DETAILED") {
      // if(PrintCout == 1) cout<<"avant cherphot"<<Beta<<" "<<Range<<endl;
      if(Range>0) CherPhot(Beta,Range); //retire Cherenkov
      if(DxStep>0) fE -= GenerateDeltaRays(Beta,Range); //retire les Delta Rays
      while(fE < ESTEP_MU[IStep]) {IStep++;}
      if ( MuonDecay() == 1 ) return STOP; //retire décroissance muon
    } else { // fast simulation
      ;
    }    
    fE = ESTEP_MU[IStep++];
    break;

  default:
    cerr << "Erreur dans ParticleInTank::DoProcesses..." << endl;                   
  }  
 
  return CONTINUE;
}


/*--------------------------------
   Propagation in space and time 
   for each elementar step       
--------------------------------*/

void ParticleInTank::Propagate()
{
  //if(PrintCout == 1) cout<<"propagate"<<endl;
  fX += Range*fUX;
  fY += Range*fUY;
  fZ += Range*fUZ;
  fT += Range/(Beta*CLIGHT);
}



/*---------------------------------------------------------
  Photon processes
---------------------------------------------------------*/

const Double_t kEMinPh = 0.0004;
const Double_t emass = 0.0005;

// Calcul IntLength et ProbPair, utiles dans DoProcesses
// photon -> création de paire ? ou Compton ?

void ParticleInTank::PairProbability()
{
  if(PrintCout == 1) cout <<"Pair Probability"<<endl;
  Double_t e_ph;
  Double_t ph_al;
  Int_t itab;

  if(fE <= kEMinPh) return;

  // position initiale et direction du photon :
  e_ph = fE;

  GeomLength();
  //if(PrintCout == 1) cout << "GLength: " << GLength << endl;

  itab = (Int_t)((log10(e_ph) + 4.1)/0.2);
    if(itab <= 0)  {ph_al = TABINTLENGTH[0]; ProbPair = TABPROBPAIR[0];}
    else{
      if(itab >= (NTAB_PHOT-1)) {ph_al = TABINTLENGTH[NTAB_PHOT-1]; ProbPair = TABPROBPAIR[NTAB_PHOT-1];}
      else{
        double log10Einf = -4.1 + 0.2 * itab;
        double log10Esup = -4.1 + 0.2 * (itab+1);
        ph_al = TABINTLENGTH[itab] + (log10(e_ph) - log10Einf) * (TABINTLENGTH[itab+1] - TABINTLENGTH[itab]) / (log10Esup - log10Einf);
        ProbPair = TABPROBPAIR[itab] + (log10(e_ph) - log10Einf) * (TABPROBPAIR[itab+1] - TABPROBPAIR[itab]) / (log10Esup - log10Einf);
      }
    }
    
   IntLength = -log(Rand()) * ph_al;
   //if(PrintCout == 1) cout <<"Dans PairProbability, IntLength: " <<IntLength<<" "<<"ProbPair: "<<ProbPair<<" "<<"GLength: "<<GLength<<endl;

   return;
 
}

/*-------------------------------------------------------
  Photon -> paire e-/e+                                 -
  Calcul des paramètres de ces particules (e_ph, x,y,z) -
 ------------------------------------------------------*/

void ParticleInTank::PairProduction()
{
  if(PrintCout == 1) cout << "pair production" << endl;
 Double_t xx,yy,zz,ux,uy,uz,e_ph,timr,weight;
 Double_t cc;
 Double_t e1,e2;

  xx = fX;
  yy = fY;
  zz = fZ;
  ux = fUX;
  uy = fUY;
  uz = fUZ;
  e_ph = fE;
  timr = fT;
  weight = fW;

  xx = xx + IntLength * ux;
  yy = yy + IntLength * uy;
  zz = zz + IntLength * uz;
  timr = timr + IntLength / CLIGHT;
  cc = STATION_RADIUS * STATION_RADIUS - xx * xx - yy * yy;


  e1 = (e_ph - 2 * emass) * Rand();
  e2 = (e_ph - 2 * emass) - e1;
  
  
   ParticleInTank * p1 =
     new ParticleInTank(fISta,2,fId,xx,yy,zz,fUX,fUY,fUZ,timr,e1,fW);
   if(PrintCout == 1) cout << "\n Electron: \n" << endl;
   if(PrintCout == 1) cout <<"Stat,Part,MotherPart,énergie: "<<fISta<<","<<"2"<<","<<fId<<","<<e1<<endl;
   p1->DoDetSim();
   if(PrintCout == 1) cout<<"PairProduction part1->fNCherphot MotherPart="<<fNCherPhot<<endl;
   fNCherPhot += p1->fNCherPhot;
   if(PrintCout == 1) cout<<"PairProduction part1->fNCherphot MotherPart+electron="<<fNCherPhot<<endl;
   for(int iph= 0; iph<p1->fNCherPhot; iph++){
     fTCherPhot.push_back(p1->fTCherPhot[iph]);
     fIpm.push_back(p1->fIpm[iph]);
     fNReflexions.push_back(p1->fNReflexions[iph]);    
   }
   delete p1;
   
   ParticleInTank * p2=
     new ParticleInTank(fISta,-2,fId,xx,yy,zz,fUX,fUY,fUZ,timr,e2,fW);
   if(PrintCout == 1) cout << "\n Positron: \n" << endl;
   if(PrintCout == 1) cout <<"Stat,Part,MotherPart,énergie: "<<fISta<<","<<"-2"<<","<<fId<<","<<e2<<endl;
   p2->DoDetSim();
   if(PrintCout == 1) cout<<"PairProduction part2->fNCherphot MotherPart="<<fNCherPhot<<endl;
   fNCherPhot += p2->fNCherPhot;
   if(PrintCout == 1) cout<<"PairProduction part2->fNCherphot MotherPart+positron="<<fNCherPhot<<endl;
   for(int iph = 0;iph<p2->fNCherPhot;iph++){
     fTCherPhot.push_back(p2->fTCherPhot[iph]);
     fIpm.push_back(p2->fIpm[iph]);
     fNReflexions.push_back(p2->fNReflexions[iph]);
   }   
   delete p2;
  
}


/*---------------------------------------------------------
  Photon -> effet Compton                                 -
  Calcul nouveaux paramètres du photon et de l'e- produit -
 --------------------------------------------------------*/

void ParticleInTank::ComptonScattering()
{
  if(PrintCout == 1) cout << "Compton scattering" << endl;
  Double_t xx,yy,zz,ux,uy,uz;
  Double_t hlength;
  Double_t e_ph, emin,smax,efin,sfin;
  Double_t c_dth,s_dth,s_dph,c_dph,dph;
  Double_t uxy,vx,vy,vz,wx,wy,wz;
  Double_t ux_f=0.,uy_f=0.,uz_f=0.,ux_el,uy_el,uz_el;
  Double_t e1, p1, aa, bb, cc;
  Int_t reject;

  xx = fX;
  yy = fY;
  zz = fZ;
  ux = fUX;
  uy = fUY;
  uz = fUZ;
  e_ph = fE;
  //if(PrintCout == 1) cout<<"avant compton "<<e_ph<<" "<<fX<<" "<<fY<<" "<<fZ<<" "<<fUX<<" "<<fUY<<" "<<fUZ<<endl;
  emin = 1/(1/e_ph + 2/emass);
  smax = emin/e_ph + e_ph/emin + pow((1+emass/e_ph-emass/emin),2) - 1;

  reject = 1;
  while(reject)
    {
      efin = emin + Rand()*(e_ph-emin);
      sfin = efin/e_ph + e_ph/efin;
      reject = Rand() > sfin/smax;
    }
  e1 = e_ph - efin;
  
  //if(PrintCout == 1) cout << "if(e1>EMIN_E || efin> kEMinPh)" << "  " << "(e1,EMIN_E,efin,kEMinPh) = " << e1 << "," << EMIN_E << "," << efin << "," << kEMinPh << endl;

  // if needed (active photon or electron after scattering) compute final
  if(e1>EMIN_E || efin> kEMinPh) {
    //diffusion angles of the photon
    c_dth = 1 + emass*(1/e_ph-1/efin);
    s_dth = sqrt(1 - c_dth*c_dth);
    dph = 2*PI*Rand();
    c_dph = cos(dph);
    s_dph = sin(dph);
    //new direction cosines of the photon
    uxy = sqrt(ux*ux+uy*uy);
    if(uxy != 0){
      vx = -uy/uxy;
      vy = ux/uxy;
      vz = 0.;
      wx = uy*vz-uz*vy;
      wy = uz*vx-ux*vz;
      wz = ux*vy-uy*vx;
    }
    else
      {
	vx = 1;
	vy = 0;
	vz = 0;
	wx = 0;
	wy = 1;
	wz = 0;
      }
    
    ux_f = c_dth*ux+s_dth*(c_dph*vx+s_dph*wx);
    uy_f = c_dth*uy+s_dth*(c_dph*vy+s_dph*wy);
    uz_f = c_dth*uz+s_dth*(c_dph*vz+s_dph*wz);
    //if(PrintCout == 1) cout << "c_dth,s_dth,dph,c_dph,s_dph,uxy,vx,vy,vz,wx,wy,wz,ux_f,uy_f,uz_f: " << c_dth << "," << s_dth << "," << dph << "," << c_dph << "," << s_dph << "," << uxy << "," << vx << "," << vy << "," << vz << "," << wx << "," << wy << "," << wz << "," << ux_f << "," << uy_f << "," << uz_f << endl;
  }
  //  scattered electron
  //VERSION 1 :
  if(e1>EMIN_E) {
    p1 = sqrt(2*emass*e1+e1*e1);
    ux_el = (e_ph*ux-efin*ux_f)/p1;
    uy_el = (e_ph*uy-efin*uy_f)/p1;
    uz_el = (e_ph*uz-efin*uz_f)/p1;
    //if(PrintCout == 1) cout << "p1,fX,fY,fZ,xx,yy,zz,ux_el,uy_el,uz_el: " << p1 << "," << fX << "," << fY << "," << fZ << "," << xx << "," << yy << "," << zz << "," << ux_el << "," << uy_el << "," << uz_el << endl;
    ParticleInTank * pel=
      new ParticleInTank(fISta,2,fId,xx,yy,zz,ux_el,uy_el,uz_el,fT,e1,fW);
    if(PrintCout == 1) cout << "Stat,Part,MotherPart,énergie: " << fISta << "," << "2" << "," << fId << "," << e1 << endl;
    pel->GeomLength();
    //if(PrintCout == 1) cout << "GLength = " << GLength << endl;
    pel->DoDetSim();
    //if(PrintCout == 1) cout << "GLength = " << GLength << endl;
    if(PrintCout == 1) cout << "ComptonScattering->fNCherphot MotherPart = " << fNCherPhot << endl;
    fNCherPhot += pel->fNCherPhot;
    if(PrintCout == 1) cout <<"ComptonScattering->fNCherphot MotherPart+électron="<<fNCherPhot<<endl;
    for(int iph= 0;iph<pel->fNCherPhot;iph++){
      fTCherPhot.push_back(pel->fTCherPhot[iph]);
      fIpm.push_back(pel->fIpm[iph]);
      fNReflexions.push_back(pel->fNReflexions[iph]);
    }
    delete pel;
    
  }
  
  //scattered photon
   
  if(efin > kEMinPh)
    {
      e_ph = efin;
      ux = ux_f;
      uy = uy_f;
      uz = uz_f;
    }
  
}


/*----------------------------------------------------------
   Muon processes
----------------------------------------------------------*/
 
Double_t  ParticleInTank::GenerateDeltaRays(Double_t beta, Double_t xstep)
{
  if(PrintCout == 1) cout << "delta ray" << endl;
  // step in m and T in Gev
  double gamma = 1/sqrt(1.-beta*beta);
  double Tmax = MASS_E* beta* beta*gamma*gamma;
  double meanNbDR = DELTARAYCONST*xstep/beta*(1./gTMinForDeltaRays-1./Tmax);
  int nbDR = (int)Rand("POISSON",meanNbDR);
  //int nbDR = (int)meanNbDR;
  //cout << nbDR << endl;
  // cout <<  DELTARAYCONST/beta*(1./gTMinForDeltaRays-1./Tmax) << " " << xstep << " " << nbDR << endl;
  double DE = 0;
  
  //cout << "T " << gTMinForDeltaRays << " " << Tmax << endl;
  for(int n=0; n < nbDR; n++)
    {
      double invT = 1./gTMinForDeltaRays-Rand()*(1./gTMinForDeltaRays-1./Tmax);
      double T = 1/invT;
      double costheta = T/Tmax*sqrt((Tmax*Tmax+2*MASS_E*Tmax)/
				  (T*T+2*MASS_E*T));
      
      //  cerr << " do delta rays" << endl;
      
      // get the distance
      double dist = xstep*Rand();
      
      // orientation of the incident muon in the tank frame
      double UR = sqrt(fUX*fUX+fUY*fUY);
      
      // compute delta ray coordinates
      double dr_x = fX + dist * fUX;
      double dr_y = fY + dist * fUY;
      double dr_z = fZ + dist * fUZ;
      double time = fT + dist/(CLIGHT*beta);
      
      // compute the orientation in the muon frame
      double sintheta = sqrt(1-costheta*costheta);
      double phi = 2*PI*Rand();
      double cosphi = cos(phi);
      double sinphi = sin(phi);
      double ux = sintheta*cosphi;
      double uy = sintheta*sinphi;
      double uz = costheta;
      int dr_id = 2;
      double dr_ux;
      double dr_uy;
      double dr_uz;
      if(UR!=0) // muon non purely vertical
	{
	  dr_ux = (ux*fUZ*fUX-uy*fUY)/UR+uz*fUX;
	  dr_uy = (ux*fUZ*fUY+uy*fUX)/UR+uz*fUY;
	  dr_uz = -UR*ux+fUZ*uz;
	}
      else
	{
	  dr_ux = ux;
	  dr_uy = uy;
	  if(fUZ>0) dr_uz = uz;
	  else   dr_uz = -uz;
	}
      
	double norm = sqrt(dr_ux*dr_ux+dr_uy*dr_uy+dr_uz*dr_uz);
	
	double dr_E = T+MASS_E;
	
	
	double radius = sqrt(dr_x*dr_x+dr_y*dr_y);
	//     cout << "probleme ?" << dr_z << " " << radius << endl;
	if( (dr_z>0) && (dr_z<STATION_HEIGHT)*(1-EPSILON)  && radius <STATION_RADIUS*(1-EPSILON) )
	  
	  {
	    ParticleInTank* p1 = new ParticleInTank(fISta,dr_id,fId,dr_x,dr_y,dr_z,dr_ux/norm,
						    dr_uy/norm,dr_uz/norm,time,dr_E,fW);
	    if(PrintCout == 1) cout << "\n Stat,Part,MotherPart,énergie" << fISta << "," << dr_id << "," << fId << "," << dr_E << endl;
	p1->DoDetSim();
	if(PrintCout == 1) cout << "DeltaRay->fNCherphot MotherPart = " << fNCherPhot << endl;
	fNCherPhot += p1->fNCherPhot;
	if(PrintCout == 1) cout << "DeltaRay->fNCherphot MotherPart+fille = " << fNCherPhot << endl;
	for(int iph=0; iph<p1->fNCherPhot; iph++){
	  fTCherPhot.push_back(p1->fTCherPhot[iph]);
            fIpm.push_back(p1->fIpm[iph]);
            fNReflexions.push_back(p1->fNReflexions[iph]);
          }
          delete p1;
	  
          DE+=T;
	  }
	//else
	//cout << " Delta Ray not simulated " << endl;
	
      }
    
  return DE;
  
}

double FuncDistribThetaCM(double x, double rand)
{
  return M_PI*rand-x-sin(x);
}


int ParticleInTank::MuonDecay()
{
  if(PrintCout == 1) cout << "Muon decay" << endl;
  return 0;
}


/*----------------------------------------------------------
  Electron processes
----------------------------------------------------------*/
 
Double_t  ParticleInTank::Bremstrahlung(Int_t istep,Double_t dxstep,Double_t beta){
  if(PrintCout == 1) cout<< "bremstrahlung" << endl;
  double pb;
  double EFrac, GammaKE;
  double brE ;
  pb = Rand();
  int ind = 0;
  for(int i=1; i<NDIV; i++)
    {
      if(gBremTable[istep][i]>pb)
        {ind=i; break;}
    }
  EFrac = (double)ind/(double)NDIV;
  GammaKE = fE*EFrac;
  
  // Determine approx RMS gamma emission angle
  // (ref Rossi page 53 with q factor=1)
  // In fact angles are small compared to scattering so no problem
  
  double mom2 = fE*(fE+2*MASS_E);
  double sig2 = pow(.0136/beta,2) / mom2 * dxstep/.361;
  double urandom=Rand();
  while(urandom==0)
    urandom=Rand();
  double theta = sqrt(-2*sig2*log(urandom));
  double costh = cos(theta);
  double sinth = sqrt(1-costh*costh);
  double phi = 2*PI * Rand();
  double cosph = cos(phi);
  double sinph = sin(phi);
  
  if(GammaKE>EMINBREM*dxstep)
    {
      // Calculate the vector components of daughter direction
      // and rotate ref frame such that z axis lies along old ptcle track
      if(PrintCout == 1) cout<<" BBREMMMMMM "<<GammaKE<<" "<<EMINBREM<<" "<<dxstep<<endl;
      double ur = sqrt(fUX*fUX+fUY*fUY);
      double vx,vy,wx,wy,wz;
      if(ur!=0)
        {
          vx = -fUY/ur;
          vy =  fUX/ur;
          wx = -fUZ*vy;
          wy = fUZ*vx        ;
          wz = fUX*vy-fUY*vx;
        }
      else
        {
          vx = 1;
          vy = 0;
          wx = 0;
          wy = 1;
          wz = 0;
        }
      
      // new direction
      
      double brUX = costh * fUX + sinth * (cosph*vx+sinph*wx);
      double brUY = costh * fUY + sinth * (cosph*vy+sinph*wy);
      double brUZ = costh * fUZ + sinth * (         sinph*wz);
      
      
      double brX = fX;
      double brY = fY;
      double brZ = fZ;
      int brId = 1;
      double brT = fT ;
      double brW = fW;
      brE = GammaKE;
      
      ParticleInTank* p1 = new ParticleInTank(fISta,brId,fId,brX,brY,brZ,
                                              brUX,brUY,brUZ,brT,brE,brW);

      p1->DoDetSim();
      fNCherPhot += p1->fNCherPhot;
      for(int iph= 0;iph<p1->fNCherPhot;iph++){
        fTCherPhot.push_back(p1->fTCherPhot[iph]);
        fIpm.push_back(p1->fIpm[iph]);
        fNReflexions.push_back(p1->fNReflexions[iph]);
      }
      delete p1;
      
    }
  else
    brE = 0;
  
  return(brE);
}
 


void ParticleInTank::MultipleScattering(Double_t beta,Double_t xstep)
{
  //if(PrintCout == 1) cout << "multiple scattering" << endl;
  double mom2 = fE*(fE+2*MASS_E);
  double sig2 = pow(.0136/beta,2) / mom2 * xstep/.361;
  double urandom = Rand();
  while(urandom==0) urandom=Rand();
  double theta = sqrt(-2*sig2*log(urandom));
  double costh = cos(theta);
  double sinth = sqrt(1-costh*costh);
  double phi = 2*PI * Rand();
  double cosph = cos(phi);
  double sinph = sin(phi);
  // construction of two unit vectors perp. to the initial direction
  // (vx,vy,0) and (wx,wy,wz)
  double ur = sqrt(fUX*fUX+fUY*fUY);
  
  double vx,vy,wx,wy,wz;
  if(ur!=0)
    {
      vx = -fUY/ur;
      vy =  fUX/ur;
      wx = -fUZ*vy;
      wy = fUZ*vx;
      wz = fUX*vy-fUY*vx;
    }
  else
    {
      vx = 1;
      vy = 0;
      wx = 0;
      wy = 1;
      wz = 0;
    }
  
  // new direction
  fUX = costh * fUX + sinth * (cosph*vx+sinph*wx);
  fUY = costh * fUY + sinth * (cosph*vy+sinph*wy);
  fUZ = costh * fUZ + sinth * (         sinph*wz);
  
}

 

/*----------------------------------------------------------
  Cherenkov production
----------------------------------------------------------*/
 
void ParticleInTank::CherPhot(Double_t beta,Double_t range)
{
  //if(PrintCout == 1) cout << "cherphot" << endl;
  // if(fId == 3 || fId == -3)cerr << "do cherenkov "<< range << endl;
  Double_t vx,vy,vz,wx,wy,wz,uxy;
  Double_t costh_cher,sinth_cher;
  Double_t alph,calph,salph;
  Double_t cx,cy,cz;
  Double_t cx_ph,cy_ph,cz_ph,x_ph,y_ph,z_ph,r_ph;
  Double_t prb,prb_refdif,prb_spec,abslen;
  Int_t n,nphot,ipm;
  nphot = 0;
  cx = fUX;
  cy = fUY;
  cz = fUZ;

  uxy = sqrt(fUX*fUX+fUY*fUY);

  if(uxy > 0.)
    {
      vx = -fUY/uxy;
      vy = fUX/uxy;
    }
  else
    {
      vx = 1.;
      vy = 0.;
    }

  vz = 0.;
  wx = fUY*vz-fUZ*vy;
  wy = fUZ*vx-fUX*vz;
  wz = fUX*vy-fUY*vx;

  // Cherenkov effect:
  // cout << range << " " << fId << " " << fE << " " << weight << endl;
  costh_cher = 1./(WATINDEX*beta);
  //  if(fId == 3 || fId == -3) cout << costh_cher << endl;
  if(costh_cher>1)return;

  sinth_cher = sqrt(1-costh_cher*costh_cher);
  Double_t nn = (gCherFact*sinth_cher*sinth_cher*range*fW);
  n = (Int_t)nn;
  //  if(fId==3 || fId==-3) cerr << "bilan" << fE << " " << range << " " << nn << endl;
  if(Rand()<nn-n) n++;

  // cout << range << " " << n << endl;
  //  cout << "88888 " << n << endl;

  for(Int_t i=0; i<n; i++)
    {
      // direction of emission
      alph = 2*PI*Rand();
      calph = cos(alph);
      salph = sin(alph);
      cx_ph = costh_cher*fUX + sinth_cher * (calph*vx+salph*wx);
      cy_ph = costh_cher*fUY + sinth_cher * (calph*vy+salph*wy);
      cz_ph = costh_cher*fUZ + sinth_cher * (salph*wz);

      // point of emission
      r_ph = range*Rand();
      x_ph = fX + r_ph*cx;
      y_ph = fY + r_ph*cy;
      z_ph = fZ + r_ph*cz;

      double wav,qe;
      int iwa = 0;

      // wavelength and absorption properties at this wavelength
      prb = Rand();
      for(Int_t j=0; j<NWAVEL; j++)
        {
          //cout << NWAVEL << " " << j << " " << gCherPrb[j] << endl;
          if(prb < gCherPrb[j]) {
            iwa = j;
            break;
          }
        }
      int deb = 0;
      if(iwa>0) {
          wav = (WAVELMIN+(iwa+.5)*DWAVEL)*1.e9;
          prb_refdif = LINABSMAX * RELLINABS[iwa];
          abslen = WATABSLENMAX * RELWATABSLEN[iwa];
          prb_spec = SPECFRACMAX * RELSPECFRAC[iwa];

          qe = PMTQE[iwa];
      }
      else
        {
          deb = 1;
          wav = (WAVELMIN+(iwa+.5) * DWAVEL) * 1.e9;
          prb_refdif = LINABSMAX * RELLINABS[iwa];
          abslen = WATABSLENMAX *  RELWATABSLEN[iwa];
          prb_spec = SPECFRACMAX * RELSPECFRAC[iwa];
          //wav = (WAVELMIN+(j+.5) * DWAVEL) * 1.e9;
          qe = PMTQE[iwa];

        }

      //cout<<iwa<<" "<<wav<<" "<<abslen<<" " <<prb_refdif<<" "<<qe<<" "<<deb<<endl;
      //   cout<<"pos "<<x_ph<<" " <<y_ph<<" "<<z_ph<<" " <<cx_ph<<" " <<cy_ph<<" "<<cz_ph<<endl;

      // following a Cherenkov photon
      ipm = this->Follow(x_ph,y_ph,z_ph,cx_ph,cy_ph,cz_ph,
                       prb_refdif,prb_spec,abslen);
      if(ipm>0) { nphot++;}

    }
  if(PrintCout == 1) cout << "Cherphot->fNCherphot MotherPart = " << fNCherPhot << endl;
  fNCherPhot += nphot;
 if(PrintCout == 1) cout << "Cherphot->fNCherphot MotherPart+phot = " << fNCherPhot << endl;

  //cout<<" "<<nphot<<endl;
  // if(fId==3 || fId==-3)cout <<fId<<" "<< n << " "<<nphot <<" "<<weight<<" "<<e1<<" "<<range<<endl;
  //if(fId==3 || fId==-3) cout <<fISta<<" "<<fId<<" "<< n << " "<<nphot <<" "<<fNCherPhot<<" "<<range<<" "<<beta<<" "<<sinth_cher<<endl;
  // if(fId==3 || fId==-3) cout<<"cer "<<n<<" "<<nphot<<" "<<fNCherPhot<<" "<<endl;
  // if(fId==3)cout<<endl;
}



/*----------------------------------------------------------
  Follow particle until PM
  --------------------------------------------------------*/

Int_t ParticleInTank::Follow(Double_t x,Double_t y,Double_t z,
                           Double_t cosx,Double_t cosy,Double_t cosz,
                           Double_t prb_refdif,Double_t prb_spec,
                           Double_t abslen)
{
  Double_t zc_pm;
  Double_t xx,yy,zz,cxx,cyy,czz,norm,prod;
  Double_t aa,bb,cc,glength,zs;
  Double_t c2bet,cbet,sbet,alph,calph,salph;
  Int_t active;
  Double_t zhit;
  Int_t ipm;
  Double_t tlength;

  zc_pm = STATION_HEIGHT+HC_PM;

  ipm = 0;
  tlength = 0.;
  xx = x;
  yy = y;
  zz = z;
  cxx = cosx;
  cyy = cosy;
  czz = cosz;
  norm = sqrt(cxx*cxx+cyy*cyy+czz*czz);
  cxx = cxx/norm;
  cyy = cyy/norm;
  czz = czz/norm;
  // prb_spec=0.1;
  active = 1;
  Int_t ind;
  ind = 0;

 // computation of geometric length
  while(active)
    {

      aa = cxx*cxx+cyy*cyy;
      bb = xx*cxx+yy*cyy;
      cc = STATION_RADIUS*STATION_RADIUS-xx*xx-yy*yy;
      glength = (-bb+sqrt(bb*bb+aa*cc))/aa;
      zs = zz+glength*czz;

      // side wall  -------------------------------------
      if(zs > 0.&& zs < STATION_HEIGHT) {

        // absorption in water or in wall
        active = Rand() < exp(-glength/abslen);
        active = active && Rand()< prb_refdif;

        if(active) {
          xx += glength*cxx;
          yy += glength*cyy;
          zz = zs;
          tlength += glength;
          ind++;
          //  Double_t intheta= acos(STATION_HEIGHT/tlength)*RAD2DEG;
          /*   geometry of reflection/diffusion unit vectors (w.r.t. surface)
               normal  n  : -xx/STATION_RADIUS  -yy/STATION_RADIUS   0
               tangent t1 :  yy/STATION_RADIUS  -xx/STATION_RADIUS   0
               t2 :         0               0                1  */

          // specular reflection
          if(Rand() < prb_spec)
            {
              prod = (xx*cxx+yy*cyy)/STATION_RADIUS;
              cxx += -2*prod*xx/STATION_RADIUS;
              cyy += -2*prod*yy/STATION_RADIUS;


              // diffusion (sinus.cosinus law -->  uniform distr. in cos(2*th)
            }
          else
            {
              c2bet = 2*Rand()-1;
              cbet = sqrt((1+c2bet)/2);
              sbet = sqrt((1-c2bet)/2);
              alph = 2*Rand()*PI;
              calph = cos(alph);
              salph = sin(alph);
              cxx = (-cbet*xx+sbet*salph*yy)/STATION_RADIUS;
              cyy = (-cbet*yy-sbet*salph*xx)/STATION_RADIUS;
              czz = sbet * calph;
              //cout<<"1"<<" "<<intheta<<" "<<180-acos(cxx)*RAD2DEG<<endl;

            }
          // if(ind>=1)cout<<ind<<" side "<< czz<<endl;
          // cout<<ind<<" side "<< czz<<endl;
        }//end active
      }//end side wall

      //  top wall -----------------------------------------
      else
        if(czz > 0.) {
          glength = (STATION_HEIGHT-zz)/czz;
          active = Rand() < exp(-glength/abslen);

          if(active) {
            xx += glength*cxx;
            yy += glength*cyy;
            zz = STATION_HEIGHT;
            tlength += glength;


            //  PMT simulation portion of a semisphere PMT
            //  PMT entry ?
            for(Int_t j=0;j<NPM;j++){
              bb = cxx*(xx-X_PM[j])+cyy*(yy-Y_PM[j])+czz*(zz-zc_pm);
              cc = RAD_PM * RAD_PM - pow((xx-X_PM[j]),2) - pow((yy-Y_PM[j]),2)
                -pow((zz-zc_pm),2);

              if(bb*bb+cc >0.)
                {
                  zhit = zz-(bb+sqrt(bb*bb +cc))*czz;
                  if(zhit <= STATION_HEIGHT &&
                     zhit >( STATION_HEIGHT + HC_PM-RAD_PM))
                    {
                      ipm = j+1;
                      fTCherPhot.push_back(tlength*WATINDEX/CLIGHT+fT);
                      fIpm.push_back(ipm);
                      fNReflexions.push_back(ind);
                      // cout<<ipm<<" "<<ind<<" "<<tlength<<endl;
                      // cout<<ind<<endl;
                      return(ipm);

                    }
                }
            }//loop on PMT

            // cout<<"999999 "<<ipm<<" "<<ind<<" "<<tlength<<" "<<xx<<" "<<yy<<endl;
            ind++;

            // no PMT entry
            active = Rand() < TOP_FACT * prb_refdif;
            if(active){
              // specular reflection

              if(Rand() < prb_spec)
                czz=-czz;
              else
                // diffusion (sinus.cosinus law -->  uniform distr. in cos(2*th)
                {
                  c2bet = 2*Rand()-1;
                  sbet = sqrt((1-c2bet)/2);
                  alph = Rand() * 2 * PI;
                  cxx = sbet * cos(alph);
                  cyy = sbet * sin(alph);
                  czz = -sqrt((1+c2bet)/2);
                }
              //if(ind>=1)cout<<ind<<" top "<< czz<<" " <<ipm<<endl;

            }//end active

          }//end active
	  
        }//end top
      
      // bottom wall ---------------------------------------
      
	else {
	  
          glength = -zz/czz;
          active = Rand()< exp(-glength/abslen);
          active = active && Rand() < prb_refdif;
          // Double_t thetain = acos(-czz)*RAD2DEG;
          if(active){
            xx += glength*cxx;
            yy += glength*cyy;
            zz=0.;
            tlength += glength;
            ind++;
            Double_t spec = Rand();
            // Double_t thetain = acos(-czz)*RAD2DEG;
            if(spec < prb_spec){
              czz=-czz;
              //  cout<<"0 "<<thetain<<" "<<acos(czz)*RAD2DEG<<endl;
            }
            else
              // diffusion (sinus.cosinus law -->  uniform distr. in cos(2*th))
              {
                c2bet = 2*Rand()-1;
                sbet = sqrt((1-c2bet)/2);
                alph = Rand()*2*PI;
                cxx = sbet * cos(alph);
                cyy = sbet * sin(alph);
                czz = sqrt((1+c2bet)/2);

                //cout<<"1 "<<thetain<<" "<< acos(czz)*RAD2DEG<<endl;
                //cout<<c2bet<<" "<<sbet<<" "<<alph<<" "<<cxx<<" "<<cyy<<" "<<czz<<endl;
              }

            //  if(ind>=1)cout<<ind<<" bottom "<< czz<<endl;
	    // cout<<"999999 "<<ipm<<" "<<ind<<" "<<tlength<<" "<<xx<<" "<<yy<<endl;

          }//end active

        }//end bottom */
    }
  // cout<<ind<<" "<<tlength<<" "<<ipm<<endl;
  return(ipm);
}


//------------------------------------------------------------------------//
//------------------------------------------------------------------------//
//------------------------------------------------------------------------//



/*-------------------------------------------------         
  class ParticleInScint                           -
                                                  -
  * Gamma :                                       -
  No interaction (thinckness 1.5 cm)              -
  Pair production -> all energy deposited         -
  Compton scattering -> use Geant 4 stat.         -
                                                  -
  * Muon / Electron :                             -
  Formula of Bethe & Bloch                        -
                                                  -
------------------------------------------------- -
  For Detailed Mode:                              -
                                                  -
  * Muon Simulation:                              -
  Bethe & Bloch (use Leo formula)                 -
                                                  -
  * Electron Simulation:                          -
  Bethe & Bloch modified for electron (Leo)       -
                                                  -
  * Gamma Simulation:                             -
  No interaction (thinckness 1.5 cm)              -
  Interaction : - Compton effect (use Geant 4)    -
                - Pair Production (all deposited) -
                                                  -
-------------------------------------------------*/

ParticleInScint::ParticleInScint()
{

}

ParticleInScint::ParticleInScint (Int_t ist,Int_t id,double x,double y,double z, 
				double cx,double cy,double cz,double tim,double en,double weight)
  //  : Particle(id, 0,0,0, tim, en, cx, cy, cz)
{
  fId = id;
  fUX = cx;
  fUY = cy;
  fUZ = cz;
  fT = tim;
  fE = en;

  fISta = ist;
  fX = x;
  fY = y;
  fZ = z;
  fW = weight;
  GLength = 0;
  ProbNoInt = 0;
  ProbPair = 0;
  fNPhot = 0;
  fIpm = 0;
  fBinContent.clear();
}

ParticleInScint::ParticleInScint (Int_t ist,Int_t id,Int_t motherid,double x,double y,double z,double cx,double cy,double cz,double tim,double en,double weight)
  //  : Particle(id, 0,0,0, tim, en, cx, cy, cz)
{
  fId = id;
  fMotherId =  motherid;
  fUX = cx;
  fUY = cy;
  fUZ = cz;
  fT = tim;
  fE = en;
  
  fISta = ist;
  fX = x;
  fY = y;
  fZ = z;
  fW = weight;
  GLength = 0;
  ProbNoInt = 0;
  ProbPair = 0;
  fNPhot = 0;
  fIpm = 0;
  fBinContent.clear();
}

ParticleInScint::~ParticleInScint()
{

}


/* ------------------------------------------
 -   DETECTOR SIMULATION FOR EACH PARTICLE  -
 --------------------------------------------


/* --------------------------------------------------------
-  Manager of the step by step propagation of each        -
-  particle in a scintillator.                            -
-  In each step, determine the length, the loss of energy -
-  and generate photons.                                  -
---------------------------------------------------------*/

void ParticleInScint::DoEDSim()  
{
  double randomInt;
  //cout<<"\n DoEDSim! \n - fId = " << fId << "\n - Energy = " << fE * 1E3 << " MeV" << endl;
  
  if (abs(fId) == 1)
    {
      GeomLength();
      InteractionProbability();
      randomInt = Rand();
      //if(PrintCout == 1) cout << "######### randomInt = " << randomInt << ", ProbNoInt = " << ProbNoInt << endl;
      if ( randomInt > ProbNoInt)
	DoGammaProcesses();
      else //return;
	{
	  //cout << " 0000ooooo Greve du travail pour les gammas oooooo0000" << endl;
	}
    }

  else if (abs(fId) == 2 || abs(fId) == 3) 
    {
      GeomLength(); 
      EnergyLoss(); 
      GeneratePhoton();
    }

  else return;
}


  /*------------------------------------
  - Computes the distance between the  -
  - particle and the scintillator.     -
  ------------------------------------*/
void ParticleInScint::GeomLength()
{
  if(PrintCout == 1) cout << "GeomLength! " << endl;
  Double_t aa,bb,cc;
  aa = fUX * fUX + fUY * fUY;
  bb = fX * fUX + fY * fUY;
  cc = SCINT_LENGTH * SCINT_LENGTH - fX * fX - fY * fY;
  //   length up to the external volume
  if(aa != 0) GLength = (-bb + sqrt(bb * bb + cc * aa)) / aa;
  else GLength = INFINITY;
  //   length up to the top or the bottom
  if(fUZ<0) GLength = min(GLength,-fZ/fUZ);
  else if(fUZ != 0)         
     GLength = min(GLength,(SCINT_HEIGHT-fZ)/fUZ);
   else  GLength = 2*(fX*fUX+fY*fUY);

  return;
}




  /*------------------------------------------------------
  -                     FOR GAMMAS                       -
  - 1) Compute the probability to do an interaction      -
  - 2) Probability to do a pair production               -
  - 3.a) If pair production -> energy deposited by gamma -
  - 3.b) If compton effect -> energy deposited by gamma  - 
  ------------------------------------------------------*/


///////////////////////////////
// PROBABILITY TO INTERACT ? //
///////////////////////////////

void ParticleInScint::InteractionProbability()
{
  Double_t GammaEnergy = fE;
  Int_t itab;
  
  itab = (Int_t)((log10(GammaEnergy) + 4.1)/0.2);
  if(itab <= 0) { ProbNoInt = TABNOINTERACT[0]; }
  else {
    if(itab >= (NTAB_PHOT-1)) {ProbNoInt = TABNOINTERACT[NTAB_PHOT-1];}
     else{
       double log10Einf = -4.1 + 0.2 * itab;
       double log10Esup = -4.1 + 0.2 * (itab+1);
       ProbNoInt = TABNOINTERACT[itab] + (log10(GammaEnergy) - log10Einf) * (TABNOINTERACT[itab+1] - TABNOINTERACT[itab]) / (log10Esup - log10Einf);
     }
  }
}

/////////////////
// IF INTERACT //
/////////////////

void ParticleInScint::DoGammaProcesses()
{
  double randomPair;
  //cout << "DoGammaProcesses : " << endl;
  PairProbability();
  randomPair = Rand();
  //cout << "randomPair = " << randomPair << ", ProbPair = " << ProbPair << endl;  

  if (randomPair <= ProbPair)
    { 
      PairProduction(); 
      //cout << "-> PairProduction();" << endl;
    }
  else 
    {
      ComptonScattering(); 
      //cout << "-> ComptonScattering();" << endl;
    }

  GeneratePhoton();
  //cout << "-> GeneratePhoton();" << endl;
}
  

///////////////////////////////////////////
// PROBABILITY TO DO A PAIR PRODUCTION ? //
///////////////////////////////////////////

 void ParticleInScint::PairProbability()
 {
   //cout << "Pair Probability : " << endl;
   Int_t itab;
   Double_t GammaEnergy = fE;

   if(GammaEnergy <= kEMinPh) return;
   
   itab = (Int_t)((log10(GammaEnergy) + 4.1)/0.2);
   if(itab <= 0)  { ProbPair = TABPROBPAIR_ED[0];}
    else{
      if(itab >= (NTAB_PHOT-1)) { ProbPair = TABPROBPAIR_ED[NTAB_PHOT-1];}
      else{
        double log10Einf = -4.1 + 0.2 * itab;
        double log10Esup = -4.1 + 0.2 * (itab+1);
        ProbPair = TABPROBPAIR_ED[itab] + (log10(GammaEnergy) - log10Einf) * (TABPROBPAIR_ED[itab+1] - TABPROBPAIR_ED[itab]) / (log10Esup - log10Einf);
      }
    }

   //cout <<"Dans PairProbability, ProbPair = "<< ProbPair << " " << ", GLength = " << GLength << endl;
   return;
 }
 

////////////////////////////////////////////
// IF PAIR PRODUCTION, ENERGY DEPOSITED ? //
////////////////////////////////////////////

 void ParticleInScint::PairProduction()
 {
   //cout << " Pair Production !" << endl;
   // We consider in first approximation, all energy of gamma is deposited in the scintillator with ionization of electron and positron :
   GammaEnergyLoss = fE * 1E3; // in MeV
   //cout << " GammaEnergyLoss = " << GammaEnergyLoss << " MeV" << endl;
   return;
 }
 

///////////////////////////////////////////////
// IF COMPTON SCATTERING, ENERGY DEPOSITED ? //
///////////////////////////////////////////////

 void ParticleInScint::ComptonScattering()
 {
   // me = electron mass in MeV = 0.511
   Double_t GammaEnergy = fE*1E3; // in MeV
   Double_t emin,smax,efin,sfin;
   Int_t reject;
   
   emin = 1. / ( 1./GammaEnergy + 2./me );
   smax = emin/GammaEnergy + GammaEnergy/emin + pow((1.+me/GammaEnergy-me/emin),2) - 1.;

  reject = 1;
  while(reject)
    {
      efin = emin + Rand()*(GammaEnergy-emin);
      sfin = efin/GammaEnergy + GammaEnergy/efin;
      reject = Rand() > sfin/smax;
    }
  
  GammaEnergyLoss = GammaEnergy - efin; 
  //cout <<  " GammaEnergyLoss = " << GammaEnergyLoss << " MeV" << endl;
 }
 




  /*-----------------------------------------------------
  -                FOR MUONS AND ELECTRONS              -
  -  Compute the energy deposited by muons & electrons  -
  -  in the scintillator function of distance !!        -
  -----------------------------------------------------*/

 void ParticleInScint::EnergyLoss()
{
  //cout<<"EnergyLoss!"<<endl;

  double beta, gamma, eta, s, p, Ek, tau, F, Wmax, lgfactor;
  double dx = GLength * 100. * density; // in g/cm²
  fE = fE * 1E3; // Convert energy particle GeV -> MeV


  switch (abs(fId)) {
    
  case 1:
    break;
    
  case 2:
    // ElectronEnergyLoss : Loss of energy by electron in the scintillator in MeV (W.R. Leo p.35) :
    //cout << "Energy of electron, fE = " << fE << " MeV" << endl;
    p = sqrt(fE*fE - me*me);
    beta = p/fE; 
    gamma = fE/me;
    eta = p/me;
    Ek = me * (gamma - 1.);
    tau = Ek/me;
    lgfactor = log( (tau*tau*(tau+2.)) / (2.*(I/me)*(I/me)) );
    F = 1. - beta*beta + ( tau*tau/8. - (2.*re+1.)*log(2.) ) / ( (tau+1.)*(tau+1.) );

    ElectronEnergyLoss = factor * Z/A * 1/(beta*beta) * (lgfactor + F - delta - 2.* C/Z) * dx; // Energy loss in MeV

    //cout <<"CAS 2 :  fE = " << fE << " MeV, Beta = " << beta << ", Energy Loss = " << ElectronEnergyLoss << " MeV, GLength = " << GLength*100 << " cm" << endl;
    break;
    
  case 3:
    // MuonEnergyLoss : Loss of energy by muon in the scintillator in MeV
    //cout << "Energy of muon, fE = "<< fE << " MeV" << endl;
    p = sqrt(fE*fE - M*M);
    gamma = fE/M;
    beta = p/fE; 
    eta = p/M;
    s  = me / M;
    Wmax = (2.*me*eta*eta)/(1. + 2.*gamma*s + s*s);
    lgfactor = log( (2.*me * eta*eta * Wmax) / (I*I));

    // Muon energy loss in GeV
    MuonEnergyLoss = factor * Z/A * (1/(beta*beta)) * (lgfactor - 2. *(beta*beta) - delta - 2.* C/Z) * dx; 

    //cout << "CAS 3 :  fE = " << fE << " MeV, MuonEnergyLoss = " << MuonEnergyLoss << ", GLength = " << GLength*100 << " cm" << endl;
    break;
    
  default:
    cerr << "Error in ParticleInScint::EnergyLoss..." << endl;    
  }

}


  /*--------------------------------------------------------
  -  Compute photons produced function the energy lost     -
  -  in the scintillator.                                  -
  -  We consider 1 particle produce 11000 photons per MeV  -
  -  deposited !!                                         -
  ------------------------------------------------------- */

void ParticleInScint::GeneratePhoton()
{
  //cout<<"GeneratePhoton!"<<endl;
  int partinfG;
  int partsupG;
  int partinfE;
  int partsupE;
  int partsupM;
  int partinfM;
  double fNPhot_temp;

  switch (abs(fId)) {

  case 1:  

    fNPhot_temp = GammaEnergyLoss * 11000;
    partinfG = floor(fNPhot_temp);
    partsupG = partinfG + 1;

    if(fE > 1E-4 && fNPhot_temp-partinfG < partsupG-fNPhot_temp)
      {
	fNPhot = partinfG;
	fTPhot = fT;
	fIpm = 1;
	//cout << "CAS 1.1 :  fNPhot_gamma = " << fNPhot << ", fTPhot = " << fTPhot << ", E = " << partinfG << "," << fE << endl;
      }
    else if(GammaEnergyLoss > 1E-4 && fNPhot_temp-partinfG > partsupG-fNPhot_temp)
      {
	fNPhot = partsupG;
	fTPhot = fT;
	fIpm = 1;
	//cout << "CAS 1.2 :  fNPhot_gamma = " << fNPhot << ", fTPhot = " << fTPhot << ", E = " << partsupG << "," << fE << endl;
      }
    else
      {
	fNPhot = 0;
	fTPhot = fT;
	fIpm = 1;
	//cout << "WARNING :  Problem with energy of gamma, Eloss = " << GammaEnergyLoss << " MeV" << endl;
      }
    break;

  case 2:    

    fNPhot_temp = ElectronEnergyLoss * 11000;
    partinfE = floor(fNPhot_temp);
    partsupE = partinfG + 1;
    
    if(ElectronEnergyLoss > 1E-4 && fNPhot_temp-partinfE < partsupE-fNPhot_temp)
      {
	fNPhot = partinfE;
	fTPhot = fT;
	fIpm = 1;
	//cout << "CAS 2.1 :  fNPhot_electron = " << fNPhot << ", fTPhot = " << fTPhot << ", E = " << partinfE << "," << fE << endl;
      }
    else if(ElectronEnergyLoss > 1E-4 && fNPhot_temp-partinfE > partsupE-fNPhot_temp)
      {
	fNPhot = partsupE;
	fTPhot = fT;
	fIpm = 1;
	//cout << "CAS 2.2 :  fNPhot_electron = " << fNPhot << ", fTPhot = " << fTPhot << ", E = " << partsupE << "," << fE << endl;
      }
    else 
      {
	fNPhot = 0;
	fTPhot = fT;
	fIpm = 1;
	//cout << "WARNING :  Problem with energy of electron, Eloss = " << ElectronEnergyLoss << " MeV, fE = " << fE << " MeV" << endl;
      }
    break;

   case 3:

     fNPhot_temp = MuonEnergyLoss * 11000;
     partinfM = floor(fNPhot_temp);
     partsupM = partinfG + 1;
    
     if(MuonEnergyLoss > 1E-4 && fNPhot_temp-partinfM < partsupM-fNPhot_temp)
      {
	fNPhot = partinfM;
	fTPhot = fT;
	fIpm = 1;
	//cout << "CAS 3.1 :  fNPhot_muon = " << fNPhot << ", fTPhot = " << fTPhot << ", E = " <<partinfM << "," << fE << endl;
      }
     else if(MuonEnergyLoss > 1E-4 && fNPhot_temp-partinfM > partsupM-fNPhot_temp)
      {
	fNPhot = partsupM;
	fTPhot = fT;
	fIpm = 1;
	//cout << "CAS 3.2 :  fNPhot_muon = " << fNPhot << ", fTPhot = " << fTPhot << ", E = " << partsupM << "," << fE << endl;
      }
     else 
      {
	fNPhot = 0;
	fTPhot = fT;
	fIpm = 1;
        //cout << "WARNING :  Problem with energy of muon, Eloss = " << MuonEnergyLoss << " MeV, fE = " << fE << " MeV" << endl;
      }
     break;
     
  default:
    cerr << "Error in ParticleInScint::GeneratePhoton..." << endl;                      
  }  

}
