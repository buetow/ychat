#ifndef MMAN_H
#define MMAN_H

#include <mysql/mysql.h>
#include <string>
#include <vector>
#include "glob.h"
#include "mcon.h"

using namespace std;
class mman
{
private:
    vector<MYSQL *> mysql;
    string s_host;
    string s_user;
    string s_pass;
    string s_db;
    unsigned int i_port;
    int i_initial_connections;
    int i_max_connections;
    int i_used_connections;

    pthread_mutex_t mut_i_used_con;
public:
    mman(int initial, int max);
    ~mman();
    void init( string host, string user, string passwd, string db, unsigned int port = PRTMSQL );
    MYSQL *get_connection();
    MYSQL *new_connection( );
    void free_connection( MYSQL *msql );
#ifdef NCURSES
    void print_init_ncurses();	
    void print_used_connections( bool b_refresh );
#endif	

};
#endif
