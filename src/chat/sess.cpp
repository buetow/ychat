#ifndef SESS_CPP
#define SESS_CPP

#include "sess.h"

sess::sess( string s_tmpid )
{
    this->p_user = NULL; 
    this->s_tmpid = s_tmpid;
}

sess::~sess()
{
}

string
sess::get_tmpid()
{
    return s_tmpid;
}

string
sess::get_name()
{
    if ( p_user = NULL )
	return "";

    return p_user->get_name();
}

void
sess::set_name(string s_name)
{
    if ( p_user != NULL )
	p_user->set_name(s_name);
}

user*
sess::get_user()
{
    return p_user;
}

void
sess::set_user(user* p_user)
{
    this->p_user = p_user;
}


void
sess::invalidate()
{
    this->s_tmpid = "0";
}

#endif
