// class cont implementation.

#ifndef CONT_CXX
#define CONT_CXX

#include "cont.h"
#include "s_mutx.h"

using namespace std;

string
cont::get_val( string s_key )
{
 if ( map_vals.find( s_key ) != map_vals.end() )
  return map_vals[ s_key ];
 return string();
}

cont::~cont()
{
 map_vals.~map_string();
}

#endif
