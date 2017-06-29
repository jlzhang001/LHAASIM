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
  fCanvasList=new TList();
  fHisto1= new TList();
  fHistTitle=new TList();
  fHistAxis=new TList(); 
 
  fTree = 0;
  fEvent =0;
  fBranch =0 ;
  fFile=0;

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

  HitStationMD* staMD;
  vector<HitStationMD> fStListMD;
  HitStationED* staED;
  vector<HitStationED> fStListED;
  double xmin,ymin,xmax,ymax;

  xmin=99999999;
  ymin=99999999;
  xmax =0;
  ymax =0;

  fTree = (TTree *) fFile->Get("TreeEvent");      // Get Tree
  fTree->Print(); 
  
  fTree->SetBranchAddress("event",&fEvent);
  
  cout << " \t * There is " << fTree->GetEntries() << " events in the input file "<<endl;
  
  // MD detector :
  fTree->GetEntry(nev);
  fStListMD = fEvent->fHitStationListMD;
  int nombtank = fStListMD.size();
  TText *stidMD[nombtank];
  double latestime=9999999;
  cout<<nombtank<<endl;
  for(int i=0;i<nombtank;i++) {
    staMD= &(fStListMD[i]);
    if(staMD->fNorthing > ymax) ymax = staMD->fNorthing;
    if(staMD->fNorthing < ymin) ymin = staMD->fNorthing; 
    if(staMD->fEasting > xmax) xmax = staMD->fEasting; 
    if(staMD->fEasting < xmin) xmin = staMD->fEasting;
    char* nameMD = new char[3];
    sprintf(nameMD,"%3d",staMD->fId);
    stidMD[i]=new TText(staMD->fEasting+5,staMD->fNorthing+5,nameMD);
    stidMD[i]->SetTextSize(0.02);
    delete nameMD;
    if(staMD->fTime0<latestime)latestime=staMD->fTime0;
  }
    // ED detector :
    fTree->GetEntry(nev);
    fStListED = fEvent->fHitStationListED;
    int nombscint = fStListED.size();
    TText *stidED[nombscint];
    cout<<nombscint<<endl;
    for(int i=0;i<nombscint;i++){
      staED= &(fStListED[i]);
      if(staED->fNorthing > ymax) ymax = staED->fNorthing;
      if(staED->fNorthing < ymin) ymin = staED->fNorthing; 
      if(staED->fEasting > xmax) xmax = staED->fEasting; 
      if(staED->fEasting < xmin) xmin = staED->fEasting;
    char* nameED = new char[3];
    sprintf(nameED,"%3d",staED->fId);
    stidED[i]=new TText(staED->fEasting+5,staED->fNorthing+5,nameED);
    stidED[i]->SetTextSize(0.02);
    delete nameED;
    if(staED->fTime0<latestime)latestime=staED->fTime0;
    }
  xmin-= 20;  
  ymin-= 20;
  xmax+= 40;
  ymax+= 30;
  
  cout<<xmin<<" "<<xmax<<" "<<ymin<<" "<<ymax<<endl;
  hcores =  new TH2F("Cores","Cores",(int)(xmax-xmin),xmin,xmax,(int)(xmax-xmin),ymin,ymax);
  
  
 
    hcores->Fill(fEvent->fEasCore,fEvent->fNorCore);
    cout << "################################### \n Position du coeur (Est,Nord) = (" << fEvent->fEasCore << "," << fEvent->fNorCore << ") \n #################################" << endl;
  
  fCanvasList->Add(new TCanvas("array"," array ",600,600));
  
  fCanvas=(TCanvas *) fCanvasList->Last();
  TStyle palette;
  palette.SetPalette(1);
  palette.Draw();

  // Draw parameters Core :
  hcores->SetMarkerStyle(29);
  hcores->SetMarkerColor(kRed);
  hcores->Draw();

  // Draw parameters MD :
  for(int i=0;i<nombtank;i++) {
    //stidMD[i]->Draw();
    staMD= &(fStListMD[i]);
    double size = 0.5 +log(staMD->fNpe)/5.;
    double time =staMD->fTime0;
    TMarker *ptMD = new TMarker (staMD->fEasting, staMD->fNorthing, 20);
      ptMD->SetMarkerStyle(24);
      ptMD->SetMarkerSize(size);
      ptMD->SetMarkerColor(50 - (int)((time-latestime)/latestime*25));
    ptMD->Draw("z");
  }


   // Draw parameters ED :
    for(int i=0;i<nombscint;i++){
      //stidED[i]->Draw();
    staED= &(fStListED[i]);
    double size= 0.5;
    double time=staED->fTime0;
    TMarker *ptED = new TMarker (staED->fEasting, staED->fNorthing, 20);
    ptED->SetMarkerStyle(21);
    ptED->SetMarkerSize(size);
    ptED->SetMarkerColor(50 - (int)((time-latestime)/latestime*25));
    ptED->Draw("z");
  
    }
  
}


void Analyze::DrawOneFE(int num,int ipm)
{
  // double fesum[MAXNUMBEROFTIMEBINS];
  
  TH1F* histo;
  
  int nmax=MAXNUMBEROFTIMEBINS ;
  
  // for(int i = 0;i<nmax;i++)
  // fesum[i]=0;
  
  
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
	  cout<<"muon "<< nm<<"\t time = "<<fStListMD[num].fMuTimes[nm]<<endl;
	}
    }
 
  ipm--;
  
     
      fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
      fCanvas=(TCanvas *) fCanvasList->Last();
      
      char*hname = new char[15];
      int idistance = (int)fStListMD[num].fR_sf;
      int id = fStListMD[num].fId;
      
      sprintf(hname,"FE%1d_%04d_%04d",ipm+1,id,idistance);
      cout<<hname<<endl;
      histo=  new TH1F(hname,hname,5000,0,5000);
      delete hname;
      
      fHisto1->Add(histo);
     
      //      fesum=fStListMD[num].fPMT[ipm].fFEFilter_hi;
      for(int k=0; k<nmax; k++)
	if(fStListMD[num].fPMT[ipm].fFEFilter_hi[k]!=0){
	  histo->Fill(k,fStListMD[num].fPMT[ipm].fFEFilter_hi[k]);
	}
         
      histo->SetLineColor(kRed);
      histo->Draw(); 
}



void Analyze::DrawProfilesMD()
{  
  int nevent,nsta,nmu,nel,nph,nPeMu,nPePh,nPeEl,nPeDir;
  double dist;
  HitStationMD* sta;
  TH1F *hLDF_Nmu = new TH1F("hLDF_Nmu","LDF muon profile of proton shower (MD detector); Distance to shower axis (m); Number of muons",300,0,300);
  TH1F *hLDF_Nel = new TH1F("hLDF_Nel","LDF electron profile of proton shower (MD detector); Distance to shower axis (m); Number of electrons",300,0,300);
  TH1F *hLDF_Nph = new TH1F("hLDF_Nph","LDF gamma profile of proton shower (MD detector); Distance to shower axis (m); Number of gammas",300,0,300);
  TH1F *hLDF_Npe_mu = new TH1F("hLDF_Npe_mu","Number of photoelectrons produced by muons function distance to shower axis; Distance to shower axis (m); Number of photoelectrons",300,0,300);
  TH1F *hLDF_Npe_ph = new TH1F("hLDF_Npe_ph","Number of photoelectrons produced by photons function distance to shower axis; Distance to shower axis (m); Number of photoelectrons (µ)",300,0,300);
  TH1F *hLDF_Npe_el = new TH1F("hLDF_Npe_el","Number of photoelectrons produced by electrons function distance to shower axis; Distance to shower axis (m); Number of photoelectrons (e-)",300,0,300);
  TH1F *hLDF_Npe_direct = new TH1F("hLDF_Npe_direct","Number of photoelectrons (direct light) function distance shower axis; Distance to shower axis (m); Number of photoelectrons",300,0,300);

  
 
  nevent=(int)fTree->GetEntries();
  
  for(int ne=0; ne<nevent; ne++)
    {
      fTree->GetEvent(ne);
      if(ne%1000==0) cout<< ne << " particles "<<endl;
      fStListMD = fEvent->fHitStationListMD;
      nsta = fStListMD.size();
      
      vector<HitStationMD>::iterator sta;  
      fStListMD = fEvent->fHitStationListMD;
       for(sta=fStListMD.begin(); sta!=fStListMD.end(); sta++)
	{
	  nmu=sta->fNmu;
	  nel=sta->fNel;
	  nph=sta->fNph;
	  nPeMu=sta->fNpe_mu;
	  nPePh=sta->fNpe_ph;
	  nPeEl=sta->fNpe_el;
	  nPeDir=sta->fNpe_direct;
	  dist = sta->fR_sf;
	  //cout<<nmu<<"\t muons at "<<dist<<" m"<<endl;
	  hLDF_Nmu->Fill(dist,nmu);
	  hLDF_Nel->Fill(dist,nel);
	  hLDF_Nph->Fill(dist,nph);
	  hLDF_Npe_mu->Fill(dist,nPeMu);
	  hLDF_Npe_ph->Fill(dist,nPePh);
	  hLDF_Npe_el->Fill(dist,nPeEl);
	  hLDF_Npe_direct->Fill(dist,nPeDir);

	}
      
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
  c2->Divide(2,2);

  c2->cd(1);
  hLDF_Npe_mu->SetMarkerStyle(34);
  hLDF_Npe_mu->SetMarkerColor(17);
  hLDF_Npe_mu->Draw("point");
  c2->cd(2);
  hLDF_Npe_ph->SetMarkerStyle(34);
  hLDF_Npe_ph->SetMarkerColor(30);
  hLDF_Npe_ph->Draw("point");
  c2->cd(3);
  hLDF_Npe_el->SetMarkerStyle(34);
  hLDF_Npe_el->SetMarkerColor(38);
  hLDF_Npe_el->Draw("point");
  c2->cd(4);
  hLDF_Npe_direct->SetMarkerStyle(34);
  hLDF_Npe_direct->SetMarkerColor(46);
  hLDF_Npe_direct->Draw("point");
}




void Analyze::DrawProfilesED()
{
  
  int nevent,nsta,nmu,nel,nph,nPeMu,nPePh,nPeEl;
  double dist;
  HitStationED* sta;
  TH1F *hLDF_Nmu = new TH1F("hLDF_Nmu","LDF muon profile of proton shower (ED detector); Distance to shower axis (m); Number of muons;",300,0,300);
  TH1F *hLDF_Nel = new TH1F("hLDF_Nel","LDF electron profile of proton shower (ED detector); Distance to shower axis (m); Number of electrons",300,0,300);
  TH1F *hLDF_Nph = new TH1F("hLDF_Nph","LDF gamma profile of proton shower (ED detector); Distance to shower axis (m); Number of gammas",300,0,300);
  TH1F *hLDF_Npe_mu = new TH1F("hLDF_Npe_mu","Number of photoelectrons produced by muons function distance to shower axis; Distance to shower axis (m); Number of photoelectrons (µ)",300,0,300);
  TH1F *hLDF_Npe_ph = new TH1F("hLDF_Npe_ph","Number of photoelectrons produced by photons function distance to shower axis; Distance to shower axis (m); Number of photoelectrons",300,0,300);
  TH1F *hLDF_Npe_el = new TH1F("hLDF_Npe_el","Number of photoelectrons produced by electrons function distance to shower axis; Distance to shower axis (m); Number of photoelectrons (e-)",300,0,300);
    
 
  nevent=(int)fTree->GetEntries();
  
  for(int ne=0; ne<nevent; ne++)
    {
      fTree->GetEvent(ne);
      if(ne%1000==0) cout<< ne << " particles "<<endl;
      fStListED = fEvent->fHitStationListED;
      nsta = fStListED.size();
      
      vector<HitStationED>::iterator sta;  
      fStListED = fEvent->fHitStationListED;
       for(sta=fStListED.begin(); sta!=fStListED.end(); sta++)
	{
	  nmu = sta->fNmu;
	  nel = sta->fNel;
	  nph = sta->fNph;
	  nPeMu=sta->fNpe_mu;
	  nPePh=sta->fNpe_ph;
	  nPeEl=sta->fNpe_el;
	  dist = sta->fR_sf;
	  cout<<nmu<<"\t muons at "<<dist<<" m"<<endl;
	  hLDF_Nmu->Fill(dist,nmu);
	  hLDF_Nel->Fill(dist,nel);
	  hLDF_Nph->Fill(dist,nph);
	  hLDF_Npe_mu->Fill(dist,nPeMu);
	  hLDF_Npe_ph->Fill(dist,nPePh);
	  hLDF_Npe_el->Fill(dist,nPeEl);

	}
      
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

  int nevent,nsta,nmu_neg,nel_neg,nph_neg,nmu_pos,nel_pos,nph_pos,nPeMu,nPePh,nPeEl;
  double dist;
  HitStationMD* sta;
  TH1F *hLDF_Nmu_pos = new TH1F("hLDF_Nmu_pos","LDF muon profile of proton shower (MD detector); Distance to shower axis (m); Number of muons;",300,0,300);
  TH1F *hLDF_Nmu_neg = new TH1F("hLDF_Nmu_neg","LDF muon profile of proton shower (MD detector); Distance to shower axis (m); Number of muons;",300,0,300);
  TH1F *hLDF_Nel_pos = new TH1F("hLDF_Nel_pos","LDF electron profile of proton shower (MD detector); Distance to shower axis (m); Number of electrons",300,0,300);
  TH1F *hLDF_Nel_neg = new TH1F("hLDF_Nel_neg","LDF electron profile of proton shower (MD detector); Distance to shower axis (m); Number of electrons",300,0,300);
  TH1F *hLDF_Nph_pos = new TH1F("hLDF_Nph_pos","LDF gamma profile of proton shower (MD detector); Distance to shower axis (m); Number of gammas",300,0,300);
  TH1F *hLDF_Nph_neg = new TH1F("hLDF_Nph_neg","LDF gamma profile of proton shower (MD detector); Distance to shower axis (m); Number of gammas",300,0,300);
  
 
  nevent=(int)fTree->GetEntries();
  
  for(int ne=0; ne<nevent; ne++)
    {
      fTree->GetEvent(ne);
      if(ne%1000==0) cout<< ne << " particles "<<endl;
      fStListMD = fEvent->fHitStationListMD;
      nsta = fStListMD.size();
      
      vector<HitStationMD>::iterator sta;  
      fStListMD = fEvent->fHitStationListMD;
       for(sta=fStListMD.begin(); sta!=fStListMD.end(); sta++)
	{
	  if(sta->fTime0 < 0) {
	    nmu_neg = sta->fNmu;
	    nel_neg = sta->fNel;
	    nph_neg = sta->fNph; }
	  else {
	    nmu_pos = sta->fNmu;
	    nel_pos = sta->fNel;
	    nph_pos = sta->fNph; }
	  dist = sta->fR_sf;
	  //cout<<nmu<<"\t muons at "<<dist<<" m"<<endl;
	  hLDF_Nmu_neg->Fill(dist,nmu_neg);
	  hLDF_Nel_neg->Fill(dist,nel_neg);
	  hLDF_Nph_neg->Fill(dist,nph_neg);
	  hLDF_Nmu_pos->Fill(dist,nmu_pos);
	  hLDF_Nel_pos->Fill(dist,nel_pos);
	  hLDF_Nph_pos->Fill(dist,nph_pos);
	}
      
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










void Analyze::DrawOneFE(int num,int ipm,int ichan)
{
  //double fesum[MAXNUMBEROFTIMEBINS];
  
  TH1F* histo;
  
  int nmax=MAXNUMBEROFTIMEBINS ;
  
  // for(int i = 0;i<nmax;i++)
  //  fesum[i]=0;
  
  
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
	  cout<<"muon "<< nm<<"\t time = "<<fStListMD[num].fMuTimes[nm]<<endl;
	}
    }
 
  ipm--;
  
     
      fCanvasList->Add(new TCanvas("c1","Trace ",600,800));
      fCanvas=(TCanvas *) fCanvasList->Last();
      
      char*hname = new char[15];
      int idistance = (int)fStListMD[num].fR_sf;
      int id = fStListMD[num].fId;
      
      sprintf(hname,"FE%1d_%04d_%04d",ipm+1,id,idistance);
      cout<<hname<<endl;
      histo=  new TH1F(hname,hname,5000,0,5000);
      delete hname;
      
      fHisto1->Add(histo);
     


      // if(ichan==0)fesum=fStListMD[num].fPMT[ipm].fFEFilter_hi;
      //else fesum=fStListMD[num].fPMT[ipm].fFEFilter_lo;
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
  fCanvas=(TCanvas *) fCanvasList->Last();
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
  
  cout<<hname_mu<<endl;
  histo_mu=  new TH1F(hname_mu,hname_mu,5000,0.,5000);
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
  
  //
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
  fCanvas=(TCanvas *) fCanvasList->Last();
  
  char*hname = new char[15];
  int idistance = (int)fStListMD[num].fR_sf;
  int id = fStListMD[num].fId;
  
  sprintf(hname,"PMT%1d_%04d_%04d",ipm+1,id,idistance);
  
  cout<<hname<<endl;
  histo=  new TH1F(hname,hname,5000,0.,5000);
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
  int nmu=fStListMD[num].fNmu;
  if(nmu<20) 
    {
      for (int nm=0;nm<nmu;nm++)
	{
	  cout<<"muon "<< nm<<"\t time = "<<fStListMD[num].fMuTimes[nm]<<endl;
	}
    }
  if(fStListMD[num].fNpe>0) 
    {
      fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
      fCanvas=(TCanvas *) fCanvasList->Last();
      
      char*hname = new char[15];
      int idistance = (int)fStListMD[num].fR_sf;
      int id = fStListMD[num].fId;
      sprintf(hname,"SUMPMT_%04d_%04d",id,idistance);
      
      cout<<hname<<endl;
      histo=  new TH1F(hname,hname,5000,0.,5000);
      histo_mu=  new TH1F(hname,hname,5000,0.,5000);
      delete hname;
      
      fHisto1->Add(histo);
      fHisto1->Add(histo_mu);
      trace=fStListMD[num].fTimeProfile;
      
      for(map<int,double>::const_iterator slot=trace.begin();
	  slot!=trace.end(); slot++)
	{
	  if(((*slot).first)<10000)
	    histo->Fill(((*slot).first),(*slot).second);
	  
	}
      
      
      trace_mu=fStListMD[num].fTimeProfile_mu;
      
      for(map<int,double>::const_iterator slot=trace_mu.begin();
	  slot!=trace_mu.end(); slot++)
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
  int nmu=fStListED[num].fNmu;
  if(nmu<20) 
    {
      for (int nm=0;nm<nmu;nm++)
	{
	  cout << "muon " << nm << "\t time = " << fStListED[num].fMuTimes[nm] << endl;
	}
    }
  if(fStListED[num].fNpe>0) 
    {
      fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
      fCanvas=(TCanvas *) fCanvasList->Last();
      
      char*hname = new char[15];
      int idistance = (int)fStListED[num].fR_sf;
      int id = fStListED[num].fId;
      sprintf(hname,"SUMPMT_%04d_%04d",id,idistance);
      
      cout << hname << endl;
      histo=  new TH1F(hname,hname,5000,0.,5000);
      histo_mu=  new TH1F(hname,hname,5000,0.,5000);
      delete hname;
      
      fHisto1->Add(histo);
      fHisto1->Add(histo_mu);
      trace = fStListED[num].fTimeProfile;
      
      for(map<int,double>::const_iterator slot=trace.begin(); slot!=trace.end(); slot++)
	{
	  if(((*slot).first)<10000){
	    histo->Fill(((*slot).first),(*slot).second);
	    cout << "histo -> " << (*slot).first << endl; }
	}
      
      trace_mu=fStListED[num].fTimeProfile_mu;
      
      for(map<int,double>::const_iterator slot=trace_mu.begin(); slot!=trace_mu.end(); slot++)
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
  int nmu=fStListMD[num].fNmu;

  if(nmu<20) 
    {
      for (int nm=0;nm<nmu;nm++)
	{
	  cout<<"muon "<< nm<<"\t time = "<<fStListMD[num].fMuTimes[nm]<<endl;
	}
    }

  ipm--;
  
  if(fStListMD[num].fNpe>0) 
    {
      fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
      fCanvas=(TCanvas *) fCanvasList->Last();
      
      char*hname = new char[15];
      int idistance = (int)fStListMD[num].fR_sf;
      int id = fStListMD[num].fId;
      
      sprintf(hname,"ADC%1d_%04d_%04d",ipm+1,id,idistance);
      
      cout<<hname<<endl;
      histo=  new TH1F(hname,hname,5000,0.,5000);
      histo_mu=  new TH1F(hname,hname,5000,0.,5000);
      delete hname;
      
      fHisto1->Add(histo);
      fHisto1->Add(histo_mu);
      trace=fStListMD[num].fPMT[ipm].fTimeProfile;
	  
      for(map<int,double>::const_iterator slot=trace.begin();
	  slot!=trace.end(); slot++)
	{
	  if(((*slot).first)<10000)
	    histo->Fill(((*slot).first),(*slot).second);
	  
	}

      
      trace_mu=fStListMD[num].fPMT[ipm].fTimeProfile_mu;
      
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
  // short adcsum[2][MAXNUMBEROFADCBINS];
  TH1F* histo;
  TH1F* histo_mu;
  int nmax=MAXNUMBEROFADCBINS;

  /* for(int i = 0;i<MAXNUMBEROFADCBINS;i++)
     for(int j = 0;j<2;j++)
     adcsum[j][i]=0;*/

  gStyle->SetOptStat(1000000); 
  gStyle->SetTitleW(0.5);
  gStyle->SetTitleH(0.08);
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.3);
  int nmu=fStListMD[num].fMuTimes.size();
  if(nmu<20) 
    {
      for (int nm=0;nm<nmu;nm++)
	{
	  cout<<"muon "<< nm<<"\t time = "<<fStListMD[num].fMuTimes[nm]<<endl;
	}
    }
 
  fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
  fCanvas=(TCanvas *) fCanvasList->Last();
  
  char*hname = new char[15];
  int idistance = (int)fStListMD[num].fR_sf;
  int id = fStListMD[num].fId;
  sprintf(hname,"SUMADC_%04d_%04d",id,idistance);
  
  cout<<hname<<endl;
  histo=  new TH1F(hname,hname,400,0.,400);
  histo_mu=  new TH1F(hname,hname,400,0.,400);
  delete hname;
  
  fHisto1->Add(histo);
  fHisto1->Add(histo_mu);
  
  //adcsum=fStListMD[num].fADC;
  for(int k=0; k<nmax; k++)
    if(fStListMD[num].fADC[0][k]!=0){
      histo->Fill(k,fStListMD[num].fADC[0][k]);
    }
  
  //adcsum=fStListMD[num].fADC_mu;
  for(int k=0; k<nmax; k++)
    if(fStListMD[num].fADC_mu[0][k]!=0){
      histo_mu->Fill(k,fStListMD[num].fADC_mu[0][k]);
    }
  
  //histo->SetLineColor(kRed);
  histo_mu->SetLineColor(kCyan);
  //histo->Draw();
  histo_mu->Draw("SAME");
}



void Analyze::DrawOneTrace(int num,int ipm)
{
  // short adcsum[2][MAXNUMBEROFADCBINS];
  
  TH1F* histo;
  TH1F* histo_mu;
  int nmax=MAXNUMBEROFADCBINS ;
  
  /* for(int i = 0;i<MAXNUMBEROFADCBINS;i++)
    for(int j = 0;j<2;j++)
    adcsum[j][i]=0;*/
  
  
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
	  cout<<"muon "<< nm<<"\t time = "<<fStListMD[num].fMuTimes[nm]<<endl;
	}
    }
 
  ipm--;
  
     
      fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
      fCanvas=(TCanvas *) fCanvasList->Last();
      
      char*hname = new char[15];
      int idistance = (int)fStListMD[num].fR_sf;
      int id = fStListMD[num].fId;
      
      sprintf(hname,"ADC%1d_%04d_%04d",ipm+1,id,idistance);
      cout<<hname<<endl;
      histo=  new TH1F(hname,hname,400,0,400);
      histo_mu=  new TH1F(hname,hname,400,0,400);
      delete hname;
      
      fHisto1->Add(histo);
      fHisto1->Add(histo_mu);



      //      adcsum=fStListMD[num].fPMT[ipm].fADC;
      for(int k=0; k<nmax; k++)
	if(fStListMD[num].fPMT[ipm].fADC[0][k]!=0){
	  histo->Fill(k,fStListMD[num].fPMT[ipm].fADC[0][k]);
	}
    
      // adcsum=fStListMD[num].fPMT[ipm].fADC_mu;
      for(int k=0; k<nmax; k++)
	if(fStListMD[num].fPMT[ipm].fADC_mu[0][k]!=0){
	  histo_mu->Fill(k,fStListMD[num].fPMT[ipm].fADC_mu[0][k]);
	}
     
      histo->SetLineColor(kRed);
      histo_mu->SetLineColor(kCyan);
      histo->Draw();
      histo_mu->Draw("SAME");
      
	
   
}
void Analyze::DrawOneTraceInVEM(int num,int ipm,int ichan)
{
  //short adcsum[2][MAXNUMBEROFADCBINS];
  
  TH1F* histo;
  // TH1F* histo_mu;
  int nmax=MAXNUMBEROFADCBINS ;
  
  // for(int i = 0;i<MAXNUMBEROFADCBINS;i++)
  //for(int j = 0;j<2;j++)
  //  adcsum[j][i]=0;
  
  
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
	  cout<<"muon "<< nm<<"\t time = "<<fStListMD[num].fMuTimes[nm]<<endl;
	}
    }
 
  ipm--;
  
     
      fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
      fCanvas=(TCanvas *) fCanvasList->Last();
      
      char*hname = new char[15];
      int idistance = (int)fStListMD[num].fR_sf;
      int id = fStListMD[num].fId;
      
      sprintf(hname,"ADC%1d_%04d_%04d",ipm+1,id,idistance);
      cout<<hname<<endl;
      histo=  new TH1F(hname,hname,400,0,400);
      //     histo_mu=  new TH1F(hname,hname,400,0,400);
      delete hname;
      
      fHisto1->Add(histo);
 //      fHisto1->Add(histo_mu);



      // adcsum=fStListMD[num].fPMT[ipm].fADC;
      cout<<VEMPEAKVALUEINADC<<endl;
      for(int k=0; k<nmax; k++)
	if(fStListMD[num].fPMT[ipm].fADC[ichan][k]!=0){
	  histo->Fill(k,(fStListMD[num].fPMT[ipm].fADC[ichan][k])/VEMPEAKVALUEINADC*32.);
	}
    
      // adcsum=fStListMD[num].fPMT[ipm].fADC_mu;
//       for(int k=0; k<nmax; k++)
// 	if(fStListMD[num].fPMT[ipm].fADC_mu[ichan][k]!=0){
// 	  histo_mu->Fill(k,fStListMD[num].fPMT[ipm].fADC_mu[ichan][k]);
// 	}
     
      histo->SetLineColor(kRed);
      //      histo_mu->SetLineColor(kCyan);
      histo->Draw();
//       histo_mu->Draw("SAME");
      
	
   
}
void Analyze::DrawOneTrace(int num,int ipm,int ichan)
{
  //short adcsum[2][MAXNUMBEROFADCBINS];
  
  TH1F* histo;
  TH1F* histo_mu;
  int nmax=MAXNUMBEROFADCBINS ;
  
  // for(int i = 0;i<MAXNUMBEROFADCBINS;i++)
  //for(int j = 0;j<2;j++)
  //  adcsum[j][i]=0;
  
  
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
	  cout<<"muon "<< nm<<"\t time = "<<fStListMD[num].fMuTimes[nm]<<endl;
	}
    }
 
  ipm--;
  
     
      fCanvasList->Add(new TCanvas("c1","Trace ",400,300));
      fCanvas=(TCanvas *) fCanvasList->Last();
      
      char*hname = new char[15];
      int idistance = (int)fStListMD[num].fR_sf;
      int id = fStListMD[num].fId;
      
      sprintf(hname,"ADC%1d_%04d_%04d",ipm+1,id,idistance);
      cout<<hname<<endl;
      histo=  new TH1F(hname,hname,400,0,400);
      //     histo_mu=  new TH1F(hname,hname,400,0,400);
      delete hname;
      
      fHisto1->Add(histo);
 //      fHisto1->Add(histo_mu);



      // adcsum=fStListMD[num].fPMT[ipm].fADC;
      for(int k=0; k<nmax; k++)
	if(fStListMD[num].fPMT[ipm].fADC[ichan][k]!=0){
	  histo->Fill(k,fStListMD[num].fPMT[ipm].fADC[ichan][k]);
	}
    
      // adcsum=fStListMD[num].fPMT[ipm].fADC_mu;
//       for(int k=0; k<nmax; k++)
// 	if(fStListMD[num].fPMT[ipm].fADC_mu[ichan][k]!=0){
// 	  histo_mu->Fill(k,fStListMD[num].fPMT[ipm].fADC_mu[ichan][k]);
// 	}
     
      histo->SetLineColor(kRed);
      //      histo_mu->SetLineColor(kCyan);
      histo->Draw();
//       histo_mu->Draw("SAME");
      
	
   
}

void Analyze::DrawTraces()
{
  TH1F* histo;
  // TH1F* histo_mu;
  int nmax=MAXNUMBEROFADCBINS;
  
  //short adcsum[2][MAXNUMBEROFADCBINS];
  
  /*  for(int i = 0;i<MAXNUMBEROFADCBINS;i++)
    for(int j = 0;j<2;j++)
    adcsum[j][i]=0;*/
  
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
      // if(sta->fNpe>10) {
        if(sta->fT2ToT||sta->fT1Threshold) 
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

	      
	      sprintf(hname,"SUMADC_%04d_%04d",id,idistance);
	      cout<<hname<<endl;
	      histo=  new TH1F(hname,hname,400,0.,400);
	      //  histo_mu=  new TH1F(hname,hname,400,0.,400);
	      delete hname;
	      
	      fHisto1->Add(histo);
	      // fHisto1->Add(histo_mu);
	      
	      // adcsum=sta->fADC;
	      for(int k=0; k<nmax; k++)
		if(sta->fADC[0][k]!=0){
		  histo->Fill(k,sta->fADC[0][k]);
		  cout<<id<<" "<<k<<" "<<sta->fADC[0][k]<<endl;
		}
	  
	      //adcsum=sta->fADC_mu;
	     //  for(int k=0; k<nmax; k++)
// 		if(sta->fADC_mu[0][k]!=0){
// 		  histo_mu->Fill(k,sta->fADC_mu[0][k]);
// 		}
	      
	 
	      fCanvas->cd(indix);
	      histo->SetLineColor(kRed);
	      //  histo_mu->SetLineColor(kCyan);
	      histo->Draw();
	      //    histo_mu->Draw("SAME");
	      
	
      
      
	      // if( indix%8==0)
// 		{
// 		  fCanvas->Print();
// 		}      
	      
	
	}// end of loop on station
  }
}


void Analyze::DrawTraces(int ipm)
{
  // short adcsum[2][MAXNUMBEROFADCBINS];
  TH1F* histo;
  TH1F* histo_mu;
  int nmax=MAXNUMBEROFADCBINS;

  /*  for(int i = 0;i<MAXNUMBEROFADCBINS;i++)
    for(int j = 0;j<2;j++)
    adcsum[j][i]=0;*/

 
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
	      histo=  new TH1F(hname,hname,400,0.,400);
	      histo_mu=  new TH1F(hname,hname,400,0.,400);
	      delete hname;
	      
	      fHisto1->Add(histo);
	      fHisto1->Add(histo_mu);
	      
	      
	      // adcsum=sta->fPMT[ipm].fADC;
	      for(int k=0; k<nmax; k++)
		if(sta->fPMT[ipm].fADC[0][k]!=0){
		  histo->Fill(k,sta->fPMT[ipm].fADC[0][k]);
		}
	      
	      //adcsum=sta->fPMT[ipm].fADC_mu;
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



void Analyze::DrawTracesInVem()
{
  TH1F* histo;
  TH1F* histo_mu;
  
  int nmax=MAXNUMBEROFADCBINS;
  
  // short adcsum[2][MAXNUMBEROFADCBINS];
  
  /* for(int i = 0;i<MAXNUMBEROFADCBINS;i++)
    for(int j = 0;j<2;j++)
    adcsum[j][i]=0;*/
  
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
		  fCanvas=(TCanvas *) fCanvasList->Last();
		  fCanvas->Divide(2,4);
		  cout<<"new canvas"<<endl;
		  
		} 
	   
	      indix++;
	      
	 
	      
	      
	      char*hname = new char[15];
	      int idistance = (int)sta->fR_sf;
	      int id = sta->fId;

	      
	      sprintf(hname,"SUMADC_%04d_%04d",id,idistance);
	      cout<<hname<<endl;
	      histo=  new TH1F(hname,hname,nmax,0.,nmax);
	      histo_mu=  new TH1F(hname,hname,nmax,0.,nmax);
	      delete hname;
	      
	      fHisto1->Add(histo);
	      fHisto1->Add(histo_mu);
	      
	      //adcsum=sta->fADC;
	      for(int k=0; k<nmax; k++)
		if(sta->fADC[0][k]!=0){
		  histo->Fill(k,sta->fADC[0][k]/VEMPEAKVALUEINADC);
		  sum+=sta->fADC[0][k];
		}
	  
	      //adcsum=sta->fADC_mu;
	      for(int k=0; k<nmax; k++)
		if(sta->fADC_mu[0][k]!=0){
		  histo_mu->Fill(k,sta->fADC_mu[0][k]/VEMPEAKVALUEINADC);
		}
	      
	 
	      fCanvas->cd(indix);
	      histo->SetLineColor(kRed);
	      histo_mu->SetLineColor(kCyan);
	      histo->Draw();
	      histo_mu->Draw("SAME");
	      
	      cout<<"signal of station "<< sta->fId<<" = "<<sum/VEMCHARGEVALUEINADC/(int)NPM<<" VEM"<<endl;

      
      
	      if( indix%8==0)
		{
		  fCanvas->Print();
		}      
	      
	
	}// end of loop on station
    }
}


void Analyze::DrawTracesInVem(int ipm)
{
  // short adcsum[2][MAXNUMBEROFADCBINS];
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
	      histo=  new TH1F(hname,hname,nmax,0.,nmax);
	      // histo_mu=  new TH1F(hname,hname,nmax,0.,nmax);
	      delete hname;
	      
	      fHisto1->Add(histo);
	      // fHisto1->Add(histo_mu);
	      
	      
	      // adcsum=sta->fPMT[ipm].fADC;
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



void Analyze::GetEventMD(Int_t event)
{
  ofstream outfile;
  outfile.open("sum_cuve.out",ios::out);
  if (event>= fTree->GetEntries()) 
    {
      cout<<"Error in Analyze::GetShower() ; only " << (Int_t) fTree->GetEntries() << " events in Tree"<<endl;
      return;  
    }	

   fTree->GetEvent(event);
   fStListMD = fEvent->fHitStationListMD;
  
  cout << " Number of hit tanks ::" <<fStListMD.size()<< " "<<fEvent->fNombTank<<endl;
 
  vector<HitStationMD>::iterator sta;
  int i=0;
 int j=0;
 int k=0;
  for(sta=fStListMD.begin();  sta!= fStListMD.end(); sta++)
    {
    if(sta->fT1Threshold ||sta->fT2ToT ||sta->fT2Threshold)
;
      outfile <<sta->fId <<"\t "<<sta->fR_sf<<"\t"<<sta->fNph<<"\t"<<sta->fNel<<"\t"<<sta->fNmu<<"\t"<<sta->fNpe_ph<<"\t"<<sta->fNpe_el<<"\t"<<sta->fNpe_mu<<" "<<(int)(sta->fNpe_ph/sta->fNph)<<" "<<(int)(sta->fNpe_el/sta->fNel)<<" "<<(int)(sta->fNpe_mu/sta->fNmu)<<" "<<sta->fNpe_mu/(sta->fNpe_el+sta->fNpe_ph)<<endl;

     cout <<i<< "\t tank " <<sta->fId <<"\t pel = "<<sta->fNpe<<         	"\t nmu = "<<sta->fNmu<<"\t nel = "<<sta->fNel<<"\t nph = "<<sta->fNph<<"\t dist = "<<sta->fR_sf<<"\t samp = "<<sta->fSampFact<<"\t time = "<<sta->fTime0<<"\t pemu= "<<sta->fNpe_mu<<"\t peel= "<<sta->fNpe_el<<"\t peph = "<<sta->fNpe_ph<<endl ; 
      i++;    
    }

  int ista=0;
  double moymu=0;
  double moyel=0;
  double moyph=0;
  cout<<endl;
 
  i=0;
  for(sta=fStListMD.begin();  sta!= fStListMD.end(); sta++)
    {
      cout <<i<< "\t tank " <<sta->fId << "  dist_Sf= " << sta->fR_sf <<" \t time ="<<sta->fTime0<<"\t petot= "<<sta->fNpe<< endl ; 
      i++;
    }
}



void Analyze::GetEventED(Int_t event)
{
  ofstream outfile;
  outfile.open("sum_cuve.out",ios::out);
  if (event>= fTree->GetEntries()) 
    {
      cout << "Error in Analyze::GetShower() ; only " << (Int_t) fTree->GetEntries() << " events in Tree " << endl;
      return;  
    }	
 
   cout<<"test"<<endl;
   fTree->GetEvent(event);
   fStListED = fEvent->fHitStationListED;
  
  cout << " Number of hit scintillators ::" <<fStListED.size()<< " "<<fEvent->fNombTank<<endl;
 
  vector<HitStationED>::iterator sta;
  int i=0;
  int j=0;
  int k=0;
  for(sta=fStListED.begin();  sta!= fStListED.end(); sta++)
    {
      if(sta->fT1Threshold ||sta->fT2ToT ||sta->fT2Threshold)
	;
      outfile <<sta->fId <<"\t "<<sta->fR_sf<<"\t"<<sta->fNph<<"\t"<<sta->fNel<<"\t"<<sta->fNmu<<"\t"<<sta->fNpe_ph<<"\t"<<sta->fNpe_el<<"\t"<<sta->fNpe_mu<<" "<<(int)(sta->fNpe_ph/sta->fNph)<<" "<<(int)(sta->fNpe_el/sta->fNel)<<" "<<(int)(sta->fNpe_mu/sta->fNmu)<<" "<<sta->fNpe_mu/(sta->fNpe_el+sta->fNpe_ph)<<endl;

     cout <<i<< "\t scint " <<sta->fId <<"\t pel = "<<sta->fNpe<<         	"\t nmu = "<<sta->fNmu<<"\t nel = "<<sta->fNel<<"\t nph = "<<sta->fNph<<"\t dist = "<<sta->fR_sf<<"\t samp = "<<sta->fSampFact<<"\t time = "<<sta->fTime0<<"\t pemu= "<<sta->fNpe_mu<<"\t peel= "<<sta->fNpe_el<<"\t peph = "<<sta->fNpe_ph<<endl ; 
      i++;    
    }

  
  int ista=0;
  double moymu=0;
  double moyel=0;
  double moyph=0;
  cout<<endl;
 
  i=0;
  for(sta=fStListED.begin();  sta!= fStListED.end(); sta++)
    {
      cout <<i<< "\t scint " <<sta->fId << "  dist_Sf= " << sta->fR_sf <<" \t time ="<<sta->fTime0<<"\t petot= "<<sta->fNpe<< endl ; 
      i++;
    }
}



void Analyze::GetEvents()
{

  ofstream outfile;
  outfile.open("sum_cuve.out",ios::out);
  for (int event=0;event< fTree->GetEntries();event++) 
    {
      
      
      cout<<"test"<<endl;
      fTree->GetEvent(event);
      
      fStListMD = fEvent->fHitStationListMD;
      
      
      
      
      cout << " Number of hit tanks ::" <<fStListMD.size()<< " "<<fEvent->fNombTank<<endl;
      
      vector<HitStationMD>::iterator sta;
      int i=0;
      int j=0;
      int k=0;
      for(sta=fStListMD.begin();  sta!= fStListMD.end(); sta++){
      if(sta->fT1Threshold ||sta->fT2ToT ||sta->fT2Threshold)
	if(sta->fR_sf<2000)
	  outfile <<sta->fId <<"\t "<<sta->fR_sf<<"\t"<<sta->fNph<<"\t"<<sta->fNel<<"\t"<<sta->fNmu<<"\t"<<sta->fNpe_ph<<"\t"<<sta->fNpe_el<<"\t"<<sta->fNpe_mu<<" "<<(sta->fNpe_ph/sta->fNph)<<" "<<(sta->fNpe_el/sta->fNel)<<" "<<(sta->fNpe_mu/sta->fNmu)<<" "<<sta->fNpe_mu/(sta->fNpe_el+sta->fNpe_ph)<<endl;
      
    }
      

    }

}

void Analyze::GetEvents(int ista)
{

  ofstream outfile;
  int nmu,nel,nph,npemu,npeph,npeel;
  float rael,ramu,raph,ramuel,nev;
  outfile.open("sum_cuve.out",ios::out);
  nmu=0;
  nel=0;
  nph=0;
  npeph=0;
  npeel=0;
  npemu=0;
  rael=0;
  ramu=0;
  raph=0;
  ramuel=0;
  nev=(float)fTree->GetEntries();
  for (int event=0;event< fTree->GetEntries();event++) 
    {
      
      
      cout<<"test"<<endl;
      fTree->GetEvent(event);
      
      fStListMD = fEvent->fHitStationListMD;
      
     
      
      
      cout << " Number of hit tanks ::" <<fStListMD.size()<< " "<<fEvent->fNombTank<<endl;
      
      vector<HitStationMD>::iterator sta;
      int i=0;
      int j=0;
      int k=0;
      for(sta=fStListMD.begin();  sta!= fStListMD.end(); sta++){
	if(sta->fId!=ista)continue;
      if(sta->fT1Threshold ||sta->fT2ToT ||sta->fT2Threshold)
	if(sta->fR_sf<2000)
	  //  outfile <<sta->fId <<"\t "<<sta->fR_sf<<"\t"<<sta->fNph<<"\t"<<sta->fNel<<"\t"<<sta->fNmu<<"\t"<<sta->fNpe_ph<<"\t"<<sta->fNpe_el<<"\t"<<sta->fNpe_mu<<" "<<(sta->fNpe_ph/sta->fNph)<<" "<<(sta->fNpe_el/sta->fNel)<<" "<<(sta->fNpe_mu/sta->fNmu)<<" "<<sta->fNpe_mu/(sta->fNpe_el+sta->fNpe_ph)<<endl;
      nmu+=sta->fNmu;
      nel+=sta->fNel;
      nph+=sta->fNph;
      npeph+=sta->fNpe_ph;
      npemu+=sta->fNpe_mu;
      npeel+=sta->fNpe_el;
      rael+=sta->fNpe_el/sta->fNel;
      ramu+=sta->fNpe_mu/sta->fNmu;
      raph+=sta->fNpe_ph/sta->fNph;
      ramuel+=sta->fNpe_mu/(sta->fNpe_el+sta->fNpe_ph);
      

    }
      

    }
  outfile<<ista<<" "<<nph/nev<<" "<<nel/nev<<" "<<nmu/nev<<" "<<npeph/nev<<" "<<npeel/nev<<" "<<npemu/nev<<" "<<raph/nev<<" "<<rael/nev<<" "<<ramu/nev<<" "<<ramuel/nev<<endl;
}



void Analyze::PlotDistances()
{
  TCanvas *CanLTP= new TCanvas("Distances","Distances",600,800);
  CanLTP->SetFillColor(0);
  
  vector<HitStationMD>::iterator sta;  
  TH1F* dist=new TH1F("Distances","Distances in Shower Frame",80,0.,4000); 
  
  
  for (int i=0;i< fChain->GetEntries() ; i++) 
    {
      fChain->GetEntry(i);
      fStListMD = fEvent->fHitStationListMD;
      
      for(sta=fStListMD.begin();  sta!= fStListMD.end(); sta++)
	{
	  dist->Fill(sta->fR_sf,1.);
	  if (sta->fR_sf<200.)
	    cout <<"tank " <<sta->fId << "  dist_Sf= " << sta->fR_sf << endl ; 
	}
    }
  dist->Draw();

}



void Analyze::PrintTree()
{  
  fTree->Print();
}



void Analyze::ReadFile(char * filename)
{
  
  if(fTree!=0)
    { 
      delete fTree;
      fTree=0;
    }
  
  fFile = new TFile(filename,"READ");	// Open file	
  fFile->ls();
  
  fTree = (TTree *) fFile->Get("TreeEvent");      // Get Tree
  
  fTree->Print(); 
  
  if(fBranch!=0)
    { 
      delete fBranch;
      fBranch=0;
    }
  
  
  
  
  
  fBranch= fTree->GetBranch("event");
  fTree->SetBranchAddress("event",&fEvent);
  
  cout << " \t * There is " << fTree->GetEntries() << " events in the input file "<<endl;
  
  // fTree->GetEntry(0);
  
  return;
}




//--------------------------------------------------------------------





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
  int nev=(int)fTree->GetEntries();
  cout<<nev<<" events to use"<<endl;
 
  outfilehi.open("pmhi.dat",ios::out);
  outfilelo.open("pmlo.dat",ios::out);

  for(int ne=0; ne<=nev; ne++)
    {
      fTree->GetEvent(ne);
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
	




void Analyze::TestNewTOT()
{
  HitStationMD* sta;

  char *filename= new char[40];
  float LowThreshold = 0.2*24.;
  float UpThreshold = 0.4*24;
  int WindowSize = 120;
  int NumberOfRealisationL = 10;
  int nentry = (int)fTree->GetEntries();
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
	  
	  
	  fTree->GetEntry(i);
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

