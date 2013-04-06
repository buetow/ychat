#include "con_base.h"

#ifdef DATABASE
#ifndef CON_BASE_CPP
#define CON_BASE_CPP

using namespace std;

con_base::con_base()
{
  renew_timeout( );
}

con_base::~con_base()
{}


#endif
#endif
