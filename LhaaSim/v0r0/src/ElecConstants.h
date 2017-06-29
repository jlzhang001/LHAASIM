#ifndef __ELECCONSTANTS_H
#define __ELECCONSTANTS_H

//CONSTANTS RELATED TO ELECTRONIC SIMULATIONS

// for PMT simulation
const int  PHOTOELCHARGEPROBSIZE = 49;
const int  PHOTOELPULSESHAPESIZE = 100;
const int  PHOTOELPULSESTARTBIN  = 31;

const double  PMGAINFORPULSESHAPE = 1.e6 ;
const double  PMGAIN              = 4.3e5 ;
const double  DYNODETOANODE       = 32.;
const double  PMTRANSITTIME       = 0; //ns

const double PHOTOELCHARGEPROB[PHOTOELCHARGEPROBSIZE] = 
{0.196005 , 0.341731 , 0.414712 , 0.472212 , 0.518365 , 
 0.558558 , 0.592884 , 0.623269 , 0.653654 , 0.68 , 
 0.705769 , 0.734039 , 0.757211 , 0.779904 , 0.802308 , 
 0.826923 , 0.848365 , 0.869519 , 0.890385 , 0.911346 , 
 0.932019 , 0.952885 , 0.975019 , 0.995865 , 1.01558 , 
 1.03548 , 1.05596 , 1.07663 , 1.09875 , 1.12067 , 
 1.1426  , 1.16443 , 1.18712 , 1.21327 , 1.23817 , 
 1.26365 , 1.29509 , 1.32279 , 1.35519 , 1.38693 , 
 1.41923 , 1.45596 , 1.49471 , 1.53606 , 1.58182 , 
 1.63452 , 1.70269 , 1.79192 , 1.9301 };


 //pulseshape in mVolts
const double  PHOTOELPULSESHAPE[PHOTOELPULSESHAPESIZE] =
{
-0.0238 , -0.253906 , -0.0527 , 0.31836 , 0.294532 ,
-0.04605 , -0.174218 , -0.207031 , 0.150001 , 0.219141 ,
0.146875 , -0.221484 , -0.122656 , 0.156641 , 0.267188 ,
0.176172 , -0.196874 , -0.235937 , 0.197266 , 0.255078 ,
0.138672 , -0.126184 , -0.0746 , 0.077344 , 0.05275 ,
0.079672 , 0.03785 , 0.0914 , 0.0262 , 0.091425 ,
0.136719 , 0.267578 , 0.313672 , 0.438672 , 1.28438 ,
3.60938 , 8.53399 , 14.5972 , 20.0203 , 22.9289 ,
23.4246 , 22.5672 , 20.6551 , 18.0867 , 15.1516 ,
12.3296 , 9.93555 , 7.93281 , 6.3914 , 5.06954 ,
4.08477 , 3.37265 , 3.0418 , 2.69883 , 2.43515 ,
1.94492 , 1.65743 , 1.60898 , 1.56211 , 1.46953 ,
1.16992 , 0.923438 , 1.00938 , 0.9625 , 1.02422 ,
0.796094 , 0.663281 , 0.694141 , 0.769141 , 0.791407 ,
0.651953 , 0.515235 , 0.616016 , 0.704297 , 0.822266 ,
0.736329 , 0.709766 , 0.657032 , 0.719531 , 0.627344 ,
0.515625 , 0.464453 , 0.421094 , 0.432032 , 0.402344 ,
0.391407 , 0.362891 , 0.291406 , 0.308204 , 0.307032 ,
0.339844 , 0.258985 , 0.257422 , 0.260937 , 0.237891 ,
0.182031 , 0.130079 , 0.153907 , 0.191016 , 0.204688 
};
//time in ns
const int  PHOTOELPULSESHAPETIME[PHOTOELPULSESHAPESIZE] =
{-42 , -41 , -40 , -39 , -38 , -37 , -36 , -35 , -34 , -33 ,
-32 , -31 , -30 , -29 , -28 , -27 , -26 , -25 , -24 , -23 ,
-22 , -21 , -20 , -19 , -18 , -17 , -16 , -15 , -14 , -13 ,
-12 , -11 , -10 , -9 , -8 , -7 , -6 , -5 , -4 , -3 ,
-2 , -1 , 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 ,
8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 , 17 ,
18 , 19 , 20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 ,
28 , 29 , 30 , 31 , 32 , 33 , 34 , 35 , 36 , 37 ,
38 , 39 , 40 , 41 , 42 , 43 , 44 , 45 , 46 , 47 ,
 48 , 49 , 50 , 51 , 52 , 53 , 54 , 55 , 56 , 57 };

//for Front end simulation
const double FEDCOEF[6] = 
{-2.88315e-05 , 4.37981 , -7.68523 , 6.75313 , -2.97163 , 0.523853 };


//for ADC sampling 
const int  NBFADCCHANNEL= 1023;

#endif // __ELECCONSTANTS_H
