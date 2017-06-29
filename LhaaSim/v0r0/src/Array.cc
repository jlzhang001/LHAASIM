
/*
  Array.cc

  implementation file for class Array

*/


#include "Array.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <strstream>
#include "LhaaSimConfig.h"
//#include "Utils.h"


ClassImp(Array)

Array *gArrayMD;
Array *gArrayED;

Array::Array(vector<Station> tanklist)
{
  Double_t north,east;
  fStationList= tanklist;
  fNorMin=99999999;
  fNorMax=0;
  fEasMin=99999999;
  fEasMax=0;
  Int_t size = fStationList.size();
  for (Int_t i=0;i<size;i++){
    north = fStationList[i].fNorthing;
    east = fStationList[i].fEasting;
    if(north>fNorMax)fNorMax = north;
    if(north<fNorMin)fNorMin = north;  
    if(east>fEasMax)fEasMax = east;
    if(east<fEasMin)fEasMin = east;     
  }
  fNorMean = (fNorMax+fNorMin)/2. ;
  fEasMean = (fEasMax+fEasMin)/2. ;
  fNorWidth = fNorMax-fNorMin ;
  fEasWidth = fEasMax-fEasMin ;
}




Array::Array(Int_t size,int EDMD)
{
  fNorMin=99999999;
  fNorMax=0;
  fEasMin=99999999;
  fEasMax=0;
   Int_t k = 101;
  Int_t NMIN = 6077556;
  Int_t EMIN = 455129;
  Int_t n;
  ofstream stafileED;
  stafileED.open("stationsdefaultED.txt",ios::out);
  ofstream stafileMD;
  stafileMD.open("stationsdefaultMD.txt",ios::out);
  
  if(EDMD==1)
    {
      n=(Int_t)sqrt(24.*size); // size of array = f(Nb_tank,Nb_scint). init=2*
      
      // cout<<"Write a default array of "<<size<<" stations"<<endl;
      for(Int_t i=0;i<n;i++){
	for(Int_t j=0;j<n;j+=2){
	  Station * sta= new Station (k,NMIN + i*sqrt(30*30-15*15),EMIN+j*15+15*(i%2),ALTITUDE_OBSERVATORY); 
	  stafileMD<<k<<"\t"<<NMIN + i*2*sqrt(30*30-15*15)<<"\t"<<EMIN+j*15+15*(i%2)<<"\t"<< ALTITUDE_OBSERVATORY<<endl;
	  k++;
	  
	  fStationList.push_back(*sta);
	  delete sta;
	  //cout<< "station = "<< k <<endl;
	}
      }
      
      fNorMax = NMIN + n * sqrt(30*30-15*15);
      fNorMin = NMIN;  
      fEasMax = EMIN + n * 15;
      fEasMin = EMIN;
    }
  
  else if(EDMD==2)
    {
      n=(Int_t)sqrt(24.*size);
      
      // cout<<"Write a default array of "<<size<<" stations"<<endl;
      for(Int_t i=0;i<n;i++)
	{
	  for(Int_t j=0;j<n;j+=2) 
	    {
	      Station * sta= new Station (k,NMIN + i * sqrt(15*15-7.5*7.5),EMIN+j*7.5+7.5*(i%2),ALTITUDE_OBSERVATORY); 
	      stafileED<<k<<"\t"<<NMIN+i*12.99038<<"\t"<<EMIN+j*7.5+7.5*(i%2)<<"\t"<< ALTITUDE_OBSERVATORY<<endl;
	      k++;
	      
	      fStationList.push_back(*sta);
	      delete sta;
	      //cout<< "station = "<< k <<endl;
	    }
	}
      
      fNorMax = NMIN + n * sqrt(15*15-7.5*7.5);
      fNorMin = NMIN;  
      fEasMax = EMIN + n * 7.5;
      fEasMin = EMIN;
    }
   
  fNorMean = (fNorMax+fNorMin)/2.;
  fEasMean = (fEasMax+fEasMin)/2.;
  fNorWidth = fNorMax-fNorMin ;
  fEasWidth = fEasMax-fEasMin;
  
  stafileED.close();
  stafileMD.close();
}


Array::Array(string arrayfilename)
{
  Int_t id;
  Double_t north,east;
  string name,endline;
  ifstream arrayfile;
  fNorMin=99999999;
  fNorMax=0;
  fEasMin=99999999;
  fEasMax=0;

  cout<<endl<<"Reading array in file = "<< arrayfilename<<endl<<endl;
  
  int nsta=0;
  
  arrayfile.open(arrayfilename.c_str(),ios::in);
  
  if(arrayfile==0)	
    {	
      cout << "++ error in opening file " << arrayfilename.c_str()  << endl;
      exit(1);
    }		
  
  cout<<"stations coordinates in Northing Easting mode"<<endl;
 
  while(!arrayfile.eof())
    {      
      arrayfile>>id>>east>>north;
      nsta++;
      cout<<id<<" id "<<north<<" entre "<<east<<" ini"<<endl;

      Station *sta =new Station(id,north,east);
      fStationList.push_back(*sta);
      delete sta;
      //cout<< "station = "<<id <<endl;
      
      if(north>fNorMax)fNorMax = north;
      if(north<fNorMin)fNorMin = north;  
      if(east>fEasMax)fEasMax = east;
      if(east<fEasMin)fEasMin = east; 
    }
    
  fNorMean = (fNorMax+fNorMin)/2. ;
  fEasMean = (fEasMax+fEasMin)/2. ;
  fNorWidth = fNorMax-fNorMin ;
  fEasWidth = fEasMax-fEasMin ;
  cout<<"array build with "<<nsta<<" stations"<<endl;
  cout<<fNorMin<<" "<<fNorMax<<" "<<fEasMin<<" "<<fEasMax<<endl;
}

Array::Array(Double_t theta,Double_t phi,Double_t E0,Double_t N0)
{
  const Int_t NBCIRCLES=20;
  const Double_t MINRADIUS=200.;
  const Double_t NBPOINTSINCIRCLE=12;
  const Double_t STEPINRADIUS=100;
  Double_t r;
  Double_t east;
  Double_t north; 
  Double_t altitude;
  fNorMin=99999999;
  fNorMax=0;
  fEasMin=99999999;
  fEasMax=0;
  
  Int_t nbstations;
 
  ofstream stafile;
  stafile.open("stararray.txt",ios::out);
  cout<<theta<<" "<<phi<<endl;
  for(Int_t i=0; i< NBCIRCLES; i++)
    {      
      r= (Double_t)(MINRADIUS + i * STEPINRADIUS);
      for(Int_t j=0; j<  NBPOINTSINCIRCLE;j++)
	{
	  nbstations++;
	
	  altitude=0.;
	  //	Double_t angle=Double_t(j+.5)*2*M_PI/Double_t(NBPOINTSINCIRCLE);
	  Double_t angle=Double_t(j)*2.*PI/Double_t(NBPOINTSINCIRCLE);
	  Double_t xsh=r*cos(angle)/cos(theta);
	  Double_t ysh=r*sin(angle);
	  Int_t id=100*(i+1)+j+1;
	  //  east=E0+xsh*cos(theta)*cos(phi)-ysh*sin(phi);
	  // north=N0+ xsh*sin(phi)*cos(theta)+ysh*cos(phi); 
	
	  east=E0+xsh*cos(phi)-ysh*sin(phi)+altitude*tan(theta)*cos(phi);
	  north=N0+ xsh*sin(phi)+ysh*cos(phi)+altitude*tan(theta)*sin(phi); 
	             
	  Station * sta= new Station (id,north,east,ALTITUDE_OBSERVATORY);
	  stafile<< id << " " << xsh << " "<< ysh <<" "<< east << " " << north << endl;
	       
	  fStationList.push_back(*sta);
	  delete sta;	  
	}
    }
  
  
  if(north>fNorMax)fNorMax = north;
  if(north<fNorMin)fNorMin = north;  
  if(east>fEasMax)fEasMax = east;
  if(east<fEasMin)fEasMin = east; 
  fNorMean = (fNorMax+fNorMin)/2. ;
  fEasMean = (fEasMax+fEasMin)/2. ;
  fNorWidth = fNorMax-fNorMin ;
  fEasWidth = fEasMax-fEasMin ;
	
  stafile.close();
    
}

void Array::Fill(Station* sta)  
{
  fStationList.push_back(* sta); 
}

Int_t Array::Size()
{
  Int_t size;
  size = fStationList.size();
  cout<<endl;
  cout<<"\tArray of " <<size << " stations"<<endl;
  return size;
}



