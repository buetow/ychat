#ifndef GMUT_H
#define GMUT_H

#include "mutx.h"

using namespace std;

class MUTX 
{
private:
 static mutx* obj; 

public:
 static void init()
 {
   obj = new mutx();
 }

 static mutx& get()
 {
  return *obj;
 }
};


#endif
