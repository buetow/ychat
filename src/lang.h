/*:*
 *: File: ./src/lang.h
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

// class conf declaration. this class parses the server config file.

#ifndef s_lang_H
#define s_lang_H

#include "incl.h"
#include "cont.h"
#include "name.h"

using namespace std;

class lang : public cont, name
{
private:

public:
    // public methods:
    lang ( string s_lang ); // standard constructor.
    ~lang(); 		 // standard destructor.

    virtual void parse( );    // parses the config file.
};

#endif
