/*:*
 *: File: ./src/data/con.cpp
 *: 
 *: yChat; Homepage: ychat.buetow.org; Version 0.9.0-CURRENT
 *: 
 *: Copyright (C) 2003 Paul C. Buetow, Volker Richter
 *: Copyright (C) 2004 Paul C. Buetow
 *: Copyright (C) 2005 EXA Digital Solutions GbR
 *: Copyright (C) 2006, 2007 Paul C. Buetow
 *: 
 *: This program is free software; you can redistribute it and/or
 *: modify it under the terms of the GNU General Public License
 *: as published by the Free Software Foundation; either version 2
 *: of the License, or (at your option) any later version.
 *: 
 *: This program is distributed in the hope that it will be useful,
 *: but WITHOUT ANY WARRANTY; without even the implied warranty of
 *: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *: GNU General Public License for more details.
 *: 
 *: You should have received a copy of the GNU General Public License
 *: along with this program; if not, write to the Free Software
 *: Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *:*/
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
