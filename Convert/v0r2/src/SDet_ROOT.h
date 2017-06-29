//----------------------------------------------------------------------------
/*! \file SDet_ROOT.h
    \brief headerfile which define the structure of the Water Tank detectors of P. Auger Observatory
    It defines two classes, the SDStation_ROOT that defines a Local Station and
the SDArray_ROOT that defines the set of Local Stations of one observatory Site.

   \author Sylvie Dagoret-Campagne and P. Billoir
   \date April 2001
   \ingroup DETECTOR
 */
//----------------------------------------------------------------------------


#ifndef SDET_ROOT_H
#define SDET_ROOT_H
#include "TString.h"
#include <string>
#include <vector>
#include <cmath>
using namespace std;
#include "Coord_ROOT.h"
#include "LocalStationConst.h"    
#include "PartGrnd_ROOT.h"
#include "SDPhotoElecCount_ROOT.h" 
#include "SDFadcTrace_ROOT.h" 
#include "SDTriggs_ROOT.h"
const int MAXCHARSTATIONNAME=30;  /**< used to store station name in an array of char */  

//--------------------------------------------------------------------
/*! \class SDStation_ROOT 
    \brief This class defines a local station
    \ingroup DETECTOR
 */
//--------------------------------------------------------------------
class SDStation_ROOT: public TObject
{
 public:
  Int_t id; /**< identifier of the Local Station */
  CartesianCoord_ROOT pos_; /**< cartesian coordinates to be used */
  Short_t flag_; /**< validity flag */
  char name_[MAXCHARSTATIONNAME]; //!< station name 
  Double_t northing_; /**< UTM northing coordinates in meters*/
  Double_t easting_; /**< UTM easting coordinates in meters*/
  Double_t altitude_;/**< UTM altitude coordinates in meters*/

  SDStation_ROOT(){strcpy(name_,"NONE");}//!< default constructor necessary for ROOT I/O
  /*! Standard constructor used to build a SDStation from its UTM coordinates only*/
  SDStation_ROOT(double north,double east,double alt=ALTITUDE_OBSERVATORY,short flag=1,const char * name="none"):flag_(flag),northing_(north),easting_(east),altitude_(alt){pos_=CartesianCoord_ROOT(north,east,alt);strcpy(name_,name);}
  /*! Standard constructor used to build a SDStation from its identifier and its UTM coordinates */
  SDStation_ROOT(int identity, double north,double east,double alt=ALTITUDE_OBSERVATORY,short flag=1,const char * name="none"):flag_(flag),northing_(north),easting_(east),altitude_(alt){ id=identity; pos_=CartesianCoord_ROOT(north,east,alt);strcpy(name_,name);}
  ~SDStation_ROOT(){}; //!< destructor
  void Print(ostream& s=cout) const; //!< print on an output C++ stream
  CartesianCoord_ROOT location() const{return pos_;}
  
  ClassDef(SDStation_ROOT,1)
};
//--------------------------------------------------------------------


//--------------------------------------------------------------------
/*! \function inline ostream& operator<<(ostream& s, const SDStation_ROOT& st)
    \brief print an SDStation_ROOT object on the ostream&s s
*/
//--------------------------------------------------------------------
inline ostream& operator<<(ostream& s, const SDStation_ROOT& st)
{
   st.Print(s);
   return s;
}
//--------------------------------------------------------------------
    
#include <TFile.h>

//--------------------------------------------------------------------
/*! \class SDArray_ROOT 
    \brief This class defines a Surface array made of Local stations (SDStation_ROOT class)
   \ingroup DETECTOR

 */
//--------------------------------------------------------------------    
class SDArray_ROOT: public TObject
{
 public:
  vector<SDStation_ROOT> StationList;/**< container of SDStation_ROOT object */ 
  SDArray_ROOT(){}; /**< default dummy constructor necessary for ROOT I/O */
  /*! Constructor to read a SDArray form a ROOT file which name is given in argument */
  SDArray_ROOT(char *filename)
    {
      TFile *f=new TFile(filename,"READ");
      this->Read("SDArray_ROOT");
      f->Close();
    };
  
  /*!  Constructor to read a SDArray form a ROOT file which name is given in argument  */
  SDArray_ROOT(const string& filename)
    {
      string temp(filename);
      SDArray_ROOT((char*)temp.c_str());
    }

  /*  Build the surface array explicitely */
  virtual void buildArray(int nstat, double eas_min, double nor_min);
  virtual void buildCalibStation();
  void Print(ostream& s=cout) const; //!< print the surface array on output stream
  
  virtual void buildArray(const char *filename);
  virtual void buildShowerArray(double theta=0,double phi=0);

  ~SDArray_ROOT(){}; //!< default destructor

  double minX(){return minX_;};
  double minY(){return minY_;};
  double maxX(){return maxX_;};
  double maxY(){return maxY_;};
  double minZ(){return minZ_;};
  double maxZ(){return maxZ_;};
  double meanX(){return meanX_;};
  double meanY(){return meanY_;};
  double meanZ(){return meanZ_;};
  double widthX(){return widthX_;};
  double widthY(){return widthY_;};
  int nbstations(){return nbstations_;};

 private:
  double minX_;
  double minY_;
  double maxX_;
  double maxY_;
  double minZ_;
  double maxZ_;
  double meanX_;
  double meanY_;
  double meanZ_;
  double widthX_;
  double widthY_;
  int nbstations_;

ClassDef(SDArray_ROOT,1)

  };
//--------------------------------------------------------------------


//--------------------------------------------------------------------
/*! \function inline ostream& operator<<(ostream& s, const SDArray_ROOT& sda)
    \brief print an SDArray_ROOT object on the ostream& s
   \ingroup DETECTOR

*/
//--------------------------------------------------------------------
//--------------------------------------------------------------------
inline ostream& operator<< (ostream& s, const SDArray_ROOT& sda)
{
  sda.Print(s);
  return s;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
/*! \class SDHitStation_ROOT 
    \brief This class defines a Local station hit by a Shower
   \ingroup DETECTOR

 */
//--------------------------------------------------------------------    
class SDHitStation_ROOT: public TObject
{
 public:
  SDStation_ROOT *station_; /**< pointer to the station */
  double Tim0_stat_; /**< arrival time of the wave front in the station */
  double RStatSh;    /**< coordinate of the stations in shower frame */
  double AzStatSh; /**< coordinate of the stations in shower frame */
  double ZStatSh;  /**< coordinate of the stations in shower frame */
  double GPS_time_; /**< GPS time in ns simulated from the local trigger*/
  int munumber;
  vector<int>mutimes;
  vector< PartGrnd_ROOT> partList_; /**< container of particles in the tank */
  SDPhotoElecCount_ROOT photoel_[NBPM+1]; /**< container of photoelectrons recorded in Pm tank */
  SDFadcTrace_ROOT fadc_[NBFADC+1]; /**< container of fadc Local Station */
  vector<SDLocalTrig_ROOT> localTriggers_;/**< container for Local Triggers */ 
  int selectedtriggernum_;
  void Print(ostream& s=cout) const;
  Bool_t status; /**< if status=true, this station is to be considered in the analysis */

  SDHitStation_ROOT(){station_=NULL;} 
  SDHitStation_ROOT(SDStation_ROOT* s) : status(true) { station_=s;} // by default, this station is valid

  SDHitStation_ROOT(SDStation_ROOT* s,const CartesianCoord_ROOT& eventcoord ,double cosThetaShower, double sinThetaShower,double cosPhiShower, double sinPhiShower);
  SDHitStation_ROOT(const SDHitStation_ROOT& s);
  SDHitStation_ROOT& operator=(const SDHitStation_ROOT& s);
  void AddMu(int tmu);
  void ClearMu();
  bool PartInSampleRegion(double RSh, double AzSh,double delt_dist,double delt_azi);
  virtual ~SDHitStation_ROOT();
  void getCalib();
  double vemtonphoto_; /*< calibration constant to convert in photoelectrons */
  double vemtofadch_; /*< calibration constant to convert high gain into vem*/
  double vemtofadcl_; /*< calibration constant to convert low gain into vem */
  double signaloverpeak_; /*< calibration constant Signal/Peak */

  ClassDef(SDHitStation_ROOT,1)
};
//--------------------------------------------------------------------


//--------------------------------------------------------------------
inline ostream& operator<< (ostream&s, const SDHitStation_ROOT& hs)
{
  hs.Print(s);
  return s;
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------
/*! \class ClearHitStation 
    \brief STL object function to remove empty hit stations
   \ingroup DETECTOR

 */
//--------------------------------------------------------------------
class ClearHitStation
{
 public:
  ClearHitStation(){}
  ~ClearHitStation(){}
  bool operator()(SDHitStation_ROOT& h)const
  {
  if(h.partList_.size()==0)
      return true;
  else
      return false;
  }
};
//--------------------------------------------------------------------

//--------------------------------------------------------------------
unsigned int MatrixId (double easting, double northing); 
//--------------------------------------------------------------------



#endif

