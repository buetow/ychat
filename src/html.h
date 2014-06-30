/*:*
 *: File: ./src/html.h
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

// class html declaration. this class manages the html-template files.

#ifndef s_html_H
#define s_html_H

#include "incl.h"
#include "cont.h"
#include "s_conf.h"
#include "user.h"
#include "name.h"


using namespace std;

class html : public cont, name
{
private:
    // needed for synchronizing the map_vals.
    pthread_mutex_t mut_map_vals;

public:
    // public methods.
    explicit html( ); // simple constructor.
    ~html( );

    // clears the template cache so that new html templates will be read
    // from hard disk. this method is needed after changeing s.t. on
    // the html-template files.
    void clear_cache( );

    // returns a parsed html-template. this method will check first if the
    // required html-template exists inside the classes template cache. if not
    // then the file will be read from file and added to the cache.
    // afterwards the html-template will be parsed and returned.
    // map_params contains the client request parameters which also will be
    // used for string substituation.
    virtual string parse( map_string &map_params );

    virtual void online_list( user *p_user, map_string &map_params );
};

#endif
