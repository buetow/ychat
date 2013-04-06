#include "../../incl.h"
#include "../../tool/tool.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C" {
 string replace_chars(string s_string) {
  s_string = tool::replace(s_string, "<", "&lt;");
  s_string = tool::replace(s_string, ">", "&gt;");
  s_string = tool::replace(s_string, "\n", "<br>");
  return s_string;
 }

 int extern_function(void *v_arg)
 {
	container* c         = (container*) v_arg;
        dynamic_wrap* p_wrap = (dynamic_wrap*) c->elem[0];
        user* 	   p_user    = (user*) c->elem[2];
        conf*      p_conf    = (conf*) p_wrap->CONF; 
        string*    p_content = &(*((map_string*) c->elem[1]))["content"];

         vector<string>* p_vec_keys = p_conf->get_key_vector();
	 sort(p_vec_keys->begin(), p_vec_keys->end());
 	 vector<string>::iterator p_vec_keys_iter = p_vec_keys->begin();


         p_content->append( "Currently are " + tool::int2string(p_vec_keys->size()) + " options available!<br><br>\n");

         p_content->append( "<table bordercolor=\"1\">\n" );

         for( ;p_vec_keys_iter != p_vec_keys->end(); ++p_vec_keys_iter ) {

          if ( p_vec_keys_iter->find(".descr") != string::npos )
           continue;

          p_content->append( "<tr><td>\n<font color=\"#FFFFFF\"><b>" + *p_vec_keys_iter + "\n</b></font></td></tr>\n" );

          string s_descr = p_conf->get_elem(*p_vec_keys_iter + ".descr");
	  if ( s_descr.length() > 0 ) 
          {    
           p_content->append( "<tr><td><font color=\"#CCCCCC\">\n" );
           p_content->append( replace_chars(p_conf->get_elem(*p_vec_keys_iter + ".descr")) );
           p_content->append( "\n</font></td></tr>\n" );
          }

          p_content->append( "<tr><td>\n" );

	  // Hide some fields!
          if ( p_vec_keys_iter->find(".password") != string::npos
		|| p_vec_keys_iter->find("base.user") != string::npos 
		|| p_vec_keys_iter->find(".dbname") != string::npos )
           p_content->append( "X X X X X X X X X X" );

          else
           p_content->append( replace_chars(p_conf->get_elem(*p_vec_keys_iter)) );

          p_content->append( "<br><br></td></tr>\n" );
         } // while

         p_content->append( "</table>\n<br>\n" );

	return 0;
 }
}

