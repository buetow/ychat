#ifndef TOOL_CPP
#define TOOL_CPP

#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>

#include "tool.h"

list<string>
tool::split_string(string s_string, string s_split) {
    list<string> list_ret;
    unsigned i_pos, i_len = s_split.length();     

    while ( (i_pos = s_string.find(s_split)) != string::npos )
    {
     list_ret.push_back( s_string.substr(0, i_pos) );
     s_string = s_string.substr( i_pos + i_len );
    }

    list_ret.push_back( s_string );

    return list_ret;
}

string
tool::trim( string s_str )
{
    if(s_str.empty())
        return "";

    char c_cur = s_str[0];
    auto int pos=0;

    // left trim
    while (c_cur==' '|| c_cur == '\n' || c_cur == '\r')
    {
        s_str.erase(pos,1);
        c_cur = s_str[++pos];
    }

    // right trim
    pos=s_str.size();
    c_cur = s_str[s_str.size()];

    while (c_cur==' ' || c_cur == '\n' || c_cur == '\0' || c_cur == '\r')
    {
        s_str.erase(pos,1);
        c_cur = s_str[--pos];
    }

    return s_str;
}

char*
tool::clean_char( char* c_str )
{
  // Ralf:
  char *c_pos;
  for (c_pos = c_str; *c_pos != '\0'; ++c_pos)
   if ( iscntrl(*c_pos) ) *c_pos = ' ';
  return c_str;
}

string
tool::replace( string s_string, string s_search, string s_replace )
{
 unsigned int i_pos[2];

 for ( i_pos[0]  = s_string.find( s_search );
       i_pos[0] != string::npos;
       i_pos[0]  = s_string.find( s_search, i_pos[1] ) ) 
 {
  s_string.replace( i_pos[0], s_search.length(), s_replace );
  i_pos[1] = i_pos[0] + s_replace.length();
 }

 return s_string;
}

string
tool::get_extension( string s_file )
{
    int pos = s_file.find_last_of(".");
    if(pos != string::npos)
    {
        string s_ext=s_file.substr(pos+1, s_file.size()-(pos+1));
        for(int i = 0;i<s_ext.size();i++)
            s_ext[i]=tolower(s_ext[i]);

        return to_lower(s_ext);
    }
    return "";
}

char*
tool::int2char( int i_int )
{
    char *buffer = new char[64];
    sprintf(buffer, "%d", i_int);
    return buffer;
}

string
tool::shell_command( string s_command, method m_method )
{
    FILE *file;
    char buffer[READBUF];
    char* c_pos;
    string s_ret = "";

    wrap::system_message(SHELLEX);
    wrap::system_message(s_command);

    if( (file=popen(s_command.c_str(), "r")) == NULL )
    {
     wrap::system_message( SHELLER );
    }

    else
    { 
     while(true)
     {
      if(fgets(buffer, READBUF, file) == NULL)
       break;
      switch (m_method) {
        case METH_NCURSES: 
		wrap::system_message( clean_char(buffer) );
		break;
	default:
		s_ret.append("\n" + string(buffer));
      } // switch
     }
     pclose(file);
    }

    return s_ret;
}

#endif

