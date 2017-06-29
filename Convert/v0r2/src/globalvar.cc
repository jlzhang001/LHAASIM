#include "TRandom.h"
#include "ObsAuger_ROOT.h"  // defines the site observatory
#include "Shower_ROOT.h"//defines the shower 
#include "Parameters.h" // The parameters obtained from the input config file
using namespace std;
//Params parameters;  // contains selected input parameters (sample type)

Shower_ROOT* shower;        // the shower
//ObsAuger_ROOT* observatory; // the surface array
//Parameters* P;       
const int NBGEN=5; 
TRandom r[NBGEN]; // array of random generators

