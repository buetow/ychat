#ifndef PERM_CPP
#define PERM_CPP

#include "perm.h"
#include "../maps/mtools.h"

using namespace std;

perm::perm()
{
 p_command_status = new shashmap< valwrap<int>* >;
 p_command_disabled = new shashmap< valwrap<bool>* >;

 set_standard_command_permissions();
}

perm::~perm()
{
 p_command_status->run_func( &mtools< valwrap<int>* >::delete_obj );
 p_command_disabled->run_func( &mtools< valwrap<bool>* >::delete_obj );

 delete p_command_status;
 delete p_command_disabled;
}

int
perm::get_command_status( string s_command )
{
 if ( p_command_status->exists( s_command ) )
  return p_command_status->get_elem( s_command )->val; 

 return 0;
}

void
perm::set_command_status( string s_command, int i_int )
{
 valwrap<int>* p_val = p_command_status->get_set_elem( new valwrap<int>(i_int), s_command ); 

 if ( p_val )
  delete p_val;
}

bool
perm::get_command_disabled( string s_command )
{
 if ( p_command_disabled->exists( s_command ) )
  return p_command_disabled->get_elem( s_command )->val; 
 return 0;
}

void
perm::set_command_disabled( string s_command, bool b_bool )
{
 valwrap<bool>* p_val = p_command_disabled->get_set_elem( new valwrap<bool>(b_bool), s_command ); 

 if ( p_val )
  delete p_val;
}

void
perm::set_standard_command_permissions()
{
 wrap::system_message(PERMSTD);

 vector<string>* p_vec_keys = wrap::CONF->get_key_vector();

 for (vector<string>::iterator iter = p_vec_keys->begin(); iter != p_vec_keys->end(); iter++ )
 {
       if ( iter->length() > 17 && iter->compare( 0, 16, "chat.permissions" ) == 0 )
       {
	valwrap<int>* p_val = new valwrap<int>( tool::string2int( wrap::CONF->get_elem(*iter) ) );
        p_command_status->add_elem( p_val, iter->substr(17) ); 
       }
 }

 delete p_vec_keys;
}

#endif
