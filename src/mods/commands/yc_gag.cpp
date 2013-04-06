#include "../../chat/user.h"
#include "../../chat/room.h"
/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C"
{
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
      string s_gag_user(*iter);
      bool b_found;
      user* p_gag_user = p_chat->get_user( s_gag_user, b_found );

      if ( b_found )
      {
        string s_time = "";
        if ( p_conf->get_elem("chat.printalwaystime") == "true" )
          s_time = p_timr->get_time() + " ";

        string s_reason = "";
        for ( iter++; iter != params->end(); iter++ )
          s_reason.append( *iter + " " );

        if ( s_reason.length() > 0 )
	  s_reason = " (" + p_user->make_colors(s_reason) + ")";

        string s_msg = s_time + "<i> " + p_user->get_colored_bold_name() + " " + p_conf->get_elem("chat.msgs.gag") + " "
			+ p_gag_user->get_colored_bold_name() + s_reason + "</i><br>\n";


	p_user->msg_post(s_msg);
	if (! p_user->same_rooms(p_gag_user) )
	  p_gag_user->msg_post(s_msg);

        p_gag_user->set_is_gag(true);

      }
      else
      {
        string s_msg = "<font color=\"#"
                       + p_conf->get_elem("chat.html.errorcolor")
                       + "\"><b>"
                       + s_gag_user + "</b> "
                       + p_conf->get_elem("chat.msgs.err.notavailable")
                       + "</font><br>\n";

        p_user->msg_post( &s_msg );
      }
    }

    return 0;
  }
}

