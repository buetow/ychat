// class sock declaration.

#ifndef SOCK_H
#define SOCK_H

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

using namespace std;

class sock 
{
private:
 // total number of server requests. 
 unsigned long long int i_req; 

 bool  b_run;      // true while socket manager is running.
 reqp* req_parser; // parses the http requests from clients.
 pool* thrd_pool;  // the thread pool.

 // the chat stream there all the chat messages will sent through.
 static void chat_stream( int i_sock, map_string &map_params );

 // creates a server socket.
 virtual int make_socket( uint16_t port );

public:
 // small inline methods:
 bool get_run() const { return b_run; }
 bool set_run( bool b_run ) { this->b_run = b_run; }

 // public methods.
 explicit sock( ); // simple constructor.
 virtual int  read_write( thrd* p_thrd, int filedes   );
 virtual int  start();
};

#endif
