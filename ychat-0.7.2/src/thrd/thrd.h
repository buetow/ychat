// class thrd declaration.
#include "../incl.h"

#ifndef THRD_H
#define THRD_H

using namespace std;

class thrd
{
private:
    int       i_sock;

public:

    // small inline methods:
    int get_sock()
    {
        return i_sock;
    }

    // public methods:
    explicit thrd( int i_sock );
    ~thrd();                     // destructor.
    virtual void run();
};

#endif
