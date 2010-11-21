/*:*
 *: File: ./src/user.h
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

// class user declaration.
#ifndef USER_H
#define USER_H

#include "incl.h"
#include "hmap.h"
#include "name.h"
#include "s_lang.h"
using namespace std;

class room;

class user : public name
{
private:
    // private members:
    bool   b_away;   // true if user is away.
    bool   b_online; // true if user is online.
    int    i_sock;   // user's stream socket descriptor.
    long   l_time;   // user's last activity time.
    rang   r_rang;   // user's rang ( see enum rang @ globals.h ).
    rang   r_oldr;   // user's previous rang.
    string s_id;
    string s_agnt;   // user's http user agent.
    string s_away;   // user's last away message.
    string s_col1;   // user's nick color.
    string s_mess;   // message string which has to be sent to the user.
    room*  p_room;   // pointer to the user's room.

    pthread_mutex_t mut_b_online;
    pthread_mutex_t mut_i_sock;
    pthread_mutex_t mut_l_time;
    pthread_mutex_t mut_s_mess;
    pthread_mutex_t mut_p_room;

public:
    pthread_cond_t  cond_message;
    pthread_mutex_t mut_message;

    // small inline methods:
    string get_col1()         const
    {
        return  s_col1;
    }
    string get_id()	   const
    {
        return  s_id;
    }
    void   set_id    ( string s_id   )
    {
        this -> s_id = s_id;
    }
    void   set_col1  ( string s_col1 )
    {
        this -> s_col1 = s_col1;
    }

    rang   get_rang  ( )      const
    {
        return  r_rang;
    }
    void   set_rang  ( rang   r_rang )
    {
        r_oldr = this -> r_rang;
        this -> r_rang = r_rang;
    }

    bool   new_msgs  ( )
    {
        return s_mess.empty();
    }
    // public methods:
    explicit user( string s_name );      // a standard constructor.
    ~user();			      // user destructor.

    // gets specific data of this user und stores it in
    // (*p_map_string)["nick"]. this method will be used
    // every time data has to be got from every user of a room
    // or even of the system.
    virtual void get_data( map_string *p_map_data );

    virtual bool  get_online();
    virtual void  set_online( bool b_online );
    virtual room* get_p_room();
    virtual void  set_p_room( room* p_room );
    virtual int   get_sock  ( );
    virtual void  set_sock  ( int i_sock );

    // gets the message and clears s_mess;
    virtual string get_mess();

    // actualizes the user's timestamp.
    virtual void  renew_stamp();

    // Here are starting methods which are mainly needed by the data<type> class.

    // appends a string to s_mess including br.
    virtual void msg_post( string *p_msg );
    virtual void get_user_list( string &s_list, string &s_seperator );
};

#endif
