/*:*
 *: File: ./src/chat/sman.cpp
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.8.3-CURRENT
 *: 
 *: Copyright (C) 2003 Paul C. Buetow, Volker Richter
 *: Copyright (C) 2004 Paul C. Buetow
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

#ifndef SMAN_CPP
#define SMAN_CPP

#include "sman.h"
#include "../tool/tool.h"
#include "../maps/mtools.h"
#include "../contrib/crypt/md5.h"

sman::sman()
{
  i_continous_session_count = i_session_count = 0;
  pthread_mutex_init( &mut_i_session_count, NULL );
}

sman::~sman()
{
  // Delete each session object of the shashmap!
  shashmap<sess*>::run_func( mtools<sess*>::delete_obj );
  pthread_mutex_destroy( &mut_i_session_count );
}

string sman::generate_id( int i_len )
{
  string s_valid = wrap::CONF->get_elem("chat.session.validchars");
  string s_ret = "";

  srand(time(0)+tool::string2int(wrap::CONF->get_elem("chat.session.kloakkey")));
  int i_char;


  for (int i = 0; i < i_len; i++)
  {
    i_char = rand() % s_valid.length();
    s_ret += s_valid[i_char];
  }

  if ( wrap::CONF->get_elem("chat.session.md5hash") == "true" )
  {
    string s_salt = wrap::CONF->get_elem("chat.session.md5salt");
    string s_hash(md5::MD5Crypt(s_ret.c_str(), s_salt.c_str()));
    s_ret.append(s_hash.substr(s_ret.find(s_salt) + s_salt.length() + 3));
  }

  // Prove, if the TempID already exists
  sess* p_sess = get_elem(s_ret);

  if (p_sess) 
  {
    wrap::system_message(SESSEXI);
    return generate_id(i_len);
  }

  return s_ret;
}

sess *sman::create_session( )
{
  string s_tmpid = generate_id( tool::string2int( wrap::CONF->get_elem( "chat.session.length" ) ) );

  sess* p_sess = new sess( s_tmpid );

  pthread_mutex_lock( &mut_i_session_count );
  i_session_count++;
  wrap::system_message(string(SESSIOC) + "(" +
		tool::int2string(++i_continous_session_count) + "," + 
		tool::int2string(reinterpret_cast<int>(p_sess)) + ")" );

  pthread_mutex_unlock( &mut_i_session_count );

  //????
  add_elem( p_sess, s_tmpid );

  return p_sess;
}

sess *sman::get_session( string s_id )
{
  return get_elem( s_id );
}

void
sman::destroy_session( string s_id )
{
  sess* p_sess = get_elem(s_id);

  pthread_mutex_lock( &mut_i_session_count );
  i_session_count--;
#ifdef NCURSES

  wrap::system_message(string(SESSIOD) + "(" +
		tool::int2string(i_continous_session_count) + "," + 
		tool::int2string(reinterpret_cast<int>(p_sess)) + ")" );

  pthread_mutex_unlock( &mut_i_session_count );

  sess* p_sess = get_elem( s_id );
  del_elem( s_id );
  delete p_sess;
}

#ifdef NCURSES
void sman::print_sessions()
{
  if ( wrap::NCUR->is_ready() )
  {
    mvprintw( NCUR_SESSION_X,NCUR_SESSION_Y, "Sessions: %d ", i_session_count);
    refresh();
  }
}

#ifdef NCURSES
void sman::print_init_ncurses()
{
  pthread_mutex_lock( &mut_i_session_count );
  print_sessions();
  pthread_mutex_unlock( &mut_i_session_count );
}
#endif

int
sman::get_session_count()
{
  pthread_mutex_lock( &mut_i_session_count );
  int i_ret = i_session_count;
  pthread_mutex_unlock( &mut_i_session_count );
  return i_ret;
}

/*
void
sman::dump() {
  shashmap<sess*>::dump(); 
  cout << "BLA" << endl;
}
*/

#endif
