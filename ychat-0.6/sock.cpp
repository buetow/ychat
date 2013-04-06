// class sock implementation. the multiplex socket implementation has been token from the
// GNU C Library Examples and modified in order to fit in here ( POSIX threads etc. ).

#ifndef s_sock_CXX
#define s_sock_CXX

#include <unistd.h>

#include "sock.h"
#include "s_chat.h"
#include "s_conf.h"
#include "s_tool.h"
#include "s_lang.h"
#include "s_sman.h"
#include "s_mman.h"
#include "s_mutx.h"
#include "s_ncur.h"
#include "chat.h"
#include "user.h"

using namespace std;

sock::sock()
{
    this->b_run      = true;
    this->i_req      = 0;
    this->i_threads  = 2; // currently there are 2 threads running ( admin interface + server listening );
    this->req_parser = new reqp();
    this->thrd_pool  = new pool();
    this->log_daemon = new logd(s_conf::get
                                    ().get_val( "ACCESS_LOG" ));


    pthread_mutex_init( &mut_threads, NULL );
}

sock::~sock()
{
    pthread_mutex_destroy( &mut_threads );
}

void
sock::chat_stream( int i_sock, user* p_user, map_string &map_params )
{
    string s_msg( "\n" );


    for ( int i = 0; i < PUSHSTR; i++ )
        send( i_sock, s_msg.c_str(), s_msg.size(), 0 );

    do
    {
        pthread_mutex_lock  ( &(p_user->mut_message) );
        s_msg = p_user->get_mess( );
        if ( 0 > send( i_sock, s_msg.c_str(), s_msg.size(), 0 ) )
            p_user->set_online( false );
        pthread_cond_wait( &(p_user->cond_message), &(p_user->mut_message) );
    }
    while( p_user->get_online() );

    // if there is still a message to send:
    s_msg = p_user->get_mess( );
    if ( ! s_msg.empty() )
        send( i_sock, s_msg.c_str(), s_msg.size(), 0 );

    pthread_mutex_unlock( &(p_user->mut_message) );

    // remove the user from its room.
    string s_user( p_user->get_name() );
    p_user->get_room()->del_elem( s_user );

#ifdef NCURSES

    {
        string s_tmp( REMUSER );
        s_tmp.append( s_user );
        s_ncur::get
            ().print( s_tmp.c_str() );
    }
#endif
    // post the room that the user has left the chat.
    p_user->get_room()->msg_post( new string( p_user->get_colored_name().append( s_lang::get
                                      ().get_val( "USERLEAV" ) ) ) );
    s_sman::get
        ().destroy_session( p_user->get_id() );

#ifdef NCURSES
    {
        string s_tmp( SESSION );
        s_tmp.append( s_tool::int2string( s_sman::get
                                              ().get_session_count() ) );
        s_ncur::get
            ().print( s_tmp );
    }
#endif
#ifdef VERBOSE
  //  cout << REMUSER << s_user << endl
  //       << SESSION << s_man::get().get_session_count() << endl;
        
#endif

    p_user->~user();
}

int
sock::make_socket( uint16_t i_port )
{
    int sock;
    struct sockaddr_in name;

    // create the server socket.
    sock = socket (PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {

#ifdef NCURSES
        s_ncur::get
            ().print( SOCKERR );
#endif

        if ( ++i_port > MAXPORT )
            exit(-1);

#ifdef NCURSES

        s_ncur::get
            ().print( SOCKERR );
#endif

        return make_socket( i_port );
    }

    // give the server socket a name.
    name.sin_family = AF_INET;
    name.sin_port = htons (i_port);
    name.sin_addr.s_addr = htonl (INADDR_ANY);
    int optval=1;

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(int));

    if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
    {

#ifdef NCURSES
        s_ncur::get
            ().print( BINDERR );
#endif

        if ( ++i_port > MAXPORT )
            exit(-1);

#ifdef SERVMSG

        cout << SOCKERR << i_port << endl;
#endif
#ifdef NCURSES

        s_ncur::get
            ().print( SOCKERR );
#endif

        return make_socket( i_port );
    }

#ifdef VERBOSE
    cout << SOCKCRT << "localhost:" << i_port << endl;
#endif
#ifdef NCURSES

    string s_tmp( SOCKCRT );
    s_tmp.append( "localhost:" );
    s_tmp.append( s_tool::int2string(i_port) );
    s_ncur::get
        ().print( s_tmp );
    mvprintw( NCUR_PORT_X,NCUR_PORT_Y, "Port: %d ", i_port);
    refresh();
#endif

    return sock;
}

int
sock::read_write( thrd* p_thrd, int i_sock )
{
    char c_req[2048];

    int i_bytes;
    i_bytes = read (i_sock, c_req, 2048);

    if (i_bytes < 0)
    {
#ifdef NCURSES
        s_ncur::get
            ().print( new string( READERR ) );
#endif

    }
    else
    {
        // stores the request params.
        map_string map_params;

        // get the s_rep ( s_html response which will be send imediatly to the client
        // and fill map_params with request values.
        auto string s_temp=(string)c_req;
        struct sockaddr_in client;
        size_t size=sizeof(client);

        getpeername( i_sock, (struct sockaddr *)&client, &size);

        map_params["REMOTE_ADDR"]=inet_ntoa(client.sin_addr);
        map_params["REMOTE_PORT"]=ntohs( client.sin_port);


        string s_rep = req_parser->parse( p_thrd, string( c_req ), map_params );
        // send s_rep to the client.
        log_daemon->log(map_params);

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
#ifdef NCURSES
    s_ncur::get
        ().print( STARTMS );
    print_hits();
    print_threads();
#endif

    auto int i_port = s_tool::string2int( s_conf::get
                                              ().get_val( "SRVRPORT" ) );

    int sock;
    fd_set active_fd_set, read_fd_set;
    int i;
    struct sockaddr_in clientname;
    size_t size;


    // create the server socket and set it up to accept connections.
    sock = make_socket ( i_port );

    if (listen (sock, 1) < 0)
    {
#ifdef NCURSES
        s_ncur::get
            ().print( LISTERR );
#endif

        exit( EXIT_FAILURE );
    }

#ifdef VERBOSE
    cout << SOCKRDY << endl;
#endif
#ifdef NCURSES

    s_ncur::get
        ().print( SOCKRDY );
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

#ifdef NCURSES
            s_ncur::get
                ().print( SELCERR );
#endif

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
                    new_sock = accept (sock,
                                       (struct sockaddr *) &clientname,
                                       &size);

                    if (new_sock < 0)
                    {

#ifdef NCURSES
                        s_ncur::get
                            ().print( ACCPERR);
#endif

                        close ( new_sock );
			
                    }
		
                    else	
		    {
#ifdef VERBOSE
                     cout << NEWREQU << i_req << " "
                     << inet_ntoa( clientname.sin_addr )
                     << ":"
                     << ntohs    ( clientname.sin_port )
                     << endl;
#endif
                     FD_SET (new_sock, &active_fd_set);
                    }
                }

                else
                {

#ifdef NCURSES
                    ///  s_ncur::get().print( NEWREQU );
#endif

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
    pthread_mutex_lock  ( &s_mutx::get().mut_stdout );
    mvprintw( NCUR_THREADS_X,NCUR_THREADS_Y, "Threads: %d ", i_threads);
    refresh();
    pthread_mutex_unlock( &s_mutx::get().mut_stdout );
}

void
sock::print_hits()
{
    pthread_mutex_lock  ( &s_mutx::get().mut_stdout );
    mvprintw( NCUR_HITS_X,NCUR_HITS_Y, "Hits: %d ", i_req);
    refresh();
    pthread_mutex_unlock( &s_mutx::get().mut_stdout );
}
#endif

#endif
