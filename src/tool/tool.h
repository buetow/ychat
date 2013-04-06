#ifndef TOOL_H
#define TOOL_H

#include "../incl.h"

#include <list>

using namespace std;

class tool
{
public:
    static list<string> split_string(string s_string, string s_split);

    static bool is_alpha_numeric( string &s_digit )
    {
     const char *digit = s_digit.c_str();
     int   i_len = strlen( digit );

     for( int i=0; i<i_len; i++ )
     {
         if ( ! isalnum( *digit ) )
             return false;
         digit++;
     }
     return true;
    }

    static char* int2char( int i_int );
    static char* clean_char( char* c_str);
    static string trim( string s_str );
    static string replace( string s_string, string s_search, string s_replace );

    static string int2string( int i_int )
    {
     char buffer[64];
     sprintf(buffer, "%d", i_int);
     return string(buffer);	
    }

    static long unixtime()
    {
     time_t clock;
     return (long) time( &clock );
    }

    static int string2int( string s_digit )
    {
     auto const char *digit = s_digit.c_str();
     int result = 0;

     // Convert each digit char and add into result.
     while (*digit >= '0' && *digit <='9')
     {
         result = (result * 10) + (*digit - '0');
         digit++;
     }

     // Check that there were no non-digits at end.
     if (*digit != 0)
     {
        return -1;
     }

     return result;
    }
    static void strip_html( string *s_str)
    {
     auto int i_pos;
     if((i_pos=s_str->find("<",0))==string::npos)
      return;
     while(true)
     {
      s_str->replace(i_pos,1,"&lt;");
      if((i_pos=s_str->find("<",0))==string::npos)
       return;
     }
    }

    static string get_extension( string s_file );
    static string to_lower( string s )
    {
	string s_tmp("");
	for(int i=0;i < s.size() ;i++)
		s_tmp=s_tmp+(char)tolower(s.at(i));
	return s_tmp;
    }

   static string shell_command( string s_command, method m_method );

   static string yhttpd_version()
   {
    return string(VERSION) + "-" + string(BRANCH) + " Build " + int2string(BUILDNR);
   }
};

#endif
