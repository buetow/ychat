// class conf declaration. this class parses the server config file.

#ifndef s_lang_H
#define s_lang_H

#include "incl.h"
#include "cont.h"
#include "name.h"

using namespace std;

class lang : public cont, name
{
private:

public:
    // public methods:
    lang ( string s_lang ); // standard constructor.
    ~lang(); 		 // standard destructor.

    virtual void parse( );    // parses the config file.
};

#endif
