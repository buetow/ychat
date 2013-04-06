// class thrd declaration.

#ifndef THRD_H
#define THRD_H

#include "incl.h"

using namespace std;

class thrd 
{
private:
 int       i_sock;

public:

 // small inline methods:
 int get_sock() { return i_sock; }

 // public methods: 
 explicit thrd( int i_sock );
 ~thrd();                     // destructor. 
 virtual void run();
};

#endif
