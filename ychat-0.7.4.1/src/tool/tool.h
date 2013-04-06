#ifndef TOOL_H
#define TOOL_H

#include "../incl.h"

#include <list>

using namespace std;

class tool
{
public:
    static list<string> split_string(string s_string, string s_split);
    static bool is_alpha_numeric( string &s_digit );
    static char* int2char( int i_int );
    static char* clean_char( char* c_str);
    static string trim( string s_str );
    static string replace( string s_string, string s_search, string s_replace );
    static string int2string( int i_int );
    static long unixtime();
    static int string2int( string s_digit );
    static string get_extension( string s_file );
    static string to_lower( string s_str );
    static void strip_html( string *p_str );
    static string shell_command( string s_command, method m_method );
    static string ychat_version();
};

#endif
