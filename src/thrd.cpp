/*:*
 *: File: ./src/thrd.cpp
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

// class thrd implementation.

#ifndef THRD_CXX
#define THRD_CXX

#include "thrd.h"
#include "s_sock.h"

using namespace std;

thrd::thrd( int i_sock )
{
    this->i_sock = i_sock;
}

thrd::~thrd()
{
    shutdown ( get_sock() , 2 );
    close    ( get_sock()     );
}

void
thrd::run()
{
    s_sock::get
        ().read_write( this, i_sock );
}

#endif
