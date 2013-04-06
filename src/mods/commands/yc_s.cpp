/*:*
 *: File: ./src/mods/commands/yc_s.cpp
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

#include "../../chat/user.h"
#include "../../chat/room.h"

using namespace std;

extern "C"
{
  string to_upper( string &s_msg )
  {
    char* c_msg = (char*) s_msg.c_str();

    for ( int i = 0; i < strlen( c_msg ); i++ )
      c_msg[i] = toupper( c_msg[i] );

    return string( c_msg );
  };

  int extern_function(void *v_arg)
  {
    container *c=(container *)v_arg;

    user *p_user = (user*) c->elem[1];		// the corresponding user
    vector<string> *params= (vector<string>*) c->elem[2];	// param array

    conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF;
    timr* p_timr = (timr*) ((dynamic_wrap*)c->elem[3])->TIMR;

    string s_time = "";
    if ( p_conf->get_elem("chat.printalwaystime") == "true" )
      s_time = p_timr->get_time() + " ";

    string s_msg = s_time + "<i>" + p_user->get_colored_name() + " " + p_conf->get_elem("chat.msgs.scream") + " <font color=\"#" + p_user->get_col2() + "\"><b>";

    if ( ! params->empty() )
    {
      vector<string>::iterator iter = params->begin();
      for ( iter = params->begin(); iter != params->end(); iter++ )
        s_msg.append( to_upper(*iter) + " " );
    }

    s_msg.append( "</b></font></i><br>\n" );
    p_user->get_room()->msg_post( &s_msg );

    return 0;

  }
}

