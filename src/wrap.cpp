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
//*>>

conf* wrap::CONF;
html* wrap::HTML;
logd* wrap::LOGD;
modl* wrap::MODL;
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
   wrap::LOGD->log_simple_line( s_message + "\n" );
}
#endif
