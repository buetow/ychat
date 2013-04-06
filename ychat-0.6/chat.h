// class chat declaration.

#ifndef s_chat_H
#define s_chat_H

#include <vector>
#include "incl.h"
#include "base.h"
#include "room.h"
#include "user.h"
#include "sess.h"
#include "s_lang.h"
#include "s_sman.h"

using namespace std;

class chat : public base<room>
{
private:
    bool b_strip_html;

public:


    room* get_room( string &s_name, bool &b_found )
    {
        return static_cast<room*>( get_elem( s_name, b_found ) );
    }

    // public methods:
    explicit chat();  // a standard constructor.
    ~chat();          // destructor.

    // get the object of a specific user.
    virtual user* get_user( string &s_nick );
    virtual user* get_user( string &s_nick, bool &b_found );
    static  void get_user_( room* room_obj, void *v_arg   );

    // will be called every time a user tries to login.
    virtual void login( map_string &map_params );

    // will be called if a user posts a message.
    virtual void post ( user* u_user, map_string &map_params );
};

#endif
