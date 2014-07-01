/*:*
 *: File: ./src/mods/commands/yc_unbannick.cpp
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

#include "../../chat/user.h"
#include "../../chat/room.h"

using namespace std;

extern "C"
{
  int extern_function(void *v_arg)
  {
    container *c=(container *)v_arg;

    user *p_user = (user*) c->elem[1];		// the corresponding user
    vector<string> *params = (vector<string>*) c->elem[2];	// param array
    chat* p_chat = (chat*) ((dynamic_wrap*)c->elem[3])->CHAT;
    conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF;
    timr* p_timr = (timr*) ((dynamic_wrap*)c->elem[3])->TIMR;

    if ( ! params->empty() )
    {
      vector<string>::iterator iter = params->begin();
      string s_unbannick_user(*iter);

      string s_reason = p_chat->unban_nick(s_unbannick_user);
      if (s_reason.empty()) { 
        string s_msg = "<font color=\"#"
                       + p_conf->get_elem("chat.html.errorcolor")
                       + "\"><b>"
                       + s_unbannick_user + "</b> "
                       + p_conf->get_elem("chat.msgs.err.isnotbanned")
                       + "</font><br>\n";

        p_user->msg_post( &s_msg );
 	return 0;
      }	

      string s_time = "";
      if ( p_conf->get_elem("chat.printalwaystime") == "true" )
        s_time = p_timr->get_time() + " ";

      string s_msg = s_time + "<i> " + p_user->get_colored_bold_name() + " " + p_conf->get_elem("chat.msgs.unban") + " <b>"
			+ s_unbannick_user + "</b> " + s_reason + "</i><br>\n";

      p_user->msg_post(s_msg);
    }

    return 0;
  }
}

