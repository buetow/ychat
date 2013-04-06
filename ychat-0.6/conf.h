// class conf declaration. this class parses the server config file.

#ifndef s_conf_H
#define s_conf_H

#include "incl.h"
#include "cont.h"
#include "name.h"

using namespace std;

class conf : public cont, name
{
private:

public:
    // public methods:
    conf ( string s_conf ); // standard constructor.
    ~conf(); 		 // standard destructor.

    virtual void parse( );    // parses the config file.
};

#endif
