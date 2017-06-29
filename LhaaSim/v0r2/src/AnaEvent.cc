#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TObject.h>
#include <TBranch.h>
#include <TRandom.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>


#include "Station.h"
#include "Event.h"


TROOT root("Main","LHAASO Simulation Program");// ROOT workspace   

using namespace std ;


int main()
{
 
  const string energy[10] = {"1E3","3E3","6E3","1E4","3E4","6E4","1E5","3E5","6E5","1E6"};
  const int theta[5] = {0,25,36,45,53};
  
  // input files
 
  TFile* infile;
  TTree* intree_ev;
  TTree* intree_md;
  TTree* intree_ed;
  HitStationMD* hitstationmd;
  HitStationED* hitstationed;
  //  HitStationLightED* hitstationed;
  Event* event;
  
  //output file
  string  outrootfilename;
  EventRec* eventrec;
  //TH1F* hist = new TH1F(hist,"hist",100,10000);
  
  ifstream inputfile;  
  inputfile.open("defaultana.inp",ios::in);
  outrootfilename="/projet/auger/Diane/LHAASIM/LhaaSim/v0r2/data/TestProton.root";
  
  cout<<outrootfilename<<endl;
  
  TFile * outrootfile = new TFile(outrootfilename.c_str(),"RECREATE");
  TTree* outtree = new TTree("TreeRec","tree for event rec objects"); 
  cout << "1" << endl;
  eventrec = new EventRec;
  TBranch* outbranch = outtree->Branch("eventrec","EventRec",&eventrec,16000,0);
  outbranch->SetAddress(&eventrec);
  delete eventrec;
  
  char name[200];
  int mul; 

  for (int ie=0;ie<1/*10*/;ie++){
    for(int it=0;it<1/*6*/;it++){
      for(int ig=1;ig<100;ig++){
  	mul=0;
       	cout << energy[ie] << " " << theta[it] << " " << ig << endl;
  	//int ttt = sprintf(name,"/projet/auger/Diane/LHAASIM/LhaaSim/v0r2/data/LhaRootProton2/Sim_corsika_%s_%i_%i.root",energy[ie].c_str(),theta[it],ig);
	int ttt = sprintf(name,"/projet/auger/Corsika_6.9/corsika_grid/v6.990.r2427/run/LhaRootProton/Sim_corsika_%s_%i_%i.root",energy[ie].c_str(),theta[it],ig);
	cout << name << endl;
	
	
	infile= TFile::Open(name);
	if(!infile || infile->IsZombie()){
	  cout<<"not found or zombie"<<endl; 
	  continue;
	}

	// Lecture de l'arbre contenant les infos événement :
	intree_ev = (TTree *) infile->Get("TreeEvent"); //Get Tree
	
	if(intree_ev==0){
	  cout<<"file not complete"<<endl;
	  continue;
	}
	event=0;
	intree_ev->Print(); 
	intree_ev->SetBranchAddress("event",&event);  
	cout << " \t * There is " << intree_ev->GetEntries() << " events in the input file (BranchEvent)"<<endl;
	intree_ev->GetEvent(0);
      	cout<<event->fEnergy<<" "<<event->fTheta*180/3.1416<<" "<<endl;
	
	
	
	// // Lecture de l'arbre contenant le détecteur MD :
	intree_md = (TTree *) infile->Get("TreeMD");   // Get Tree
       	intree_md->Print(); 
	hitstationmd=0;
	intree_md->SetBranchAddress("hstationMD",&hitstationmd);
       	cout << " \t * There is " << intree_md->GetEntries() << " events in the input file (inbranchmd)"<<endl;

	// Lecture de l'arbre contenant le détecteur ED :
	intree_ed = (TTree *) infile->Get("TreeED");   // Get Tree
       	intree_ed->Print(); 
	hitstationed=0;
	intree_ed->SetBranchAddress("hstationED",&hitstationed);
       	cout << " \t * There is " << intree_ed->GetEntries() << " events in the input file (inbranched)"<<endl;

       
  	//creation du nouvel evt;
	eventrec = new EventRec(*event);
	
  	cout<<"new object EventRec created"<<endl;
	
  	int nev=(int)intree_ev->GetEntries();
  	int nmd=(int)intree_md->GetEntries();
	int ned=(int)intree_ed->GetEntries();
	
  	cout << " ****** read tree *********" << nev<< "events"<<endl;
	
  	for(int imd=0;imd<nmd;imd++){
	  intree_md->GetEvent(imd);
	  if(hitstationmd->fNmu>0)
	    eventrec->fHitStationListMD.push_back(*hitstationmd);
  	}

	for(int ied=0;ied<ned;ied++){

	  intree_ed->GetEvent(ied);
	  if(hitstationed->fNPartTot>0){
	    HitStationLightED light(*hitstationed);
	    if(light.IsTriggered) eventrec->fHitStationLightListED.push_back(light);
	    cout<<ied<<"new station written"<<endl;
	  }
	}
	cout<<"here"<<endl;
	float th;
	int thmu;
	th=3;
  	eventrec->fNTrigMDStaVEM[th]= eventrec->CountTrigMDStaVEM(th);
 	th=4;
  	eventrec->fNTrigMDStaVEM[th]= eventrec->CountTrigMDStaVEM(th);
  	th=5;
  	eventrec->fNTrigMDStaVEM[th]= eventrec->CountTrigMDStaVEM(th);
	thmu=2;
  	eventrec->fNTrigMDStaMu[thmu]= eventrec->CountTrigMDStaMu(thmu);
	thmu=3;
  	eventrec->fNTrigMDStaMu[thmu]= eventrec->CountTrigMDStaMu(thmu);
	thmu=4;
  	eventrec->fNTrigMDStaMu[thmu]= eventrec->CountTrigMDStaMu(thmu);
	
  	outtree->Fill();
  	cout<<"arbre rempli"<<endl;
	delete eventrec;
  	cout<<" end of loop after shower deleted"<<endl;
	
   
	infile->Close();
    
  	delete infile;
   
      }
    }
  }
  
  

  cout<<"fichier bientot ferme"<<endl;
  outrootfile->Write();
  cout<<"fichier ferme"<<endl;
  
}








