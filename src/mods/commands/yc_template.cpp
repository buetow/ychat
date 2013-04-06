#include <iostream>
#include "../../chat/user.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C" {
 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
	
	user *p_user = (user*)c->elem[1];		// the corresponding user
	vector<string> *params=(vector<string>*)c->elem[2];	// param array
	
  	string *quitstring=new string("Text to send<br>");
	p_user->msg_post( quitstring );
 }
}

