#include "../incl.h"

#ifndef SOCK_H
#define SOCK_H

#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "../reqp.h"
#include "../chat/user.h"
#include "../thrd/pool.h"
#include "../maps/shashmap.h"

#ifdef LOGGING
#include "../logd.h"
#endif

using namespace std;

class sock : protected shashmap
< string, uint32_t, self_hash<uint32_t>, equals_allocator<uint32_t> >
{
private:
#ifdef LOGGING
  logd *log_daemon; // the log daemon
#endif

  // total number of server requests.
  unsigned long long i_req;
  bool  b_run;      // true while socket manager is running.
  reqp *req_parser; // parses the http requests from clients.

  char *c_buffer;   // char buffer!
  pthread_mutex_t mut_hits;

  static string inet_ntoa_callback(void* p_void);

public:
  // creates a server socket.
  int make_server_socket( int i_port );

  // small inline methods:
  bool get_run() const
  {
    return b_run;
  }

  bool set_run( bool b_run )
  {
    this->b_run = b_run;
  }

  sock( );
  ~sock( );

  int read_write( int* p_sock );
  int start();
  void clean_ipcache();

  // the chat stream there all the chat messages will sent through.
  static void chat_stream( int i_sock, user* p_user, map<string,string> &map_params ); //<<

#ifdef NCURSES

  void print_hits();
#endif

};

#endif
