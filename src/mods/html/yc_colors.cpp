#include "../../incl.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C" {
 int extern_function(void *v_arg)
 {
	container* c          = (container*) v_arg;
        dynamic_wrap* p_wrap = (dynamic_wrap*) c->elem[0];
        conf*      p_conf    = (conf*) p_wrap->CONF; 
        string*    p_col1     = &(*((map_string*) c->elem[1]))["col1"];
        string*    p_col2     = &(*((map_string*) c->elem[1]))["col2"];
        string*    p_flag     = &(*((map_string*) c->elem[1]))["flag"];
        user*      p_user     = (user*) c->elem[2];

        p_col1->erase(0);
        p_col2->erase(0);

        p_col1->append( p_user->get_col1() );
        p_col2->append( p_user->get_col2() );

        if ( *p_flag == "submit" )
        {
         string*  p_msgs  = &(*((map_string*) c->elem[1]))["msgs"];
         p_msgs->append( p_conf->get_elem( "chat.msgs.optionschanged" ) );
        }

	return 0;
 } 
}

