// class conf implementation.

#ifndef s_conf_CXX
#define s_conf_CXX

#include <fstream>
#include "conf.h"

using namespace std;

conf::conf( string s_conf = CONFILE ) : name( s_conf )
{
 parse( ); // parse the config file. 
}

conf::~conf()
{
}

void
conf::parse()
{
#ifdef VERBOSE
 cout << CFILEOK << get_name() << endl;
#endif

 ifstream fs_conf( get_name().c_str() );

 if ( ! fs_conf ) 
 {
#ifdef VERBOSE
 cout << CFILENO << get_name() << endl;
#endif
  return;
 }

 char c_buf[READBUF];

 while( fs_conf.getline( c_buf, READBUF ) ) 
 {
  string s_token( c_buf );
  unsigned int ui_pos = s_token.find( "#", 0 );
   
  // if line is commented out:
  if ( ui_pos == 0 ) 
   continue;

  ui_pos = s_token.find( ";", 0 );

  // if token has not been found.
  if ( ui_pos == string::npos )
   continue;

  s_token = s_token.substr( 0   , --ui_pos );
  ui_pos  = s_token.find  ( "\"", 0        );   

  if ( ui_pos == string::npos )
   continue;

  string s_val = s_token.substr( ui_pos+1, s_token.length() );
  string s_key = s_token.substr( 0      , --ui_pos          ); 

#ifdef VERBOSE2
   cout << s_key << "=" << s_val << endl;
#endif

  // fill the map.
  map_vals[s_key] = s_val;
 } 

 fs_conf.close();
 fs_conf.~ifstream();
}

#endif
