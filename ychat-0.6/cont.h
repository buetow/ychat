// class cont declaration. defines a simple data container class.

#ifndef CONT_H
#define CONT_H

#include "incl.h"
#include "hmap.h"

using namespace std;

class cont
{
protected:
    map_string map_vals;

public:
    cont::~cont();

    // small inline methods:
    void clear_vals()
    {
        map_vals.clear();
    } // removes all values.

    // public methods:
    virtual string get_val( string s_key ); // get a specific map_vals value.
};

#endif
