#include "../../incl.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C" {
 int extern_function(void *v_arg)
 {

	container *c=(container *)v_arg;
	
	user *p_user = (user*) c->elem[1];		// the corresponding user
	vector<string> *params = (vector<string>*) c->elem[2];	// param array
        chat* p_chat = (chat*) ((dynamic_wrap*)c->elem[3])->CHAT; 
        conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF; 

        if ( ! params->empty() )
        {
         vector<string>::iterator iter = params->begin();
         string s_searched_user( *iter );
         bool b_found;
         user* p_searched_user = p_chat->get_user( s_searched_user, b_found );
       
         if ( b_found )
         {
          string s_msg         = "<b>" + p_searched_user->get_colored_name() + "</b>"
                               + p_conf->get_elem("chat.msgs.userinroom") + "<b>"
                               + p_searched_user->get_room()->get_name() 
          		       + "</b><br>\n";
          p_user->msg_post( &s_msg ); 
         }
      
         else
         {
          string s_msg = "<font color=\"#" 
                       + p_conf->get_elem("chat.html.errorcolor")
                       + "\"><b>" 
                       + s_searched_user + "</b> "
                       + p_conf->get_elem("chat.msgs.err.notavailable")
                       + "</font><br>\n";
          p_user->msg_post( &s_msg ); 

         }
        }

	return 0;
 }

}

