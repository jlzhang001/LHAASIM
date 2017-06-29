
/*
  LhaaSimConfig.cc

  implementation file for class LhaaSimConfig

*/


#include "LhaaSimConfig.h"

//----------------------------------------------------------------
/*
  singleton bouzin
*/
//----------------------------------------------------------------
LhaaSimConfig::LhaaSimConfig()
{

}
LhaaSimConfig *LhaaSimConfig::_instance = new LhaaSimConfig();
LhaaSimConfig *LhaaSimConfig::Instance()
{
  return _instance;
}
LhaaSimConfig* theConfig()
{
  return LhaaSimConfig::Instance();
}
