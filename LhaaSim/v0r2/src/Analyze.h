#ifndef _ANALYZE_H
#define _ANALYZE_H

#include "TObject.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TVector3.h"
#include "TTree.h"
#include "TFile.h"
#include "TBrowser.h"
#include "Event.h"
#include "Array.h"
#include "TBranch.h"
#include <vector>
#include <map>
#include "TChain.h"


class Analyze : public TObject{
 private:
  TList *fCanvasList;
  TList *fHisto1;
  TList *fHistTitle;
  TList *fHistAxis;
  TCanvas* fCanvas;
  TFile* fFile;
  TTree * fTreeEvent;
  TTree * fTreeMD;
  TTree * fTreeED;
  TChain* fChain;
  Event* fEvent;
  TBranch* fBranchEvent;
  TBranch* fBranchED;
  TBranch* fBranchMD;
  HitStationMD* fHitStationMD;
  vector<HitStationMD> fStListMD;
  HitStationED* fHitStationED;
  vector<HitStationED> fStListED;
  
  
 public:
  Analyze();
  ~Analyze();
  
  // Fonctions utilisables pour 1 fichier après ReadFile() :
  void ReadFileChoice(char * ener);
  void ReadFileChoiceDebug(char * ener);
  void ReadFile(char * file ); // Lecture fichier
  void PrintTrees(); // Affiche infos sur les 3 arbres
  void GetMD(); // Affichage détaillé cuve, particule, pe,..
  void GetED(); // Affichage détaillé cuve, particule, pe,..
  void GetMDED(); // Affichage succint pour ED & MD 
  void DrawMuons(int opt); 
  void DrawEvent(int nev); // Trace réseau ED & MD ->Pb Core
  void DrawStationPe(int nev);
  void DrawOnePM(int num,int ipm);       
  void DrawProfilesED(); // Particules = f(distance_gerbe)
  void DrawProfilesMD(); // Particules = f(distance_gerbe)
  void DrawOneProfileMD(int num);
  void DrawOneProfileED(int num);          
  void ParticlesDistanceTime(); //Profils LDF hypoth. temps
  void DeleteFile();
  
  // Sélection fichiers + triggers + trace efficacité : 
  //  int GetListFileSelectEnergy();
  int GetListGoodFile(); // -> ListGoodFile.txt 
  int Efficiency(); // -> ListEfficiency.txt : E th mul_mu mul_VEM
  void ComputeEfficiency(); // -> efficiency.txt : E th mul_mean 
  void DrawEfficiency();  // -> Nb cuves trigger ED / MD (sig>3VEM, Nmu ...)
  void DrawOneEfficiency(); // 
  void DrawMulMDvsED(int it_en); // Nb cuves trigger MD f=(Nb scint trigger ED)
  // Draw Q ::
  void DrawQ(int it_en, int mode);  // en=[3,9] , mode=1 ->Sig & mode=2 -> Nmu
  void DrawTotQ(int mode); // mode = 1 -> SumSig  ou  mode = 2 -> Nmu
  int GetListMDED(); // Liste evt (Sum_signal , Sum_Npe)
  // Draw LDF ::
  pair <float,float> DrawLDF_MD();  // float -> pair <float,float>
  pair <float,float> DrawLDF_ED();
  

    
  double GetMDChargeInVEM(int num);
  int TrigInMDCharge(); // trigger MD [VEM]
  int TrigInMuons(); // trigger MD [Nmu]
  int TrigED(); // trigger ED [Npe]
  //// TrigForQ /////
  int TrigEDforQ();  // somme Npe -réseau ED- pour chaque gerbe !!
  float TrigMDforQ(); // somme sig -réseau MD- pour chaque gerbe !
  float TrigMDMuforQ(); // somme Nmu -réseau MD- pour chaque gerbe !
  
  // Analyse à partir du fichier Root AnaEvent.cc : 
  // N.B. : Changer le trigger dans Station.cc !!
  void TestEfficiencyMu(int th);
  void TestEfficiencyVEM(float th);
  void TestEfficiencyED();
  void NpeED();
 
  /*
    void DrawEvent();
    void Help();
  */
 private:
  
  
  ClassDef(Analyze,1)
    };
#endif








