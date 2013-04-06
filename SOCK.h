#ifndef GSOC_H
#define GSOC_H

#include "sock.h"

using namespace std;

class SOCK 
{
private:
 static sock* obj; 

public:
 static void init()
 {
   obj = new sock();
 }

 static sock& get()
 {
  return *obj;
 }
};


#endif
