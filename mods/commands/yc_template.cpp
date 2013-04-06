#include <iostream>
#include "../../user.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

typedef vector<string> str_vector;

extern "C" {
 export int extern_function(void *v_arg);

 
 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
	
	string *sCommandLine=(string *)c->elem[0]; 	// contains the whole line
	user *p_user = (user*)c->elem[1];		// the corresponding user
	str_vector *params=(str_vector*)c->elem[2];	// param array

	
  	string *quitstring=new string("Text to send<br>");
	p_user->msg_post( quitstring );
 }
}

