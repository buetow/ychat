#include "incl.h"

#ifdef LOGGING
#ifndef LOGD_H
#define LOGD_H

#include <queue>
#include <time.h>

class logd
{
private:
    string s_logfile;
    queue<string> s_queue;
    pthread_mutex_t mut_s_logging;
    int i_lines;

    void initialize( string s_filename, int i_log_lines );
    void flush();
    void set_lines( const int i_lines );
    string get_time_string();

public:
    logd( string s_filename, string s_log_lines );
    logd( string s_filename, int i_log_lines );
    ~logd();

    void set_logfile( string s_path, string s_filename );
    void log_access( map<string,string> request );
    void log_simple_line( string s_line );
    void flush_logs();
    static string remove_html_tags( string s_log );
};

#endif
#endif

