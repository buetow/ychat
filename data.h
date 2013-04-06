#ifndef DATA_H
#define DATA_H

#include "base.h"
#include "MUTX.h"

using namespace std;

template<class type>
class data : public base
{
public:

 // chat::msg_post sends to all users of the system a message.
 // room::msg_post sends to all users of the room a message.
 // user::msg_post sends to the user a message.
 void msg_post( string *s_msg )
 {
  run_func( &data<type>::msg_post_ , (void*)s_msg );
 }
 static void msg_post_( name* name_obj, void* v_arg )
 {
   string *p_msg    = (string*) v_arg;
   type   *type_obj = static_cast<type*>(name_obj);

   type_obj -> msg_post( p_msg ); 
 }

 void get_data( map_string *p_map_string )
 {
  run_func( &data<type>::get_data_ , (void*)p_map_string );
 }
 static void get_data_( name* name_obj, void* v_arg )
 {
   map_string *map_params = (map_string*) v_arg;
   type       *type_obj   = static_cast<type*>(name_obj);

   type_obj -> get_data ( map_params );
 }

 // chat::get_user_list gets a list of all users of the system.
 // room::get_user_list gets a list of all users of the room.
 // user::get_user_list gets a "list" of a user <font color="usercolor">username</font>seperator
 void get_user_list( string &s_list, string &s_seperator )
 {

   container c; 
   c.elem[0] = (void*) &s_list; 
   c.elem[1] = (void*) &s_seperator;

   run_func( &data<type>::get_user_list_, (void*)&c );
 }
 static void get_user_list_( name* name_obj, void* v_arg )
 {
   container *c = (container*) v_arg; 
   type *type_obj = static_cast<type*>(name_obj);

   type_obj -> get_user_list( *((string*)c->elem[0]), *((string*)c->elem[1]) ); 
 } 
};

#endif
