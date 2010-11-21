/*:*
 *: File: ./src/msgs.h
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.5.6-BASIC
 *: 
 *: Copyright (C) 2003, 2004 Paul C. Buetow, Volker Richter
 *: Copyright (C) 2005 EXA Digital Solutions GbR
 *: 
 *: This program is free software; you can redistribute it and/or
 *: modify it under the terms of the GNU General Public License
 *: as published by the Free Software Foundation; either version 2
 *: of the License, or (at your option) any later version.
 *: 
 *: This program is distributed in the hope that it will be useful,
 *: but WITHOUT ANY WARRANTY; without even the implied warranty of
 *: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *: GNU General Public License for more details.
 *: 
 *: You should have received a copy of the GNU General Public License
 *: along with this program; if not, write to the Free Software
 *: Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *:*/

#ifndef MSGS_H
#define MSGS_H


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
#define CONTACT "Homepage: http://www.yChat.org"
#define DESCRIP "yChat Server Daemon"
#define DESCRI2 "Copyright (C) 2003 Paul C. Buetow, Volker Richer"
#define DESCRI3 "Copyright (C) 2005 Paul C. Buetow"
#define DESCRI4 "Copyright (C) 2005 EXA Digital Solutions GbR"
#define DOWNMSG "Shutting down "
#define LOGINPR "Login procedure succeeded for nick "
#define MODULEC "Caching module "
#define NEWROOM "Adding room "
#define POOLER1 "Pool: Did not allocate all threads ("
#define POOLER2 "Pool: Max pool size reached ("
#define POOLFLL "Pool: Allocating new threads ("
#define REQUEST "Request string "
#define SEPERAT "----------------------------------------- "
#define SOCKCRT "Creating server socket "
#define SOCKERR "Could not create socket. Trying next port "
#define SOCKRDY "Server socket is ready. See port above "
#define STARTMS "Starting up "
#define TECACHE "Caching template "
#define THREADS "Starting thread job "
#define THREADE "Exiting thread job "
#define VERSION "Version: yChat 0.5.6-BASIC"

#endif
