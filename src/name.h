#include "incl.h"

#ifndef NAME_H
#define NAME_H

using namespace std;

class name
{
protected:
  string s_name; // object's name.
  pthread_mutex_t mut_s_name;

public:
  virtual string get_name  ( );
  virtual string get_lowercase_name  ( );
  virtual void   set_name  ( string s_name );

  name();
  name( string s_name );      // a standard constructor.
  ~name();
};

#endif
