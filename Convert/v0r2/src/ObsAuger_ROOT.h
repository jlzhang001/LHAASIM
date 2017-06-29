//----------------------------------------------------------------------------
/*! \file ObsAuger_ROOT.h
    \brief headerfile which define the structure one site
of P. Auger Observatory

   \author Sylvie Dagoret-Campagne
   \date June 2001
   \ingroup DETECTOR
 */
//----------------------------------------------------------------------------


#ifndef OBSAUGER_ROOT_H
#define OBSAUGER_ROOT_H
#include <string>
#include <vector>
#include "SDet_ROOT.h" // Surface array part

/**
 * \defgroup DETECTOR The Pierre Auger Detector
 * Data Structure describing the detector  
 */

//--------------------------------------------------------------------
/*! \class ObsAuger_ROOT.h 
    \brief This class defines the full observatory
    \ingroup DETECTOR
 */
//--------------------------------------------------------------------
class ObsAuger_ROOT: public TObject
{
 public:
  ObsAuger_ROOT(); //!< default dummy constructor necessary for ROOT I/O
  ObsAuger_ROOT(string sitename); //!< standard constructor
  virtual ~ObsAuger_ROOT(); //!< destructor
  void Print(ostream& s=cout) const; //!< print obs on output stream 
  SDArray_ROOT* surfaceArray()const{return surfaceArray_;} //!< reference on stations
 private:
  string sitename_; /**< site variable name */
  SDArray_ROOT* surfaceArray_ ; /**< the surface array reference */

ClassDef(ObsAuger_ROOT,1)
};
//--------------------------------------------------------------------


//--------------------------------------------------------------------    
inline ostream& operator<<(ostream& s, const ObsAuger_ROOT& o)
{
   o.Print(s);
   return s;
}
//--------------------------------------------------------------------

#endif
