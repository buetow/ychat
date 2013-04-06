#ifndef MCON_H
#define MCON_H

#include <mysql/mysql.h>
#include <string>
#include <iostream>

using namespace std;

class mcon
{

private:
    bool query_sent;
    MYSQL_RES *current_result;
public:
    MYSQL *myc;
    mcon( MYSQL *mysql );
    ~mcon();
    int query(string query);
    MYSQL_ROW get_next_row();

};
#endif
