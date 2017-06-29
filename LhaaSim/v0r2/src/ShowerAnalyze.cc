#include <TROOT.h>
#include <TFile.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TRandom.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include "SDShower.h"
#include "ShowerAnalyze.h" 
#include "Array.h"
#include "Event.h"
#include "AnaConstants.h"
#include "TFile.h"
#include "TH1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TArrayD.h"
#include "TF1.h"
#include "TF2.h"



const int NBGEN=2;  
TRandom r[NBGEN]; // array of random generators   

TROOT root("Main","PAO Simulation Program");// ROOT workspace   

using namespace std ;




int main()
{
 
  string filename;
  string  outrootfilename;
  int NbOfFiles;
   
  short* trace;   
  short* trace_mu;
  short* trace_em;
  
  
  SDShower* shower;
  Tank* tank;
  ZeroTank *ztank;
  
  vector<Station>* StationsList;

  ifstream inputfile;  
  inputfile.open("default.inp",ios::in);
  inputfile>>outrootfilename;
  inputfile>>NbOfFiles;
  

  
  
  cout<<outrootfilename<<" "<< NbOfFiles<<" fichiers a traiter"<<endl;
  
  TFile * outrootfile= new TFile(outrootfilename.c_str(),"RECREATE");
  TTree* outtree = new TTree("TreeShower","tree for shower objects"); 
  cout << "1" << endl;
  shower = new SDShower;
  TBranch* outbranch =outtree->Branch("shower","SDShower",&shower,16000,0);
   cout << "2" << endl;
  outbranch->SetAddress(&shower);
 cout << "3" << endl;
  delete shower;
  
  for(int iii=0;iii<NbOfFiles;iii++){
    
    //CDF showers...
    string Primary;
    double Energy;
    double Theta;
    //double Phi;
    int ShowerIndex;
    inputfile>>filename;
    inputfile>> Primary>>Energy>>Theta>>ShowerIndex;	
    
    //cout<<"lecture parametres ok"<<endl;
    //cout<<Primary<<" "<<ShowerIndex<<" "<<endl;
    
    //Defining and Reading the root file 
    //char* hname1;
    //hname1 = new char[100];
    //cout<<filename<<" "<<Primary<<" "<<Energy<<" "<<Theta<<endl;
    //sprintf(hname1,"%s_%s_%2d_%02d_%d.root",filename.c_str(),primary.c_str(),Energy,Theta,ShowerIndex);
    //sprintf(hname1,"%s_%s_%d_%d_%d.root",filename.c_str(),Primary.c_str(),Energy,Theta,ShowerIndex);
    //cout<<"File to read"<<hname1<<endl;
    //string  inrootfilename = hname1;

   

   
    
    //TFile* inrootfile = new TFile(inrootfilename.c_str(),"READ");  // Open file	
    TFile* inrootfile = new TFile(filename.c_str(),"READ");  // Open file	
    inrootfile->ls();
    cout << "4" << endl;
    //Array* array= new Array;

//      TTree* arraytree = (TTree *) inrootfile->Get("TreeArray");      // Get Tree
//      arraytree->Print();  
//      TBranch* arraybranch =  arraytree->GetBranch("array");
//      arraybranch->SetAddress(&array);
    
//      cout << " \t * There is " << arraytree->GetEntries() << " array in the input file "<<endl;
//      arraytree->GetEvent(0);
//      cout<<"array loaded"<<endl;
//      

    //vector<Station>* StationsList =&(array->fStationList);
    ShowerParam *gShParamP;
    gShParamP=new ShowerParam();
    gShParamP->Theta=0.*DEG2RAD;
    gShParamP->Phi=54.*DEG2RAD;
    gShParamP->CosTheta=cos(Theta);
    gShParamP->CosPhi=cos(Phi);
    gShParamP->SinTheta=sin(Theta);
    gShParamP->SinPhi=sin(Phi);
    gShParamP->Energy=1.;
    
    gShParamP->EasCore=0.;
    gShParamP->NorCore=0.;
  
    cout << "4,5" << endl;
    ifstream * in1;
    in1=new ifstream("/projet/auger/EASYSIM/ShowerAna/v1r0/cmt/stararray_E18_tta0_phi54",ios::in);
    if (!in1)
      cout << "fichier mal ouvert" << endl;
    Double_t xsh=0.;
    Double_t ysh=0.;
    Int_t id=0;
    Double_t east=0.;
    Double_t north=0.; 
    
    while (!in1->eof())
      {
	*in1 >> id >> xsh >> ysh >> east >> north ;  
	cout << "id " << id << "  xsh" << xsh << "  ysh " << ysh << "  east" << east << " north "<< north << endl;
	//Station * sta= new Station (id,north,east,1400.);
	
	//StationsList->push_back(*sta);
	cout << "5" << endl;
	//delete sta;
	cout << "6" << endl;
      }
    in1->close();
    cout << "toto" << endl;
    Event* event = new Event;
    TTree* intree = (TTree *) inrootfile->Get("TreeEvent");      // Get Tree
    intree->Print(); 
    TBranch* inbranch =  intree->GetBranch("event");
    inbranch->SetAddress(&event);
    
    int NbOfEntries=(int)intree->GetEntries();
    
    cout << " ****** read tree *********" << NbOfEntries<< "events"<<endl;
    
    for(int icore=0;icore<NbOfEntries;icore++){
      
      cout<<"******	******************************"<<endl;
      cout<<"new Event "<<icore<<endl;
      cout<<"******	******************************"<<endl;
      
      //creation de la nouvelle gerbe
      cout<<"avant event accede "<<icore<<endl;
      intree->GetEvent(icore);
      cout<<"event accede "<<icore<<endl;
      cout<<"core "<<event->fEasCore<<" "<<event->fNorCore<<endl;
      
      double Phi =RAD2DEG*(event->fAzim + event->fPhiRot);
      cout << "event->fAzim= " <<RAD2DEG*event->fAzim << "  event->fPhiRot=  "<< RAD2DEG*event->fPhiRot << endl;
      double EasCore = event->fEasCore;
      double NorCore = event->fNorCore;
      
      shower = new SDShower(Primary,Energy,Theta,Phi,EasCore,NorCore,ShowerIndex,icore);
      cout<<"new object SDShower created             "<<shower->fCoreIndex<<endl;

      
      int NbOfTanks=0;
      
      vector<HitStation>* HitStationsList=&event->fHitStationList;
      double DistToCore;
      cout<< HitStationsList->size()<< " stations potentially hit"<<endl;

      //adding zero tanks
      int nst=0;
      for(  vector<Station>::iterator s=StationsList->begin();s!=StationsList->end();s++){	
	
	DistToCore= sqrt(pow(EasCore-s->fEasting,2)+pow((NorCore-s->fNorthing),2));
	
	int flaghit=0;
	for(vector<HitStation>::iterator hs=HitStationsList->begin();hs!=HitStationsList->end();hs++){
	  if(hs->fNorthing==s->fNorthing &&
	     hs->fEasting==s->fEasting)
	    flaghit+=1;
	}

	if(flaghit==0 && DistToCore<6000.){
	  ztank=new ZeroTank(s->fId,s->fEasting,s->fNorthing);
	  shower->Add(ztank);
	  delete ztank;
	  nst++;
	}
      }
      cout<<nst<< "  zero tanks added "<<endl;
      
      //adding hit tanks
      for(vector<HitStation>::iterator hs=HitStationsList->begin();hs!=HitStationsList->end();hs++){
	if(hs->fT1Threshold || hs->fT2ToT){
	  //ajout d'un tank
	  int id = hs->fId;
	  double distancetoaxis =  hs->fR_sf;
	  double azimut  =  hs->fAzim_sf;
	  double easting = hs->fEasting;
	  double northing = hs->fNorthing ;
	 
	  //cout<<endl<<" new tank ";
	  //cout<<id<<" "<<easting<<" "<<northing<<" " <<azimut<<endl;
	  //time = sdHitStation->Tim0_stat_;
	  
	  
	  tank = new Tank( id,distancetoaxis,azimut,easting,northing);
	  
	  // Check saturation of  PMTs ( saturation = 1 if at least one trace is saturated)
	  
	  
	  int saturation = tank->IsSaturate(hs->fPMT[0].fADC[0],
					    hs->fPMT[1].fADC[0],
					    hs->fPMT[2].fADC[0]);
	  int channel=saturation;
	  
	  // Reading ADC Traces in the correct channel  	  
	 
	  trace=hs->fADC[channel];
	  trace_mu=hs->fADC_mu[channel];
	  trace_em=hs->fADC_em[channel];
	  tank->WriteTraces(trace,trace_mu,trace_em,saturation);
	  for (int ipm=0; ipm<3; ipm++)
	    {
	      trace=hs->fPMT[ipm].fADC[channel];
	      trace_mu=hs->fPMT[ipm].fADC_mu[channel];
	      trace_em=hs->fPMT[ipm].fADC_em[channel];
	      tank->fADC[ipm].WriteTraces(trace,trace_mu,trace_em,saturation);
	    }
	  

	  // Reading data on muons ( number and time) 
	  
	  tank->WriteT2(hs->fT2ToT,hs->fT2Threshold);
	  tank->WriteMu(hs->fNmu);
	  
	  for (vector<int>::iterator mutimes= hs->fMuTimes.begin();mutimes != hs->fMuTimes.end();mutimes++)
	    tank->AddMu(*mutimes);
	  
  //execution des fonctions specifiques aux tanks
	  
	  
	  tank->FirstBin();
	  tank->CodeUpDown();
	  tank->Scaling(SHAPE_DISTMIN);
	  tank->Integrate();
	  tank->DoT50();
	  tank->DoT90();
	  tank->SetTime((int)hs->fTime0);
	  shower->Add(tank);
	  NbOfTanks++;	 
	  //cout<<"nouveau tank ajoute    "<<NbOfTanks<<" "<<tank->fId<<" "<<tank->fT2ToT<<" "<<tank->fT2Threshold<<endl;
	  //cout<<shower->fTanksList.back().fDistToAxis<<endl;
	}
	else
	  { 
	    ztank = new ZeroTank(hs->fId,hs->fEasting,hs->fNorthing);
	    shower->Add(ztank);
	    delete ztank;
	  } 
	
	
      }//end of loop on tanks
      
      //execution of functions on showers
      shower->CountTank();
      int removed=shower->TankSelection();
      cout<<removed<<" stations removed from TankSelection"<<endl;
      shower->CountTank();
      //cout<<"premiere fonction executee"<<endl;
      
      //shower->SumScaledTraces( SHAPE_DISTMIN, SHAPE_DISTMAX);
      //shower->SumTracesInDistanceBins(); 
      //shower->Shape();
      //shower->Summary();
      // shower->SetName();
      //shower->DoT50();
     
      //NOUVEAU RECONSTRUCTION DE LA GERBE SIMULEE!!!!!!!!!!!!!!!
     
      //int flagrec=shower->Reconstruct();
      outbranch->SetAddress(&shower);
      //if(flagrec>0)cout<<" reconstruction failed code = "<<flagrec<<endl;
      cout<<"arbre bientot rempli"<< endl;
      outtree->Fill();
      cout<<"arbre rempli"<<endl;
      
      delete shower;
      cout<<" end of loop after shower deleted"<<endl;
      
    } //end of loop on entries in inrootfile
    
    //delete hname1;
    delete inrootfile;
   
    
    
    
    
  }  //end of loop on files

  cout<<"fichier bientot ferme"<<endl;
  outrootfile->Write();
  cout<<"fichier ferme"<<endl;
  
}








