/*:*
 *: File: ./src/sess.cpp
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

#ifndef SESS_CPP
#define SESS_CPP

#include "sess.h"

sess::sess( string s_id )
{
    this->sess_id=s_id;
}

string sess::getId()
{
    return this->sess_id;
}


void sess::invalidate()
{
    this->sess_id="0";
    this->sess_values.clear();
}

void sess::setValue( string s_key, void *lpvalue )
{
    this->sess_values[s_key]=lpvalue;
}
void *sess::getValue( string s_key )
{
    return this->sess_values[s_key];
}
string sess::dump()
{
    string s_ret=string("Session Dump of Session ") + this->getId();
    map<string, void*>::const_iterator it;
    for(it=this->sess_values.begin();it!=this->sess_values.end();it++)
        s_ret=s_ret + "\nkey: " + it->first;
    return s_ret;
}
#endif
