/*:*
 *: File: ./src/time/timo.cpp
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.7.9.5-RELEASE
 *: 
 *: Copyright (C) 2003 Paul C. Buetow, Volker Richter
 *: Copyright (C) 2004 Paul C. Buetow
 *: Copyright (C) 2005 EXA Digital Solutions GbR
 *: 
 *: This program is free software; you can redistribute it and/or
 *: modify it under the terms of the GNU General Public License
 *: as published by the Free Software Foundation; either version 2
 *: of the License, or (at your option) any later version.
 *: 
 *: This program is distributed in the hope that it will be useful,
 *: but WITHOUT ANY WARRANTY; without even the implied warranty of
 *: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *: GNU General Public License for more details.
 *: 
 *: You should have received a copy of the GNU General Public License
 *: along with this program; if not, write to the Free Software
 *: Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *:*/

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
