#include <iostream>

using namespace std;

extern "C" {
 export int function(void *v_arg);

 int function(void *v_arg)
 {
  cout << "Hallo " << (char*)v_arg << endl;
 }
}
