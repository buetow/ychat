/*:*
 *: File: ./src/reqp.h
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.7.9.5-RELEASE
 *: 
 *: Copyright (C) 2003 Paul C. Buetow, Volker Richter
 *: Copyright (C) 2004 Paul C. Buetow
 *: Copyright (C) 2005 EXA Digital Solutions GbR
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

#include "incl.h"
#ifndef REQP_H
#define REQP_H

#include "maps/hashmap.h"

using namespace std;

class reqp
{
private:
  static const string s_http;
  static const string s_http_stream;
  static const string s_http_colength;
  static const string s_http_cotype;
  static const string s_http_cotype_add;

  // returns the request url from thr client's http request header
  // until the first "?" and stores all request parameter values
  // ( behind "?" ) into map_params.
  string get_url( string s_req, map<string,string> &map_params, int& i_postpayloadoffset );
  // returns a specific value of the client's http request header.
  // ( s.t. like the User-Agent, Referer etc... ).
  string get_from_header( string s_req, string s_hdr );

  int htoi( string *p_str );
  void run_html_mod( string s_event, map<string,string> &map_params, user* p_user ); //<<
  // Removes double dots ".."
  string remove_dots( string s_req );

  // Parses "event=bla?blu=bli&sadasda=asddds ..." string and stores them in the map
  void get_request_parameters( string s_parameters, map<string,string>& map_params );

public:
  reqp( );
  string parse( socketcontainer* p_sock, string s_req, map<string,string> &map_params, int &i_postpayloadoffset );
  string url_decode ( string s_url );
  string get_content_type( string& s_file );
  void parse_headers( string s_req, map<string,string> &map_params );
};

#endif
