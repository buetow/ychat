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
 dir_vec.clear();
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

 dp = opendir( s_dir.c_str() );

 if ( dp == NULL )
  return false; // Could not open dir.

 b_open = true;

 return true; // Could open dir with success.
}

void
dir::close_dir()
{
 if ( b_open && dp != NULL )
 {
  closedir( dp );
  b_open = false;
 }
}

void
dir::read_dir()
{
 if ( dp != NULL )
  while( ep = readdir( dp ) )
   dir_vec.push_back( string( ep->d_name ) ); 
}

vector<string> 
dir::get_dir_vec()
{
 return dir_vec;
}

#endif
