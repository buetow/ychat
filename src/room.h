/*:*
 *: File: ./src/room.h
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

// class room declaration.

#ifndef ROOM_H
#define ROOM_H

#include "incl.h"
#include "base.h"
#include "name.h"
#include "user.h"

using namespace std;

class room : public base<user>, public name
{
private:

public:
    void add_user( user* p_user  )
    {
        p_user->set_p_room( this );
        add_elem( p_user );
    }

    user* get_user( string &s_name, bool &b_found )
    {
        return static_cast<user*>( get_elem( s_name, b_found ) );
    }

    // public methods:
    explicit room( string s_name );      // a constructor.
    ~room();  			       // room destructor.
};

#endif
