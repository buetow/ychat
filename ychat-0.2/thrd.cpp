// class thrd implementation.

#ifndef THRD_CXX
#define THRD_CXX

#include "thrd.h"
#include "SOCK.h"

using namespace std;

thrd::thrd( int i_sock )
{
 this->i_sock = i_sock;
}

thrd::~thrd()
{
  shutdown ( get_sock() , 2 );
}

void
thrd::run()
{
 SOCK::get().read_write( this, i_sock );
}

#endif
