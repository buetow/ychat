// class cont implementation.

#ifndef CONT_CXX
#define CONT_CXX

#include "cont.h"
#include "MUTX.h"

using namespace std;

string
cont::get_val( string s_key )
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "cont::get_val( \"" << s_key << "\" )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif
 if ( map_vals.find( s_key ) != map_vals.end() )
  return map_vals[ s_key ];
 return string();
}

cont::~cont()
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "cont::cont()" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 map_vals.~map_string();
}

#endif
