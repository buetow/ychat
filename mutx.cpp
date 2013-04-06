// class mutx implementation. 

#ifndef s_mutx_CXX
#define s_mutx_CXX

#include "mutx.h"

using namespace std;

mutx::mutx()
{
 pthread_mutex_init( &mut_stdout, NULL );
}

mutx::~mutx()
{
 pthread_mutex_destroy( &mut_stdout );
}

#endif
