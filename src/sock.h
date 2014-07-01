/*:*
 *: File: ./src/sock.h
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

// class sock declaration.

#ifndef s_sock_H
#define s_sock_H

#include <queue>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "incl.h"
#include "pool.h"
#include "reqp.h"
#include "thrd.h"
#include "user.h"
using namespace std;

class sock
{
private:
    // total number of server requests.
    unsigned long long int i_req;

    bool  b_run;      // true while socket manager is running.
    reqp* req_parser; // parses the http requests from clients.
    pool* thrd_pool;  // the thread pool.
    // creates a server socket.
    virtual int make_socket( uint16_t port );

public:
    // small inline methods:
    bool get_run() const
    {
        return b_run;
    }
    bool set_run( bool b_run )
    {
        this->b_run = b_run;
    }

    // public methods.
    explicit sock( ); // simple constructor.
    virtual int  read_write( thrd* p_thrd, int filedes   );
    virtual int  start();

    // the chat stream there all the chat messages will sent through.
    static void chat_stream( int i_sock, user* p_user, map_string &map_params );

};

#endif
