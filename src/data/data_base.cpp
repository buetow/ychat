#include "data_base.h"

#ifdef DATABASE
#ifndef DATA_BASE_CPP
#define DATA_BASE_CPP

using namespace std;

data_base::data_base( )
{
  pthread_mutex_init ( &mut_con, NULL );

  vector<string> vec_keys = *wrap::CONF->get_key_vector();
  vector<string>::iterator iter;

  // Reads all DATA_ elements of conf.txt!
  for ( iter = vec_keys.begin(); iter != vec_keys.end(); iter++ )
    {
      if ( iter->length() > 20
           && iter->compare(0, 19, "chat.database.mysql") == 0
           && iter->find(".descr") == string::npos )
        {
          vector<string> vec_tokens;
          string s_element = wrap::CONF->get_elem(*iter);
          unsigned i_pos = 0;

          for ( bool b_find = 1; b_find; )
            {
              i_pos = s_element.find_first_of( " ", 0 );

              if ( i_pos != string::npos )
                {
                  vec_tokens.push_back( s_element.substr(0, i_pos) );
                  s_element = s_element.substr( i_pos+1 );
                }

              else
                {
                  vec_tokens.push_back( s_element );
                  b_find = 0;
                }
            }

          string s_mysqlquery = iter->substr(20);
          wrap::system_message(MYSQLQ2 + s_mysqlquery);
          map_queries[s_mysqlquery] = vec_tokens;
        }
    }
}

void data_base::initialize_connections()
{
  int i_min_con = tool::string2int( wrap::CONF->get_elem("chat.database.mincon") ),
                  i_max_con = tool::string2int( wrap::CONF->get_elem("chat.database.maxcon") );

  wrap::system_message(DATAIN0 + tool::int2string(i_max_con));
  wrap::system_message(DATAIN1 + tool::int2string(i_min_con));

  for ( int i = 0; i < i_min_con && i < i_max_con; i++ )
    push_back( new con() );

#ifdef NCURSES

  print_queue_size();
#endif
}

data_base::~data_base()
{
  pthread_mutex_destroy ( &mut_con );
}

hashmap<string>
data_base::select_user_data( string s_user, string s_query)
{
  print_query( DATAQUE + s_query );
  hashmap<string> map_ret;
  return map_ret;
}

void
data_base::insert_user_data( string s_user, string s_query, hashmap<string> insert_map )
{
  print_query( DATAQUE + s_query );
}


void
data_base::update_user_data( string s_user, string s_query, hashmap<string> update_map )
{
  print_query( DATAQUE + s_query );
}

void
data_base::print_query( string s_query )
{
#ifdef DATA_PRINT_QUERIES
  print_query_( s_query );
#endif
}

#ifdef DATA_PRINT_QUERIES
void
data_base::print_query_( string s_query )
{
  wrap::system_message( s_query );
}
#endif

con*
data_base::get_con()
{
  pthread_mutex_lock( &mut_con );

  if ( empty() )
    {
      wrap::system_message( DATANEW + string("(") + tool::int2string(size()+1) + ")" );
      pthread_mutex_unlock( &mut_con );
      return new con;
    }

  else if ( size() > i_max_con-1 )
    {
      wrap::system_message( DATAMAX + string("(") + tool::int2string(i_max_con) + ")" );
      usleep( 5000000 );
      pthread_mutex_unlock( &mut_con );
      return get_con();
    }


  con* p_con = *begin();
  pop_front();

#ifdef NCURSES

  print_queue_size();
#endif

  pthread_mutex_unlock( &mut_con );
  wrap::system_message( DATAGET  );

  p_con->renew_timeout();
  return p_con;
}

void
data_base::push_con( con* p_con )
{
  pthread_mutex_lock( &mut_con );
  push_front( p_con );
#ifdef NCURSES

  print_queue_size();
#endif

  pthread_mutex_unlock( &mut_con );
  wrap::system_message( DATAADD );
}

#ifdef NCURSES
void
data_base::print_queue_size()
{
  if ( wrap::NCUR->is_ready() )
    {
      mvprintw( NCUR_CON_QUEUE_X,NCUR_CON_QUEUE_Y, "Con. queue: %d ", size());
      refresh();
    }
}
#endif

void
data_base::disconnect_all_connections()
{
  wrap::system_message( DATADIS );
  pthread_mutex_lock( &mut_con );

  while ( !empty() )
    {
      con* p_con = *begin();
      pop_front();
      delete p_con;
    }

  pthread_mutex_unlock( &mut_con );
#ifdef NCURSES

  print_queue_size();
#endif
}

void
data_base::check_data_con_timeout()
{
  int i_timeout_time = tool::string2int(wrap::CONF->get_elem("chat.database.contimeout"));
  int i_last_activity;

  pthread_mutex_lock( &mut_con );

  list< list<con*>::iterator > erase_list;
  for ( list<con*>::iterator iter = begin();
        iter != end(); iter++ )
    {
      i_last_activity = (int) (*iter)->get_last_activity();
      if ( i_timeout_time <= i_last_activity )
        {
          con* p_con = *iter;
          erase_list.push_back(iter);
          delete p_con;
          wrap::system_message(DATADI2 + tool::int2string(size()-erase_list.size()+1) + ","
                               + tool::int2string(i_timeout_time) + ","
                               + tool::int2string(i_last_activity) + ")");
        }
    }
  for ( list< list<con*>::iterator >::iterator erase_iter = erase_list.begin();
        erase_iter != erase_list.end(); erase_iter++ )
    erase( *erase_iter );

#ifdef NCURSES

  print_queue_size();
#endif

  pthread_mutex_unlock( &mut_con );
}


#endif
#endif
