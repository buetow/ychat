#ifndef GSMAN_H
#define GSMAN_H

#include "sman.h"

using namespace std;

class s_sman
{
private:
    static sman* obj;

public:
    static void init()
    {
        obj = new sman();
    }

    static sman& get
        ()
    {
        return *obj;
    }
};


#endif
