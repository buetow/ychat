#ifndef THRO_CPP
#define THRO_CPP

#include "thro.h"

using namespace std;

thro::thro()
{
}

thro::~thro()
{
}

void
thro::run()
{
 void* p_void;
 run( p_void );
}

void
thro::run( void *p_void )
{
 elem.p_thro = this;
 elem.p_void = p_void;
 pthread_create( &pthread, NULL, start_, &elem );
}

void*
thro::start_( void *p_void )
{
 elements *e = (elements*) p_void;
 e->p_thro->start( e->p_void ); 
}

void
thro::start( void *p_void )
{
 wrap::system_message( THRDSTR );
}


#endif
