/*:*
 *: File: ./src/glob.h
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.5.6-BASIC
 *: 
 *: Copyright (C) 2003, 2004 Paul C. Buetow, Volker Richter
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

// global variables.

#ifndef GLOB_H
#define GLOB_H

#include <map>
#include <pthread.h>

// definition of boolean values.
#define true   1
#define false  0

// config filename.
#define CONFILE "conf.txt"

// the highest port which is allowed to use. if ychat is unable to create the server
// socket it will increment the port number and tries to create another socket.
// this procedure will go on until MAXPORT is reached.
#define MAXPORT 65535

// max length of a line read from a socket or a file ( config-file, html-template ).
#define READBUF 1024

// definition for verbosity level 0 ( normal outputs ). see vmsg.h for custumizing all
// the messages. this messages will only printed out by the master thread.
#define VERBOSE

// Defines the amount of newlines which have to send to the client's
// chat stream the first log-in. ( prevents white screen because of buffers
// or proxies ).
#define PUSHSTR 1000


//////////////////////////////////////////////////////////////////////////////////////////
// DO NOT CHANGE ANYTHING BEHIND THIS LINE!
//////////////////////////////////////////////////////////////////////////////////////////

using namespace std;

// internal rang descriptors ( their external names may be specified different )
enum rang
{
    CODER ,  // programmer.
    ADMIN ,  // administrator.
    MAGIC ,  // super user with special privileges.
    SUPER ,  // temporary super user.
    BASIC ,  // normal user without special privileges.
    GUEST ,  // guest user, has almost no privileges.
    RESTR ,  // a very restrivted user.
    OUTBN    // banned out of the system.
};

// some custom typedefs for datatypes which are needed often.
typedef map<string, string> map_string;
typedef int function( void *v_arg );

struct container
{
    void* elem[3];
};

struct dynmod
{
    function *the_func  ;
    void     *the_module;
};

#endif
