// class cmnd implementation.

#ifndef CMND_CXX
#define CMND_CXX

#include "cmnd.h"
#include "MUTX.h"

using namespace std;

cmnd::cmnd( )
{
#ifdef VERBOSE
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "cmnd::cmnd( )" << cout;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif
}

cmnd::~cmnd()
{
#ifdef VERBOSE
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "cmnd::~cmnd( )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif
}

#endif
