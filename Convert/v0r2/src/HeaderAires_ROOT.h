//-------------------------------------------------------------------
/*! \file HeaderAires_ROOT.h
    \brief Describe the inhalt of a Header of a shower produced by Aires
   
 */
//-------------------------------------------------------------------



#ifndef HEADERAIRES_ROOT_H
#define HEADERAIRES_ROOT_H
#include "root.h"
#include "Header_ROOT.h"
#include "math.h"
//--------------------------------------------------------------
/*! \class  HeaderAires_ROOT
    \brief Header for shower produced by Aires
 */
//--------------------------------------------------------------

class HeaderAires_ROOT:public Header_ROOT { private:
 public:
  //retest 1 int
  //float ZcoordFI;
  int PrimaryType;
  float PrimaryEnergy;
  float PrimaryZenithAngle;
  float PrimaryAzimuthAngle;
  float ThinningEnergy;
  float FirstInteractionDepth;
  float TimeShift;
  float GroundLevel;
  float XMax;
  float NMax;

  char DateOfCreation[50];

  HeaderAires_ROOT() {
  };
  virtual ~HeaderAires_ROOT() {
  };
  
  //rest 1 int
  /*float GetZcoordFI() {
  return (float) ZcoordFI;
  };
  */
  float GetPrimaryType() {
    return (float) PrimaryType;
  };
  float GetPrimaryEnGev() {
    return PrimaryEnergy;
  };
  float GetPrimaryThin() {
    return ThinningEnergy;
  };
  float GetPrimaryZenAng() {
    return PrimaryZenithAngle;
  };
  float GetPrimaryAzAng() {
    return PrimaryAzimuthAngle;
  };

  float GetPrimaryPx() {
    return PrimaryEnergy*sin(PrimaryZenithAngle)*cos(PrimaryAzimuthAngle+M_PI);
  };

  float GetPrimaryPy() {
    return PrimaryEnergy*sin(PrimaryZenithAngle)*sin(PrimaryAzimuthAngle+M_PI);
  };

  float GetPrimaryPz() {
    return -PrimaryEnergy*cos(PrimaryZenithAngle);
  };

  char *GetDateOfCreation() {
    return DateOfCreation;
  };
  
  float GetNbGAM(char W) {
    return 0.;
  };
  float GetNbEp(char W) {
    return 0.;
  };
  float GetNbEm(char W) {
    return 0.;
  };
  float GetNbMUp(char W) {
    return 0.;
  };
  float GetNbMUm(char W) {
    return 0.;
  };
  float GetNbHad(char W) {
    return 0.;
  };
  float GetNbPart(char W) {
    return 0.;
  };
  
  ClassDef(HeaderAires_ROOT, 1)
};

#endif
