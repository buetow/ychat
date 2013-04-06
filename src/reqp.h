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

  // returns the request url from thr client's http request header
  // until the first "?" and stores all request parameter values
  // ( behind "?" ) into map_params.
  string get_url( int &i_sock, string s_req, map<string,string> &map_params );
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
  string parse( int &i_sock, string s_req, map<string,string> &map_params );
  string url_decode ( string s_url );
  string get_content_type( string s_file );
  void parse_headers( string s_req, map<string,string> &map_params );
};

#endif
