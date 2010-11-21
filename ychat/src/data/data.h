/*:*
 *: File: ./src/data/data.h
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
  void insert_query( string s_query, map<string,string> map_insert );
  string secure_query( string s_mysql_query );
public:
  data( );
  ~data( );

  hashmap<string> select_user_data( string s_user, string s_query );
  void insert_user_data( string s_user, string s_query, map<string,string> insert_map );
  void update_user_data( string s_user, string s_query, hashmap<string> update_map );
};

#endif
#endif
