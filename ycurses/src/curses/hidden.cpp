#ifndef HIDDEN_CPP
#define HIDDEN_CPP

#include "hidden.h"

hidden::hidden() 
{
  show();
}

void
hidden::hide()
{
  b_hidden = true;  
}

void
hidden::show()
{
  b_hidden = false;
}

bool
hidden::is_hidden()
{
  return b_hidden;
}

#endif
