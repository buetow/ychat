#include <iostream>
#include "../../conf.h"
#include "../../room.h"
#include "../../user.h"
#include "../../s_tool.cpp"
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
	
//	string *s_command_line=(string *)c->elem[0]; 	// contains the whole line
	user *p_user = (user*)c->elem[1];		// the corresponding user
	str_vector *params= (str_vector*) c->elem[2];	// param array

        string s_color;
        string s_color2;

        conf* p_conf = (conf*) c->elem[3]; 

        if ( params->empty() )
        {
	 s_color  = p_conf->get_val( "USERCOL1" );
         s_color2 = p_conf->get_val( "USERCOL2" );
        }

        else
        {
	 s_color = (string) params->front();
         params->erase( params->begin() );
         if ( ! params->empty() )
          s_color2 = (string) params->front();
         else
          s_color2 = p_conf->get_val( "USERCOL2" );
        }

	s_color  = s_tool::to_lower( s_color  );
	s_color2 = s_tool::to_lower( s_color2 );

	if( valid_color(s_color) != 1  )	
	{
		string *answerstring=new string(s_color + " is not a valid color.<br>\n");
		p_user->msg_post( answerstring );
	}
	else if( valid_color(s_color2) != 1  )	
	{
		string *answerstring=new string( s_color2 + " is not a valid color.<br>\n");
		p_user->msg_post( answerstring );
	}
        else  
	{  	
		string *answerstring=new string(p_user->get_colored_name()+ " changes color to <font color=\"#" 
                       + s_color + "\">" + s_color + "</font> <font color=\"#" 
                       + s_color2 + "\">" + s_color2 + "</font><br>\n");
		p_user->get_room()->msg_post( answerstring );
		p_user->set_col1(s_color);
		p_user->set_col2(s_color2);
	}
 }
 int valid_color( string s_color ){
	
	if(s_color.size()!=6) return 0;
	string valid="abcdef0123456789";
	for(int i=0;i<s_color.size();i++)
	{
		string s_char=s_color.substr(i,1);
		if(valid.find(s_char)==string::npos)
		return 0;
	}	
	return 1;
 }

}

