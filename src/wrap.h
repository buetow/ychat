#ifndef WRAP_H
#define WRAP_H

#include "incl.h"

#ifdef DATABASE
#include "data/data.h"
#endif
#include "chat/chat.h"
#include "conf/conf.h"
#include "chat/gcol.h"
#include "html.h"
#include "logd.h"
#include "modl.h"
#ifdef NCURSES
#include "ncur/ncur.h"
#endif
#include "chat/sman.h"
#include "sock/sock.h"
#include "stats.h"
#include "time/timr.h"

using namespace std;

class dynamic_wrap 
{
 public: 
    //<<*
    chat* CHAT;
#ifdef DATABASE
    data* DATA;
#endif
    gcol* GCOL;
    sman* SMAN;
    //*>>

    conf* CONF;
    html* HTML;
    logd* LOGD;
    modl* MODL;
#ifdef NCURSES
    ncur* NCUR;
#endif
    sock* SOCK;
    stats* STAT;
    timr* TIMR;
};

class wrap
{
public:
    static void system_message( char* c_message )
    { 
     wrap::system_message( string(c_message) );
    }

    static void system_message( string* p_message )
    { 
     wrap::system_message( *p_message );
    }

    static void system_message( string s_message );

    //<<*
    static chat* CHAT;
#ifdef DATABASE
    static data* DATA;
#endif
    static gcol* GCOL;
    static sman* SMAN;
#ifdef IRCBOT
    static ybot* YBOT;
#endif
    //*>>

    static conf* CONF;
    static html* HTML;
    static logd* LOGD;
    static modl* MODL;
#ifdef NCURSES
    static ncur* NCUR;
#endif
    static sock* SOCK;
    static stats* STAT;
    static timr* TIMR;
    static dynamic_wrap* WRAP; 
};


#endif
