#ifndef SHASHMAP_H
#define SHASHMAP_H

#include <pthread.h>
#include "hashmap.h"

#include "../monitor/dump.h"

using namespace std;

template
<
  class obj_type,
  class key_type_ = string,
  class hash_type = size_hash<string>,
  class alloc_type = compare_allocator<string>
>
class shashmap : protected hashmap<obj_type, key_type_, hash_type, alloc_type>,
		 public dumpable
{
private:
  pthread_mutex_t mut_shashmap;

protected:
  virtual void dumpit();

public:
  explicit shashmap();
  ~shashmap();
  virtual inline void set_elem(obj_type t_obj, key_type_ t_key);
  virtual inline obj_type get_set_elem(obj_type t_obj, key_type_ t_key);
  virtual inline obj_type get_or_callback_set
	(obj_type (*func)(void*), void* p_void, key_type_ t_key);
  virtual inline void add_elem(obj_type t_obj, key_type_ t_key);
  virtual inline void add_elem_insecure(obj_type t_obj, key_type_ t_key);
  virtual inline obj_type get_elem(key_type_ t_key);
  virtual inline void del_elem(key_type_ t_key);
  virtual inline void del_elem_insecure(key_type_ t_key);
  virtual inline void clear();
  virtual inline int size();
  virtual inline bool exists(key_type_ t_key);
  virtual inline vector<key_type_>* get_key_vector();
  virtual inline void run_func( void (*func)(obj_type) );
  virtual inline void run_func( void (*func)(obj_type, void*), void* v_arg );

};

#include "shashmap.tmpl"
#endif
