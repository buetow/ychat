#include "../../chat/user.h"
#include "../../chat/room.h"

using namespace std;

extern "C"
{
  string to_upper( string &s_msg )
  {
    char* c_msg = (char*) s_msg.c_str();

    for ( int i = 0; i < strlen( c_msg ); i++ )
      c_msg[i] = toupper( c_msg[i] );

    return string( c_msg );
  };

  int extern_function(void *v_arg)
  {
    container *c=(container *)v_arg;

    user *p_user = (user*) c->elem[1];		// the corresponding user
    vector<string> *params= (vector<string>*) c->elem[2];	// param array

    conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF;
    timr* p_timr = (timr*) ((dynamic_wrap*)c->elem[3])->TIMR;

    string s_time = "";
    if ( p_conf->get_elem("chat.printalwaystime") == "true" )
      s_time = p_timr->get_time() + " ";

    string s_msg = s_time + "<i>" + p_user->get_colored_name() + " " + p_conf->get_elem("chat.msgs.scream") + " <font color=\"#" + p_user->get_col2() + "\"><b>";

    if ( ! params->empty() )
    {
      vector<string>::iterator iter = params->begin();
      for ( iter = params->begin(); iter != params->end(); iter++ )
        s_msg.append( to_upper(*iter) + " " );
    }

    s_msg.append( "</b></font></i><br>\n" );
    p_user->get_room()->msg_post( &s_msg );

    return 0;

  }
}

