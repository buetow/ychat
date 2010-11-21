/*:*
 *: File: ./src/mods/commands/yc_fake.cpp
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

#include "../../incl.h"
#include "../../chat/room.h"
#include "../../chat/user.h"

using namespace std;

extern "C"
{
  int extern_function(void *v_arg)
  {
    container *c=(container *)v_arg;

    user* p_user = (user*) c->elem[1];		// the corresponding user
    conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF;

    bool b_fake = p_user->get_fake();

    string s_msg = (b_fake == 1
                    ? p_conf->get_elem("chat.msgs.unsetmodefake") + "<br>\n"
                    : p_conf->get_elem("chat.msgs.setmodefake") + "<br>\n");


    p_user->set_fake( !b_fake );
    p_user->get_room()->reload_onlineframe();
    p_user->msg_post ( &s_msg  );

    return 0;
  }
}

