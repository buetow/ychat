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
s_tool::trim( string s_str )
{
    if(s_str.empty())
        return "";
    char c_cur=s_str[0];
    auto int pos=0;
    // left trim
    while(c_cur==' ' || c_cur == '\n' || c_cur == '\r')
    {
        s_str.erase(pos,1);

        c_cur=s_str[++pos];

    }
    // right trim

    pos=s_str.size();
    c_cur=s_str[s_str.size()];

    while(c_cur==' ' || c_cur == '\n' || c_cur == '\0' || c_cur == '\r')
    {
        s_str.erase(pos,1);
        c_cur=s_str[--pos];

    }
    return s_str;
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

char*
s_tool::int2string( int i_int )
{
    char *buffer = new char[64];
    sprintf(buffer, "%d", i_int);
    return buffer;
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
string
s_tool::to_lower( string s )
{
	string s_tmp("");
	for(int i=0;i < s.size() ;i++)
		s_tmp=s_tmp+(char)tolower(s.at(i));

	return s_tmp;
}
#endif
