/*:*
 *: File: ./src/s_tool.h
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.5.6-BASIC
 *: 
 *: Copyright (C) 2003, 2004 Paul C. Buetow, Volker Richter
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

#ifndef s_tool_H
#define s_tool_H

#include "incl.h"

using namespace std;

class s_tool
{
public:
    static bool is_alpha_numeric( string &s_digit );
    static int  string2int( string s_digit );
    static string  int2string( int i_num );
    static string trim( string s_str );
    static long unixtime();
    static void strip_html( string *s_str);
    static string getExtension( string s_file );
    static string replace( string s_string, string s_search, string s_replace );
};

#endif
