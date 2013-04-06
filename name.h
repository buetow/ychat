// class name declaration.

#ifndef NAME_H
#define NAME_H

#include "incl.h"

using namespace std;

class name
{
protected:
    // private members:
    string s_name; // object's name.

public:
    virtual string get_name  ( ) const;
    virtual void   set_name  ( string s_name );


    // public methods:
    explicit name( )
    { }
    ;      	      // a standard constructor.
    explicit name( string s_name );      // a standard constructor.
    ~name( );

};

#endif
