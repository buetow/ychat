#ifndef SMAN_CPP
#define SMAN_CPP

#include "sman.h"
#include "../tool/tool.h"
#include "../maps/mtools.h"
#include "../contrib/crypt/md5.h"

sman::sman() 
{
    i_session_count = 0;
    pthread_mutex_init( &mut_i_session_count, NULL );
}

sman::~sman()
{
    // Delete each session object of the shashmap!
    shashmap<sess*>::run_func( mtools<sess*>::delete_obj );	
    pthread_mutex_destroy( &mut_i_session_count );
}

string sman::generate_id( int i_len )
{
    string valid_chars = wrap::CONF->get_elem("chat.session.validchars");
    string s_ret = "";

    srand(time(0)+tool::string2int(wrap::CONF->get_elem("chat.session.kloakkey")));
    int i_char;

    for (int i = 0; i < i_len; i++)
    {
     i_char = rand() % 64;
     s_ret += valid_chars[i_char];
    }

    if ( wrap::CONF->get_elem("chat.session.md5hash") == "true" )
    {
      string s_salt = wrap::CONF->get_elem("chat.session.md5salt");
      s_ret = string(md5::MD5Crypt(s_ret.c_str(), s_salt.c_str())); 
      return s_ret.substr(s_ret.find(s_salt) + s_salt.length() + 3);
    }

    return s_ret;
}

sess *sman::create_session( )
{
    string s_tmpid = generate_id( tool::string2int( wrap::CONF->get_elem( "chat.session.length" ) ) );

    sess* p_sess = new sess( s_tmpid );

    pthread_mutex_lock( &mut_i_session_count );
    i_session_count++;
#ifdef NCURSES
    print_sessions();
#endif
    pthread_mutex_unlock( &mut_i_session_count );

    //????
    add_elem( p_sess, s_tmpid );

    return p_sess;
}

sess *sman::get_session( string s_id )
{
    return get_elem( s_id );
}

void
sman::destroy_session( string s_id )
{
    pthread_mutex_lock( &mut_i_session_count );
    i_session_count--;
#ifdef NCURSES
    print_sessions();
#endif
    pthread_mutex_unlock( &mut_i_session_count );

    sess* p_sess = get_elem( s_id );
    del_elem( s_id );
    delete p_sess;
}

#ifdef NCURSES
void sman::print_sessions()
{
    if ( wrap::NCUR->is_ready() )
    {
     mvprintw( NCUR_SESSION_X,NCUR_SESSION_Y, "Sessions: %d ", i_session_count);
     refresh();
    }
}

#ifdef NCURSES
void sman::print_init_ncurses()
{
    pthread_mutex_lock( &mut_i_session_count );
    print_sessions();
    pthread_mutex_unlock( &mut_i_session_count );
}
#endif

int
sman::get_session_count()
{
    int i_ret;
    pthread_mutex_lock( &mut_i_session_count );
    i_ret = i_session_count; 
    pthread_mutex_unlock( &mut_i_session_count );
    return i_ret;
}

#endif
#endif
