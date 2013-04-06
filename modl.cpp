// class modl implementation.

#ifndef MODL_CXX
#define MODL_CXX

#include <limits.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

#include "s_mutx.h"
#include "modl.h"

using namespace std;

modl::modl(  )
{
 map_mods = new hmap<dynmod*,string>(80);
 pthread_mutex_init( &mut_map_mods, NULL );
}

modl::~modl()
{
 pthread_mutex_lock   ( &mut_map_mods );

 // dlclose all the_module's first! 
 map_mods->run_func   ( &modl::dlclose_ ); 

 // then clean the hash map.
 map_mods->make_empty (               );

 pthread_mutex_unlock ( &mut_map_mods );
 pthread_mutex_destroy( &mut_map_mods );
}

void
modl::dlclose_( dynmod* mod )
{
 dlclose( mod->the_module ); 
}

dynmod*
modl::cache_module( string s_name )
{
 void     *the_module = NULL;
 function *the_func   = NULL;

 the_module = dlopen( s_name.c_str(), RTLD_NOW );

 if ( the_module == NULL )
 {
  pthread_mutex_lock  ( &s_mutx::get().mut_stdout );
  cerr << "dlerror: " << dlerror() << endl;
  pthread_mutex_unlock( &s_mutx::get().mut_stdout );
  return NULL;
 }

 the_func = (function*) dlsym( the_module, "extern_function" );

 if ( the_func == NULL )
 {
  pthread_mutex_lock  ( &s_mutx::get().mut_stdout );
  cerr << "dlerror: " << dlerror() << endl; 
  pthread_mutex_unlock( &s_mutx::get().mut_stdout );
  return NULL;
 }

#ifdef VERBOSE
 pthread_mutex_lock  ( &s_mutx::get().mut_stdout );
 cout << MODULEC << s_name << endl;
 pthread_mutex_unlock( &s_mutx::get().mut_stdout );
#endif

 dynmod *mod     = new dynmod; // encapsulates the function and module handler.
 mod->the_func   = the_func  ; // the function to execute
 mod->the_module = the_module; // the module handler to close if function 
                               // is not needed anymore. 
 pthread_mutex_lock  ( &mut_map_mods );
 map_mods->add_elem  ( mod, s_name  );
 pthread_mutex_unlock( &mut_map_mods );

// DO NOT CLOSE AS LONG THERE EXIST A POINTER TO THE FUNCTION 
// dlclose( module );

 return mod; 
}

dynmod*
modl::get_module( string s_name )
{
 pthread_mutex_lock  ( &mut_map_mods );
 dynmod* mod = map_mods->get_elem( s_name );
 pthread_mutex_unlock( &mut_map_mods );

 return ! mod ? cache_module( s_name ) : mod;
}

#endif
