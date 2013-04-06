#ifndef SMAN_CXX
#define SMAN_CXX

#include "sman.h"

sman::sman()
{
    this->sessions=new hmap<sess *, string>(80);
    this->session_count=0;
    pthread_mutex_init( &mut_sessions, NULL );
    pthread_mutex_init( &mut_session_count, NULL );
}
sman::~sman()
{
    pthread_mutex_lock( &mut_sessions );
    delete this->sessions;
    pthread_mutex_unlock( &mut_sessions );

    pthread_mutex_destroy( &mut_sessions );
    pthread_mutex_destroy( &mut_session_count );
}
string sman::generate_id( int len )
{
    string valid_chars=".abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_";
    string s_ret="";
    srand(time(0)+160682);
    for(int i=0;i<len;i++)
    {
        int i_char=rand() % 64;
        s_ret+=valid_chars[i_char];
    }
    return s_ret;
}
sess *sman::create_session( )
{
    string new_id=this->generate_id( s_tool::string2int( s_conf::get
                                         ().get_val( "SESSION_LENGTH" ) ) );

    sess *new_sess= new sess( new_id  );

    pthread_mutex_lock( &mut_session_count );
    this->session_count++;
#ifdef NCURSES
    print_sessions();
#endif
    pthread_mutex_unlock( &mut_session_count );

    pthread_mutex_lock( &mut_sessions );
    this->sessions->add_elem( new_sess, new_id );
    pthread_mutex_unlock( &mut_sessions );


    return new_sess;
}

sess *sman::get_session( string s_id )
{
    pthread_mutex_lock( &mut_sessions );
    sess *ret_sess = this->sessions->get_elem( s_id );
    pthread_mutex_unlock( &mut_sessions );

    return ret_sess;
}
void sman::destroy_session( string s_id )
{
    pthread_mutex_lock( &mut_session_count );
    this->session_count--;
#ifdef NCURSES
    print_sessions();
#endif
    pthread_mutex_unlock( &mut_session_count );

    this->sessions->del_elem( s_id );
}

#ifdef NCURSES
void sman::print_sessions()
{
    pthread_mutex_lock  ( &s_mutx::get().mut_stdout );
    mvprintw( NCUR_SESSION_X,NCUR_SESSION_Y, "Sessions: %d ", session_count);
    refresh();
    pthread_mutex_unlock( &s_mutx::get().mut_stdout );
}

void sman::print_init_ncurses()
{
    pthread_mutex_lock( &mut_session_count );
    print_sessions();
    pthread_mutex_unlock( &mut_session_count );
}

#endif
#endif
