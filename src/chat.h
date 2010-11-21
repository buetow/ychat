/*:*
 *: File: ./src/chat.h
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

// class chat declaration.

#ifndef s_chat_H
#define s_chat_H

#include <vector>
#include "incl.h"
#include "base.h"
#include "room.h"
#include "user.h"
#include "sess.h"
#include "s_lang.h"
#include "s_sman.h"

using namespace std;

class chat : public base<room>
{
private:
    bool b_strip_html;

public:


    room* get_room( string &s_name, bool &b_found )
    {
        return static_cast<room*>( get_elem( s_name, b_found ) );
    }

    // public methods:
    explicit chat();  // a standard constructor.
    ~chat();          // destructor.

    // get the object of a specific user.
    virtual user* get_user( string &s_nick );
    virtual user* get_user( string &s_nick, bool &b_found );
    static  void get_user_( room* room_obj, void *v_arg   );

    // will be called every time a user tries to login.
    virtual void login( map_string &map_params );

    // will be called if a user posts a message.
    virtual void post ( user* u_user, map_string &map_params );
};

#endif
