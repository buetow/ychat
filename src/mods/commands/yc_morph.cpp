#include "../../chat/user.h"
#include "../../chat/room.h"
#include "../../tool/tool.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C"
{
  int extern_function(void *v_arg)
  {
    container *c = (container*) v_arg;

    user *p_user = (user*) c->elem[1];		// the corresponding user
    vector<string> *p_params = (vector<string>*) c->elem[2];	// param array
    conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF;
    timr* p_timr = (timr*) ((dynamic_wrap*)c->elem[3])->TIMR;

    if ( p_params->empty() )
    {
      string s_msg = p_conf->colored_error_msg("chat.msgs.err.wrongcommandusage");
      p_user->msg_post(s_msg);
    }
    else
    {
      string s_morphed_name = p_params->at(0);

      if ( tool::to_lower(s_morphed_name) != p_user->get_lowercase_name() )
      {
        string s_msg = p_conf->colored_error_msg("chat.msgs.err.morphnick");
        p_user->msg_post(s_msg);
      }
      else
      {
        string s_msg = p_timr->get_time() + " " + p_user->get_colored_bold_name() + p_conf->get_elem( "chat.msgs.usermorphs" ) + "<font color=\"#" + p_user->get_col1() + "\"><b>" + s_morphed_name + "</b></font><br>\n";

        room* p_room = p_user->get_room();
        p_room->msg_post( &s_msg );
        p_user->set_name( s_morphed_name );
        p_room->reload_onlineframe();
      }
    }

    return 0;
  }

}


