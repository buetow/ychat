#include "../../chat/chat.h"
#include "../../chat/user.h"
/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C" {
 int valid_color( string );
 
 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
        chat* p_chat = (chat*) ((dynamic_wrap*)c->elem[3])->CHAT; 
        conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF; 
	user *p_user = (user*)c->elem[1];

	string s_list;
 	p_chat->get_user_list(s_list);	
 	p_user->msg_post( p_conf->get_elem("chat.msgs.userlist") + 
		"<br><table>" + s_list + "</table>\n");

	return 0;
 }
}

