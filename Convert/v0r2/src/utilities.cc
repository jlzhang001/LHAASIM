#include "Globals.h"
#include "utilities.h"

Params::Params()
{
 Set(DEFAULT_FILE); 
}

Params::Params(const string& filename)
{
  Set(DEFAULT_FILE);
  Set(filename);
}

Params::Params(const string& keyword, double value)
{
  Set(DEFAULT_FILE);
  Set(keyword,value);
}

void Params::Set(const string& keyword, double value)
{
  if( keyword=="D_NORTH") _D_NORTH=value;
  else if( keyword=="D_EAST") _D_EAST=value;
  else if( keyword=="NORTH_MIN") _NORTH_MIN=value;
  else if( keyword=="EAST_MIN") _EAST_MIN=value;
  else if( keyword=="N_EVENT") _N_EVENT=(unsigned int)value;
  else if( keyword=="I_PRINT") _I_PRINT=(unsigned int)value;
  else if( keyword=="GLOB_WEIGHT") _GLOB_WEIGHT=value;
  else if( keyword=="DELT_DIST") _DELT_DIST=value;
  else if( keyword=="DELT_AZI") _DELT_AZI=value*DEG2RAD;
  else if( keyword=="DISTMAX_STATION") _DISTMAX_STATION=value;
  else if( keyword=="N_CLONE") _N_CLONE=(unsigned int)value;
  else if( keyword=="SD_SIM") _SD_SIM=(unsigned int)value;
  else if( keyword=="FD_SIM") _FD_SIM=(unsigned int)value;
  else if( keyword=="DEBUG_LEVEL") _DEBUG_LEVEL=(unsigned int)value;
  else cerr << "unknown keyword " << keyword << endl;
}

void Params::Set(const string& filename)
{
  vector<string> keywords;
  vector<double> values;
  read_params(filename,keywords,values);
  unsigned int size=keywords.size();
  for( unsigned int i=0;i<size;i++ ) this->Set(keywords[i],values[i]);
}

void Params::Set(const vector<string>& keywords,
		 const vector<double>& values)
{
  unsigned int size=keywords.size();
  if( size!=values.size() )
    {
      cerr << "Problem in setting parameters" << endl;
      exit(0);
    }
  for( unsigned int i=0;i<size;i++ ) this->Set(keywords[i],values[i]);
}

void Params::Print(ostream& os) const
{
  os << setw(20) << "SD_SIM=" << setw(15) << _SD_SIM << endl;
  os << setw(20) << "FD_SIM=" << setw(15) << _FD_SIM << endl;
  os << setw(20) << "DEBUG_LEVEL=" << setw(15) << _DEBUG_LEVEL << endl;
  os << setw(20) << "D_NORTH=" << setw(15) << _D_NORTH << endl;
  os << setw(20) << "NORTH_MIN=" << setw(15) << _NORTH_MIN << endl;
  os << setw(20) << "EAST_MIN=" << setw(15) << _EAST_MIN << endl;
  os << setw(20) << "N_EVENT=" << setw(15) << _N_EVENT << endl;
  os << setw(20) << "I_PRINT=" << setw(15) << _I_PRINT << endl;
  os << setw(20) << "GLOB_WEIGHT=" << setw(15) << _GLOB_WEIGHT << endl;
  os << setw(20) << "DELT_DIST=" << setw(15) << _DELT_DIST << endl;
  os << setw(20) << "DELT_AZI=" << setw(15) << _DELT_AZI << endl;
  os << setw(20) << "DISTMAX_STATION=" << setw(15) << _DISTMAX_STATION << endl;
  os << setw(20) << "N_CLONE=" << setw(15) << _N_CLONE << endl;
}

void Params::ResetToDefault()
{
  Set(DEFAULT_FILE);
}

void Params::Zero()
{
  _D_NORTH=0;
  _NORTH_MIN=0;
  _EAST_MIN=0;
  _N_EVENT=0;
  _I_PRINT=0;
  _GLOB_WEIGHT=0;
  _DELT_DIST=0;
  _DELT_AZI=0;
  _DISTMAX_STATION=0;
  _N_CLONE=0;
}

void usage(bool flag)
{
  if( flag )
    {
      cerr << "you can specify an input parameters file: test [-f file.inp] file.truc" << endl;
      cerr << "using default parameters file default.inp" << endl;
    }
  else
    {
      cerr << "Bad calling sequence" << endl;
      cerr << "usage: test [-f file.inp] file.truc" << endl;
      exit(0);
    }
}

void sort_remove_duplicated(vector<char>& vc)
{
  if( vc.size() != 0 )
    {
      sort(vc.begin(),vc.end());
      vector<char>::iterator it=unique(vc.begin(),vc.end());
      vc.erase(it,vc.end());
    }
}

template<class T> ostream& operator<<(ostream& s, const vector<T>& vc)
{
  long size=vc.size();
  if( size>0 ) for( int i=0;i<size;i++ ) s << vc[i] << " ";
  else cerr << endl;
  return s;
}

void read_params(const string& filename,
		 vector<string>& keywords,
		 vector<double>& values)
{
  char ch[LINESIZE];
  ifstream f(filename.c_str());
  if( !f )
    {
      cerr << "Problem with input file " << filename << endl;
      exit(0);
    }
  keywords.clear();
  values.clear();
  while( f.getline(ch,LINESIZE) )
    {
      if( !isspace(*ch) && *ch != '#' )
	{
	  char keyword[MAXPARAMLENGTH];
	  float value;
	  if( sscanf(ch,"%s %f",keyword,&value)==2 )	    
	    {
	      keywords.push_back(keyword);
	      values.push_back((double)value);
	    }
	}
    }
  return;
}

void det_flags(int argc, char** argv, string& params_file, string& filename)
{
  int i,c;
  vector<char> myflags(1),flags;
  myflags[0]='f';
  while( --argc>0 && ((*++argv)[0]=='-') ) while( (c=*++(argv[0])) ) flags.push_back((char)c);
  long size=flags.size();
  if( size==0 )
    {
      cerr << "no flags on input" << endl;
      usage(CONT);
      params_file=DEFAULT_FILE;
      if( argc<1 )
	{
	  cerr << "missing ROOT input file" << endl;
	  exit(0);
	}
      filename=*argv;
      return;
    }
  else
    {
      sort_remove_duplicated(flags);
      sort_remove_duplicated(myflags);
      vector<char> * result = new vector<char>;
      insert_iterator< vector<char> > it(*result,result->begin());
      set_intersection(flags.begin(),flags.end(),myflags.begin(),myflags.end(),it);
      size=result->size();
      if( size>0 )
	{      
	  for( i=0;i<size;i++ )
	    {
	      switch( (*result)[i] )
		{
		case 'f':
		  params_file=*argv;
		  if( argc!=2 )
		    {
		      cerr << "missing ROOT input file" << endl;
		      usage(EXIT);
		    }
		  else filename=*++argv;
		  break;
		}
	    }
	}
      else usage(EXIT);

      delete result;
    }
}
