/*:*
 *: File: ./src/tool/tool.cpp
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.8.3-CURRENT
 *: 
 *: Copyright (C) 2003 Paul C. Buetow, Volker Richter
 *: Copyright (C) 2004 Paul C. Buetow
 *: Copyright (C) 2005 EXA Digital Solutions GbR
 *: 
 *: This program is free software; you can redistribute it and/or
 *: modify it under the terms of the GNU General Public License
 *: as published by the Free Software Foundation; either version 2
 *: of the License, or (at your option) any later version.
 *: 
 *: This program is distributed in the hope that it will be useful,
 *: but WITHOUT ANY WARRANTY; without even the implied warranty of
 *: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *: GNU General Public License for more details.
 *: 
 *: You should have received a copy of the GNU General Public License
 *: along with this program; if not, write to the Free Software
 *: Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *:*/

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

bool
tool::is_alpha_numeric( string &s_digit )
{
  const char *p_digit = s_digit.c_str();
  int   i_len = strlen( p_digit );

  for( int i=0; i<i_len; i++ )
  {
    if ( ! isalnum( *p_digit ) )
      return false;
    p_digit++;
  }

  return true;
}

string
tool::int2string( int i_int )
{
  char buf[64];
  sprintf(buf, "%d", i_int);
  return buf;
}

long
tool::unixtime()
{
  time_t clock;
  return (long) time( &clock );
}

int
tool::string2int( string s_digit )
{
  const char *p_digit = s_digit.c_str();
  int i_res = 0;

  // Convert each digit char and add into result.
  while (*p_digit >= '0' && *p_digit <='9')
  {
    i_res = (i_res * 10) + (*p_digit - '0');
    p_digit++;
  }

  // Check that there were no non-digits at end.
  if (*p_digit != 0)
  {
    return -1;
  }

  return i_res;
}

string
tool::to_lower( string s_str )
{
  string s_tmp("");

  for( int i = 0; i < s_str.size() ;i++ )
    s_tmp = s_tmp + (char) tolower( s_str.at(i) );

  return s_tmp;
}

void
tool::strip_html( string *p_str)
{
  int i_pos;

  if( (i_pos=p_str->find("<", 0)) == string::npos )
    return;

  while(true)
  {
    p_str->replace(i_pos, 1, "&lt;");

    if( (i_pos = p_str->find("<", 0)) == string::npos )
      return;
  }
}

string
tool::ychat_version()
{
  return "yChat " + string(VERSION)
         + "-" + string(BRANCH)
         + " Build " + int2string(BUILDNR);
}

list<string>
tool::split_string(string s_string, string s_split)
{
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
  if( s_str.empty() )
    return s_str;

  char c_cur = s_str[0];
  int i_pos = 0;

  // left trim
  while ( c_cur == ' '|| c_cur == '\n' || c_cur == '\r' )
  {
    s_str.erase(i_pos,1);
    c_cur = s_str[++i_pos];
  }

  // right trim
  i_pos = s_str.size();
  c_cur = s_str[s_str.size()];

  while ( c_cur == ' ' || c_cur == '\n' || c_cur == '\0' || c_cur == '\r' )
  {
    s_str.erase(i_pos, 1);
    c_cur = s_str[--i_pos];
  }

  return s_str;
}

char*
tool::clean_char( char* c_str )
{
  // Ralf:
  for ( char* c_pos = c_str; *c_pos != '\0'; ++c_pos )
    if ( iscntrl(*c_pos) )
      *c_pos = ' ';

  return c_str;
}

string
tool::replace( string s_string, string s_search, string s_replace )
{
  unsigned i_pos[2];

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
  int i_pos = s_file.find_last_of(".");

  if( i_pos != string::npos )
  {
    string s_ext = s_file.substr(i_pos+1, s_file.size()-i_pos-1 );
    for( int i = 0; i < s_ext.size(); ++i )
      s_ext[i] = tolower(s_ext[i]);

    return to_lower(s_ext);
  }

  return "";
}

char*
tool::int2char( int i_int )
{
  char *buf = new char[64];
  sprintf(buf, "%d", i_int);
  return buf;
}

string
tool::shell_command( string s_command, method m_method )
{
  FILE *file;
  char buf[READBUF];
  char *c_pos;
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
      if(fgets(buf, READBUF, file) == NULL)
        break;

      switch (m_method)
      {
      case METH_NCURSES:
        wrap::system_message( clean_char(buf) );
        break;
      default:
        s_ret.append("\n" + string(buf));
      } // switch
    }

    pclose(file);
  }

  return s_ret;
}

#endif

