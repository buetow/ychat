/*:*
 *: File: ./src/mods/html/yc_options.cpp
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

#include "../../incl.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C"
{
  int extern_function(void *v_arg)
  {
    container* c          = (container*) v_arg;
    dynamic_wrap* p_wrap  = (dynamic_wrap*) c->elem[0];
    conf*      p_conf     = (conf*) p_wrap->CONF;
    string*    p_flag     = &(*((map<string,string>*) c->elem[1]))["flag"];
    string*    p_email    = &(*((map<string,string>*) c->elem[1]))["email"];
    string*    p_pass     = &(*((map<string,string>*) c->elem[1]))["pass"];
    string*    p_newpass  = &(*((map<string,string>*) c->elem[1]))["newpass"];
    string*    p_newpass2 = &(*((map<string,string>*) c->elem[1]))["newpass2"];
    user*      p_user     = (user*) c->elem[2];

    if ( *p_flag == "submit" )
    {
      string*  p_msgs  = &(*((map<string,string>*) c->elem[1])
                          )["msgs"];

      // If the password has to be changed:
      if ( *p_pass != "" || *p_newpass != "" || *p_newpass2 != "" )
      {
        if ( p_user->get_pass() != *p_pass )
        {
          p_msgs->append( p_conf->get_elem( "chat.msgs.err.changepassword" ) );
          return 0;
        }

        if ( *p_newpass != *p_newpass2 )
        {
          p_msgs->append( p_conf->get_elem( "chat.msgs.err.changepassword2" ) );
          return 0;
        }
        p_user->set_pass( *p_newpass );
      }

      p_user->set_email( *p_email );
      p_msgs->append( p_conf->get_elem( "chat.msgs.optionschanged" ) );
    }
    else
    {
      p_email->append( p_user->get_email() );
    }

    return 0;
  }
}

