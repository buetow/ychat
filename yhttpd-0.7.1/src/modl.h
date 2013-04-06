#include "incl.h"

#ifndef MODL_H
#define MODL_H

#include "maps/smap.h"

using namespace std;

class modl : private smap<dynmod*,string>
{
private:
    static  void dlclose_( dynmod* mod   );
    dynmod* cache_module ( string s_name );
    void preload_modules( string s_path );

#ifdef NCURSES
    void print_cached( int i_mods );
#endif

public:
    modl();
    ~modl();

    dynmod* get_module( string s_name );

    vector<string>* get_mod_vector()
    {
     vector<string>* p_ret = get_key_vector(); 
     return p_ret;
    }

    void unload_modules(); 
    void reload_modules(); 
};

#endif
