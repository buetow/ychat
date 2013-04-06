#ifndef SMAN_CXX
#define SMAN_CXX

#include "sman.h"

sman::sman()
{
	this->sessions=new hmap<sess *, string>(80);
	this->sessioncount=0;
}
sman::~sman()
{
	delete this->sessions;
}
string sman::generateId( int len )
{
	string valid_chars="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_";
	string s_ret="";
	srand(time(0)+160682);
	for(int i=0;i<len;i++)
	{
		int i_char=rand() % 64;
		s_ret+=valid_chars[i_char];
	}
	return s_ret;
}
sess *sman::createSession( )
{
	string new_id=this->generateId(s_tool::string2int( s_conf::get().get_val( "SESSION_LENGTH" ) ) ); 

	sess *new_sess= new sess( new_id  );

	this->sessioncount++;
	this->sessions->add_elem( new_sess, new_id );
	
	return new_sess;
}

sess *sman::getSession( string s_id )
{
	return this->sessions->get_elem( s_id );
}
void sman::destroySession( string s_id )
{
	this->sessioncount--;
	this->sessions->del_elem( s_id );
}
#endif
