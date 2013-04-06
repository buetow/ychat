#ifndef USER_CPP
#define USER_CPP

#include "user.h"
#include "../tool/tool.h"

using namespace std;

user::user() : name( "" )
{
 initialize();
}

user::user( string s_name ) : name( s_name )
{
 initialize();
}

user::~user()
{
//    pthread_mutex_destroy( & mut_map_mods );
    pthread_mutex_destroy( & mut_away );
    pthread_mutex_destroy( & mut_b_online );
    pthread_mutex_destroy( & mut_b_fake );
    pthread_mutex_destroy( & mut_b_invisible );
    pthread_mutex_destroy( & mut_b_has_sess );
    pthread_mutex_destroy( & mut_b_is_reg );
    pthread_mutex_destroy( & mut_s_mess );
    pthread_mutex_destroy( & mut_s_pass );
    pthread_mutex_destroy( & mut_p_room );
    pthread_mutex_destroy( & mut_s_col1 );
    pthread_mutex_destroy( & mut_s_col2 );
    pthread_mutex_destroy( & mut_s_email );
    pthread_mutex_destroy( & mut_s_tmpid );
    pthread_mutex_destroy( & mut_i_status );
    pthread_mutex_destroy( & mut_map_changed_data );
    pthread_cond_destroy ( &cond_message );
    pthread_mutex_destroy( &mut_message  );
}

void
user::initialize()
{
    this -> b_is_reg = false;
    this -> b_set_changed_data = false;
    this -> b_away = false;
    this -> b_fake = false;
    this -> b_invisible = false;
    this -> s_col1 = wrap::CONF->get_elem( "chat.html.user.color1" );
    this -> s_col2 = wrap::CONF->get_elem( "chat.html.user.color2" );
    //map_mods = new smap<dynmod*,string>(HMAPOCC);

   // pthread_mutex_init( & mut_map_mods , NULL );
    pthread_mutex_init( & mut_away , NULL );
    pthread_mutex_init( & mut_b_online , NULL );
    pthread_mutex_init( & mut_b_fake , NULL );
    pthread_mutex_init( & mut_b_invisible , NULL );
    pthread_mutex_init( & mut_b_has_sess , NULL );
    pthread_mutex_init( & mut_b_is_reg , NULL );
    pthread_mutex_init( & mut_s_mess , NULL );
    pthread_mutex_init( & mut_s_pass , NULL );
    pthread_mutex_init( & mut_p_room , NULL );
    pthread_mutex_init( & mut_s_col1 , NULL );
    pthread_mutex_init( & mut_s_col2 , NULL );
    pthread_mutex_init( & mut_s_email , NULL );
    pthread_mutex_init( & mut_s_tmpid , NULL );
    pthread_mutex_init( & mut_i_status , NULL );
    pthread_mutex_init( & mut_map_changed_data , NULL );
    pthread_cond_init ( &cond_message, NULL);
    pthread_mutex_init( &mut_message , NULL);
    renew_timeout();
}

void
user::clean()
{
   // If this user has a session	
   if ( get_has_sess() )
   {
#ifdef DATABASE
    // Store all changed data into the mysql table if this user is registered:
    if ( b_is_reg )
     wrap::DATA->update_user_data( get_name(), "savechangednick", map_changed_data );    
    wrap::SMAN->destroy_session( get_tmpid() );
#endif
     // wrap::system_message( SESSION + tool::int2string( wrap::SMAN->get_session_count() ) );
   }

   set_fake( false );
   set_invisible( false );
}

string
user::get_colored_name()
{
 return "<font color=\"#" + get_col1() + "\">" + get_name() + "</font>";
}

void
user::get_data( map<string,string> *p_map_data )
{
    string s_req = (*p_map_data)["!get"];

    // get the nick and the color of the user.
    if ( s_req == "nick" )
        (*p_map_data)[get_name()] = get_col1();
}

string
user::get_mess( )
{
    pthread_mutex_lock  ( &mut_s_mess );
    string s_ret = s_mess;
    s_mess = "";
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
bool
user::get_fake( )
{
    bool b_ret;
    pthread_mutex_lock  ( &mut_b_fake );
    b_ret = b_fake;
    pthread_mutex_unlock( &mut_b_fake );
    return b_ret;
}
bool
user::get_invisible( )
{
    bool b_ret;
    pthread_mutex_lock  ( &mut_b_invisible );
    b_ret = b_invisible;
    pthread_mutex_unlock( &mut_b_invisible );
    return b_ret;
}

bool
user::get_has_sess( )
{
    bool b_ret;
    pthread_mutex_lock  ( &mut_b_has_sess );
    b_ret = b_has_sess;
    pthread_mutex_unlock( &mut_b_has_sess );
    return b_ret;
}

void
user::set_has_sess( bool b_has_sess )
{
    pthread_mutex_lock  ( &mut_b_has_sess );
    this->b_has_sess = b_has_sess;
    pthread_mutex_unlock( &mut_b_has_sess );
}

bool
user::get_is_reg( )
{
    bool b_ret;
    b_ret = b_is_reg;
    return b_ret;
}

void
user::set_is_reg( bool b_is_reg )
{
    this -> b_is_reg = b_is_reg;
}

void
user::set_online( bool b_online )
{
    pthread_mutex_lock  ( &mut_b_online );
    this -> b_online = b_online;
    pthread_mutex_unlock( &mut_b_online );
}

void
user::set_fake( bool b_fake )
{
    pthread_mutex_lock  ( &mut_b_fake );
    this -> b_fake = b_fake;
    pthread_mutex_unlock( &mut_b_fake );
}

void
user::set_invisible( bool b_invisible )
{
    pthread_mutex_lock  ( &mut_b_invisible );
    this -> b_invisible = b_invisible;
    pthread_mutex_unlock( &mut_b_invisible );
}

bool
user::get_away()
{
    bool b_ret;
    pthread_mutex_lock  ( &mut_away );
    b_ret = b_away;
    pthread_mutex_unlock( &mut_away );
    return b_ret;
}

string
user::get_away_msg()
{
    string s_ret;
    pthread_mutex_lock  ( &mut_away );
    s_ret = s_away;
    pthread_mutex_unlock( &mut_away );
    return s_ret;
}

void
user::set_away( bool b_away, string s_away )
{
    pthread_mutex_lock  ( &mut_away );
    this -> b_away = b_away;
    this -> s_away = s_away;
    pthread_mutex_unlock( &mut_away );
}

void
user::set_away( bool b_away )
{
    pthread_mutex_lock  ( &mut_away );
    this -> b_away = b_away;
    pthread_mutex_unlock( &mut_away );
}

room*
user::get_room( )
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

string
user::get_pass()
{
    string s_ret;
    pthread_mutex_lock  ( &mut_s_pass );
    s_ret = s_pass;
    pthread_mutex_unlock( &mut_s_pass );
    return s_ret;
}

string
user::get_col1()
{
    string s_ret;
    pthread_mutex_lock  ( &mut_s_col1 );
    s_ret = s_col1;
    pthread_mutex_unlock( &mut_s_col1 );
    return s_ret;
}

string
user::get_col2()
{
    string s_ret;
    pthread_mutex_lock  ( &mut_s_col2 );
    s_ret = s_col2;
    pthread_mutex_unlock( &mut_s_col2 );
    return s_ret;
}

string
user::get_email()
{
    string s_ret;
    pthread_mutex_lock  ( &mut_s_email );
    s_ret = s_email;
    pthread_mutex_unlock( &mut_s_email );
    return s_ret;
}

string
user::get_tmpid()
{
    string s_ret;
    pthread_mutex_lock  ( &mut_s_tmpid );
    s_ret = s_tmpid;
    pthread_mutex_unlock( &mut_s_tmpid );
    return s_ret;
}

void
user::set_tmpid    ( string s_tmpid   )
{
    pthread_mutex_lock  ( &mut_s_tmpid );
    this -> s_tmpid = s_tmpid;
    pthread_mutex_unlock( &mut_s_tmpid );
}

void
user::set_pass  ( string s_pass )
{
    set_changed_data( "password", s_pass );
    pthread_mutex_lock  ( &mut_s_pass );
    this -> s_pass = s_pass;
    pthread_mutex_unlock( &mut_s_pass );
}

void
user::set_col1  ( string s_col1 )
{
    set_changed_data( "color1", s_col1 );
    pthread_mutex_lock  ( &mut_s_col1 );
    this -> s_col1 = s_col1;
    pthread_mutex_unlock( &mut_s_col1 );
}

void
user::set_col2  ( string s_col2 )
{
    set_changed_data( "color2", s_col2 );
    pthread_mutex_lock  ( &mut_s_col2 );
    this -> s_col2 = s_col2;
    pthread_mutex_unlock( &mut_s_col2 );
}

void
user::set_email ( string s_email )
{
    set_changed_data( "email", s_email );
    pthread_mutex_lock  ( &mut_s_email );
    this -> s_email = s_email;
    pthread_mutex_unlock( &mut_s_email );
}

int
user::get_status  ( )
{
    int r_ret;
    pthread_mutex_lock  ( &mut_i_status );
    r_ret = i_status;
    pthread_mutex_unlock(&mut_i_status );
    return  r_ret;
}

void
user::set_status  ( int   i_status )
{
    set_changed_data( "status", tool::int2string(i_status));
    pthread_mutex_lock  ( &mut_i_status );
	    i_old_status = this -> i_status;
    this -> i_status = i_status;
    pthread_mutex_unlock( &mut_i_status );
}

void
user::set_changed_data( string s_varname, string s_value )
{
    if ( b_set_changed_data )
    { 
     pthread_mutex_lock  ( &mut_map_changed_data );
     map_changed_data[s_varname] = s_value; 
     pthread_mutex_unlock( &mut_map_changed_data );
    }
}

bool
user::new_msgs  ( )
{
    pthread_mutex_lock  ( &mut_s_mess );
    return s_mess.empty();
    pthread_mutex_unlock( &mut_s_mess );
}

void
user::command( string &s_command )
{
    check_restore_away();

    unsigned pos = s_command.find( "/" );
    unsigned pos2 = s_command.find( " " );
    if( pos != string::npos )
    {
        s_command.replace( pos, 1, "" );
    }
    else
        return;

    if (pos2 == string::npos)
        pos2 = s_command.size() + 1;

    string s_mod( wrap::CONF->get_elem("httpd.modules.commandsdir") + "yc_" );
    string s_command2 = s_command.substr(0, pos2-1);
    s_mod.append( s_command2  ).append( ".so" );

    dynmod *mod = wrap::MODL->get_module( s_mod );

    if ( mod == NULL || 
         wrap::CHAT->get_command_disabled( s_command2 ) ||
         get_status() > wrap::CHAT->get_command_status( s_command2 ) )
    { 
        string s_msg =  "<font color=\"" + wrap::CONF->get_elem("chat.html.errorcolor") + "\">" 
			+ wrap::CONF->get_elem( "chat.msgs.err.findingcommand" )
			+ "</font>\n";
        msg_post( &s_msg );
        return;
    }

    vector<string> params;

    // execute the module.
    if (s_command.find(" ") != string::npos)
    {
        s_command = s_command.substr(s_command2.size()+1);
        pos = s_command.find(" ");
        pos2 = 0;

        while (pos != string::npos)
        {
            string sParam = s_command.substr(pos2, pos - pos2);
            params.push_back(sParam);
            pos2 = pos + 1;
            pos = s_command.find(" ", pos2);
        }

        if (pos2 < s_command.size())
        {
            params.push_back(s_command.substr(pos2, s_command.size()-pos2));
        }

    }

    container* c = new container;
    //c->elem[0]=(void*) unreserved 
    c->elem[1] = (void*) this;
    c->elem[2] = (void*) &params;
    c->elem[3] = (void*) wrap::WRAP;

    ( *(mod->the_func) ) ( (void*) c );

    delete c;
}


void
user::s_mess_delete( )
{
    pthread_mutex_lock  ( &mut_s_mess );
    s_mess = "";
    pthread_mutex_unlock( &mut_s_mess );
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
user::check_timeout( int* i_idle_timeout )
{
 int i_user_timeout = (int) get_last_activity();  

 if ( get_away() ? i_idle_timeout[1] <= i_user_timeout : i_idle_timeout[0] <= i_user_timeout )
 {
  wrap::system_message( string(TIMERTO) + "(" + get_name() + "," + tool::int2string(i_user_timeout) + ")");
  string s_quit = "<script language=JavaScript>top.location.href='/"
                + wrap::CONF->get_elem("httpd.startsite")
                + "';</script>";
  msg_post( &s_quit ); 
  set_online( false );

  pthread_cond_signal( &cond_message );
 }

 else if ( ! get_away() && i_idle_timeout[2] <= i_user_timeout )
 {
  wrap::system_message( string(TIMERAT) + "(" + get_name() + "," + tool::int2string(i_user_timeout) + ")");
  string s_msg = wrap::CONF->get_elem("chat.msgs.userautoawaytimeout");
  set_away( true, s_msg );
  string s_msg2 = wrap::TIMR->get_time() + " <b>" + get_colored_name()+ "</b>" + s_msg + "<br>\n";
  get_room()->msg_post( &s_msg2 ); 
  get_room()->reload_onlineframe();
 }
}

void
user::get_user_list( string &s_list  )
{
    if ( get_invisible() )
     return;

    s_list.append( wrap::CONF->get_elem("chat.html.onlinebefore") );
    
    if ( get_away() )
    {
     s_list.append("<img src=\"" + wrap::CONF->get_elem("chat.html.rangimages.location")+ "away.gif\"" )
           .append( " alt='" )
           .append( get_away_msg() )
           .append( "' title='" )
           .append( get_away_msg() )
           .append( "'" + wrap::CONF->get_elem("chat.html.rangimages.options") + "> " );
    }
    else if ( ! get_is_reg() ) 
    {
     string s_msgs = wrap::CONF->get_elem("chat.msgs.guest"); 
     s_list.append("<img src=\"" + wrap::CONF->get_elem("chat.html.rangimages.location")+ "guest.png\"" )
           .append( " alt='" )
           .append( s_msgs )
           .append( "' title='" )
           .append( s_msgs )
           .append( "'" + wrap::CONF->get_elem("chat.html.rangimages.options") + "> " );
    }
    else if ( get_status() != tool::string2int( wrap::CONF->get_elem("chat.defaultrang") ) && ! get_fake() )
    {
     string s_status = "RANG" + tool::int2string( get_status() );
     string s_msgs = wrap::CONF->get_elem( "chat.msgs." + s_status ); 
     s_list.append("<img src=\"" + wrap::CONF->get_elem("chat.html.rangimages.location") +  tool::to_lower(s_status) + ".png\"" )
           .append( " alt='" )
           .append( s_msgs )
           .append( "' title='" )
           .append( s_msgs )
           .append( "'" + wrap::CONF->get_elem("chat.html.rangimages.options") + "> " );
    }

    else 
    {
     s_list.append("<img src=\"images/blank.gif\"" + wrap::CONF->get_elem("chat.html.rangimages.options") + "> ");
    }

    s_list.append( get_colored_name() );
    s_list.append( wrap::CONF->get_elem("chat.html.onlinebehind") );
    s_list.append( "\n" );
}

void
user::check_restore_away()
{
 if ( get_away() )
 {
  get_room()->msg_post( 
   new string( 
    wrap::TIMR->get_time()
    + " <b>" + get_colored_name()
    + "</b> " + wrap::CONF->get_elem( "chat.msgs.unsetmodeaway" )
    + "( <font color=" + get_col2() + ">"
    + get_away_msg() + "</font>)<br>\n" 
   )
  );
  set_away( false ); 
  get_room()->reload_onlineframe();
 }
}

void
user::reconf()
{
}

#endif
