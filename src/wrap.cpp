#ifndef WRAP_CPP
#define WRAP_CPP

#include "wrap.h"

using namespace std;

//<<*
chat* wrap::CHAT;
#ifdef DATABASE
data* wrap::DATA;
#endif
gcol* wrap::GCOL;
sman* wrap::SMAN;
modl* wrap::MODL;
//*>>

conf* wrap::CONF;
html* wrap::HTML;
#ifdef LOGGING
logd* wrap::LOGD;
#endif
#ifdef NCURSES
ncur* wrap::NCUR;
#endif
sock* wrap::SOCK;
stats* wrap::STAT;
timr* wrap::TIMR;
dynamic_wrap* wrap::WRAP;

void
wrap::system_message( string s_message )
{
#ifdef NCURSES
   wrap::NCUR->print( s_message );
#endif
#ifdef SERVMSG
   cout << s_message << endl;
#endif
#ifdef LOGGING
   wrap::LOGD->log_simple_line( s_message + "\n" );
#endif
}
#endif
