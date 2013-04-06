#ifndef MEMB_H
#define MEMB_H

#include "../incl.h"
#include "../wrap.h"
#include "../maps/hashmap.h"
#include "tupel.h"

using namespace std;

template<class obj_type>
class memb
{
private:
  hashmap< mutexed_tupel<obj_type>* > map_elems;

public:
  memb()
  {}
  memb(vector<string> &vec_fields);
  void initialize(vector<string> &vec_fields);
  inline obj_type get_elem(string s_key);
  inline void set_elem(obj_type t_obj, string s_key);
};

struct memb_string : protected memb<string>
{
  memb_string()
  {}
  memb_string(vector<string> &vec_fields) : memb<string>(vec_fields)
  {}
  void init_strings(vector<string> vec_fields)
  {
    initialize(vec_fields);
  }
  inline string get_string(string s_key);
  inline void set_string(string s_obj, string s_key);
};

struct memb_int : protected memb<int>
{
  memb_int()
  {}
  void init_ints(vector<string> vec_fields)
  {
    initialize(vec_fields);
  }
  inline int get_int(string s_key);
  inline void set_int(int i_obj, string s_key);
};

struct memb_bool : protected memb<bool>
{
  memb_bool()
  {}
  void init_bools(vector<string> vec_fields)
  {
    initialize(vec_fields);
  }
  inline bool get_bool(string s_key);
  inline void set_bool(bool b_obj, string s_key);
};

class memb_base : public memb_string, public memb_int, public memb_bool
  {}
;

#include "memb.tmpl"
#endif
