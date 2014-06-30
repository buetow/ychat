/*:*
 *: File: ./src/sman.cpp
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

#ifndef SMAN_CXX
#define SMAN_CXX

#include "sman.h"

sman::sman()
{
    this->sessions=new hmap<sess *, string>(80);
    this->sessioncount=0;
}
sman::~sman()
{
    delete this->sessions;
}

string sman::generateId( int len )
{
    string valid_chars="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_";
    string s_ret="";
    srand(time(0)+160682);
    for(int i=0;i<len;i++)
    {
        int i_char=rand() % 64;
        s_ret+=valid_chars[i_char];
    }
    return s_ret;
}
sess *sman::createSession( )
{
    string new_id=this->generateId(s_tool::string2int( s_conf::get
                                       ().get_val( "SESSION_LENGTH" ) ) );

    sess* p_sess = getSession(new_id);

    // Prove if session id already exists.
    if (p_sess)
        return createSession();

    sess *new_sess= new sess( new_id  );

    this->sessioncount++;
    this->sessions->add_elem( new_sess, new_id );


    return new_sess;
}

sess *sman::getSession( string s_id )
{
    return this->sessions->get_elem( s_id );
}
void sman::destroySession( string s_id )
{
    this->sessioncount--;
    this->sessions->del_elem( s_id );
}
#endif
