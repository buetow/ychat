/*:*
 *: File: ./src/reqp.h
 *: 
 *: yChat; Homepage: www.yChat.org; Version 0.5.6-BASIC
 *: 
 *: Copyright (C) 2003, 2004 Paul C. Buetow, Volker Richter
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
    string remove_dots( string s_ret );
    void get_request_parameters( string s_parameters, map<string,string>& map_params );

public:
    // public methods.
    explicit reqp( ); // simple constructor.
    virtual string parse( thrd* p_thrd, string s_req, map_string &map_params );
    virtual string url_decode ( string );
    virtual string get_content_type( string );
    virtual void parse_headers( string s_req, map_string &map_params );
};

#endif
