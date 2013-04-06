#include <iostream>
#include "../../chat/user.h"
#include "../../chat/room.h"
/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C" {
 int valid_color( string );
 
 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
	
	user *p_user = (user*) c->elem[1];		// the corresponding user
	vector<string> *params= (vector<string>*) c->elem[2];	// param array
        timr* p_timr = (timr*) ((dynamic_wrap*)c->elem[3])->TIMR; 
        conf* p_conf = (conf*) ((dynamic_wrap*)c->elem[3])->CONF; 

	string s_msg = "";

        if ( p_conf->get_elem("chat.printalwaystime") == "true" )
         s_msg = p_timr->get_time() + " ";

        s_msg.append(  "<i>" + p_user->get_colored_name() 
                       + " <font color=\"#" + p_user->get_col2() 
                       + "\">" );
        if ( ! params->empty() )
        {
         vector<string>::iterator iter = params->begin();
         for ( iter = params->begin(); iter != params->end(); iter++ )
          s_msg.append( *iter + " " );
        }

        s_msg.append( "</font></i><br>\n" );
        p_user->get_room()->msg_post( &s_msg );

	return 0;
 }
}

