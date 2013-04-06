#ifndef GCOL_CPP
#define GCOL_CPP

#include "gcol.h"

using namespace std;

gcol::gcol()
{
  pthread_mutex_init( &mut_vec_rooms  , NULL);

  p_map_users = new shashmap<user*>;
  wrap::system_message( GARBAGE );

#ifdef NCURSES

  print_garbage();
#endif
}

gcol::~gcol()
{
  remove_garbage();
  pthread_mutex_destroy( &mut_vec_rooms );
  delete p_map_users;
}

void
gcol::add_room_to_garbage( room* p_room )
{
  pthread_mutex_lock  ( &mut_vec_rooms );
  vec_rooms.push_back( p_room );
  pthread_mutex_unlock( &mut_vec_rooms );
  wrap::system_message( GARROOM + p_room->get_name() );
#ifdef NCURSES

  print_garbage();
#endif
}

void
gcol::add_user_to_garbage( user* p_user )
{
  p_user->s_mess_delete();
  p_map_users->add_elem( p_user, tool::to_lower(p_user->get_name()) );
  wrap::system_message( GARUSER + p_user->get_name() );
  p_user->destroy_session();

#ifdef NCURSES

  print_garbage();
#endif
}

bool
gcol::remove_garbage()
{
  bool b_empty;

  pthread_mutex_lock  ( &mut_vec_rooms );
  b_empty  = ( vec_rooms.empty() && p_map_users->size() == 0);
  pthread_mutex_unlock( &mut_vec_rooms );

  if ( b_empty )
    return false;

  wrap::system_message( GARBACT );

  pthread_mutex_lock  ( &mut_vec_rooms );
  for ( vector<room*>::iterator iter = vec_rooms.begin();
        iter != vec_rooms.end(); iter++ )
  {
    wrap::system_message( REMROOM + (*iter)->get_name() );
    delete *iter;
  }
  vec_rooms.clear();
  pthread_mutex_unlock( &mut_vec_rooms );


  p_map_users->run_func( delete_users_ );
  p_map_users->clear();

#ifdef NCURSES

  print_garbage();
#endif

  return true;
}

room*
gcol::get_room_from_garbage()
{
  pthread_mutex_lock  ( &mut_vec_rooms );

  if ( vec_rooms.empty() )
  {
    pthread_mutex_unlock( &mut_vec_rooms );
    return NULL;
  }


  room* p_room = vec_rooms.back();
  vec_rooms.pop_back();
  pthread_mutex_unlock( &mut_vec_rooms );

#ifdef NCURSES

  print_garbage();
#endif

  return p_room;
}

room*
gcol::get_room_from_garbage_or_new( string s_room )
{
  room* p_room = get_room_from_garbage();
  if ( p_room == NULL )
    return new room( s_room );
  p_room->set_name( s_room );
  return p_room;
}

user*
gcol::get_user_from_garbage( string s_user )
{

  user* p_user = p_map_users->get_elem( tool::to_lower(s_user) );
  ;

  if ( p_user != NULL )
  {
    p_map_users->del_elem( tool::to_lower(s_user) );
    p_user->set_name( s_user );
    p_user->set_online( true );
    p_user->set_invisible( false );
    p_user->renew_timeout();
    wrap::system_message(GARUSE2 + p_user->get_name() );
#ifdef NCURSES

    print_garbage();
#endif

  }

  return p_user;
}

void
gcol::delete_users_( user *user_obj )
{
  wrap::system_message( REMUSER + user_obj->get_name() );
  user_obj->clean();
  delete user_obj;
}

void
gcol::lock_mutex()
{
  pthread_mutex_lock  ( &mut_vec_rooms );
}

void
gcol::unlock_mutex()
{
  pthread_mutex_unlock ( &mut_vec_rooms );
}

#ifdef NCURSES
void
gcol::print_garbage( )
{
  if ( wrap::NCUR->is_ready() )
  {
    pthread_mutex_lock  ( &mut_vec_rooms );
    mvprintw( NCUR_GARBAGE_X,NCUR_GARBAGE_Y, "Garbage: %d ", p_map_users->size() + vec_rooms.size() );
    pthread_mutex_unlock( &mut_vec_rooms );
    refresh();
  }
}

#endif

#endif
