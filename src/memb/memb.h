#ifndef MEMB_H
#define MEMB_H

#include "../incl.h"
#include "../maps/nhashmap.h"
#include "tupel.h"

using namespace std;

template<class obj_type>
class memb
{
private:
  nhashmap< mutexed_tupel<obj_type>* > map_elems;

public:
  inline obj_type get_elem(string s_key);
  inline void set_elem(obj_type t_obj, string s_key);

};

#include "memb.tmpl"
#endif
