#ifndef GHTM_H
#define GHTM_H

#include "html.h"

using namespace std;

class HTML 
{
private:
 static html* obj; 

public:
 static void init()
 {
   obj = new html();
 }

 static html& get()
 {
  return *obj;
 }
};


#endif
