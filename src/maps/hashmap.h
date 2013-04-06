#ifndef HASHMAP_H
#define HASHMAP_H

#include <ext/hash_map>
#include "hash.h"

using namespace std;

using __gnu_cxx::hash_map;

template<class obj_type>
struct hashmap : public hash_map< string, obj_type, hash, hashmap_allocator>
{
 virtual inline void set_elem(obj_type t_obj, string s_key);
 virtual inline obj_type get_elem(string s_key);
 virtual inline obj_type get_set_elem(obj_type t_obj, string s_key);
 virtual inline vector<string>* get_key_vector();
 virtual inline bool exists(string s_key);
 virtual inline void run_func( void (*func)(obj_type) );
 virtual inline void run_func( void (*func)(obj_type, void*), void* v_arg );
};

#include "hashmap.tmpl"
#endif
