#include <iostream>
#include <string>
#include "../../glob.h"
#include "../../hmap.h"
#include "../../lang.h"

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C" {
 export int extern_function(void *v_arg);

 int extern_function(void *v_arg)
 {
	container* c         = (container*) v_arg;
        string*    s_content = &(*((map_string*) c->elem[2]))["content"];
        lang*      p_lang    = (lang*) c->elem[0]; 

        vector<string>* p_vec_keys =  
          ((hmap<dynmod*,string>*) c->elem[1])->get_key_vector();
  	
	sort(p_vec_keys->begin(), p_vec_keys->end());
	vector<string>::iterator p_vec_keys_iter = p_vec_keys->begin();

        while( p_vec_keys_iter != p_vec_keys->end() ) {
         if ( p_vec_keys_iter->find( "mods/commands/", 0 ) != string::npos )
         {
          s_content->append( "<tr><td>" );
          string s_helptext = p_lang->get_val( *p_vec_keys_iter ) ;

          unsigned int i_pos = s_helptext.find( "||", 0 ); 
          if ( i_pos != string::npos )
           s_helptext.replace( i_pos, 2, "</td><td>" );

          s_content->append( s_helptext );
          s_content->append( "</td></tr>\n" );
         }
         p_vec_keys_iter++;
        }
 }
}

