#include "incl.h"

#ifndef MODL_H
#define MODL_H

#include "maps/shashmap.h"

using namespace std;

class modl : public shashmap<dynmod*>
{
private:
  static  void dlclose_( dynmod* mod   );
  dynmod* cache_module ( string s_name, bool b_print_sys_msg );
  void preload_modules( string s_path );

#ifdef NCURSES

  void print_cached( int i_mods );
#endif

public:
  modl();
  ~modl();

  dynmod* get_module( string s_name );
  dynmod* get_module( string s_name, string s_user );

  vector<string>* get_mod_vector()
  {
    vector<string>* p_ret = get_key_vector();
    return p_ret;
  }

  void unload_modules();
  void reload_modules();
};

#endif
