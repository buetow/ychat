#include "../../incl.h"
#include "../../chat/room.h"
#include "../../chat/user.h"

using namespace std;

extern "C" {
 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
	
	user* p_user = (user*) c->elem[1];	
        conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF; 
        timr* p_timr = (timr*) ((dynamic_wrap*)c->elem[3])->TIMR; 
        chat* p_chat = (chat*) ((dynamic_wrap*)c->elem[3])->CHAT; 

	vector<string> *params = (vector<string>*) c->elem[2];	// param array
      
        bool b_invalid_syntax = 0;
 
        if ( params->size() >= 3 )
        { 
         string s_msg;
         string s_command_name = params->at(0);
         string s_option = params->at(1);
         string s_newval = params->at(2);

         if ( s_option == "status" )
         {
          const char* c_newval = s_newval.c_str();
          if ( isdigit( c_newval[0] ) )
          {
           int i_newval = c_newval[0] - '0';

           s_msg = p_timr->get_time()
              + " "
              + p_user->get_colored_bold_name()
              + " "
              + p_conf->get_elem("chat.msgs.setcommandstatus")
              + " (" 
              + s_command_name + "," + s_newval + ","
              + tool::int2string( p_chat->get_command_disabled(s_command_name)) 
              + ")<br>\n";

           p_user->get_room()->msg_post(&s_msg);
           p_chat->set_command_status( s_command_name, i_newval );
          } 
         }
         else if ( s_option == "disable" )
         {
          const char* c_newval = s_newval.c_str();
          if ( isdigit( c_newval[0] ) )
          {
           int i_newval = c_newval[0] - '0';

           if ( i_newval != 0 )
            i_newval = 1;

           s_newval = tool::int2string(i_newval);

           s_msg = p_timr->get_time()
              + " "
              + p_user->get_colored_bold_name()
              + " "
              + p_conf->get_elem("chat.msgs.setcommandstatus")
              + " (" 
              + s_command_name + "," 
              + tool::int2string( p_chat->get_command_status(s_command_name)) 
              + "," + s_newval + ")<br>\n";

           p_user->get_room()->msg_post(&s_msg);
           p_chat->set_command_disabled( s_command_name, (bool)i_newval );
          } 
         }

         else 
         {
          b_invalid_syntax = 1;
         }  
 
        }
       
        else
        {
         b_invalid_syntax = 1;
        }  
         
        if ( b_invalid_syntax )
        {
          string s_msg = "<font color=\"#" 
                       + p_conf->get_elem("chat.html.errorcolor")
                       + "\">" 
                       + p_conf->get_elem("chat.msgs.err.wrongcommandusage")
                       + "</font><br>\n";
          p_user->msg_post( &s_msg ); 
        }

	return 0;
 }
}

