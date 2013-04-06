// class user implementation.

#ifndef USER_CXX
#define USER_CXX

#include "user.h"
#include "CONF.h"
#include "TOOL.h"

using namespace std;

user::user( string s_name ) : name( s_name )
{
 this -> b_online = true; 
 this -> l_time   = TOOL::unixtime();
 this -> s_col1   = CONF::get().get_val( "USERCOL1" );

 pthread_mutex_init( &mut_b_online, NULL);
 pthread_mutex_init( &mut_i_sock  , NULL);
 pthread_mutex_init( &mut_l_time  , NULL);
 pthread_mutex_init( &mut_p_room  , NULL);
 pthread_mutex_init( &mut_s_mess  , NULL);
 pthread_cond_init ( &cond_message, NULL);
 pthread_mutex_init( &mut_message , NULL);
}

user::~user()
{
 pthread_mutex_destroy( &mut_b_online );
 pthread_mutex_destroy( &mut_i_sock   );
 pthread_mutex_destroy( &mut_l_time   );
 pthread_mutex_destroy( &mut_p_room   );
 pthread_mutex_destroy( &mut_s_mess   );
 pthread_cond_destroy ( &cond_message );
 pthread_mutex_destroy( &mut_message  );
}

void
user::get_data( map_string *p_map_data )
{
 string s_req = (*p_map_data)["!get"];

 // get the nick and the color of the user.
 if ( s_req == "nick" )
  (*p_map_data)[get_name()] = get_col1();
}

string
user::get_mess( )
{
 string s_ret( "" );
 pthread_mutex_lock  ( &mut_s_mess );
 s_ret.append( s_mess );
 s_mess = *new string("");
 pthread_mutex_unlock( &mut_s_mess );

 return s_ret;
}

bool
user::get_online( )
{
 bool b_ret; 
 pthread_mutex_lock  ( &mut_b_online );
 b_ret = b_online; 
 pthread_mutex_unlock( &mut_b_online );
 return b_ret;
}

void
user::set_online( bool b_online )
{
 pthread_mutex_lock  ( &mut_b_online );
 this -> b_online = b_online; 
 pthread_mutex_unlock( &mut_b_online );
}

room*
user::get_p_room( )
{
 room* p_return;
 pthread_mutex_lock  ( &mut_p_room );
 p_return = p_room;
 pthread_mutex_unlock( &mut_p_room );
 return p_return;
}

void
user::set_p_room( room* p_room )
{
 pthread_mutex_lock  ( &mut_p_room );
 this -> p_room = p_room;
 pthread_mutex_unlock( &mut_p_room );
}

int
user::get_sock( )
{
 int i_ret;
 pthread_mutex_lock  ( &mut_i_sock );
 i_ret = i_sock;
 pthread_mutex_unlock( &mut_i_sock );
 return i_ret;
}

void
user::set_sock( int i_sock )
{
 pthread_mutex_lock  ( &mut_i_sock );
 this -> i_sock = i_sock;
 pthread_mutex_unlock( &mut_i_sock );
}

void
user::renew_stamp( )
{
 pthread_mutex_lock  ( &mut_l_time );
 l_time = TOOL::unixtime();
 pthread_mutex_unlock( &mut_l_time );
}

void
user::msg_post( string *p_msg )
{
 pthread_mutex_lock  ( &mut_s_mess );
 s_mess.append( *p_msg );
 pthread_mutex_unlock( &mut_s_mess );

 pthread_cond_signal( &cond_message );
}

void
user::get_user_list( string &s_list, string &s_seperator )
{
 s_list.append( "<font color=\"" )
       .append( get_col1()       )
       .append( "\">"            )
       .append( get_name()       )
       .append( "</font>\n"      )
       .append( s_seperator      );
   
}
#endif
