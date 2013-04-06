// class name implementation.

#ifndef NAME_CXX
#define NAME_CXX

#include "name.h"
#include "MUTX.h"

using namespace std;

name::name( string s_name )
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "name::name( \"" << s_name << "\" )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 set_name( s_name );
}

name::~name()
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "name::~name[ " << s_name << " ]" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif
}

string
name::get_name() const
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "name::get_name()" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 return s_name;
}

void
name::set_name( string s_name )
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "name::set_name( \"" << s_name << "\" )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 this->s_name = s_name;
}
#endif
