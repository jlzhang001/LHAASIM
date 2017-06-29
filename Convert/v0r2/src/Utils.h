//============================================================================== 
//
// Header-File  : Utils.h (This file is part of the FDSim++ package)
//
// Date         : Wed May 17 2000
//
// Author	: Luis Prado Jr.
//                State University at Campinas
//
// E-Mail       : pradojr@ifi.unicamp.br
//
// Description  : Include some system headers and declare some useful functions.
//
//============================================================================== 


#ifndef __UTILS_H
#define __UTILS_H

//============================================================================= 
// g++ lib <-> STL patch for string manipulation ( String.h <-> bastring )  
//============================================================================= 
using namespace std;

// System Headers:
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <string>
#define String std::string
#include <complex>
#define Complex double_complex                     


// Global Variables:
const int NO_ERROR     =    0;
const int NO_FILE      =    1;
const int NO_SECTION   =    2;
const int NO_VARIABLE  =    3;
const int NO_PARAMETER =  100;
const int WRONG_TYPE   = 1000;


// Global Functions:

// File handling
int ReadVariable (const char* FileName, const char* SectionName, 
		  const char* VariableName, const char* Types, ...);
int OpenSection (istream& Input, const char* SectionName);
int OpenSection (ifstream& Input, const char* FileName, const char* SectionName);
int ReadVariable (istream& Input, const char* VariableName, const char* Types, ...);

// Comparing objects
template<class T> inline T max (T a, T b) { return (a > b ? a : b); }
template<class T> inline T min (T a, T b) { return (a < b ? a : b); }

// Swap objects
//template<class T> inline void swap (T& a, T& b) { T c=a; a=b; b=c; }

// Math functions
double sqr (const double& x);

// Error handling
void Error (const char* Message);
void IllegalOption (const char* option);
void IllegalArgument (const char* argument, const char* option);

// Copying and warranty conditions
void ShowCopyingLicense (ostream& Output);
void ShowWarranty (ostream& Output);

// Memory handling
void SetMemorySize ();


// Classes
//! Class for output-stream manipulators
template<class T> class UtilsOM
{
public:
    typedef ostream& (*F)(ostream&, T, T);
    UtilsOM(F ff, T tt1, T tt2) : f(ff), t1(tt1), t2(tt2) {}
    friend ostream& operator << (ostream& os, const UtilsOM<T>& om)
    { return om.f(os, om.t1, om.t2); }

private:
    const F f;
    const T t1;
    const T t2;
};

//! Class for output-stream manipulators
template<class T> class UtilsApp
{
public:
    UtilsApp(typename UtilsOM<T>::F ff) : f(ff) {}
    UtilsOM<T> operator()(T n, T m) const { return UtilsOM<T>(f, n, m); }

private:
    const typename UtilsOM<T>::F f;
};


inline ostream& setfixedd (ostream& os, int n, int m)
{
    os.flags (ios::fixed);
    os.width (n);
    os.precision (m);

    return os;
}

inline ostream& setsciencee (ostream& os, int n, int m)
{
    os.flags (ios::scientific);
    os.width (n);
    os.precision (m);

    return os;
}

ostream& spacess (ostream& os, int n);


// Available Manipulators
const UtilsApp<int> setfixed(setfixedd);
const UtilsApp<int> setscience(setsciencee);
//extern oapp<int> spaces;

//
// Replacements for libg++ functions missing in ANSI C++
// by Luis Prado Jr. 1998-06-15
// see http://www.msoe.edu/~sebern/resource/stl/string.htm 
//

inline std::string string_before(std::string line, std::string seperator) {
    // ligb++ equivalent: return line.before(seperator);
    return line.substr(0,line.find(seperator));
}

inline std::string string_after(std::string line, std::string seperator) {
    // ligb++ equivalent: return line.before(seperator);
    return line.substr(line.find(seperator)+1);
}

inline void string_removewhitespace(string& line) {
    // libg++ equivalent: line.gsub (RXwhite , "");
    // patch to remove whitespace ANSI C++
    // s1 (with whitespace) --> s2 (without whitespace)
    // Luis Prado Jr 1998-05-18

    const char* s1=line.data();
    const int l1=line.length();
    char* s2=new char[strlen(s1)+1];
    char* p2=s2;
    for (int i=0; i<l1; i++) {
        const char c=s1[i];
        if (c!=' ' && c!='\t' && c!='\n') {
            *p2++=c;
        }
    }
    *p2=0;
    line=std::string(s2);
    delete s2;
}

inline int string_freq(std::string line, char c) {
    // ligb++ equivalent: return line.freq(c);
    int n = 0;
    const int l=line.length();
    for (int i=0; i<l; i++) {
        if (line[i]==c) {
            n++;
        }
    }
    return n;
}

inline bool string_contains(std::string line, std::string s) {
    // ligb++ equivalent: return line.find(s);
    return line.find(s)==std::string::npos?false:true;
}

inline char* string_new_cstring(const String& Line){
    const int l=Line.length();
    char* s=new char[l+1];
    strncpy(s,Line.data(),l);
    s[l]=0;
    return s;
}    

#endif // __UTILS_H

//==End of File=================================================================

