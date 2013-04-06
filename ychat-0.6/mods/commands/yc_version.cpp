#include <iostream>
#include "../../msgs.h"
#include "../../user.h"
/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C" {
 export int extern_function(void *v_arg);
 int valid_color( string );
 
 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
	
	user *p_user = (user*)c->elem[1];		// the corresponding user
 	p_user->msg_post( new string( *new string(VERSION) + "<br>\n") );
 }
}

