#include "../../incl.h"
#include "../../chat/user.h"

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
        conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF; 
	string s_msg = "";	

        if (params->size() >= 1)
        {
	  vector<string>::iterator iter = params->begin();

	  string s_help = p_conf->get_elem("chat.msgs.help." + *iter);

          if ( s_help != "" ) {
           s_msg.append("<b>" + *iter + ":</b> " + s_help + "<br>\n");
           p_user->msg_post( &s_msg ); 
	   return 0;
          }

	  s_msg.append(*iter + " " + p_conf->get_elem("chat.msgs.err.notavailable"));
         }
 
         else
         {
          s_msg.append(p_conf->get_elem("chat.msgs.err.wrongcommandusage"));
         }

         s_msg = "<font color=\"#" 
                       + p_conf->get_elem("chat.html.errorcolor")
                       + "\"> "
                       + s_msg
                       + "</font><br>\n";

         p_user->msg_post( &s_msg ); 

	return 0;
 }
}

