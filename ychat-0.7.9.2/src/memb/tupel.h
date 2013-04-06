#ifndef TUPEL_H
#define TUPEL_H

#include "../incl.h"

#define mutexed_tupel tupel

using namespace std;

template<class obj_type>
class tupel
{
private:
  pthread_mutex_t mut_tupel;
  obj_type t_obj;
public:
  tupel();
  tupel(obj_type t_obj);
  ~tupel();

  inline obj_type get_elem();
  inline void set_elem(obj_type t_obj);
};

#include "tupel.tmpl"
#endif
