//============================================================================== 
//
// Source-File  : Utils.cpp (This file is part of the FDSim++ package)
//
// Date         : Wed May 17 2000
//
// Author	: Luis Prado Jr.
//                State University at Campinas
//
// E-Mail       : pradojr@ifi.unicamp.br
//
// Description  : Define some useful functions
//
//============================================================================== 


// Local Headers:
#include "Utils.h"


//============================================================================== 
//
// Function name: ReadVariable
// Input        : 
// Output       : -
// Date         : Wed May 17 2000
// Author       : Luis Prado Jr.
//
// Last change  : Wed May 17 2000
// Done by      : Luis Prado Jr.
//
// Status       : finished and tested
//
// Description  : This Function opens the file 'FileName' and looks in the 
//                section 'SectionName' for the variable 'VariableName'. Then 
//                it gets the parameters of the variable. Types have to be 
//                given in 'Types'. The next arguments are determined by 'Types'. 
//                They have to be pointers. ReadVariable expects an '=' after 
//                the variable. Parameters have to be seperated by ','.  
//                
//                Allowed types:
//                i:     integer
//                l:     long integer
//                f:     float
//                d:     double
//                s:     String (whitespaces will be deleted)
//
//                Return values:
//                0:     no error
//                1:     file not found
//                2:     section not found
//                3:     variable not found
//                100:   no parameters found 
//                101:   only one parameter found, for more than one
//                102:   only two parameters found, for more than two
//                199:   only 99 parameters found, for more than 99
//                1000:  wrong type found in 'Types'
//
//                Example:
//
//                file:
//                [Section]
//                Variable= 10, 234.5556, example
//
//                program:
//                int i;
//                double d;
//                String s;
//                ReadVariables (File, "Section", "Variable", "ids", &i, &d, &s);
//
//                after running function ReadVariables:
//                i = 10
//                d = 234.5556
//                s = "example"
//
//============================================================================== 
int ReadVariable (const char *FileName, const char *SectionName, 
		  const char *VariableName, const char *Types, ...)
{
  const char Separator = ',';

  // Open file 'FileName'
  ifstream Input;
  //  Input.open (FileName, ios::in|ios::nocreate);
  Input.open (FileName, ios::in);
  if (!Input)
    return (NO_FILE);

  // Go to section 'SectionName'
  String Line;
  String Section (SectionName);

  Section = "[" + Section + "]";
  do 
    {
      getline (Input, Line);
      if (Input.eof ())
	return (NO_SECTION);
    } while (!string_contains (Line,Section));


  // Go to variable 'VariableName'
  String Name;
  do
    {
      getline (Input, Line);
      if (Input.eof () || string_contains (Line,"["))
	return (NO_VARIABLE);
      if (Input.eof ())
	return (NO_VARIABLE);
      Name = string_before(Line,"=");
      unsigned int f;
      for (f = 0; f < Name.length (); f++)
	{
	  char c = Name[f];
	  if (isspace (c))
	    break;
	}
      Name = Name.substr (0, f);
    }
    while (String(VariableName)!=Name.substr(0,strlen(VariableName)));


  // Extract 'VariableName' and the following '=' from Line
  Line = string_after(Line,VariableName);
  Line = string_after(Line,"=");
  
  // Determine number of arguments
  int Arguments = string_freq(Line,Separator);

  va_list ap;
  va_start (ap, Types);

  for (int i = 0; Types[i] && (i <= Arguments); i++)
    {
      int    *VAR_INT;
      long   *VAR_LINT;
      float  *VAR_FLOAT;
      double *VAR_DOUBLE;
      String *VAR_STRING;

      // Get the next parameter from Line
      String Parameter;
      char *endptr;
      if ((Types[i+1]) && (i < Arguments))
	{
	  Parameter = string_before(Line,",");
	  Line = string_after(Line,String(","));
	}
      else
	{
	  Parameter = Line;
	}
      const char* parameter=string_new_cstring(Parameter);
      switch (Types[i])
	{
	  case 'i':
	    VAR_INT = va_arg (ap, int*);
	    *VAR_INT = atoi (parameter);
	    break;
	  case 'l':
	    VAR_LINT = va_arg (ap, long*);
	    *VAR_LINT = atol (parameter);
	    break;
	  case 'f':
	    VAR_FLOAT = va_arg (ap, float*);
	    *VAR_FLOAT = atof (parameter);
	    break;
	  case 'd':
	    VAR_DOUBLE = va_arg (ap, double*);
            *VAR_DOUBLE = strtod (parameter, &endptr);
	    break;
	  case 's':
	    VAR_STRING = va_arg (ap, String*);
	    *VAR_STRING = parameter;
            string_removewhitespace(*VAR_STRING);
	    break;
	  default:
	    va_end (ap);
            delete parameter;
	    return (WRONG_TYPE + NO_PARAMETER + i);
	}
      delete parameter;
      if ((i == Arguments) && Types[i+1])
	{
	  va_end (ap);
	  return (NO_PARAMETER + i + 1);
	}
    }

  va_end (ap);

  return (NO_ERROR);
} // End of ReadVariable


//============================================================================== 
//
// Function name: ReadVariable
// Input        : 
// Output       : -
// Date         : Wed May 17 2000
// Author       : Luis Prado Jr.
//
// Last change  : Wed May 17 2000
// Done by      : Luis Prado Jr.
//
// Status       : finished and tested
//
// Description  : This Function reads a variable from 'Input'. It looks in the 
//                actual section for the next variable named 'VariableName'.
//                
//                Allowed types:
//                i:     integer
//                l:     long integer
//                f:     float
//                d:     double
//                s:     String (whitespaces will be deleted)
//
//                Return values:
//                0:     no error
//                1:     file not found
//                2:     section not found
//                3:     variable not found
//                100:   no parameters found 
//                101:   only one parameter found, for more than one
//                102:   only two parameters found, for more than two
//                199:   only 99 parameters found, for more than 99
//                1000:  wrong type found in 'Types'
//
//============================================================================== 
int ReadVariable (istream &Input, const char *VariableName, const char *Types, ...)
{
  const char Separator = ',';
  
  // Is 'Input' OK?
  if (!Input || !Input.good ())
    return (NO_FILE);
  
  // Go to variable 'VariableName'
  String Name;
  String Line;
  do
    {
      getline (Input, Line);
      
      cerr<<"READ \""<<Line<<"\"."<<endl;
      
      if (Input.eof () || string_contains (Line,String("[")))
	return (NO_VARIABLE);
      if (Input.eof ())
	return (NO_VARIABLE);
      Name = string_before(Line,"=");
      unsigned int f;
      for (f = 0; f < Name.length (); f++)
	{
	  char c = Name[f];
	  if (isspace (c))
	    break;
	}
      Name = Name.substr (0, f);
    }
  while (String(VariableName)!=Name.substr(0,strlen(VariableName)));
  
  // Extract 'VariableName' and the following '=' from Line
  Line = string_after(Line,VariableName);
  Line = string_after(Line,"=");
  
  
  // Determine number of arguments
  int Arguments = string_freq(Line,Separator);
  
  va_list ap;
  va_start (ap, Types);
  
  for (int i = 0; Types[i] && (i <= Arguments); i++)
    {
      int    *VAR_INT;
      long   *VAR_LINT;
      float  *VAR_FLOAT;
      double *VAR_DOUBLE;
      String *VAR_STRING;
      
      // Get the next parameter from Line
      String Parameter;
      char *endptr;
      if ((Types[i+1]) && (i < Arguments))
	{
	  Parameter = string_before(Line,",");
	  Line = string_after(Line,String(","));
	}
      else
	{
	  Parameter = Line;
	}
      
      const char* parameter = string_new_cstring(Parameter);
      switch (Types[i])
	{
	case 'i':
	  VAR_INT = va_arg (ap, int*);
	  *VAR_INT = atoi (parameter);
	  break;
	case 'l':
	  VAR_LINT = va_arg (ap, long*);
	  *VAR_LINT = atol (parameter);
	  break;
	case 'f':
	  VAR_FLOAT = va_arg (ap, float*);
	  *VAR_FLOAT = atof (parameter);
	  break;
	case 'd':
	  VAR_DOUBLE = va_arg (ap, double*);
	  *VAR_DOUBLE = strtod (parameter, &endptr);
	  break;
	case 's':
	  VAR_STRING = va_arg (ap, String*);
	  *VAR_STRING = parameter;
	  string_removewhitespace(*VAR_STRING);
	  break;
	default:
	  va_end (ap);
	  delete parameter;
	  return (WRONG_TYPE + NO_PARAMETER + i);
	}
      delete parameter;
      if ((i == Arguments) && Types[i+1])
	{
	  va_end (ap);
	  return (NO_PARAMETER + i + 1);
	}
    }
  
  va_end (ap);
  
  return (NO_ERROR);
  
} // End of ReadVariable



//============================================================================== 
//
// Function name: OpenSection
// Input        : Input, FileName, SectionName
// Output       : -
// Date         : Wed May 17 2000
// Author       : Luis Prado Jr.
//
// Last change  : Wed May 17 2000
// Done by      : Luis Prado Jr.
//
// Status       : finished and tested
//
// Description  : Close and reopen file `FileName'; then look for section
//                `SectionName'.
//
//============================================================================== 
int OpenSection (ifstream &Input, const char* FileName, const char* SectionName)
{
  // Is 'Input' OK?
  Input.close ();
  //  Input.open (FileName, ios::in|ios::nocreate);
  Input.open (FileName, ios::in);
  if (!Input)
    return (NO_FILE);
  
  return OpenSection (Input, SectionName);
} // End of OpenSection


//============================================================================== 
//
// Function name: OpenSection
// Input        : Input, SectionName
// Output       : -
// Date         : Wed May 17 2000
// Author       : Luis Prado Jr.
//
// Last change  : Wed May 17 2000
// Done by      : Luis Prado Jr.
//
// Status       : finished and tested
//
// Description  : Go to section `SectionName'.
//
//============================================================================== 
int OpenSection (istream &Input, const char* SectionName)
{
  // Is 'Input' OK?
  if (!Input || !Input.good ())
    return (NO_FILE);
  
  // Go to section 'SectionName'
  String Line;
  String Section (SectionName);
  
  Section = "[" + Section + "]";
  do 
    {
      getline (Input, Line);
      if (Input.eof ())
	return (NO_SECTION);
      unsigned int f;
      for (f = 0; f < Line.length (); f++)
	{
	  char c = Line[f];
	  if (isspace (c))
	    break;
	}
      Line = Line.substr (0, f);
      
    } while (Section != Line);
  
  return NO_ERROR;
} // End of OpenSection



//=============================================================================
//	Additional math functions
//=============================================================================
double sqr (const double& x)
{ 
  return (x * x);
} // End of sqr


//============================================================================== 
//
// Function name: Error
// Input        : Message
// Output       : Error message
// Date         : Wed May 17 2000
// Author       : Luis Prado Jr.
//
// Last change  : Wed May 17 2000
// Done by      : Luis Prado Jr.
//
// Status       : finished and tested
//
// Description  : Write an error message to `cerr' saying that option `option'
//                is illegal and exit the program.
//
//============================================================================== 
void Error (const char *Message)
{
  cerr << Message << '\n';
  exit (1);
} // End of Error



//============================================================================== 
//
// Function name: IllegalOption
// Input        : Option
// Output       : Error message
// Date         : Wed May 17 2000
// Author       : Luis Prado Jr.
//
// Last change  : Wed May 17 2000
// Done by      : Luis Prado Jr.
//
// Status       : finished and tested
//
// Description  : Write an error message to `cerr' saying that option `option'
//                is illegal and exit the program.
//
//============================================================================== 
void IllegalOption (const char* Option)
{
    cerr << "Illegal Option \"" << Option << "\" found\n";
    exit (1);
} // End of IllegalOption



//============================================================================== 
//
// Function name: IllegalArgument
// Input        : Option, Argument
// Output       : Error message
// Date         : Wed May 17 2000
// Author       : Luis Prado Jr.
//
// Last change  : Wed May 17 2000
// Done by      : Luis Prado Jr.
//
// Status       : finished and tested
//
// Description  : Write an error message to `cerr' saying that for the option
//                `option' the argument `Argument' is illegal and exit.
//
//============================================================================== 
void IllegalArgument (const char* Argument, const char* Option)
{
    cerr << "Illegal Argument \"" << Argument << "\" for option \"" << Option << "\" found\n";
    exit (1);
} // End of IllegalOption



//============================================================================== 
//
// Function name: ShowWarranty
// Input        : Standard output is 'Output'
// Output       : -
// Date         : Wed May 17 2000
// Author       : Luis Prado Jr.
//
// Last change  : Wed May 17 2000
// Done by      : Luis Prado Jr.
//
// Status       : finished and tested
//
// Description  : Show the warranty conditions.
//
//============================================================================== 
void ShowWarranty (ostream& Output)
{
    Output << '\n';
    Output << "                    GNU GENERAL PUBLIC LICENSE\n";
    Output << "                       Version 2, June 1991\n";
    Output << '\n';
    Output << "                            NO WARRANTY\n";
    Output << '\n';
    Output << "  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY\n";
    Output << "FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN\n";
    Output << "OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES\n";
    Output << "PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED\n";
    Output << "OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF\n";
    Output << "MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS\n";
    Output << "TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE\n";
    Output << "PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,\n";
    Output << "REPAIR OR CORRECTION.\n";
    Output << '\n';
    Output << "  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING\n";
    Output << "WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR\n";
    Output << "REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,\n";
    Output << "INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING\n";
    Output << "OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED\n";
    Output << "TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY\n";
    Output << "YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER\n";
    Output << "PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE\n";
    Output << "POSSIBILITY OF SUCH DAMAGES.\n";
} // End of ShowWarranty



//============================================================================== 
//
// Functionname : ShowCopyingLicense
// Input        : Standard output is 'Output'
// Output       : -
// Date         : Wed May 17 2000
// Author       : Luis Prado Jr.
//
// Last change  : Wed May 17 2000
// Done by      : Luis Prado Jr.
//
// Status       : finished and tested
//
// Description  : Show the copying conditions.
//
//============================================================================== 
void ShowCopyingLicense (ostream& Output)
{
    Output << '\n';
    Output << "                    GNU GENERAL PUBLIC LICENSE\n";
    Output << "                       Version 2, June 1991\n";
    Output << '\n';
    Output << "   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION\n";
    Output << '\n';
    Output << "  0. This License applies to any program or other work which contains\n";
    Output << "a notice placed by the copyright holder saying it may be distributed\n";
    Output << "under the terms of this General Public License.  The \"Program\", below,\n";
    Output << "refers to any such program or work, and a \"work based on the Program\"\n";
    Output << "means either the Program or any derivative work under copyright law:\n";
    Output << "that is to say, a work containing the Program or a portion of it,\n";
    Output << "either verbatim or with modifications and/or translated into another\n";
    Output << "language.  (Hereinafter, translation is included without limitation in\n";
    Output << "the term \"modification\".)  Each licensee is addressed as \"you\".\n";
    Output << '\n';
    Output << "Activities other than copying, distribution and modification are not\n";
    Output << "covered by this License; they are outside its scope.  The act of\n";
    Output << "running the Program is not restricted, and the output from the Program\n";
    Output << "is covered only if its contents constitute a work based on the\n";
    Output << "Program (independent of having been made by running the Program).\n";
    Output << "Whether that is true depends on what the Program does.\n";
    Output << '\n';
    Output << "  1. You may copy and distribute verbatim copies of the Program's\n";
    Output << "source code as you receive it, in any medium, provided that you\n";
    Output << "conspicuously and appropriately publish on each copy an appropriate\n";
    Output << "copyright notice and disclaimer of warranty; keep intact all the\n";
    Output << "notices that refer to this License and to the absence of any warranty;\n";
    Output << "and give any other recipients of the Program a copy of this License\n";
    Output << "along with the Program.\n";
    Output << '\n';
    Output << "You may charge a fee for the physical act of transferring a copy, and\n";
    Output << "you may at your option offer warranty protection in exchange for a fee.\n";
    Output << '\n';
    Output << "  2. You may modify your copy or copies of the Program or any portion\n";
    Output << "of it, thus forming a work based on the Program, and copy and\n";
    Output << "distribute such modifications or work under the terms of Section 1\n";
    Output << "above, provided that you also meet all of these conditions:\n";
    Output << '\n';
    Output << "    a) You must cause the modified files to carry prominent notices\n";
    Output << "    stating that you changed the files and the date of any change.\n";
    Output << '\n';
    Output << "    b) You must cause any work that you distribute or publish, that in\n";
    Output << "    whole or in part contains or is derived from the Program or any\n";
    Output << "    part thereof, to be licensed as a whole at no charge to all third\n";
    Output << "    parties under the terms of this License.\n";
    Output << '\n';
    Output << "    c) If the modified program normally reads commands interactively\n";
    Output << "    when run, you must cause it, when started running for such\n";
    Output << "    interactive use in the most ordinary way, to print or display an\n";
    Output << "    announcement including an appropriate copyright notice and a\n";
    Output << "    notice that there is no warranty (or else, saying that you provide\n";
    Output << "    a warranty) and that users may redistribute the program under\n";
    Output << "    these conditions, and telling the user how to view a copy of this\n";
    Output << "    License.  (Exception: if the Program itself is interactive but\n";
    Output << "    does not normally print such an announcement, your work based on\n";
    Output << "    the Program is not required to print an announcement.)\n";
    Output << '\n';
    Output << "These requirements apply to the modified work as a whole.  If\n";
    Output << "identifiable sections of that work are not derived from the Program,\n";
    Output << "and can be reasonably considered independent and separate works in\n";
    Output << "themselves, then this License, and its terms, do not apply to those\n";
    Output << "sections when you distribute them as separate works.  But when you\n";
    Output << "distribute the same sections as part of a whole which is a work based\n";
    Output << "on the Program, the distribution of the whole must be on the terms of\n";
    Output << "this License, whose permissions for other licensees extend to the\n";
    Output << "entire whole, and thus to each and every part regardless of who wrote it.\n";
    Output << '\n';
    Output << "Thus, it is not the intent of this section to claim rights or contest\n";
    Output << "your rights to work written entirely by you; rather, the intent is to\n";
    Output << "exercise the right to control the distribution of derivative or\n";
    Output << "collective works based on the Program.\n";
    Output << '\n';
    Output << "In addition, mere aggregation of another work not based on the Program\n";
    Output << "with the Program (or with a work based on the Program) on a volume of\n";
    Output << "a storage or distribution medium does not bring the other work under\n";
    Output << "the scope of this License.\n";
    Output << '\n';
    Output << "  3. You may copy and distribute the Program (or a work based on it,\n";
    Output << "under Section 2) in object code or executable form under the terms of\n";
    Output << "Sections 1 and 2 above provided that you also do one of the following:\n";
    Output << '\n';
    Output << "    a) Accompany it with the complete corresponding machine-readable\n";
    Output << "    source code, which must be distributed under the terms of Sections\n";
    Output << "    1 and 2 above on a medium customarily used for software interchange; or,\n";
    Output << '\n';
    Output << "    b) Accompany it with a written offer, valid for at least three\n";
    Output << "    years, to give any third party, for a charge no more than your\n";
    Output << "    cost of physically performing source distribution, a complete\n";
    Output << "    machine-readable copy of the corresponding source code, to be\n";
    Output << "    distributed under the terms of Sections 1 and 2 above on a medium\n";
    Output << "    customarily used for software interchange; or,\n";
    Output << '\n';
    Output << "    c) Accompany it with the information you received as to the offer\n";
    Output << "    to distribute corresponding source code.  (This alternative is\n";
    Output << "    allowed only for noncommercial distribution and only if you\n";
    Output << "    received the program in object code or executable form with such\n";
    Output << "    an offer, in accord with Subsection b above.)\n";
    Output << '\n';
    Output << "The source code for a work means the preferred form of the work for\n";
    Output << "making modifications to it.  For an executable work, complete source\n";
    Output << "code means all the source code for all modules it contains, plus any\n";
    Output << "associated interface definition files, plus the scripts used to\n";
    Output << "control compilation and installation of the executable.  However, as a\n";
    Output << "special exception, the source code distributed need not include\n";
    Output << "anything that is normally distributed (in either source or binary\n";
    Output << "form) with the major components (compiler, kernel, and so on) of the\n";
    Output << "operating system on which the executable runs, unless that component\n";
    Output << "itself accompanies the executable.\n";
    Output << '\n';
    Output << "If distribution of executable or object code is made by offering\n";
    Output << "access to copy from a designated place, then offering equivalent\n";
    Output << "access to copy the source code from the same place counts as\n";
    Output << "distribution of the source code, even though third parties are not\n";
    Output << "compelled to copy the source along with the object code.\n";
    Output << '\n';
    Output << "  4. You may not copy, modify, sublicense, or distribute the Program\n";
    Output << "except as expressly provided under this License.  Any attempt\n";
    Output << "otherwise to copy, modify, sublicense or distribute the Program is\n";
    Output << "void, and will automatically terminate your rights under this License.\n";
    Output << "However, parties who have received copies, or rights, from you under\n";
    Output << "this License will not have their licenses terminated so long as such\n";
    Output << "parties remain in full compliance.\n";
    Output << '\n';
    Output << "  5. You are not required to accept this License, since you have not\n";
    Output << "signed it.  However, nothing else grants you permission to modify or\n";
    Output << "distribute the Program or its derivative works.  These actions are\n";
    Output << "prohibited by law if you do not accept this License.  Therefore, by\n";
    Output << "modifying or distributing the Program (or any work based on the\n";
    Output << "Program), you indicate your acceptance of this License to do so, and\n";
    Output << "all its terms and conditions for copying, distributing or modifying\n";
    Output << "the Program or works based on it.\n";
    Output << '\n';
    Output << "  6. Each time you redistribute the Program (or any work based on the\n";
    Output << "Program), the recipient automatically receives a license from the\n";
    Output << "original licensor to copy, distribute or modify the Program subject to\n";
    Output << "these terms and conditions.  You may not impose any further\n";
    Output << "restrictions on the recipients' exercise of the rights granted herein.\n";
    Output << "You are not responsible for enforcing compliance by third parties to\n";
    Output << "this License.\n";
    Output << '\n';
    Output << "  7. If, as a consequence of a court judgment or allegation of patent\n";
    Output << "infringement or for any other reason (not limited to patent issues),\n";
    Output << "conditions are imposed on you (whether by court order, agreement or\n";
    Output << "otherwise) that contradict the conditions of this License, they do not\n";
    Output << "excuse you from the conditions of this License.  If you cannot\n";
    Output << "distribute so as to satisfy simultaneously your obligations under this\n";
    Output << "License and any other pertinent obligations, then as a consequence you\n";
    Output << "may not distribute the Program at all.  For example, if a patent\n";
    Output << "license would not permit royalty-free redistribution of the Program by\n";
    Output << "all those who receive copies directly or indirectly through you, then\n";
    Output << "the only way you could satisfy both it and this License would be to\n";
    Output << "refrain entirely from distribution of the Program.\n";
    Output << '\n';
    Output << "If any portion of this section is held invalid or unenforceable under\n";
    Output << "any particular circumstance, the balance of the section is intended to\n";
    Output << "apply and the section as a whole is intended to apply in other\n";
    Output << "circumstances.\n";
    Output << '\n';
    Output << "It is not the purpose of this section to induce you to infringe any\n";
    Output << "patents or other property right claims or to contest validity of any\n";
    Output << "such claims; this section has the sole purpose of protecting the\n";
    Output << "integrity of the free software distribution system, which is\n";
    Output << "implemented by public license practices.  Many people have made\n";
    Output << "generous contributions to the wide range of software distributed\n";
    Output << "through that system in reliance on consistent application of that\n";
    Output << "system; it is up to the author/donor to decide if he or she is willing\n";
    Output << "to distribute software through any other system and a licensee cannot\n";
    Output << "impose that choice.\n";
    Output << '\n';
    Output << "This section is intended to make thoroughly clear what is believed to\n";
    Output << "be a consequence of the rest of this License.\n";
    Output << '\n';
    Output << "  8. If the distribution and/or use of the Program is restricted in\n";
    Output << "certain countries either by patents or by copyrighted interfaces, the\n";
    Output << "original copyright holder who places the Program under this License\n";
    Output << "may add an explicit geographical distribution limitation excluding\n";
    Output << "those countries, so that distribution is permitted only in or among\n";
    Output << "countries not thus excluded.  In such case, this License incorporates\n";
    Output << "the limitation as if written in the body of this License.\n";
    Output << '\n';
    Output << "  9. The Free Software Foundation may publish revised and/or new versions\n";
    Output << "of the General Public License from time to time.  Such new versions will\n";
    Output << "be similar in spirit to the present version, but may differ in detail to\n";
    Output << "address new problems or concerns.\n";
    Output << '\n';
    Output << "Each version is given a distinguishing version number.  If the Program\n";
    Output << "specifies a version number of this License which applies to it and \"any\n";
    Output << "later version\", you have the option of following the terms and conditions\n";
    Output << "either of that version or of any later version published by the Free\n";
    Output << "Software Foundation.  If the Program does not specify a version number of\n";
    Output << "this License, you may choose any version ever published by the Free Software\n";
    Output << "Foundation.\n";
    Output << '\n';
    Output << "  10. If you wish to incorporate parts of the Program into other free\n";
    Output << "programs whose distribution conditions are different, write to the author\n";
    Output << "to ask for permission.  For software which is copyrighted by the Free\n";
    Output << "Software Foundation, write to the Free Software Foundation; we sometimes\n";
    Output << "make exceptions for this.  Our decision will be guided by the two goals\n";
    Output << "of preserving the free status of all derivatives of our free software and\n";
    Output << "of promoting the sharing and reuse of software generally.\n";
} // End of CopyingLicense



ostream& spacess (ostream& os, int n)
{
    for (int i = 0; i < n; i++)
    {
        os << " ";
    }

    return os;
}

//oapp<int> spaces(spacess);



void SetMemorySize ()
{
    struct rlimit rlp;

    // set datasize
    cout << "rlp->rlim_cur= " << flush << rlp.rlim_cur << '\n';
    cout << "rlp->rlim_max= " << rlp.rlim_max << '\n';
    cout << "getrlimit(RLIMIT_DATA,&rlp)= " << getrlimit(RLIMIT_DATA,&rlp) << '\n';
    cout << "rlp->rlim_cur= " << flush << rlp.rlim_cur << '\n';
    cout << "rlp->rlim_max= " << rlp.rlim_max << '\n';
    rlp.rlim_cur = rlp.rlim_max;
    cout << "setrlimit(RLIMIT_DATA,&rlp)= " << setrlimit(RLIMIT_DATA,&rlp) << '\n';
    cout << "rlp->rlim_cur= " << flush << rlp.rlim_cur << '\n';
    cout << "rlp->rlim_max= " << rlp.rlim_max << '\n';
    cout << "getrlimit(RLIMIT_DATA,&rlp)= " << getrlimit(RLIMIT_DATA,&rlp) << '\n';
    cout << "rlp->rlim_cur= " << flush << rlp.rlim_cur << '\n';
    cout << "rlp->rlim_max= " << rlp.rlim_max << '\n';

    // set stacksize
    cout << "getrlimit(RLIMIT_STACK,&rlp)= " << getrlimit(RLIMIT_STACK,&rlp) << '\n';
    cout << "rlp->rlim_cur= " << flush << rlp.rlim_cur << '\n';
    cout << "rlp->rlim_max= " << rlp.rlim_max << '\n';
    rlp.rlim_cur = rlp.rlim_max;
    cout << "setrlimit(RLIMIT_STACK,&rlp)= " << setrlimit(RLIMIT_STACK,&rlp) << '\n';
    cout << "getrlimit(RLIMIT_STACK,&rlp)= " << getrlimit(RLIMIT_STACK,&rlp) << '\n';
    cout << "rlp->rlim_cur= " << flush << rlp.rlim_cur << '\n';
    cout << "rlp->rlim_max= " << rlp.rlim_max << '\n';
}



//==End of File=================================================================






