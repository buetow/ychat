/*:*
 *: File: ./src/tool/tool.h
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.7.9.5-RELEASE
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

#ifndef TOOL_H
#define TOOL_H

#include "../incl.h"

#include <list>

using namespace std;

class tool
{
public:
  static list<string> split_string(string s_string, string s_split);
  static bool is_alpha_numeric( string &s_digit );
  static char* int2char( int i_int );
  static char* clean_char( char* c_str);
  static string trim( string s_str );
  static string replace( string s_string, string s_search, string s_replace );
  static string int2string( int i_int );
  static long unixtime();
  static int string2int( string s_digit );
  static string get_extension( string s_file );
  static string to_lower( string s_str );
  static void strip_html( string *p_str );
  static string shell_command( string s_command, method m_method );
  static string ychat_version();
};

#endif
