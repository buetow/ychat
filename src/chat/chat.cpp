// class chat implementation.

#ifndef CHAT_CPP
#define CHAT_CPP

#include "chat.h"
#include "../tool/tool.h"
#include "../maps/mtools.h"
#include <list>

using namespace std;

chat::chat( )
{
  { // Set up replace strings;
    wrap::system_message(CHATREP);
    vector<string>* p_vec_keys = wrap::CONF->get_key_vector();

    for (vector<string>::iterator iter = p_vec_keys->begin();
         iter != p_vec_keys->end();
         iter++ )
    {
      if ( iter->length() >= 24 && iter->compare( 0, 22, "chat.html.replace.from" ) == 0 )
      {
        string s_from = wrap::CONF->get_elem(*iter);
        map_replace_strings[s_from] = "chat.html.replace.into." + s_from;
        vec_replace_keys.push_back(s_from);
      }
    }

    sort(vec_replace_keys.begin(), vec_replace_keys.end());
    delete p_vec_keys;
  }
}

chat::~chat( )
{
  // Delete all room objects!
  run_func( mtools<room*>::delete_obj );
}

user*
chat::get_user( string &s_user )
{
  bool b_flag;
  return get_user( s_user, b_flag );
}

user*
chat::get_user( string &s_user, bool &b_found )
{
  container param;

  param.elem[0] = (void*) &s_user ;
  param.elem[1] = (void*) &b_found;

  b_found = false;

  base<room>::run_func( get_user_, (void*)&param );

  if ( *( (bool*) param.elem[1] ) )
    return (user*) param.elem[2];
}

void
chat::get_user_( room *room_obj, void *v_arg )
{
  container* param = (container*) v_arg;

  if ( *((bool*)param->elem[1]) )
    return;

  param->elem[2] = (void*)room_obj->get_elem( *((string*)param->elem[0]), *((bool*)param->elem[1]) );
}

void
chat::login( hashmap<string> &map_params )
{
  string s_user = map_params["nick"];

  // prove if nick is empty:
  if ( s_user.empty() )
  {
    map_params["INFO"]    = wrap::CONF->get_elem( "chat.msgs.err.nonick" );
    map_params["request"] = wrap::CONF->get_elem( "httpd.startsite" ); // redirect to the startpage.
    wrap::system_message( LOGINE0 );
    return;
  }

  // prove if the nick is alphanumeric:
  else if ( ! tool::is_alpha_numeric( s_user ) )
  {
    map_params["INFO"]    = wrap::CONF->get_elem( "chat.msgs.err.alpnum" );
    map_params["request"] = wrap::CONF->get_elem( "httpd.startsite" ); // redirect to the startpage.
    wrap::system_message( LOGINE1 + s_user  );
    return;
  }

  // prove if the nick is too long:
  else if ( s_user.length()  > tool::string2int( wrap::CONF->get_elem("chat.maxlength.username") ) )
  {
    map_params["INFO"]    = wrap::CONF->get_elem( "chat.msgs.err.nicklength" );
    map_params["request"] = wrap::CONF->get_elem( "httpd.startsite" ); // redirect to the startpage.
    wrap::system_message( LOGINE2 + s_user  );
    return;
  }

  // prove if the room name is too long:
  else if ( map_params["room"].length() > tool::string2int( wrap::CONF->get_elem("chat.maxlength.roomname") ) )
  {
    map_params["INFO"]    = wrap::CONF->get_elem( "chat.msgs.err.roomnamelength" );
    map_params["request"] = wrap::CONF->get_elem( "httpd.startsite" ); // redirect to the startpage.
    wrap::system_message( LOGINE3 + s_user + " / " + map_params["room"] );
    return;
  }

  // prove if the room name is valid
  else if ( map_params["room"].length() < 1 )
  {
    map_params["INFO"]    = wrap::CONF->get_elem( "chat.msgs.err.noroom" );
    map_params["request"] = wrap::CONF->get_elem( "httpd.startsite" ); // redirect to the startpage.
    wrap::system_message( LOGINE3 + s_user + " / " + map_params["room"] );
    return;
  }

  bool b_flag;

  // prove if nick is already online / logged in.
  get_user( s_user, b_flag );

  if ( b_flag )
  {
    map_params["INFO"]    = wrap::CONF->get_elem( "chat.msgs.err.online" );
    map_params["request"] = wrap::CONF->get_elem( "httpd.startsite" );

    return;
  }

  // Prove if user is recycleable from the garbage collector:
  user *p_user = wrap::GCOL->get_user_from_garbage( s_user );

  if ( p_user != NULL )
  {
    // 1. possibility to prove the password at login! (using recycled user)
    if ( p_user->get_pass() != map_params["password"] )
    {
      map_params["INFO"]    = wrap::CONF->get_elem( "chat.msgs.err.wrongpassword" );
      map_params["request"] = wrap::CONF->get_elem( "httpd.startsite" ); // redirect to the startpage.
      wrap::system_message( LOGINER + s_user );
      return;
    }
    if ( p_user->get_has_sess() )
    {
      map_params["tmpid"] = p_user->get_tmpid();
    }
    else
    {
      sess* p_sess = wrap::SMAN->create_session();
      //p_sess->set_value( string("nick"), (void *) new string(s_user) );
      p_sess->set_user(p_user);
      map_params["tmpid"] = p_sess->get_tmpid();
      p_user->set_tmpid( map_params["tmpid"] );
      p_user->set_has_sess( true );
    }
  }
  else //  if ( p_user == NULL ) // If not in garbage create a new user!
  {
    p_user = new user( s_user );

    // prove if nick is registered, else create a guest chatter.
#ifdef DATABASE

    hashmap<string> map_results = wrap::DATA->select_user_data( tool::to_lower(s_user), "selectlogin");

    if ( map_results["nick"] == tool::to_lower(s_user) )
    {
      p_user->set_is_reg( true );
      // User exists in database, prove password:
      // 2. possibility to prove the password at login! (using new created user from database)
      if ( map_results["password"] != map_params["password"] )
      {
        map_params["INFO"]    = wrap::CONF->get_elem( "chat.msgs.err.wrongpassword" );
        map_params["request"] = wrap::CONF->get_elem( "httpd.startsite" ); // redirect to the startpage.
        wrap::system_message( LOGINER + s_user );
        return;
      }
      else
      { // If registered use saved options
        map_params["registered"] = "yes";
        map_params["color1"] = map_results["color1"];
        map_params["color2"] = map_results["color2"];
        map_params["email"] = map_results["email"];
        map_params["status"] = map_results["status"];
      }
    }
    else
#endif

    { // If not registered prove if guest chatting is enabled.
      if (wrap::CONF->get_elem("chat.enableguest") != "true")
      {
        map_params["INFO"]    = wrap::CONF->get_elem( "chat.msgs.err.noguest" );
        map_params["request"] = wrap::CONF->get_elem( "httpd.startsite" ); // redirect to the startpage.
        wrap::system_message( LOGINE4 + s_user );
        return;
      }

      // Guest chatter are enabled, use standard font colors
      map_params["color1"] = wrap::CONF->get_elem( "chat.html.user.color1" );
      map_params["color2"] = wrap::CONF->get_elem( "chat.html.user.color2" );
      map_params["status"] = wrap::CONF->get_elem( "chat.defaultrang" );
    }

    sess* p_sess = wrap::SMAN->create_session();
    p_sess->set_user(p_user);
    map_params["tmpid"] = p_sess->get_tmpid();
    p_user->set_tmpid( map_params["tmpid"] );
    p_user->set_col1( map_params["color1"] );
    p_user->set_col2( map_params["color2"] );
    p_user->set_status( tool::string2int(map_params["status"]));
    // p_user->set_sess( p_sess );
  }

  // Prove if user is the default operator.
  if ( tool::to_lower(wrap::CONF->get_elem("chat.defaultop")) == tool::to_lower(s_user) )
  {
    wrap::system_message(CHATDOP);
    p_user->set_status(0);
  }

  string s_room = map_params["room"];
  room*  p_room = get_room( s_room , b_flag );

  // if room does not exist add room to list!
  if ( ! b_flag )
  {
    p_room = wrap::GCOL->get_room_from_garbage();

    if ( p_room )
    {
      p_room->set_name( s_room );
      wrap::system_message( REUROOM + s_room );
    }
    else
    {
      p_room = new room( s_room );
      wrap::system_message( NEWROOM + s_room );
    }

    add_elem( p_room );
  }

  // add user to the room.
  p_room->add_user( p_user );

  wrap::system_message( NEWUSER + s_user );

  // post "username enters the chat" into the room.
  string s_msg = wrap::TIMR->get_time() + " " + p_user->get_colored_bold_name() + wrap::CONF->get_elem( "chat.msgs.userenterschat" ) + "<br>\n";

  // If created a new user from database while logging on (not a recycled user, they already have this set)
  if ( map_params["registered"] == "yes" )
  {
    p_user->set_email( map_params["email"] );
    p_user->set_pass( map_params["password"] );
    // Now we will store all wanted user data into MySQL after logging out! (recycled user already have this set)
    p_user->set_changed_data_on();
  }
  p_room->msg_post( &s_msg );
}

void
chat::post( user* p_user, hashmap<string> &map_params )
{
  p_user->renew_timeout();

  string s_msg( map_params["message"] );

  if ( s_msg.empty() )
    return;

  int i_max_message_length = tool::string2int(wrap::CONF->get_elem( "chat.maxlength.message" ));
  if ( s_msg.length() > i_max_message_length )
  {
    s_msg = s_msg.substr( 0, i_max_message_length );
    string s_private = "<font color=\"" + wrap::CONF->get_elem( "chat.html.errorcolor" ) + "\">"
                       + wrap::CONF->get_elem( "chat.msgs.err.messagelength" ) + "</font><br>\n";
    p_user->msg_post( &s_private );
  }

  int i_max_word_length = tool::string2int(wrap::CONF->get_elem( "chat.maxlength.word" ));

  // Check max word length
  list<string> list_msg = tool::split_string( s_msg, " ");
  list<string>::iterator iter = list_msg.begin();

  for ( s_msg = ""; iter != list_msg.end(); ++iter )
  {
    if ( iter->length() > i_max_word_length )
    {
      string s_tmp[] = { iter->substr(0, i_max_word_length ), iter->substr(i_max_word_length) };
      iter = list_msg.erase( iter );
      iter = list_msg.insert( iter, s_tmp[1] );
      iter = list_msg.insert( iter, s_tmp[0] );
      --iter;
    }
    else
    {
      s_msg.append(*iter + " ");
    }
  }

  if ( wrap::CONF->get_elem( "chat.html.tagsallow" ) != "true" )
    tool::strip_html( &s_msg );

  unsigned i_pos = s_msg.find( "/" );
  if ( i_pos == 0 )
    return p_user->command( s_msg );

  string_replacer(&s_msg);
  string s_post;

  if ( wrap::CONF->get_elem( "chat.printalwaystime" ) == "true" )
    s_post.append( wrap::TIMR->get_time() + " " );

  s_post.append( "<font color=\"#" )
  .append( p_user->get_col1() )
  .append( "\">"              )
  .append( p_user->get_name() )
  .append( ":</font> <font color=\"#")
  .append( p_user->get_col2() )
  .append( "\">"  )
  .append( s_msg              )
  .append( "</font><br>\n"    );

  p_user->get_room()->msg_post( &s_post );
}

void
chat::reconf()
{}

void
chat::string_replacer(string *p_msg)
{
  if ( wrap::CONF->get_elem( "chat.html.replace.activate" ) == "true" )
  {
    for (vector<string>::iterator iter = vec_replace_keys.end()-1;
         iter != vec_replace_keys.begin();
         iter-- )
      *p_msg = tool::replace( *p_msg, *iter, wrap::CONF->get_elem(map_replace_strings[*iter]) );
  }
}

#endif
