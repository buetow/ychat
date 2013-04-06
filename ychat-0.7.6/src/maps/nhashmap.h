#ifndef NHASHMAP_H
#define NHASHMAP_H

#include "shashmap.h"

using namespace std;

template<class obj_type>
struct nhashmap : public shashmap<obj_type>
{
 inline obj_type get_elem(string s_key);
};

#include "nhashmap.tmpl"
#endif
