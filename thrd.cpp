// class thrd implementation.

#ifndef THRD_CXX
#define THRD_CXX

#include "thrd.h"
#include "MUTX.h"
#include "SOCK.h"

using namespace std;

thrd::thrd( int i_sock )
{
#ifdef VERBOSE
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "thrd::thrd( " << i_sock << " )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 this->i_sock = i_sock;
}

thrd::~thrd()
{
#ifdef VERBOSE
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "thrd::~thrd()" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

  shutdown ( get_sock() , 2 );
}

void
thrd::run()
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "thrd::run()" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 SOCK::get().read_write( i_sock );
}

#endif
