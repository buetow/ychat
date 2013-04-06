/*
 Notice: 

 All #defines which start with an CONFIG can be edited through
 gmake config in the main directory!
*/

#include <map>

// global variables.
#ifndef GLOB_H
#define GLOB_H

// Definition of boolean values.
#define true   1
#define false  0

/* - CONFIG -
 What should be the name of the config file?
*/
#define CONFILE "ychat.conf"

/* - CONFIG -
 In which prefix should yChat be installed if typing gmake inst-
 all? 
*/
#define PREFIX "/usr/local"

//<<*
/* - CONFIG - 
 Should yChat get compiled with database support? Currently MyS-
 QL only is a supported database. 
*/
#define DATABASE

#ifdef DATABASE
/* - CONFIG - 
 Should all database queries printed out at the admin interface? 
 (This option wont take action if database support has not been 
 chosen) 
*/
#define DATA_PRINT_QUERIES

#define USE_MYSQL 
#endif
//*>>

/* - CONFIG -
 Please enter the highest networking port which is allowed to be 
 used. If yChat is unable to create the server socket on a cert-
 ain port, it will increment the port number and retries to cre-
 ate another socket on the incremented port number. This proced-
 ure will continue until MAXPORT has been reached.
*/
#define MAXPORT 65535

/* - CONFIG - 
 Please specify the maximum length of a line read from a socket 
 or a file. ( config-file, html-template )
*/
#define READSOCK 2048

/* - CONFIG - 
 Please specify the size of a temporary buffer. (Will be used f-
 or different tasks) 
*/
#define READBUF 2048

/* - CONFIG - 
 Please specify the maximum length of a HTTP post request. 
*/
#define POSTBUF 512

/* - CONFIG - 
 Please specify the maximum occupancy of the internal hash-map
 data structures given in percentage. 0.9 means 90% iccupancy.  
*/
#define HMAPOCC .9

/* - CONFIG - 
 Please chose if you want to use verbose server outputs or not. 
 The verbose messages will appear in the ncurses menu if ncurses
 is enabled or in the server-window if yChat has been compiled 
 without ncurses support. This option shows you all incoming
 requests with the client IP and port numbers. You probably want
 this to be turned off if you have heavy server load.
*/
//#define VERBOSE

/* - CONFIG - 
 If you want to enable EXPERIMENTAL features, then set this val-
 ue to true. Else use false which is recommended! All experimen-
 al features are marked inside of the running yChat!
*/
//#define EXPERIM

/* - CONFIG -
 Should yChat get compiled with logging support? 
*/ 
#define LOGGING

/* - CONFIG -
 Should yChat get compiled with ncurses support?
*/ 
#define NCURSES

/* - CONFIG -
 Should yChat get compiled with comand line interface support?
*/
#define CLI


/* - CONFIG -
 DO NOT USE TOGETHER WITH NCURSES! Displays important server mes-
 ages. This one will print all messages to stdout if no NCURSES
 is defined. Don't use this until NCURSES is defined! all messag-
 es will appear in the ncurses interface anyways.
*/
//#define SERVMSG

// Enables debugging options 
//#define DEBUG

//<<*
/* - CONFIG -
 Defines the amount of newlines which have to be sent to the clie-
 nt's chat stream after the first log-in. It prevents a white scr-
 een because of browser buffers or proxies.
*/
#define PUSHSTR 500
//*>>


// The following values define the positions of the data stats in the NCURSES interface.
#ifdef NCURSES
#define NCUR_SERVER_HEADER_X 21
#define NCUR_SERVER_HEADER_Y 2 
#define NCUR_PORT_X 22
#define NCUR_PORT_Y 2
#define NCUR_HITS_X 23
#define NCUR_HITS_Y 2

#define NCUR_POOL_HEADER_X 21
#define NCUR_POOL_HEADER_Y 18 
#define NCUR_POOL_SIZE_X 22
#define NCUR_POOL_SIZE_Y 18 
#define NCUR_POOL_RUNNING_X 23
#define NCUR_POOL_RUNNING_Y 18 

#define NCUR_DATA_HEADER_X 21
#define NCUR_DATA_HEADER_Y 35 
#define NCUR_GARBAGE_X 22
#define NCUR_GARBAGE_Y 35
#define NCUR_CON_QUEUE_X 23 
#define NCUR_CON_QUEUE_Y 35

#define NCUR_CHAT_HEADER_X 21
#define NCUR_CHAT_HEADER_Y 52
#define NCUR_NUM_ROOMS_X 22
#define NCUR_NUM_ROOMS_Y 52 
#define NCUR_SESSION_X 23
#define NCUR_SESSION_Y 52 

#define NCUR_CACHED_HEADER_X 21
#define NCUR_CACHED_HEADER_Y 68 
#define NCUR_CACHED_DOCS_X 22
#define NCUR_CACHED_DOCS_Y 68 
#define NCUR_CACHED_MODS_X 23 
#define NCUR_CACHED_MODS_Y 68 

#define NCUR_MENU_CHAR_X 0
#define NCUR_MENU_CHAR_Y 33 
#define NCUR_UPTIME_X 0
#define NCUR_UPTIME_Y 44
#define NCUR_TIME_X 0
#define NCUR_TIME_Y 64

#endif

//////////////////////////////////////////////////////////////////////////////////////////
// DO NOT CHANGE ANYTHING BEHIND THIS LINE!
//////////////////////////////////////////////////////////////////////////////////////////

using namespace std;

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

typedef enum method_ {
  METH_NCURSES,
  METH_RETSTRING
} method;

// Define external executables:
#define GMAKE "/usr/local/bin/gmake \0"
#endif
