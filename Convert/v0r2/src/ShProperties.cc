#include "ShProperties.h"
#include <cmath>
#include <iostream>

ShowerProperties::ShowerProperties()
{
  cosTheta_=0;
  sinTheta_=0;
  cosPhi_=0;
  sinPhi_=0;
}



void ShowerProperties::computeCosDir(double theta,double phi)
{
  cosTheta_=cos(theta);
  sinTheta_=sin(theta);
  cosPhi_=cos(phi);
  sinPhi_=sin(phi);

}


void ShowerProperties::Print(ostream& s) const
{
  s << " *** ShowerProperties::cosTheta sinTheta cosPhi sinPhi :" ;
  s << cosTheta_ << " , ";
  s << sinTheta_ << " , ";
  s << cosPhi_  << " , ";
  s << sinPhi_ << endl;
  
}

void PartProperties::computeLocation(ShowerProperties* sh,double xp,double yp ,double zp)
{
  xSh_=(xp*sh->cosPhi()+yp*sh->sinPhi())*sh->cosTheta()-zp*sh->sinTheta();
  ySh_= yp*sh->cosPhi()-xp*sh->sinPhi();
  zSh_= (xp*sh->cosPhi()+yp*sh->sinPhi())*sh->sinTheta()+zp*sh->cosTheta();
  rSh_=sqrt(pow(xSh_,2)+pow(ySh_,2));
  azSh_=atan2(ySh_,xSh_);

}

void PartProperties::Print(ostream& s) const
{
 s << " *** PartProperties:: xSh ySh zSh rSh azSh :" ;
 s <<  xSh_ << " , ";
 s <<  ySh_ << " , ";
 s <<  zSh_ << " , ";
 s <<  rSh_ << " , ";
 s <<  azSh_ << endl;


}

ostream& operator<<(ostream& s, const ShowerProperties& sh)
{
  sh.Print(s);
  return s;
}
ostream& operator<<(ostream& s, const PartProperties& p)
{
  p.Print(s);
  return s;
}
