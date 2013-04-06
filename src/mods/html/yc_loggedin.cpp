#include "../../chat/chat.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C"
{
  int extern_function(void *v_arg)
  {
    container* c          = (container*) v_arg;
    dynamic_wrap* p_wrap  = (dynamic_wrap*) c->elem[0];
    chat*      p_chat     = (chat*) p_wrap->CHAT;
    map<string,string>*  p_map  = (map<string,string>*) c->elem[1];

    /*
    	string s_list;
            p_chat->get_user_list(s_list);
    	(*p_map)["USERLIST"] = s_list;
    */

    p_chat->get_user_list((*p_map)["USERLIST"]);

    return 0;
  }
}

