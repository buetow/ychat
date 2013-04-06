// template class data implementation;

#ifndef BASE_CPP
#define BASE_CPP

#include "base.h"
#include "MUTX.h"

base::base()
{
#ifdef VERBOSE
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "base::base()" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 pthread_mutex_init (&mut_vec_elem, NULL ); 
}

base::~base( )
{
#ifdef VERBOSE
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "base::~base( )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 pthread_mutex_destroy( &mut_vec_elem );
}

void
base::add_elem( name* p_name )
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "base::add_elem( name* )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 pthread_mutex_lock  ( &mut_vec_elem   );
 vec_elem.push_back  ( p_name          ); 
 pthread_mutex_unlock( &mut_vec_elem   );
}

bool
base::del_elem( string &s_name )
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "base::del_elem( \"" << s_name << "\" )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 vector<name*>::iterator iter;
 pthread_mutex_lock  ( &mut_vec_elem );

 iter = vec_elem.begin();
 while( iter != vec_elem.end() )
 {
   if ( (*iter)->get_name() == s_name ) 
   {
    vec_elem.erase( iter );
    pthread_mutex_unlock( &mut_vec_elem );
    return true;
   }
   iter++;
 }

 pthread_mutex_unlock( &mut_vec_elem );
 return false;
}

name*
base::get_elem( string &s_name, bool &b_found )
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "base:get_elem( \"" << s_name << "\", bool )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 vector<name*>::iterator iter;
 pthread_mutex_lock  ( &mut_vec_elem );

 iter = vec_elem.begin();
 while( iter != vec_elem.end() )
 {
   if ( (*iter)->get_name() == s_name ) 
   {
    b_found = true;
    pthread_mutex_unlock( &mut_vec_elem );
    return (*iter);
   }
   iter++;
 }

 pthread_mutex_unlock( &mut_vec_elem );

 b_found = false;

 return new name(); 
}

void
base::run_func( void (*func)(name*, void*), void* v_arg )
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "base:run_func( void (*func)(name*, void*), void* )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 vector<name*>::iterator iter;
 pthread_mutex_lock  ( &mut_vec_elem );

 // execute func foreach element of vec_elem with 
 // 1st argument: a pointer of a element of vec_elem.
 // 2nd argument: a void pointer of a object.
 for( iter = vec_elem.begin(); iter != vec_elem.end(); iter++ )
   ( *func ) ( (*iter), v_arg );
 
 pthread_mutex_unlock( &mut_vec_elem );
}

#endif
