#ifndef s_tool_CXX
#define s_tool_CXX

#include <ctype.h> 
#include <time.h>
#include "s_tool.h"

bool
s_tool::is_alpha_numeric( string &s_digit )
{
 auto const char *digit = s_digit.c_str();
 auto int   i_len = strlen( digit );

 for( int i=0; i<i_len; i++ )
 {
  if ( ! isalnum( *digit ) )
   return false;
  digit++;
 }

 return true;
}
string 
s_tool::getExtension( string s_file )
{
	int pos = s_file.find_last_of(".");
	if(pos != string::npos)
	{	
		string s_ext=s_file.substr(pos+1, s_file.size()-(pos+1));
		for(int i = 0;i<s_ext.size();i++)
			s_ext[i]=toupper(s_ext[i]);
		return s_ext;
	}
	return "";
}

int
s_tool::string2int( string s_digit )
{
 auto const char *digit = s_digit.c_str();
 int result = 0;

 // Convert each digit char and add into result.
 while (*digit >= '0' && *digit <='9') {
  result = (result * 10) + (*digit - '0');
  digit++;
 }

 // Check that there were no non-digits at end.
 if (*digit != 0) {
  return -1;
 }

 return result;
}

long
s_tool::unixtime()
{
 return (long) time( NULL );
}


void
s_tool::strip_html( string *s_str )
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
#endif
