/*:*
 *: File: ./src/glob.h
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.8.3-CURRENT
 *: 
 *: Copyright (C) 2003 Paul C. Buetow, Volker Richter
 *: Copyright (C) 2004 Paul C. Buetow
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

/*
 Notice: 
 
 All #defines which start with an CONFIG can be edited through
 gmake config in the main directory!
*/

#include "maps/hashmap.h"

// global variables.
#ifndef GLOB_H
#define GLOB_H

// Definition of boolean values.
#define true   1
#define false  0

//<<*
/* FIRST THE YCHAT ONLY OPTIONS */

#ifdef HAVE_LIBMYSQLCLIENT 
#ifdef HAVE_MYSQL_MYSQL_H 
#define USE_MYSQL
/* - CONFIG -
 Should all database queries printed out at the admin interface? 
 (This option wont take action if database support has not been 
 chosen) 
*/
#define DATA_PRINT_QUERIES
#endif

#define PUSHSTR 500

#ifdef HAVE_LIBSSL
#ifdef HAVE_OPENSSL_SSL_H 
#define OPENSSL
#endif
#endif

/* - CONFIG -
 Should yChat get compiled with comand line interface support?
*/
#define CLI

/* - CONFIG -
 What should be the name of the config file?
*/
#define CONFILE "ychat.conf"

/* - DISABLED -
 Enable debugging options. 
*/
//#define DEBUG

/* - DISABLED -
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
 Please enter the highest networking port which is allowed to be 
 used. If yChat is unable to create the server socket on a cert-
 ain port, it will increment the port number and retries to cre-
 ate another socket on the incremented port number. This proced-
 ure will continue until MAXPORT has been reached.
*/
#define MAXPORT 65535

/* Specifies the max amount of lines to read from a HTTP request 
   header
*/
#define MAXLINES 30

/* Specifies the max length of a lines to read from a HTTP request 
   header
*/
#define MAXLENGTH 1024

/* - CONFIG -
 Should yChat get compiled with ncurses support?
*/
#define NCURSES

/* - CONFIG -
 Please specify the maximum length of a HTTP post request. 
*/
#define POSTBUF 512

#define POSTBUF 1024
#define READBUF 2048 
#define READSOCK 2048
//#define SERVMSG
//#define CTCSEGV
//#define VERBOSE

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
  METH_RETSTRING
} method;

// Define external executables:
#define GMAKE "/usr/local/bin/gmake \0"
#endif
