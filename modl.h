// class modl declaration.

#ifndef MODL_H
#define MODL_H

#include "incl.h"
#include "hmap.h"

using namespace std;


class modl 
{
private:
 hmap<dynmod*,string>* map_mods;
 pthread_mutex_t mut_map_mods;

 static  void dlclose_( dynmod* mod   );
 dynmod* cache_module ( string s_name );

public:
 modl();
 ~modl();

 dynmod* get_module  ( string s_name );

};

#endif
