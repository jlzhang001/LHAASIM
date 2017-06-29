#include "Analyze.h"
#include "Constants.h"
#include "Trigger.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <string.h>
#include <map>
#include "TFile.h"
#include "TH2.h"
#include "TGraph.h"
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
 
  TankNbPeMD = new TH2F("TankNbPe","Tank function number of pe",(int)(xmax-xmin),xmin,xmax,(int)(xmax-xmin),ymin,ymax);

  // Draw parameters MD :
  for(int i=0;i<nombtank;i++) {
    fTreeMD->GetEvent(i);
    TankNbPeMD->Fill(fHitStationMD->fEasting, fHitStationMD->fNorthing,fHitStationMD->fNpe);
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
 
  TankNbPeED = new TH2F("TankNbPeED","Tank function number of pe",(int)(xmax-xmin),xmin,xmax,(int)(xmax-xmin),ymin,ymax);

  // Draw parameters ED :
  for(int i=0;i<nombscint;i++) {
    fTreeED->GetEvent(i);
    TankNbPeED->Fill(fHitStationED->fEasting, fHitStationED->fNorthing,fHitStationED->fNpe);
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
    if(fHitStationED->fTime0<latestime)latestime=fHitStationED->fTime0;
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
    ptMD->SetMarkerColor(fHitStationMD->fNpe);   //   ptMD->SetMarkerColor(50 - (int)((time-latestime)/latestime*25));
    ptMD->Draw("z");
  }

  
  // Draw parameters ED :
  for(int i=0;i<nombscint;i++){
    fTreeED->GetEvent(i);
    double size = 0.5;
    double time = fHitStationED->fTime0;
    TMarker *ptED = new TMarker (fHitStationED->fEasting, fHitStationED->fNorthing, 20);
    ptED->SetMarkerStyle(21);
    ptED->SetMarkerSize(size);
    ptED->SetMarkerColor(50 - (int)((time-latestime)/latestime*25));
    ptED->Draw("z");
  } 
}


// PAS MODIFIÉ !!
void Analyze::DrawOneFE(int num,int ipm)
{
  // double fesum[MAXNUMBEROFTIMEBINS];
  
  TH1F* histo;
  
  int nmax = MAXNUMBEROFTIMEBINS;
  
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);
  
  
  int nmu = fStListMD[num].fNmu;
  if(nmu<20) 
    {
      for (int nm=0;nm<nmu;nm++)
	{
	  cout << "muon " << nm << "\t time = " << fStListMD[num].fMuTimes[nm] << endl;
	}
    }
 
  ipm--;
  
     
      fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
      fCanvas = (TCanvas *) fCanvasList->Last();
      
      char*hname = new char[15];
      int idistance = (int)fStListMD[num].fR_sf;
      int id = fStListMD[num].fId;
      
      sprintf(hname,"FE%1d_%04d_%04d",ipm+1,id,idistance);
      cout << hname << endl;
      histo = new TH1F(hname,hname,5000,0,5000);
      delete hname;
      
      fHisto1->Add(histo);
     
      for(int k=0; k<nmax; k++)
	if(fStListMD[num].fPMT[ipm].fFEFilter_hi[k]!=0){
	  histo->Fill(k,fStListMD[num].fPMT[ipm].fFEFilter_hi[k]);
	}
         
      histo->SetLineColor(kRed);
      histo->Draw(); 
}



void Analyze::DrawProfilesMD()
{  
  int nevent,nmu,nel,nph,nPeMu;
  double dist;

  TH1F *hLDF_Nmu = new TH1F("hLDF_Nmu","LDF muon profile of proton shower (MD detector); Distance to shower axis (m); Number of muons",300,0,300);
  TH1F *hLDF_Nel = new TH1F("hLDF_Nel","LDF electron profile of proton shower (MD detector); Distance to shower axis (m); Number of electrons",300,0,300);
  TH1F *hLDF_Nph = new TH1F("hLDF_Nph","LDF gamma profile of proton shower (MD detector); Distance to shower axis (m); Number of gammas",300,0,300);
  TH1F *hLDF_Npe_mu = new TH1F("hLDF_Npe_mu","Number of photoelectrons produced by muons function distance to shower axis; Distance to shower axis (m); Number of photoelectrons",300,0,300);

  nevent = (int)fTreeMD->GetEntries();
  
  for(int ne=0; ne<nevent; ne++)
    {
      fTreeMD->GetEvent(ne);
      nmu = fHitStationMD->fNmu;
      nel = fHitStationMD->fNel;
      nph = fHitStationMD->fNph;
      nPeMu = fHitStationMD->fNpe_mu;
      dist = fHitStationMD->fR_sf;
      cout<<nmu<<"\t muons at "<<dist<<" m"<<endl;
      hLDF_Nmu->Fill(dist,nmu);
      hLDF_Nel->Fill(dist,nel);
      hLDF_Nph->Fill(dist,nph);
      hLDF_Npe_mu->Fill(dist,nPeMu);
     
    }//end of loop en event number
  
  TCanvas* c1 = new TCanvas;
 
  c1->Divide(3,1);
  c1->cd(1);
  hLDF_Nmu->SetMarkerStyle(34);
  hLDF_Nmu->SetMarkerColor(17);
  hLDF_Nmu->Draw("point");
  c1->cd(2);
  hLDF_Nel->SetMarkerStyle(34);
  hLDF_Nel->SetMarkerColor(38);
  hLDF_Nel->Draw("point");
  c1->cd(3);
  hLDF_Nph->SetMarkerStyle(34);
  hLDF_Nph->SetMarkerColor(46);
  hLDF_Nph->Draw("point");
 

  TCanvas* c2 = new TCanvas;

  c2->cd(1);
  hLDF_Npe_mu->SetMarkerStyle(34);
  hLDF_Npe_mu->SetMarkerColor(17);
  hLDF_Npe_mu->Draw("point");
}




void Analyze::DrawProfilesED()
{
  int nevent,nmu,nel,nph,nPeMu,nPePh,nPeEl;
  double dist;

  TH1F *hLDF_Nmu = new TH1F("hLDF_Nmu","LDF muon profile of proton shower (ED detector); Distance to shower axis (m); Number of muons;",300,0,300);
  TH1F *hLDF_Nel = new TH1F("hLDF_Nel","LDF electron profile of proton shower (ED detector); Distance to shower axis (m); Number of electrons",300,0,300);
  TH1F *hLDF_Nph = new TH1F("hLDF_Nph","LDF gamma profile of proton shower (ED detector); Distance to shower axis (m); Number of gammas",300,0,300);
  TH1F *hLDF_Npe_mu = new TH1F("hLDF_Npe_mu","Number of photoelectrons produced by muons function distance to shower axis; Distance to shower axis (m); Number of photoelectrons (µ)",300,0,300);
  TH1F *hLDF_Npe_ph = new TH1F("hLDF_Npe_ph","Number of photoelectrons produced by photons function distance to shower axis; Distance to shower axis (m); Number of photoelectrons",300,0,300);
  TH1F *hLDF_Npe_el = new TH1F("hLDF_Npe_el","Number of photoelectrons produced by electrons function distance to shower axis; Distance to shower axis (m); Number of photoelectrons (e-)",300,0,300);
    
 
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
      dist = fHitStationED->fR_sf;
      cout << nmu << "\t muons at " << dist << " m" << endl;
      hLDF_Nmu->Fill(dist,nmu);
      hLDF_Nel->Fill(dist,nel);
      hLDF_Nph->Fill(dist,nph);
      hLDF_Npe_mu->Fill(dist,nPeMu);
      hLDF_Npe_ph->Fill(dist,nPePh);
      hLDF_Npe_el->Fill(dist,nPeEl);
      
    }//end of loop en event number
  
  TCanvas* c1 = new TCanvas;
  c1->Divide(3,1);
  
  c1->cd(1);
  hLDF_Nmu->SetMarkerStyle(34);
  hLDF_Nmu->SetMarkerColor(17);
  hLDF_Nmu->Draw("point");
  c1->cd(2);
  hLDF_Nel->SetMarkerStyle(34);
  hLDF_Nel->SetMarkerColor(38);
  hLDF_Nel->Draw("point");
  c1->cd(3);
  hLDF_Nph->SetMarkerStyle(34);
  hLDF_Nph->SetMarkerColor(46);
  hLDF_Nph->Draw("point");

  TCanvas* c2 = new TCanvas;
  c2->Divide(3,1);

  c2->cd(1);
  hLDF_Npe_mu->SetMarkerStyle(34);
  hLDF_Npe_mu->SetMarkerColor(17);
  hLDF_Npe_mu->Draw("point");
  c2->cd(2);
  hLDF_Npe_ph->SetMarkerStyle(34);
  hLDF_Npe_ph->SetMarkerColor(38);
  hLDF_Npe_ph->Draw("point");
  c2->cd(3);
  hLDF_Npe_el->SetMarkerStyle(34);
  hLDF_Npe_el->SetMarkerColor(46);
  hLDF_Npe_el->Draw("point");
}



void Analyze::ParticlesDistanceTime()
{
  int nevent,nmu_neg,nel_neg,nph_neg,nmu_pos,nel_pos,nph_pos,nPeMu,nPePh,nPeEl;
  double dist;

  TH1F *hLDF_Nmu_pos = new TH1F("hLDF_Nmu_pos","LDF muon profile of proton shower (MD detector); Distance to shower axis (m); Number of muons;",300,0,300);
  TH1F *hLDF_Nmu_neg = new TH1F("hLDF_Nmu_neg","LDF muon profile of proton shower (MD detector); Distance to shower axis (m); Number of muons;",300,0,300);
  TH1F *hLDF_Nel_pos = new TH1F("hLDF_Nel_pos","LDF electron profile of proton shower (MD detector); Distance to shower axis (m); Number of electrons",300,0,300);
  TH1F *hLDF_Nel_neg = new TH1F("hLDF_Nel_neg","LDF electron profile of proton shower (MD detector); Distance to shower axis (m); Number of electrons",300,0,300);
  TH1F *hLDF_Nph_pos = new TH1F("hLDF_Nph_pos","LDF gamma profile of proton shower (MD detector); Distance to shower axis (m); Number of gammas",300,0,300);
  TH1F *hLDF_Nph_neg = new TH1F("hLDF_Nph_neg","LDF gamma profile of proton shower (MD detector); Distance to shower axis (m); Number of gammas",300,0,300);
  
 
  nevent = (int)fTreeMD->GetEntries();
  
  for(int ne=0; ne<nevent; ne++)
    {
      fTreeMD->GetEvent(ne);
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

    }//end of loop en event number


TCanvas* c1 = new TCanvas;
  c1->Divide(3,1);

  c1->cd(1);
  hLDF_Nmu_pos->SetMarkerStyle(34);
  hLDF_Nmu_pos->SetMarkerColor(38);
  hLDF_Nmu_pos->Draw("point");
  hLDF_Nmu_neg->SetMarkerStyle(34);
  hLDF_Nmu_neg->SetMarkerColor(46);
  hLDF_Nmu_neg->Draw("pointsame");

  c1->cd(2);
  hLDF_Nel_pos->SetMarkerStyle(34);
  hLDF_Nel_pos->SetMarkerColor(38);
  hLDF_Nel_pos->Draw("point");
  hLDF_Nel_neg->SetMarkerStyle(34);
  hLDF_Nel_neg->SetMarkerColor(46);
  hLDF_Nel_neg->Draw("pointsame");
  c1->cd(3);
  hLDF_Nph_pos->SetMarkerStyle(34);
  hLDF_Nph_pos->SetMarkerColor(38);
  hLDF_Nph_pos->Draw("point");
  hLDF_Nph_neg->SetMarkerStyle(34);
  hLDF_Nph_neg->SetMarkerColor(46);
  hLDF_Nph_neg->Draw("pointsame");
}


// PAS MODIFIÉ !!
void Analyze::DrawOneFE(int num,int ipm,int ichan)
{
  TH1F* histo;
  int nmax = MAXNUMBEROFTIMEBINS ;
  
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);
  
  
  int nmu=fStListMD[num].fNmu;
  if(nmu<20) 
    {
      for (int nm=0;nm<nmu;nm++)
	{
	  cout << "muon " << nm << "\t time = " << fStListMD[num].fMuTimes[nm] << endl;
	}
    }
 
  ipm--;
  
     
      fCanvasList->Add(new TCanvas("c1","Trace ",600,800));
      fCanvas = (TCanvas *) fCanvasList->Last();
      
      char*hname = new char[15];
      int idistance = (int)fStListMD[num].fR_sf;
      int id = fStListMD[num].fId;
      
      sprintf(hname,"FE%1d_%04d_%04d",ipm+1,id,idistance);
      cout << hname << endl;
      histo = new TH1F(hname,hname,5000,0,5000);
      delete hname;
      
      fHisto1->Add(histo);
     
      if(ichan==0)
	for(int k=0; k<nmax; k++)
	  if(fStListMD[num].fPMT[ipm].fFEFilter_hi[k]!=0){
	    histo->Fill(k,fStListMD[num].fPMT[ipm].fFEFilter_hi[k]);
	  }
      
	  else 
	    for(int k=0; k<nmax; k++)
	      if(fStListMD[num].fPMT[ipm].fFEFilter_lo[k]!=0){
		histo->Fill(k,fStListMD[num].fPMT[ipm].fFEFilter_lo[k]);
	  }  
 
      histo->SetLineColor(kRed);
      histo->Draw();         
} 




// PAS MODIFIÉ !!
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
  int idistance = (int)fStListMD[num].fR_sf;
  int id = fStListMD[num].fId;
  
  sprintf(hname,"PMT%1d_%04d_%04d",ipm+1,id,idistance);
  
  cout<<hname<<endl;
  histo=  new TH1F(hname,hname,5000,0.,5000);
  delete hname;
  
  sprintf(hname_mu,"PMT_MUONS",ipm+1,id,idistance);
  
  cout << hname_mu << endl;
  histo_mu = new TH1F(hname_mu,hname_mu,5000,0.,5000);
  delete hname_mu;
  
  fHisto1->Add(histo);
  trace = fStListMD[num].fPMT[ipm].fPMTSignal_hi;
  fHisto1->Add(histo_mu); //
  trace_mu = fStListMD[num].fPMT[ipm].fPMTSignal_hi_mu; //
    
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


// PAS MODIFIÉ !!
void Analyze::DrawOnePM(int num,int ipm,int ichan)
{
  map<int,double> trace;
  TH1F* histo;
 
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);

  ipm--;
  
  fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
  fCanvas = (TCanvas *) fCanvasList->Last();
  
  char*hname = new char[15];
  int idistance = (int)fStListMD[num].fR_sf;
  int id = fStListMD[num].fId;
  
  sprintf(hname,"PMT%1d_%04d_%04d",ipm+1,id,idistance);
  
  cout << hname << endl;
  histo = new TH1F(hname,hname,5000,0.,5000);
  delete hname;
      
  fHisto1->Add(histo);
  
  if(ichan==0)trace=fStListMD[num].fPMT[ipm].fPMTSignal_hi;
  else if (ichan==0)trace=fStListMD[num].fPMT[ipm].fPMTSignal_lo;
  else {
    cout<<" ichan not valide"<<endl;
    return;
  }
  
  for(map<int,double>::const_iterator slot=trace.begin();
      slot!=trace.end(); slot++)
    {
      if(((*slot).first)<10000)
	histo->Fill(((*slot).first),(*slot).second);	  
    }
   
  histo->SetLineColor(kRed);
  histo->Draw();
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
      int idistance = (int)fHitStationMD->fR_sf;
      int id = fHitStationMD->fId;
      sprintf(hname,"SUMPMT_%04d_%04d",id,idistance);
      
      cout << hname << endl;
      histo = new TH1F(hname,hname,5000,0.,5000);
      histo_mu = new TH1F(hname,hname,5000,0.,5000);
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

      histo->SetLineColor(kRed);  // A COMMENTER
      //histo_mu->SetLineColor(kBlue);
      histo_mu->Draw();
      //histo->Draw("SAME"); // A COMMENTER
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
      histo = new TH1F(hname,hname,5000,0.,5000);
      histo_mu = new TH1F(hname,hname,5000,0.,5000);
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
      //  histo_mu->Draw();
      histo->Draw(""); // A COMMENTER     
    }
}





void Analyze::DrawOneProfile(int num,int ipm)
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

  if(nmu<20) 
    {
      for (int nm=0; nm<nmu; nm++)
	{
	  cout << "muon " << nm <<"\t time = " << fHitStationMD->fMuTimes[nm] << endl;
	}
    }

  ipm--;
  
  if(fHitStationMD->fNpe>0) 
    {
      fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
      fCanvas = (TCanvas *) fCanvasList->Last();
      
      char*hname = new char[15];
      int idistance = (int)fHitStationMD->fR_sf;
      int id = fHitStationMD->fId;
      
      sprintf(hname,"ADC%1d_%04d_%04d",ipm+1,id,idistance);
      
      cout << hname << endl;
      histo = new TH1F(hname,hname,5000,0.,5000);
      histo_mu = new TH1F(hname,hname,5000,0.,5000);
      delete hname;
      
      fHisto1->Add(histo);
      fHisto1->Add(histo_mu);
      trace = fHitStationMD->fPMT[ipm].fTimeProfile;
	  
      for(map<int,double>::const_iterator slot=trace.begin();
	  slot!=trace.end(); slot++)
	{
	  if(((*slot).first)<10000)
	    histo->Fill(((*slot).first),(*slot).second);
	  
	}

      
      trace_mu = fHitStationMD->fPMT[ipm].fTimeProfile_mu;
      
      for(map<int,double>::const_iterator slot=trace_mu.begin();
	  slot!=trace_mu.end(); slot++)
	{
	  if(((*slot).first)<10000)
	    histo_mu->Fill(((*slot).first),(*slot).second);
	}      
      //histo->SetLineColor(kRed);
      histo_mu->SetLineColor(kBlue);
      histo_mu->Draw();
      //histo->Draw("SAME");     
    }
}



void Analyze::DrawOneTrace(int num)
{
  TH1F* histo;
  TH1F* histo_mu;
  int nmax = MAXNUMBEROFADCBINS;
  
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);

  fTreeMD->GetEvent(num);
  int nmu = fHitStationMD->fMuTimes.size();
  if(nmu<20) 
    {
      for (int nm=0;nm<nmu;nm++)
	{
	  cout << "muon " << nm << "\t time = " << fHitStationMD->fMuTimes[nm] << endl;
	}
    }
 
  fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
  fCanvas = (TCanvas *) fCanvasList->Last();
  
  char*hname = new char[15];
  int idistance = (int) fHitStationMD->fR_sf;
  int id = fHitStationMD->fId;
  sprintf(hname,"SUMADC_%04d_%04d",id,idistance);
  
  cout << hname << endl;
  histo = new TH1F(hname,hname,400,0.,400);
  histo_mu = new TH1F(hname,hname,400,0.,400);
  delete hname;
  
  fHisto1->Add(histo);
  fHisto1->Add(histo_mu);

  for(int k=0; k<nmax; k++)
    if(fHitStationMD->fADC[0][k]!=0){
      histo->Fill(k,fHitStationMD->fADC[0][k]);
    }
  
  for(int k=0; k<nmax; k++)
    if(fHitStationMD->fADC_mu[0][k]!=0){
      histo_mu->Fill(k,fHitStationMD->fADC_mu[0][k]);
    }

  //histo->SetLineColor(kRed);
  histo_mu->SetLineColor(kCyan);
  //histo->Draw();
  histo_mu->Draw("SAME");
}


// PAS MODIFIÉ !!
void Analyze::DrawOneTrace(int num,int ipm)
{
  TH1F* histo;
  TH1F* histo_mu;
  int nmax = MAXNUMBEROFADCBINS ;
 
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);
  
  fTreeMD->GetEvent(num);
  int nmu = fHitStationMD->fNmu;
  if(nmu<20) 
    {
      for (int nm=0;nm<nmu;nm++)
	{
	  cout << "muon " << nm <<"\t time = " << fHitStationMD->fMuTimes[nm] << endl;
	}
    }
 
  ipm--;
  
  
  fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
  fCanvas = (TCanvas *) fCanvasList->Last();
  
  char*hname = new char[15];
  int idistance = (int) fHitStationMD->fR_sf;
  int id = fHitStationMD->fId;
  
  sprintf(hname,"ADC%1d_%04d_%04d",ipm+1,id,idistance);
  cout<<hname<<endl;
  histo = new TH1F(hname,hname,400,0,400);
  histo_mu = new TH1F(hname,hname,400,0,400);
  delete hname;
  
  fHisto1->Add(histo);
  fHisto1->Add(histo_mu);


  for(int k=0; k<nmax; k++)
    if(fHitStationMD->fPMT[ipm].fADC[0][k]!=0){
      histo->Fill(k,fHitStationMD->fPMT[ipm].fADC[0][k]);
    }
  
  for(int k=0; k<nmax; k++)
    if(fHitStationMD->fPMT[ipm].fADC_mu[0][k]!=0){
      histo_mu->Fill(k,fHitStationMD->fPMT[ipm].fADC_mu[0][k]);
    }
  
  histo->SetLineColor(kRed);
  histo_mu->SetLineColor(kCyan);
  histo->Draw();
  histo_mu->Draw("SAME"); 
}



void Analyze::DrawOneTraceInVEM(int num,int ipm,int ichan)
{
  TH1F* histo;
  // TH1F* histo_mu;
  int nmax = MAXNUMBEROFADCBINS ;
  
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);
  
  fTreeMD->GetEvent(num);
  int nmu = fHitStationMD->fNmu;
  if(nmu<20) 
    {
      for (int nm=0; nm<nmu; nm++)
	{
	  cout << "muon "<< nm << "\t time = " << fHitStationMD->fMuTimes[nm] << endl;
	}
    }
  
  ipm--;
  
  
  fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
  fCanvas = (TCanvas *) fCanvasList->Last();
  
  char*hname = new char[15];
  int idistance = (int) fHitStationMD->fR_sf;
  int id = fHitStationMD->fId;
      
  sprintf(hname,"ADC%1d_%04d_%04d",ipm+1,id,idistance);
  cout << hname << endl;
  histo = new TH1F(hname,hname,400,0,400);
  //     histo_mu = new TH1F(hname,hname,400,0,400);
  delete hname;
  
  fHisto1->Add(histo);
  //      fHisto1->Add(histo_mu);
      
  cout << VEMPEAKVALUEINADC << endl;
  for(int k=0; k<nmax; k++)
    if(fHitStationMD->fPMT[ipm].fADC[ichan][k]!=0){
      histo->Fill(k,(fHitStationMD->fPMT[ipm].fADC[ichan][k])/VEMPEAKVALUEINADC*32.);
    }
  
  histo->SetLineColor(kRed);
  //      histo_mu->SetLineColor(kCyan);
  histo->Draw();
  //       histo_mu->Draw("SAME"); 
}



void Analyze::DrawOneTrace(int num,int ipm,int ichan)
{
  TH1F* histo;
  TH1F* histo_mu;
  int nmax = MAXNUMBEROFADCBINS ;
  
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);
  
  fTreeMD->GetEvent(num);
  int nmu = fHitStationMD->fNmu;
  if(nmu<20) 
    {
      for (int nm=0;nm<nmu;nm++)
	{
	  cout << "muon " << nm << "\t time = " << fHitStationMD->fMuTimes[nm] << endl;
	}
    }
 
  ipm--;
  
      fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
      fCanvas = (TCanvas *) fCanvasList->Last();
      
      char*hname = new char[15];
      int idistance = (int) fHitStationMD->fR_sf;
      int id = fHitStationMD->fId;
      
      sprintf(hname,"ADC%1d_%04d_%04d",ipm+1,id,idistance);
      cout << hname << endl;
      histo = new TH1F(hname,hname,400,0,400);
      //     histo_mu = new TH1F(hname,hname,400,0,400);
      delete hname;
      
      fHisto1->Add(histo);
 //      fHisto1->Add(histo_mu);

      for(int k=0; k<nmax; k++)
	if(fHitStationMD->fPMT[ipm].fADC[ichan][k]!=0){
	  histo->Fill(k,fHitStationMD->fPMT[ipm].fADC[ichan][k]);
	}
         
      histo->SetLineColor(kRed);
      //      histo_mu->SetLineColor(kCyan);
      histo->Draw();
//       histo_mu->Draw("SAME");
}



// PAS MODIFIÉ !!
void Analyze::DrawTraces()
{
  TH1F* histo;
  // TH1F* histo_mu;
  int nmax = MAXNUMBEROFADCBINS;  
  int indix = 0;
  int indix2 = 0;
   
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);
  
  for (int i=0; i < fTreeMD->GetEntries(); i++)
    {
      fTreeMD->GetEvent(i);
      if(fHitStationMD->fT2ToT||fHitStationMD->fT1Threshold)
  	{
	  if ( indix%8==0)
	    {
	      indix = 0;
	      indix2++;
	      char* canvasname = new char[2];
	      sprintf(canvasname,"c%1d",indix2);
	      
	      fCanvasList->Add(new TCanvas(canvasname,"Traces ",600,800));
	      delete canvasname;
	      fCanvas = (TCanvas *) fCanvasList->Last();
	      fCanvas->Divide(2,4);
	      cout<<"new canvas"<<endl;
	      
	    } 
	  
	  indix++;
	  
	  char*hname = new char[15];
	  int idistance = (int) fHitStationMD->fR_sf;
	  int id = fHitStationMD->fId;
	  
	  sprintf(hname,"SUMADC_%04d_%04d",id,idistance);
	  cout << hname << endl;
	  histo = new TH1F(hname,hname,400,0.,400);
	  //  histo_mu =  new TH1F(hname,hname,400,0.,400);
	  delete hname;
	  
	  fHisto1->Add(histo);
	  // fHisto1->Add(histo_mu);
	  
	  for(int k=0; k<nmax; k++)
	    if(fHitStationMD->fADC[0][k]!=0){
	      histo->Fill(k,fHitStationMD->fADC[0][k]);
	      cout << id << " " << k << " " << fHitStationMD->fADC[0][k] << endl;
	    }	    
	  
	  fCanvas->cd(indix);
	  histo->SetLineColor(kRed);
	  //  histo_mu->SetLineColor(kCyan);
	  histo->Draw();
	  //    histo_mu->Draw("SAME");
	  
	}// end of loop on station
    }
}


// PAS MODIFIÉ !!
void Analyze::DrawTraces(int ipm)
{
  TH1F* histo;
  TH1F* histo_mu;
  int nmax=MAXNUMBEROFADCBINS;
 
  int indix=0;
  int indix2=0;
 
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);
  
  vector<HitStationMD>::iterator sta;

  ipm--;
  for(sta=fStListMD.begin();  sta!= fStListMD.end(); sta++)
    {
      if(sta->fNpe>0) 
	{
	  if ( indix%8 == 0)
	    {	      
	      indix = 0;
	      indix2++;
	      char* canvasname = new char[2];
	      sprintf(canvasname,"c%1d",indix2);
	      
	      fCanvasList->Add(new TCanvas(canvasname,"Traces ",600,800));
	      delete canvasname;
	      fCanvas = (TCanvas *) fCanvasList->Last();
	      fCanvas->Divide(2,4);
	      cout<<"new canvas"<<endl;  
	    } 
	      
	  indix++;
	      
	  char*hname = new char[15];
	  int idistance = (int)sta->fR_sf;
	  int id = sta->fId;
	  sprintf(hname,"PMT%1d_%04d_%04d",ipm+1,id,idistance);
	  
	  cout<<hname<<endl;
	  histo = new TH1F(hname,hname,400,0.,400);
	  histo_mu = new TH1F(hname,hname,400,0.,400);
	  delete hname;
	      
	  fHisto1->Add(histo);
	  fHisto1->Add(histo_mu);
	      
	  for(int k=0; k<nmax; k++)
	    if(sta->fPMT[ipm].fADC[0][k]!=0){
	      histo->Fill(k,sta->fPMT[ipm].fADC[0][k]);
	    }
	  for(int k=0; k<nmax; k++)
	    if(sta->fPMT[ipm].fADC_mu[0][k]!=0){
	      histo_mu->Fill(k,sta->fPMT[ipm].fADC_mu[0][k]);
	    }
	  fCanvas->cd(indix);
	  histo->SetLineColor(kRed);
	  histo_mu->SetLineColor(kCyan);
	  histo->Draw();
	  histo_mu->Draw("SAME");    
	}
      
      if( indix%8==0)
	{
	  fCanvas->Print();
	}           
    }// end of loop on station
}


// PAS MODIFIÉ !!
void Analyze::DrawTracesInVem()
{
  TH1F* histo;
  TH1F* histo_mu;
  
  int nmax=MAXNUMBEROFADCBINS;
  int indix=0;
  int indix2=0;
  
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);
  
  vector<HitStationMD>::iterator sta;

  for(sta=fStListMD.begin();  sta!= fStListMD.end(); sta++)
    {
      double sum=0;
      if(sta->fNpe>0 && (sta->fT1Threshold || sta->fT2ToT)) 
	{ 
	  if ( indix%8==0)
	    {
	      indix=0;
	      indix2++;
	      char* canvasname = new char[2];
	      sprintf(canvasname,"c%1d",indix2);
		 
	      fCanvasList->Add(new TCanvas(canvasname,"Traces ",600,800));
	      delete canvasname;
	      fCanvas = (TCanvas *) fCanvasList->Last();
	      fCanvas->Divide(2,4);
	      cout<<"new canvas"<<endl;	  
	    } 
	  indix++;
      
	  char*hname = new char[15];
	  int idistance = (int)sta->fR_sf;
	  int id = sta->fId;
	  
	  sprintf(hname,"SUMADC_%04d_%04d",id,idistance);
	  cout << hname << endl;
	  histo = new TH1F(hname,hname,nmax,0.,nmax);
	  histo_mu = new TH1F(hname,hname,nmax,0.,nmax);
	  delete hname;
	      
	  fHisto1->Add(histo);
	  fHisto1->Add(histo_mu);
	      
	  for(int k=0; k<nmax; k++)
	    if(sta->fADC[0][k]!=0){
	      histo->Fill(k,sta->fADC[0][k]/VEMPEAKVALUEINADC);
	      sum+=sta->fADC[0][k];
	    }
	  for(int k=0; k<nmax; k++)
	    if(sta->fADC_mu[0][k]!=0){
	      histo_mu->Fill(k,sta->fADC_mu[0][k]/VEMPEAKVALUEINADC);
	    }
	  
	  fCanvas->cd(indix);
	  histo->SetLineColor(kRed);
	  histo_mu->SetLineColor(kCyan);
	  histo->Draw();
	  histo_mu->Draw("SAME");
	      
	  cout << "signal of station " << sta->fId << " = " << sum/VEMCHARGEVALUEINADC/(int)NPM << " VEM" << endl;

	  if( indix%8==0)
	    {
	      fCanvas->Print();
	    }      
	  
	}// end of loop on station
    }
}


// PAS MODIFIÉ !!
void Analyze::DrawTracesInVem(int ipm)
{
  TH1F* histo;
  TH1F* histo_mu;
  int nmax=MAXNUMBEROFADCBINS;
  int indix=0;
  int indix2=0;
 
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);
  
  vector<HitStationMD>::iterator sta;
  
  ipm--;
  for(sta=fStListMD.begin();  sta!= fStListMD.end(); sta++)
    {
      double sum=0;
      if(sta->fNpe>0 &&(sta->fT1Threshold || sta->fT2ToT)) 
	{
	  if ( indix%8==0)
	    {	  
	      indix=0;
	      indix2++;
	      char* canvasname = new char[2];
	      sprintf(canvasname,"c%1d",indix2);
	      
	      fCanvasList->Add(new TCanvas(canvasname,"Traces ",600,800));
	      delete canvasname;
	      fCanvas=(TCanvas *) fCanvasList->Last();
	      fCanvas->Divide(2,4);
	      cout<<"new canvas"<<endl;	  
	    } 
	  indix++;    
	  
	  char*hname = new char[15];
	  int idistance = (int)sta->fR_sf;
	  int id = sta->fId;
	  
	  sprintf(hname,"PMT%1d_%04d_%04d",ipm+1,id,idistance);    
	  
	  cout<<hname<<endl;
	  histo = new TH1F(hname,hname,nmax,0.,nmax);
	  // histo_mu = new TH1F(hname,hname,nmax,0.,nmax);
	  delete hname;
	  
	  fHisto1->Add(histo);
	  // fHisto1->Add(histo_mu);    
	      
	  for(int k=0; k<nmax; k++)
	    if(sta->fPMT[ipm].fADC[0][k]!=0){
	      histo->Fill(25*k,sta->fPMT[ipm].fADC[0][k]/VEMPEAKVALUEINADC);
	      sum+=sta->fPMT[ipm].fADC[0][k];    
	    }
	  
	  fCanvas->cd(indix);
	  histo->SetLineColor(kRed);
	  //   histo_mu->SetLineColor(kCyan);
	  histo->Draw();
	  //  histo_mu->Draw("SAME");
	      
	  cout<<"signal of station "<< sta->fId<<" = "<<sum/VEMCHARGEVALUEINADC<<" VEM"<<endl;     
	}      
      if( indix%8==0)
	{
	  fCanvas->Print();
	}      
    }// end of loop on station 
}



void Analyze::GetMD()
{
  int tank;
  for (int i=0; i <= fTreeMD->GetEntries(); i++)
    {
      tank = i;
      fTreeMD->GetEvent(tank);
      
      cout << "\t tank " << fHitStationMD->fId << "\t pel = "<<fHitStationMD->fNpe << "\t nmu = " << fHitStationMD->fNmu << "\t nel = " << fHitStationMD->fNel << "\t nph = " << fHitStationMD->fNph << "\t dist = " << fHitStationMD->fR_sf << "\t samp = " << fHitStationMD->fSampFact << "\t time = " << fHitStationMD->fTime0 << "\t pemu = " << fHitStationMD->fNpe_mu << "\t peel = " << fHitStationMD->fNpe_el << "\t peph = " << fHitStationMD->fNpe_ph << endl; 
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

      cout << i << "\t scint " << fHitStationED->fId << "\t pel = " << fHitStationED->fNpe << "\t nmu = " << fHitStationED->fNmu << "\t nel = " << fHitStationED->fNel << "\t nph = " << fHitStationED->fNph << "\t dist = " << fHitStationED->fR_sf << "\t samp = " << fHitStationED->fSampFact << "\t time = " << fHitStationED->fTime0 << "\t pemu = " << fHitStationED->fNpe_mu << "\t peel = " << fHitStationED->fNpe_el << "\t peph = " << fHitStationED->fNpe_ph << endl ; 
    }
}



void Analyze::GetMDED()
{
  int tank, scint;

  cout << "Réseau MD : \n" << endl;

// Détecteur MD :
for (int i=0; i <= fTreeMD->GetEntries(); i++)
    {
      tank = i;
      fTreeMD->GetEvent(tank);
      cout << i << "\t tank " << fHitStationMD->fId << "  dist_Sf = " << fHitStationMD->fR_sf <<" \t time = "<<fHitStationMD-> fTime0 << "\t petot = " << fHitStationMD->fNpe << endl; 
    }

 cout << "\nRéseau ED : \n" << endl; 


// Détecteur ED :
for (int i=0; i <= fTreeED->GetEntries(); i++)
    {
      scint = i;
      fTreeED->GetEvent(scint);
      cout << i << "\t scint " << fHitStationED->fId << "  dist_Sf = " << fHitStationED->fR_sf <<" \t time = "<<fHitStationED-> fTime0 << "\t petot = " << fHitStationED->fNpe << endl;      }
 
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




//--------------------------------------------------------------------


// PAS MODIFIÉ !!
void Analyze::TestAnodeDynode()
{
  map<int,double> pm_hi;
  map<int,double> pm_lo;
  ofstream outfilehi,outfilelo;
  HitStationMD* sta;
  int nbsta;
  
  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);
  int nev = (int)fTreeMD->GetEntries();
  cout << nev << " events to use" << endl;
 
  outfilehi.open("pmhi.dat",ios::out);
  outfilelo.open("pmlo.dat",ios::out);

  for(int ne=0; ne<=nev; ne++)
    {
      fTreeMD->GetEvent(ne);
      if(ne%1000==0) cout<< ne << " particles "<<endl;
      fStListMD = fEvent->fHitStationListMD;
      
      nbsta= fStListMD.size();
      for(int ista=0;ista<nbsta;ista++){
	sta=&(fStListMD[ista]);
	
	
	if(sta->fNpe>2000) 
	  {
	    pm_hi =sta->fPMT[0].fPMTSignal_hi;
	    pm_lo =sta->fPMT[0].fPMTSignal_lo;
	    double samp=sta->fSampFact;
	    int firstbin=-100;
	    
	    for(map<int,double>::const_iterator slot=pm_hi.begin();
		slot!=pm_hi.end(); slot++)
	      {
		if(((*slot).first<10000))
		  {
		    firstbin=(*slot).first;
		    break;
		  }
	      }
	    
	    if(firstbin>0)
	      for(int k=firstbin; k<5000+firstbin; k++){
		if(pm_hi.count(k))
		  outfilehi<<" "<<pm_hi[k]/samp<<" "<<pm_hi[k]/samp<<" "<<pm_hi[k]/samp<<endl;
		else
		  outfilehi<<" 0 0 0"<<endl;
	      }
	    
	    firstbin=-100;
	    for(map<int,double>::const_iterator slot=pm_lo.begin();
		slot!=pm_lo.end(); slot++)
	       {
		 if(((*slot).first<10000))
		   {
		     firstbin=(*slot).first;
		     break;
		   }
	       }
	     if(firstbin>0)
	       for(int k=firstbin; k<5000+firstbin; k++){
		 if(pm_lo.count(k))
		   outfilelo<<" "<<pm_lo[k]/samp<<" "<<pm_lo[k]/samp<<" "<<pm_lo[k]/samp<<endl;
		 else
		   outfilelo<<" 0 0 0"<<endl;
	       }
	  }
      }
    }
  outfilelo.close();
  outfilehi.close();
}


// PAS MODIFIÉ !!
void Analyze::TestNewTOT()
{
  HitStationMD* sta;

  char *filename= new char[40];
  float LowThreshold = 0.2*24.;
  float UpThreshold = 0.4*24;
  int WindowSize = 120;
  int NumberOfRealisationL = 10;
  int nentry = (int)fTreeMD->GetEntries();
  int n;
  ofstream out1;
  
  out1.open("totut.dat",ios::out);
  
  for(int upperadd=0; upperadd<2; upperadd++){
    UpThreshold+=0.2*24;
    if( upperadd ==4 )UpThreshold=1000;
    for(int realadd=0;realadd<2;realadd++)
      {
	int NumberOfRealisation=NumberOfRealisationL+realadd*2;
	n=0;
	for(int i = 0; i < nentry; i++){
	  
	  fTreeMD->GetEntry(i);
	  fStListMD = fEvent->fHitStationListMD;
	  sta=&(fStListMD[0]);
	  if(i%1000==0) cout<< i << " particles "<<endl;
  
	  int count=0;
	  
	  for(int ipm=0; ipm<NPM; ipm++)
	    {  
	      for(int ibin=0;ibin<400;ibin++){ 
		int count2 = 0;
		int begin = 0;
		if(sta->fPMT[ipm].fADC[0][ibin]>LowThreshold && sta->fPMT[ipm].fADC[0][ibin]<UpThreshold){
		  begin = ibin;
		  
		  do{
		    if(sta->fPMT[ipm].fADC[0][ibin] > LowThreshold && sta->fPMT[ipm].fADC[0][ibin]<UpThreshold) count2+=1;
		    ibin++;
		  }while(ibin<(begin+WindowSize) && ibin<400);
		}
	      
		if(count2>=NumberOfRealisation)
		  {
		    if(upperadd!=4) out1<<" TOT sur PM#"<<ipm+1<< "  muon "<<i<<endl;
		    // out1<<endl;
		    count+=1;
		    break;
		  }
	      } 
	    }
	  if(count>=2)
	    {
	      n++;
	      out1<<"TOT SUR DOUBLE MU Numero = "<<i<<" "<<n<<endl;
	    }
	  
	}
	out1<<WindowSize<<" "<<UpThreshold<<" "<<NumberOfRealisation<< " "<<n<<endl;
      }
  }
  out1.close();
  delete filename;
}

