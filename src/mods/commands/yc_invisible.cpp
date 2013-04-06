#include "../../incl.h"
#include "../../chat/room.h"
#include "../../chat/user.h"

using namespace std;

extern "C" {
 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
	
	user* p_user = (user*) c->elem[1];		// the corresponding user
        conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF; 
       
        bool b_invisible = p_user->get_invisible(); 
 
        string s_msg = b_invisible
                     ? p_conf->get_elem("chat.msgs.setmodeinvisible") 
                     + "<br>\n"
                     : p_conf->get_elem("chat.msgs.unsetmodeinvisible") 
                     + "<br>\n"; 

        p_user->set_invisible( !b_invisible );
        p_user->get_room()->reload_onlineframe();
        p_user->msg_post ( &s_msg  );

	return 0;
 }
}

