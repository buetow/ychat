#include "con.h"

using namespace std;

#ifdef DATABASE
#ifndef CON_CPP
#define CON_CPP

con::con()
{
  p_mysql = mysql_init(NULL);

  while ( !p_mysql )
    {
      wrap::system_message( MYSQLE1 );
      usleep( 30000000 );
      mysql_init(p_mysql);
    }

  while ( mysql_real_connect(
            p_mysql,
            (const char*)wrap::CONF->get_elem("chat.database.serverhost").c_str(),
            (const char*)wrap::CONF->get_elem("chat.database.user").c_str(),
            (const char*)wrap::CONF->get_elem("chat.database.password").c_str(),
            (const char*)wrap::CONF->get_elem("chat.database.dbname").c_str(),
            tool::string2int(wrap::CONF->get_elem("chat.database.port")),
            NULL, 0 ) == NULL )
    {
      wrap::system_message( MYSQLQU + string( mysql_error(p_mysql) ) );
      usleep( 30000000 );
    }
}

con::~con()
{
  if ( p_mysql )
    {
      if (mysql_ping( p_mysql ) != 0)
        mysql_close( p_mysql );
    }
}

#endif
#endif
