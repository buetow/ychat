#include "../incl.h"
#ifdef DATABASE

#ifndef CON_H
#define CON_H

#include <mysql/mysql.h>
#include <iostream>
#include "con_base.h"

using namespace std;

class con : public con_base
  {
  public:
    MYSQL* p_mysql;
    con( );
    ~con( );
  };

#endif
#endif
