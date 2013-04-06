#include "../../incl.h"

using namespace std;

extern "C"
{
  int extern_function(void *v_arg)
  {
    container *c=(container *)v_arg;

    user *p_user = (user*) c->elem[1];		// the corresponding user
    conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF;
    timr* p_timr = (timr*) ((dynamic_wrap*)c->elem[3])->TIMR;

    string s_msg = p_conf->get_elem("chat.msgs.uptime") + p_timr->get_uptime() + "<br>\n";

    p_user->msg_post( &s_msg );

    return 0;
  }
}

