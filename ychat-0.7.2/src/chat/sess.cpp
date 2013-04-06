#ifndef SESS_CPP
#define SESS_CPP

#include "sess.h"

sess::sess( string s_id )
{
    this->sess_id=s_id;
}

sess::~sess()
{
    
}

string
sess::get_id()
{
    return this->sess_id;
}


void
sess::invalidate()
{
    this->sess_id = "0";
}

#endif
