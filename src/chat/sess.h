#include "../incl.h"

#ifndef SESS_H
#define SESS_H

#include <map>
#include <string>

class cont;

using namespace std;

class sess : public name
{

private:
    string sess_id;

public:
    sess(string s_id);
    ~sess();

    string get_id();
    void invalidate();
};
#endif
