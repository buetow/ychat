// class modl implementation.

#ifndef MODL_CPP
#define MODL_CPP

#include <limits.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

#include "s_conf.h"
#include "s_mutx.h"
#include "s_ncur.h"
#include "modl.h"
#include "dir.h"

using namespace std;

modl::modl(  )
{
    map_mods = new hmap<dynmod*,string>(80);
    pthread_mutex_init( &mut_map_mods, NULL );

    if ( s_conf::get().get_val( "PRE_MODS_COMMANDS" ).compare( "ON" ) == 0 )
     preload_modules( new string("mods/commands/") );

    if ( s_conf::get().get_val( "PRE_MODS_HTML" ).compare( "ON" ) == 0 )
     preload_modules( new string("mods/html/") );
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
modl::preload_modules( string *p_path )
{
 dir* p_dir = new dir();
 p_dir->open_dir( *p_path );
 p_dir->read_dir();

 vector<string>* p_dir_vec = p_dir->get_dir_vec();

 if ( ! p_dir_vec->empty() ) 
 {
  vector<string>::iterator iter = p_dir_vec->begin();

  do
  {
   if ( iter->length() >= 3 && iter->compare( iter->length()-3, 3, ".so" ) == 0 )
    cache_module( *p_path + *iter );
  }
  while ( ++iter != p_dir_vec->end() );
 }
 p_dir->close_dir();
 p_dir->~dir();
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
        pthread_mutex_lock  ( &s_mutx::get
                                  ().mut_stdout );
#ifdef NCURSES

        s_ncur::get
            ().print( dlerror() );
#else

        cerr << "dlerror: " << dlerror() << endl;
#endif

        pthread_mutex_unlock( &s_mutx::get
                                  ().mut_stdout );
        return NULL;
    }

    the_func = (function*) dlsym( the_module, "extern_function" );

    if ( the_func == NULL )
    {
        pthread_mutex_lock  ( &s_mutx::get
                                  ().mut_stdout );
#ifdef NCURSES

        s_ncur::get
            ().print( dlerror() );
#else

        cerr << "dlerror: " << dlerror() << endl;
#endif

        pthread_mutex_unlock( &s_mutx::get
                                  ().mut_stdout );
        return NULL;
    }

#ifdef VERBOSE
    pthread_mutex_lock  ( &s_mutx::get
                              ().mut_stdout );
    cout << MODULEC << s_name << endl;
    pthread_mutex_unlock( &s_mutx::get
                              ().mut_stdout );
#endif
#ifdef NCURSES

    {
        string s_tmp( MODULEC );
        s_tmp.append( s_name );
        s_ncur::get
            ().print( s_tmp.c_str() );
    }
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
#ifdef VERBOSE
    pthread_mutex_lock  ( &s_mutx::get
                              ().mut_stdout );
    cout << MODULER << s_name << endl;
    pthread_mutex_unlock( &s_mutx::get
                              ().mut_stdout );
#endif
#ifdef NCURSES

    {
        string s_tmp( MODULER );
        s_tmp.append( s_name );
        s_ncur::get
            ().print( s_tmp.c_str() );
    }
#endif
    pthread_mutex_lock  ( &mut_map_mods );
    dynmod* mod = map_mods->get_elem( s_name );
    pthread_mutex_unlock( &mut_map_mods );

    return ! mod ? cache_module( s_name ) : mod;
}

#endif
