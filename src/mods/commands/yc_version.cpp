#include <iostream>
#include "../../msgs.h"
#include "../../chat/user.h"
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
    user *p_user = (user*)c->elem[1];		// the corresponding user
    p_user->msg_post( tool::ychat_version() + "<br>\n");

    return 0;
  }
}

