// class room declaration.

#include "../incl.h"
#include "../wrap.h"

#ifndef ROOM_H
#define ROOM_H

#include "base.h"
#include "../name.h"
#include "user.h"
#include "../tool/tool.h"
#include "../logd.h"

using namespace std;

class room : public base<user>, public name
{
private:
  string s_topic;
  pthread_mutex_t mut_s_topic;
#ifdef LOGGING

  logd* p_logd;
#endif

  void dumpit();

public:
  room( string s_name );
  ~room();

  virtual void reload_onlineframe();

  void add_user( user* p_user  )
  {
    add_elem( p_user );
    p_user->set_p_room( this );
    reload_onlineframe();
  }

  user* get_user( string &s_name, bool &b_found )
  {
    return static_cast<user*>( get_elem( s_name, b_found ) );
  }

  void del_elem( string &s_name )
  {
    base<user>::del_elem( s_name );

    if ( base<user>::size() == 0 )
      clean_room();

    else
      reload_onlineframe();
  }

  void msg_post( string s_msg )
  {
    msg_post( &s_msg );
  }

  void msg_post( string *p_msg )
  {
#ifdef LOGGING
    p_logd->log_simple_line( logd::remove_html_tags(*p_msg) );
#endif

    base<user>::msg_post( p_msg );
  }

  void javascript_post( string s_msg )
  {
    string s_js = "<script language=\"JavaScript\">"+s_msg+"</script>\n";
    base<user>::msg_post( &s_js );
  }

  string get_bold_name()
  {
    return "<b>" + get_name() + "</b>";
  }

  virtual void set_name( string s_name );
  string get_topic();
  void set_topic( string s_topic );
  void set_topic( string s_topic, string s_color );
  void clean_room();



};

#endif