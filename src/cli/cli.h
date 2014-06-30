/*:*
 *: File: ./src/cli/cli.h
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

#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>
#include <vector>

#ifndef	RUSAGE_SELF
#define RUSAGE_SELF 0
#endif
#ifndef	RUSAGE_CHILDREN
#define RUSAGE_CHILDREN -1
#endif

#include "../incl.h"

#ifndef CLI_H
#define CLI_H
#ifdef CLI

#ifndef NCURSES
#include "../thrd/thro.h"
#endif

#include "../monitor/dump.h"

using namespace std;

#ifndef NCURSES
class cli : public thro
{
#else
class cli 
{
#endif
private:
  int parse_input(string s_input);
  vector<string> vectorize(string s_param);
    
public:
cli( );
~cli( );

#ifdef DEBUG

void debug_routine();
#endif

void print_rusage();

#ifndef NCURSES

void start(void* p_void);
#else

void start();
#endif
};

#endif
#endif
