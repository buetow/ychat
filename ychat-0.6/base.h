// template class data declaration;

#ifndef BASE_H
#define BASE_H

#include "incl.h"
#include "hmap.h"

template<class type>
class base
{
private:
    hmap<type*,string>* map_elem;
    pthread_mutex_t mut_map_elem;

public:
    base();
    ~base();

    virtual void  add_elem( type*   p_type );                // add a element.
    virtual void  del_elem( string &s_name );                // delete a alement.
    virtual type* get_elem( string &s_name, bool &b_found ); // get a element.

    // execute func on all elements of map_elem. v_pointer is the argument.
    virtual void  run_func( void (*func)(type*, void*), void* v_arg );

    // chat::msg_post sends to all users of the system a message.
    // room::msg_post sends to all users of the room a message.
    // user::msg_post sends to the user a message.
    void msg_post( string *s_msg )
    {
        run_func( &base<type>::msg_post_ , (void*)s_msg );
    }
    static void msg_post_( type* type_obj, void* v_arg )
    {
        string *p_msg    = (string*) v_arg;
        type_obj -> msg_post( p_msg );
    }

    void get_data( map_string *p_map_string )
    {
        run_func( &base<type>::get_data_ , (void*)p_map_string );
    }
    static void get_data_( type* type_obj, void* v_arg )
    {
        map_string *map_params = (map_string*) v_arg;
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

        run_func( &base<type>::get_user_list_, (void*)&c );
    }
    static void get_user_list_( type* type_obj, void* v_arg )
    {
        container *c = (container*) v_arg;
        type_obj -> get_user_list( *((string*)c->elem[0]), *((string*)c->elem[1]) );
    }
};

#include "base.cpp"

#endif
