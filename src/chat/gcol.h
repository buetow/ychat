// garbage collector
#include "../incl.h"

#ifndef GCOL_H
#define GCOL_H

#include <vector>
#include "room.h"
#include "user.h"
#include "../maps/shashmap.h"

using namespace std;

class gcol
{
private:
  vector<room*> vec_rooms;
  shashmap<user*>* p_map_users;

  pthread_mutex_t mut_vec_rooms;

  static void delete_users_( user* user_obj  );

#ifdef NCURSES

  void print_garbage( );
#endif

public:
  gcol();
  ~gcol();

  bool remove_garbage();
  void add_room_to_garbage( room* p_room );
  void add_user_to_garbage( user* p_user );
  virtual room* get_room_from_garbage();
  virtual room* get_room_from_garbage_or_new( string s_room );
  user* get_user_from_garbage( string s_user );
  virtual void lock_mutex();
  virtual void unlock_mutex();
};

#endif
