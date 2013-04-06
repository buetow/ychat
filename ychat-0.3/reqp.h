// class reqp declaration. this class parses the client requests. 

#ifndef REQP_H
#define REQP_H

#define RQ_GET  1
#define RQ_POST 2

#include <map> 
#include "incl.h"
#include "thrd.h"

using namespace std;

typedef map<string, string, less<string> > map_string;

class reqp 
{
private:
 static string  HTTP_CODEOK,
 		HTTP_CODENF,
 		HTTP_SERVER,
 		HTTP_CONTAC,
		HTTP_CACHEC,
 		HTTP_CONNEC,
 		HTTP_COTYPE;

 // returns the request url from thr client's http request header
 // until the first "?" and stores all request parameter values
 // ( behind "?" ) into map_params. 
 virtual string get_url( thrd* p_thrd, string s_req, map_string &map_params );
 // returns a specific value of the client's http request header.
 // ( s.t. like the User-Agent, Referer etc... ). 
 virtual string get_from_header( string s_req, string s_hdr ); 

 virtual int htoi( string *s );

public:
 // public methods.
 explicit reqp( ); // simple constructor.
 virtual string parse( thrd* p_thrd, string s_req, map_string &map_params ); 
 virtual string url_decode ( string );
 virtual string getContentType( string );
};

#endif
