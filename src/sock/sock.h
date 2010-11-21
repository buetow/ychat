/*:*
 *: File: ./src/sock/sock.h
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

#ifndef SOCK_H
#define SOCK_H

#include <queue>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../reqp.h"
#include "../chat/user.h"

#include "../thrd/pool.h"
#include "../maps/shashmap.h"

#ifdef LOGGING
#include "../logd.h"
#endif

using namespace std;

class sock 
{
protected:
#ifdef LOGGING

  logd *log_daemon; // the log daemon
#endif
  shashmap< string, unsigned, self_hash<unsigned>, equals_allocator<unsigned> > *ip_cache_map;

  int i_server_sock;
  int i_server_port;

  unsigned long long i_req; // total number of server requests.
  bool  b_run;      		// true while socket manager is running.
  reqp *req_parser; 		// parses the http requests from clients.
  char *c_buffer;   		// char buffer!
  pthread_mutex_t mut_hits;

  static string inet_ntoa_callback(void* p_void);

public:
  // creates a server socket.
  int read_http(_socket *p_sock, char *c_zbuf, int  &i_payloadoffset);
  string read_http_line(_socket *p_sock);

  // small inline methods:

  string dump() { return ip_cache_map->dump(); }
  bool get_server_() const { return b_run; }
  bool get_run() const { return b_run; }
  bool set_run( bool b_run ) { this->b_run = b_run; }

  sock();

  int read_write(_socket* p_sock);

  int start();
  void clean_ipcache();

  // the chat stream there all the chat messages will sent through.
  void chat_stream(_socket* p_sock, user* p_user, map<string,string> &map_params); //<<
  virtual int _send(_socket *p_sock, const char *sz, int len);
  virtual int _read(_socket *p_sock, char *sz, int len);
  virtual int _close(_socket *p_sock);
  virtual void _main_loop_init();

#ifdef OPENSSL
  virtual bool _main_loop_do_ssl_stuff(int& i_new_sock);
#endif

  virtual _socket* _create_container(int& i_sock);
  virtual int _make_server_socket(int i_port);
};
#endif
