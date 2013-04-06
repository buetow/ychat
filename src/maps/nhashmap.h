#ifndef NHASHMAP_H
#define NHASHMAP_H

#include "shashmap.h"

using namespace std;

template
<
  class obj_type,
  class key_type_ = string,
  class hash_type = size_hash<string>,
  class alloc_type = compare_allocator<string>
>
struct nhashmap : public shashmap<obj_type, key_type_, hash_type, alloc_type>
{
  inline obj_type get_elem(key_type_ t_key);
};

#include "nhashmap.tmpl"
#endif
