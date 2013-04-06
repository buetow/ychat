// class sock implementation. the multiplex socket implementation has been token from the
// GNU C Library Examples and modified in order to fit in here ( POSIX threads etc. ).

#ifndef SOCK_CXX
#define SOCK_CXX

#include <unistd.h>

#include "sock.h"
#include "CHAT.h"
#include "CONF.h"
#include "MUTX.h"
#include "TOOL.h"

#include "chat.h"
#include "user.h"

using namespace std;


// the posix thread function. this one will called every time a new request socket
// was created succsessfull. in this function a new POSIX thread life begins.
// be carefull because of synchronization issues!!!!
void *sock::posix_thread_func( void *v_pointer )
{
#ifdef VERBOSE_
  pthread_mutex_lock  ( &MUTX::get().mut_stdout );
  cout << "*posix_thread_func( void *v_pointer )" << endl;
  pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

  // recasting the client thread object.
  thrd *t = (thrd*) v_pointer;

  // start parsing the client request and sending response's back.
  t-> run ();
 
  t->~thrd();
}

void *sock::posix_thread_func_( void *v_pointer )
{
#ifdef VERBOSE_
  pthread_mutex_lock  ( &MUTX::get().mut_stdout );
  cout << "*posix_thread_func_( void *v_pointer )" << endl;
  pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 user *p_user = (user*) v_pointer;

 string s_msg( "" );

 int i_sock = p_user->get_sock();
 pthread_mutex_lock  ( &(p_user->mut_message) );

 while( p_user->get_online() )
 {
  pthread_cond_wait( &(p_user->cond_message), &(p_user->mut_message) );
  s_msg = p_user->get_mess( );
  if ( 0 > send( i_sock, s_msg.c_str(), s_msg.size(), 0 ) )
   p_user->set_online( false );
 }
 pthread_mutex_unlock( &(p_user->mut_message) );

 // remove the user from its room.
 string s_user( p_user->get_name() );
 p_user->get_p_room()->del_elem( s_user );

 // post the room that the user has left the chat.
 p_user->get_p_room()->msg_post( new string( p_user->get_name().append( USERLEAV ) ) );  

 p_user->~user();
}

sock::sock()
{
#ifdef VERBOSE
 cout << "sock::sock()" << endl;
#endif

 this->b_run      = true;
 this->i_req      = 0;
 this->req_parser = new reqp();
}

void
#ifdef THRDMOD
sock::chat_stream( int i_sock, map_string &map_params )
#else
sock::chat_stream( int i_sock, map_string &map_params, queue<pthread_t> &thrd_pool )
#endif
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "sock::chat_stream( " << i_sock << ", map_string& )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 user* p_user = CHAT::get().get_user( map_params["nick"] );
 p_user->set_sock( i_sock );

#ifdef THRDMOD
 posix_thread_func_( (void*) p_user );
#else

#ifdef _VERBOSE
 cout << THREAD2 << endl; 
#endif
 posix_thread_func_( (void*) p_user );

 auto int i_fail = pthread_create( &thrd_pool.front(), NULL, posix_thread_func_, (void*) p_user ); 

 // remove this thread from thread pool because its now in use.
 thrd_pool.pop();

 // check if the thread started correctly.
 if ( i_fail )
 {
  cerr << "Thrd: error with return code " << i_fail << endl;
 }	

#endif
}

int
sock::make_socket( uint16_t i_port )
{
#ifdef VERBOSE_
 cout << "sock::make_socket( " << i_port << " )" << endl;
#endif

 int sock;
 struct sockaddr_in name;

 // create the server socket.
 sock = socket (PF_INET, SOCK_STREAM, 0);
 if (sock < 0)
 {
  cerr << "Sock: socket error" << endl;

  if ( ++i_port > MAXPORT )
   exit(-1);

  cerr << SOCKERR << i_port << endl;
  return make_socket( i_port );
 }

 // give the server socket a name.
 name.sin_family = AF_INET;
 name.sin_port = htons (i_port);
 name.sin_addr.s_addr = htonl (INADDR_ANY);

 if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
 {
  cerr << "Sock: bind error" << endl;

  if ( ++i_port > MAXPORT )
   exit(-1);

  cout << SOCKERR << i_port << endl;
  return make_socket( i_port );
 }

 return sock;
}

int
sock::read_write( int i_sock )
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "sock::read_write( " << i_sock << " )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 char c_req[2048];
 int i_bytes;
 i_bytes = read (i_sock, c_req, 2048);
 
 if (i_bytes < 0)
 {
  cerr << "Sock: read error " << endl;
 }

 else
 {
  // stores the request params.
  map_string map_params; 

  // get the s_rep ( HTML response which will be send imediatly to the client
  // and fill map_params with request values. 
  string s_rep = req_parser->parse( string( c_req ), map_params );

  // send s_rep to the client.
  send( i_sock, s_rep.c_str(), s_rep.size(), 0 );

  // prove if this is a request for a chat stream!
  if ( map_params["event"] == "stream" )
#ifdef THRDMOD
   chat_stream( i_sock, map_params );
#else
   chat_stream( i_sock, map_params, thrd_pool );
#endif

  return 0;
 }
 
 return -1;
}

void
sock::refill_thrd_pool( )
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "sock::refill_thrd_pool( )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif
#ifdef _VERBOSE
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << THRPOOL << i_thrd_pool_size << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 for (int i=0; i<i_thrd_pool_size; i++ ) 
 {
  pthread_t new_thread;
  thrd_pool.push( new_thread );
 }
}

int
sock::start()
{
#ifdef VERBOSE_
 cout << "sock::start( )" << endl;
#endif

 i_thrd_pool_size = TOOL::string2int( CONF::get().get_val( "THRDPOOL" ) );
 auto int i_port  = TOOL::string2int( CONF::get().get_val( "SRVRPORT" ) );

 int sock;
 fd_set active_fd_set, read_fd_set;
 int i;
 struct sockaddr_in clientname;
 size_t size;

#ifdef _VERBOSE
 cout << SOCKCRT << "localhost:" << i_port << endl;
#endif

 // create the server socket and set it up to accept connections.
 sock = make_socket ( i_port );

 if (listen (sock, 1) < 0)
 {
  cerr << "Sock: listen error" << endl;
  exit( EXIT_FAILURE );
 }

#ifdef _VERBOSE
 cout << SOCKRDY << endl;
#endif

 // initialize the set of active sockets. 
 FD_ZERO (&active_fd_set);
 FD_SET  (sock, &active_fd_set);

 while( b_run )
 {
  // block until input arrives on one or more active sockets.
  read_fd_set = active_fd_set;
  if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
  {
   cerr << "Sock: select error" << endl;
   exit( EXIT_FAILURE );
  }

  // service all the sockets with input pending. 
  for ( i = 0; i < FD_SETSIZE; i++ )
   if ( FD_ISSET (i, &read_fd_set) )
   {
    if ( i == sock )
    {
     // connection request on original socket.
     i_req++;
     int new_sock;
     size = sizeof (clientname);
     new_sock = accept (sock,
                        (struct sockaddr *) &clientname,
                        &size);

     if (new_sock < 0)
     {
      cerr << "Sock: accept error" << endl;
      close ( new_sock );
     }

#ifdef _VERBOSE
     cout << CONNECT << i_req << " "  
          << inet_ntoa( clientname.sin_addr )
          << ":"
          << ntohs    ( clientname.sin_port )
          << endl;
#endif
 
      FD_SET (new_sock, &active_fd_set);
    }

    else
    {
      // create a client thread object. this one will contain all data which is needed by a new posix
      // thread in order to do its tasks. the thr_client pointer will be passed to the posix function
      // there the life of a new thread begins.
      thrd* thr_client = new thrd( i );

      // if the thread pool is empty refill it.
      if ( thrd_pool.empty() )
       refill_thrd_pool( );

#ifndef THRDMOD 
      posix_thread_func( (void*) thr_client ); 

#else
#ifdef _VERBOSE
      cout << THREAD1 << endl; 
#endif

      // creating a new posix thread.
      auto int i_fail = pthread_create( &thrd_pool.front(), NULL, posix_thread_func, (void*) thr_client ); 

      // remove this thread from thread pool because its now in use.
      thrd_pool.pop();
      // check if the thread started correctly.
      if ( i_fail )
      {
        cerr << "Thrd: error with return code " << i_fail << endl;
        close ( i );
      }	

#endif
      FD_CLR( i, &active_fd_set );
    }
   }
 }
}

#endif
