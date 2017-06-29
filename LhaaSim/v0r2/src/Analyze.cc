#include "Analyze.h"
#include "Constants.h"
#include "Trigger.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <string.h>
#include <map>
#include "TFile.h"
#include "TH2.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "TEllipse.h"
#include "TText.h"
#include "TLine.h"
#include "TLatex.h"
#include "TMarker.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TArrayD.h"
#include "TF1.h"
#include "TF2.h"
#include "TBRIK.h"
#include "TTUBE.h"
#include "TTUBS.h"
#include "TROOT.h"
#include "TNtuple.h"
#include "TNode.h"
#include "TRotMatrix.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TString.h"
#include "TGraph2D.h"

using namespace std;

ClassImp(Analyze)


  Analyze::Analyze()
{
  fCanvasList = new TList();
  fHisto1 = new TList();
  fHistTitle = new TList();
  fHistAxis = new TList(); 
 
  fTreeEvent = 0;
  fTreeMD = 0;
  fTreeED = 0;
  fEvent = 0;
  fBranchEvent = 0 ;
  fBranchMD = 0 ;
  fBranchED = 0 ;
  fFile = 0;

 }

Analyze::~Analyze()
{ 
  fHistAxis->Delete();
  fHistTitle->Delete();
  fHisto1->Delete();
  fCanvasList->Delete();
  
  
  delete fHistAxis;
  delete fHistTitle;
  delete fHisto1;
  delete fCanvasList;
}




void Analyze::DrawMuons(int opt)
{
  ofstream out;
  out.open("resultfit.out",ios::out);
  map<int,double> trace;
  map<int,double> pmtrace;
  map<int,double> pmtracetot;

  int nmu;
  double npesum,npemax;
  HitStationMD* staMD;

  TH1F* hadcch;
  TH1F* hpech;
  TH1F* hpmch;

  gStyle->SetOptStat(1001111); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);
  gStyle->SetLineColor(kRed);     
  
  char* hname1 = new char[15];
  sprintf(hname1,"ADCcharge_%1d",1);
  hadcch =  new TH1F(hname1,hname1,1000,0.,1000);
  delete hname1;
  char* hname2 = new char[15];
  sprintf(hname2,"PEcharge_%1d",1);
  hpech =  new TH1F(hname2,hname2,200,0.,200);
  delete hname2;
  char* hname3 = new char[15];
  sprintf(hname3,"pmcharge_%1d",1);
  hpmch =  new TH1F(hname3,hname3,100,0.,20);    
  delete hname3;

  fTreeMD = (TTree *) fFile->Get("TreeMD");      // Get Tree
  fTreeMD->Print();

  nmu=(int)fTreeMD->GetEntries();
  cout << nmu << " muons to analyze" << endl;
  
  for(int ne=0; ne<nmu; ne++)
    {
      fTreeMD->GetEvent(ne);
      
      // Charge with ADC signal  
      npesum=0;  
      for(int k=0; k<400; k++){
	if(fHitStationMD->fPMT[0].fADC[0][k] !=0 ) {
	  cout << "ici " << endl;
	  npesum+=fHitStationMD->fPMT[0].fADC[0][k];
	}
      }// end of loop on k bins
      if(npesum>0) hadcch->Fill(npesum);
      
      cout << "oups " << endl;
      // Charge with number of PE   
      npesum=0;
      trace =fHitStationMD->fPMT[0].fTimeProfile;
      for(int k=0; k<10000; k++){
	if(trace.count(k))
	  npesum+=trace[k];  
      }
      hpech->Fill(npesum);
      
      
      // Charge with PM signal   
      npesum=0;
      pmtrace =fHitStationMD->fPMT[0].fPMTSignal_hi;
      for(int k=0; k<10000; k++){
	if(pmtrace.count(k))
	  npesum+=pmtrace[k];    
      }	  
      hpmch->Fill(npesum);   
    }
  
  
  fCanvasList->Add(new TCanvas("charges","Charges",1000,1000));  
  fCanvas=(TCanvas *) fCanvasList->Last();
  fCanvas->Divide(3,1);
  fCanvas->cd(1);
  hpech->SetLineColor(kRed);
  hpech->Draw();
  fCanvas->cd(2);
  hpmch->SetLineColor(kRed);
  hpmch->Draw();
  fCanvas->cd(3);
  hadcch->SetLineColor(kRed);
  hadcch->Draw();
}






void Analyze::DrawStationPe(int nev)
{
  TH2F* TankNbPeMD;
  TH2F* TankNbPeED;
  
  double latestime = 9999999;
  double xmin,ymin,xmax,ymax;
  xmin = 99999999;
  ymin = 99999999;
  xmax = 0;
  ymax = 0;
  

  // MD détecteur
  fTreeMD = (TTree *) fFile->Get("TreeMD");      // Get Tree
  fTreeMD->Print(); 

  fTreeMD->GetEntry(nev);
  int nombtank = fTreeMD->GetEntries();
  cout << "nombre de tank = " << nombtank << endl;
  for(int i=0;i<nombtank;i++){
    fTreeMD->GetEvent(i);
    if(fHitStationMD->fNorthing > ymax) ymax = fHitStationMD->fNorthing;
    if(fHitStationMD->fNorthing < ymin) ymin = fHitStationMD->fNorthing; 
    if(fHitStationMD->fEasting > xmax) xmax = fHitStationMD->fEasting; 
    if(fHitStationMD->fEasting < xmin) xmin = fHitStationMD->fEasting;
    if(fHitStationMD->fTime0<latestime) latestime=fHitStationMD->fTime0;
  }
  xmin-= 20;  
  ymin-= 20;
  xmax+= 40;
  ymax+= 30;
 
  TankNbPeMD = new TH2F("TankNbPe","Tank function number of pe (MD)",(int)(xmax-xmin),xmin,xmax,(int)(xmax-xmin),ymin,ymax);

  // Draw parameters MD :
  for(int i=0;i<nombtank;i++) {
    fTreeMD->GetEvent(i);
    TankNbPeMD->Fill(fHitStationMD->fEasting, fHitStationMD->fNorthing,fHitStationMD->fNpe);
    TankNbPeMD->SetMarkerStyle(24);
    TankNbPeMD->SetMarkerSize(1.5);
    TankNbPeMD->Draw("zcol");
  }
  



  // ED détecteur
  fTreeED = (TTree *) fFile->Get("TreeED");      // Get Tree
  fTreeED->Print(); 

  fTreeED->GetEntry(nev);
  int nombscint = fTreeED->GetEntries();
  cout << "nombre de scint = " << nombscint << endl;
  for(int i=0;i<nombscint;i++){
    fTreeED->GetEvent(i);
    if(fHitStationED->fNorthing > ymax) ymax = fHitStationED->fNorthing;
    if(fHitStationED->fNorthing < ymin) ymin = fHitStationED->fNorthing; 
    if(fHitStationED->fEasting > xmax) xmax = fHitStationED->fEasting; 
    if(fHitStationED->fEasting < xmin) xmin = fHitStationED->fEasting;
    if(fHitStationED->fTime0<latestime) latestime=fHitStationED->fTime0;
  }
  xmin-= 20;  
  ymin-= 20;
  xmax+= 40;
  ymax+= 30;
 
  TankNbPeED = new TH2F("TankNbPeED","Tank function number of pe (ED)",(int)(xmax-xmin),xmin,xmax,(int)(xmax-xmin),ymin,ymax);

  // Draw parameters ED :
  for(int i=0;i<nombscint;i++) {
    fTreeED->GetEvent(i);
    TankNbPeED->Fill(fHitStationED->fEasting, fHitStationED->fNorthing,fHitStationED->fNpe);
    TankNbPeED->SetMarkerStyle(24);
    TankNbPeED->SetMarkerSize(1.5);
    TankNbPeED->Draw("zcol");

  }
}





void Analyze::DrawEvent(int nev)
{
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);
  gStyle->SetLineColor(kRed);
  gStyle->SetPalette(1);
  TH2F* hcores;
  
  // HitStationMD* staMD;
  // vector<HitStationMD> fStListMD;
  // HitStationED* staED;
  // vector<HitStationED> fStListED;
  double xmin,ymin,xmax,ymax;
  
  xmin = 99999999;
  ymin = 99999999;
  xmax = 0;
  ymax = 0;
  
  fTreeMD = (TTree *) fFile->Get("TreeMD");      // Get Tree
  fTreeMD->Print(); 
  
  cout << " \t * There is " << fTreeMD->GetEntries() << " tanks in the input file "<< endl;
  
  // MD detector :
  fTreeMD->GetEntry(nev);
  int nombtank = fTreeMD->GetEntries();
  TText *stidMD[nombtank];
  double latestime = 9999999;
  cout << "nombre de tank = " << nombtank << endl;
  for (int i=0; i <= nombtank; i++) {
    fTreeMD->GetEvent(i);
    if(fHitStationMD->fNorthing > ymax) ymax = fHitStationMD->fNorthing;
    if(fHitStationMD->fNorthing < ymin) ymin = fHitStationMD->fNorthing; 
    if(fHitStationMD->fEasting > xmax) xmax = fHitStationMD->fEasting; 
    if(fHitStationMD->fEasting < xmin) xmin = fHitStationMD->fEasting;
    char* nameMD = new char[3];
    sprintf(nameMD,"%3d",fHitStationMD->fId);
    stidMD[i] = new TText(fHitStationMD->fEasting+5,fHitStationMD->fNorthing+5,nameMD);
    stidMD[i]->SetTextSize(0.02);
    delete nameMD;
    if(fHitStationMD->fTime0<latestime)latestime = fHitStationMD->fTime0;
  }


  fTreeED = (TTree *) fFile->Get("TreeED");      // Get Tree
  fTreeED->Print(); 
  
  cout << " \t * There is " << fTreeED->GetEntries() << " scints in the input file "<< endl;
  
  // ED detector :
  fTreeED->GetEntry(nev);
  int nombscint = fTreeED->GetEntries();
  TText *stidED[nombscint];
  double latestimeED = 9999999;
  double NpeMax = 0.;
  cout << "nombre de scint = " << nombscint << endl;
  for(int i=0;i<nombscint;i++){
    fTreeED->GetEvent(i);
    if(fHitStationED->fNorthing > ymax) ymax = fHitStationED->fNorthing;
    if(fHitStationED->fNorthing < ymin) ymin = fHitStationED->fNorthing; 
    if(fHitStationED->fEasting > xmax) xmax = fHitStationED->fEasting; 
    if(fHitStationED->fEasting < xmin) xmin = fHitStationED->fEasting;
    char* nameED = new char[3];
    sprintf(nameED,"%3d",fHitStationED->fId);
    stidED[i] = new TText(fHitStationED->fEasting+5,fHitStationED->fNorthing+5,nameED);
    stidED[i]->SetTextSize(0.02);
    delete nameED;
    if(fHitStationED->fTime0<latestimeED)latestimeED=fHitStationED->fTime0;
    // En fonction Npe : 
    if(fHitStationED->fNpe > NpeMax) NpeMax = fHitStationED->fNpe; 
  }
  xmin-= 20;  
  ymin-= 20;
  xmax+= 40;
  ymax+= 30;
  
  cout << xmin << " " << xmax << " " << ymin << " " << ymax << endl;
  hcores = new TH2F("Cores","Cores",(int)(xmax-xmin),xmin,xmax,(int)(xmax-xmin),ymin,ymax);
  

  fTreeEvent = (TTree *) fFile->Get("TreeEvent");   
  fTreeEvent->SetBranchAddress("event",&fEvent);
  fTreeEvent->GetEntry(nev);

  hcores->Fill(fEvent->fEasCore,fEvent->fNorCore);
  cout << "################################################ \n Position du coeur (Est,Nord) = (" << fEvent->fEasCore << "," << fEvent->fNorCore << ") \n################################################" << endl;
  
  fCanvasList->Add(new TCanvas("array"," array ",600,600));
  
  fCanvas = (TCanvas *) fCanvasList->Last();
  TStyle palette;
  palette.SetPalette(1);
  palette.Draw();

  // Draw parameters Core :
  hcores->SetMarkerStyle(29);
  hcores->SetMarkerSize(1.5);
  hcores->SetMarkerColor(kRed);
  hcores->Draw();

  // Draw parameters MD :
  for(int i=0;i<nombtank;i++) {
    fTreeMD->GetEvent(i);
    double size = 0.5 + log(fHitStationMD->fNpe)/5.;
    double time = fHitStationMD->fTime0;
    TMarker *ptMD = new TMarker (fHitStationMD->fEasting, fHitStationMD->fNorthing, 20);
    ptMD->SetMarkerStyle(24);
    ptMD->SetMarkerSize(size);
    //ptMD->SetMarkerColor(fHitStationMD->fNpe);   
    if(fHitStationMD->fNpe > 0)
      ptMD->SetMarkerColor(50 - (int)((time-latestime)/latestime*25));
    else 
      ptMD->SetMarkerColor(0);
     ptMD->Draw("z");
     //stidMD[i]->Draw();
  }

  
  // Draw parameters ED :
  for(int i=0;i<nombscint;i++){
    fTreeED->GetEvent(i);
    double size = 0.5;
    double time = fHitStationED->fTime0;
    TMarker *ptED = new TMarker (fHitStationED->fEasting, fHitStationED->fNorthing, 20);
    ptED->SetMarkerStyle(21);
    ptED->SetMarkerSize(size);
    if(fHitStationED->fNpe > 0)
      ptED->SetMarkerColor(50 - (int)((time-latestimeED)/latestimeED*25));
    else 
      ptED->SetMarkerColor(0);
    //ptED->SetMarkerColor(50 - (int)((fHitStationED->fNpe - NpeMax)/NpeMax));
    cout << "diff = " << (50 - (int)((fHitStationED->fNpe - NpeMax)/NpeMax)) << " -> NpeMax = " << NpeMax << " -> Npe = " << fHitStationED->fNpe << endl; 
    ptED->Draw("z");
    // stidED[i]->Draw();
  } 
}




void Analyze::DrawProfilesMD()
{  
  int nevent,nmu,nel,nph,nPeMu;
  double dist, sig;

  // TH1F *hLDF_Nmu = new TH1F("hLDF_Nmu","LDF muon profile (MD detector); Distance to shower axis (m); Number of muons",300,0,300);
  // TH1F *hLDF_Nel = new TH1F("hLDF_Nel","LDF electron profile (MD detector); Distance to shower axis (m); Number of electrons",300,0,300);
  // TH1F *hLDF_Nph = new TH1F("hLDF_Nph","LDF gamma profile (MD detector); Distance to shower axis (m); Number of gammas",300,0,300);
  // TH1F *hLDF_Npe_mu = new TH1F("hLDF_Npe_mu","Number of photoelectrons produced by muons function distance to shower axis; Distance to shower axis (m); Number of photoelectrons",300,0,300);
  // Signal = f(distance) :
  //TH1F *hLDF_Sig = new TH1F("hLDF_Sig","Signal function distance to shower axis; Distance to shower axis [m]; Signal [VEM]",3000,0,3000);
  TH1F *hLDF_Sig = new TH1F("hLDF_Sig","LDF MD; Distance to shower axis [m]; Signal [VEM]",700,0,700);
  TF1 *fit2 = new TF1("fit2","[0]*pow( ((x/15.) * (1+(x/15.))), [1])",8,700);
 
  nevent = (int)fTreeMD->GetEntries();
  
  for(int ne=0; ne<nevent; ne++)
    {
      fTreeMD->GetEvent(ne);
      // commenter la ligne dessous si pas sig = f(dist) :
      //     if(GetMDChargeInVEM(ne) < 2)  continue;
      sig = GetMDChargeInVEM(ne);
      // nmu = fHitStationMD->fNmu;
      // nel = fHitStationMD->fNel;
      // nph = fHitStationMD->fNph;
      // nPeMu = fHitStationMD->fNpe_mu;
      dist = fHitStationMD->fR_sf;
      //cout << "nevt = " << nevent << " : charge = " << sig << " : dist = " << dist << endl; 
      // if(nmu > 0)
      // 	cout << nmu << "\t muons at " << dist << " m, NPe = " << nPeMu << endl;
      // hLDF_Nmu->Fill(dist,nmu);
      // hLDF_Nel->Fill(dist,nel);
      // hLDF_Nph->Fill(dist,nph);
      // hLDF_Npe_mu->Fill(dist,nPeMu);
      hLDF_Sig->Fill(dist,sig);
      
    }//end of loop en event number
  
  TCanvas* c1 = new TCanvas;
  //c1->Divide(3,1);
  //c1->Divide(2,1);
  c1->cd(1);
  hLDF_Sig->SetMarkerStyle(34);  
  hLDF_Sig->SetMarkerColor(46); 
  fit2->SetMarkerColor(2);
  hLDF_Sig->Draw("point");
 
  fit2->SetParameters(0,5); fit2->SetParameters(1,-2);
  hLDF_Sig->Fit("fit2");
  fit2->Draw("same");
  
  // c1->cd(2);
  //  hLDF_Nmu->SetMarkerStyle(34);
  //  hLDF_Nmu->SetMarkerColor(38);
  //  hLDF_Nmu->Draw("point");
  // c1->cd(2);
  // hLDF_Npe_mu->SetMarkerStyle(34);
  // hLDF_Npe_mu->SetMarkerColor(46);
  // hLDF_Npe_mu->Draw("point");
  //c1->cd(2);
  //hLDF_Nel->SetMarkerStyle(34);
  //hLDF_Nel->SetMarkerColor(38);
  //hLDF_Nel->Draw("point");
  //c1->cd(3);
  //hLDF_Nph->SetMarkerStyle(34);
  //hLDF_Nph->SetMarkerColor(46);
  //hLDF_Nph->Draw("point");
  
  //TCanvas* c2 = new TCanvas;
  //c2->cd(1);
  //hLDF_Npe_mu->SetMarkerStyle(34);
  //hLDF_Npe_mu->SetMarkerColor(17);
  //hLDF_Npe_mu->Draw("point");
}




void Analyze::DrawProfilesED()
{
  int nevent,nmu,nel,nph,nPeMu,nPePh,nPeEl, nPe;
  double dist;

  // TH1F *hLDF_Nmu = new TH1F("hLDF_Nmu","LDF muon profile of proton shower (ED detector); Distance to shower axis (m); Number of muons;",300,0,300);
  // TH1F *hLDF_Nel = new TH1F("hLDF_Nel","LDF electron profile of proton shower (ED detector); Distance to shower axis (m); Number of electrons",300,0,300);
  // TH1F *hLDF_Nph = new TH1F("hLDF_Nph","LDF gamma profile of proton shower (ED detector); Distance to shower axis (m); Number of gammas",300,0,300);
  // TH1F *hLDF_Npe_mu = new TH1F("hLDF_Npe_mu","Number of photoelectrons produced by muons function distance to shower axis; Distance to shower axis (m); Number of photoelectrons (µ)",300,0,300);
  // TH1F *hLDF_Npe_ph = new TH1F("hLDF_Npe_ph","Number of photoelectrons produced by photons function distance to shower axis; Distance to shower axis (m); Number of photoelectrons",300,0,300);
  // TH1F *hLDF_Npe_el = new TH1F("hLDF_Npe_el","Number of photoelectrons produced by electrons function distance to shower axis; Distance to shower axis (m); Number of photoelectrons (e-)",300,0,300);
    
  TH1F *hLDF_Npe = new TH1F("hLDF_Npe","Npe function distance to shower axis; Distance to shower axis [m]; Npe",4,0,496);
  TF1 *fit = new TF1("fit","[0]*pow( ((x/8.) * (1+(x/8.))), [1])",4,500);
 
  nevent=(int)fTreeED->GetEntries();
  
  for(int ne=0; ne<nevent; ne++)
    {
      fTreeED->GetEvent(ne);
      nmu = fHitStationED->fNmu;
      nel = fHitStationED->fNel;
      nph = fHitStationED->fNph;
      nPeMu = fHitStationED->fNpe_mu;
      nPePh = fHitStationED->fNpe_ph;
      nPeEl = fHitStationED->fNpe_el;
      nPe = fHitStationED->fNpe;
      dist = fHitStationED->fR_sf;
      //  cout << nmu << "\t muons at " << dist << " m" << endl;
      // hLDF_Nmu->Fill(dist,nmu);
      // hLDF_Nel->Fill(dist,nel);
      // hLDF_Nph->Fill(dist,nph);
      // hLDF_Npe_mu->Fill(dist,nPeMu);
      // hLDF_Npe_ph->Fill(dist,nPePh);
      // hLDF_Npe_el->Fill(dist,nPeEl);
      if(/*dist >= 8  &*/ nPe !=0) {
	hLDF_Npe->Fill(dist,nPe);
	cout << dist << " " << nPe << endl; }
    }//end of loop en event number
  
  TCanvas* c1 = new TCanvas;
  c1->cd(1);
  hLDF_Npe->SetMarkerStyle(34);
  hLDF_Npe->SetMarkerColor(38);
  fit->SetMarkerColor(2);
  hLDF_Npe->Draw("point");
  
  fit->SetParameters(0,5); fit->SetParameters(1,-2);
  
  hLDF_Npe->Fit("fit");
  fit->Draw("same");
  
  // c1->Divide(3,1);
  // c1->cd(1);
  // hLDF_Nmu->SetMarkerStyle(34);
  // hLDF_Nmu->SetMarkerColor(17);
  // hLDF_Nmu->Draw("point");
  // c1->cd(2);
  // hLDF_Nel->SetMarkerStyle(34);
  // hLDF_Nel->SetMarkerColor(38);
  // hLDF_Nel->Draw("point");
  // c1->cd(3);
  // hLDF_Nph->SetMarkerStyle(34);
  // hLDF_Nph->SetMarkerColor(46);
  // hLDF_Nph->Draw("point");

  // TCanvas* c2 = new TCanvas;
  // c2->Divide(3,1);

  // c2->cd(1);
  // hLDF_Npe_mu->SetMarkerStyle(34);
  // hLDF_Npe_mu->SetMarkerColor(17);
  // hLDF_Npe_mu->Draw("point");
  // c2->cd(2);
  // hLDF_Npe_ph->SetMarkerStyle(34);
  // hLDF_Npe_ph->SetMarkerColor(38);
  // hLDF_Npe_ph->Draw("point");
  // c2->cd(3);
  // hLDF_Npe_el->SetMarkerStyle(34);
  // hLDF_Npe_el->SetMarkerColor(46);
  // hLDF_Npe_el->Draw("point");
}



void Analyze::ParticlesDistanceTime()
{
  int neventED, neventMD, nmu_neg, nel_neg, nph_neg, nmu_pos, nel_pos, nph_pos, nPeMu, nPePh, nPeEl;
  double dist, distMD, distED;

  // Histos fonction temps + ou -
  TH1F *hLDF_Nmu_pos = new TH1F("hLDF_Nmu_pos","LDF muon profile of proton shower (MD detector); Distance to shower axis (m); Number of muons;",300,0,300);
  TH1F *hLDF_Nmu_neg = new TH1F("hLDF_Nmu_neg","LDF muon profile of proton shower (MD detector); Distance to shower axis (m); Number of muons;",300,0,300);
  TH1F *hLDF_Nel_pos = new TH1F("hLDF_Nel_pos","LDF electron profile of proton shower (MD detector); Distance to shower axis (m); Number of electrons",300,0,300);
  TH1F *hLDF_Nel_neg = new TH1F("hLDF_Nel_neg","LDF electron profile of proton shower (MD detector); Distance to shower axis (m); Number of electrons",300,0,300);
  TH1F *hLDF_Nph_pos = new TH1F("hLDF_Nph_pos","LDF gamma profile of proton shower (MD detector); Distance to shower axis (m); Number of gammas",300,0,300);
  TH1F *hLDF_Nph_neg = new TH1F("hLDF_Nph_neg","LDF gamma profile of proton shower (MD detector); Distance to shower axis (m); Number of gammas",300,0,300);
  
  // Histos ED/MD
 TH1F *hLDF_Nmu_MD = new TH1F("hLDF_Nmu_MD","LDF muon profile of proton shower (MD detector); Distance to shower axis (m); Number of muons;",400,0,800);
  TH1F *hLDF_Nmu_ED = new TH1F("hLDF_Nmu_ED","LDF muon profile of proton shower (ED detector); Distance to shower axis (m); Number of muons;",400,0,800);
  TH1F *hLDF_Nel_MD = new TH1F("hLDF_Nel_MD","LDF electron profile of proton shower (MD detector); Distance to shower axis (m); Number of electrons",400,0,800);
  TH1F *hLDF_Nel_ED = new TH1F("hLDF_Nel_ED","LDF electron profile of proton shower (ED detector); Distance to shower axis (m); Number of electrons",400,0,800);
  TH1F *hLDF_Nph_MD = new TH1F("hLDF_Nph_MD","LDF gamma profile of proton shower (MD detector); Distance to shower axis (m); Number of gammas",400,0,800);
  TH1F *hLDF_Nph_ED = new TH1F("hLDF_Nph_ED","LDF gamma profile of proton shower (ED detector); Distance to shower axis (m); Number of gammas",400,0,800);
  
 
  neventMD = (int)fTreeMD->GetEntries();
  for(int ne=0; ne<neventMD; ne++)
    {
      fTreeMD->GetEvent(ne);
      // Histos temps positif/négatif
      if(fHitStationMD->fTime0 < 0) {
	nmu_neg = fHitStationMD->fNmu;
	nel_neg = fHitStationMD->fNel;
	nph_neg = fHitStationMD->fNph; }
      else {
	nmu_pos = fHitStationMD->fNmu;
	nel_pos = fHitStationMD->fNel;
	nph_pos = fHitStationMD->fNph; }
      dist = fHitStationMD->fR_sf;
      //cout<<nmu<<"\t muons at "<<dist<<" m"<<endl;
      hLDF_Nmu_neg->Fill(dist,nmu_neg);
      hLDF_Nel_neg->Fill(dist,nel_neg);
      hLDF_Nph_neg->Fill(dist,nph_neg);
      hLDF_Nmu_pos->Fill(dist,nmu_pos);
      hLDF_Nel_pos->Fill(dist,nel_pos);
      hLDF_Nph_pos->Fill(dist,nph_pos);

      // Histos Nb µ,e-,hv fonction distance pour MD:
      distMD = fHitStationMD->fR_sf;
      hLDF_Nmu_MD->Fill(distMD,fHitStationMD->fNmu);
      hLDF_Nel_MD->Fill(distMD,fHitStationMD->fNel);
      hLDF_Nph_MD->Fill(distMD,fHitStationMD->fNph);
     }

  neventED = (int)fTreeED->GetEntries();
  for(int ne=0; ne<neventED; ne++)
    {
      fTreeED->GetEvent(ne);
      // Histos Nb µ,e-,hv fonction distance pour MD:
      distED = fHitStationED->fR_sf;
      hLDF_Nmu_ED->Fill(distED,fHitStationED->fNmu);
      hLDF_Nel_ED->Fill(distED,fHitStationED->fNel);
      hLDF_Nph_ED->Fill(distED,fHitStationED->fNph);
    }


  // TCanvas* c1 = new TCanvas;
  // c1->Divide(3,1);

  // c1->cd(1);
  // hLDF_Nmu_pos->SetMarkerStyle(34);
  // hLDF_Nmu_pos->SetMarkerColor(38);
  // hLDF_Nmu_pos->Draw("point");
  // hLDF_Nmu_neg->SetMarkerStyle(34);
  // hLDF_Nmu_neg->SetMarkerColor(46);
  // hLDF_Nmu_neg->Draw("pointsame");

  // c1->cd(2);
  // hLDF_Nel_pos->SetMarkerStyle(34);
  // hLDF_Nel_pos->SetMarkerColor(38);
  // hLDF_Nel_pos->Draw("point");
  // hLDF_Nel_neg->SetMarkerStyle(34);
  // hLDF_Nel_neg->SetMarkerColor(46);
  // hLDF_Nel_neg->Draw("pointsame");
  // c1->cd(3);
  // hLDF_Nph_pos->SetMarkerStyle(34);
  // hLDF_Nph_pos->SetMarkerColor(38);
  // hLDF_Nph_pos->Draw("point");
  // hLDF_Nph_neg->SetMarkerStyle(34);
  // hLDF_Nph_neg->SetMarkerColor(46);
  // hLDF_Nph_neg->Draw("pointsame");

  TCanvas* c2 = new TCanvas;
  c2->Divide(3,2);
  c2->cd(1);
  hLDF_Nmu_MD->SetMarkerStyle(34);
  hLDF_Nmu_MD->SetMarkerColor(38);
  hLDF_Nmu_MD->Draw("point");
  c2->cd(2);
  hLDF_Nel_MD->SetMarkerStyle(34);
  hLDF_Nel_MD->SetMarkerColor(46);
  hLDF_Nel_MD->Draw("point");
  c2->cd(3);
  hLDF_Nph_MD->SetMarkerStyle(34);
  hLDF_Nph_MD->SetMarkerColor(18);
  hLDF_Nph_MD->Draw("point");
  c2->cd(4);
  hLDF_Nmu_ED->SetMarkerStyle(34);
  hLDF_Nmu_ED->SetMarkerColor(38);
  hLDF_Nmu_ED->Draw("point");
  c2->cd(5);
  hLDF_Nel_ED->SetMarkerStyle(34);
  hLDF_Nel_ED->SetMarkerColor(46);
  hLDF_Nel_ED->Draw("point");
  c2->cd(6);
  hLDF_Nph_ED->SetMarkerStyle(34);
  hLDF_Nph_ED->SetMarkerColor(18);
  hLDF_Nph_ED->Draw("point");
}





void Analyze::DrawOnePM(int num,int ipm)
{
  map<int,double> trace;
  map<int,double> trace_mu; //
  TH1F* histo;
  TH1F* histo_mu; //
 
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);

  ipm--;
  
  fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
  fCanvas = (TCanvas *) fCanvasList->Last();
  fCanvas->Divide(2,1);
  char*hname = new char[15];
  char*hname_mu = new char[15];
  int idistance = fHitStationMD->fR_sf;
  int id = fHitStationMD->fId;
  
  sprintf(hname,"PMT%1d_%04d_%04d",ipm+1,id,idistance);
  
  cout << hname << endl;
  histo = new TH1F(hname,hname,5000,0.,5000);
  delete hname;
  
  sprintf(hname_mu,"PMT_MUONS",ipm+1,id,idistance);
  
  cout << hname_mu << endl;
  histo_mu = new TH1F(hname_mu,hname_mu,5000,0.,5000);
  delete hname_mu;
  
  fHisto1->Add(histo);
  trace = fHitStationMD->fPMT[ipm].fPMTSignal_hi;
  fHisto1->Add(histo_mu); //
  trace_mu = fHitStationMD->fPMT[ipm].fPMTSignal_hi_mu; //
    
  for(map<int,double>::const_iterator slot=trace.begin();
      slot!=trace.end(); slot++)
    {
      if(((*slot).first)<10000)
	histo->Fill(((*slot).first),(*slot).second);
    }
  
  for(map<int,double>::const_iterator slot=trace_mu.begin();
      slot!=trace_mu.end(); slot++)
    {
      if(((*slot).first)<10000)
	histo_mu->Fill(((*slot).first),(*slot).second);
    }
   
  histo->SetLineColor(kRed);
  fCanvas->cd(1); //
  histo->Draw();
  
  histo_mu->SetLineColor(kBlue); //
  fCanvas->cd(2); //
  histo_mu->Draw(); //
}




void Analyze::DrawOneProfileMD(int num)
{
  map<int,double> trace;
  map<int,double> trace_mu;
  TH1F* histo;
  TH1F* histo_mu;
  
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);

  fTreeMD->GetEvent(num);
  int nmu = fHitStationMD->fNmu;
  if(nmu<20 && nmu != 0) 
    {
      for (int nm=0; nm<nmu; nm++)
	{
	  cout << "muon " << nm << "\t time = " << fHitStationMD->fMuTimes[nm] << endl;
	}
    }

  if(fHitStationMD->fNpe > 0) 
    {
      fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
      fCanvas = (TCanvas *) fCanvasList->Last();
      
      char*hname = new char[15];
      char*hname_mu = new char[15];
      int idistance = (int)fHitStationMD->fR_sf;
      int id = fHitStationMD->fId;
      sprintf(hname,"SUMPMT_%04d_%04d",id,idistance);
      sprintf(hname_mu,"SUMPMT_%04d_%04d_mu",id,idistance);
      
      cout << hname << endl;
      histo = new TH1F(hname,hname,5000,0.,5000);
      histo_mu = new TH1F(hname_mu,hname_mu,5000,0.,5000);
      delete hname;
      
      fHisto1->Add(histo);
      fHisto1->Add(histo_mu);
      trace = fHitStationMD->fTimeProfile;
      
      for(map<int,double>::const_iterator slot = trace.begin();
	  slot != trace.end(); slot++)
	{
	  if(((*slot).first)<10000)
	    histo->Fill(((*slot).first),(*slot).second);
	} 
      
      trace_mu = fHitStationMD->fTimeProfile_mu;
      
      for(map<int,double>::const_iterator slot = trace_mu.begin();
	  slot != trace_mu.end(); slot++)
	{
	  if(((*slot).first)<10000)
	    histo_mu->Fill(((*slot).first),(*slot).second);
	}
      
      // Discrimination photon/muon !!

      //histo->SetLineColor(kRed);  // A COMMENTER
      histo_mu->SetLineColor(kBlue);
      histo_mu->Draw();
      // histo->Draw("SAME"); // A COMMENTER
    }
}



void Analyze::DrawOneProfileED(int num)
{
  map<int,double> trace;
  map<int,double> trace_mu;
  TH1F* histo;
  TH1F* histo_mu;
  
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);

  fTreeED->GetEvent(num);
  int nmu = fHitStationED->fNmu;
  if(nmu<20) 
    {
      for (int nm=0;nm<nmu;nm++)
	{
	  cout << "muon " << nm << "\t time = " << fHitStationED->fMuTimes[nm] << endl;
	}
    }
  if(fHitStationED->fNpe > 0) 
    {
      fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
      fCanvas = (TCanvas *) fCanvasList->Last();
      
      char*hname = new char[15];
      int idistance = (int) fHitStationED->fR_sf;
      int id = fHitStationED->fId;
      sprintf(hname,"SUMPMT_%04d_%04d",id,idistance);
      
      cout << hname << endl;
      histo = new TH1F(hname,hname,200,0.,200);
      histo_mu = new TH1F(hname,hname,200,0.,200);
      delete hname;
      
      fHisto1->Add(histo);
      fHisto1->Add(histo_mu);
      trace = fHitStationED->fTimeProfile;
      
      for(map<int,double>::const_iterator slot = trace.begin(); slot != trace.end(); slot++)
	{
	  if(((*slot).first)<10000){
	    histo->Fill(((*slot).first),(*slot).second);
	    cout << "histo -> " << (*slot).first << endl; }
	}
      
      trace_mu = fHitStationED->fTimeProfile_mu;
      
      for(map<int,double>::const_iterator slot = trace_mu.begin(); slot != trace_mu.end(); slot++)
      {
	if(((*slot).first)<10000) {
	  histo_mu->Fill(((*slot).first),(*slot).second);
	  cout << "histo_mu -> " << (*slot).first << endl; }
	
      }
      // Discrimination photon/muon !!
      histo->SetLineColor(kRed); // A COMMENTER
      histo_mu->SetLineColor(kBlue);
      histo->Draw(); // A COMMENTER     
      histo_mu->Draw("same");
      // histo->Draw(); // A COMMENTER     
    }
}


double Analyze::GetMDChargeInVEM(int num)
{
  map<int,double> trace; 
  fTreeMD->GetEvent(num);

  int nmu = fHitStationMD->fNmu;
  double charge;
  double chargeinvem;
  charge=0;
  
  trace = fHitStationMD->fTimeProfile_mu;
  
  for(map<int,double>::const_iterator slot = trace.begin();
      slot != trace.end(); slot++)
    {
      if(((*slot).first)<10000)
	charge+=(*slot).second;
    } 
  chargeinvem = charge/VEMCHARGEVALUEINPE;
  //cout << "in GetMDChargeInVEM : " << num << " " << chargeinvem << " = " << charge << "/" <<  VEMCHARGEVALUEINPE << endl;
  //if(charge>0) cout<< num << "  muon charge in pe and in VEM "<<charge<<" "<<chargeinvem<<" "<<nmu<<endl; 
  
  return chargeinvem;  
}



int Analyze::TrigInMDCharge() {
  int nev=fTreeMD->GetEntries();
  int mul=0;
  
  for (int i=0; i<nev; i++) {
    if(GetMDChargeInVEM(i) >= 0.5 && fHitStationMD->fNmu > 0)
      mul++;
  }
  return mul;
}

int Analyze::TrigInMuons() {
  int nev=fTreeMD->GetEntries();
  int mul = 0, nmu = 0;
   
  for (int i=0; i<nev; i++) {  
    fTreeMD->GetEvent(i);
    nmu= fHitStationMD->fNmu;
    if(nmu>1) {
      mul++;
      cout << "StaId = " << fHitStationMD->fId << " & Nmu = " << nmu << endl; }
  }
  return mul;
}


int Analyze::TrigED() {
  int nev=fTreeED->GetEntries();
  int mul = 0, npe = 0;
  cout << "Nevent ED = " << nev << endl;
  for (int i=0; i<nev; i++) {  
    fTreeED->GetEvent(i);
    npe = fHitStationED->fNpe;
    if(npe>18)
      mul++;
  }
  return mul;
}

/////////// ED : Sum(Npe)   &  MD : Sum(Sig) /////////////// 
int Analyze::TrigEDforQ(){
  
  int nev=fTreeED->GetEntries();
  
  int mul = 0, npe=0;
    
  for (int i=0; i<nev; i++) {  
    fTreeED->GetEvent(i);
    npe = fHitStationED->fNpe;
    if(npe>18)
      mul+=npe;
  }
  //cout<<mul<<endl;
  return mul;
}

/////// Change Trigger MD : Sig > 3 VEM ---> Sig > 1 VEM !!! Ajouter avec Sig > 0.5 VEM //////////////
float Analyze::TrigMDforQ() {
  int nev=fTreeMD->GetEntries();
  float mulSig=0.;
  
  for (int i=0; i<nev; i++) {
    if(GetMDChargeInVEM(i) >= 0.5 && fHitStationMD->fNmu > 0)
      mulSig+=GetMDChargeInVEM(i);
  }
  //cout<<mul<<endl;
  return mulSig;
}


float Analyze::TrigMDMuforQ() {
  int nev=fTreeMD->GetEntries();
  float mulSig=0.;
  
  for (int i=0; i<nev; i++) {
    if(GetMDChargeInVEM(i) >= 0.9 && fHitStationMD->fNmu > 0)
      fTreeMD->GetEvent(i);
      mulSig+=fHitStationMD->fNmu;
  }
  //cout<<mul<<endl;
  return mulSig;
}



///////////////////////////////////////////////////////////////



int Analyze::GetListGoodFile()
{
  char good_size_11[50] = "11M", good_size_12[50] = "12M";
  char name[500];
  char size[50];
  int compare_size_11, compare_size_12;
  int mul;
  int compteur = 0;
  char cmd[255]; 
  //  /projet/auger/Corsika_6.9/corsika_grid/v6.990.r2427/run/LhaRootGamma/*corsika**.root
  //  LhaRootProton/*corsika**.root
  sprintf(cmd, "ls -lh LhaRootProton/Sim_corsika**.root  | awk '{print $9,$5}' >> res2.txt");
  system(cmd);
  
  ifstream fichier("res2.txt", ios::in);
  if(fichier) 
    {
      while(fichier.good())
	{
	  fichier >> name >> size;
	  //cout << name << " " << size << endl;
	  compare_size_11 = strcmp(size,good_size_11);
	  compare_size_12 = strcmp(size,good_size_12);
	  if(compare_size_11 == 0 || compare_size_12 == 0)
	    {
	      cout << name << " " << size << endl;
	      ofstream file("ListGoodFileProton.txt", ios::out | ios::app); 
	      if(file)
		{
		  file << name << endl;
		  file.close();
		}
	    }     
	  //cout << name  << " hasn't a good size = " << size << " !" << endl; 
	  compteur++;
	  cout << compteur << endl;
	}
    }
}




int Analyze::Efficiency()
{
  if(fTreeEvent!=0 || fTreeED!=0 || fTreeMD!= 0)
    { 
      delete fTreeEvent; delete fTreeED; delete fTreeMD;
      fTreeEvent = 0; fTreeED=0; fTreeMD=0;
    }
  
  char name[500];
  char size[50];
  int compare_size;
  int mul_mu, mul_VEM, mul_ED;
  int compteur = 0;
  

  ifstream fichier("ListGoodFileGamma.txt", ios::in);
  if(fichier) 
    {
      while(fichier.good())
	{
	  mul_mu = 0;
	  mul_VEM = 0;
	  mul_ED = 0;
	  fichier >> name;
	  // cout << name << endl;
	  
	  fFile = new TFile(name,"READ");	// Open file	
  
	  // Lecture de l'arbre contenant le détecteur MD :
	  fTreeMD = (TTree *) fFile->Get("TreeMD");   // Get Tree 
	  if(fBranchMD!=0)
	    { 
	      delete fBranchMD;
	      fBranchMD = 0;
	    }
	  fBranchMD = fTreeMD->GetBranch("HitStationMD");
	  fTreeMD->SetBranchAddress("hstationMD",&fHitStationMD);
	  
	  // Lecture de l'arbre contenant le détecteur ED :
	  fTreeED = (TTree *) fFile->Get("TreeED");   // Get Tree 
	  if(fBranchED!=0)
	    { 
	      delete fBranchED;
	      fBranchED = 0;
	    }
	  fBranchED = fTreeMD->GetBranch("HitStationED");
	  fTreeED->SetBranchAddress("hstationED",&fHitStationED);
	  
	  // Lecture de l'arbre contenant les infos événement :
	  fTreeEvent = (TTree *) fFile->Get("TreeEvent");  // Get Tree
	  //	fTreeEvent->Print(); 
	  if(fBranchEvent!=0)
	    { 
	      delete fBranchEvent;
	      fBranchEvent = 0;
	    }
	  fBranchEvent = fTreeEvent->GetBranch("Event");
	  fTreeEvent->SetBranchAddress("event",&fEvent);  
	  //	cout << " \t * There is " << fTreeEvent->GetEntries() << " events in the input file (BranchEvent)"<<endl;
	  fTreeEvent->GetEvent(0);
	  //cout<<fEvent->fEnergy<<" "<<fEvent->fTheta*180/3.1416<<" "<<mul<<endl;
	  mul_mu = TrigInMuons();
	  mul_VEM = TrigInMDCharge();
	  mul_ED = TrigED();

	  ofstream file("ListEfficencyGamma.txt", ios::out | ios::app); 
	  if(file)
	    {
	      file << fEvent->fEnergy << " " << fEvent->fTheta*180/3.1416 << " " << mul_mu << " " << mul_VEM << " " << mul_ED << endl;
	      file.close();
	    }
	  
	  cout << fEvent->fEnergy << " " << fEvent->fTheta*180/3.1416 << " " << mul_mu << " " << mul_VEM << " " << mul_ED << " " << name << endl;

	  delete fFile;
	}
    }    
  return 1; 
}


int Analyze::GetListMDED()
{
  if(fTreeEvent!=0 || fTreeED!=0 || fTreeMD!= 0)
    { 
      delete fTreeEvent; delete fTreeED; delete fTreeMD;
      fTreeEvent = 0; fTreeED=0; fTreeMD=0;
    }
  char name[500];
  char size[50];
  int compare_size;
  float mul_MD= 0.;
  int compteur = 0, mul_ED =0, mul_MDmu = 0;

  ifstream fichier("ListGoodFileProton.txt", ios::in);
  if(fichier) 
    {
      while(fichier.good())
	{
	  mul_MD=0; mul_ED=0; mul_MDmu=0;
	  fichier >> name;
	  fFile = new TFile(name,"READ");
	  // Lecture de l'arbre contenant le détecteur MD :
	  fTreeMD = (TTree *) fFile->Get("TreeMD");   // Get Tree 
	  if(fBranchMD!=0)
	    { 
	      delete fBranchMD;
	      fBranchMD = 0;
	    }
	  fBranchMD = fTreeMD->GetBranch("HitStationMD");
	  fTreeMD->SetBranchAddress("hstationMD",&fHitStationMD);
	  
	  // Lecture de l'arbre contenant le détecteur ED :
	  fTreeED = (TTree *) fFile->Get("TreeED");   // Get Tree 
	  if(fBranchED!=0)
	    { 
	      delete fBranchED;
	      fBranchED = 0;
	    }
	  fBranchED = fTreeMD->GetBranch("HitStationED");
	  fTreeED->SetBranchAddress("hstationED",&fHitStationED);
	  
	  // Lecture de l'arbre contenant les infos événement :
	  fTreeEvent = (TTree *) fFile->Get("TreeEvent");  // Get Tree
	  //	fTreeEvent->Print(); 
	  if(fBranchEvent!=0)
	    { 
	      delete fBranchEvent;
	      fBranchEvent = 0;
	    }
	  fBranchEvent = fTreeEvent->GetBranch("Event");
	  fTreeEvent->SetBranchAddress("event",&fEvent);  
	  fTreeEvent->GetEvent(0);

	  mul_MD = TrigMDforQ();
	  mul_MDmu = TrigMDMuforQ();
	  mul_ED = TrigEDforQ();

	  ofstream file("ListSigMDNpeEDTrig05_Proton.txt", ios::out | ios::app); 
	  if(file)
	    {
	      file << fEvent->fEnergy << " " << fEvent->fTheta*180/3.1416 << " " << mul_MD << " " << mul_ED << " " << mul_MDmu << endl;
	      file.close();
	    }
	  
	  cout << fEvent->fEnergy << " " << fEvent->fTheta*180/3.1416 << " " << mul_MD << " " << mul_ED << " " << mul_MDmu << " " << name << endl;
	  
	  delete fFile;
	}
    }   
  
  return 1; 
}

void Analyze::DrawTotQ(int mode)
{
  // Déclaration de variables :
  double DEG2RAD = PI/180.;
  vector<float> vmul_MD0, vmul_MD1, vmul_ED0, vgmul_MD0, vgmul_MD1, vgmul_ED0;
  int energy; 
  float mul_MD, mul_ED, mul_MDmu;
  float theta;
  float E[10] = {1000.,3000.,6000.,10000.,30000.,60000.,100000.,300000.,600000.,1000000.};

// Fichier proton :
  ifstream in2;
  //in2.open("ListSigMDNpeEDTrig1_Proton2.txt",ios::in);
  //in2.open("SaveFiles/ListSigMDNpeED_Proton.txt",ios::in);
  in2.open("ListSigMDNpeEDTrig05_Proton.txt",ios::in);

  while(!in2.eof()) {
    in2 >> energy >> theta >> mul_MD >> mul_ED; //>> mul_MDmu;
    //    in2 >> energy >> theta >> mul_MD >> mul_ED;
    // cout << energy << " " << theta << " " << mul_MD << " " << mul_ED << " " << mul_MDmu << endl; 
    vmul_MD0.push_back(mul_MD); vmul_ED0.push_back(mul_ED/25.); //vmul_MD1.push_back(mul_MDmu);
    if(in2.eof()) break;
    // if(mode == 1)
    
  } // fin lecture fichier "ListSigMDNpeED.txt"
  
  
  // Fichier Gamma :
  ifstream in;  
  //in.open("ListSigMDNpeEDTrig1_Gamma2.txt",ios::in);
  //in.open("SaveFiles/ListSigMDNpeED_GammaRE.txt",ios::in);
  in.open("ListSigMDNpeEDTrig05_Gamma.txt",ios::in);

  while(!in.eof()) {
    in >> energy >> theta >> mul_MD >> mul_ED; //>> mul_MDmu;
    //    in >> energy >> theta >> mul_MD >> mul_ED;
    //    cout << energy << ", " << theta << ", " << mul_ED << ", " << mul_MD <<endl; 
    vgmul_MD0.push_back(mul_MD); vgmul_ED0.push_back(mul_ED/25.); //vgmul_MD1.push_back(mul_MDmu);
    if(in.eof()) break;
    
  } // fin lecture fichier GAMMA
  
  ///////////// TGraph ////////////////
  int size0 = vmul_MD0.size(); int vsize0 = vgmul_MD0.size();
    
  // PROTON
  TGraph *gr0 = new TGraph(size0,&vmul_ED0[0],&vmul_MD0[0]);  gr0->SetMarkerStyle(7); gr0->SetMarkerColor(2); gr0->GetXaxis()->SetTitle("Ne"); gr0->GetYaxis()->SetTitle("Sum Signal [VEM]");
  // GAMMA :
  TGraph *gg0 = new TGraph(vsize0,&vgmul_ED0[0],&vgmul_MD0[0]);  gg0->SetMarkerStyle(7); gg0->SetMarkerColor(4); gg0->GetXaxis()->SetTitle("Ne"); gg0->GetYaxis()->SetTitle("Sum Signal [VEM]");
  // Multi Graph : SumSig = f(Ne)  ::  
  TMultiGraph *mpg0 = new TMultiGraph(); 
  mpg0->Add(gr0); mpg0->Add(gg0); mpg0->SetTitle(";Ne;Sum Signal [VEM]");
 
  // /////////////// Sum Sig --> Nmu /////////////////

  // // PROTON
  // TGraph *gr1 = new TGraph(size0,&vmul_ED0[0],&vmul_MD1[0]); gr1->SetMarkerStyle(7); gr1->SetMarkerColor(2); gr1->GetXaxis()->SetTitle("Ne"); gr1->GetYaxis()->SetTitle("Nmu");
  // // GAMMA :
  // TGraph *gg1 = new TGraph(vsize0,&vgmul_ED0[0],&vgmul_MD1[0]); gg1->SetMarkerStyle(7); gg1->SetMarkerColor(4); gg1->GetXaxis()->SetTitle("Ne"); gg1->GetYaxis()->SetTitle("Nmu");
  // // Multi Graph : Nmu = f(Ne)  ::  
  // TMultiGraph *mpg1 = new TMultiGraph(); 
  // mpg1->Add(gr1); mpg1->Add(gg1); mpg1->SetTitle(";Ne;Nmu");


  ///////////////////////////////////////////
  if(mode == 1) {
    TCanvas *c = new TCanvas("c");
    c->cd(1); mpg0->Draw("AP");
  }
  // else if(mode == 2) {
  //     TCanvas *c2 = new TCanvas("c2");
  //     c2->cd(1); mpg1->Draw("AP");
  //   }
}



void Analyze::DrawQ(int it_en, int mode)  
{
  // Déclaration de variables :
  double DEG2RAD = PI/180.;
  vector<int> vmul_MD0, vmul_ED0, vmul_MD1, vmul_ED1, vmul_MD2, vmul_ED2, vmul_MD3, vmul_ED3, vmul_MD4, vmul_ED4, vmul_MD5, vmul_ED5, vmulM_MD0, vmulM_MD1, vmulM_MD2, vmulM_MD3, vmulM_MD4, vmulM_MD5;
  
  vector<int> vgmul_MD0,vgmul_ED0, vgmul_MD1, vgmul_ED1, vgmul_MD2, vgmul_ED2, vgmul_MD3, vgmul_ED3, vgmul_MD4, vgmul_ED4, vgmul_MD5, vgmul_ED5, vgmulM_MD0, vgmulM_MD1, vgmulM_MD2, vgmulM_MD3, vgmulM_MD4, vgmulM_MD5;

  int energy; 
  double mul_MD, mul_ED, mul_MDmu;
  double theta;
  float E[10] = {1000.,3000.,6000.,10000.,30000.,60000.,100000.,300000.,600000.,1000000.};
  float th[6] = {0.,0.1,0.2,0.3,0.4,0.5};

  // Fichier proton :
  ifstream in2;
  //  in2.open("ListSigMDNpeED_Proton.txt",ios::in);
  // in2.open("ListSigMDNpeEDTrig05_Proton.txt",ios::in);
  in2.open("ListSigMDNpeEDTrig05_Gamma.txt",ios::in);

  while(!in2.eof()) {
    in2 >> energy >> theta >> mul_MD >> mul_ED >> mul_MDmu;
    // in2 >> energy >> theta >> mul_MD >> mul_ED;
    //cout << energy << ", " << theta << ", " << mul_ED << ", " << mul_MD <<endl; 
    if(in2.eof()) break;
    
    if(energy == E[it_en]) {
      //      cout << energy << ", " << theta << ", " << mul_MD << ", " << mul_MDmu << ", " << mul_ED <<endl; 
      cout << theta << " " << mul_MD << " " << mul_MDmu << " " << mul_ED/25 <<endl; 
      if(theta == 0) {
	vmul_MD0.push_back(mul_MD); vmul_ED0.push_back(mul_ED/25.); vmulM_MD0.push_back(mul_MDmu);
      }
      else if(theta == 0.1) {
	vmul_MD1.push_back(mul_MD); vmul_ED1.push_back(mul_ED/25.); vmulM_MD1.push_back(mul_MDmu);
      }
      else if(theta == 0.2) {
	vmul_MD2.push_back(mul_MD); vmul_ED2.push_back(mul_ED/25.); vmulM_MD2.push_back(mul_MDmu);
      }
      else if(theta == 0.3) {
	vmul_MD3.push_back(mul_MD); vmul_ED3.push_back(mul_ED/25.); vmulM_MD3.push_back(mul_MDmu);
      }
      else if(theta == 0.4) {
	vmul_MD4.push_back(mul_MD); vmul_ED4.push_back(mul_ED/25.); vmulM_MD4.push_back(mul_MDmu);
      }
      else if(theta == 0.5) {
	vmul_MD5.push_back(mul_MD); vmul_ED5.push_back(mul_ED/25.); vmulM_MD5.push_back(mul_MDmu);
      }
    }
  } // fin lecture fichier "ListSigMDNpeED.txt"
  

  // // Fichier Gamma :
  // ifstream in;  
  // //in.open("ListSigMDNpeED_Gamma.txt",ios::in);
  // in.open("ListSigMDNpeEDTrig05_Gamma.txt",ios::in);
  
  // while(!in.eof()) {
  //   in >> energy >> theta >> mul_MD >> mul_ED >> mul_MDmu;
  //   if(in.eof()) break;
  //   //cout << it_en << endl;
  //   if(energy == E[it_en]) {
  //     // cout << energy << ", " << theta << ", " << mul_ED << ", " << mul_MD <<endl; 
  //     if(theta == 0) {
  // 	vgmul_MD0.push_back(mul_MD); vgmul_ED0.push_back(mul_ED/25.); vgmulM_MD0.push_back(mul_MDmu);
  //     }
  //     else if(theta == 0.1) {
  // 	vgmul_MD1.push_back(mul_MD); vgmul_ED1.push_back(mul_ED/25.); vgmulM_MD1.push_back(mul_MDmu);
  //     }
  //     else if(theta == 0.2) {
  // 	vgmul_MD2.push_back(mul_MD); vgmul_ED2.push_back(mul_ED/25.); vgmulM_MD2.push_back(mul_MDmu);
  //     }
  //     else if(theta == 0.3) {
  // 	vgmul_MD3.push_back(mul_MD); vgmul_ED3.push_back(mul_ED/25.); vgmulM_MD3.push_back(mul_MDmu);
  //     }
  //     else if(theta == 0.4) {
  // 	vgmul_MD4.push_back(mul_MD); vgmul_ED4.push_back(mul_ED/25.); vgmulM_MD4.push_back(mul_MDmu);
  //     }
  //     else if(theta == 0.5) {
  // 	vgmul_MD5.push_back(mul_MD); vgmul_ED5.push_back(mul_ED/25.); vgmulM_MD5.push_back(mul_MDmu);
  //     }
  //   }
  // } // fin lecture fichier GAMMA
  

  //////////////////////////////////////////////////////////////
  ///                    TGRAPH                               //
  //////////////////////////////////////////////////////////////
    int size0 = vmul_MD0.size(), size1 = vmul_MD1.size(), size2 = vmul_MD2.size(), size3 = vmul_MD3.size(), size4 = vmul_MD4.size(), size5 = vmul_MD5.size();
    int vsize0 = vgmul_MD0.size(), vsize1 = vgmul_MD1.size(), vsize2 = vgmul_MD2.size(), vsize3 = vgmul_MD3.size(), vsize4 = vgmul_MD4.size(), vsize5 = vgmul_MD5.size();


    ///////////////////////////////////
    // MODE 1 : Sum Signal = f (Npe) //
    ///////////////////////////////////
    // Sum Npe -> Ne // PROTON
    TGraph *gr0 = new TGraph(size0,&vmul_ED0[0],&vmul_MD0[0]);  gr0->SetTitle("th = 0"); gr0->SetMarkerStyle(7); gr0->SetMarkerColor(2); gr0->GetXaxis()->SetTitle("Ne"); gr0->GetYaxis()->SetTitle("Sum Signal [VEM]");
    TGraph *gr1 = new TGraph(size1,&vmul_ED1[0],&vmul_MD1[0]); gr1->SetTitle("th = 18"); gr1->SetMarkerStyle(7); gr1->SetMarkerColor(2); gr1->GetXaxis()->SetTitle("Ne"); gr1->GetYaxis()->SetTitle("Sum Signal [VEM]");
    TGraph *gr2 = new TGraph(size2,&vmul_ED2[0],&vmul_MD2[0]); gr2->SetTitle("th = 26"); gr2->SetMarkerStyle(7); gr2->SetMarkerColor(2); gr2->GetXaxis()->SetTitle("Ne"); gr2->GetYaxis()->SetTitle("Sum Signal [VEM]");
    TGraph *gr3 = new TGraph(size3,&vmul_ED3[0],&vmul_MD3[0]); gr3->SetTitle("th = 33"); gr3->SetMarkerStyle(7); gr3->SetMarkerColor(2); gr3->GetXaxis()->SetTitle("Ne"); gr3->GetYaxis()->SetTitle("Sum Signal [VEM]");
    TGraph *gr4 = new TGraph(size4,&vmul_ED4[0],&vmul_MD4[0]); gr4->SetTitle("th = 39"); gr4->SetMarkerStyle(7); gr4->SetMarkerColor(2); gr4->GetXaxis()->SetTitle("Ne"); gr4->GetYaxis()->SetTitle("Sum Signal [VEM]");
    TGraph *gr5 = new TGraph(size5,&vmul_ED5[0],&vmul_MD5[0]); gr5->SetTitle("th = 45"); gr5->SetMarkerStyle(7); gr5->SetMarkerColor(2); gr5->GetXaxis()->SetTitle("Ne"); gr5->GetYaxis()->SetTitle("Sum Signal [VEM]");
    // GAMMA :
    TGraph *gg0 = new TGraph(vsize0,&vgmul_ED0[0],&vgmul_MD0[0]);  gg0->SetTitle("th = 0"); gg0->SetMarkerStyle(7); gg0->SetMarkerColor(4); gg0->GetXaxis()->SetTitle("Ne"); gr0->GetYaxis()->SetTitle("Sum Signal [VEM]");
    TGraph *gg1 = new TGraph(vsize1,&vgmul_ED1[0],&vgmul_MD1[0]); gg1->SetTitle("th = 18"); gg1->SetMarkerStyle(7); gg1->SetMarkerColor(4); gg1->GetXaxis()->SetTitle("Ne"); gr1->GetYaxis()->SetTitle("Sum Signal [VEM]");
    TGraph *gg2 = new TGraph(vsize2,&vgmul_ED2[0],&vgmul_MD2[0]); gg2->SetTitle("th = 26"); gg2->SetMarkerStyle(7); gg2->SetMarkerColor(4); gg2->GetXaxis()->SetTitle("Ne"); gr2->GetYaxis()->SetTitle("Sum Signal [VEM]");
    TGraph *gg3 = new TGraph(vsize3,&vgmul_ED3[0],&vgmul_MD3[0]); gg3->SetTitle("th = 33"); gg3->SetMarkerStyle(7); gg3->SetMarkerColor(4); gg3->GetXaxis()->SetTitle("Ne"); gr3->GetYaxis()->SetTitle("Sum Signal [VEM]");
    TGraph *gg4 = new TGraph(vsize4,&vgmul_ED4[0],&vgmul_MD4[0]); gg4->SetTitle("th = 39"); gg4->SetMarkerStyle(7); gg4->SetMarkerColor(4); gg4->GetXaxis()->SetTitle("Ne"); gr4->GetYaxis()->SetTitle("Sum Signal [VEM]");
    TGraph *gg5 = new TGraph(vsize5,&vgmul_ED5[0],&vgmul_MD5[0]); gg5->SetTitle("th = 45"); gg5->SetMarkerStyle(7); gg5->SetMarkerColor(4); gg5->GetXaxis()->SetTitle("Ne"); gr5->GetYaxis()->SetTitle("Sum Signal [VEM]");
    // MultiGraph par theta & par énergie :
    TMultiGraph *mpg0 = new TMultiGraph(); 
    mpg0->Add(gr0); mpg0->Add(gg0); mpg0->SetTitle("th 0;Ne;Sum Signal [VEM]");
    TMultiGraph *mpg1 = new TMultiGraph(); 
    mpg1->Add(gr1); mpg1->Add(gg1); mpg1->SetTitle("th 18;Ne;Sum Signal [VEM]");
    TMultiGraph *mpg2 = new TMultiGraph();
    mpg2->Add(gr2); mpg2->Add(gg2); mpg2->SetTitle("th 26;Ne;Sum Signal [VEM]");
    TMultiGraph *mpg3 = new TMultiGraph();
    mpg3->Add(gr3); mpg3->Add(gg3); mpg3->SetTitle("th 33;Ne;Sum Signal [VEM]");
    TMultiGraph *mpg4 = new TMultiGraph();
    mpg4->Add(gr4); mpg4->Add(gg4); mpg4->SetTitle("th 39;Ne;Sum Signal [VEM]");
    TMultiGraph *mpg5 = new TMultiGraph();
    mpg5->Add(gr5); mpg5->Add(gg5); mpg5->SetTitle("th 45;Ne;Sum Signal [VEM]");
    // MultiGraph tout theta confondu & par énergie :
    TMultiGraph *mpg = new TMultiGraph();
    mpg->Add(gr0); mpg->Add(gg0); mpg->Add(gr1); mpg->Add(gg1); mpg->Add(gr2); mpg->Add(gg2); mpg->Add(gr3); mpg->Add(gg3); mpg->Add(gr4); mpg->Add(gg4); mpg->Add(gr5); mpg->Add(gg5); mpg->SetTitle(";Ne;Sum Signal [VEM]");


    ////////////////////////////
    // MODE 2 : Nmu = f (Npe) //
    ////////////////////////////
    // Sum Npe -> Ne // PROTON
    TGraph *grM0 = new TGraph(size0,&vmul_ED0[0],&vmulM_MD0[0]);  grM0->SetTitle("th = 0"); grM0->SetMarkerStyle(7); grM0->SetMarkerColor(2); grM0->GetXaxis()->SetTitle("Ne"); grM0->GetYaxis()->SetTitle("Nmu");
    TGraph *grM1 = new TGraph(size1,&vmul_ED1[0],&vmulM_MD1[0]); grM1->SetTitle("th = 18"); grM1->SetMarkerStyle(7); grM1->SetMarkerColor(2); grM1->GetXaxis()->SetTitle("Ne"); grM1->GetYaxis()->SetTitle("Nmu");
    TGraph *grM2 = new TGraph(size2,&vmul_ED2[0],&vmulM_MD2[0]); grM2->SetTitle("th = 26"); grM2->SetMarkerStyle(7); grM2->SetMarkerColor(2); grM2->GetXaxis()->SetTitle("Ne"); grM2->GetYaxis()->SetTitle("Nmu");
    TGraph *grM3 = new TGraph(size3,&vmul_ED3[0],&vmulM_MD3[0]); grM3->SetTitle("th = 33"); grM3->SetMarkerStyle(7); grM3->SetMarkerColor(2); grM3->GetXaxis()->SetTitle("Ne"); grM3->GetYaxis()->SetTitle("Nmu");
    TGraph *grM4 = new TGraph(size4,&vmul_ED4[0],&vmulM_MD4[0]); grM4->SetTitle("th = 39"); grM4->SetMarkerStyle(7); grM4->SetMarkerColor(2); grM4->GetXaxis()->SetTitle("Ne"); grM4->GetYaxis()->SetTitle("Nmu");
    TGraph *grM5 = new TGraph(size5,&vmul_ED5[0],&vmulM_MD5[0]); grM5->SetTitle("th = 45"); grM5->SetMarkerStyle(7); grM5->SetMarkerColor(2); grM5->GetXaxis()->SetTitle("Ne"); grM5->GetYaxis()->SetTitle("Nmu");
    // GAMMA :
    TGraph *ggM0 = new TGraph(vsize0,&vgmul_ED0[0],&vgmulM_MD0[0]);  ggM0->SetTitle("th = 0"); ggM0->SetMarkerStyle(7); ggM0->SetMarkerColor(4); ggM0->GetXaxis()->SetTitle("Ne"); ggM0->GetYaxis()->SetTitle("Nmu");
    TGraph *ggM1 = new TGraph(vsize1,&vgmul_ED1[0],&vgmulM_MD1[0]); ggM1->SetTitle("th = 18"); ggM1->SetMarkerStyle(7); ggM1->SetMarkerColor(4); ggM1->GetXaxis()->SetTitle("Ne"); ggM1->GetYaxis()->SetTitle("Nmu");
    TGraph *ggM2 = new TGraph(vsize2,&vgmul_ED2[0],&vgmulM_MD2[0]); ggM2->SetTitle("th = 26"); ggM2->SetMarkerStyle(7); ggM2->SetMarkerColor(4); ggM2->GetXaxis()->SetTitle("Ne"); ggM2->GetYaxis()->SetTitle("Nmu");
    TGraph *ggM3 = new TGraph(vsize3,&vgmul_ED3[0],&vgmulM_MD3[0]); ggM3->SetTitle("th = 33"); ggM3->SetMarkerStyle(7); ggM3->SetMarkerColor(4); ggM3->GetXaxis()->SetTitle("Ne"); ggM3->GetYaxis()->SetTitle("Nmu");
    TGraph *ggM4 = new TGraph(vsize4,&vgmul_ED4[0],&vgmulM_MD4[0]); ggM4->SetTitle("th = 39"); ggM4->SetMarkerStyle(7); ggM4->SetMarkerColor(4); ggM4->GetXaxis()->SetTitle("Ne"); ggM4->GetYaxis()->SetTitle("Nmu");
    TGraph *ggM5 = new TGraph(vsize5,&vgmul_ED5[0],&vgmulM_MD5[0]); ggM5->SetTitle("th = 45"); ggM5->SetMarkerStyle(7); ggM5->SetMarkerColor(4); ggM5->GetXaxis()->SetTitle("Ne"); ggM5->GetYaxis()->SetTitle("Nmu");
    // MultiGraph par theta & par énergie :
    TMultiGraph *mpgM0 = new TMultiGraph(); 
    mpgM0->Add(gr0); mpgM0->Add(ggM0); mpgM0->SetTitle("th 0;Ne;Nmu");
    TMultiGraph *mpgM1 = new TMultiGraph(); 
    mpgM1->Add(gr1); mpgM1->Add(ggM1); mpgM1->SetTitle("th 18;Ne;Nmu");
    TMultiGraph *mpgM2 = new TMultiGraph();
    mpgM2->Add(gr2); mpgM2->Add(ggM2); mpgM2->SetTitle("th 26;Ne;Nmu");
    TMultiGraph *mpgM3 = new TMultiGraph();
    mpgM3->Add(gr3); mpgM3->Add(ggM3); mpgM3->SetTitle("th 33;Ne;Nmu");
    TMultiGraph *mpgM4 = new TMultiGraph();
    mpgM4->Add(gr4); mpgM4->Add(ggM4); mpgM4->SetTitle("th 39;Ne;Nmu");
    TMultiGraph *mpgM5 = new TMultiGraph();
    mpgM5->Add(gr5); mpgM5->Add(ggM5); mpgM5->SetTitle("th 45;Ne;Nmu");
    // MultiGraph tout theta confondu & par énergie :
    TMultiGraph *mpgM = new TMultiGraph();
    mpgM->Add(grM0); mpgM->Add(ggM0); mpgM->Add(grM1); mpgM->Add(ggM1); mpgM->Add(grM2); mpgM->Add(ggM2); mpgM->Add(grM3); mpgM->Add(ggM3); mpgM->Add(grM4); mpgM->Add(ggM4); mpgM->Add(grM5); mpgM->Add(ggM5); mpgM->SetTitle(";Ne;Nmu");


    if(mode == 1) 
      {
	// 6 THETA :
	TCanvas *c = new TCanvas("c");  
	c->Divide(3,2);  c->cd(1);  mpg0->Draw("AP"); c->cd(2); mpg1->Draw("AP"); c->cd(3); mpg2->Draw("AP"); c->cd(4); mpg3->Draw("AP"); c->cd(5); mpg4->Draw("AP");  c->cd(6); mpg5->Draw("AP"); 
	// THETA CONFONDU :
	TCanvas *d = new TCanvas("d"); d->SetLogy();  d->SetLogx();
	d->cd(1); mpg->Draw("AP"); 
      }
    else if(mode == 2)
      {
	// 6 THETA :
	TCanvas *c2 = new TCanvas("c2");
	c2->Divide(3,2); c2->cd(1); mpgM0->Draw("AP"); c2->cd(2); mpgM1->Draw("AP"); c2->cd(3); mpgM2->Draw("AP"); c2->cd(4); mpgM3->Draw("AP"); c2->cd(5); mpgM4->Draw("AP");  c2->cd(6); mpgM5->Draw("AP"); 
	// THETA CONFONDU :
	TCanvas *d2 = new TCanvas("d2");
	d2->cd(1); mpgM->Draw("AP"); 
      }
}






void Analyze::DrawMulMDvsED(int it_en)
{
  ifstream in;
  in.open("ListEfficencyGamma.txt",ios::in);
  
  // Déclaration de variables :
  double DEG2RAD = PI/180.;
  // vector<int> ven, vmul, vmul_VEM, vmul_ED; vector<float> vth;
  vector<int> vmul0, vmul_VEM0, vmul_ED0, vmul1, vmul_VEM1, vmul_ED1, vmul2, vmul_VEM2, vmul_ED2, vmul3, vmul_VEM3, vmul_ED3, vmul4, vmul_VEM4, vmul_ED4, vmul5, vmul_VEM5, vmul_ED5;
  
  int energy, mul, mul_VEM, mul_ED;
  double theta;
  int E[10] = {1000,3000,6000,10000,30000,60000,100000,300000,600000,1000000};
  // float th[6] = {0.,0.1,0.2,0.3,0.4,0.5};
  
  while(!in.eof()) {
    in >> energy >> theta >> mul >> mul_VEM >> mul_ED;
    //cout << energy << ", " << theta << ", " << mul << ", " << mul_VEM <<  ", " << mul_ED <<endl; 
    if(in.eof()) break;
    
    if(energy == E[it_en]) {
      cout << energy << ", " << theta << ", " << mul << ", " << mul_VEM <<  ", " << mul_ED <<endl; 
      if(theta == 0) {
	vmul0.push_back(mul); vmul_VEM0.push_back(mul_VEM); vmul_ED0.push_back(mul_ED); 
	//cout << "--> " << energy << ", " << theta << ", " << mul << ", " << mul_VEM <<  ", " << mul_ED <<endl;
      }
      else if(theta == 0.1) {
	vmul1.push_back(mul); vmul_VEM1.push_back(mul_VEM); vmul_ED1.push_back(mul_ED); 
	cout << "--> " << energy << ", " << theta << ", " << mul << ", " << mul_VEM <<  ", " << mul_ED <<endl;
      }
      else if(theta == 0.2) {
	vmul2.push_back(mul); vmul_VEM2.push_back(mul_VEM); vmul_ED2.push_back(mul_ED); 
      }
      else if(theta == 0.3) {
	vmul3.push_back(mul); vmul_VEM3.push_back(mul_VEM); vmul_ED3.push_back(mul_ED); 
      }
      else if(theta == 0.4) {
	vmul4.push_back(mul); vmul_VEM4.push_back(mul_VEM); vmul_ED4.push_back(mul_ED); 
    }
      else if(theta == 0.5) {
	vmul5.push_back(mul); vmul_VEM5.push_back(mul_VEM); vmul_ED5.push_back(mul_ED); 
      }
    } // if energy == E[5]

  }
  
    int size0 = vmul0.size(), size1 = vmul1.size(), size2 = vmul2.size(), size3 = vmul3.size(), size4 = vmul4.size(), size5 = vmul5.size();
    cout << "Size : " << size0 << " " << size1 << " " << size2 << " " << size3 << " " << size4 << " " << size5 << endl; 
     
    TGraph *gr0 = new TGraph(size0,&vmul_ED0[0],&vmul_VEM0[0]);  gr0->SetTitle("theta = 0"); gr0->SetMarkerStyle(7); gr0->SetMarkerColor(2); gr0->GetXaxis()->SetTitle("Nb scintillateurs > 18 pe"); gr0->GetYaxis()->SetTitle("Nb cuves > 3 VEM");
    TGraph *gr1 = new TGraph(size1,&vmul_ED1[0],&vmul_VEM1[0]); gr1->SetTitle("theta = 18"); gr1->SetMarkerStyle(7); gr1->SetMarkerColor(2); gr1->GetXaxis()->SetTitle("Nb scintillateurs > 18 pe"); gr1->GetYaxis()->SetTitle("Nb cuves > 3 VEM");
    TGraph *gr2 = new TGraph(size2,&vmul_ED2[0],&vmul_VEM2[0]); gr2->SetTitle("theta = 26"); gr2->SetMarkerStyle(7); gr2->SetMarkerColor(2); gr2->GetXaxis()->SetTitle("Nb scintillateurs > 18 pe"); gr2->GetYaxis()->SetTitle("Nb cuves > 3 VEM");
    TGraph *gr3 = new TGraph(size3,&vmul_ED3[0],&vmul_VEM3[0]); gr3->SetTitle("theta = 33"); gr3->SetMarkerStyle(7); gr3->SetMarkerColor(2); gr3->GetXaxis()->SetTitle("Nb scintillateurs > 18 pe"); gr3->GetYaxis()->SetTitle("Nb cuves > 3 VEM");
    TGraph *gr4 = new TGraph(size4,&vmul_ED4[0],&vmul_VEM4[0]); gr4->SetTitle("theta = 39"); gr4->SetMarkerStyle(7); gr4->SetMarkerColor(2); gr4->GetXaxis()->SetTitle("Nb scintillateurs > 18 pe"); gr4->GetYaxis()->SetTitle("Nb cuves > 3 VEM");
    TGraph *gr5 = new TGraph(size5,&vmul_ED5[0],&vmul_VEM5[0]); gr5->SetTitle("theta = 45"); gr5->SetMarkerStyle(7); gr5->SetMarkerColor(2); gr5->GetXaxis()->SetTitle("Nb scintillateurs > 18 pe"); gr5->GetYaxis()->SetTitle("Nb cuves > 3 VEM");
    
    TCanvas *c = new TCanvas("c");
    c->Divide(3,2);
    c->cd(1); gr0->Draw("AP"); c->cd(2); gr1->Draw("AP"); c->cd(3); gr2->Draw("AP"); c->cd(4); gr3->Draw("AP"); c->cd(5); gr4->Draw("AP");  c->cd(6); gr5->Draw("AP"); 


  //TMultiGraph *mpg = new TMultiGraph("mpg","N_tanks = f(N_scintillators); N_scintillators; N_tanks");
  //mpg->Add(gr0); mpg->Add(gr1); mpg->Add(gr2); mpg->Add(gr3); mpg->Add(gr4); mpg->Add(gr5); mpg->Add(gr6); mpg->Add(gr7); mpg->Add(gr8); mpg->Add(gr9); 
  //mpg->Draw("AP")

}


void Analyze::ComputeEfficiency()
{
  ifstream in;
  in.open("ListEfficencyGamma.txt",ios::in);

  // Déclaration de variables :
  double DEG2RAD = PI/180.;
  vector<float> vmul, vmul_VEM, vmul_ED;
  vmul.clear();	vmul_VEM.clear(); vmul_ED.clear();
  vector<float> vth;
  double NShower = 0.;
  int energy, mul, mul_VEM, mul_ED,energy_temp=0, mul_temp;
  float theta, theta_temp=0., mul_mean = 0., mul_sum = 0., mul_VEM_mean = 0., mul_VEM_sum = 0., mul_ED_mean = 0., mul_ED_sum = 0.;

  while(!in.eof()) {
    in >> energy >> theta >> mul >> mul_VEM >> mul_ED;
    cout << energy << ", " << theta << ", " << mul << ", " << mul_VEM <<  ", " << mul_ED <<endl; 
    if(in.eof()) break;


    if(NShower > 0 && energy_temp == energy && theta_temp == theta)
      {
	NShower++;
	vmul.push_back(mul);
	vmul_VEM.push_back(mul_VEM);
	vmul_ED.push_back(mul_ED);
	energy_temp = energy; 
	theta_temp = theta;
	//cout << " NShower > 0 && energy_temp == energy && theta_temp == theta, NShower  : " <<  NShower << endl;
      }
    else if(NShower == 0)
      {
	vmul.push_back(mul);
	vmul_VEM.push_back(mul_VEM);
	vmul_ED.push_back(mul_ED);
	energy_temp = energy; 
	theta_temp = theta; 
	NShower ++;
      }
    else
      {
	cout << "NShower = " << NShower << " & size_vmul = " << vmul.size() << endl;
	mul_sum = 0.; mul_VEM_sum =0.; mul_ED_sum=0.;mul_mean=0.; mul_VEM_mean=0.; mul_ED_mean=0.;
	for(int i=0; i<NShower; i++) {
	  mul_sum += vmul[i];
	  mul_VEM_sum += vmul_VEM[i];
	  mul_ED_sum += vmul_ED[i];
	  cout << "### mul_sim = " << mul_sum << " " << mul_VEM_sum << " " << mul_ED_sum <<endl;
	}
	mul_mean = mul_sum/NShower;
	mul_VEM_mean = mul_VEM_sum/NShower;
	mul_ED_mean = mul_ED_sum/NShower;
	cout << "--> " << mul_mean << " " << mul_VEM_mean << " " << mul_ED_mean << endl;
	
	ofstream fichier("efficencyGamma.txt",ios::out | ios::app);
	if(fichier)
	  {
	    fichier << energy_temp << " " << theta_temp << " " << mul_mean << " " << mul_VEM_mean << " " << mul_ED_mean << endl;
	    //   cout << 
	    fichier.close();
	  }

	//cout << "fin : " << mul_sum << " " << NShower << " " << mul_mean << " " << ven[1] << " " << vth[1] << endl;
	vmul.clear();
	vmul_VEM.clear();
	vmul_ED.clear();
	cout << "apres clean vector, rempli avec : " << mul << " " <<  mul_VEM <<  " " <<  mul_ED << endl;
	vmul.push_back(mul);
	vmul_VEM.push_back(mul_VEM);
	vmul_ED.push_back(mul_ED);
	energy_temp = energy; 
	theta_temp = theta;
	NShower = 1.;
	mul_sum = 0.; mul_mean = 0.;
	mul_VEM_sum = 0.; mul_VEM_mean = 0.;
	mul_ED_sum; mul_ED_mean = 0.;
	cout << "et il a compris  : " << vmul[0] << " " <<  vmul_VEM[0] <<  " " <<  vmul_ED[0] << endl;
	
      }
  }
}



void Analyze::DrawEfficiency()
{
  // Déclaration de variables :
  int energy, line = 0;
  double theta, mul, mul_VEM, mul_ED;
  int NbinsX = 100, Xinf=2, Xsup=7, NbinsY = 60, Yinf = 0, Ysup=0.6;
  vector<int> vmul, vmul_ED;
  
  // Déclaration d'histogramme :
  TH2F *hist = new TH2F("hist","(E,theta)=f(Nmu > 3); Energy [GeV]; sin²(theta)",NbinsX,Xinf,Xsup,NbinsY,Yinf,Ysup);
  TH2F *hist_VEM = new TH2F("hist_VEM","(E,theta)=f(Signal > 3 VEM); Energy [GeV]; sin²(theta)",NbinsX,Xinf,Xsup,NbinsY,Yinf,Ysup);
  TH2F *hist_ED = new TH2F("hist_ED","(E,theta)=f(Npe > 18); Energy [GeV]; sin²(theta)",NbinsX,Xinf,Xsup,NbinsY,Yinf,Ysup);
    TH1F *MDED = new TH1F("MDED","Nb tank = f(Nb scintillator)",500,0,5000);

  ifstream in;
  in.open("efficencyGamma.txt",ios::in);
  while(!in.eof()) {
    in >> energy >> theta >> mul >> mul_VEM >> mul_ED;
    //cout << energy << " " << theta << " " << mul << " "<< mul_VEM << endl;
    if(in.eof()) break;
    
    hist->Fill(log10(energy),theta,mul);
    hist_VEM->Fill(log10(energy),theta,mul_VEM);
    hist_ED->Fill(log10(energy),theta,mul_ED);
    cout << energy << " " << theta << " " << mul << " "<< mul_VEM << " " << mul_ED << endl;
    vmul.push_back(mul);  vmul_ED.push_back(mul_ED);
    //MDED->Fill(mul,mul_ED);
    //cout << energy_eff << " " << log10(energy_eff) << endl;
  }

  
  // Draw plots :
  TCanvas *c1 = new TCanvas("c1");
  c1->cd(1);
  hist->Draw("zcol");

  TCanvas *cVEM = new TCanvas("cVEM");
  cVEM->cd(1);
  hist_VEM->Draw("zcol");
  
  TCanvas *cED = new TCanvas("cED");
  cED->cd(1);
  hist_ED->Draw("zcol");

  // int size = vmul.size();
  // TGraph *gr = new TGraph(size,&vmul_ED[0],&vmul[0]);
  // TCanvas *cgr = new TCanvas("cgr");
  // cgr->cd(1);
  // gr->Draw("AP");

}


void Analyze::DrawOneEfficiency()
{
  // Déclaration de variables :
  int energy, line = 0;
  double theta, mul, mul_VEM, mul_ED;
  int NbinsX = 100, Xinf=2, Xsup=7, NbinsY = 60, Yinf = 0, Ysup=0.6;
  int Nlines = 60;
  
  
  // Déclaration d'histogramme :
  TH1F * h0 = new TH1F("h0","Trigger > 1 mu -MD-; Energy [log(GeV)]; multiplicity [Nb tank]",500,2,7); 
  TH1F * h18 = new TH1F("h18","Theta 18; Energy [log(GeV)]; multiplicity [Nb tank]",500,2,7); 
  TH1F * h26 = new TH1F("h26","Theta 26; Energy [log(GeV)]; multiplicity [Nb tank]",500,2,7); 
  TH1F * h33 = new TH1F("h33","Theta 33; Energy [log(GeV)]; multiplicity [Nb tank]",500,2,7); 
  TH1F * h39 = new TH1F("h39","Theta 39; Energy [log(GeV)]; multiplicity [Nb tank]",500,2,7); 
  TH1F * h45 = new TH1F("h45","Theta 45; Energy [log(GeV)]; multiplicity [Nb tank]",500,2,7); 

  TH1F * h0_VEM = new TH1F("h0_VEM","Trigger > 1 VEM -MD-; Energy [log(GeV)]; multiplicity [Nb tank]",500,2,7); 
  TH1F * h18_VEM = new TH1F("h18_VEM","Theta 18; Energy [log(GeV)]; multiplicity [Nb tank]",500,2,7); 
  TH1F * h26_VEM = new TH1F("h26_VEM","Theta 26; Energy [log(GeV)]; multiplicity [Nb tank]",500,2,7); 
  TH1F * h33_VEM = new TH1F("h33_VEM","Theta 33; Energy [log(GeV)]; multiplicity [Nb tank]",500,2,7); 
  TH1F * h39_VEM = new TH1F("h39_VEM","Theta 39; Energy [log(GeV)]; multiplicity [Nb tank]",500,2,7); 
  TH1F * h45_VEM = new TH1F("h45_VEM","Theta 45; Energy [log(GeV)]; multiplicity [Nb tank]",500,2,7); 

  TH1F * h0_ED = new TH1F("h0_ED","Trigger > 18 pe -ED-; Energy [log(GeV)]; multiplicity [Nb scintillator]",500,2,7); 
  TH1F * h18_ED = new TH1F("h18_ED","Theta 18; Energy [log(GeV)]; multiplicity [Nb scintillator]",500,2,7); 
  TH1F * h26_ED = new TH1F("h26_ED","Theta 26; Energy [log(GeV)]; multiplicity [Nb scintillator]",500,2,7); 
  TH1F * h33_ED = new TH1F("h33_ED","Theta 33; Energy [log(GeV)]; multiplicity [Nb scintillator]",500,2,7); 
  TH1F * h39_ED = new TH1F("h39_ED","Theta 39; Energy [log(GeV)]; multiplicity [Nb scintillator]",500,2,7); 
  TH1F * h45_ED = new TH1F("h45_ED","Theta 45; Energy [log(GeV)]; multiplicity [Nb scintillator]",500,2,7); 


  // Déclaration fit :
  TF1 *fit0= new TF1("fit0","expo"); 
  TF1 *fit18= new TF1("fit18","expo"); 
  TF1 *fit26= new TF1("fit26","expo"); 
  TF1 *fit33= new TF1("fit33","expo"); 
  TF1 *fit39= new TF1("fit39","expo"); 
  TF1 *fit45= new TF1("fit45","expo"); 



  ifstream fichier("efficencyGamma.txt",ios::in);
  if(fichier)
    {
      while(line < Nlines)
	{
	  fichier >> energy >> theta >> mul >> mul_VEM >> mul_ED;
	  //cout << energy << " " << theta << " " << mul << " "<< mul_VEM << endl;
	  //cout << energy_eff << " " << log10(energy_eff) << endl;

	  if(theta == 0) {
	    h0->Fill(log10(energy), mul);
	    h0_VEM->Fill(log10(energy), mul_VEM); 
	    h0_ED->Fill(log10(energy), mul_ED); }
	  else if(theta == 0.1) {
	    h18->Fill(log10(energy), mul);
	    h18_VEM->Fill(log10(energy), mul_VEM); 
	    h18_ED->Fill(log10(energy), mul_ED); }
	  else if(theta == 0.2) {
	    h26->Fill(log10(energy), mul);
	    h26_VEM->Fill(log10(energy), mul_VEM);
	    h26_ED->Fill(log10(energy), mul_ED); }
	  else if(theta == 0.3) { 
	    h33->Fill(log10(energy), mul);
	    h33_VEM->Fill(log10(energy), mul_VEM);
	    h33_ED->Fill(log10(energy), mul_ED); }
	  else if(theta == 0.4) {
	    h39->Fill(log10(energy), mul);
	    h39_VEM->Fill(log10(energy), mul_VEM);
	    h39_ED->Fill(log10(energy), mul_ED);  }
	  else if(theta == 0.5) {
	    h45->Fill(log10(energy), mul);
	    h45_VEM->Fill(log10(energy), mul_VEM);
	    h45_ED->Fill(log10(energy), mul_ED); }
	  line++;
	}
    }
  
  // Draw plots :
  TCanvas *c1 = new TCanvas("ED");
  c1->Divide(3,2);
  c1->cd(1);
  h0_ED->SetMarkerColor(1);
  h0_ED->SetMarkerStyle(28);
  h0_ED->Fit("expo","","",3,6.1);
  h0_ED->Draw("p");
  c1->cd(2);
  h18_ED->SetMarkerColor(1);
  h18_ED->SetMarkerStyle(28);
  h18_ED->Fit("expo","","", 3,6.1);
  h18_ED->Draw("p");
  c1->cd(3);
  h26_ED->SetMarkerColor(1);
  h26_ED->SetMarkerStyle(28);
  h26_ED->Fit("expo","","",3,6.1);
  h26_ED->Draw("p");
  c1->cd(4);
  h33_ED->SetMarkerColor(1);
  h33_ED->SetMarkerStyle(28);
  h33_ED->Fit("expo","","",3,6.1);
  h33_ED->Draw("p");
  c1->cd(5);
  h39_ED->SetMarkerColor(1);
  h39_ED->SetMarkerStyle(28);
  h39_ED->Fit("expo","","",3,6.1);
  h39_ED->Draw("p");
  c1->cd(6);
  h45_ED->SetMarkerColor(1);
  h45_ED->SetMarkerStyle(28);
  h45_ED->Fit("expo","","",3,6.1);
  h45_ED->Draw("p");
    




  TCanvas *c2 = new TCanvas("MD_nmu");
  c2->Divide(3,2);
  c2->cd(1);
  h0->SetMarkerColor(1);
  h0->SetMarkerStyle(28);
  h0->Fit("expo","","",3,6.1);
  h0->Draw("p");
  c2->cd(2);
  h18->SetMarkerColor(1);
  h18->SetMarkerStyle(28);
  h18->Fit("expo","","", 3,6.1);
  h18->Draw("p");
  c2->cd(3);
  h26->SetMarkerColor(1);
  h26->SetMarkerStyle(28);
  h26->Fit("expo","","",3,6.1);
  h26->Draw("p");
  c2->cd(4);
  h33->SetMarkerColor(1);
  h33->SetMarkerStyle(28);
  h33->Fit("expo","","",3,6.1);
  h33->Draw("p");
  c2->cd(5);
  h39->SetMarkerColor(1);
  h39->SetMarkerStyle(28);
  h39->Fit("expo","","",3,6.1);
  h39->Draw("p");
  c2->cd(6);
  h45->SetMarkerColor(1);
  h45->SetMarkerStyle(28);
  h45->Fit("expo","","",3,6.1);
  h45->Draw("p");

  TCanvas *c3 = new TCanvas("MD_Sig");
  c3->Divide(3,2);
  c3->cd(1);
  h0_VEM->SetMarkerColor(1);
  h0_VEM->SetMarkerStyle(28);
  h0_VEM->Fit("expo","","",3,6.1);
  h0_VEM->Draw("p");
  c3->cd(2);
  h18_VEM->SetMarkerColor(1);
  h18_VEM->SetMarkerStyle(28);
  h18_VEM->Fit("expo","","", 3,6.1);
  h18_VEM->Draw("p");
  c3->cd(3);
  h26_VEM->SetMarkerColor(1);
  h26_VEM->SetMarkerStyle(28);
  h26_VEM->Fit("expo","","",3,6.1);
  h26_VEM->Draw("p");
  c3->cd(4);
  h33_VEM->SetMarkerColor(1);
  h33_VEM->SetMarkerStyle(28);
  h33_VEM->Fit("expo","","",3,6.1);
  h33_VEM->Draw("p");
  c3->cd(5);
  h39_VEM->SetMarkerColor(1);
  h39_VEM->SetMarkerStyle(28);
  h39_VEM->Fit("expo","","",3,6.1);
  h39_VEM->Draw("p");
  c3->cd(6);
  h45_VEM->SetMarkerColor(1);
  h45_VEM->SetMarkerStyle(28);
  h45_VEM->Fit("expo","","",3,6.1);
  h45_VEM->Draw("p");

  TCanvas *c4 = new TCanvas("MD"); 
  c4->Divide(2,1);
  c4->cd(1);
  h0->SetMarkerColor(1);
  h0->SetMarkerStyle(28);
  fit0->SetLineColor(1);
  h0->Fit(fit0,"","",2.5,6.5);
  h18->SetMarkerColor(38);
  h18->SetMarkerStyle(28);
  fit18->SetLineColor(38);
  h18->Fit(fit18,"","",2.5,6.5);
  h26->SetMarkerColor(46);
  h26->SetMarkerStyle(28);
  fit26->SetLineColor(46);
  h26->Fit(fit26,"","",2.5,6.5);
  h33->SetMarkerColor(17);
  h33->SetMarkerStyle(28);
  fit33->SetLineColor(17);
  h33->Fit(fit33,"","",2.5,6.5);
  h39->SetMarkerColor(3);
  h39->SetMarkerStyle(28);
  fit39->SetLineColor(3);
  h39->Fit(fit39,"","",2.5,6.5);
  h45->SetMarkerColor(9);
  h45->SetMarkerStyle(28);
  fit45->SetLineColor(9);
  h45->Fit(fit45,"","",2.5,6.5);

  h0->Draw("p");
  fit0->Draw("same");
  h18->Draw("psame");
  fit18->Draw("same");
  h26->Draw("psame");
  fit26->Draw("same");
  h33->Draw("psame");
  fit33->Draw("same"); 
  h39->Draw("psame");
  fit39->Draw("same");
  h45->Draw("psame");
  fit45->Draw("same");
  
  TLegend *leg = new TLegend(0.1,0.7,0.48,0.9);
  leg->SetHeader("For different theta");
  leg->AddEntry(fit0,"theta = 0");
  leg->AddEntry(fit18,"theta = 18");
  leg->AddEntry(fit26,"theta = 26");
  leg->AddEntry(fit33,"theta = 33");
  leg->AddEntry(fit39,"theta = 39");
  leg->AddEntry(fit45,"theta = 45");
  leg->Draw();
  
  c4->cd(2);
  h0_ED->SetMarkerColor(1);
  h0_ED->SetMarkerStyle(28);
  fit0->SetLineColor(1);
  h0_ED->Fit(fit0,"","",2.5,6.5);
  h18_ED->SetMarkerColor(38);
  h18_ED->SetMarkerStyle(28);
  fit18->SetLineColor(38);
  h18_ED->Fit(fit18,"","",2.5,6.5);
  h26_ED->SetMarkerColor(46);
  h26_ED->SetMarkerStyle(28);
  fit26->SetLineColor(46);
  h26_ED->Fit(fit26,"","",2.5,6.5);
  h33_ED->SetMarkerColor(17);
  h33_ED->SetMarkerStyle(28);
  fit33->SetLineColor(17);
  h33_ED->Fit(fit33,"","",2.5,6.5);
  h39_ED->SetMarkerColor(3);
  h39_ED->SetMarkerStyle(28);
  fit39->SetLineColor(3);
  h39_ED->Fit(fit39,"","",2.5,6.5);
  h45_ED->SetMarkerColor(9);
  h45_ED->SetMarkerStyle(28);
  fit45->SetLineColor(9);
  h45_ED->Fit(fit45,"","",2.5,6.5);

  h0_ED->Draw("p");
  fit0->Draw("same");
  h18_ED->Draw("psame");
  fit18->Draw("same");
  h26_ED->Draw("psame");
  fit26->Draw("same");
  h33_ED->Draw("psame");
  fit33->Draw("same");
  h39_ED->Draw("psame");
  fit39->Draw("same");
  h45_ED->Draw("psame");
  fit45->Draw("same");

  TLegend *leg2 = new TLegend(0.1,0.7,0.48,0.9);
  leg2->SetHeader("For different theta");
  leg2->AddEntry(fit0,"theta = 0");
  leg2->AddEntry(fit18,"theta = 18");
  leg2->AddEntry(fit26,"theta = 26");
  leg2->AddEntry(fit33,"theta = 33");
  leg2->AddEntry(fit39,"theta = 39");
  leg2->AddEntry(fit45,"theta = 45");
  leg2->Draw();
}



// void Analyze::DrawLDF_MD()
//  {
//    TH1F* histo;
//    char*hname = new char[15];

//    if(fTreeEvent!=0 || fTreeMD!= 0)
//      { 
//        delete fTreeEvent; delete fTreeMD;
//       fTreeEvent = 0; fTreeMD=0;
//      }
//    char name[500];
//    char size[50];
//    int compteur = 0;
//    vector<double> vsig0, vsig1, vsig2, vsig3, vsig4, vsig5;
//    vector<double> vdist0, vdist1, vdist2, vdist3, vdist4, vdist5;
   
//    ifstream fichier("ListFile_1E6_proton.txt", ios::in);
//    if(fichier) 
//     {
//       while(fichier.good())
// 	{
// 	  if(compteur==1) continue;
// 	  fichier >> name;
// 	  cout << name << endl;
// 	  //fFile = TFile::Open(name);//new TFile(name,"READ");
// 	  gROOT->Clear();
// 	  fFile = new TFile(name,"READ");	  

// 	  // // Lecture de l'arbre contenant le détecteur MD :
// 	  fTreeMD = (TTree *) fFile->Get("TreeMD");   // Get Tree 
// 	  if(fBranchMD!=0)
// 	     { 
// 	       delete fBranchMD;
// 	       fBranchMD = 0;
// 	     }
// 	  fBranchMD = fTreeMD->GetBranch("HitStationMD");
// 	  fTreeMD->SetBranchAddress("hstationMD",&fHitStationMD);
	  
// 	  // // Lecture de l'arbre contenant les infos événement :
// 	  fTreeEvent = (TTree *) fFile->Get("TreeEvent");  // Get Tree
// 	  //	fTreeEvent->Print(); 
// 	  if(fBranchEvent!=0)
// 	    { 
// 	      delete fBranchEvent;
// 	      fBranchEvent = 0;
// 	    }
// 	  fBranchEvent = fTreeEvent->GetBranch("Event");
// 	  fTreeEvent->SetBranchAddress("event",&fEvent);  
// 	  fTreeEvent->GetEvent(0);
// 	  // cout << "E = " << fEvent->fEnergy << " & th = " <<  fEvent->fTheta*180/3.1416 << endl;
// 	  // if(fEvent->fEnergy != 1000) continue;
	  
// 	   // Boucle sur stations : 
// 	  int nev = (int) fTreeMD->GetEntries();
// 	  cout << "nb stat " << nev << endl;
// 	  for(int i=0; i<nev; i++) {
// 	    fTreeMD->GetEvent(i);
	    
// 	    // if(fEvent->fEnergy != 1000) continue;
	    
// 	    if(GetMDChargeInVEM(i) < 3)  continue;
// 	    cout << "Sta " << i << " & ChargeVEM = " << GetMDChargeInVEM(i) << endl;
// 	    if(fEvent->fTheta*180/3.1416 == 0) {
// 	      sprintf(hname,"LDF_1E6_th0_%1d",compteur);
// 	      vsig0.push_back(GetMDChargeInVEM(i)); vdist0.push_back(fHitStationMD->fR_sf);}
// 	       // else if(fEvent->fTheta*180/3.1416 > 17. && fEvent->fTheta*180/3.1416 > 19.) {
// 	       // 	vsig1.push_back(GetMDChargeInVEM(i)); vdist1.push_back(fHitStationMD->fR_sf);}
// 	       // else if(fEvent->fTheta*180/3.1416 > 23. && fEvent->fTheta*180/3.1416 > 27.) {
// 	       // 	vsig2.push_back(GetMDChargeInVEM(i)); vdist2.push_back(fHitStationMD->fR_sf);}
// 	       // else if(fEvent->fTheta*180/3.1416 > 32. && fEvent->fTheta*180/3.1416 > 34.) {
// 	       // 	vsig3.push_back(GetMDChargeInVEM(i)); vdist3.push_back(fHitStationMD->fR_sf);}
// 	       // else if(fEvent->fTheta*180/3.1416 > 38. && fEvent->fTheta*180/3.1416 > 40.) {
// 	       // 	vsig4.push_back(GetMDChargeInVEM(i)); vdist4.push_back(fHitStationMD->fR_sf);}
// 	       // else if(fEvent->fTheta*180/3.1416 > 44. && fEvent->fTheta*180/3.1416 > 46.) {
// 	       // 	vsig5.push_back(GetMDChargeInVEM(i)); vdist5.push_back(fHitStationMD->fR_sf);}
	  
// 	  } // boucles sur station
// 	  compteur++;

// 	  fFile->Close();
// 	  //delete fEvent;
// 	  // delete fTreeEvent; delete fTreeMD;
// 	  //delete fFile2;
// 	} // fin boucle fichier ListGoodFile.txt (evt)
//     } // fin fichier   
  

//   int size0 = vsig0.size(), size1 = vsig1.size(), size2 = vsig2.size(), size3 = vsig3.size(), size4 = vsig4.size(), size5 = vsig5.size();
  
//   TGraph *gr0 = new TGraph(size0,&vdist0[0],&vsig0[0]);  gr0->SetTitle("th = 0"); gr0->SetMarkerStyle(7); gr0->SetMarkerColor(2);
//   TGraph *gr1 = new TGraph(size1,&vdist1[0],&vsig1[0]);  gr1->SetTitle("th = 18"); gr1->SetMarkerStyle(7); gr1->SetMarkerColor(2);
//   TGraph *gr2 = new TGraph(size2,&vdist2[0],&vsig2[0]);  gr2->SetTitle("th = 26"); gr2->SetMarkerStyle(7); gr2->SetMarkerColor(2);
//   TGraph *gr3 = new TGraph(size3,&vdist3[0],&vsig3[0]);  gr3->SetTitle("th = 33"); gr3->SetMarkerStyle(7); gr3->SetMarkerColor(2);
//   TGraph *gr4 = new TGraph(size4,&vdist4[0],&vsig4[0]);  gr4->SetTitle("th = 39"); gr4->SetMarkerStyle(7); gr4->SetMarkerColor(2);
//   TGraph *gr5 = new TGraph(size5,&vdist5[0],&vsig5[0]);  gr5->SetTitle("th = 45"); gr5->SetMarkerStyle(7); gr5->SetMarkerColor(2);

//   TCanvas *c = new TCanvas("c");
//   c->Divide(3,2);
//   c->cd(1); gr0->Draw("AP"); c->cd(2); gr1->Draw("AP"); c->cd(3); gr2->Draw("AP"); c->cd(4); gr3->Draw("AP"); c->cd(5); gr4->Draw("AP");  c->cd(6); gr5->Draw("AP");

//  }



// void Analyze::DrawLDF_ED()
//  {
//    if(fTreeEvent!=0 || fTreeED!= 0)
//     { 
//       delete fTreeEvent; delete fTreeED;
//       fTreeEvent = 0; fTreeED=0;
//     }
//   char name[500];
//   char size[50];
//   int compteur = 0;
//   vector<double> vsig0, vsig1, vsig2, vsig3, vsig4, vsig5;
//   vector<double> vdist0, vdist1, vdist2, vdist3, vdist4, vdist5;
  
//   ifstream fichier("ListGoodFile.txt", ios::in);
//   if(fichier) 
//     {
//       while(fichier.good())
// 	{
// 	  fichier >> name;
// 	  fFile = new TFile(name,"READ");
// 	  // Lecture de l'arbre contenant le détecteur ED :
// 	  fTreeED = (TTree *) fFile->Get("TreeED");   // Get Tree 
// 	  if(fBranchED!=0)
// 	    { 
// 	      delete fBranchED;
// 	      fBranchED = 0;
// 	    }
// 	  fBranchED = fTreeED->GetBranch("HitStationED");
// 	  fTreeED->SetBranchAddress("hstationED",&fHitStationED);
	  
// 	  // Lecture de l'arbre contenant les infos événement :
// 	  fTreeEvent = (TTree *) fFile->Get("TreeEvent");  // Get Tree
// 	  //	fTreeEvent->Print(); 
// 	  if(fBranchEvent!=0)
// 	    { 
// 	      delete fBranchEvent;
// 	      fBranchEvent = 0;
// 	    }
// 	  fBranchEvent = fTreeEvent->GetBranch("Event");
// 	  fTreeEvent->SetBranchAddress("event",&fEvent);  
// 	  fTreeEvent->GetEvent(0);

// 	  // Boucle sur stations : 
	  
// 	  int nev=fTreeED->GetEntries();
// 	  for (int i=0; i<nev; i++) {
// 	    fTreeED->GetEvent(i);
// 	    if(fEvent->fEnergy != 1000) continue;
// 	    if(fHitStationED->fNpe >= 18) {
// 	      if(fEvent->fTheta*180/3.1416 == 0) {
// 		vsig0.push_back(fHitStationED->fNpe); vdist0.push_back(fHitStationED->fR_sf);}
// 	      else if(fEvent->fTheta*180/3.1416 > 17. && fEvent->fTheta*180/3.1416 > 19.) {
// 		vsig1.push_back(fHitStationED->fNpe); vdist1.push_back(fHitStationED->fR_sf);}
// 	      else if(fEvent->fTheta*180/3.1416 > 23. && fEvent->fTheta*180/3.1416 > 27.) {
// 		vsig2.push_back(fHitStationED->fNpe); vdist2.push_back(fHitStationED->fR_sf);}
// 	      else if(fEvent->fTheta*180/3.1416 > 32. && fEvent->fTheta*180/3.1416 > 34.) {
// 		vsig3.push_back(fHitStationED->fNpe); vdist3.push_back(fHitStationED->fR_sf);}
// 	      else if(fEvent->fTheta*180/3.1416 > 38. && fEvent->fTheta*180/3.1416 > 40.) {
// 		vsig4.push_back(fHitStationED->fNpe); vdist4.push_back(fHitStationED->fR_sf);}
// 	      else if(fEvent->fTheta*180/3.1416 > 44. && fEvent->fTheta*180/3.1416 > 46.) {
// 		vsig5.push_back(fHitStationED->fNpe); vdist5.push_back(fHitStationED->fR_sf);}
// 	    } // fin condition trigger cuves
// 	  } // boucles sur station

// 	} // fin boucle fichier ListGoodFile.txt
//     } // fin fichier   
  

//   int size0 = vsig0.size(), size1 = vsig1.size(), size2 = vsig2.size(), size3 = vsig3.size(), size4 = vsig4.size(), size5 = vsig5.size();

//   TGraph *gr0 = new TGraph(size0,&vdist0[0],&vsig0[0]);  gr0->SetTitle("th = 0"); gr0->SetMarkerStyle(7); gr0->SetMarkerColor(2);
//   TGraph *gr1 = new TGraph(size1,&vdist1[0],&vsig1[0]);  gr1->SetTitle("th = 18"); gr1->SetMarkerStyle(7); gr1->SetMarkerColor(2);
//   TGraph *gr2 = new TGraph(size2,&vdist2[0],&vsig2[0]);  gr2->SetTitle("th = 26"); gr2->SetMarkerStyle(7); gr2->SetMarkerColor(2);
//   TGraph *gr3 = new TGraph(size3,&vdist3[0],&vsig3[0]);  gr3->SetTitle("th = 33"); gr3->SetMarkerStyle(7); gr3->SetMarkerColor(2);
//   TGraph *gr4 = new TGraph(size4,&vdist4[0],&vsig4[0]);  gr4->SetTitle("th = 39"); gr4->SetMarkerStyle(7); gr4->SetMarkerColor(2);
//   TGraph *gr5 = new TGraph(size5,&vdist5[0],&vsig5[0]);  gr5->SetTitle("th = 45"); gr5->SetMarkerStyle(7); gr5->SetMarkerColor(2);

//   TCanvas *c = new TCanvas("c");
//   c->Divide(5,2);
//   c->cd(1); gr0->Draw("AP"); c->cd(2); gr1->Draw("AP"); c->cd(3); gr2->Draw("AP"); c->cd(4); gr3->Draw("AP"); c->cd(5); gr4->Draw("AP");  c->cd(6); gr5->Draw("AP");

//  }






///////////// FONCTIONS SI UTILISE ReadFile("nom_fichier.txt") ////////

void Analyze::DeleteFile()
 {
   char cmd[255];
   sprintf(cmd, "rm -f res.txt ListGoodFile.txt ListEfficency.txt");
   system(cmd); 
 }

void Analyze::GetMD()
{
  int tank;
  for (int i=0; i <= fTreeMD->GetEntries(); i++)
    {
      tank = i;
      fTreeMD->GetEvent(tank);
      
      if(fHitStationMD->fNpe > 0) cout << i << " \t tank " << fHitStationMD->fId << "\t pel = "<<fHitStationMD->fNpe << "\t nmu = " << fHitStationMD->fNmu << "\t nel = " << fHitStationMD->fNel << "\t nph = " << fHitStationMD->fNph << "\t dist = " << fHitStationMD->fR_sf << "\t samp = " << fHitStationMD->fSampFact << "\t time = " << fHitStationMD->fTime0 << "\t pemu = " << fHitStationMD->fNpe_mu << "\t peel = " << fHitStationMD->fNpe_el << "\t peph = " << fHitStationMD->fNpe_ph << endl; 
    }
}


void Analyze::GetED()
{
  int scint;
  
  // Affichage détaillé des stations, particules, pe, ...
  for (int i=0; i <= fTreeED->GetEntries(); i++)
    {
      scint = i;
      fTreeED->GetEvent(scint);
      if(fHitStationED->fNpe != 0)
	cout << i << "\t scint " << fHitStationED->fId << "\t pel = " << fHitStationED->fNpe << "\t nmu = " << fHitStationED->fNmu << "\t nel = " << fHitStationED->fNel << "\t nph = " << fHitStationED->fNph << "\t dist = " << fHitStationED->fR_sf << "\t samp = " << fHitStationED->fSampFact << "\t time = " << fHitStationED->fTime0 << "\t pemu = " << fHitStationED->fNpe_mu << "\t peel = " << fHitStationED->fNpe_el << "\t peph = " << fHitStationED->fNpe_ph << endl ; 
    }
}







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////
// Fonction à partir de AnaEvent.cc : //
////////////////////////////////////////

void Analyze::TestEfficiencyVEM(float th)
{
  int theta0[6] = {0,18,25,33,39,45};
  double  energy, mul;
  int theta;
  TFile* infile;
  TTree* tree;
  TBranch* branch;
  EventRec* event;
  map<float,int>::iterator iter;
  char* hname;
  TProfile* hmul[6];
  gStyle->SetOptStat(0);
  
  for(int i=0; i<6; i++)
    {
      hname = new char[15];
      sprintf(hname,"mul_%d",theta0[i]);
      hmul[i]= new TProfile(hname,hname,60,0,6);
      delete hname;
      hmul[i]->SetMarkerStyle(20);
      hmul[i]->SetMarkerColor(2);
      hmul[i]->SetLineColor(2);
    }
  infile = TFile::Open("proton.root");
  
  tree = (TTree *) infile->Get("TreeRec");
  tree->SetBranchAddress("eventrec",&event);
  int nev = tree->GetEntries();
  cout << nev << endl;
  tree->GetEvent(0);
  iter=event->fNTrigMDStaVEM.find(th);
  if(iter == event->fNTrigMDStaVEM.end())
    {
      cout << "le seuil n'existe pas " << endl;
      exit(1);
    }
  
  for (int i=0; i<nev; i++)
    {
      tree->GetEvent(i);
      theta =  event->fTheta * RAD2DEG;
      energy = event->fEnergy;
      mul = event->fNTrigMDStaVEM[th];
      cout<<mul<<" "<<theta<<" "<<energy<<endl;
      for(int j=0; j<6; j++)
	{
	  if(theta == theta0[j])
	    hmul[j]->Fill(log10(energy),mul);
	}
    }
  TCanvas *c1 = new TCanvas("c1");
  
  c1->Divide(3,2);
  c1->cd(1);
  hmul[0]->Draw();
  c1->cd(2);
  hmul[1]->Draw();
  c1->cd(3);
  hmul[2]->Draw();
  c1->cd(4);
  hmul[3]->Draw();
  c1->cd(5);
  hmul[4]->Draw();
  c1->cd(6);
  hmul[5]->Draw();
}


void Analyze::TestEfficiencyMu(int th)
{
  int theta0[6] = {0,18,25,33,39,45};
  double  energy, mul;
  int theta;
  TFile* infile;
  TTree* tree;
  TBranch* branch;
  EventRec* event;
  map<int,int>::iterator iter;
  char* hname;
  TProfile* hmul[6];
  gStyle->SetOptStat(0);
  
  for(int i=0; i<6; i++)
    {
      hname = new char[15];
      sprintf(hname,"mul_%d",theta0[i]);
      hmul[i]= new TProfile(hname,hname,60,0,6);
      delete hname;
      hmul[i]->SetMarkerStyle(20);
      hmul[i]->SetMarkerColor(2);
      hmul[i]->SetLineColor(2);
    }
  infile = TFile::Open("proton.root");
  
  tree = (TTree *) infile->Get("TreeRec");
  tree->SetBranchAddress("eventrec",&event);
  int nev = tree->GetEntries();
  cout << nev << endl;
  tree->GetEvent(0);
  iter=event->fNTrigMDStaMu.find(th);
  if(iter == event->fNTrigMDStaMu.end())
    {
      cout << "le seuil n'existe pas " << endl;
      exit(1);
    }
  
  for (int i=0; i<nev; i++)
    {
      tree->GetEvent(i);
      theta =  event->fTheta * RAD2DEG;
      energy = event->fEnergy;
      mul = event->fNTrigMDStaMu[th];
      cout<<mul<<" "<<theta<<" "<<energy<<endl;
      for(int j=0; j<6; j++)
	{
	  if(theta == theta0[j])
	    hmul[j]->Fill(log10(energy),mul); 
	}
    }
  TCanvas *c1 = new TCanvas("c1");
  
  c1->Divide(3,2);
  c1->cd(1);
  hmul[0]->Draw();
  c1->cd(2);
  hmul[1]->Draw();
  c1->cd(3);
  hmul[2]->Draw();
  c1->cd(4);
  hmul[3]->Draw();
  c1->cd(5);
  hmul[4]->Draw();
  c1->cd(6);
  hmul[5]->Draw();
}









void Analyze::TestEfficiencyED()
{
  int theta0[6] = {0,18,25,33,39,45};
  double energy, NTrig20; 
  int mul,th_temp; 
  double num, Nb_it;
  double en_temp;
  num = 0.;
  Nb_it = 0.;
  NTrig20 = 0.;
  int theta;
  TFile* infile;
  TTree* tree;
  TBranch* branch;
  EventRec* event;
  char* hname;
  char* hname2;
  TProfile* hmul[6];
  TProfile* hTrig[6];
  gStyle->SetOptStat(0);


  for(int i=0; i<6; i++)
    {
      hname = new char[15];
      sprintf(hname,"mul_%d",theta0[i]);
      hmul[i]= new TProfile(hname,hname,60,0,6);
      delete hname;
      hmul[i]->SetMarkerStyle(20);
      hmul[i]->SetMarkerColor(2);
      hmul[i]->SetLineColor(2);

      hname2 = new char[15];
      sprintf(hname2,"NTrig_%d",theta0[i]);
      hTrig[i]= new TProfile(hname2,hname2,60,0,6);
      delete hname2;
      hTrig[i]->SetMarkerStyle(20);
      hTrig[i]->SetMarkerColor(2);
      hTrig[i]->SetLineColor(2);
    }
  infile = TFile::Open("test.root");
  
  tree = (TTree *) infile->Get("TreeRec");
  tree->SetBranchAddress("eventrec",&event);
  int nev = tree->GetEntries();
  cout << nev << endl;
  tree->GetEvent(0);
 
  for (int i=0; i<nev; i++)
    {
      tree->GetEvent(i);
      theta = event->fTheta * RAD2DEG;
      energy = event->fEnergy;
      mul = event->fHitStationLightListED.size();
      cout  << mul << " " << theta << " " << energy << endl;

      for(int j=0; j<6; j++)
	{
	  if(theta == theta0[j]) {
	    if(i == 0) th_temp = j;
	    if(th_temp == j) 
	      {
	    	Nb_it++;
	    	if(mul>19) num++;
	      } 
	    else 
	      {
	    	NTrig20 = num/Nb_it;
		hTrig[j]->Fill(log10(energy),NTrig20);
		cout << log10(energy) << " " << NTrig20 << endl;
	    	if(mul > 19) 
	    	  num =1.;
	    	else 
	    	  num =0.;
	    	Nb_it =1.;
		//NTrig20 = 0;
	      }
	    hmul[j]->Fill(log10(energy),mul); 
	    //hTrig[j]->Fill(log10(energy),NTrig20);
	    //cout << log10(energy) << " " << NTrig20 << endl;
	    th_temp = j; 
	    //cout << j << " " << NTrig20 << endl;
	  }
	}
    }
  TCanvas *c1 = new TCanvas("c1");
      
  c1->Divide(3,2);
  c1->cd(1);
  hmul[0]->Draw();
  hmul[0]->GetXaxis()->SetTitle("log(Energy)  [GeV]");
  hmul[0]->GetYaxis()->SetTitle("N scintillators");
  hmul[0]->Fit("expo","","",4,6.2);
  hmul[0]->SetLineColor(1);
  hTrig[0]->Draw();
  c1->cd(2);
  hmul[1]->Draw();
  hmul[1]->GetXaxis()->SetTitle("log(Energy)  [GeV]");
  hmul[1]->GetYaxis()->SetTitle("N scintillators");
  hmul[1]->Fit("expo","","",4,6.2);
  hmul[1]->SetLineColor(1);
  hTrig[1]->Draw();
  c1->cd(3);
  hmul[2]->Draw();
  hmul[2]->GetXaxis()->SetTitle("log(Energy)  [GeV]");
  hmul[2]->GetYaxis()->SetTitle("N scintillators");
  hmul[2]->Fit("expo","","",4,6.2);
  hmul[2]->SetLineColor(1);
  hTrig[2]->Draw();
  c1->cd(4);
  hmul[3]->Draw();
  hmul[3]->GetXaxis()->SetTitle("log(Energy)  [GeV]");
  hmul[3]->GetYaxis()->SetTitle("N scintillators");
  hmul[3]->Fit("expo","","",4,6.2);
  hmul[3]->SetLineColor(1);
  hTrig[3]->Draw();
  c1->cd(5);
  hmul[4]->Draw();
  hmul[4]->GetXaxis()->SetTitle("log(Energy)  [GeV]");
  hmul[4]->GetYaxis()->SetTitle("N scintillators");
  hmul[4]->Fit("expo","","",4,6.2);
  hmul[4]->SetLineColor(1);
  hTrig[4]->Draw();
  c1->cd(6);
  hmul[5]->Draw();
  hmul[5]->GetXaxis()->SetTitle("log(Energy)  [GeV]");
  hmul[5]->GetYaxis()->SetTitle("N scintillators");
  hmul[5]->Fit("expo","","",4,6.2);
  hmul[5]->SetLineColor(1);
  hTrig[5]->Draw();
  
}






void Analyze::NpeED()
{
   
   
 }




void Analyze::GetMDED()
{
  int tank, scint;

  // Détecteur MD :
  cout << "Réseau MD : \n" << endl;
  for (int i=0; i <= fTreeMD->GetEntries(); i++)
    {
      tank = i;
      fTreeMD->GetEvent(tank);
      if(fHitStationMD->fNpe > 0) {
 	cout << i << "\t tank " << fHitStationMD->fId << "  dist_Sf = " << fHitStationMD->fR_sf <<"\t petot = " << fHitStationMD->fNpe <<  "\t nmu = " << fHitStationMD->fNmu << "\t nel = " << fHitStationMD->fNel << "\t nph = " << fHitStationMD->fNph << endl; }
    }
  cout << " " << endl;
  
  // Détecteur ED :
  cout << "Réseau ED : \n" << endl;
  for (int i=0; i <= fTreeED->GetEntries(); i++)
    {
      scint = i;
      fTreeED->GetEvent(scint);
      if(fHitStationED->fNpe > 0) {
	//cout << fHitStationED->fNpe << endl;
	cout << i << "\t scint " << fHitStationED->fId << "  dist_Sf = " << fHitStationED->fR_sf << "\t petot = " << fHitStationED->fNpe <<  "\t nmu = " << fHitStationED->fNmu << "\t nel = " << fHitStationED->fNel << "\t nph = " << fHitStationED->fNph << "\t dist = " << endl;    
      }
    }

}


void Analyze::PrintTrees()
{  
  fTreeEvent->Print();
  fTreeMD->Print();
  fTreeED->Print();
}


void Analyze::ReadFile(char * filename)
{ 
  if(fTreeEvent!=0 || fTreeED!=0 || fTreeMD!= 0)
    { 
      delete fTreeEvent; delete fTreeED; delete fTreeMD;
      fTreeEvent = 0; fTreeED=0; fTreeMD=0;
    }

  // Lecture du fichier "Sim_DAT050300.root" :
  fFile = new TFile(filename,"READ");	// Open file	
  fFile->ls();
  

  // Lecture de l'arbre contenant les infos événement :
  fTreeEvent = (TTree *) fFile->Get("TreeEvent");  // Get Tree
  fTreeEvent->Print(); 
  if(fBranchEvent!=0)
    { 
      delete fBranchEvent;
      fBranchEvent = 0;
    }
  fBranchEvent = fTreeEvent->GetBranch("Event");
  fTreeEvent->SetBranchAddress("event",&fEvent);  
  cout << " \t * There is " << fTreeEvent->GetEntries() << " events in the input file (BranchEvent)"<<endl;
  

  // Lecture de l'arbre contenant le détecteur MD :
  fTreeMD = (TTree *) fFile->Get("TreeMD");   // Get Tree
  fTreeMD->Print(); 
  if(fBranchMD!=0)
    { 
      delete fBranchMD;
      fBranchMD = 0;
    }
  fBranchMD = fTreeMD->GetBranch("HitStationMD");
  fTreeMD->SetBranchAddress("hstationMD",&fHitStationMD);
  cout << " \t * There is " << fTreeMD->GetEntries() << " events in the input file (BranchMD)"<<endl;
  

  // Lecture de l'arbre contenant le détecteur ED :
  fTreeED = (TTree *) fFile->Get("TreeED");   // Get Tree
  fTreeED->Print(); 
  if(fBranchED!=0)
    { 
      delete fBranchED;
      fBranchED = 0;
    }
  fBranchED = fTreeED->GetBranch("HitStationED");
  fTreeED->SetBranchAddress("hstationED",&fHitStationED);  
  cout << " \t * There is " << fTreeED->GetEntries() << " events in the input file  (BranchED)"<<endl; 
  
  return;
}



///////////////////////////////////////////////////////////////////
/////////////////// LDF ///////////////////////////////////////
////////////////////////////////////////////////////////////

pair <float,float> Analyze::DrawLDF_MD()
{  
  int nevent,nmu;
  double dist, sig;

  TH1F *hLDF_Sig = new TH1F("hLDF_Sig","Signal=f(distance); Distance [m]; Signal [VEM]",500,0,500);
  TF1 *fit2 = new TF1("fit2","[0]*pow( ((x/15.) * (1+(x/15.))), [1])",8,500);
  
  nevent = (int)fTreeMD->GetEntries();
  
  for(int ne=0; ne<nevent; ne++)
    {
      fTreeMD->GetEvent(ne);
      // if(GetMDChargeInVEM(ne) < 2)  continue;
      sig = GetMDChargeInVEM(ne);
      nmu = fHitStationMD->fNmu;
      dist = fHitStationMD->fR_sf;
      if((dist >= 8) & (sig !=0) ) {
	hLDF_Sig->Fill(dist,sig); }
    } //end of loop en event number
  
  //TCanvas* c2 = new TCanvas;
  //c2->cd(1);
  hLDF_Sig->SetMarkerStyle(34);
  hLDF_Sig->SetMarkerColor(38);
  fit2->SetMarkerColor(2);
  hLDF_Sig->Draw("point");
  
  fit2->SetParameters(0,5); fit2->SetParameters(1,-2);
  hLDF_Sig->Fit("fit2");
  fit2->Draw("same");
  
  float ampMD = fit2->GetParameter(0);
  
  return make_pair( fit2->GetParameter(0), fit2->GetParameter(1) );
  // return ampMD;
}




pair <float,float> Analyze::DrawLDF_ED()
{
  int nevent, nPe;
  double dist;
    
  TH1F *hLDF_Npe = new TH1F("hLDF_Npe","Npe=f(distance); Distance [m]; Npe",500,0,500);
  TF1 *fit = new TF1("fit","[0]*pow( ((x/15.) * (1+(x/15.))), [1])",8,500);
 
  nevent=(int)fTreeED->GetEntries();
  
  for(int ne=0; ne<nevent; ne++)
    {
      fTreeED->GetEvent(ne);
      nPe = fHitStationED->fNpe;
      dist = fHitStationED->fR_sf;
      //  cout << nmu << "\t muons at " << dist << " m" << endl;
      
      if( (dist >= 8) & (nPe !=0) ) {
	hLDF_Npe->Fill(dist,nPe); }
	//	cout << dist << " " << nPe << endl; }
    }//end of loop en event number
  
  // Trace LDF evt appelé :
  //TCanvas* c1 = new TCanvas;
  //c1->cd(1);
  hLDF_Npe->SetMarkerStyle(34);
  hLDF_Npe->SetMarkerColor(38);
  fit->SetMarkerColor(2);
  hLDF_Npe->Draw("point");
  
  fit->SetParameters(0,5); fit->SetParameters(1,-2);
  hLDF_Npe->Fit("fit");
  fit->Draw("same");
  
  float ampED = fit->GetParameter(0);
  cout << fit->GetParameter(0) << " " << fit->GetParameter(1) << endl;

  // return ampED;
  return make_pair( fit->GetParameter(0), fit->GetParameter(1) );

}






void Analyze::ReadFileChoice(char * ener)
{ 

  pair<float, float> paramFitED;
  pair<float, float> paramFitMD;

  vector<float> ampED_th0, ampED_th18, ampED_th26, ampED_th33, ampED_th39, ampED_th45;
  vector<float> ampMD_th0, ampMD_th18, ampMD_th26, ampMD_th33, ampMD_th39, ampMD_th45;
 
  cout << "====> energy = " << ener << endl;
  // initialisation 3 arbres :
  if(fTreeEvent!=0 || fTreeED!=0 || fTreeMD!= 0)
    { 
      delete fTreeEvent; delete fTreeED; delete fTreeMD;
      fTreeEvent = 0; fTreeED=0; fTreeMD=0;
    }

  // Lecture du fichier "Sim_DAT050300.root" :
  char filename[200];
   int th[6] = {0,18,25,33,39,45};
  //int th[1] = {0};
   // ig : 0 -> 6   &   it : 0 -> 100 //
   for(int ig=0; ig <6; ig++) {
     for(int it = 1; it <= 100; it++) {
       int ttt = sprintf(filename, "/projet/auger/Diane/LHAASIM/LhaaSim/v0r2/data/LhaRootGammaRE/Sim_corsika_%s_%i_%i.root", ener, th[ig], it);
       //Sim_corsika_3E3_39_83.root
       //
       
       fFile = TFile::Open(filename);
       if(!fFile || fFile->IsZombie()) {
	 cout << "file not found or zombie" << endl; 
	 continue;
       }
      
      fFile = new TFile(filename,"READ");	// Open file	
      fFile->ls();
   
      
      // Lecture de l'arbre contenant les infos événement :
      fTreeEvent = (TTree *) fFile->Get("TreeEvent");  // Get Tree
      fTreeEvent->Print(); 
      if(fBranchEvent!=0)
	{ 
	  delete fBranchEvent;
	  fBranchEvent = 0;
	}
      fBranchEvent = fTreeEvent->GetBranch("Event");
      fTreeEvent->SetBranchAddress("event",&fEvent);  
      cout << " \t * There is " << fTreeEvent->GetEntries() << " events in the input file (BranchEvent)"<<endl;
      
      
      // Lecture de l'arbre contenant le détecteur MD :
      fTreeMD = (TTree *) fFile->Get("TreeMD");   // Get Tree
      fTreeMD->Print(); 
      if(fBranchMD!=0)
	{ 
	  delete fBranchMD;
	  fBranchMD = 0;
	}
      fBranchMD = fTreeMD->GetBranch("HitStationMD");
      fTreeMD->SetBranchAddress("hstationMD",&fHitStationMD);
      cout << " \t * There is " << fTreeMD->GetEntries() << " events in the input file (BranchMD)"<<endl;
      
      
      // Lecture de l'arbre contenant le détecteur ED :
      fTreeED = (TTree *) fFile->Get("TreeED");   // Get Tree
      fTreeED->Print(); 
      if(fBranchED!=0)
	{ 
	  delete fBranchED;
	  fBranchED = 0;
	}
      fBranchED = fTreeED->GetBranch("HitStationED");
      fTreeED->SetBranchAddress("hstationED",&fHitStationED);  
      cout << " \t * There is " << fTreeED->GetEntries() << " events in the input file  (BranchED)"<<endl; 
      
      //   float ED = DrawLDF_ED() ; float MD = DrawLDF_MD();
      //cout << ED << " " << MD << endl;
      ////////// blabla LDF /////////////
      paramFitED = DrawLDF_ED();
      paramFitMD = DrawLDF_MD();

      ofstream fichier("ParamLDF_3E4.txt",ios::out | ios::app);
      if(fichier)
	{
	  fichier << ig << " " << it << " " << paramFitED.first << " " << paramFitED.second << " " << paramFitMD.first << " " << paramFitMD.second << endl;
	  fichier.close();
	}
      
      
	// if(ig == 0) {
      // 	ampED_th0.push_back(DrawLDF_ED()); ampMD_th0.push_back(DrawLDF_MD());}
      // else if(ig == 1) {
      // 	ampED_th18.push_back(DrawLDF_ED()); ampMD_th18.push_back(DrawLDF_MD());}
      // else if(ig == 2) {
      // 	ampED_th26.push_back(DrawLDF_ED()); ampMD_th26.push_back(DrawLDF_MD());}
      // else if(ig == 3) {
      // 	ampED_th33.push_back(DrawLDF_ED()); ampMD_th33.push_back(DrawLDF_MD());}
      // else if(ig == 4) {
      // 	ampED_th39.push_back(DrawLDF_ED()); ampMD_th39.push_back(DrawLDF_MD());}
      // else if(ig == 5) {
      // 	ampED_th45.push_back(DrawLDF_ED()); ampMD_th45.push_back(DrawLDF_MD());}

      fFile->Close();
     } // boucle it
   } // boucle theta

  // int size0 = ampED_th0.size(), size18 = ampED_th18.size(),  size26 = ampED_th26.size(), size33 = ampED_th33.size(), size39 = ampED_th39.size(), size45 = ampED_th45.size();
  
  
 
  // TGraph *gr0 = new TGraph(size0,&ampED_th0[0],&ampMD_th0[0]);  gr0->SetTitle("th = 0"); gr0->SetMarkerStyle(7); gr0->SetMarkerColor(2); gr0->GetXaxis()->SetTitle("Facteur Amp LDF ED"); gr0->GetYaxis()->SetTitle("Facteur Amp LDF MD");
  //   TGraph *gr1 = new TGraph(size18,&ampED_th18[0],&ampMD_th18[0]); gr1->SetTitle("th = 18"); gr1->SetMarkerStyle(7); gr1->SetMarkerColor(2); gr1->GetXaxis()->SetTitle("Facteur Amp LDF ED"); gr1->GetYaxis()->SetTitle("Facteur Amp LDF MD");
  //   TGraph *gr2 = new TGraph(size26,&ampED_th26[0],&ampMD_th26[0]); gr2->SetTitle("th = 26"); gr2->SetMarkerStyle(7); gr2->SetMarkerColor(2); gr2->GetXaxis()->SetTitle("Facteur Amp LDF ED"); gr2->GetYaxis()->SetTitle("Facteur Amp LDF MD");
  //   TGraph *gr3 = new TGraph(size33,&ampED_th33[0],&ampMD_th33[0]); gr3->SetTitle("th = 33"); gr3->SetMarkerStyle(7); gr3->SetMarkerColor(2); gr3->GetXaxis()->SetTitle("Facteur Amp LDF ED"); gr3->GetYaxis()->SetTitle("Facteur Amp LDF MD");
  //   TGraph *gr4 = new TGraph(size39,&ampED_th39[0],&ampMD_th39[0]); gr4->SetTitle("th = 39"); gr4->SetMarkerStyle(7); gr4->SetMarkerColor(2); gr4->GetXaxis()->SetTitle("Facteur Amp LDF ED"); gr4->GetYaxis()->SetTitle("Facteur Amp LDF MD");
  //   TGraph *gr5 = new TGraph(size45,&ampED_th45[0],&ampMD_th45[0]); gr5->SetTitle("th = 45"); gr5->SetMarkerStyle(7); gr5->SetMarkerColor(2); gr5->GetXaxis()->SetTitle("Facteur Amp LDF ED"); gr5->GetYaxis()->SetTitle("Facteur Amp LDF MD");

  //   TCanvas *c = new TCanvas("c");
  //   c->Divide(3,2);
  //   c->cd(1); gr0->Draw("AP"); c->cd(2); gr1->Draw("AP"); c->cd(3); gr2->Draw("AP"); c->cd(4); gr3->Draw("AP"); c->cd(5); gr4->Draw("AP");  c->cd(6); gr5->Draw("AP");  
  //   c->SaveAs("LDF_pro_1E4.root");


  return;
}









void Analyze::ReadFileChoiceDebug(char * ener)
{ 

  pair<float, float> paramFitED;
  pair<float, float> paramFitMD;

  vector<float> ampED_th0, ampED_th18, ampED_th26, ampED_th33, ampED_th39, ampED_th45;
  vector<float> ampMD_th0, ampMD_th18, ampMD_th26, ampMD_th33, ampMD_th39, ampMD_th45;
 
  cout << "====> energy = " << ener << endl;
  // initialisation 3 arbres :
  if(fTreeEvent!=0 || fTreeED!=0 || fTreeMD!= 0)
    { 
      delete fTreeEvent; delete fTreeED; delete fTreeMD;
      fTreeEvent = 0; fTreeED=0; fTreeMD=0;
    }

  // Lecture du fichier "Sim_DAT050300.root" :
  char filename[200];
   int th[6] = {0,18,25,33,39,45};
  //int th[1] = {0};
   // ig : 0 -> 6   &   it : 0 -> 100 //
   for(int ig=3; ig <4; ig++) {
     for(int it = 1; it <= 100; it++) {
       int ttt = sprintf(filename, "/projet/auger/Diane/LHAASIM/LhaaSim/v0r2/data/LhaRootGammaRE/Sim_corsika_%s_%i_%i.root", ener, th[ig], it);

       
       fFile = TFile::Open(filename);
       if(!fFile || fFile->IsZombie()) {
	 cout << "file not found or zombie" << endl; 
	 continue;
       }
      
      fFile = new TFile(filename,"READ");	// Open file	
      fFile->ls();
   
      
      // Lecture de l'arbre contenant les infos événement :
      fTreeEvent = (TTree *) fFile->Get("TreeEvent");  // Get Tree
      fTreeEvent->Print(); 
      if(fBranchEvent!=0)
	{ 
	  delete fBranchEvent;
	  fBranchEvent = 0;
	}
      fBranchEvent = fTreeEvent->GetBranch("Event");
      fTreeEvent->SetBranchAddress("event",&fEvent);  
      cout << " \t * There is " << fTreeEvent->GetEntries() << " events in the input file (BranchEvent)"<<endl;
      
      
      // Lecture de l'arbre contenant le détecteur MD :
      fTreeMD = (TTree *) fFile->Get("TreeMD");   // Get Tree
      fTreeMD->Print(); 
      if(fBranchMD!=0)
	{ 
	  delete fBranchMD;
	  fBranchMD = 0;
	}
      fBranchMD = fTreeMD->GetBranch("HitStationMD");
      fTreeMD->SetBranchAddress("hstationMD",&fHitStationMD);
      cout << " \t * There is " << fTreeMD->GetEntries() << " events in the input file (BranchMD)"<<endl;
      
      
      // Lecture de l'arbre contenant le détecteur ED :
      fTreeED = (TTree *) fFile->Get("TreeED");   // Get Tree
      fTreeED->Print(); 
      if(fBranchED!=0)
	{ 
	  delete fBranchED;
	  fBranchED = 0;
	}
      fBranchED = fTreeED->GetBranch("HitStationED");
      fTreeED->SetBranchAddress("hstationED",&fHitStationED);  
      cout << " \t * There is " << fTreeED->GetEntries() << " events in the input file  (BranchED)"<<endl; 
    
      
      int nev = fTreeMD->GetEntries();
      ofstream fichier("Debug_3E4.txt",ios::out | ios::app);
      if(fichier)
	{
	  fichier << "Sim_corsika_" << ener << "_" << ig << "_" << it << ".root" << " -> MoyNmu = " << TrigInMuons() << endl;
	  for(int i=0; i<nev; i++) {  
	    fTreeMD->GetEvent(i);
	    if(fHitStationMD->fNmu > 3) {
	      //fichier << "Sim_corsika_" << ener << "_" << ig << "_" << it << ".root" << " -> moyNmu = " << TrigInMuons() << " :: StaId = " << fHitStationMD->fId << " & Nmu = " << fHitStationMD->fNmu << endl;   }
	      fichier << " :: StaId = " << fHitStationMD->fId << " & Nmu = " << fHitStationMD->fNmu << endl;   }
	  }
	  fichier.close();
	}
      
      
      fFile->Close();
     } // boucle it
   } // boucle theta


   return;
}
