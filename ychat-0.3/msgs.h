#ifndef MSGS_H
#define MSGS_H

// message templates.

// will be posed if a command does not exist for an user.
#define ERRORCMD " <font color='#FF0000'>No such command!</font><br>\n"
// will post "nickname enters the chat" into the room.
#define USERENTR " enters the chat<br>\n"
// will post "nickname leaves the chat" into the room.
#define USERLEAV " leaves the chat<br>\n"

// several error messages which will apear by the clients.
#define E_ALPNUM "The nick you have specified is not alphanumeric, please change that.<br><br>"
#define E_NONICK "You need to specify a nick name.<br><br>"
#define E_NOTONL "An error occured. Your nick is not online.<br><br>"
#define E_ONLINE "The nick you have specified is already online. Try another nick.<br><br>"

// all the custom messages for verbosity outputs. this messages may not
// be used for html-template value substituation except the CONTACT and
// DESCRIP variables. the verbosity output will appear in the standard
// output of the server.
// alphabetical ordered.
#define CFILEOK "Parsing config file "
#define CFILENO "Failed opening config file "
#define CONNECT "Receiving connection "
#define CONTACT "Contact: www.yChat.org, Mail@yChat.org "
#define DESCRIP "yChat++ Copyright (C) 2003 Paul C. Buetow, Volker Richer " 
#define DOWNMSG "Shutting down "
#define LOGINPR "Login procedure succeeded for nick " 
#define NEWROOM "Adding room "
#define REQUEST "Request string "
#define SEPERAT "----------------------------------------- "
#define SOCKCRT "Creating server socket "
#define SOCKERR "Could not create socket. Trying next port "
#define SOCKRDY "Server socket is ready. See port above "
#define STARTMS "Starting up "
#define TECACHE "Caching template "
#define THREADS "Starting thread job "
#define THREADE "Exiting thread job "
#define VERSION "Version: CVS" 

#endif
