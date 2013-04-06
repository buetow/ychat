#ifndef TIMO_CPP
#define TIMO_CPP

#include "timo.h"

using namespace std;

timo::timo()
{
    pthread_mutex_init( &mut_t_time, NULL );
}
timo::~timo()
{
    pthread_mutex_destroy( &mut_t_time );
}

double
timo::get_last_activity( )
{
    double d_ret;
    pthread_mutex_lock  ( &mut_t_time );
    d_ret = wrap::TIMR->get_time_diff( t_time );
    pthread_mutex_unlock( &mut_t_time );
    return d_ret;
}

void
timo::renew_timeout( )
{
    pthread_mutex_lock  ( &mut_t_time );
    time( &t_time );
    pthread_mutex_unlock( &mut_t_time );
}


#endif
