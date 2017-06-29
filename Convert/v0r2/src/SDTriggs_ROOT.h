//------------------------------------------------------------
/*! \file SDTriggs_ROOT.h
    \brief Header file for trigger definitions
    \author Sylvie Dagoret-Campagne
    \date july 2001 (last update)

 */
//------------------------------------------------------------

#ifndef _TRIGGERS_
#define _TRIGGERS_

#include "TObject.h"
#include <vector>
#include <iostream>

//--------------------------------------------------------------

/*! \class SDLocalTrig_ROOT
    \brief Class containing the result of the Local Trigger
*/
//--------------------------------------------------------------
class SDLocalTrig_ROOT : public TObject
{
 public:
  SDLocalTrig_ROOT():binnumber_(0),triggernumber_(0),level_(0){}
  SDLocalTrig_ROOT(int bin,int num,int level);
  SDLocalTrig_ROOT(const SDLocalTrig_ROOT& t);
  SDLocalTrig_ROOT& operator=(const SDLocalTrig_ROOT& t);
  virtual ~SDLocalTrig_ROOT();
  int binnumber_;
  int triggernumber_;
  int level_;
  void Print(ostream& o=cout) const;
ClassDef(SDLocalTrig_ROOT,1)
};
//--------------------------------------------------------------

//-------------------------------------------
ostream& operator<<(ostream& o,const SDLocalTrig_ROOT& t);
//-------------------------------------------

#endif
