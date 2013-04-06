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
       
        bool b_fake = p_user->get_fake(); 
 
        string s_msg = (b_fake == 1
                        ? p_conf->get_elem("chat.msgs.setmodefake") + "<br>\n" 
                        : p_conf->get_elem("chat.msgs.unsetmodefake") + "<br>\n"); 
                     

        p_user->set_fake( !b_fake );
        p_user->get_room()->reload_onlineframe();
        p_user->msg_post ( &s_msg  );
 }
}

