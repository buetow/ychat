// class room implementation.

#ifndef ROOM_CXX
#define ROOM_CXX

#include "room.h"
#include "MUTX.h"

using namespace std;

room::room( string s_name ) : name( s_name )
{
#ifdef VERBOSE
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "room::room( \"" << s_name << "\" )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif
}

room::~room()
{
#ifdef VERBOSE
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "room::~room[ " << get_name() << " ]" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif
}

#endif
