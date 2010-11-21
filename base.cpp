/*
	This file is part of yChat

	$Author: cvs $
	$Date: 2004-04-05 10:37:17 $
	
	$Header: /cvs/cvsroot/ychat-basic/base.cpp,v 1.1.1.1 2004-04-05 10:37:17 cvs Exp $
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

template<class type> void
base<type>::add_elem( type* p_type )
{
 pthread_mutex_lock  ( &mut_map_elem   );
 map_elem->add_elem  ( p_type, p_type->get_name()); 
 pthread_mutex_unlock( &mut_map_elem   );
}

template<class type> void
base<type>::del_elem( string &s_name )
{
 pthread_mutex_lock  ( &mut_map_elem );
 map_elem->del_elem  ( s_name	     ); 
 pthread_mutex_unlock( &mut_map_elem );
}

template<class type> type*
base<type>::get_elem( string &s_name, bool &b_found )
{
 pthread_mutex_lock  ( &mut_map_elem );
 type* p_type = map_elem->get_elem( s_name ); 
 pthread_mutex_unlock( &mut_map_elem );

 b_found = p_type == NULL ?  false : true;

 return p_type; 
}

template<class type> void
base<type>::run_func( void (*func)(type*, void*), void* v_arg )
{
 pthread_mutex_lock  ( &mut_map_elem );
 map_elem->run_func( func, v_arg ); 
 pthread_mutex_unlock( &mut_map_elem );
}

#endif
