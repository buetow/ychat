#ifndef DIR_CPP
#define DIR_CPP

#include "dir.h"

using namespace std;

dir::dir()
{
 b_open = false;
}

dir::~dir()
{
 vec_dir.clear();
 close_dir();
}

bool
dir::open_dir( char *c_dir )
{
 string s_dir( c_dir );
 return open_dir( s_dir  );
}

bool
dir::open_dir( string &s_dir )
{
 if ( b_open )
  return false;

 p_d = opendir( s_dir.c_str() );

 if ( p_d == NULL )
  return false; // Could not open dir.

 b_open = true;

 return true; // Could open dir with success.
}

void
dir::close_dir()
{
 if ( b_open && p_d != NULL )
 {
  closedir( p_d );
  b_open = false;
 }
}

void
dir::read_dir()
{
 if ( p_d != NULL )
  while( p_ep = readdir( p_d ) )
   vec_dir.push_back( string( p_ep->d_name ) ); 
}

vector<string> 
dir::get_dir_vec()
{
 return vec_dir;
}

#endif
