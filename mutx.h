// class mutx declaration.

#ifndef MUTX_H
#define MUTX_H

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
