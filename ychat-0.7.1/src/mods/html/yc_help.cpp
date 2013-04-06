#include "../../incl.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C" {
 int extern_function(void *v_arg)
 {
	container* c         = (container*) v_arg;
        dynamic_wrap* p_wrap = (dynamic_wrap*) c->elem[0];
        user* 	   p_user    = (user*) c->elem[2];
        modl*      p_modl    = (modl*) p_wrap->MODL; 
        conf*      p_conf    = (conf*) p_wrap->CONF; 
        chat*      p_chat    = (chat*) p_wrap->CHAT; 
        string*    s_content = &(*((map_string*) c->elem[1]))["content"];

        vector<string>* p_vec_keys = p_modl->get_mod_vector();
          
	sort(p_vec_keys->begin(), p_vec_keys->end());
	vector<string>::iterator p_vec_keys_iter = p_vec_keys->begin();

        while( p_vec_keys_iter != p_vec_keys->end() ) {

         if ( p_vec_keys_iter->find( "mods/commands/", 0 ) != string::npos )
         {

          string s_command_name = p_vec_keys_iter->substr(17);
          s_command_name.erase( s_command_name.find( ".so" ) );

          if ( !p_chat->get_command_disabled( s_command_name ) && 
                p_chat->get_command_status( s_command_name ) >= p_user->get_status())
          { 
           s_content->append( "<table bordercolor=\"1\">\n" );
           s_content->append( "<tr><td><font color=\"#FFFFFF\"><b>/" + s_command_name + "</b></font></td></tr>\n" );
           s_content->append( "<tr><td>" + p_conf->get_elem("chat.msgs.help." + s_command_name) + "</td></tr>\n" );
           s_content->append( "</table>\n<br>\n" );
          }
         }
         p_vec_keys_iter++;
        }
 }
}

