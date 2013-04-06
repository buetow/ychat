// class mutx declaration.

#ifndef s_mutx_H
#define s_mutx_H

#include "incl.h"

using namespace std;

class mutx 
{
public:
 // this mutex is needed for sync stdout and sdterr of different threads.
 pthread_mutex_t mut_stdout;

 // public methods.
 explicit mutx( ); // simple constructor.
 ~mutx( ); // simple constructor.
};

#endif
