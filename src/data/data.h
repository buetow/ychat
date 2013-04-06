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
  hashmap<string> parse_result( MYSQL_RES* p_result, vector<string>& vec_elements );
  void insert_query( string s_query, hashmap<string> map_insert );
  string secure_query( string s_mysql_query );
public:
  data( );
  ~data( );

  hashmap<string> select_user_data( string s_user, string s_query );
  void insert_user_data( string s_user, string s_query, hashmap<string> insert_map );
  void update_user_data( string s_user, string s_query, hashmap<string> update_map );
};

#endif
#endif
