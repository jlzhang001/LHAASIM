/*------------------------------------------------
-               Particle.cc  (v0r2)              -
-                                                -
- implementation file for all classes Particle   -
------------------------------------------------*/



#include "Particle.h"
#include "LhaaSimConfig.h"
#include "BuildProcessesTables.h"
#include "Utils.h"
#include "Constants.h"
#include <math.h>
#include <fstream>
#include "TRandom.h"
#include "TRandom3.h"

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

// DebugMode : Print "cout" if PrintCout = 1, remove it if = 0
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
                                                  -
 First approximation :                            -
 -> EM absorbed at the surface                    -
 -> muons not affected                            -
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
  if(PrintCout == 1) cout << "DoDetSim : \n fX,fY,fZ = " << fX << "," << fY << "," << fZ << endl;
  
  if(abs(fId) == 1)
    {
      GeomLength(); 
      DoProcesses();
    }
  
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


/* ------------------------------------
   Computes the distance between the  -
   particle and the tank.             -
   ----------------------------------*/
void ParticleInTank::GeomLength()
{
  Double_t aa,bb,cc;
  aa = fUX*fUX + fUY*fUY;
  bb = fX*fUX + fY*fUY;
  cc = STATION_RADIUS*STATION_RADIUS - fX*fX - fY*fY;
  //   length up to the external cylinder
  if(aa!=0) GLength = (-bb + sqrt(bb*bb+cc*aa))/aa;
  else GLength = INFINITY;
  //   length up to the top or the bottom
  if(fUZ<0) GLength = min(GLength,-fZ/fUZ);
   else if(fUZ!=0)
     GLength = min(GLength,(STATION_HEIGHT-fZ)/fUZ);
   else  GLength = 2*(fX*fUX + fY*fUY);
  
  return;
}


/*-----------------------------------------
  Decides if the step by step tracking   -
  must be continued or stopped.          -
  -----------------------------------------*/

 int ParticleInTank::DoNextStep()
{
  int ret = 0;
  IStep = 0;
  
  switch (abs(fId)) {
    
  case 1:
    
    break;
    
  case 2:
    while (fE <= ESTEP_E[IStep]) IStep++;
    ret = (IStep<NSTEP_E && Range>=0);
    
    break;

  case 3:
    while (fE <= ESTEP_MU[IStep]) IStep++;
    ret = (IStep <= NSTEP_MU && Range>0);
    break;

  default:
    cerr << "Error in ParticleInTank::DoNextStep..." << endl;

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
  switch (abs(fId)) {

  case 1:

    break;

  case 2:
   
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
  switch (abs(fId)) {

  case 1:
   
    PairProbability();
    if (GLength < IntLength) return STOP;
    random2011 = Rand();

    if (random2011 <= ProbPair) {
      PairProduction();
      return STOP;
    } else {
      ComptonScattering();
    }
   
    break;

  case 2:    
   
    if (theConfig()->SimMode == "DETAILED") {
      if(Range>0) CherPhot(Beta,Range); 
      MultipleScattering(Beta,Range);
      probIntInOneStep=gBremTable[IStep][0]*(Range*100);
      if(Rand()<probIntInOneStep) fE -= Bremstrahlung(IStep,Range,Beta);
      fE -= Range*DEDX_E;
      if (Range < DxStep || fE < EMIN_E) return STOP;
    } else { // fast simulation
      ;
    }
   
    break;

  case 3:    
    if (theConfig()->SimMode == "DETAILED") {
    
      if(Range>0) CherPhot(Beta,Range);
      if(DxStep>0) fE -= GenerateDeltaRays(Beta,Range);
      while(fE < ESTEP_MU[IStep]) {IStep++;}
      if ( MuonDecay() == 1 ) return STOP;
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

// Calculate IntLength and ProbPair, useful in DoProcesses
// photon ->  paire production ? or Compton ?

void ParticleInTank::PairProbability()
{
  Double_t e_ph;
  Double_t ph_al;
  Int_t itab;

  if(fE <= kEMinPh) return;

  // initial posiiton and direction of photon :
  e_ph = fE;

  GeomLength();
  
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

   return;
 
}

/*-------------------------------------------------------
  Photon -> paire e-/e+                                 -
  Calculate parameters of these particles (e_ph, x,y,z) -
 ------------------------------------------------------*/

void ParticleInTank::PairProduction()
{
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
   p1->DoDetSim();
   fNCherPhot += p1->fNCherPhot;
   for(int iph= 0; iph<p1->fNCherPhot; iph++){
     fTCherPhot.push_back(p1->fTCherPhot[iph]);
     fIpm.push_back(p1->fIpm[iph]);
     fNReflexions.push_back(p1->fNReflexions[iph]);    
   }
   delete p1;
   
   ParticleInTank * p2=
     new ParticleInTank(fISta,-2,fId,xx,yy,zz,fUX,fUY,fUZ,timr,e2,fW);
   p2->DoDetSim();
   fNCherPhot += p2->fNCherPhot;
   for(int iph = 0;iph<p2->fNCherPhot;iph++){
     fTCherPhot.push_back(p2->fTCherPhot[iph]);
     fIpm.push_back(p2->fIpm[iph]);
     fNReflexions.push_back(p2->fNReflexions[iph]);
   }   
   delete p2;
}


/*---------------------------------------------------------
  Photon -> Compton effect                                -
  Calculate new parameters for photon and e- created      -
 --------------------------------------------------------*/

void ParticleInTank::ComptonScattering()
{
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
  }
  //  scattered electron
  //VERSION 1 :
  if(e1>EMIN_E) {
    p1 = sqrt(2*emass*e1 + e1*e1);
    ux_el = (e_ph*ux - efin*ux_f)/p1;
    uy_el = (e_ph*uy - efin*uy_f)/p1;
    uz_el = (e_ph*uz - efin*uz_f)/p1;
  
    ParticleInTank * pel=
      new ParticleInTank(fISta,2,fId,xx,yy,zz,ux_el,uy_el,uz_el,fT,e1,fW);
    pel->GeomLength();
    pel->DoDetSim();
    fNCherPhot += pel->fNCherPhot;

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
  // step in m and T in Gev
  double gamma = 1/sqrt(1.-beta*beta);
  double Tmax = MASS_E* beta* beta*gamma*gamma;
  double meanNbDR = DELTARAYCONST*xstep/beta*(1./gTMinForDeltaRays-1./Tmax);
  int nbDR = (int)Rand("POISSON",meanNbDR);
  double DE = 0;
  
  for(int n=0; n < nbDR; n++)
    {
      double invT = 1./gTMinForDeltaRays-Rand()*(1./gTMinForDeltaRays-1./Tmax);
      double T = 1/invT;
      double costheta = T/Tmax*sqrt((Tmax*Tmax+2*MASS_E*Tmax)/
				  (T*T+2*MASS_E*T));
      
        
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
	if( (dr_z>0) && (dr_z<STATION_HEIGHT)*(1-EPSILON)  && radius <STATION_RADIUS*(1-EPSILON) )
	  
	  {
	    ParticleInTank* p1 = new ParticleInTank(fISta,dr_id,fId,dr_x,dr_y,dr_z,dr_ux/norm,
						    dr_uy/norm,dr_uz/norm,time,dr_E,fW);
	    p1->DoDetSim();
	    fNCherPhot += p1->fNCherPhot;
	    
	    for(int iph=0; iph<p1->fNCherPhot; iph++){
	      fTCherPhot.push_back(p1->fTCherPhot[iph]);
	      fIpm.push_back(p1->fIpm[iph]);
	      fNReflexions.push_back(p1->fNReflexions[iph]);
	    }
	    delete p1;
	    
	    DE+=T;
	  }
    }
  
  return DE;
}

double FuncDistribThetaCM(double x, double rand)
{
  return M_PI*rand-x-sin(x);
}


int ParticleInTank::MuonDecay()
{
  return 0;
}


/*----------------------------------------------------------
  Electron processes
----------------------------------------------------------*/
 
Double_t  ParticleInTank::Bremstrahlung(Int_t istep,Double_t dxstep,Double_t beta){
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
  // In fact angles are small compared to scattering, so that is not a problem
  
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
      double brUX = costh * fUX + sinth * (cosph*vx+sinph*wx);
      double brUY = costh * fUY + sinth * (cosph*vy+sinph*wy);
      double brUZ = costh * fUZ + sinth * (sinph*wz);
      
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
  fUZ = costh * fUZ + sinth * (sinph*wz);
  
}

 

/*----------------------------------------------------------
  Cherenkov production
----------------------------------------------------------*/
 
void ParticleInTank::CherPhot(Double_t beta,Double_t range)
{
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
  costh_cher = 1./(WATINDEX*beta);
  if(costh_cher>1)return;

  sinth_cher = sqrt(1-costh_cher*costh_cher);
  Double_t nn = (gCherFact*sinth_cher*sinth_cher*range*fW);
  n = (Int_t)nn;
  if(Rand()<nn-n) n++;

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
	  qe = PMTQE[iwa]; 
        }
      
      // following a Cherenkov photon
      ipm = this->Follow(x_ph,y_ph,z_ph,cx_ph,cy_ph,cz_ph,
			 prb_refdif,prb_spec,abslen);
      if(ipm>0) { nphot++;}

    }
  
  fNCherPhot += nphot;
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
	    }
          
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
		      return(ipm);
                    }
                }
            }//loop on PMT

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
             
            }//end active
	    
          }//end active
	  
        }//end top
      
      // bottom wall ---------------------------------------
      
	else {
	  
          glength = -zz/czz;
          active = Rand()< exp(-glength/abslen);
          active = active && Rand() < prb_refdif;
	  if(active){
            xx += glength*cxx;
            yy += glength*cyy;
            zz=0.;
            tlength += glength;
            ind++;
            Double_t spec = Rand();
	    if(spec < prb_spec){
              czz=-czz;
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
	      }
	    
          }//end active
	  
        }//end bottom */
    }
  return(ipm);
}







//------------------------------------------------------------------------//
//------------------------------------------------------------------------//
//------------------------ ED DETECTOR -----------------------------------//
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
  Proba interaction                               -
  Proba processes (3 processes) :
  Interaction : - Compton effect (use Geant 4)    -
                - Pair Production (all deposited) -
                - Photoelectric Effect (Geant4)   - 
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
  WhereAmI = 0;
  fISta = ist;
  fX = x;
  fY = y;
  fZ = z;
  fW = weight;
  GLength = 0;
  ProbNoInt = 0;
  ProbIntLead = 0;
  ProbPair = 0;
  fNPhot = 0;
  fNPhotList.clear();
  fCodePhot.clear();
  //fTPhot =0;
  fTPhot.clear();
  fIpm.clear();
  //fIpm = 0;
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
  WhereAmI = 0;
  fISta = ist;
  fX = x;
  fY = y;
  fZ = z;
  fW = weight;
  GLength = 0;
  ProbNoInt = 0;
  ProbIntLead = 0;
  ProbPair = 0;
  fNPhot = 0;
  fNPhotList.clear();
  fCodePhot.clear();
  //fTPhot =0;
  //fCodePhot.clear();
  fTPhot.clear();
  fIpm.clear();
  //fIpm = 0;
  fBinContent.clear();
}

ParticleInScint::~ParticleInScint()
{

}


/*-------------------------------------------
 -   DETECTOR SIMULATION FOR EACH PARTICLE  -
 ------------------------------------------*/


/* --------------------------------------------------------
-  Manager of the step by step propagation of each        -
-  particle in a scintillator / lead.                     -
-  In each step, determine the length, the loss of energy -
-  and generate photons.                                  -
---------------------------------------------------------*/

// In Lead :
void ParticleInScint::DoLeadSim()  
{
  Double_t randomIntLead;
  if(PrintCout == 1) cout << "\n DoLeadSim" << endl;

  // GAMMAS
    if (abs(fId) == 1)
    {
      if(PrintCout == 1) cout << "GAMMA " << endl; //(fId,fE,fX,fY,fZ,fUX,fUY,fUZ,fT) \n = " << fId << ", " << fE << ", " << fX << ", " << fY << ", " << fZ << ", " << fUX << ", " << fUY << ", " << fUZ << ", " << fT << endl;

      WhereAmI = 1; // In lead
      GeomLengthLead();
      InteractionProbabilityInLead();
      randomIntLead = Rand();
      if (randomIntLead < ProbIntLead)
	{
	  ProbabilityProcess();
	  DoGammaProcesses_Lead();
	}
      else
	PositionEnterScint();
    }	
    
    // ELECTRONS
    else if(abs(fId) == 2) 
      {
	if(PrintCout == 1) cout << "ELECTRON " << endl; //(fId,fE,fX,fY,fZ,fUX,fUY,fUZ,fT) \n = " << fId << ", " << fE << ", " << fX << ", " << fY << ", " << fZ << ", " << fUX << ", " << fUY << ", " << fUZ << ", " << fT << endl;
	if( (fE*1E3)*(fE*1E3) > me*me )
	  { 
	    GeomLengthLead();
	    if(EnergyLossInLead())
	      PositionExitLead();
	  }
	else 
	  if(PrintCout == 1) cout << " fE_MeV < me   -->  racine<0 --> on annule ce cas !!!" << endl;
      }
    
    // MUONS
    else if (abs(fId) == 3) 
      {
	if(PrintCout == 1) cout << "MUON " << endl; //(fId,fE,fX,fY,fZ,fUX,fUY,fUZ,fT) \n = " << fId << ", " << fE << ", " << fX << ", " << fY << ", " << fZ << ", " << fUX << ", " << fUY << ", " << fUZ << ", " << fT << endl;
	if((fE*1E3)*(fE*1E3) > M*M )
	  {
	    GeomLengthLead();
	    if(EnergyLossInLead())
	      PositionExitLead();
	  }
	else
	  if(PrintCout == 1) cout << " fE_MeV < M   -->  racine<0 --> on annule ce cas !!!" << endl;
      }

    else return;
}


// In Scintillator :

void ParticleInScint::DoScintAfterDR()  
{
  if (abs(fId) == 2) 
    {
      GeomLengthScint();
      if( (fE*1E3)*(fE*1E3) > me*me )
	{
	  GeomLengthScint(); 
	  if(EnergyLossInScint()) 
	    GeneratePhoton();
	}
      else
	if(PrintCout == 1) cout << " fE_MeV < me   -->  racine<0 --> on annule ce cas !!!" << endl;
    }
}

void ParticleInScint::DoScintSim()  
{
  Double_t randomIntScint;
  if(PrintCout == 1) cout << "\n DoScintSim" << endl;
  
  // GAMMAS
  if (abs(fId) == 1)
    {
      if(PrintCout == 1) cout << "GAMMA (fId,fE,fX,fY,fZ,fUX,fUY,fUZ,fT) \n = " << fId << ", " << fE << ", " << fX << ", " << fY << ", " << fZ << ", " << fUX << ", " << fUY << ", " << fUZ << ", " << fT << endl;
      
      WhereAmI = 2; // In scintillator
      GeomLengthScint();
      InteractionProbabilityInScint();
      randomIntScint = Rand();
      if(randomIntScint > ProbNoInt) {
	  PairProbability();
	  DoGammaProcesses_Scint();
	}
    }       
  
  // ELECTRONS
  else if (abs(fId) == 2) 
    {
      if(PrintCout == 1) cout << "ELECTRON (fId,fE,fX,fY,fZ,fUX,fUY,fUZ,fT) \n = " << fId << ", " << fE << ", " << fX << ", " << fY << ", " << fZ << ", " << fUX << ", " << fUY << ", " << fUZ << ", " << fT << endl;
      GeomLengthScint();
      if (PrintCout == 1) cout << "fNPhot avant DR = " << fNPhot << endl; 
      GenerateDeltaRays();
      if (PrintCout == 1) cout << "fNPhot après DR = " << fNPhot << endl;
      if( (fE*1E3)*(fE*1E3) > me*me )
	{
	  GeomLengthScint(); 
	  if(EnergyLossInScint()) 
	    GeneratePhoton();
	}
      else
	if(PrintCout == 1) cout << " fE_MeV < me   -->  racine<0 --> on annule ce cas !!!" << endl;
    }

  // MUONS
  else if (abs(fId) == 3)  
    {
      if(PrintCout == 1) cout << "MUON (fId,fE,fX,fY,fZ,fUX,fUY,fUZ,fT) \n = " << fId << ", " << fE << ", " << fX << ", " << fY << ", " << fZ << ", " << fUX << ", " << fUY << ", " << fUZ << ", " << fT << endl;
      if ( (fE*1E3)*(fE*1E3) > M*M )
	{ 
	  GeomLengthScint(); 
	  if(EnergyLossInScint())  
	    GeneratePhoton();
	}
      else
	if(PrintCout == 1) cout << " fE_MeV < M   -->  racine<0 --> on annule ce cas !!!" << endl;
    }


  else return;
}




  /*------------------------------------
  - Computes the distance between the  -
  - particle and the scintillator.     -
  ------------------------------------*/
void ParticleInScint::GeomLengthLead()
{
  if(PrintCout == 1) cout << "GeomLength in Lead !" << endl;
  Double_t aa,bb,cc;
  aa = fUX*fUX + fUY*fUY;
  bb = fX*fUX + fY*fUY;
  cc = LEAD_RADIUS*LEAD_RADIUS - fX*fX - fY*fY;
  //   length up to the external volume
  if(aa != 0) GLength = (-bb + sqrt(bb*bb + cc*aa))/aa;
  else GLength = INFINITY;
  //   length up to the top or the bottom
  if((GLength < 0 || (bb*bb + cc*aa) < 0) && fUZ < 0) GLength = -fZ/fUZ;
  else if(fUZ < 0) GLength = min(GLength,-fZ/fUZ);
  else if(fUZ != 0)         
     GLength = min(GLength,(LEAD_HEIGHT-fZ)/fUZ);
   else  GLength = 2 * (fX*fUX + fY*fUY);

  if(PrintCout == 1) cout << "GLength in Lead = " << GLength << endl;
  return;
}

void ParticleInScint::GeomLengthScint()
{
  if(PrintCout == 1) cout << "GeomLength in Scintillator !" << endl;
  Double_t aa,bb,cc;
  aa = fUX*fUX + fUY*fUY;
  bb = fX*fUX + fY*fUY;
  cc = SCINT_RADIUS*SCINT_RADIUS - fX*fX - fY*fY;
  //   length up to the external volume
  if(aa != 0) GLength = (-bb + sqrt(bb*bb + cc*aa))/aa;
  else GLength = INFINITY;
  //   length up to the top or the bottom
  if((GLength < 0 || (bb*bb + cc*aa) < 0) && fUZ < 0) GLength = -fZ/fUZ;
  else if(fUZ < 0) GLength = min(GLength,-fZ/fUZ);
  else if(fUZ != 0)         
    GLength = min(GLength,(SCINT_HEIGHT-fZ)/fUZ);
  else  GLength = 2 * (fX*fUX + fY*fUY);
 
  if(PrintCout == 1) cout << "GLength in Scint= " << GLength << endl;
  return;
}


  /*-----------------------------------------------------------
  -                     FOR GAMMAS                            - 
  -                                                           -
  - A/ ED composed of Lead & Scintillator :                   -
  -    ---> DoLeadSim()   *Chosen in Station.cc*              -
  -    1) GeomLengthLead ()                                   -
  -    -> Compute distance to sides : GLength                 -   
  -    2) InteractionProbabilityInLead() --> Compute proba    -
  -    to do an interaction                                   -
  -    in lead.                                               -
  -    2.a) If doesn't interact --> New coord. when reach sc  -
  -    --> PositionEnterScint() -> DoScintSim()               - 
  -    2.b) If iteract with lead :                            -
  -    *ProbabilityProcess() --> Probability to do a          -
  -    Pair/Compton/Photolectic with a random int length      -
  -    *DoGammaProcesses_Lead() --> Select the process        -
  -    3.a) If pair production -> create e+/e-                -
  -    ---> New coord. -> DoLeadSim()                         - 
  -    3.b) If compton effect -> create new gamma & e-        -
  -    ---> New coord. -> DoLeadSim()                         - 
  -    3.c) If photoelectric -> new e- (less El)              -
  -    ---> New coord. -> DoLeadSim()                         -
  -                                                           -
  - B/ ED composed only with Scintillator :                   -
  -    ---> DoScintSim ()  *Chosen in Station.cc*             -
  -    1) Compute the probability to do an interaction        -
  -    in the scintillator.                                   -
  -    2.a) If doesn't interact ---> Do nothing !!            -
  -    2.b) If interact with the scintillator :               -     
  -    Probability to do a Pair/Compton with random length    -
  -    3.a) If pair production -> create e+/e-                -
  -    ---> New coord. -> DoScintSim()                        -  
  -    3.b) If compton effect -> create new gamma & e-        -
  -    ---> New coord. -> DoScintSim()                        -  
  -                                                           -
  -----------------------------------------------------------*/




///////////////////////////////////////
// PROBABILITY TO INTERACT IN LEAD ? //
///////////////////////////////////////

void ParticleInScint::InteractionProbabilityInLead()
{
  if(PrintCout == 1) cout << "InteractionProbabilityInLead" << endl;
  Double_t GammaEnergy = fE;
  Int_t itab;
  
  itab = (Int_t)((log10(GammaEnergy) + 4.1)/0.2);
  if(itab <= 0) { ProbIntLead = TABINTERACT_Pb[0]; }
  else 
    {
    if(itab >= (NTAB_PHOT-1)) {ProbIntLead = TABINTERACT_Pb[NTAB_PHOT-1];}
     else{
       double log10Einf = -4.1 + 0.2 * itab;
       double log10Esup = -4.1 + 0.2 * (itab+1);
       ProbIntLead = TABINTERACT_Pb[itab] + (log10(GammaEnergy) - log10Einf) * (TABINTERACT_Pb[itab+1] - TABINTERACT_Pb[itab]) / (log10Esup - log10Einf);
     }
    }
}

/////////////////////////////////////////////// 
// IF INTERACT IN THE LEAD : PROBA PROCESS ? //
///////////////////////////////////////////////

void ParticleInScint::ProbabilityProcess()
{
  if(PrintCout == 1) cout << "ProbabilityProcess" << endl;
  Double_t GammaEnergy = fE;
  Int_t itab;
  
  itab = (Int_t)((log10(GammaEnergy) + 4.1)/0.2);
  if(itab <= 0) 
    { 
      ProbPairLead = TABPROBPAIR_Pb[0]; 
      ProbComptLead = TABPROBCOMPT_Pb[0];
      ProbPhotLead = 1 - ProbPairLead - ProbComptLead;
    }
  else if(itab >= (NTAB_PHOT-1))  
    { 
      ProbPairLead = TABPROBPAIR_Pb[NTAB_PHOT-1];
      ProbComptLead = TABPROBCOMPT_Pb[NTAB_PHOT-1];
      ProbPhotLead = 1 - ProbPairLead - ProbComptLead;
    }
  else
    {
     double log10Einf = -4.1 + 0.2 * itab;
     double log10Esup = -4.1 + 0.2 * (itab+1);
     ProbPairLead = TABPROBPAIR_Pb[itab] + (log10(GammaEnergy) - log10Einf) * (TABPROBPAIR_Pb[itab+1] - TABPROBPAIR_Pb[itab]) / (log10Esup - log10Einf);
     ProbComptLead = TABPROBCOMPT_Pb[itab] + (log10(GammaEnergy) - log10Einf) * (TABPROBCOMPT_Pb[itab+1] - TABPROBCOMPT_Pb[itab]) / (log10Esup - log10Einf);
     ProbPhotLead = 1 - ProbPairLead - ProbComptLead;
   }
}



/////////////////////////////////////////////// 
// IF INTERACT IN THE LEAD : WHICH PROCESS ? //
///////////////////////////////////////////////

void ParticleInScint::DoGammaProcesses_Lead()
{
  if(PrintCout == 1) cout << "DoGammaProcesses_Lead" << endl;
  double randomProcess, InteractionLength;
  randomProcess = Rand();

  if(PrintCout == 1) cout << "randomProcess = " << randomProcess << " & ProbPairLead = " << ProbPairLead << " & ProbComptLead = " << ProbComptLead << endl;
  
  if (randomProcess <= ProbPairLead)
    PairProduction();
  else if (ProbPairLead < randomProcess && randomProcess <= ProbPairLead + ProbComptLead)
    ComptonScattering();
  else if (randomProcess > ProbPairLead + ProbComptLead)
    PhotoElectricEffect();
  else
    if(PrintCout == 1) cout << "###################### ERROR #####################\n ######## NO PROCESS CHOSEN FOR THIS GAMMA ######## \n ##################################################" << endl;
}




///////////////////////////////////////////////////////
// NEW POSITION WHEN GAMMA REACHES THE SCINTILLATOR  //
///////////////////////////////////////////////////////

void ParticleInScint::PositionEnterScint()
{
  if(PrintCout == 1) cout << "PositionEnterScint \n" << endl;
  Double_t xx, yy, zz, ux, uy, uz, x_out, y_out, z_out, timr, distance, factor_param;

  /* Calculate new position when reaches scintillator
     1/ Position of particle in lead (xx,yy,zz,ux,uy,uz) */
  
  xx = fX;
  yy = fY;
  zz = fZ;
  ux = fUX;
  uy = fUY;
  uz = fUZ;
  timr = fT; 
 
  // 2/ Position of exit point from bottom of lead
  factor_param = - zz/uz;
  x_out = xx + factor_param * ux;
  y_out = yy + factor_param * uy;
  z_out = 0.; 
  
  // 3/ Distance between initial point and exit point
  distance = sqrt( (x_out-xx)*(x_out-xx) + (y_out-yy)*(y_out-yy) + (z_out-zz)*(z_out-zz) );
  
  if(PrintCout == 1) cout << "x_out,y_out,z_out,distance = " << x_out << "," << y_out << "," << z_out << "," << distance << endl;
 
  // 4/ Condition on the position of exit point & new coord.
  if ( x_out >= - LEAD_RADIUS && x_out <= LEAD_RADIUS && y_out >= - LEAD_RADIUS && y_out <= LEAD_RADIUS) 
    {
      xx = x_out;
      yy = y_out;
      zz = SCINT_HEIGHT;
      timr = timr + distance/CLIGHT;
    
      ParticleInScint * p1 =
	new ParticleInScint(fISta,fId,xx,yy,zz,fUX,fUY,fUZ,timr,fE,fW);
      p1->DoScintSim();
      fNPhot += p1->fNPhot;
      //fTPhot = p1->fTPhot;
      for(int iph = 0; iph < p1->fNPhot; iph++) {
      	fTPhot.push_back(p1->fTPhot[iph]); 
	fIpm.push_back(p1->fIpm[iph]);
      }
      delete p1;
    }
}




///////////////////////////////////////////////////
// PROBABILITY TO INTERACT IN THE SCINTILLATOR ? //
///////////////////////////////////////////////////

void ParticleInScint::InteractionProbabilityInScint()
{
  if(PrintCout == 1) cout << "InteractionProbabilityInScint" << endl;
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



///////////////////////////////////// 
// IF INTERACT IN THE SCINTILLATOR //
/////////////////////////////////////

void ParticleInScint::DoGammaProcesses_Scint()
{
  if(PrintCout == 1) cout << "DoGammaProcesses_Scint" << endl;
  double randomPair = Rand();
  if(PrintCout == 1) cout << "randomPair = " << randomPair << " & ProbPair = " << ProbPair << endl;
  if (randomPair <= ProbPair)
    PairProduction(); 
  else 
    ComptonScattering();
  
  GeneratePhoton();
}


///////////////////////////////////////////
// PROBABILITY TO DO A PAIR PRODUCTION ? //
//         (only in scintillator)        //
///////////////////////////////////////////

void ParticleInScint::PairProbability()
{
  if(PrintCout == 1) cout << "PairProbability" << endl;
  Int_t itab;
  Double_t GammaEnergy = fE;
  
  if(GammaEnergy <= kEMinPh) return;
  
  itab = (Int_t)((log10(GammaEnergy) + 4.1)/0.2);
  if(itab <= 0)  { ProbPair = TABPROBPAIR_Scint[0];}
  else{
    if(itab >= (NTAB_PHOT-1)) { ProbPair = TABPROBPAIR_Scint[NTAB_PHOT-1];}
    else{
      double log10Einf = -4.1 + 0.2 * itab;
      double log10Esup = -4.1 + 0.2 * (itab+1);
      ProbPair = TABPROBPAIR_Scint[itab] + (log10(GammaEnergy) - log10Einf) * (TABPROBPAIR_Scint[itab+1] - TABPROBPAIR_Scint[itab]) / (log10Esup - log10Einf);
    }
  }
  
  return;
}
 

///////////////////////////////////////////////////
//          -----> PROCESSES                     //
///////////////////////////////////////////////////

////////////////////////
// IF PAIR PRODUCTION //
////////////////////////

void ParticleInScint::PairProduction()
{
  if(PrintCout == 1) cout << "PairProduction" << endl;
  Double_t xx,yy,zz,ux,uy,uz,e_ph,timr,weight;
  Double_t cc;
  Double_t e1,e2;
  Double_t x_min,x_max,y_min,y_max,z_min,z_max,distance;
  Double_t x_min_temp,x_max_temp,y_min_temp,y_max_temp,z_min_temp,z_max_temp;
  Double_t random1, random2, random3;
 
  xx = fX;
  yy = fY;
  zz = fZ;
  ux = fUX;
  uy = fUY;
  uz = fUZ;
  e_ph = fE;
  timr = fT;
  weight = fW;
  
  
// Calcul bornes du Rand()
  if(WhereAmI == 1)
    {
      x_min_temp = (- LEAD_RADIUS - xx)/ux;
      x_max_temp = (LEAD_RADIUS - xx)/ux;
      y_min_temp = (- LEAD_RADIUS - yy)/uy;
      y_max_temp = (LEAD_RADIUS - yy)/uy;
      z_min_temp = - zz/uz;
      z_max_temp = (LEAD_HEIGHT - zz)/uz;
      cc = LEAD_RADIUS*LEAD_RADIUS - xx*xx - yy*yy;
    }
  if(WhereAmI == 2)
    {
      x_min_temp = (- SCINT_RADIUS - xx)/ux;
      x_max_temp = (SCINT_RADIUS - xx)/ux;
      y_min_temp = (- SCINT_RADIUS - yy)/uy;
      y_max_temp = (SCINT_RADIUS - yy)/uy;
      z_min_temp = - zz/uz;
      z_max_temp = (SCINT_HEIGHT - zz)/uz;
      cc = SCINT_RADIUS*SCINT_RADIUS - xx*xx - yy*yy;
    }
  x_min = min(x_min_temp,x_max_temp);
  x_max = max(x_min_temp,x_max_temp);
  y_min = min(y_min_temp,y_max_temp);
  y_max = max(y_min_temp,y_max_temp);
  z_min = min(z_min_temp,z_max_temp);
  z_max = max(z_min_temp,z_max_temp);
  random1 = x_min + Rand() * (x_max - x_min);
  random2 = y_min + Rand() * (y_max - y_min);
  random3 = z_min + Rand() * (z_max - z_min);

  // Nouvelles coordonnées (xx,yy,zz,timr) :
  xx = xx + random1 * ux;
  yy = yy + random2 * uy;
  zz = zz + random3 * uz;
  distance = sqrt( (xx-fX)*(xx-fX) + (yy-fY)*(yy-fY) + (zz-fZ)*(zz-fZ) );
  timr = timr + distance / CLIGHT;

  // Énergies de la paire e- / e+ :
  e1 = (e_ph - 2*MASS_E) * Rand();
  e2 = (e_ph - 2*MASS_E) - e1;
  
  if(PrintCout == 1) cout << "New coord. xx,yy,zz,timr,e1,e2 = " << xx << "," << yy << "," << zz << "," << timr << "," << e1 << "," << e2 << endl;
  
  ParticleInScint * p1 =
    new ParticleInScint(fISta,2,fId,xx,yy,zz,fUX,fUY,fUZ,timr,e1,fW);
  if (WhereAmI == 1) {
    p1->DoLeadSim();
    fNPhot += p1->fNPhot;
    fNPhotList.push_back(fNPhot);
    if (PrintCout == 1) cout << "fNPhot = " << p1->fNPhot << endl;
    fCodePhot.push_back(101);
    for(int iph = 0; iph < p1->fNPhot; iph++) {
      fTPhot.push_back(p1->fTPhot[iph]);
      fIpm.push_back(p1->fIpm[iph]);
    }
  } 
  else if (WhereAmI == 2) {
    p1->DoScintSim();
    fNPhot += p1->fNPhot; 
    fNPhotList.push_back(p1->fNPhot);
    if (PrintCout == 1) cout << "fNPhot = " << p1->fNPhot << endl;
    fCodePhot.push_back(11);
    for(int iph = 0; iph < p1->fNPhot; iph++) {
    fTPhot.push_back(p1->fTPhot[iph]);
    fIpm.push_back(p1->fIpm[iph]);
    }
  }
  delete p1;
  
  ParticleInScint * p2=
    new ParticleInScint(fISta,-2,fId,xx,yy,zz,fUX,fUY,fUZ,timr,e2,fW);
  if (WhereAmI == 1) {
    p2->DoLeadSim();
    fNPhot += p2->fNPhot;
    fCodePhot.push_back(102);
    fNPhotList.push_back(p2->fNPhot);
    if (PrintCout == 1) cout << "fNPhot = " << p2->fNPhot << endl;
    fTPhot = p2->fTPhot;
    for(int iph = 0; iph < p2->fNPhot; iph++) {
      fTPhot.push_back(p2->fTPhot[iph]);
      fIpm.push_back(p2->fIpm[iph]);
    }
  } 
  else if (WhereAmI == 2) {
    p2->DoScintSim();
    fNPhot += p2->fNPhot;
    fNPhotList.push_back(fNPhot);
    if (PrintCout == 1) cout << "fNPhot = " << p2->fNPhot << endl;
    fCodePhot.push_back(12);
    //fTPhot = p1->fTPhot;
    for(int iph = 0; iph < p2->fNPhot; iph++) {
      fTPhot.push_back(p2->fTPhot[iph]);
      fIpm.push_back(p2->fIpm[iph]);
    }
  } 
  delete p2;
}


///////////////////////////////////////////////
// IF COMPTON SCATTERING, ENERGY DEPOSITED ? //
///////////////////////////////////////////////

void ParticleInScint::ComptonScattering()
{
  if(PrintCout == 1) cout << "ComptonScattering" << endl;
  // me = electron mass in MeV = 0.511
  Double_t e_ph;
  Double_t xx,yy,zz,ux,uy,uz,timr;
  Double_t x_min,x_max,y_min,y_max,z_min,z_max,distance;
  Double_t x_min_temp,x_max_temp,y_min_temp,y_max_temp,z_min_temp,z_max_temp;
  Double_t hlength;
  Double_t emin,smax,efin,sfin;
  Double_t c_dth,s_dth,s_dph,c_dph,dph;
  Double_t uxy,vx,vy,vz,wx,wy,wz;
  Double_t ux_f=0.,uy_f=0.,uz_f=0.,ux_el,uy_el,uz_el;
  Double_t e1, p1, aa, bb, cc;
  Int_t reject;
  Double_t random1, random2, random3;
  
  xx = fX;
  yy = fY;
  zz = fZ;
  ux = fUX;
  uy = fUY;
  uz = fUZ;
  e_ph = fE; // in GeV

  // Limit of  Rand()
  if(WhereAmI == 1)
    {
      x_min_temp = (- LEAD_RADIUS - xx)/ux;
      x_max_temp = (LEAD_RADIUS - xx)/ux;
      y_min_temp = (- LEAD_RADIUS - yy)/uy;
      y_max_temp = (LEAD_RADIUS - yy)/uy;
      z_min_temp = - zz/uz;
      z_max_temp = (LEAD_HEIGHT - zz)/uz;
    }
  if(WhereAmI == 2)
    {
      x_min_temp = (- SCINT_RADIUS - xx)/ux;
      x_max_temp = (SCINT_RADIUS - xx)/ux;
      y_min_temp = (- SCINT_RADIUS - yy)/uy;
      y_max_temp = (SCINT_RADIUS - yy)/uy;
      z_min_temp = - zz/uz;
      z_max_temp = (SCINT_HEIGHT - zz)/uz;
    }
  x_min = min(x_min_temp,x_max_temp);
  x_max = max(x_min_temp,x_max_temp);
  y_min = min(y_min_temp,y_max_temp);
  y_max = max(y_min_temp,y_max_temp);
  z_min = min(z_min_temp,z_max_temp);
  z_max = max(z_min_temp,z_max_temp);
  random1 = x_min + Rand() * (x_max - x_min);
  random2 = y_min + Rand() * (y_max - y_min);
  random3 = z_min + Rand() * (z_max - z_min);

  // New coord. (xx,yy,zz,timr) :
  xx = xx + random1 * ux;
  yy = yy + random2 * uy;
  zz = zz + random3 * uz;
  distance = sqrt( (xx-fX)*(xx-fX) + (yy-fY)*(yy-fY) + (zz-fZ)*(zz-fZ) );
  timr = timr + distance / CLIGHT;

  // Calcul energy
  emin = 1. / ( 1./e_ph + 2./MASS_E );
  smax = emin/e_ph + e_ph/emin + pow((1. + MASS_E/e_ph - MASS_E/emin),2) - 1.;
  
  reject = 1;
  while(reject)
    {
      efin = emin + Rand()*(e_ph - emin);
      sfin = efin/e_ph + e_ph/efin;
      reject = Rand() > sfin/smax;
    }
// After compton : e1 -> energy e-  &  efin -> energy phot
  e1 = e_ph - efin;   

  if(PrintCout == 1) cout << "e_ph,e1,efin,EMIN_E,kEMinPh = " << e_ph << "," << e1 << "," << efin << "," << EMIN_E << "," << kEMinPh << endl;

 if(e1>EMIN_E || efin> kEMinPh) {
    //diffusion angles of the photon
    c_dth = 1 + MASS_E*(1./e_ph - 1./efin);
    s_dth = sqrt(1 - c_dth*c_dth);
    dph = 2 * PI * Rand();
    c_dph = cos(dph);
    s_dph = sin(dph);

    //new direction cosines of the photon
    uxy = sqrt(ux*ux + uy*uy);

    if(PrintCout == 1) cout << "if(e1 > EMIN_E || efin> kEMinPh) :  c_dth,s_dth,dph,c_dph,s_dph,uxy = " << c_dth << "," << s_dth << "," << dph << "," << c_dph << "," << s_dph << "," << uxy << endl;


    if(uxy != 0){
      vx = -uy/uxy;
      vy = ux/uxy;
      vz = 0.;
      wx = uy*vz-uz*vy;
      wy = uz*vx-ux*vz;
      wz = ux*vy-uy*vx;

 if(PrintCout == 1) cout << "if(uxy != 0) : vx,vy,vz,wx,wy,wz = " << vx << "," << vy << "," << vz << "," << wx << "," << wy << "," << wz << endl;
    }

    else
      {
	vx = 1;
	vy = 0;
	vz = 0;
	wx = 0;
	wy = 1;
	wz = 0;

if(PrintCout == 1) cout << "if(uxy == 0) : vx,vy,vz,wx,wy,wz = " << vx << "," << vy << "," << vz << "," << wx << "," << wy << "," << wz << endl;
      }

    ux_f = c_dth*ux+s_dth*(c_dph*vx+s_dph*wx);
    uy_f = c_dth*uy+s_dth*(c_dph*vy+s_dph*wy);
    uz_f = c_dth*uz+s_dth*(c_dph*vz+s_dph*wz);

    if(PrintCout == 1) cout << "ux_f,uy_f,uz_f = " << ux_f << "," << uy_f << "," << uz_f  << endl;
  }
 
 //  scattered electron
 if(e1>EMIN_E) {
   p1 = sqrt(2*MASS_E*e1 + e1*e1);
   ux_el = (e_ph*ux - efin*ux_f)/p1;
   uy_el = (e_ph*uy - efin*uy_f)/p1;
   uz_el = (e_ph*uz - efin*uz_f)/p1;
   
   if(PrintCout == 1) cout << "New coord. electron (e1>EMIN_E) : xx,yy,zz,ux_el,uy_el,uz_el,timr,e1 = " << xx << "," << yy << "," << zz << "," << ux_el << "," << uy_el << "," << uz_el << "," << timr << "," << e1 << endl;

   ParticleInScint * pel=
     new ParticleInScint(fISta,-2,fId,xx,yy,zz,ux_el,uy_el,uz_el,timr,e1,fW);
   if(WhereAmI == 1) {
     pel->DoLeadSim();
     fNPhot += pel->fNPhot;
     fCodePhot.push_back(103);
     fNPhotList.push_back(pel->fNPhot);
     if (PrintCout == 1) cout << "fNPhot = " << pel->fNPhot << endl;
     for(int iph = 0; iph < pel->fNPhot; iph++) {
       fTPhot.push_back(pel->fTPhot[iph]);
       fIpm.push_back(pel->fIpm[iph]);
     }
   } 
   else if(WhereAmI == 2) {
     pel->DoScintSim();
     fNPhot += pel->fNPhot;
     fNPhotList.push_back(pel->fNPhot);
     if (PrintCout == 1) cout << "fNPhot = " << pel->fNPhot << endl;
     fCodePhot.push_back(13);
     //fTPhot = pel->fTPhot;
     for(int iph = 0; iph < pel->fNPhot; iph++) {
       fTPhot.push_back(pel->fTPhot[iph]);
       fIpm.push_back(pel->fIpm[iph]);
     }
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
       
       if(PrintCout == 1) cout << "New coord. ph  xx,yy,zz,ux_f,uy_f,uz_f,timr,efin = " << xx << "," << yy << "," << zz << "," << ux << "," << uy << "," << uz << "," << timr << "," << efin << endl;

       ParticleInScint * ph =
	 new ParticleInScint(fISta,1,fId,xx,yy,zz,ux,uy,uz,timr,efin,fW);
       if(WhereAmI == 1) {
	 ph->DoLeadSim();
	 fNPhot += ph->fNPhot;
	 fCodePhot.push_back(104);
	 fNPhotList.push_back(ph->fNPhot);
	 if (PrintCout == 1) cout << "fNPhot = " << ph->fNPhot << endl;
	 //fTPhot = ph->fTPhot;
	 for(int iph = 0; iph < ph->fNPhot; iph++) {
	   fTPhot.push_back(ph->fTPhot[iph]);
	   fIpm.push_back(ph->fIpm[iph]);
	 }
       } 
       else if(WhereAmI == 2) {
	 ph->DoScintSim();
	 fNPhot += ph->fNPhot;
	 fNPhotList.push_back(fNPhot);
	 fCodePhot.push_back(14);
	 if (PrintCout == 1) cout << "fNPhot = " << ph->fNPhot << endl;
	 //fTPhot = ph->fTPhot;
	 for(int iph = 0; iph < ph->fNPhot; iph++) {
	   fTPhot.push_back(ph->fTPhot[iph]);
	   fIpm.push_back(ph->fIpm[iph]);
	 }
       } 
       delete ph;   
     }
}



/////////////////////////////////////////////////
// IF PHOTOELECTRIC EFFECT, ENERGY DEPOSITED ? //
/////////////////////////////////////////////////

 void ParticleInScint::PhotoElectricEffect()
 {
 if(PrintCout == 1) cout << " Photoelectric Effect !" << endl;
   Double_t xx, yy, zz, ux, uy, uz, e_ph, timr, weight;
   Double_t x_min,x_max,y_min,y_max,z_min,z_max,distance;
   Double_t x_min_temp,x_max_temp,y_min_temp,y_max_temp,z_min_temp,z_max_temp;
   Double_t random1, random2, random3;
   Double_t cc, e1;
   Double_t WorkFunction = 4.25 * 1E-9; //Energy to extract an electron from lead in GeV
 
   xx = fX;
   yy = fY;
   zz = fZ;
   ux = fUX;
   uy = fUY;
   uz = fUZ;
   e_ph = fE;
   timr = fT;
   weight = fW;
   
   // Limit of  Rand()
   x_min_temp = (- LEAD_RADIUS - xx)/ux;
   x_max_temp = (LEAD_RADIUS - xx)/ux;
   y_min_temp = (- LEAD_RADIUS - yy)/uy;
   y_max_temp = (LEAD_RADIUS - yy)/uy;
   z_min_temp = - zz/uz;
   z_max_temp = (LEAD_HEIGHT - zz)/uz;
   
   x_min = min(x_min_temp,x_max_temp);
   x_max = max(x_min_temp,x_max_temp);
   y_min = min(y_min_temp,y_max_temp);
   y_max = max(y_min_temp,y_max_temp);
   z_min = min(z_min_temp,z_max_temp);
   z_max = max(z_min_temp,z_max_temp);
   random1 = x_min + Rand() * (x_max - x_min);
   random2 = y_min + Rand() * (y_max - y_min);
   random3 = z_min + Rand() * (z_max - z_min);

   // New coord. (xx,yy,zz,timr) :
   xx = xx + random1 * ux;
   yy = yy + random2 * uy;
   zz = zz + random3 * uz;
   distance = sqrt( (xx-fX)*(xx-fX) + (yy-fY)*(yy-fY) + (zz-fZ)*(zz-fZ) );
   timr = timr + distance / CLIGHT;
   
   // Electron energy :
   e1 = e_ph - WorkFunction; // electron energy

   if(PrintCout == 1) cout << "New coord. ph  xx,yy,zz,fUX,fUY,fUZ,timr,e1 = " << xx << "," << yy << "," << zz << "," << fUX << "," << fUY << "," << fUZ << "," << timr << "," << e1 << endl;

   ParticleInScint * p1 =
     new ParticleInScint(fISta,-2,fId,xx,yy,zz,fUX,fUY,fUZ,timr,e1,fW);
   p1->DoLeadSim();
   fNPhot += p1->fNPhot; 
   fCodePhot.push_back(105);
   fNPhotList.push_back(fNPhot);
   if (PrintCout == 1) cout << "fNPhot = " << p1->fNPhot << endl;
   //fTPhot = p1->fTPhot;
   for(int iph = 0; iph < p1->fNPhot; iph++) {
     fTPhot.push_back(p1->fTPhot[iph]);
     fIpm.push_back(p1->fIpm[iph]);
   }
   delete p1;
   
   return;
 }



 /*-------------------------------------------------------------
  -                 FOR MUONS AND ELECTRONS                    - 
  -                                                            -
  - A/ ED composed of Lead & Scintillator :                    -
  -    ---> DoLeadSim()   *Chosen in Station.cc*               -
  -    1) GeomLengthLead ()                                    -    
  -    ---> Compute distance to sides with initial position    -
  -    2) EnergyLossInLead ()                                  -
  -    ---> compute energy loss in lead with Bethe & Bloch     -
  -    3) PositionExitLead()                                   -
  -    ---> Coord. when exit Lead                              - 
  -    If in bottom of lead layer & fE>0                       -
  -    -> New particle (new_coord,new_fT,new_fE)->DoScintSim() -
  -    4) GeomLengthScint ()                                   -
  -    --> Compute distance to sides with new position         -
  -    5) EnergyLossInScint ()                                 -
  -    ---> compute energy loss in scint. with Bethe & Bloch   -
  -    6) GeneratePhoton () --> NPhot = EnergyLoss * 8000      -
  -                                                            -
  - B/ ED composed only with Scintillator :                    -
  -    ---> DoScintSim ()  *Chosen in Station.cc*              -
  -    1) GeomLengthScint ()                                   -
  -    --> Compute distance to sides with new position         -
  -    2) EnergyLossInScint ()                                 -
  -    ---> compute energy loss in scint. with Bethe & Bloch   -
  -    3) GeneratePhoton () --> NPhot = EnergyLoss * 8000      -     
  -                                                            -
  ------------------------------------------------------------*/



///////////////////////// 
// ENERGY LOSS IN LEAD //
/////////////////////////

bool ParticleInScint::EnergyLossInLead()
{
  if(PrintCout == 1) cout<<"EnergyLossLead!"<<endl;
  double beta, gamma, eta, s, p, Ek, tau, F, Wmax, lgfactor;
  double FactorLandau, lgfactorLandau;
  double dx = GLength * 100. * density_Pb; // in g/cm²
  double fE_MeV = fE * 1E3; // Convert energy particle GeV -> MeV
  
  switch (abs(fId)) {
    
  case 1:
    return 1;
    

  case 2:
    if(PrintCout == 1) cout << "Energy of electron before Lead, fE_MeV = fE * 1E3 = " << fE_MeV << " MeV" << endl;
    
    //////////////////////////////////
    // BETHE & BLOCH FOR ELECTRON : //
    //////////////////////////////////
    p = sqrt(fE_MeV*fE_MeV - me*me);
    beta = p/fE_MeV; 
    gamma = fE_MeV/me;
    eta = p/me;
    Ek = me * (gamma - 1.);
    tau = Ek/me;
    lgfactor = log( (tau*tau*(tau+2.)) / (2.*(I_Pb/me)*(I_Pb/me)) );
    F = 1. - beta*beta + ( tau*tau/8. - (2.*re+1.)*log(2.) ) / ( (tau+1.)*(tau+1.) );
    
    ElectronEnergyLossLead = factor * Z_Pb/A_Pb * 1/(beta*beta) * (lgfactor + F - delta_Pb - 2.* C_Pb/Z_Pb) * dx; // Energy loss in MeV
    ElectronEnergyLossLead = ElectronEnergyLossLead*1E-3;
   
    if(PrintCout == 1) cout <<"CAS 2 (in Lead) :  fE = " << fE << " GeV, Beta = " << beta << ", Energy Loss in Lead = " << ElectronEnergyLossLead << " GeV, GLength = " << GLength*100 << " cm" << endl;

    ///////////////////////////
    // LANDAU FOR ELECTRON : //
    ///////////////////////////
    // FactorLandau = (K/2) * (Z_Pb/A_Pb) * (dx/(beta*beta));
    // lgfactorLandau = log( (2*me* beta*beta * gamma*gamma)/I_Pb );
    // ElectronEnergyLossLead = FactorLandau * (lgfactorLandau + log(FactorLandau/I_Pb) + j - beta*beta); 
    
    // ElectronEnergyLossLead = ElectronEnergyLossLead*1E-3;
    
    // if(PrintCout == 1) cout <<"CAS 2 (in Lead): fE_MeV = " << fE_MeV << " MeV, p = " << p << ", Beta = " << beta << ", GLength = " << GLength*100 << " cm, FactorLandau = " << FactorLandau << ",lgfactorLandau = " << lgfactorLandau << ", Eloss = " << ElectronEnergyLossLead << " MeV" << endl;
    
    if(ElectronEnergyLossLead > fE_MeV*1E-3)
      {
	if(PrintCout == 1) cout << "##### PB : Eloss > E ???!!! #####" << endl;
	return 0;
      }

    return 1;
    

  case 3:
    if(PrintCout == 1) cout << "Energy of muon before Lead, fE_MeV = fE * 1E3 = " << fE_MeV << " MeV" << endl;
    //////////////////////////////
    // BETHE & BLOCH FOR MUON : //
    //////////////////////////////
    p = sqrt(fE_MeV*fE_MeV - M*M);
    gamma = fE_MeV/M;
    beta = p/fE_MeV; 
    eta = p/M;
    s = me / M;
    Wmax = (2.*me*eta*eta)/(1. + 2.*gamma*s + s*s);
    lgfactor = log( (2.*me * eta*eta * Wmax) / (I_Pb*I_Pb));
    
    MuonEnergyLossLead = factor * Z_Pb/A_Pb * (1/(beta*beta)) * (lgfactor - 2. *(beta*beta) - delta_Pb - 2.* C_Pb/Z_Pb) * dx; 
    MuonEnergyLossLead = MuonEnergyLossLead*1E-3;
    
    ///////////////////////
    // LANDAU FOR MUON : //
    ///////////////////////
    // FactorLandau = (K/2) * (Z_Pb/A_Pb) * (dx/(beta*beta));
    // lgfactorLandau = log( (2*M* beta*beta * gamma*gamma)/I_Pb );
    
    // MuonEnergyLossLead = FactorLandau * (lgfactorLandau + log(FactorLandau/I_Pb) + j - beta*beta);
    // MuonEnergyLossLead = MuonEnergyLossLead*1E-3;
    
    if(PrintCout == 1) cout <<"CAS 3 (in Lead): fE_MeV = " << fE_MeV << " MeV, p = " << p << ", Beta = " << beta << ", GLength = " << GLength*100 << " cm, FactorLandau = " << FactorLandau << ",lgfactorLandau = " << lgfactorLandau << ", Eloss = " << MuonEnergyLossLead << " MeV" << endl;


    if(MuonEnergyLossLead > fE_MeV*1E-3)
      {
	if(PrintCout == 1) cout << "##### PB : Eloss > E ???!!! #####" << endl;
	return 0;
      }

    return 1;
    

  default:
    cerr << "Error in ParticleInScint::EnergyLossLead..." << endl;    
  }
}



///////////////////////////////////////////////////////// 
// POSITION AFTER INTERACTION IN LEAD. GO INTO SCINT ? //
/////////////////////////////////////////////////////////

void ParticleInScint::PositionExitLead()
{
  Double_t xx, yy, zz, ux, uy, uz, x_out, y_out, z_out, factor_param, timr, E, distance;

  if(PrintCout == 1) cout << "PositionExitLead " << endl;
  /* Calculate position after Bethe-Bloch in Lead
     1/ Position of particle in lead (xx,yy,zz,ux,uy,uz) */
  
  xx = fX;
  yy = fY;
  zz = fZ;
  ux = fUX;
  uy = fUY;
  uz = fUZ;
  E = fE; // GeV
  timr = fT;
  
  // 2/ Position of exit point from bottom of lead
  factor_param = - zz/uz;
  x_out = xx + factor_param * ux;
  y_out = yy + factor_param * uy;
  z_out = 0.; 
  
 // 3/ Distance between initial point and exit point
  distance = sqrt( (x_out-xx)*(x_out-xx) + (y_out-yy)*(y_out-yy) + (z_out-zz)*(z_out-zz) );

  if(PrintCout == 1) cout << "Ex_Coord = " << xx << "," << yy << "," << zz << "," << fT << "," << fE << endl;

  if(PrintCout == 1 && abs(fId) == 2) 
    cout << "Coord_exit = " << x_out << "," << y_out << "," << z_out << "," << timr << "," << E - ElectronEnergyLossLead << endl;
  if(PrintCout == 1 && abs(fId) == 3) 
    cout << "Coord_exit = " << x_out << "," << y_out << "," << z_out << "," << timr << "," << E - MuonEnergyLossLead << endl;

  if(E > 0 && x_out >= - LEAD_RADIUS && x_out <= LEAD_RADIUS && y_out >= - LEAD_RADIUS && y_out <= LEAD_RADIUS)
    {
      if(PrintCout == 1) cout << "Conditions E>0 & dans cylindre satisfaites !! " << endl;
      xx = x_out;
      yy = y_out;
      zz = SCINT_HEIGHT;
      timr = timr + distance/CLIGHT;

      if(PrintCout == 1) cout << "E_avant = " << E << endl;
      if(abs(fId) == 2) 
	{ E = E - ElectronEnergyLossLead; 
	  if(PrintCout == 1) cout << "E = E - ElecLossLead : ElecLossLead = " << ElectronEnergyLossLead << endl;// GeV
	}
      if(abs(fId) == 3) 
	{ E = E - MuonEnergyLossLead; 
	  if(PrintCout == 1) cout << "E = E - MuonLossLead : MuonLossLead = " << MuonEnergyLossLead << endl;// GeV
	}
      if(PrintCout == 1) cout << "E_après = " << E << endl;
      
      ParticleInScint * p1 =
	new ParticleInScint(fISta,fId,xx,yy,zz,fUX,fUY,fUZ,timr,E,fW);
      p1->DoScintSim();
      fNPhot += p1->fNPhot; 
      if(abs(fId) == 1)
	fCodePhot.push_back(106);
      if(abs(fId) == 2)
	fCodePhot.push_back(2);
      if(abs(fId) == 3)
	fCodePhot.push_back(3);
      fNPhotList.push_back(fNPhot);
      if (PrintCout == 1) cout << "fNPhot = " << p1->fNPhot << endl;
      //fTPhot = p1->fTPhot;
      for(int iph = 0; iph < p1->fNPhot; iph++) {
      	fTPhot.push_back(p1->fTPhot[iph]);
      	fIpm.push_back(p1->fIpm[iph]);
      }
      delete p1;
    }
}





////////////////////////////////
// DELTA RAYS IN SCINTILLATOR //
////////////////////////////////

void ParticleInScint::GenerateDeltaRays()
{
  if(PrintCout == 1) cout << "Delta Rays" << endl;
  //AfterOneDR = 0;
  // step in m and T in Gev
  double xstep = GLength;
  double p = sqrt(fE*fE - MASS_E*MASS_E);
  double beta = p/fE;
  double gamma = 1/sqrt(1.-beta*beta);
  double Tmax = MASS_E* beta*beta * gamma*gamma;
  double gTMinForDeltaRaysInScint = MASS_E*(1/sqrt(1- 1./(ScintIndex*ScintIndex))-1);
  double meanNbDR = DELTARAYCONST * xstep/beta * (1./gTMinForDeltaRaysInScint-1./Tmax);
  int nbDR = (int)Rand("POISSON",meanNbDR);
  double DE = 0;
  double dr_x, dr_y, dr_z, dr_ux, dr_uy, dr_uz, time;
  
  if (PrintCout == 1) cout << "nbDR = " << nbDR << endl;
  
  //for(int n=0; n < 1; n++)
  for(int n=0; n < nbDR; n++)
    {
      double invT = 1./gTMinForDeltaRaysInScint - Rand()*(1./gTMinForDeltaRaysInScint-1./Tmax);
      double T = 1/invT;
      double costheta = T/Tmax*sqrt((Tmax*Tmax + 2*MASS_E*Tmax)/(T*T+2*MASS_E*T));
        
      // get the distance
      double dist = xstep*Rand();
      
      // orientation of the incident electron in the scint frame
      double UR = sqrt(fUX*fUX+fUY*fUY);
      
      // compute delta ray coordinates
      dr_x = fX + dist * fUX;
      dr_y = fY + dist * fUY;
      dr_z = fZ + dist * fUZ;
      time = fT + dist/(CLIGHT*beta);
      
      // compute the orientation in the electron frame
      double sintheta = sqrt(1-costheta*costheta);
      double phi = 2*PI*Rand();
      double cosphi = cos(phi);
      double sinphi = sin(phi);
      double ux = sintheta*cosphi;
      double uy = sintheta*sinphi;
      double uz = costheta;
      int dr_id = 2;
      if(UR!=0) // electron non purely vertical
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
	if( (dr_z>0) && (dr_z<SCINT_HEIGHT)*(1-EPSILON)  && radius < SCINT_RADIUS*(1-EPSILON) )
	  {
	    if (PrintCout == 1) cout << "electron cree : " << dr_x << " " << dr_y << " " << dr_z << " " << dr_ux/norm << " " << dr_uy/norm << " " << dr_uz/norm << " " << time << " "<< dr_E << endl; 
	    ParticleInScint* p1 = new ParticleInScint(fISta,dr_id,dr_x,dr_y,dr_z,dr_ux/norm,dr_uy/norm,dr_uz/norm,time,dr_E,fW);
	    
	    // p1->DoScintSim();
	    p1->DoScintAfterDR();
	    
	    fNPhot += p1->fNPhot;
	    fNPhotList.push_back(p1->fNPhot);
	    if (PrintCout == 1) cout << "fNPhot = " << p1->fNPhot << " " << fNPhot << endl;
	    for(int iph=0; iph<p1->fNPhot; iph++){
	      fTPhot.push_back(p1->fTPhot[iph]);
	      fIpm.push_back(p1->fIpm[iph]);
	    }
	    delete p1;
	    if (PrintCout == 3 && n == 0) cout << "1  " << (T/fE)*100 << endl;
	    DE+=T;
	  }
	
	
	if(n == nbDR -1) 
	  {
	    if (PrintCout == 2) cout << "NDR=" << nbDR << " -> %Eloss=" << (DE/fE)*100 << " %" << endl;
	    fX = dr_x;
	    fY = dr_y;
	    fZ = dr_z;
	    fE -= DE;
	    fT = time;
	    if (PrintCout == 1) cout << "nouveaux parametres electron apres delta rays :" << fX << " " << fY << " " << fZ << " " << fE << " " << fT << endl;
	  }
    }

  return;
}





//////////////////////////////////////
// ENERGY DEPOSITED IN SCINTILLATOR //
//////////////////////////////////////

bool ParticleInScint::EnergyLossInScint()
{
  if(PrintCout == 1) cout<<"EnergyLossInScint!"<<endl;
  double beta, gamma, eta, s, p, Ek, tau, F, Wmax, lgfactor;
  double FactorLandau, lgfactorLandau;
  double dx = GLength * 100. * density; // in g/cm²
  double fE_MeV = fE * 1E3;

  switch (abs(fId)) {
    
  case 1:
    return 1;
    
  case 2:
    // ElectronEnergyLoss : Loss of energy by electron in the scintillator in MeV (W.R. Leo p.35) :
    if(PrintCout == 1) cout << "Energy of electron before Lead, fE_MeV = fE * 1E3 = " << fE_MeV << " MeV" << endl;
    
    //////////////////////////////////
    // BETHE & BLOCH FOR ELECTRON : //
    //////////////////////////////////
    p = sqrt(fE_MeV*fE_MeV - me*me);
    beta = p/fE_MeV; 
    gamma = fE_MeV/me;
    eta = p/me;
    Ek = me * (gamma - 1.);
    tau = Ek/me;
    lgfactor = log( (tau*tau*(tau+2.)) / (2.*(I/me)*(I/me)) );
    F = 1. - beta*beta + ( tau*tau/8. - (2.*re+1.)*log(2.) ) / ( (tau+1.)*(tau+1.) );
    
    ElectronEnergyLoss = factor * Z/A * 1/(beta*beta) * (lgfactor + F - delta - 2.* C/Z) * dx; // Energy loss in MeV
    
    if(PrintCout == 1) cout <<"CAS 2 : fE_MeV = " << fE_MeV << " MeV, Beta = " << beta << ", Energy Loss = " << ElectronEnergyLoss << " MeV, GLength = " << GLength*100 << " cm" << endl;
  
    ///////////////////////////
    // LANDAU FOR ELECTRON : //
    ///////////////////////////
    // FactorLandau = (K/2) * (Z/A) * (dx/(beta*beta));
    // lgfactorLandau = log( (2*me* beta*beta * gamma*gamma)/I );
    // ElectronEnergyLoss = FactorLandau * (lgfactorLandau + log(FactorLandau/I) + j - beta*beta); 
   
    // if(PrintCout == 1) cout <<"CAS 2 (scint): fE_MeV = " << fE_MeV << ", p = " << p << ", beta = " << beta << ", gamma = " << gamma << ", GLength = " << GLength*100 << " cm, dx = " << dx << endl;
    // if(PrintCout == 1) cout << "lgfactor = " << lgfactorLandau << ", factor = " << FactorLandau << ", Eloss = " << ElectronEnergyLoss << " MeV" << endl;
    // if(PrintCout == 1) cout << FactorLandau << " * " << (lgfactorLandau + log(FactorLandau/I) + j - beta*beta)  << " = " << ElectronEnergyLoss << " soit : " << FactorLandau * (lgfactorLandau + log(FactorLandau/I) + j - beta*beta) << endl;

    if(ElectronEnergyLoss > fE_MeV)
      {
	if(PrintCout == 1) cout << "##### PB : Eloss > E ???!!! #####" << endl;
	return 0;
      }

    return 1;
    

  case 3:
    // MuonEnergyLossInScint : Loss of energy by muon in the scintillator in MeV
    if(PrintCout == 1) cout << "Energy of muon before Lead, fE_MeV = fE * 1E3 = " << fE_MeV << " MeV" << endl;
    //////////////////////////////
    // BETHE & BLOCH FOR MUON : //
    //////////////////////////////
    p = sqrt(fE_MeV*fE_MeV - M*M);
    gamma = fE_MeV/M;
    beta = p/fE_MeV; 
    eta = p/M;
    s  = me / M;
    Wmax = (2.*me*eta*eta)/(1. + 2.*gamma*s + s*s);
    lgfactor = log( (2.*me * eta*eta * Wmax) / (I*I));
    
    //Muon energy loss in GeV
    MuonEnergyLoss = factor * Z/A * (1/(beta*beta)) * (lgfactor - 2. *(beta*beta) - delta - 2.* C/Z) * dx; 
    
    ///////////////////////
    // LANDAU FOR MUON : //
    ///////////////////////
    // FactorLandau = (K/2) * (Z_Pb/A_Pb) * (dx/(beta*beta));
    // lgfactorLandau = log( (2*M* beta*beta * gamma*gamma)/I_Pb );       
    // MuonEnergyLoss = FactorLandau * (lgfactorLandau + log(FactorLandau/I_Pb) + j - beta*beta);
    
    if(PrintCout == 1) cout <<"CAS 3 (scint): fE_MeV = " << fE_MeV << " MeV, p = " << p << ", Beta = " << beta << ", GLength = " << GLength*100 << " cm, FactorLandau = " << FactorLandau << ",lgfactorLandau = " << lgfactorLandau << ", Energy Loss = " << MuonEnergyLoss << " MeV" << endl;
    
    if(MuonEnergyLoss > fE_MeV)
      {
	if(PrintCout == 1) cout << "##### PB : Eloss > E ???!!! #####" << endl;
	return 0;
      }

    return 1;

    
  default:
    cerr << "Error in ParticleInScint::EnergyLossInScint..." << endl;    
  }  
}



  /*--------------------------------------------------------
  -  Compute photons produced function the energy lost     -
  -  in the scintillator.                                  -
  -  1 particle produce 8000 photons per MeV deposited !!  -
  -  Light production of about 40% & 2*1E4 hv/MeV deposed  - 
  -     ---->  so, we have 8000 hv/MeV.                    - 
  ------------------------------------------------------- */

void ParticleInScint::GeneratePhoton()
{
  if(PrintCout == 1)  cout<<"GeneratePhoton!"<<endl;

  int partinfE;
  int partsupE;
  int partsupM;
  int partinfM;
  int ipm;
  double fNPhot_temp;
  // TRandom r; 
  // int seed = time(NULL);
  // r.SetSeed(seed);
 
  switch (abs(fId)) {
    
  case 1:  
    break;
    
  case 2:    
    fNPhot_temp = (int) Rand ("LANDAU", 25., 4.28);
    ////////fNPhot_temp = r.Landau(25.,4.28); // VERSION LANDAU RANDOM 
    ///cout << "##el## " << fNPhot_temp<< endl;
    //fNPhot_temp = ElectronEnergyLoss * Ener2Phot* FiberPurcent * QE_PMT; // VERSION IDEAL 
    //fNPhot_temp = ElectronEnergyLoss * Ener2Phot; // VERSION A MOI
    partinfE = floor(fNPhot_temp);
    partsupE = partinfE + 1;
    if(fNPhot_temp-partinfE < partsupE-fNPhot_temp)
      {
	fNPhot += partinfE;
	ipm = 1;
	for(int Nph = 0; Nph < fNPhot; Nph++) {
	  fTPhot.push_back(fT + GLength/CLIGHT + FiberLength/CLIGHT);
	  fIpm.push_back(ipm);
	}
      }
    else if(fNPhot_temp-partinfE > partsupE-fNPhot_temp)
      {
	fNPhot += partsupE;
	ipm = 1;
	for(int Nph = 0; Nph < fNPhot; Nph++) {
	  fTPhot.push_back (fT + GLength/CLIGHT + FiberLength/CLIGHT);
	  fIpm.push_back(ipm);
	}
      }
    //if(PrintCout == 1) cout << "CAS ELECTRON : fNPhot = " << fNPhot << ", fTPhot = " << fTPhot << endl;
    break;
    
  case 3:
    fNPhot_temp = (int) Rand ("LANDAU", 25., 4.28);
    /////////fNPhot_temp = r.Landau(25.,4.28); //VERSION LANDAU RANDOM 
    ////cout << "##mu## " << fNPhot_temp<< endl;
    //fNPhot_temp = MuonEnergyLoss * Ener2Phot; // VERSION A MOI
    //fNPhot_temp = MuonEnergyLoss * Ener2Phot* FiberPurcent * QE_PMT; // VERSION IDEAL
    partinfM = floor(fNPhot_temp);
    partsupM = partinfM + 1;
    
    if(fNPhot_temp-partinfM < partsupM-fNPhot_temp)
      {
	fNPhot += partinfM;
	ipm = 1;
	for(int Nph = 0; Nph < fNPhot; Nph++) {
	  fTPhot.push_back(fT + GLength/CLIGHT  + FiberLength/CLIGHT);
	  fIpm.push_back(ipm);
	}
      }
    else if(fNPhot_temp-partinfM > partsupM-fNPhot_temp)
      {
	fNPhot += partsupM;
	ipm = 1;
	for(int Nph = 0; Nph < fNPhot; Nph++) {
	  fTPhot.push_back(fT + GLength/CLIGHT  + FiberLength/CLIGHT);
	  fIpm.push_back(ipm);
	}
      }
    //if(PrintCout == 1)  cout << "CAS MUON : fNPhot = " << fNPhot << ", fTPhot = " << fTPhot << endl;
    break;
    
  default:
    cerr << "Error in ParticleInScint::GeneratePhoton..." << endl;                      
  }  
  
}














	
	////////// TEST RENFORCEMENT DELTA RAYS :
	// if( nbDR == 1 && n == 0)
	//   {
	// 	if (PrintCout == 2) cout << "NDR=" << nbDR << " -> %Eloss=" << (DE/fE)*100 << " %" << endl;
	// 	fX = dr_x;
	// 	fY = dr_y;
	// 	fZ = dr_z;
	// 	fE -= DE;
	// 	fT = time;
	// 	if (PrintCout == 1) cout << "nouveaux parametres electron apres delta rays :" << fX << " " << fY << " " << fZ << " " << fE << " " << fT << endl;
	//   }
	
	// else if( nbDR != 1 && nbDR !=0 && n == 1)
	//   {
	// 	if (PrintCout == 2) cout << "NDR=" << nbDR << " -> %Eloss=" << (DE/fE)*100 << " %" << endl;
	// 	fX = dr_x;
	// 	fY = dr_y;
	// 	fZ = dr_z;
	// 	fE -= DE;
	// 	fT = time;
	// 	if (PrintCout == 1) cout << "nouveaux parametres electron apres delta rays :" << fX << " " << fY << " " << fZ << " " << fE << " " << fT << endl;
	// 	continue;
	//   }
