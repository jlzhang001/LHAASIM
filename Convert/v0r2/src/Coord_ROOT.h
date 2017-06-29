//----------------------------------------------------------------------------
/*! \file Coord_ROOT.h
    \brief headerfile which define the structure of the coordinate system of
a Local Station
   \author Sylvie Dagoret-Campagne
   \date May 2001
   \ingroup DETECTOR

 */
//----------------------------------------------------------------------------


#ifndef COORD_ROOT_H
#define COORD_ROOT_H

#include <iostream>
#include <string>
#include "TObject.h"
#include "Globals.h"



class UToC_Coeff 
{
 public:
  UToC_Coeff(int ReferenceEllipsoid=UTM_ELLIPSE_NUM,double Lat=LATITUDE_CENTER, double Long=LONGITUDE_CENTER , double Altit =ALTITUDE_OBSERVATORY , double distx=SD_EAST_SIZE , double disty=SD_NORTH_SIZE);

  double UToC_E0()const {return UToC_E0_;}
  double UToC_N0() const {return UToC_N0_;}
  double UToC_alpha()const {return UToC_alpha_;}
  double UToC_beta()const {return UToC_beta_;}
  double UToC_gamma()const {return UToC_gamma_;}
  void Print(ostream& s=cout) const;
 private:
  double  UToC_E0_;
  double  UToC_N0_;
  double  UToC_alpha_;
  double  UToC_beta_;
  double  UToC_gamma_;
};



//-----------------------------------------------------------------------
void LLtoUTM(int ReferenceEllipsoid, double Lat, double Long, 
	     double &UTMNorthing, double &UTMEasting, char* UTMZone);
//-----------------------------------------------------------------------
void UTMtoLL(int ReferenceEllipsoid, double UTMNorthing, double UTMEasting, const char* UTMZone,
	     double& Lat,  double& Long );
//-----------------------------------------------------------------------
char UTMLetterDesignator(double Lat);
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
void LLtoCart(int ReferenceEllipsoid, double Lat, double Long, double Altit, 
              double LatOrigin, double LongOrigin, double AltitOrigin,
              double& x, double& y,double& z);
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
/*!
\class Ellipsoid
*/
//-----------------------------------------------------------------------

class Ellipsoid
{
public:
  Ellipsoid(){};
  Ellipsoid(int Id, char* name, double radius, double ecc)
  {
    id = Id; ellipsoidName = name; 
    EquatorialRadius = radius; eccentricitySquared = ecc;
  }
  
  int id;
  char* ellipsoidName;
  double EquatorialRadius; 
  double eccentricitySquared;  
  
};

//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
static const Ellipsoid ellipsoid[] = 
{
  //  id, Ellipsoid name, Equatorial Radius, square of eccentricity	
  Ellipsoid( -1, "Placeholder", 0, 0),//placeholder only, To allow array indices to match id numbers
  Ellipsoid( 1, "Airy", 6377563, 0.00667054),
  Ellipsoid( 2, "Australian National", 6378160, 0.006694542),
  Ellipsoid( 3, "Bessel 1841", 6377397, 0.006674372),
  Ellipsoid( 4, "Bessel 1841 (Nambia) ", 6377484, 0.006674372),
  Ellipsoid( 5, "Clarke 1866", 6378206, 0.006768658),
  Ellipsoid( 6, "Clarke 1880", 6378249, 0.006803511),
  Ellipsoid( 7, "Everest", 6377276, 0.006637847),
  Ellipsoid( 8, "Fischer 1960 (Mercury) ", 6378166, 0.006693422),
  Ellipsoid( 9, "Fischer 1968", 6378150, 0.006693422),
  Ellipsoid( 10, "GRS 1967", 6378160, 0.006694605),
  Ellipsoid( 11, "GRS 1980", 6378137, 0.00669438),
  Ellipsoid( 12, "Helmert 1906", 6378200, 0.006693422),
  Ellipsoid( 13, "Hough", 6378270, 0.00672267),
  Ellipsoid( 14, "International", 6378388, 0.00672267),
  Ellipsoid( 15, "Krassovsky", 6378245, 0.006693422),
  Ellipsoid( 16, "Modified Airy", 6377340, 0.00667054),
  Ellipsoid( 17, "Modified Everest", 6377304, 0.006637847),
  Ellipsoid( 18, "Modified Fischer 1960", 6378155, 0.006693422),
  Ellipsoid( 19, "South American 1969", 6378160, 0.006694542),
  Ellipsoid( 20, "WGS 60", 6378165, 0.006693422),
  Ellipsoid( 21, "WGS 66", 6378145, 0.006694542),
  Ellipsoid( 22, "WGS-72", 6378135, 0.006694318),
  Ellipsoid( 23, "WGS-84", 6378137, 0.00669438)
};

//-----------------------------------------------------------------------
/*! \function void ComputeCoeff_UTMtoCart(int ReferenceEllipsoid, 
	      double Lat, double Long, double Altit, double distx, double disty)
    \brief this function compute once for all some coefficients used to
     translate local station UTM coordinates into cartesian coordinates`.  	      

 */
//-----------------------------------------------------------------------
void ComputeCoeff_UTMtoCart(int ReferenceEllipsoid, 
	      double Lat, double Long, double Altit, double distx, double disty);
//-----------------------------------------------------------------------



//---------------------------------------------------------------------
/*!
  \class CartesianCoord_ROOT
  \brief This class defines the coordinates a Station in a cartesian system 
  
 */
//---------------------------------------------------------------------
class  CartesianCoord_ROOT :public TObject
{
 public:
  CartesianCoord_ROOT(){}//!< default empty constructor
  CartesianCoord_ROOT(double northing,double easting,double altitude=ALTITUDE_OBSERVATORY);//!< constructor from UTM coordinates 
  virtual ~CartesianCoord_ROOT(){}
  Float_t radialDistance(const CartesianCoord_ROOT& coord,double cosThetaShower, double sinThetaShower,double cosPhiShower, double sinPhiShower) const;//!< radial distance to the shower axis
  Float_t radialDistance2(const CartesianCoord_ROOT& coord,double cosThetaShower, double sinThetaShower,double cosPhiShower, double sinPhiShower) const;//!< radial distance to the shower axis for impact point computation
  void setCoordinates(double x,double y,double z=0); //!< fix the coordinates
  Float_t distance(const CartesianCoord_ROOT&) const;
  Double_t x() const {return x_;} //!< return the cartesian x
  Double_t y() const {return y_;} //!< return the cartesian y
  Double_t z() const {return z_;} //!< return the local cartesian altitude
  Double_t zc() const {return zc_;} //!< return the central cartesian altitude
  Double_t northing() const;
  Double_t easting() const;

  void Print(ostream& s =cout) const;
 private:
 
  Double_t x_;/**< units in meters, direction toward east */ 
  Double_t y_;/**< units in meters, direction toward north */
  Double_t z_;/**< altitude along the local vertical relative to OBSERVATORY_ALTITUDE */
  Double_t zc_;/**< altitude along the Auger reference center vertical relative to the OBSERVATORY_ALTITUDE */
  ClassDef(CartesianCoord_ROOT,1)
    
};

//-----------------------------------------------------------------------

inline ostream& operator<< (ostream& s, const CartesianCoord_ROOT& c)
{
  c.Print(s);
  return s;
}
//-----------------------------------------------------------------------









#endif
