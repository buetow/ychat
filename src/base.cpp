/*:*
 *: File: ./src/base.cpp
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

/*
	This file is part of yChat
 
	$Author: buetow $
	$Date: 2005-11-12 15:51:43 $
	
	$Header: /cvs/cvsroot/ychat-0.5/src/base.cpp,v 1.4 2005-11-12 15:51:43 buetow Exp $
*/
// template class data implementation;

#ifndef BASE_CPP
#define BASE_CPP

#include "base.h"

template<class type>
base<type>::base()
{
    map_elem = new hmap<type*,string>(80);
    pthread_mutex_init (&mut_map_elem, NULL );
}

template<class type>
base<type>::~base( )
{
    pthread_mutex_destroy( &mut_map_elem );
}

template<class type>
void
base<type>::add_elem( type* p_type )
{
    pthread_mutex_lock  ( &mut_map_elem   );
    map_elem->add_elem  ( p_type, p_type->get_name());
    pthread_mutex_unlock( &mut_map_elem   );
}

template<class type>
void
base<type>::del_elem( string &s_name )
{
    pthread_mutex_lock  ( &mut_map_elem );
    map_elem->del_elem  ( s_name	     );
    pthread_mutex_unlock( &mut_map_elem );
}

template<class type>
type*
base<type>::get_elem( string &s_name, bool &b_found )
{
    pthread_mutex_lock  ( &mut_map_elem );
    type* p_type = map_elem->get_elem( s_name );
    pthread_mutex_unlock( &mut_map_elem );

    b_found = p_type == NULL ?  false : true;

    return p_type;
}

template<class type>
void
base<type>::run_func( void (*func)(type*, void*), void* v_arg )
{
    pthread_mutex_lock  ( &mut_map_elem );
    map_elem->run_func( func, v_arg );
    pthread_mutex_unlock( &mut_map_elem );
}

#endif
