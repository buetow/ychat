#include <iostream>
#include "../user.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;


extern "C" {
 export int extern_function(void *v_arg);

 int extern_function(void *v_arg)
 {
  user *p_user = (user*) v_arg;
  p_user->msg_post(  new string( "<script language='JavaScript'> window.open( 'index.html', '_top' ); </script>\n  " ) );
  p_user->set_online( false );
 }
}

