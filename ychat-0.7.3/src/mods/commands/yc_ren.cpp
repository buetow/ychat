#include "../../chat/user.h"
#include "../../chat/room.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C" {
 int extern_function(void *v_arg)
 {
	container *c = (container*) v_arg;
	
	user *p_user = (user*) c->elem[1];		// the corresponding user
	vector<string> *params = (vector<string>*) c->elem[2];	// param array

        chat* p_chat = (chat*) ((dynamic_wrap*)c->elem[3])->CHAT; 
        conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF; 
        timr* p_timr = (timr*) ((dynamic_wrap*)c->elem[3])->TIMR; 

        if ( ! params->empty() )
        {
         vector<string>::iterator iter; 

         string s_room;

         for ( iter = params->begin(); iter != params->end(); iter++ )
         {
          s_room.append( *iter ); 
          if ( iter+1 != params->end() )
           s_room.append(" ");
         }

         int i_max_room_len = tool::string2int( p_conf->get_elem("chat.maxlength.roomname") ); 
         if ( i_max_room_len < s_room.length() )
          s_room = s_room.substr(0, i_max_room_len );         

         bool b_found;
         string s_room_lower = tool::to_lower(s_room);
         room* p_room = p_user->get_room();
         
         if ( s_room_lower != p_room->get_lowercase_name() && 
              NULL != p_chat->get_room( s_room_lower , b_found ) )
         {
          string s_msg = "<font color=\"#" 
                       + p_conf->get_elem("chat.html.errorcolor")
                       + "\">" 
                       + p_conf->get_elem("chat.msgs.err.roomexists")
                       + "</font><br>\n";

          p_user->msg_post( &s_msg ); 
         }

         else
         { 
          string s_msg = p_timr->get_time() + " "
                       + p_user->get_colored_bold_name() 
                       + p_conf->get_elem( "chat.msgs.userrenamesroom" )
                       + "<b>" + s_room + "</b><br>\n";

          p_chat->del_elem( p_room->get_lowercase_name() ); 
          p_room->set_name( s_room );
          p_chat->add_elem( p_room ); 
          p_room->msg_post( &s_msg ); 
          p_room->reload_onlineframe();
         }
	}

	return 0;
 }

}

