//----------------------------------------------------------------
/*! \file Header_ROOT.h
    \brief File describing a Shower Header
    \author Sylvie Dagoret-Campagne according X. Bertou and Gilles S original
    \date July 2001
    \ingroup SHOWER
 */
//----------------------------------------------------------------

/**
 * \defgroup SHOWER The Shower Data structure
 *  Set of classes describing the structure of an Air-Shower
 */


#ifndef HEADER_ROOT_H
#define HEADER_ROOT_H
enum ProgName { CORSIKA, AIRES };
using namespace std;
#include "root.h"

//--------------------------------------------------------------
/*! \class Header_ROOT
    \brief Class describing the header of a shower
    \ingroup SHOWER
 */
//--------------------------------------------------------------
class Header_ROOT:public TObject 
{
 private:
 public:
  ProgName ProgFlag;
  char MotherProgram[50];
  char ShowerName[10];
  virtual float GetPrimaryType() = 0;
  virtual float GetPrimaryEnGev() = 0;
  virtual float GetPrimaryThin() = 0;
  virtual float GetPrimaryZenAng() = 0;
  virtual float GetPrimaryAzAng() = 0;
  virtual float GetPrimaryPx() = 0;
  virtual float GetPrimaryPy() = 0;
  virtual float GetPrimaryPz() = 0;
  virtual char *GetDateOfCreation() = 0;


//statistics
  virtual float GetNbGAM(char W) = 0;
  virtual float GetNbEp(char W) = 0;
  virtual float GetNbEm(char W) = 0;
  virtual float GetNbMUp(char W) = 0;
  virtual float GetNbMUm(char W) = 0;
  virtual float GetNbHad(char W) = 0;
  virtual float GetNbPart(char W) = 0;
  
  Header_ROOT() {};
  virtual ~Header_ROOT() {};

  ClassDef(Header_ROOT, 2)
};
//--------------------------------------------------------------


#include "HeaderCorsika_ROOT.h"
//#include "HeaderAires_ROOT.h"

#endif
