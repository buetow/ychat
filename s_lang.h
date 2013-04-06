#ifndef GLANG_H
#define GLANG_H

#include "lang.h"
#include "s_conf.h"
using namespace std;

class s_lang
{
private:
    static lang* obj;

public:
    static void init()
    {
        obj = new lang( s_conf::get
                            ().get_val( "LANGUAGE" ) );
    }

    static lang& get
        ()
    {
        return *obj;
    }
};


#endif
