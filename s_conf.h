#ifndef GCON_H
#define GCON_H

#include "conf.h"

using namespace std;

class s_conf
{
private:
    static conf* obj;

public:
    static void init()
    {
        obj = new conf( CONFILE );
    }

    static conf& get
        ()
    {
        return *obj;
    }
};


#endif
