#ifndef HASHMAP_H
#define HASHMAP_H

#include <ext/hash_map>

using namespace std;

template<class key_type_>
struct compare_allocator
{
  inline bool operator()(key_type_ t_key_1, key_type_ t_key_2) const;
};

template<class key_type_>
struct equals_allocator
{
  inline bool operator()(key_type_ t_key_1, key_type_ t_key_2) const;
};

template<class key_type_>
struct size_hash
{
  inline int operator()(key_type_ t_key) const;
};

template<class key_type_>
struct self_hash
{
  inline int operator()(key_type_ t_key) const;
};

template
<
class obj_type,
class key_type_ = string,
class hash_type = size_hash<string>,
class alloc_type = compare_allocator<string>
>
struct hashmap : public __gnu_cxx::hash_map<key_type_, obj_type, hash_type, alloc_type>
{
  virtual inline void set_elem(obj_type t_obj, key_type_ t_key);
  virtual inline obj_type get_elem(key_type_ t_key);
  virtual inline obj_type get_set_elem(obj_type t_obj, key_type_ t_key);
  virtual inline obj_type get_or_callback_set
  (obj_type (*func)(void*), void* p_void, key_type_ t_key);
  virtual inline vector<key_type_>* get_key_vector();
  virtual inline bool exists(key_type_ t_key);
  virtual inline void run_func( void (*func)(obj_type) );
  virtual inline void run_func( void (*func)(obj_type, void*), void* v_arg );
};

#include "hashmap.tmpl"
#endif
