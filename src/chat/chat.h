// class chat declaration.

class chat;

#ifndef CHAT_H
#define CHAT_H

#include <vector>
#include "../incl.h"
#include "base.h"
#include "perm.h"
#include "room.h"
#include "user.h"
#include "sess.h"
#include "../tool/tool.h"

using namespace std;


#undef chat
class chat : public base<room>, public perm
{
private:
    map_string map_replace_strings; 
    vector<string> vec_replace_keys;

public:
    virtual room* get_room( string s_name )
    {
        bool b_bool;
        return get_room( s_name, b_bool );
    }

    virtual room* get_room( string &s_name, bool &b_found )
    {
        return static_cast<room*>( get_elem( s_name, b_found ) );
    }

    // public methods:
    explicit chat();  // a standard constructor.
    ~chat();          // destructor.

    // get the object of a specific user.
    user* get_user( string &s_nick );
    user* get_user( string &s_nick, bool &b_found );
    static void get_user_( room* room_obj, void *v_arg   );

    // will be called every time a user tries to login.
    void login( map_string &map_params );

    // will be called if a user posts a message.
    void post ( user* u_user, map_string &map_params );

    void del_elem( string s_name )
    {
     smap<room*,string>::del_elem( s_name ); 
    }

    void reconf();	
};
#endif
