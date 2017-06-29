#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <stdio.h>
#include <cstdlib>
#include <cctype>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>

using namespace std;

#define LINESIZE 300
#define MAXPARAMLENGTH 50
#define DEFAULT_FILE "default.inp"
#define CONT 1
#define EXIT 0

void usage();
void sort_remove_duplicated(vector<char>&);
ostream& operator<<(ostream& s, const vector<char>&);
void det_flags(int argc, char** argv, string& params_file, string& filename);
void read_params(const string& filename,
		 vector<string>& keywords,
		 vector<double>& values);
class Params
{

 private:

  double _D_NORTH;
  double _D_EAST;
  double _NORTH_MIN;
  double _EAST_MIN;
  unsigned int _N_EVENT;
  unsigned int _I_PRINT;
  double _GLOB_WEIGHT;
  double _DELT_DIST;
  double _DELT_AZI;
  double _DISTMAX_STATION;
  unsigned int _N_CLONE;
  unsigned int _SD_SIM;
  unsigned int _FD_SIM;
  unsigned int _DEBUG_LEVEL;
 public:

  Params();
  Params(const string& filename);
  Params(const string& keyword, double value);
  void Set(const string& keyword, double value);
  void Set(const string& filename);
  void Set(const vector<string>&, const vector<double>&);
  void Print(ostream& os) const;
  void ResetToDefault();
  void Zero();

  double D_NORTH() const {return _D_NORTH;}
  double D_EAST() const {return _D_EAST;}
  double NORTH_MIN() const {return _NORTH_MIN;}
  double EAST_MIN() const {return _EAST_MIN;}
  unsigned int N_EVENT() const {return _N_EVENT;}
  unsigned int I_PRINT() const {return _I_PRINT;}
  double GLOB_WEIGHT() const {return _GLOB_WEIGHT;}
  double DELT_DIST() const {return _DELT_DIST;}
  double DELT_AZI() const {return _DELT_AZI;}
  double DISTMAX_STATION() const {return _DISTMAX_STATION;}
  unsigned int N_CLONE() const {return _N_CLONE;}
  bool SD_SIM() const {if (_SD_SIM==0)return false; else return true; }
  bool FD_SIM() const {if (_FD_SIM==0)return false; else return true; }
  unsigned int DEBUG_LEVEL() const {return _DEBUG_LEVEL;}
};

inline ostream& operator<<(ostream& s, const Params& p)
{
  p.Print(s);
  return s;
}


#endif
