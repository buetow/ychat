#include <iostream>
#include "../../chat/user.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;


extern "C"
{
  int extern_function(void *v_arg)
  {
    container* c = (container*) v_arg;
    user* p_user = (user*)c->elem[1];

    conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF;

    string s_quit = "<script confuage=JavaScript>top.location.href='/"
                    + p_conf->get_elem("httpd.startsite")
                    + "';</script>";

    p_user->msg_post( &s_quit );
    p_user->set_online(false);

    return 0;
  }
}

