#include "../incl.h"

#ifndef LANG_H
#define LANG_H

#include "../name.h"
#include "../maps/nmap.h"

using namespace std;

class lang : public nmap<string,string>, name
{
private:

public:
    // public methods:
    lang ( string s_lang ); // standard constructor.
    ~lang(); 		 // standard destructor.

    virtual void parse( );    // parses the config file.
};

#endif
