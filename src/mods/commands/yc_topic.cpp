#include "../../tool/tool.h"
#include "../../incl.h"
#include "../../chat/room.h"
#include "../../chat/user.h"

using namespace std;

extern "C" {
 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
	
	user* p_user = (user*) c->elem[1];		// the corresponding user
        timr* p_timr = (timr*) ((dynamic_wrap*)c->elem[3])->TIMR; 
        conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF; 
	vector<string> *params = (vector<string>*) c->elem[2];	// param array
        
        string s_msg;
        string s_topic;
        string s_col;

        room* p_room = p_user->get_room();

        if ( ! params->empty() )
        {
         vector<string>::iterator iter;

         s_col = "<font color=\"" + p_user->get_col1() + "\">"; 

         s_msg = p_timr->get_time()
               + " "
               + p_user->get_colored_bold_name()
               + " "
               + p_conf->get_elem("chat.msgs.topic")
               + s_col;

         for ( iter = params->begin(); iter != params->end(); iter++ )
         {
          s_msg.append( " " + *iter );
          s_topic.append( *iter + " " );
         }
         s_msg.append( "</font>" );

         int i_topic_length = tool::string2int(p_conf->get_elem("chat.maxlength.topic"));
         if ( s_topic.length() > i_topic_length )
         {
          s_topic = s_topic.substr(0, i_topic_length );
          s_topic.append(" ... ");

          string s_private_msg = "<font color=#" + p_conf->get_elem("chat.html.errorcolor") 
           + ">" + p_conf->get_elem( "chat.msgs.topiccut" ) + "</font><br>";

          p_user->msg_post ( &s_private_msg ); 
         }

         s_topic = "( " + s_topic + ")";
         p_room->set_topic( s_topic, p_user->get_col1() );
        } 

        else
        {
         s_msg = p_timr->get_time()
               + " "
               + p_user->get_colored_name()
               + " "
               + p_conf->get_elem("chat.msgs.topicdelete");

         p_room->set_topic( "" );
        }

        s_msg.append( "<script confuage='JavaScript'>parent.online.location.reload();</script><br>\n" );

        p_room->msg_post ( &s_msg  );

	return 0;
 }
}

