#ifndef s_sess_H
#define s_sess_H

#include "incl.h"
#include "cont.h"
#include "name.h"
#include <map>
#include <string>
#include "hmap.h"
using namespace std;

typedef map<string, void *> sess_map;

class sess : public cont, name
{

private:
    sess_map sess_values;
    string sess_id;
public:
    sess(string s_id);
    string get_id();
    void setValue(string s_key, void *lpvalue);
    void *get_elem( string s_key );
    void invalidate();
    string dump();
};
#endif
