#include "../incl.h"

#ifndef SMAN_H
#define SMAN_H

#include <cstdlib>

#include "sess.h"
#include "../maps/smap.h"

using namespace std;

class sman : private smap<sess*,string>
{
private:
    string generate_id( int len );
    int i_session_count;

    pthread_mutex_t mut_i_session_count;
#ifdef NCURSES
    void print_sessions();
#endif

public:
    sman();
    ~sman();
    sess *get_session( string s_id );
    int get_session_count( );
    sess *create_session( );
    void destroy_session( string s_id );
#ifdef NCURSES
    void print_init_ncurses();
#endif
};


#endif

