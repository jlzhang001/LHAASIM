//-----------------------------------------------------------------------
/*! \file dumpShower.cc
    \brief dump the Shower content produced by AiresToROOT or CorsToROOT
    \author Sylvie Dagoret-Campagne
    \date 10 octobre 2001
 */
//-----------------------------------------------------------------------
#include <iostream>
#include <Shower_ROOT.h>
#include <PartGrnd_ROOT.h>
#include <TROOT.h>
#include <TFile.h>
#include <TObject.h>
#include "TCanvas.h"
#include "TVector3.h"
#include "TArrayD.h"
#include "TF1.h"
#include "TH2.h"
#include "TArrow.h"
#include "TLine.h"
#include "TGaxis.h"
#include "TView.h"
#include "TMarker.h"
#include "TPolyMarker3D.h"
#include "TPolyLine3D.h"
#include "TRint.h"
#include "TList.h"
#include "TProfile2D.h"
TFile* file;

double OBSERVATORYLEVEL;
double TIMESHIFT;

TROOT root("ShowParticels", "Show Particles at ground");

extern void InitGui();//initializer for GUI needed for interactive interface

VoidFuncPtr_t Initfuncs[]={InitGui,0};

const int MAXPARTICLES=10000;

int main(int argc, char *argv[])
{

  cout << " argc= " << argc << endl;
  cout << " argv[0]= " << argv[0] << endl;
  cout << " argv[1]= " << argv[1] << endl;
  

  if(argc<=1 || argc>2)
    {
      cerr << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;  
      cerr << "Usage : " << argv[0] << " Shower-Root-filename " << endl;
      cerr << "    or: " << argv[0] << " Grnd-Root-filename " << endl;  
      cerr << " \t where  Shower-Root-filename is the ROOT file containing both the Ground particles of the showers and the profile " << endl;
      cerr << " \t or   Grnt-Root-filename is the ROOT file containing the Ground particles  " << endl;
      cerr << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;  

      exit(1);
    }

  cout << " \t   Plot Particles  of Shower from a ROOT file :" << endl;
  cout << " \t   --------------------------------" << endl;


  cout << "   \t - The Shower must have be produced by AiresToROOT or by CorsToRoot" << endl;
  cout << " \t   - Selected input file : " << argv[1] << endl;



  switch (argc)
    {
    case 1:
      cerr << " ++++ Input file missing " << endl;
      exit(1);
      break;
    case 2:
      cerr  << " The selected input file is " << argv[1] << endl;
      break;
    default:
      cerr << " ++++ Too mmany arguments " << endl;
      exit(1);
      break;      
    };



  file=new TFile(argv[1],"R");
  file->ls();


  
  TList list;

  TObject* p; 


  if( (p=(Shower_ROOT*)file->Get("Header_ROOT"))!=0)
    {      
      cout << "\t Header_ROOT object found " << endl;
    }

  double theta=((Header_ROOT*)p)->GetPrimaryZenAng();
  double phi=((Header_ROOT*)p)->GetPrimaryAzAng();
  cout << " THETA= " << 180/M_PI*theta << endl;
  cout << " PHI= " << 180/M_PI*phi << endl;


  double UX=sin(theta)*cos(phi);
  double UY=sin(theta)*sin(phi);
  double UZ=cos(theta);

 



  cout << "-------------------------------------------------------------"<< endl;
  cout << " \t NOW START WITH GROUND PARTICLES  " << endl;

  ProgName program=((Header_ROOT*)p)->ProgFlag;

  Shower_ROOT* shower;


  shower=new Shower_ROOT(argv[1]);
  shower->showerGrnd()->QuickLook() ;
  double ux,uy,uz;
  double x,y,z,x1,y1,z1,w,t;
  int nbparticles=(int)shower->showerGrnd()->GetEntries();
  int id;

  double XMIN=1000000;
  double YMIN=1000000;
  double XMAX=-100000;
  double YMAX=-100000;
  double ZMIN=1000000;
  double ZMAX;
  double MAX=0;
  double TMIN = 1000000;
  double TMAX = -1000000;



  for(int i=0;  i<nbparticles; i++)
    {
      PartGrnd_ROOT* p= shower->showerGrnd()->GetPart(i);
      double x=p->GetX();
      double y=p->GetY();
      double z=p->GetZ();
      double t=p->GetT();
      if(XMIN>x) XMIN=x;
      if(YMIN>y) YMIN=y;
      if(ZMIN>z) ZMIN=z;
      if(XMAX<x) XMAX=x;
      if(YMAX<y) YMAX=y;
      if(abs(x)>MAX) MAX=abs(x);
      if(abs(y)>MAX) MAX=abs(y);
      if(t<TMIN) TMIN=t;
      if(t>TMAX) TMAX=t;

    }// end if loop on pcles

  ZMAX=MAX/5;

  cout << " X    : " << XMIN << "  --  " << XMAX << endl;
  cout << " Y    : " << YMIN << "  --  " << YMAX << endl;
  cout << " ZMIN : " << ZMIN << endl;
  cout << " ZMAX : " << ZMAX << endl;
  cout << " MAX  : " << MAX  << endl;
  cout << " T    : " << TMIN << " -- " << TMAX << endl;



  TRint *theApp=new TRint("ROOT-SDSIM",&argc,argv);

  /* ************************************************************************/
  /*  This canvas 1 plots the direction of arrival of each particles        */
  /*  at AUGER ground level  !                                              */
  /*  at bins corresponding to their impact point location                  */
  /*  A 3D ROOT view is taken for that !                                    */
  /* ************************************************************************/


  TCanvas* canvas1=new TCanvas("Particlesin3D","The particles at ground",800,800);
  list.Add(canvas1);


  canvas1->cd();


  TView *view = new TView(1);

  view->SetRange(-MAX,-MAX,ZMIN,MAX,MAX,ZMAX);
  view->ShowAxis();
  view->Side();
  list.Add(view);


  // loop on particles in the shower
  for(int i=0;  i<nbparticles; i++)
    {
      if(i>MAXPARTICLES) break;

      PartGrnd_ROOT* p= shower->showerGrnd()->GetPart(i);
      
      TPolyLine3D*  particleaxis= new TPolyLine3D(2);
      id=p->GetId();
      x=p->GetX();
      y=p->GetY();
      z=p->GetZ();
      w=p->GetW();
      ux=p->GetUX();
      uy=p->GetUY();
      uz=p->GetUZ();
      x1=x+ux/uz*(ZMAX-ZMIN)/4.;
      y1=y+uy/uz*(ZMAX-ZMIN)/4.;
      z1=z+(ZMAX-ZMIN)/4.;

      particleaxis->SetPoint(0,x,y,z);
      particleaxis->SetPoint(1,x1,y1,z1);
      particleaxis->SetLineWidth(.5);
      
      switch (id)
	{
	case MUp: case MUm:case HADRONS:
	   particleaxis->SetLineColor(kBlue);
	   break;
	case GAMMAS: case POSITRONS: case ELECTRONS:
	   particleaxis->SetLineColor(kGreen);
	   break;
	default:
	  particleaxis->SetLineColor(kBlack);
	  break;
	};
      particleaxis->Draw();
      list.Add(particleaxis);


    }// end if loop on pcles
  


  // draw the shower axis in Red
  TPolyLine3D*  showeraxis= new TPolyLine3D(2);
  showeraxis->SetPoint(0,0,0,0);
  showeraxis->SetPoint(1,UX/UZ*ZMAX,UY/UZ*ZMAX,ZMAX);
  showeraxis->SetLineWidth(5);
  showeraxis->SetLineColor(kRed);
  showeraxis->Draw();
  list.Add(showeraxis);

  // draw the projected shower axis in Red
  TPolyLine3D*  projectedshoweraxis= new TPolyLine3D(2);
  projectedshoweraxis->SetPoint(0,0,0,0);
  projectedshoweraxis->SetPoint(1,MAX*cos(phi),MAX*sin(phi),0);
  projectedshoweraxis->SetLineWidth(3);
  projectedshoweraxis->SetLineColor(kRed);
  projectedshoweraxis->Draw();
  list.Add(projectedshoweraxis);


  // draw the usual X,Y,Z axis in Black
  TPolyLine3D*  xaxis= new TPolyLine3D(2);
  TPolyLine3D*  yaxis= new TPolyLine3D(2);
  TPolyLine3D*  zaxis= new TPolyLine3D(2);

  xaxis->SetPoint(0,-MAX*1.2,0,0);
  xaxis->SetPoint(1,MAX*1.2,0,0);
  xaxis->SetLineWidth(4);
  xaxis->SetLineColor(kBlack);
  xaxis->Draw();
  list.Add(xaxis);

  yaxis->SetPoint(0,0,-MAX*1.2,0);
  yaxis->SetPoint(1,0,MAX*1.2,0);
  yaxis->SetLineWidth(4);
  yaxis->SetLineColor(kBlack);
  yaxis->Draw();
  list.Add(yaxis);

  zaxis->SetPoint(0,0,0,0);
  zaxis->SetPoint(1,0,0,ZMAX*1.3);
  zaxis->SetLineWidth(4);
  zaxis->SetLineColor(kBlack);
  zaxis->Draw();
  list.Add(zaxis);
  
  canvas1->Modified();
  canvas1->Draw();



  /* ************************************************************************/
  /*  This canvas 2 shows the the location of each particles                */ 
  /* at the AUGER ground level                                              */
  /*  A 2D Histogram is taken for that !                                    */
  /* ************************************************************************/

 
  TCanvas* canvas2=new TCanvas("PartDist","The particles at ground",500,500);
  list.Add(canvas2);
  canvas2->SetGridx();
  canvas2->SetGridy();
  canvas2->cd();

  TH2F* histo=new TH2F("histo"," distribution of impact points at ground ",100,XMIN,XMAX,100,YMIN,YMAX);

  // loop on particles
  for(int i=0;  i<nbparticles; i++)
    {
      if(i>MAXPARTICLES) break;

      PartGrnd_ROOT* p= shower->showerGrnd()->GetPart(i);
      x=p->GetX();
      y=p->GetY();
      w=p->GetW();
      histo->Fill(x,y,w);
    }// end if loop on pcles
  histo->Draw("CONT");
  list.Add(histo);

  // draw the projected shower axis on the ground 
  double xx1=MAX*cos(phi); double yy1=MAX*sin(phi);
  double xx2=-MAX*cos(phi); double yy2=-MAX*sin(phi);

  // the big axis is the shower axis, it is an arraw which has the
  // arrival direction of the shower
  TArrow* bigaxis=new TArrow(xx1,yy1,xx2,yy2);
  bigaxis->Draw();
  list.Add(bigaxis);

  double xx3=MAX*cos(phi+M_PI_2); double yy3=MAX*sin(phi+M_PI_2);
  double xx4=-MAX*cos(phi+M_PI_2); double yy4=-MAX*sin(phi+M_PI_2);

  TLine* smallaxis=new TLine(xx3,yy3,xx4,yy4);
  smallaxis->Draw();
  list.Add(smallaxis);
  

  canvas2->Modified();
  canvas2->Draw();

 
  /* ************************************************************************/
  /*  This canvas 3 shows the location of each particles                    */
  /*  at the AUGER ground level  weighted by their energy                   */
  /*  A 2D Histogram is taken for that !                                    */
  /* ************************************************************************/

  TCanvas* canvas3=new TCanvas("PartDist2","The particles at ground weighted by E ",500,500);
  list.Add(canvas3);
  canvas3->cd();
  canvas3->SetGridx();
  canvas3->SetGridy();

  TH2F* histo2=new TH2F("histo2"," distribution of impact points weighted by E at ground ",100,XMIN,XMAX,100,YMIN,YMAX);
  // loop on particles
  for(int i=0;  i<nbparticles; i++)
    {
      if(i>MAXPARTICLES) break;

      PartGrnd_ROOT* p= shower->showerGrnd()->GetPart(i);
      x=p->GetX();
      y=p->GetY();
      w=p->GetW();
      histo2->Fill(x,y,p->GetE());
    }// end if loop on pcles
  histo2->Draw("COLZ");
  list.Add(histo2);

  // draw the projected shower axis
  bigaxis->Draw();
  smallaxis->Draw();


  canvas3->Modified();
  canvas3->Draw();
 
  /* ************************************************************************/
  /*  This canvas 4 shows arrival time of each particles versus             */
  /*  the location of each particles                                        */
  /*  at the AUGER ground level.                                            */
  /*  A 2D Profile Histogram is taken for that !                            */
  /*  Note that 2D profile in ROOT works only if bin content is positive    */
  /*  That is why the time shown is shifted with the earliest time  TMIN    */ 
  /* ************************************************************************/



  TCanvas* canvas4=new TCanvas("PartTime","The particles arrrival time   ",500,500);
  list.Add(canvas4);
  canvas4->cd();
  canvas4->SetGridx();
  canvas4->SetGridy();
  TProfile2D* hprof=new TProfile2D("hprof"," distribution of particle arrival time ",100,XMIN,XMAX,100,YMIN,YMAX,0,TMAX-TMIN);

  // loop on particles
  for(int i=0;  i<nbparticles; i++)
    {
      if(i>MAXPARTICLES) break;

      PartGrnd_ROOT* p= shower->showerGrnd()->GetPart(i);
      x=p->GetX();
      y=p->GetY();
      t=p->GetT();
      w=p->GetW();
      hprof->Fill(x,y,t-TMIN,1);
    }// end if loop on pcles

  //  hprof->Draw("CONT");
  hprof->Draw("COLZ");
  list.Add(hprof);
  // draw on top of it the shower axis
  bigaxis->Draw();
  smallaxis->Draw();


  canvas4->Modified();
  canvas4->Draw();
 

  /* ************************************************************************/
  /*  This canvas 3 plots the arrival time of each particles                */
  /*  at AUGER ground level  !                                              */
  /*  at bins corresponding to their impact point location                  */
  /*  A 3D ROOT view is taken for that !                                    */
  /* ************************************************************************/

 
  TCanvas* canvas5=new TCanvas("PartTime3D","The particles arrrival time 3D  ",500,500);
  list.Add(canvas5);
  canvas5->cd();


  TView *view2 = new TView(1);
  view2->ShowAxis();
  view2->Side();

  view2->SetRange(-MAX,-MAX,TMIN-100,MAX,MAX,TMAX);

  list.Add(view2);

  TPolyMarker3D*  particles= new TPolyMarker3D(nbparticles);

  // loop on particles
  for(int i=0;  i<nbparticles; i++)
    {
      if(i>MAXPARTICLES) break;

      PartGrnd_ROOT* p= shower->showerGrnd()->GetPart(i);
      
      id=p->GetId();
      x=p->GetX();
      y=p->GetY();
      z=p->GetZ();
      t=p->GetT();

      particles->SetPoint(i,x,y,t);
      particles->SetMarkerSize(.1);
      particles->SetMarkerStyle(8);

      switch (id)
	{
	case MUp: case MUm:case HADRONS:
	   particles->SetMarkerColor(kBlue);
	   break;
	case GAMMAS: case POSITRONS: case ELECTRONS:
	   particles->SetMarkerColor(kGreen);
	   break;
	default:
	  particles->SetMarkerColor(kBlack);
	  break;
	};

  
    }// end if loop on pcles

  particles->Draw();
  list.Add(particles);



  // draw the principal shower axis on the ground in Red
  TPolyLine3D*  newprojectedshoweraxis= new TPolyLine3D(2);
  newprojectedshoweraxis->SetPoint(0,-MAX*cos(phi),-MAX*sin(phi),0);
  newprojectedshoweraxis->SetPoint(1,MAX*cos(phi),MAX*sin(phi),0);
  newprojectedshoweraxis->SetLineWidth(3);
  newprojectedshoweraxis->SetLineColor(kRed);
  newprojectedshoweraxis->Draw();
  list.Add(newprojectedshoweraxis);


  // draw a vertical axis in black
  TPolyLine3D*  timeaxis= new TPolyLine3D(2);
  timeaxis->SetPoint(0,0,0,0);
  timeaxis->SetPoint(1,0,0,TMAX);
  timeaxis->SetLineWidth(3);
  timeaxis->SetLineColor(kBlack);
  timeaxis->Draw();
  
  list.Add(timeaxis);

  canvas5->Modified();
  canvas5->Draw();
 
  

  

  cout << " Remark : Canvas Plotting could be long , be patient !" << endl;
  cout << " QUIT ROOT TO STOP IN ONE OF THE CANVAS! " << endl;  
 
  theApp->Run();


  list.Delete();

  exit(0);



}
