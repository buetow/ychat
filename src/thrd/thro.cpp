/*:*
 *: File: ./src/thrd/thro.cpp
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

#ifndef THRO_CPP
#define THRO_CPP

#include "thro.h"

using namespace std;

thro::thro()
{}

thro::~thro()
{}

void
thro::run()
{
  void *p_void;
  run( p_void );
}

void
thro::run( void *p_void )
{
  elem.p_thro = this;
  elem.p_void = p_void;
  //wrap::POOL->add_task(start_, &elem);
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
