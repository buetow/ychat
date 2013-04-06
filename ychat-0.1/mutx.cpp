// class mutx implementation. 

#ifndef MUTX_CXX
#define MUTX_CXX

#include "mutx.h"

using namespace std;

mutx::mutx()
{
#ifdef VERBOSE
 cout << "mutx::mutx()" << endl;
#endif

 pthread_mutex_init( &mut_stdout, NULL );
}

mutx::~mutx()
{
#ifdef VERBOSE
 cout << "mutx::~mutx()" << endl;
#endif

 pthread_mutex_destroy( &mut_stdout );
}

#endif
