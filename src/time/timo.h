/*:*
 *: File: ./src/time/timo.h
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.8.3-CURRENT
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

#ifndef TIMO_H
#define TIMO_H

using namespace std;

class timo // timeout class
{
protected:
  time_t t_time;   // last activity time.
  pthread_mutex_t mut_t_time;

public:
  timo( );
  ~timo( );

  double get_last_activity();
  void renew_timeout();
};

#endif
