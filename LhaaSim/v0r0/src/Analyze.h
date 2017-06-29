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

  void DrawEvent(int nev); // Trace réseau ED & MD ->Pb Core
  void DrawStationPe(int nev);
  void DrawOneFE(int num,int ipm);
  void DrawOneFE(int num,int ipm,int ichan);         
  void DrawOnePM(int num,int ipm);       
  void DrawOnePM(int num,int ipm,int ichan);      
  void DrawProfilesED(); // Particules = f(distance_gerbe)
  void DrawProfilesMD(); // Particules = f(distance_gerbe)
  void DrawOneProfileMD(int num);
  void DrawOneProfileED(int num);          
  void DrawOneProfile(int num,int ipm);          
  void ParticlesDistanceTime(); //Profils LDF hypoth. temps
  void DrawOneTrace(int num); 
  void DrawOneTrace(int num,int ipm);
  void DrawOneTrace(int num,int ipm,int ichan);
  void DrawOneTraceInVEM(int num,int ipm,int ichan);
  void DrawTraces();    //Draw sum of PMTS
  void DrawTraces(int ipm);
  void DrawTracesInVem();                   //Draw sum of PMTS
  void DrawTracesInVem(int ipm);
  void GetMD(); // Affichage détaillé cuve, particule, pe,..
  void GetED(); // Affichage détaillé cuve, particule, pe,..
  void GetMDED(); // Affichages succints pour ED & MD 
  void PrintTrees(); // Affiche infos sur les 3 arbres 
  void ReadFile(char * file ); // Lit le fichier
  void TestAnodeDynode();
  void TestNewTOT();
 
  /*
 
  void DrawEvent();
  void Help();
  */
 private:
 

  ClassDef(Analyze,1)
    };
#endif
    







