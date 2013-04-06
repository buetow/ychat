#include "../incl.h"
#ifdef DATABASE

#ifndef DATA_BASE_H
#define DATA_BASE_H

#include <vector>
#include <list>
#include "con.h"

using namespace std;

class data_base : protected list<con*>
{
 private:
  pthread_mutex_t mut_con;
  int i_max_con;

#ifdef NCURSES
  void print_queue_size();
#endif

 protected:
  map< string, vector<string> > map_queries;
  void print_query( string s_query );
#ifdef DATA_PRINT_QUERIES
  virtual void print_query_( string s_query );
#endif
  con* get_con();
  void push_con( con* p_con );

 public:
  data_base();
  ~data_base();

 void initialize_connections();
 virtual map_string select_user_data( string s_user, string s_query );
 virtual void insert_user_data( string s_user, string s_query, map_string insert_map );
 virtual void update_user_data( string s_user, string s_query, map_string update_map );
 void disconnect_all_connections();
 void check_data_con_timeout();
};

#endif
#endif
