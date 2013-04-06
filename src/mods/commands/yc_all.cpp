#include <iostream>
#include "../../chat/chat.h"
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
    vector<string> *params= (vector<string>*) c->elem[2];	// param array
    timr* p_timr = (timr*) ((dynamic_wrap*)c->elem[3])->TIMR;
    conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF;
    chat* p_chat = (chat*) ((dynamic_wrap*)c->elem[3])->CHAT;

    string s_tmp = "";
    string s_msg = "";


    if ( ! params->empty() )
    {
      vector<string>::iterator iter = params->begin();
      for ( iter = params->begin(); iter != params->end(); iter++ )
        s_tmp.append( *iter + " " );

      s_msg = p_timr->get_time() + " <b><font color=\"#"
              + p_conf->get_elem("chat.html.sysmsgcolor")
              + "\"> "
              + p_conf->get_elem("chat.msgs.system");

      p_chat->string_replacer(&s_tmp);
      s_msg.append( s_tmp + "</font></b><br>\n" );
      p_chat->msg_post( &s_msg );
      return 0;
    }

    s_msg = p_conf->colored_error_msg("chat.msgs.err.wrongcommandusage");
    p_user->msg_post(s_msg);
    return 0;
  }
}

