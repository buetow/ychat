// template class data declaration;

#ifndef BASE_H
#define BASE_H

#include <vector> 
#include "incl.h"
#include "name.h"

class base
{
private:
 vector<name*>   vec_elem    ;
 pthread_mutex_t mut_vec_elem; 

public:
  base();
 ~base();

 virtual void  add_elem( name*   p_name );                // add a element.
 virtual bool  del_elem( string &s_name );                // delete a alement.
 virtual name* get_elem( string &s_name, bool &b_found ); // get a element.

 // execute func on all elements of vec_elem. v_pointer is the argument.
 virtual void  run_func( void (*func)(name*, void*), void* v_arg );
};

#endif
