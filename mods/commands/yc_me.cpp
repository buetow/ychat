#include <iostream>
#include "../../user.h"
#include "../../room.h"
/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

typedef vector<string> str_vector;

extern "C" {
 export int extern_function(void *v_arg);
 int valid_color( string );
 
 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
	
	user *p_user = (user*) c->elem[1];		// the corresponding user
	str_vector *params= (str_vector*) c->elem[2];	// param array

        string s_msg = "<i><font color=\"#" +p_user->get_col1() + "\">"
                       + p_user->get_name() + " ";

        if ( ! params->empty() )
        {
         str_vector::iterator iter = params->begin();
         for ( iter = params->begin(); iter != params->end(); iter++ )
          s_msg.append( *iter + " " );
        }

        s_msg.append( "</font></i><br>\n" );
        p_user->get_room()->msg_post( &s_msg );

 }
}

