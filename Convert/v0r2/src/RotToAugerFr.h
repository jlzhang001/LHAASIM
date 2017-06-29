//-------------------------------------------------------------------
/*! \file RotToAugerFr.h
    \brief function to Rotage particles and axis from Air Shower Frame to Auger Frame
    \author Sylvie Dagoret-Campagne

*/
//-------------------------------------------------------------------
#ifndef _ROTTOAUGERFR_H_
#define _ROTTOAUGERFR_H_
#include <cmath>
// few physical definition 
const double MAGDECLI = 4.233333; // magnetic declination in degree 
                                  // magnetic field is 4.2 degrees 
                                  // toward east with respect to 
                                  // geographic north
// rotation matrix elements from Corsika to Auger conventions
const double COSROT=cos(M_PI_2*(-1.+MAGDECLI/90.));
const double SINROT=sin(M_PI_2*(-1.+MAGDECLI/90.));
const double COSD=cos(MAGDECLI/180.*M_PI);
const double SIND=sin(MAGDECLI/180.*M_PI);


//---------------------------------------------------------------------
class RotToAuger
{
 public:
  double rotAzimuth(double oldAz);
  void rotMoment(double& px,double& py, double& pz);
  void rotPart(double& x, double& y, double& z,double& ux,double& uy,double& uz);
};
//---------------------------------------------------------------------
#endif

