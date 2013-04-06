#include <iostream>
#include "../../user.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;


extern "C" {
 export int extern_function(void *v_arg);

 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
	string *sCommandLine=(string *)c->elem[0];
	user *p_user = (user*)c->elem[1];

  	string *quitstring=new string("<script language=JavaScript>top.location.href='/index.html';</script>");
	p_user->msg_post( quitstring );
        p_user->set_online(false);
 }
}

