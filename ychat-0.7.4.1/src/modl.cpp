#ifndef MODL_CPP 
#define MODL_CPP

#include <limits.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

#include "modl.h"
#include "tool/dir.h"

using namespace std;

modl::modl() : smap<dynmod*,string>(HMAPOCC)
{
#ifdef NCURSES
    print_cached( 0 );
#endif

    if ( wrap::CONF->get_elem( "httpd.modules.preloadcommands" ).compare( "true" ) == 0 )
     preload_modules( wrap::CONF->get_elem("httpd.modules.commandsdir") );

    if ( wrap::CONF->get_elem( "httpd.modules.preloadhtml" ).compare( "true" ) == 0 )
     preload_modules( wrap::CONF->get_elem("httpd.modules.htmldir") );
}

modl::~modl()
{
    // dlclose all the_module's first!
    run_func( &modl::dlclose_ );

    // then clean the hash map.
    unload_modules(); 
}

void
modl::preload_modules( string s_path )
{
 dir* p_dir = new dir();
 p_dir->open_dir( s_path );

 p_dir->read_dir();
 
 vector<string> dir_vec = p_dir->get_dir_vec();

 if ( ! dir_vec.empty() ) 
 {
  vector<string>::iterator iter = dir_vec.begin();

  do
  {
   if ( iter->length() >= 3 && iter->compare( iter->length()-3, 3, ".so" ) == 0 )
    cache_module( s_path + *iter, false );
  }
  while ( ++iter != dir_vec.end() );
 }

 dir_vec.clear();

 // This also closes the dir.
 delete p_dir;
}

void
modl::dlclose_( dynmod* mod )
{
    dlclose( mod->the_module );
    free   ( mod );
}

dynmod*
modl::cache_module( string s_name, bool b_print_sys_msg )
{
    void     *the_module = NULL;
    function *the_func   = NULL;

    the_module = dlopen( s_name.c_str(), RTLD_LAZY );
    //the_module = dlopen( s_name.c_str(), RTLD_NOW );

    if ( the_module == NULL )
    {
        wrap::system_message( dlerror() );
        return NULL;
    }

    the_func = (function*) dlsym( the_module, "extern_function" );

    if ( the_func == NULL )
    {
        wrap::system_message( dlerror() );
        return NULL;
    }

    if ( b_print_sys_msg )
     wrap::system_message( MODULEC + s_name.substr( s_name.find_last_of("/")+1 ) ); 

    dynmod *mod     = new dynmod; // encapsulates the function and module handler.
    mod->the_func   = the_func  ; // the function to execute
    mod->the_module = the_module; // the module handler to close if function

    // is not needed anymore.
    add_elem( mod, s_name  );

    // DO NOT CLOSE AS LONG THERE EXIST A POINTER TO THE FUNCTION
    // dlclose( module ); will be called in modl::~modl()!

#ifdef NCURSES
    print_cached( get_size() ); 
#endif
    
    return mod;
}

dynmod*
modl::get_module( string s_name )
{
    wrap::system_message( MODULER + s_name.substr( s_name.find_last_of("/")+1 ) ); 
    dynmod* mod = get_elem( s_name );
    return ! mod ? cache_module( s_name, true ) : mod;
}

void
modl::unload_modules()
{
    wrap::system_message( MODUNLO );

    // dlclose all the_module's first!
    run_func( &modl::dlclose_ );
    
    // then clean the hash map.
    make_empty();

#ifdef NCURSES
    print_cached( get_size() ); 
#endif
    
}

void
modl::reload_modules()
{
    unload_modules();

    wrap::system_message( MODRELO );
    preload_modules( wrap::CONF->get_elem("httpd.modules.commandsdir") );
    preload_modules( wrap::CONF->get_elem("httpd.modules.htmldir") );
}

#ifdef NCURSES
void
modl::print_cached( int i_mods )
{
    if ( !wrap::NCUR->is_ready() )
      return;

    mvprintw( NCUR_CACHED_MODS_X, NCUR_CACHED_MODS_Y, "Mods: %d ", i_mods);
    refresh();
}
#endif

#endif
