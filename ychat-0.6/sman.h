#ifndef SMAN_H
#define SMAN_H

#include "incl.h"
#include "hmap.h"
#include "sess.h"
#include "s_mutx.h"
#include "s_ncur.h"
#include "s_tool.h"
#include "s_conf.h"
#include <cstdlib>

using namespace std;

class sman
{
private:
    hmap<sess *, string> *sessions;
    string generate_id( int len );
    int session_count;

    pthread_mutex_t mut_sessions;
    pthread_mutex_t mut_session_count;
#ifdef NCURSES
    void print_sessions();
#endif

public:
    sman();
    ~sman();
    sess *get_session( string s_id );
    int get_session_count( )
    {
        return this->session_count;
    }
    sess *create_session( );
    void destroy_session( string s_id );
#ifdef NCURSES
    void print_init_ncurses();
#endif
};


#endif

