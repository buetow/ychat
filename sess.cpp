#ifndef SESS_CPP
#define SESS_CPP

#include "sess.h"

sess::sess( string s_id )
{ 
	this->sess_id=s_id;
}

string sess::getId(){ return this->sess_id; }


void sess::invalidate() {
	this->sess_id="0";
	this->sess_values.clear();
}

void sess::setValue( string s_key, void *lpvalue )
{
	this->sess_values[s_key]=lpvalue;
}
void *sess::getValue( string s_key )
{
	return this->sess_values[s_key];
}
string sess::dump()
{
	string s_ret=string("Session Dump of Session ") + this->getId();
	map<string, void*>::const_iterator it;
	for(it=this->sess_values.begin();it!=this->sess_values.end();it++)
		s_ret=s_ret + "\nkey: " + it->first;
	return s_ret;
}
#endif
