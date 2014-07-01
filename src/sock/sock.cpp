/*:*
 *: File: ./src/sock/sock.cpp
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

#ifndef SOCK_CPP
#define SOCK_CPP

#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#include "sock.h"

using namespace std;

sock::sock()
{
  this->b_run      = true;
  this->i_req      = 0;
  this->req_parser = new reqp();

#ifdef LOGGING
  this->log_daemon = new logd( wrap::CONF->get_elem( "httpd.logging.accessfile" ), 
                               wrap::CONF->get_elem( "httpd.logging.access_lines" ) );
#endif
}

  this->ip_cache_map = new shashmap
	< string, unsigned, self_hash<unsigned>, equals_allocator<unsigned> >;
}

int
sock::_send(_socket *p_sock, const char *sz, int len)
{
#ifdef OPENSSL
  return send(p_sock->i_sock, sz, len, 0);
#else
  return send(*p_sock, sz, len, 0);
#endif
}

int
sock::_read(_socket *p_sock,  char *sz, int len)
{
#ifdef OPENSSL
  return read(p_sock->i_sock, sz, len);
#else
  return read(*p_sock, sz, len);
#endif
}

int
sock::_close(_socket *p_sock)
{
#ifdef OPENSSL
  shutdown( p_sock->i_sock, 2 );
  close   ( p_sock->i_sock    );
#else
  shutdown( *p_sock, 2 );
  close   ( *p_sock    );
#endif
  delete p_sock;
}

//<<*
void
sock::chat_stream( _socket *p_sock, user *p_user, map<string,string> &map_params )
{
  string s_msg( "\n" );

  for ( int i = 0; i < PUSHSTR; i++ )
    _send(p_sock,s_msg.c_str(), s_msg.size());

  pthread_mutex_t mutex;
  pthread_mutex_init( &mutex, NULL );
  pthread_mutex_lock( &mutex );

  do
  {
    s_msg = p_user->get_mess( );

    if ( 0 > _send( p_sock, s_msg.c_str(), s_msg.size() ) )
    {
      p_user->set_online( false );
      break;
    }

    pthread_cond_wait( &(p_user->cond_message), &mutex );
  }
  while( p_user->get_online() );

  pthread_mutex_destroy( &mutex );

  // if there is still a message to send:
  s_msg = p_user->get_mess( );

  if ( ! s_msg.empty() )
    _send( p_sock, s_msg.c_str(), s_msg.size());

  // remove the user from its room.
  string s_user( p_user->get_name() );
  string s_user_lowercase( p_user->get_lowercase_name() );

  room* p_room = p_user->get_room();
  p_room->del_elem( s_user_lowercase );

  // post the room that the user has left the chat.
  s_msg = wrap::TIMR->get_time() + " "
          + p_user->get_colored_bold_name()
          + wrap::CONF->get_elem( "chat.msgs.userleaveschat" )
          + "<br>\n";

  p_room->msg_post( &s_msg );
  p_room->reload_onlineframe();

#ifdef VERBOSE
  cout << REMUSER << s_user << endl;
#endif

  wrap::GCOL->add_user_to_garbage( p_user );
}
//*>>

int
sock::_make_server_socket( int i_port )
{
  size_t i_sock;
  struct sockaddr_in name;

  // create the server socket.
  i_sock = socket (PF_INET, SOCK_STREAM, 0);
  if (i_sock < 0)
  {
    wrap::system_message( SOCKERR );

    if ( ++i_port > MAXPORT )
      exit(1);

    wrap::system_message( SOCKERR );

    return _make_server_socket( i_port );
  }

  // give the server socket a name.
  name.sin_family = AF_INET;
  name.sin_port = htons(i_port);
  name.sin_addr.s_addr = htonl(INADDR_ANY);
  int i_optval = 1;

  setsockopt( i_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&i_optval, sizeof(int) );

  if ( bind(i_sock, (struct sockaddr *) &name, sizeof (name)) < 0 )
  {
    wrap::system_message( BINDERR );

    if ( ++i_port > MAXPORT )
      exit(1);

    wrap::system_message( string(SOCKERR) + tool::int2string(i_port) );

    // Rerun recursive.
    return _make_server_socket( i_port );
  }

  wrap::system_message( SOCKCRT + string("localhost:") + tool::int2string(i_port) );

  i_server_port = i_port;
  i_server_sock = i_sock;

  return i_sock;
}

string
sock::read_http_line(_socket *p_sock)
{
  string s_line;
  int i_total = 0;
  int i_read = 0;
  char ch;

  do
  {
    i_read = _read(p_sock, &ch, sizeof(char));

    if(i_read <= 0)
      return "";

    s_line += ch;
    i_total++;
  }
  while((ch != '\n') && i_total < MAXLENGTH);

  if(ch != '\n')
    /*
    **  the games people play
    */
    return "";

  return s_line;
}
int
sock::read_http(_socket *p_sock, char *c_zbuf, int &i_postpayloadoffset)
{
  /*
  **  1)  Read the first line
  **  2)  If GET, handle as such
  **  3)  If POST, handle as such
  */
  char ch;
  int i_read;
  int i_ret = -1;
  int x,z;

  string s_content_length;
  string s_cl;
  string s_post_return;
  string s_line = read_http_line(p_sock);

  i_postpayloadoffset = 0;
  if(s_line.empty()) {
    wrap::system_message(SOCKER4);
    return -1;
  }

  /*
  **  GET yada\r\n Followed by stuff we don't care about :) heh.
  **  01234
  */
  /*
  **  POST yada\r\n
  **  xxxxx
  **  Content-Length: NNN\n
  **  \n
  */
  if(s_line.substr(0, 3) == "GET")
  {
    if(s_line.length() > READSOCK)
      /*
      **  Buffer overflow
      */
	  s_line = s_line.substr(0, READSOCK);

    memcpy(c_zbuf,s_line.c_str(),s_line.length());
    return s_line.length();
  }

  else
  {
    /*
    **  POST yada
    **  01234
    */
    if(s_line.substr(0, 4) != "POST") {
      wrap::system_message(SOCKER4);
      return -1;
    }

    /*
    **  Get us to the Content-Length:
    */
    s_post_return += s_line;
    i_postpayloadoffset += s_line.length();

    for(x =0; x < MAXLINES; ++x)
    {
      s_line = read_http_line(p_sock);
      s_post_return += s_line;
      i_postpayloadoffset += s_line.length();

      if (s_line.compare(0, 15, "Content-Length:"))
        continue;

      // Match found on Content-Length:...  process, and then break out and get us to the promised land
      s_content_length = s_line.substr( 16 /*strlen("Content-Length: ")*/, 
		         s_line.length() - 16 /*strlen("Content-Length: ")*/);

      /*
      **  Content-Length: 333\n
      **  0123456789abcdefghijklmnopqrstuvwxyzAB
      */
      z = 0;

      do
      {
        ch = s_content_length[z];
        if(isdigit(ch))
          s_cl += ch;

        ++z;
      }
      while(ch != '\n');

      break;
    }

    if(s_cl.empty()) {
      wrap::system_message(SOCKER4);
      return -1;
    }

    z = atoi(s_cl.c_str());

    /*
    **  If we are going to overflow the buffer just by the payload, leave
    **  of, if z did not convert correctly. (should have been ok by isdigit)
    */
    if(z > READSOCK || z < 0) {
      wrap::system_message(SOCKER4);
      return -1;
    }

    /*
    **  We have MAXLINES to get to the blank line separating POST data.
    */
    for(x = 0; x < MAXLINES; ++x)
    {
      s_line = read_http_line(p_sock);
      s_post_return += s_line;

      i_postpayloadoffset += s_line.length();
      if(s_line == "\r\n")
        break;
    }

    /*
    **  funny business
    */
    if(x == MAXLINES) {
      wrap::system_message(SOCKER4);
      return -1;
    }

    for(x = 0; x < z; ++x)
    {
      if(_read(p_sock,&ch,sizeof(ch)) != 1) {
        wrap::system_message(SOCKER4);
        return -1;
      }

      s_post_return += ch;
    }

    if(s_post_return.length() > READSOCK) {
      wrap::system_message(SOCKER4);
      return -1;
    }

    memcpy(c_zbuf,s_post_return.c_str(),s_post_return.length());
    return s_post_return.length();
  }
}

int
sock::read_write(_socket* p_sock)
{
  int i_postpayloadoffset;

#ifdef OPENSSL
  int i_sock = p_sock->i_sock;
#else
  int i_sock = *p_sock;
#endif

  char c_req[READSOCK+1];
  memset(c_req, 0, sizeof(c_req));

  int i_bytes = read_http(p_sock, c_req, i_postpayloadoffset);

  if (i_bytes <= 0)
  {
    wrap::system_message( READERR );

  }

  else
  {
    // stores the request params.
    map<string,string> map_params;

    // get the s_rep ( s_html response which will be send imediatly to the client
    struct sockaddr_in client;
    size_t size = sizeof(client);

    getpeername(i_sock, (struct sockaddr *)&client, &size);

    unsigned &s_addr = client.sin_addr.s_addr;
    if ( (map_params["REMOTE_ADDR"] = ip_cache_map->get_elem(s_addr)) == "" )
    {
      map_params["REMOTE_ADDR"] = string(inet_ntoa(client.sin_addr));
      ip_cache_map->set_elem(map_params["REMOTE_ADDR"], s_addr);
      wrap::system_message(SOCKCAC+map_params["REMOTE_ADDR"]);
    }

    string s_rep = req_parser->parse(p_sock, string(c_req), map_params, i_postpayloadoffset);

#ifdef LOGGING
    log_daemon->log_access(map_params);
#endif

    // send s_rep to the client.
    _send(p_sock, s_rep.c_str(), s_rep.size());

    // dont need those vals anymore.
    map_params.clear();

    _close(p_sock);
    return 0;
  }

  _close(p_sock);
  return 1;
}

void
sock::_main_loop_init()
{
  wrap::system_message(SOCKUNS);
}

#ifdef OPENSSL
// This method is virtual, and is overloaded by sslsock!
bool
sock::_main_loop_do_ssl_stuff(int &i_new_sock)
{
  return 0;
}
#endif

_socket*
sock::_create_container(int &i_sock)
{
  _socket* p_sock = new _socket;

#ifdef OPENSSL
  p_sock->i_sock = i_sock;
#else
  *p_sock = i_sock;
#endif
  return p_sock;
}

int
sock::start()
{
  wrap::system_message( SOCKSRV );
  int i_sock = i_server_sock;

  int i_port = tool::string2int( wrap::CONF->get_elem("httpd.serverport") );
  _main_loop_init();

  int i;
  fd_set active_fd_set, read_fd_set;
  struct sockaddr_in clientname;
  size_t size;

  if (listen (i_sock, 1) < 0)
  {
    wrap::system_message( LISTERR );
    exit( EXIT_FAILURE );
  }

  wrap::system_message( SOCKRDY );

  // initialize the set of active sockets.
  FD_ZERO (&active_fd_set);
  FD_SET  (i_sock, &active_fd_set);

  while(b_run)
  {
    // block until input arrives on one or more active sockets.
    read_fd_set = active_fd_set;
    if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
    {
      wrap::system_message( SELCERR );
      exit(EXIT_FAILURE);
    }

    // service all the sockets with input pending.
    for (i = 0; i < FD_SETSIZE; ++i)
      if (FD_ISSET(i, &read_fd_set))
      {
        if (i == i_sock)
        {
          ++i_req; // connection request on original socket.

          int i_new_sock;
          size = sizeof(clientname);
          i_new_sock = accept(i_sock, (struct sockaddr *) &clientname, &size);

#ifdef OPENSSL
          if (_main_loop_do_ssl_stuff(i_new_sock))
            continue;
#endif

#ifdef VERBOSE
          wrap::system_message(NEWREQU
                               + tool::int2string(i_req) +  " "
                               + string(inet_ntoa( clientname.sin_addr )) + ":"
                               + tool::int2string(ntohs ( clientname.sin_port ))
                              );
#endif

          FD_SET(i_new_sock, &active_fd_set);

        }
        else
        {
          _socket *p_sock = _create_container(i);
          pool::run( (void*) p_sock );
          FD_CLR( i, &active_fd_set );
        }
      }
  }
}

void
sock::clean_ipcache()
{
  int i_ipcachesize = wrap::CONF->get_int("httpd.ipcachesize");
  int i_currentsize = ip_cache_map->size();

  if ( i_currentsize > 0 && (i_ipcachesize == 0 || i_ipcachesize <= i_currentsize) )
  {
    wrap::system_message(
      SOCKCA2+tool::int2string(i_currentsize)+","+tool::int2string(i_ipcachesize)+")");
  }
}
#endif

#endif
