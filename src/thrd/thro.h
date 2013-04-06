#include "../incl.h"

#ifndef THRO_H
#define THRO_H

using namespace std;

class thro
{
private:
 pthread_t pthread;

 struct elements {
  thro *p_thro;
  void *p_void;
 } elem;  

 static void* start_( void *p_void );

public:
 thro( );
 ~thro( );
 void run();
 void run( void *p_void );
 virtual void start( void *p_void );
};

#endif
