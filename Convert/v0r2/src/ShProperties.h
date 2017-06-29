//------------------------------------------------------------------
/*! \file ShProperties.h
    \brief Classes holding informations about shower or particles computed by the program not stored in a ROOT file
    \author Sylvie Dagoret-Campagne 
 */
//------------------------------------------------------------------


#ifndef SHPROPERTIES_H
#define SHPROPERTIES_H

#include <iostream>
using namespace std;
//--------------------------------------------------------------
/*! \class ShowerProperties
    \brief This class pre-compute the Cosinus directors of a shower
 */
//--------------------------------------------------------------
class ShowerProperties
{
 public:
  ShowerProperties();
  void computeCosDir(double th,double ph);
  double cosTheta(){return cosTheta_;}
  double sinTheta(){return sinTheta_;}
  double cosPhi(){return cosPhi_;}
  double sinPhi(){return sinPhi_;}
  virtual ~ShowerProperties(){}
  void Print(ostream& )const;
 private:
  double cosTheta_;
  double sinTheta_;
  double cosPhi_;
  double sinPhi_;
  
};
//--------------------------------------------------------------

//--------------------------------------------------------------
/*! \class PartProperties
    \brief properties of a particle in a Shower
 */
//--------------------------------------------------------------

class PartProperties
{
 public:
  PartProperties(){}
  ~PartProperties(){}
  void computeLocation(ShowerProperties* sh,double xp,double yp ,double zp);
  double rSh()const{return rSh_;} 
  double azSh()const{return azSh_;}
  double xSh()const{return xSh_;}
  double ySh()const{return ySh_;}
  double zSh()const{return zSh_;}

  void Print(ostream& ) const;
 private:
  double rSh_; // coordinate in the Shower frame
  double azSh_;
  double xSh_;
  double ySh_;
  double zSh_;

};
//--------------------------------------------------------------

ostream& operator<<(ostream& ,const ShowerProperties&);
ostream& operator<<(ostream& ,const PartProperties&);
//--------------------------------------------------------------


#endif
