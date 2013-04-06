// global variables.

#ifndef GLOB_H
#define GLOB_H

#include <map>
#include <pthread.h>

// definition of boolean values.
#define true   1
#define false  0

// config filename. ( will be searched in ~/.ychat/ and if not found in the working 
// directory of ychat ).
#define CONFILE "conf.txt"

// the hard limit of mysql connections at the samte time in the mysql connection
// manager.
#define MAXMSQL 20

// defines the standard value of the init method of the mysql connection manager
// for the mysql server port. 
#define PRTMSQL 3306

// the highest port which is allowed to use. if ychat is unable to create the server
// socket it will increment the port number and tries to create another socket.
// this procedure will go on until MAXPORT is reached.
#define MAXPORT 65535

// max length of a line read from a socket or a file ( config-file, html-template ).
#define READBUF 1024

// max length of a char buffer
#define CHARBUF 100

// definition for verbosity level ( normal outputs ). see vmsg.h for custumizing all
// the messages. this messages will only printed out by the master thread.
// dont use this until NCURSES is defined! all messages will appear in the
// ncurses interface anyways.
//#define VERBOSE

// displays important server messages such as socket or fatal errors!
// dont use this until NCURSES is defined! all messages will appear in the
// ncurses interface anyways.
//#define SERVMSG

// is used for interaction with the ncurses admin interface at stdout.
#define NCURSES

// The following values define the positions of the data stats in the NCURSES interface.
#ifdef NCURSES
#define NCUR_HITS_X 21
#define NCUR_HITS_Y 35
#define NCUR_MYSQL_X 22
#define NCUR_MYSQL_Y 2
#define NCUR_PORT_X 21
#define NCUR_PORT_Y 2
#define NCUR_SESSION_X 22
#define NCUR_SESSION_Y 18 
#define NCUR_THREADS_X 21
#define NCUR_THREADS_Y 18 
#endif

// Defines the amount of newlines which have to send to the client's
// chat stream the first log-in. ( prevents white screen because of buffers
// or proxies ).
#define PUSHSTR 1000

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
typedef int function( void *v_arg );

struct container
{
    void* elem[4];
};

struct dynmod
{
    function *the_func  ;
    void     *the_module;
};

#endif
