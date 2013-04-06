#include "incl.h"

#ifndef REQP_H
#define REQP_H

#include <map>
#include "thrd/thrd.h"

using namespace std;

typedef map<string, string, less<string> > map_string;

class reqp
{
private:
    static string  HTTP_CODEOK,
    HTTP_CODENF,
    HTTP_EXPIRE,
    HTTP_SERVER,
    HTTP_CONTAC,
    HTTP_CACHEC,
    HTTP_CONNEC,
    HTTP_CHUNKE,
    HTTP_COTYPE;

    // returns the request url from thr client's http request header
    // until the first "?" and stores all request parameter values
    // ( behind "?" ) into map_params.
    virtual string get_url( thrd* p_thrd, string s_req, map_string &map_params );
    // returns a specific value of the client's http request header.
    // ( s.t. like the User-Agent, Referer etc... ).
    virtual string get_from_header( string s_req, string s_hdr );

    virtual int htoi( string *s );
    // Removes double dots ".." 
    string remove_dots( string s_req );

    // Parses "event=bla?blu=bli&sadasda=asddds ..." string and stores them in the map
    void get_request_parameters( string s_parameters, map_string& map_params );

public:
    reqp( );
    virtual string parse( thrd* p_thrd, string s_req, map_string &map_params );
    virtual string url_decode ( string );
    virtual string get_content_type( string );
    virtual void parse_headers( string s_req, map_string &map_params );
};

#endif
