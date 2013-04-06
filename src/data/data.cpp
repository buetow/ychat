#include "data.h"

#ifdef DATABASE
#ifndef DATA_CPP
#define DATA_CPP

using namespace std;

data::data()
{}

data::~data()
{}

hashmap<string>
data::select_user_data( string s_user, string s_query)
{
  string s_where_rule = " WHERE nick = \"" + s_user + "\"";
  vector<string> vec_elements;
  MYSQL_RES* p_result = select_query( s_query, s_where_rule, vec_elements );
  return parse_result( p_result, vec_elements );
}

MYSQL_RES*
data::select_query( string s_query, string s_where_rule, vector<string>& vec_elements )
{
  con* p_con = get_con();

  vec_elements = map_queries[s_query];
  string s_mysql_query = "SELECT ";
  vector<string>::iterator iter = vec_elements.begin();

  string s_table = *iter;
  iter++;

  while ( iter != vec_elements.end() )
  {
    s_mysql_query.append( secure_query(*iter) );
    if ( ++iter != vec_elements.end() )
      s_mysql_query.append( ", " );
  }

  s_mysql_query.append(" FROM " + s_table + s_where_rule );
  print_query( MYSQLQU + s_mysql_query );

  MYSQL_RES* p_result = NULL;

  if ( 0 == mysql_query( p_con->p_mysql, (const char*)s_mysql_query.c_str() ) )
  {
    p_result = mysql_store_result( p_con->p_mysql );
    push_con( p_con );
  }
  else
  {
    wrap::system_message( MYSQLQU + string( mysql_error(p_con->p_mysql) ) );
    if (p_con != NULL)
      delete p_con;
  }

  return p_result;
}

hashmap<string>
data::parse_result( MYSQL_RES* p_result, vector<string>& vec_elements )
{
  hashmap<string> map_ret;
  if ( p_result != NULL )
  {
    MYSQL_ROW row;
    vector<string>::iterator vec_iter = vec_elements.begin();
    vec_iter++;

    while( (row = mysql_fetch_row(p_result)) )
      for ( int i=0; i < mysql_num_fields(p_result); i++, vec_iter++ )
        map_ret[*vec_iter] = string(row[i]);

    mysql_free_result( p_result );
  }
  return map_ret;
}

void
data::insert_user_data( string s_user, string s_query, map<string,string> insert_map )
{
  insert_query( s_query, insert_map );
}

void
data::insert_query( string s_query, map<string,string> map_insert )
{
  vector<string> vec_elements = map_queries[s_query];
  vector<string>::iterator iter = vec_elements.begin();

  string s_table = *iter;
  iter++;
  string s_mysql_query = "INSERT INTO " + s_table + " (";

  while ( iter != vec_elements.end() )
  {
    s_mysql_query.append( *iter );

    if ( ++iter != vec_elements.end() )
      s_mysql_query.append( ", " );
    else
      s_mysql_query.append( ") VALUES(" );
  }

  iter = vec_elements.begin();
  iter++;

  while ( iter != vec_elements.end() )
  {
    s_mysql_query.append( "\"" + secure_query(map_insert[*iter]) + "\"" );
    if ( ++iter != vec_elements.end() )
      s_mysql_query.append( ", " );
    else
      s_mysql_query.append( ")" );
  }

  print_query( MYSQLQU + s_mysql_query );

  con* p_con = get_con();

  if ( 0 != mysql_query( p_con->p_mysql, (const char*)s_mysql_query.c_str() ) )
    wrap::system_message( MYSQLQU + string( mysql_error(p_con->p_mysql) ) );

  push_con( p_con );

  return;
}

void
data::update_user_data( string s_user, string s_query, hashmap<string> update_map )
{
  vector<string> vec_elements = map_queries[s_query];

  if ( vec_elements.size() == 0 )
    return;

  vector<string>::iterator iter = vec_elements.begin();
  vector<string>::iterator iter_second = vec_elements.begin();
  iter_second++;

  string s_table = *iter;
  iter++;
  string s_mysql_query = "UPDATE " + s_table + " SET ";
  bool b_flag = 0;

  while ( iter != vec_elements.end() )
  {
    if ( update_map[*iter] == "" ) // Dont update data if it has not been changed / if its empty!
    {
      iter++;
      continue;
    }

    if ( iter != iter_second && b_flag )
      s_mysql_query.append( ", " );

    s_mysql_query.append( *iter + "=\"" + secure_query(update_map[*iter]) + "\"" );
    b_flag = 1;
    iter++;
  }

  if ( b_flag )
  {
    s_mysql_query.append( " WHERE nick=\"" + tool::to_lower(s_user) + "\"" );

    con* p_con = get_con();
    print_query( MYSQLQU + s_mysql_query );

    if ( 0 != mysql_query( p_con->p_mysql, (const char*)s_mysql_query.c_str() ) )
      wrap::system_message( MYSQLQU + string( mysql_error(p_con->p_mysql) ) );

    push_con( p_con );
  }
}

string
data::secure_query( string s_mysql_query )
{
  // Prevent from MySQL injection attacks (escaping " and \)
  unsigned i_pos = s_mysql_query.find("\\");

  while ( i_pos != string::npos )
  {
    s_mysql_query.replace( i_pos, 1, "/" );
    i_pos = s_mysql_query.find("\\");
  }

  i_pos = s_mysql_query.find("\"");

  while ( i_pos != string::npos )
  {
    s_mysql_query.replace( i_pos, 1, "'" );
    i_pos = s_mysql_query.find("\"");
  }

  return s_mysql_query;
}

#endif
#endif
