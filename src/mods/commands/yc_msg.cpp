#include "../../chat/user.h"
#include "../../chat/room.h"
/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C"
{
  int valid_color( string );

  int extern_function(void *v_arg)
  {
    container *c=(container *)v_arg;

    user *p_user = (user*) c->elem[1];		// the corresponding user
    vector<string> *params = (vector<string>*) c->elem[2];	// param array
    chat* p_chat = (chat*) ((dynamic_wrap*)c->elem[3])->CHAT;
    conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF;
    timr* p_timr = (timr*) ((dynamic_wrap*)c->elem[3])->TIMR;

    if ( ! params->empty() )
    {
      vector<string>::iterator iter = params->begin();
      string s_whisper_user( *iter );
      bool b_found;
      user* p_whisper_user = p_chat->get_user( s_whisper_user, b_found );

      if ( b_found )
      {
        string s_time = "";
        if ( p_conf->get_elem("chat.printalwaystime") == "true" )
          s_time = p_timr->get_time() + " ";

        string s_msg = s_time + "<i> " + p_conf->get_elem("chat.msgs.whisperto") + " " + p_whisper_user->get_colored_name() + ": <font color=\"#" + p_user->get_col2() + "\">";

        string s_whisper_msg = s_time + "<i>" + p_user->get_colored_name() + " " + p_conf->get_elem("chat.msgs.whisper") + ": <font color=\"#" + p_user->get_col2() + "\">";

        string s_tmp = "";
        for ( iter++; iter != params->end(); iter++ )
          s_tmp.append( *iter + " " );

        p_chat->string_replacer(&s_tmp);

        s_msg        .append( s_tmp + "</font></i><br>\n" );
        s_whisper_msg.append( s_tmp + "</font></i><br>\n" );

        p_user        ->msg_post( &s_msg         );
        p_whisper_user->msg_post( &s_whisper_msg );
      }
      else
      {
        string s_msg = "<font color=\"#"
                       + p_conf->get_elem("chat.html.errorcolor")
                       + "\"><b>"
                       + s_whisper_user + "</b> "
                       + p_conf->get_elem("chat.msgs.err.notavailable")
                       + "</font><br>\n";

        p_user->msg_post( &s_msg );
      }
    }

    return 0;
  }
}

