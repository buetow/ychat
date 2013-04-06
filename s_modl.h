#ifndef GMOD_H
#define GMOD_H

#include "modl.h"

using namespace std;

class s_modl
{
private:
    static modl* obj;

public:
    static void init()
    {
        obj = new modl();
    }

    static modl& get
        ()
    {
        return *obj;
    }
};


#endif
