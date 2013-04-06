#ifndef SMAN_H
#define SMAN_H

#include "incl.h"
#include "hmap.h"
#include "sess.h"
#include "s_tool.h"
#include "s_conf.h"
#include <cstdlib>

using namespace std;

class sman{

	private:
		hmap<sess *, string> *sessions;
		string generateId( int len );
		int sessioncount;	
	public:
		sman();
		~sman();
		sess *getSession( string s_id );
		int getSessionCount( ) { return this->sessioncount; }
		sess *createSession( );
		void destroySession( string s_id );


};


#endif

