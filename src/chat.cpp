/*:*
 *: File: ./src/chat.cpp
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.5.6-BASIC
 *: 
 *: Copyright (C) 2003, 2004 Paul C. Buetow, Volker Richter
 *: Copyright (C) 2005 EXA Digital Solutions GbR
 *: 
 *: This program is free software; you can redistribute it and/or
 *: modify it under the terms of the GNU General Public License
 *: as published by the Free Software Foundation; either version 2
 *: of the License, or (at your option) any later version.
 *: 
 *: This program is distributed in the hope that it will be useful,
 *: but WITHOUT ANY WARRANTY; without even the implied warranty of
 *: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *: GNU General Public License for more details.
 *: 
 *: You should have received a copy of the GNU General Public License
 *: along with this program; if not, write to the Free Software
 *: Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *:*/

// class chat implementation.

#ifndef s_chat_CXX
#define s_chat_CXX

#include "chat.h"
#include "s_conf.h"
#include "s_mutx.h"
#include "s_tool.h"

using namespace std;

chat::chat( )
{
    if ( s_conf::get
                ().get_val( "HTML" ) == "OFF" )
            b_strip_html = true;
    else
        b_strip_html = false;

}

chat::~chat( )
{}

user*
chat::get_user( string &s_user )
{
    bool b_flag;
    return get_user( s_user, b_flag );
}

user*
chat::get_user( string &s_user, bool &b_found )
{
    container param;

    param.elem[0] = (void*) &s_user ;
    param.elem[1] = (void*) &b_found;

    b_found = false;

    run_func( get_user_, (void*)&param );

    if ( *( (bool*)param.elem[1] ) )
        return (user*)param.elem[2];
}

void
chat::get_user_( room *room_obj, void *v_arg )
{
    container* param = (container*) v_arg;
    param->elem[2] = (void*)room_obj->get_elem( *((string*)param->elem[0]), *((bool*)param->elem[1]) );
}

void
chat::login( map_string &map_params )
{
    string s_user = map_params["nick"];

    // prove if nick is empty:
    if ( s_user.empty() )
    {
        map_params["INFO"]    = E_NONICK;
        map_params["request"] = s_conf::get
                                    ().get_val( "STARTMPL" ); // redirect to the startpage.
        return;
    }

    // prove if the nick ist alphanumeric:
    else if ( ! s_tool::is_alpha_numeric( s_user ) )
    {
        map_params["INFO"]    = E_ALPNUM;
        map_params["request"] = s_conf::get
                                    ().get_val( "STARTMPL" ); // redirect to the startpage.
        return;
    }

    bool b_flag;

    // prove if nick is already online / logged in.
    get_user( s_user, b_flag );
    if ( b_flag )
    {
        map_params["INFO"]    = E_ONLINE;
        map_params["request"] = s_conf::get
                                    ().get_val( "STARTMPL" );
        return;
    }

    string s_room = map_params["room"];
    room*  p_room = get_room( s_room , b_flag );

    // if room does not exist add room to list!
    if ( ! b_flag )
    {
        p_room = new room( s_room );

#ifdef VERBOSE

        pthread_mutex_lock  ( &s_mutx::get
                                  ().mut_stdout );
        cout << NEWROOM << s_room << endl;
        pthread_mutex_unlock( &s_mutx::get
                                  ().mut_stdout );
#endif

        add_elem( p_room );
    }

    user *p_user = new user( s_user );

    // add user to the room.
    p_room->add_user( p_user );
    sess *ns =s_sman::get
                  ().createSession();
    ns->setValue(string("nick"), (void *)new string(s_user) );
    map_params["tmpid"]=ns->getId();
    // post "username enters the chat" into the room.
    p_room->msg_post( new string( s_user.append( s_lang::get
                                      ().get_val( "USERENTR" ) ) ) );

#ifdef VERBOSE

    pthread_mutex_lock  ( &s_mutx::get
                              ().mut_stdout );
    cout << LOGINPR << s_user << endl;
    pthread_mutex_unlock( &s_mutx::get
                              ().mut_stdout );
#endif
}

void
chat::post( user* p_user, map_string &map_params )
{

    string s_msg( map_params["message"] );

    auto unsigned i_pos = s_msg.find( "/" );

    if ( b_strip_html )
        s_tool::strip_html( &s_msg );

    string s_post( "<font color=\""   );

    s_post.append( p_user->get_col1() )
    .append( "\">"              )
    .append( p_user->get_name() )
    .append( ": "               )
    .append( s_msg              )
    .append( "</font><br>\n"    );

    p_user->get_p_room()->msg_post( &s_post );
}

#endif
