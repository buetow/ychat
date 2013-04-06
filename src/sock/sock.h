/*:*
 *: File: ./src/sock/sock.h
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

class sock : public shashmap
      < string, uint32_t, self_hash<uint32_t>, equals_allocator<uint32_t> >
{
protected:
#ifdef LOGGING

  logd *log_daemon; // the log daemon
#endif

  int i_server_sock;
  int i_server_port;

  // total number of server requests.
  unsigned long long i_req;
  bool  b_run;      // true while socket manager is running.
  reqp *req_parser; // parses the http requests from clients.
  char *c_buffer;   // char buffer!
  pthread_mutex_t mut_hits;

  static string inet_ntoa_callback(void* p_void);

public:
  // creates a server socket.
  int read_http(socketcontainer *p_sock, char *c_zbuf, int i_buflen, int  &i_payloadoffset);
  string read_http_line(socketcontainer *p_sock);

  // small inline methods:
  bool get_server_() const
  {
    return b_run;
  }
  // small inline methods:
  bool get_run() const
  {
    return b_run;
  }
  bool set_run( bool b_run )
  {
    this->b_run = b_run;
  }

  sock();

  int read_write( socketcontainer* p_sock );

  int start();
  void clean_ipcache();

  // the chat stream there all the chat messages will sent through.
  void chat_stream( socketcontainer* p_sock, user* p_user, map<string,string> &map_params ); //<<
  virtual inline int _send(socketcontainer *p_sock, const char *sz, int len);
  virtual inline int _read(socketcontainer *p_sock, char *sz, int len);
  virtual inline int _close(socketcontainer *p_sock);
  virtual void _main_loop_init();
  virtual inline socketcontainer* _create_container(int& i_sock);
  virtual int _make_server_socket(int i_port);

#ifdef NCURSES
  void print_server_port();
  void print_hits();
#endif

};
#endif
