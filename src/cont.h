/*:*
 *: File: ./src/cont.h
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

// class cont declaration. defines a simple data container class.

#ifndef CONT_H
#define CONT_H

#include "incl.h"
#include "hmap.h"

using namespace std;

class cont
{
protected:
    map_string map_vals;

public:
    cont::~cont();

    // small inline methods:
    void clear_vals()
    {
        map_vals.clear();
    } // removes all values.

    // public methods:
    virtual string get_val( string s_key ); // get a specific map_vals value.
};

#endif
