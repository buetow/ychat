#include "../incl.h"
#ifdef DATABASE

#ifndef CON_BASE_H
#define CON_BASE_H

#include "../time/timo.h"

using namespace std;

class con_base : public timo
  {
  public:
    con_base( );
    ~con_base( );
  };

#endif
#endif
