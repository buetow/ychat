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

int
sock::_send(socketcontainer *p_sock, const char *sz, int len)
{

  return send( p_sock->i_sock, sz, len, 0 );
}

int
sock::_read(socketcontainer *p_sock,  char *sz, int len)
{

  return read( p_sock->i_sock, sz, len );
}

int
sock::_close(socketcontainer *p_sock)
{
  shutdown( p_sock->i_sock, 2 );
  close   ( p_sock->i_sock    );
  delete p_sock;
}


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
sock::read_http_line(socketcontainer *p_sock)
{
  string s_line;
  int i_total = 0;
  int i_read = 0;
  char ch;

  do
  {
    i_read = _read(p_sock, &ch, sizeof(ch));

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
sock::read_http(socketcontainer *p_sock, char *c_zbuf, int i_buflen, int &i_postpayloadoffset)
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
  if(s_line.length() <= 0)
    return -1;

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
  if(s_line.substr(0,3) == "GET")
  {
    if(s_line.length() > i_buflen)
    {
      /*
      **  Buffer overflow
      */
      return -1;
    }
    else
    {
      memcpy(c_zbuf,s_line.c_str(),s_line.length());
      return s_line.length();
    }
  }

  else
  {
    /*
    **  POST yada
    **  01234
    */
    if(s_line.substr(0,4) != "POST")
      return -1;

    /*
    **  Get us to the Content-Length:
    */
    s_post_return += s_line;
    i_postpayloadoffset += s_line.length();

    for(x=0 ;x < MAXLINES; x++)
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

        z++;
        
      }
      while(ch != '\n');

      break;
    }

    if(s_cl.length() <= 0)
      return -1;

    z = atoi(s_cl.c_str());

    /*
    **  If we are going to overflow the buffer just by the payload, leave
    **  of, if z did not convert correctly. (should have been ok by isdigit)
    */
    if(z > i_buflen || z < 0)
      return -1;

    /*
    **  We have MAXLINES to get to the blank line separating POST data.
    */
    for(x=0 ;x < MAXLINES; x++)
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
    if(x == MAXLINES)
      return -1;

    for(x=0; x < z; x++)
    {
      if(_read(p_sock,&ch,sizeof(ch)) != 1)
        return -1;

      s_post_return += ch;
    }

    if(s_post_return.length() > i_buflen)
      return -1;

    memcpy(c_zbuf,s_post_return.c_str(),s_post_return.length());
    return s_post_return.length();
  }
}

int
sock::read_write(socketcontainer* p_sock)
{
  int i_postpayloadoffset;
  int i_sock = p_sock->i_sock;

  char c_req[READSOCK];

  memset(c_req,0,sizeof(c_req));

  int i_bytes = read_http(p_sock, c_req, READSOCK-1,i_postpayloadoffset);

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

    uint32_t &s_addr = client.sin_addr.s_addr;
    if ( (map_params["REMOTE_ADDR"] = get_elem(s_addr)) == "" )
    {
      map_params["REMOTE_ADDR"] = string(inet_ntoa(client.sin_addr));
      set_elem(map_params["REMOTE_ADDR"], s_addr);
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

socketcontainer*
sock::_create_container(int &i_sock)
{
  socketcontainer* p_sock = new socketcontainer;
  p_sock->i_sock = i_sock;
  return p_sock;
}

int
sock::start()
{
  wrap::system_message( SOCKSRV );
  pool* p_pool = wrap::POOL;
  int i_sock = i_server_sock;

#ifdef NCURSES

  print_hits();
  p_pool->print_pool_size();
#endif

  int i_port = tool::string2int( wrap::CONF->get_elem( "httpd.serverport" ) );
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

  print_server_port();

  while( b_run )
  {
    // block until input arrives on one or more active sockets.
    read_fd_set = active_fd_set;
    if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
    {
      wrap::system_message( SELCERR );

      exit( EXIT_FAILURE );
    }

    // service all the sockets with input pending.
    for ( i = 0; i < FD_SETSIZE; i++ )
      if ( FD_ISSET (i, &read_fd_set) )
      {
        if ( i == i_sock )
        {
          // connection request on original socket.
          ++i_req;

#ifdef NCURSES

          print_hits();
#endif

          int i_new_sock;
          size = sizeof(clientname);
          i_new_sock = accept (i_sock, (struct sockaddr *) &clientname, &size);

#ifdef VERBOSE

          wrap::system_message(NEWREQU
                               + tool::int2string(i_req) +  " "
                               + string(inet_ntoa( clientname.sin_addr )) + ":"
                               + tool::int2string(ntohs ( clientname.sin_port ))
                              );
#endif

          FD_SET (i_new_sock, &active_fd_set);

        }
        else
        {
          socketcontainer *p_sock = _create_container(i);
          p_pool->run( (void*) p_sock );
          FD_CLR( i, &active_fd_set );
        }
      }
  }
}

void
sock::clean_ipcache()
{
  int i_ipcachesize = wrap::CONF->get_int("httpd.ipcachesize");
  int i_currentsize = size();

  if ( i_currentsize > 0 && (i_ipcachesize == 0 || i_ipcachesize <= i_currentsize) )
  {
    wrap::system_message(
      SOCKCA2+tool::int2string(i_currentsize)+","+tool::int2string(i_ipcachesize)+")");
    clear();
  }
}

#ifdef NCURSES
void
sock::print_server_port() {
  mvprintw( NCUR_PORT_X,NCUR_PORT_Y, "Port: %d ", i_server_port);
  refresh();
}

void
sock::print_hits()
{
  if ( wrap::NCUR->is_ready() )
  {
    mvprintw( NCUR_HITS_X,NCUR_HITS_Y, "Hits: %d ", i_req);
    refresh();
  }
}
#endif

#endif
