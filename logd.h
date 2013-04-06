#ifndef LOGD_H
#define LOGD_H

#include "incl.h"
#include "s_tool.h"
#include "s_conf.h"
#include <fstream>
#include <queue>
#include <time.h>
class logd
{

private:
    string s_logfile;
    queue<string> s_queue;
    ofstream s_output;
    int i_lines;
public:
    logd( string filename );
    ~logd();

    void flush();
    void log( map_string request );
};
#endif
