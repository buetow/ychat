#ifndef REQP_CPP
#define REQP_CPP

#include "reqp.h"
#include "tool/tool.h"

using namespace std;

#define HEADER HEADER1 HEADER2 HEADER3 HEADER4
#define STREAM HEADER5 HEADER6

const string reqp::s_http = HEADER;
const string reqp::s_http_stream = STREAM;
const string reqp::s_http_colength = HEADER7;
const string reqp::s_http_cotype = HEADER8;

reqp::reqp( )
{}

void
reqp::get_request_parameters( string s_parameters, hashmap<string>& map_params )
{
  string s_tmp;
  unsigned i_pos, i_pos2;

  while( (i_pos = s_parameters.find("&")) != string::npos )
  {
    s_tmp = s_parameters.substr(0, i_pos );

    if ( (i_pos2 = s_tmp.find("=")) != string::npos )
      map_params[ s_tmp.substr(0, i_pos2) ] = tool::replace( s_tmp.substr( i_pos2+1 ), "\\AND", "&");

    s_parameters = s_parameters.substr( i_pos + 1 );
  }

  // Get the last request parameter, which does not have a "&" on the end!
  if( (i_pos = s_parameters.find("=")) != string::npos )
    map_params[ s_parameters.substr(0, i_pos) ] = s_parameters.substr( i_pos+1 );

  //map<string,string>::iterator iter;
  //for ( iter = map_params.begin(); iter != map_params.end(); ++iter )
  //cout << ">>>" << iter->first << "=" << iter->second << endl;
}

string
reqp::get_url( int &i_sock, string s_req, hashmap<string> &map_params )
{
  unsigned i_pos, i_pos2;
  string s_vars( "" );
  string s_ret;
  int i_req;

  // GET request
  if ( s_req.find("GET") != string::npos)
  {
    // Be sure that the GET request has minimum length
    if ( s_req.length() > 5 )
    {
      // Get rid of "GET /"
      if ( (i_pos = s_req.find("\n")) == string::npos )
        i_pos = s_req.length() - 1;

      s_req = s_req.substr(5, i_pos - 5);

      // Get HTML site to be displayed
      if ( (i_pos = s_req.find("?")) == string::npos )
      {
        if ( (i_pos2 = s_req.find(" HTTP")) != string::npos )
          s_ret = url_decode( s_req.substr(0, i_pos2));
      }
      else
      {
        s_ret = url_decode( s_req.substr(0, i_pos) );

        // Get request parameters:
        if ( (i_pos2 = s_req.find(" HTTP")) != string::npos )
        {
          s_req = url_decode( s_req.substr(i_pos + 1, i_pos2 - i_pos - 1) );
          get_request_parameters( s_req, map_params );
        }
      }

    }
  }

  // POST request
  else
  {
    if ( (i_pos2 = s_req.find("HTTP")) != string::npos )
    {
      if ( 13 < i_pos2 )
      {
        s_ret = url_decode( s_req.substr(6,i_pos2-7) );

        int i_len  = s_ret.length();
        int i_len2 = s_req.length()-1;

        s_req = s_req.substr( i_len < i_len2 ? i_len : i_len2 );

        if ( (i_pos = s_req.find("event=")) == string::npos)
        {
          char c_req[POSTBUF];
          i_len = read(i_sock, c_req, POSTBUF);
          s_req = c_req;
          s_req = s_req.substr(0, i_len);

          if ( (i_pos = s_req.find("event=")) != string::npos )
            get_request_parameters( url_decode( s_req.substr(i_pos) ), map_params );
        }
        else
        {
          get_request_parameters( url_decode( s_req.substr(i_pos) ), map_params );
        }
      }
    }
  }

#ifdef VERBOSE
  wrap::system_message( REQUEST + s_ret );
#endif

  if ( s_ret.empty() )
    s_ret = wrap::CONF->get_elem( "httpd.startsite" );

  else
    s_ret = remove_dots(s_ret);

  map_params["request"] = s_ret;

  return s_ret;
}

string
reqp::get_content_type( string s_file )
{
  string s_ext(tool::get_extension( s_file ));

  if( s_ext == "" )
    s_ext = "default";

  return wrap::CONF->get_elem( "httpd.contenttypes." + s_ext );
}

void
reqp::parse_headers( string s_req, hashmap<string> &map_params )
{
  int pos = s_req.find("\n");

  if (pos != string::npos)
  {
    map_params["QUERY_STRING"] = tool::trim(s_req.substr(0,pos-1));

    int pos2;
    do
    {
      string s_line( s_req.substr(0, pos) );
      pos2 = s_line.find(":");

      if (pos2 != string::npos && s_line.length() > pos2+1)
        map_params[ tool::trim(s_line.substr(0, pos2)) ] = tool::trim(s_line.substr(pos2+1));

      s_req = s_req.substr( s_line.size() + 1 );
      pos = s_req.find("\n");
    }
    while( pos != string::npos);
  } // if
}

int
reqp::htoi(string *p_str)
{
  int value, c;
  c = p_str->at(0);

  if( isupper(c) )
    c = tolower(c);

  value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

  c = p_str->at(1);

  if( isupper(c) )
    c = tolower(c);

  value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

  return value;
}

string
reqp::url_decode( string s_url )
{
  string s_dest = "";
  int i_len = s_url.size();
  int i_prv = i_len - 2;

  char c;
  for( int i = 0; i < i_len; ++i)
  {
    c = s_url.at(i);
    if( c == '+' )
    {
      s_dest += " ";
    }
    else if (c == '%' && i < i_prv)
    {
      string s_tmp = s_url.substr(i+1, 2);
      c = (char) htoi(&s_tmp);
      s_dest += c;
      i += 2;
    }
    else
    {
      s_dest += c;
    }
  }

  return s_dest;
}

string
reqp::get_from_header( string s_req, string s_hdr )
{
  unsigned i_pos[2];
  if ( (i_pos[0] = s_req.find( s_hdr, 0 )) == string::npos )
    return "";

  if ( (i_pos[1] = s_req.find( "\n", i_pos[0]) ) == string::npos )
    return "";

  unsigned i_len = s_hdr.length();
  return s_req.substr( i_pos[0] + i_len, i_pos[1] - i_pos[0] - i_len - 1 );
}

string
reqp::parse( int &i_sock, string s_req, hashmap<string> &map_params )
{
  // store all request informations in map_params. store the url in
  // map_params["request"].
  get_url( i_sock, s_req, map_params );

  parse_headers( s_req, map_params );
  string s_event( map_params["event"] );

  map_params["content-type"] = get_content_type( map_params["request"] );

  string s_rep( "" );

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

      if( p_sess != NULL )
      {
        p_user = p_sess->get_user();
      }
      else
      {
        return s_rep;
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
          string s_msg ( wrap::HTML->parse( map_params ) );
          p_user->msg_post( &s_msg);
          wrap::SOCK->chat_stream( i_sock, p_user, map_params );
        }

        // if a request for the online list of the active room.
        else if ( s_event == "online" )
        {
          wrap::HTML->online_list( p_user, map_params );
        }
        else if ( s_event != "input" )
        {
          run_html_mod( s_event, map_params, p_user );
        }
      }
    }
  }
  //*>>

  if ( wrap::CONF->get_elem("httpd.enablecgi").compare("true") == 0 &&
       string::npos != map_params["request"].find(".cgi") )
  {
    s_rep.append( tool::shell_command(
                    wrap::CONF->get_elem("httpd.templatedir") + map_params["request"],
                    METH_RETSTRING ) );
  }
  else
  {
    // parse and get the requested html-template and also use
    // the values stored in map_params for %%KEY%% substituations.
    s_rep.append( wrap::HTML->parse( map_params ) );
  }

  // create the http header.

  string s_resp(s_http);
  if ( s_event.compare("stream") == 0 )
    s_resp.append( s_http_stream );

  s_resp.append( s_http_colength + tool::int2string(s_rep.size()) + "\n"
                 + map_params["content-type"] + "\r\n\r\n" );

  s_resp.append(s_rep);


  // return the parsed html-template.
  return  s_resp;
}

//<<*
void
reqp::run_html_mod( string s_event, hashmap<string> &map_params, user* p_user )
{
  container *c = new container;

  c->elem[0] = (void*) wrap::WRAP;
  c->elem[1] = (void*) &map_params;
  c->elem[2] = (void*) p_user;

  string s_mod = wrap::CONF->get_elem("httpd.modules.htmldir") + "yc_" + s_event + ".so";

  dynmod* p_module = wrap::MODL->get_module( s_mod );

  if ( p_module != NULL )
    ( *( p_module->the_func ) ) ( (void*) c );

  delete c;
}
//*>>

string
reqp::remove_dots( string s_ret )
{
  // remove ".." from the request.
  unsigned pos;

  if( (pos = s_ret.find( ".." )) != string::npos)
    return s_ret.substr(0, pos);

  return s_ret;
}

#endif
