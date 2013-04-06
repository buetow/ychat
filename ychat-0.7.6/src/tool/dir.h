
#ifndef DIR_H
#define DIR_H

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#include <vector>

#include "../incl.h"

using namespace std;

class dir
{
private:
    bool b_open;
    DIR *p_d;  	
    struct dirent *p_ep;
    vector<string> vec_dir;

public:
    dir();
    ~dir();

   bool open_dir( char *c_dir ); 
   bool open_dir( string &s_dir ); 
   void close_dir(); 
   void read_dir();
   vector<string> get_dir_vec();
};

#endif
