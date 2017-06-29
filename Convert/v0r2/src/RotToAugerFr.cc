//-------------------------------------------------------------------
/*! \file RotToAugerFr.cc
    \brief implementation file of  RotToAugerFr.h
    \author Sylvie Dagoret-Campagne

*/
//-------------------------------------------------------------------
#include "RotToAugerFr.h"

RotToAuger rot;

//---------------------------------------------------------------------
double RotToAuger::rotAzimuth(double oldAz)
{
  double AzAng=oldAz-M_PI_2-MAGDECLI*M_PI/180.;

  if(AzAng>2*M_PI)
    AzAng-=2*M_PI;

  if(AzAng<0)
    AzAng+=2*M_PI;

  return AzAng; 
}
//---------------------------------------------------------------------

//---------------------------------------------------------------------
void RotToAuger:: rotMoment(double& px,double& py, double& pz)
{
  double Px; double Py; double Pz;

  Px= SIND*px-COSD*py; 
  Py=COSD*px+SIND*py; 
 
  if(pz>=0)
    Pz=-pz;
  else
    Pz=pz;

  px=Px;
  py=Py;
  pz=Pz;
  return;

}
//---------------------------------------------------------------------


//---------------------------------------------------------------------
void RotToAuger:: rotPart(double& x, double& y, double& z,double& ux,double& uy,double& uz)
{
  double X,Y,Z,UX,UY,UZ;
  UX=SIND*ux-COSD*uy;
  UY=COSD*ux+SIND*uy;
  if(uz>=0) 
    UZ=-uz;
  else
    UZ=uz;

  X=COSROT*x+SINROT*y;
  Y=-SINROT*x+COSROT*y;
  Z=z;

  // return values by reference
  x=X;
  y=Y;
  z=Z;
  ux=UX;
  uy=UY;
  uz=UZ;

  return;

}
//---------------------------------------------------------------------
