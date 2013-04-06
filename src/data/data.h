#include "../incl.h"
#ifdef DATABASE

#ifndef DATA_H
#define DATA_H

#include "data_base.h"
#include <mysql/mysql.h>

using namespace std;

class data : public data_base // data implementation used in data.h
  {
  private:
    MYSQL_RES* select_query( string s_query, string s_where_rule, vector<string>& vec_elements );
    map<string,string> parse_result( MYSQL_RES* p_result, vector<string>& vec_elements );
    void insert_query( string s_query, map<string,string> map_insert );
    string secure_query( string s_mysql_query );
  public:
    data( );
    ~data( );

    map<string,string> select_user_data( string s_user, string s_query );
    void insert_user_data( string s_user, string s_query, map<string,string> insert_map );
    void update_user_data( string s_user, string s_query, map<string,string> update_map );
  };

#endif
#endif
