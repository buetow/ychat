#ifndef HIDDEN_H
#define HIDDEN_H

#include "incl.h"

using namespace std;

class hidden 
{
 private:
  bool b_hidden;

 public:
  hidden();
  virtual void hide();
  virtual void show();
  bool is_hidden();
};

#endif

