// class name implementation.

#ifndef NAME_CXX
#define NAME_CXX

#include "name.h"

using namespace std;

name::name( string s_name )
{
    set_name( s_name );
}

name::~name()
{}

string
name::get_name() const
{
    return s_name;
}

void
name::set_name( string s_name )
{
    this->s_name = s_name;
}


#endif
