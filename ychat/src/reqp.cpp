/*:*
 *: File: ./src/reqp.cpp
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

#ifndef REQP_CPP
#define REQP_CPP

#include "reqp.h"
#include "tool/tool.h"

using namespace std;

#define HEADER HEADER1 HEADER2 HEADER3 HEADER4 HEADER9
#define STREAM HEADER5 HEADER6

const string reqp::s_http = HEADER;
const string reqp::s_http_stream = STREAM;
const string reqp::s_http_colength = HEADER7;
const string reqp::s_http_cotype = HEADER8;
const string reqp::s_http_cotype_add = HEADER8b;

reqp::reqp( )
{}

void
reqp::parse(context *p_context)
{
  map<string, string> &map_params = *p_context->p_map_params;

  string *p_response = p_context->p_response;
  string &s_event = map_params["event"];

  //<<*
  // check the event variable.
  if ( ! s_event.empty() )
  {
    // login procedure.
    if ( s_event == "login" )
    {
      wrap::CHAT->login( map_params );
    }
    else if ( s_event == "register" )
    {
      user* p_user = new user;
      map_params["INFO"] = "";
      run_html_mod( s_event, map_params, p_user );
      wrap::GCOL->add_user_to_garbage( p_user );
    }
    else
    {
      sess *p_sess = wrap::SMAN->get_session( map_params["tmpid"] );
      user *p_user = NULL;

      if ( p_sess != NULL )
      {
        p_user = p_sess->get_user();
      }
      else
      {
        wrap::system_message(SESSERR);
        return;
      }

      if ( ! p_user )
      {
        map_params["INFO"]    = wrap::CONF->get_elem( "chat.msgs.err.notonline" );
        map_params["request"] = wrap::CONF->get_elem( "httpd.startsite" ); // redirect to the startpage.
      }
      else
      {
        map_params["nick"] = p_user->get_name().c_str();

        // if a message input.
        if ( s_event == "input" )
        {
          if ( p_user )
          {
            p_user->check_restore_away();
            wrap::CHAT->post( p_user, map_params );
          }
        }

        // if a chat stream
        else if ( s_event == "stream" )
        {
          /*
          string s_resp(s_http+s_http_stream);
          s_resp.append(s_http_colength + tool::int2string(s_response.size()) + "\r\n" +
               s_http_cotype + map_params["content-type"]
                // + s_http_cotype_add
          	 + "\r\n\r\n" );*/
          /*		  string s_resp("");
                    s_resp.append(wrap::HTML->parse(map_params));
          		  p_user->set_context(p_context);
          		  */
          map_params["KEEP_ALIVE"] = "yes";
          return;
        }

        // if a request for the online list of the active room.
        else if ( s_event == "online" )
        {
          wrap::HTML->online_list( p_user, map_params );
        }
        else //if ( s_event != "input" )
        {
          run_html_mod( s_event, map_params, p_user );
        }
      }
    }
  }

  if ( wrap::CONF->get_elem("httpd.enablecgi").compare("true") == 0 &&
       string::npos != map_params["request"].find(".cgi") )
  {
    p_response->append( tool::shell_command(
                          wrap::CONF->get_elem("httpd.templatedir") + map_params["request"],
                          METH_RETSTRING ) );
  }
  else
  {
    // parse and get the requested html-template and also use
    // the values stored in map_params for %%KEY%% substituations.
    p_response->append( wrap::HTML->parse( map_params ) );
  }

  // create the http header if not a stream
//  if ( s_event.compare("stream") != 0 ) {
  string s_resp("");
  s_resp.append(s_http);
  //s_resp.append( s_http_stream );
  s_resp.append( s_http_colength + tool::int2string(p_response->size()) + "\r\n" +
                 s_http_cotype + map_params["content-type"] +
                 s_http_cotype_add + "\r\n" );

  s_resp.append(*p_response);
// }
}

//<<*
void
reqp::run_html_mod( string s_event, map<string,string> &map_params, user* p_user )
{
  container *c = new container;

  c->elem[0] = (void*) wrap::WRAP;
  c->elem[1] = (void*) &map_params;
  c->elem[2] = (void*) p_user;

  string s_mod = wrap::CONF->get_elem("httpd.modules.htmldir") + "yc_" + s_event + ".so";

  dynmod* p_module = wrap::MODL->get_module( s_mod, p_user->get_name() );

  if ( p_module != NULL )
    ( *( p_module->the_func ) ) ( static_cast<void*>(c) );

  delete c;
}
//*>>

#endif
