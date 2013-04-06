#ifndef SOCK_CPP
#define SOCK_CPP

#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include "sock.h"
#include "../chat/chat.h"
#include "../chat/user.h"

using namespace std;

sock::sock()
{
    this->b_run      = true;
    this->i_req      = 0;
    this->i_threads  = 0;
    this->req_parser = new reqp();
    this->thrd_pool  = new pool();
    this->log_daemon = new logd( wrap::CONF->get_elem( "httpd.logging.accessfile" ),
                                 wrap::CONF->get_elem( "httpd.logging.accesslines" ) );

    pthread_mutex_init( &mut_threads, NULL );
}

sock::~sock()
{
    pthread_mutex_destroy( &mut_threads );
}

//<<*
void
sock::chat_stream( int i_sock, user* p_user, map_string &map_params )
{
    string s_msg( "\n" );

    for ( int i = 0; i < PUSHSTR; i++ )
     send( i_sock, s_msg.c_str(), s_msg.size(), 0 );

    pthread_mutex_t mutex;
    pthread_mutex_init( &mutex, NULL ); 
    pthread_mutex_lock( &mutex );

    do
    {
        s_msg = p_user->get_mess( );
        if ( 0 > send( i_sock, s_msg.c_str(), s_msg.size(), 0 ) )
            p_user->set_online( false );
        pthread_cond_wait( &(p_user->cond_message), &mutex );
    }
    while( p_user->get_online() );

    pthread_mutex_destroy( &mutex );
 
    // if there is still a message to send:
    s_msg = p_user->get_mess( );
    if ( ! s_msg.empty() )
        send( i_sock, s_msg.c_str(), s_msg.size(), 0 );

    // remove the user from its room.
    string s_user( p_user->get_name() );
    string s_user_lowercase( p_user->get_lowercase_name() );
    p_user->get_room()->del_elem( s_user_lowercase );

    // post the room that the user has left the chat.
    s_msg = wrap::TIMR->get_time() + " " 
          + p_user->get_colored_bold_name()
          + wrap::CONF->get_elem( "chat.msgs.userleaveschat" )
          + "<br>\n";

    p_user->get_room()->msg_post( &s_msg );
    p_user->get_room()->reload_onlineframe();

#ifdef VERBOSE
    cout << REMUSER << s_user << endl;
#endif

    wrap::GCOL->add_user_to_garbage( p_user );
}
//*>>

int
sock::make_server_socket( int i_port )
{
    size_t sock;
    struct sockaddr_in name;

    // create the server socket.
    sock = socket (PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        wrap::system_message( SOCKERR );

        if ( ++i_port > MAXPORT )
            exit(-1);

        wrap::system_message( SOCKERR );

        return make_server_socket( i_port );
    }

    // give the server socket a name.
    name.sin_family = AF_INET;
    name.sin_port = htons (i_port);
    name.sin_addr.s_addr = htonl (INADDR_ANY);
    int optval=1;

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(int));

    if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
    {

     wrap::system_message( BINDERR );

        if ( ++i_port > MAXPORT )
            exit(-1);

        wrap::system_message( string(SOCKERR) + tool::int2string(i_port) );

        return make_server_socket( i_port );
    }

    wrap::system_message( SOCKCRT + string("localhost:") + tool::int2string(i_port) );

#ifdef NCURSES
    mvprintw( NCUR_PORT_X,NCUR_PORT_Y, "Port: %d ", i_port);
    refresh();
#endif

    return sock;
}

int
sock::read_write( thrd* p_thrd, int i_sock )
{
    char c_req[READSOCK];
    int i_bytes = read (i_sock, c_req, READSOCK);

    if (i_bytes <= 0)
    {
     wrap::system_message( READERR );
    }
    else
    {
        // stores the request params.
        map_string map_params;

        // get the s_rep ( s_html response which will be send imediatly to the client
        struct sockaddr_in client;
        size_t size = sizeof(client);

        getpeername( i_sock, (struct sockaddr *)&client, &size);

        map_params["REMOTE_ADDR"] = inet_ntoa(client.sin_addr);
        //map_params["REMOTE_PORT"] = ntohs(client.sin_port);

        string s_rep = req_parser->parse( p_thrd, string( c_req ), map_params );

        log_daemon->log_access(map_params);

        // send s_rep to the client.
        send( i_sock, s_rep.c_str(), s_rep.size(), 0 );

        // dont need those vals anymore.
        map_params.clear();

        return 0;
    }

    return -1;
}

int
sock::start()
{
    wrap::system_message( SOCKSRV );

#ifdef NCURSES
    print_hits();
    print_threads();
    thrd_pool->print_pool_size();
#endif

    auto int i_port = tool::string2int( wrap::CONF->get_elem( "httpd.serverport" ) );

    int sock;
    fd_set active_fd_set, read_fd_set;
    int i;
    struct sockaddr_in clientname;
    size_t size;


    // create the server socket and set it up to accept connections.
    sock = make_server_socket ( i_port );

    if (listen (sock, 1) < 0)
    {
      wrap::system_message( LISTERR );
      exit( EXIT_FAILURE );
    }

    wrap::system_message( SOCKRDY );

    // initialize the set of active sockets.
    FD_ZERO (&active_fd_set);
    FD_SET  (sock, &active_fd_set);

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
                if ( i == sock )
                {
                    // connection request on original socket.
                    i_req++;
#ifdef NCURSES
                    print_hits();
#endif
                    int new_sock;
                    size = sizeof (clientname);
                    new_sock = accept (sock, (struct sockaddr *) &clientname, &size);

                    if (new_sock < 0)
                    {
	             wrap::system_message( ACCPERR );
                     close ( new_sock );
                    }
		
                    else	
		    {
#ifdef VERBOSE
                     wrap::system_message(NEWREQU 
			+ tool::int2string(i_req) +  " "
                     	+ string(inet_ntoa( clientname.sin_addr )) + ":"
                     	+ tool::int2string(ntohs ( clientname.sin_port ))
			);
#endif
                     FD_SET (new_sock, &active_fd_set);
                    }
                }

                else
                {
                    thrd_pool->run( (void*) new thrd( i ) );
                    FD_CLR( i, &active_fd_set );
                }
            }
    }
}

void
sock::increase_num_threads()
{
    pthread_mutex_lock( &mut_threads );
    i_threads++;
    pthread_mutex_unlock( &mut_threads );

#ifdef NCURSES
    print_threads();
#endif
}

void
sock::decrease_num_threads()
{
    pthread_mutex_lock( &mut_threads );
    i_threads--;
    pthread_mutex_unlock( &mut_threads );

#ifdef NCURSES
    print_threads();
#endif
}

#ifdef NCURSES
void
sock::print_threads()
{
    if ( wrap::NCUR->is_ready() )
    {
     mvprintw( NCUR_POOL_RUNNING_X,NCUR_POOL_RUNNING_Y, "In use: %d ", i_threads);
     refresh();
   }
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
