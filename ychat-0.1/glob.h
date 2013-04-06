// global variables.

#ifndef GLOB_H
#define GLOB_H

#include <map>
#include <pthread.h>

// definition of boolean values.
#define true   1
#define false  0

// config filename.
#define CONFILE "conf.txt"

// the highest port which is allowed to use. if ychat is unable to create the server
// socket it will increment the port number and tries to create another socket.
// this procedure will go on until MAXPORT is reached.
#define MAXPORT 65535

// max length of a line read from a socket or a file ( config-file, html-template ).
#define READBUF 1024

// if defined for every http request a new thread will be created. otherwise only for the
// chat streams extra threads will be created. THRDMOD2 DOES NOT WORK RIGHT NOW!! SO
// LET THIS DEFINE UNTOUCHED! BUGFIX WILL FOLLOW!
#define THRDMOD

// definition for verbosity level 0 ( normal outputs ). see vmsg.h for custumizing all 
// the messages. this messages will only printed out by the master thread.
#define _VERBOSE

// definition for verbosity level 1 ( constructors and destructors which are not inline ).
//#define VERBOSE 

// definition for verbosity level 2 ( all methods which are not inline except VERBOSE 1 ).
//#define VERBOSE_

//////////////////////////////////////////////////////////////////////////////////////////
// DO NOT CHANGE ANYTHING BEHIND THIS LINE!
//////////////////////////////////////////////////////////////////////////////////////////

using namespace std;

// internal rang descriptors ( their external names may be specified different )
enum rang
{ 
 CODER ,  // programmer.
 ADMIN ,  // administrator.
 MAGIC ,  // super user with special privileges. 
 SUPER ,  // temporary super user. 
 BASIC ,  // normal user without special privileges.
 GUEST ,  // guest user, has almost no privileges. 
 RESTR ,  // a very restrivted user.
 OUTBN    // banned out of the system. 
};	

// some custom typedefs for datatypes which are needed often.
typedef map<string, string> map_string;

struct container
{
 void* elem[3];
};

#endif
