#ifndef MSGS_H
#define MSGS_H


// several error messages which will apear by the clients.
#define E_ALPNUM "The nick you have specified is not alphanumeric, please change that.<br><br>\0"
#define E_NONICK "You need to specify a nick name.<br><br>\0"
#define E_NOTONL "An error occured. Your nick is not online.<br><br>\0"
#define E_ONLINE "The nick you have specified is already online. Try another nick.<br><br>\0"

// alphabetical ordered:
#define ACCPERR "Sock accept error \0"
#define BINDERR "Sock bind error \0"
#define CFILEOK "Parsing config file \0"
#define CFILENO "Failed opening config file \0"
#define CONTACT "Contact: http://www.yChat.org, Mail@yChat.org, ICQ: 11655527\0"
#define DESCRIP "yChat++: Copyright (C) 2003 Paul C. Buetow, Volker Richter\0"
#define DOWNMSG "Shutting down \0"
#define LISTERR "Sock listen error \0"
#define LOGERR1 "Logging: Could not open logfile \0"
#define LOGERR2 "Logging: No filename specified \0"
#define MODULEC "Caching \0"
#define MODULER "Requesting \0"
#define MYMANAG "MySQL: \0" 
#define MYLIMIT "MySQL reached hardlimit of connections: \0"
#define MYINITC "MySQL initial connections: \0" 
#define MYINITM "MySQL max. connections: \0" 
#define MYERROR "MySQL error: no suitable connection found \0"
#define MYERRO1 "MySQL error: init failed \0"
#define MYERRO2 "MySQL error: reached connection limit \0"
#define NEWREQU "New request \0"
#define NEWROOM "New room \0"
#define NEWUSER "New user \0"
#define NOFOUND "File not found \0"
#define POOLERR "Thread pool malloc error \0"
#define READERR "Sock read error \0"
#define REMUSER "Removing user \0"
#define REQUEST "Request string \0"
#define SELCERR "Sock select error \0"
#define SEPERAT "-----------------------\0"
#define SESSION "Session count \0"
#define SOCKCRT "Created server socket on \0"
#define SOCKERR "Couldn't create socket, trying next port \0"
#define SOCKRDY "Server socket is ready \0"
#define STARTMS "Starting up \0"
#define TECACHE "Caching template \0"
#define VERSION "yChat 0.6-RELEASE \0"

#endif
