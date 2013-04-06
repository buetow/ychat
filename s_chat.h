#ifndef GCHT_H
#define GCHT_H

#include "chat.h"

using namespace std;

class s_chat
{
private:
    static chat* obj;

public:
    static void init()
    {
        obj = new chat();
    }

    static chat& get
        ()
    {
        return *obj;
    }
};


#endif
