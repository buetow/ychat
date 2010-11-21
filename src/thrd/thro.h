/*:*
 *: File: ./src/thrd/thro.h
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

#include "../incl.h"

#ifndef THRO_H
#define THRO_H

using namespace std;

class thro
{
private:
  pthread_t pthread;

  struct elements
  {
    thro *p_thro;
    void *p_void;
  }
  elem;

  static void *start_( void *p_void );

public:
  thro( );
  ~thro( );
  void run();
  void run( void *p_void );
  virtual void start( void *p_void );
};

#endif
