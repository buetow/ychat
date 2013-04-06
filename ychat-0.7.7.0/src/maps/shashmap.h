#ifndef SHASHMAP_H
#define SHASHMAP_H

#include <pthread.h>
#include "hashmap.h"

using namespace std;

template<class obj_type>
class shashmap : public hashmap<obj_type>
{
private:
  pthread_mutex_t mut_shashmap;
public:
  explicit shashmap();
  ~shashmap();
  virtual inline void set_elem(obj_type t_obj, string s_key);
  virtual inline obj_type get_set_elem(obj_type t_obj, string s_key);
  virtual inline void add_elem(obj_type t_obj, string s_key);
  virtual inline void add_elem_insecure(obj_type t_obj, string s_key);
  virtual inline obj_type get_elem(string s_key);
  virtual inline void del_elem(string s_key);
  virtual inline void del_elem_insecure(string s_key);
  virtual inline void clear();
  virtual inline int size();
  virtual inline bool exists(string s_key);
  virtual inline vector<string>* get_key_vector();
  virtual inline void run_func( void (*func)(obj_type) );
  virtual inline void run_func( void (*func)(obj_type, void*), void* v_arg );
};

#include "shashmap.tmpl"
#endif
