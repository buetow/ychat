#ifndef S_NCUR_H
#define S_NCUR_H

#include "ncur.h"

using namespace std;

class s_ncur
{
private:
    static ncur* obj;

public:
    static void init()
    {
        obj = new ncur();
    }

    static ncur& get
        ()
    {
        return *obj;
    }
};

#endif
