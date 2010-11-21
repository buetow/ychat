/*:*
 *: File: ./src/sess.h
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

#ifndef s_sess_H
#define s_sess_H

#include "incl.h"
#include "cont.h"
#include "name.h"
#include <map>
#include <string>
#include "hmap.h"
using namespace std;

typedef map<string, void *> sess_map;

class sess : public cont, name
{

private:
    sess_map sess_values;
    string sess_id;
public:
    sess(string s_id);
    string getId();
    void setValue(string s_key, void *lpvalue);
    void *getValue( string s_key );
    void invalidate();
    string dump();
};
#endif
