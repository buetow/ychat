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
#include "logd.h"
using namespace std;

class sock
{
private:
    // total number of server requests.
    unsigned long long int i_req;

    bool  b_run;      // true while socket manager is running.
    reqp* req_parser; // parses the http requests from clients.
    pool* thrd_pool;  // the thread pool.
    logd* log_daemon; // the log daemon
    char* c_buffer;   // char buffer!

    int   i_threads;     // total amount of threads inside the thread pool.
    pthread_mutex_t mut_threads;
    pthread_mutex_t mut_hits;

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
    ~sock( );
    virtual int  read_write( thrd* p_thrd, int filedes   );
    virtual int  start();

    // the chat stream there all the chat messages will sent through.
    static void chat_stream( int i_sock, user* p_user, map_string &map_params );

    void increase_num_threads();
    void decrease_num_threads();
#ifdef NCURSES

    void print_threads();
    void print_hits();
#endif

};

#endif
