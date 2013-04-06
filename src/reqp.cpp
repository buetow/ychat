// class reqp implementation.

#ifndef REQP_CXX
#define REQP_CXX

#include "reqp.h"
#include "s_chat.h"
#include "s_html.h"
#include "s_mutx.h"
#include "s_sock.h"
#include "s_tool.h"
using namespace std;

// inititialization of static members.
string reqp::HTTP_CODEOK = "HTTP/1.1 200 OK\n";
string reqp::HTTP_SERVER = "Server: yChat (Unix)\n";
string reqp::HTTP_CONTAC = "Contact: www.yChat.org\n";
string reqp::HTTP_CACHEC = "Expires: 0\nCache-control: no-cache\nPragma: no-cache\n";
string reqp::HTTP_CONNEC = "Connection: keep-alive\n";
string reqp::HTTP_COTYPE = "Content-Type: ";

reqp::reqp( )
{}

string
reqp::get_url( thrd* p_thrd, string s_req, map_string &map_params )
{
  int i_sock = p_thrd->get_sock();
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
          char c_req[1024];
          i_len = read(i_sock, c_req, 1024);
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

  pthread_mutex_lock  ( &s_mutx::get
                          ().mut_stdout );
  cout << REQUEST << s_ret << endl;
  pthread_mutex_unlock( &s_mutx::get
                          ().mut_stdout );
#endif

  if ( s_ret.empty() )
    s_ret = s_conf::get
              ().get_val( "STARTMPL" );

  else
    s_ret = remove_dots(s_ret);

  map_params["request"] = s_ret;

  return s_ret;
}

/*
string
reqp::get_url( thrd* p_thrd, string s_req, map_string &map_params )
{
  auto unsigned int pos;
  string s_ret ( "" );
  string s_vars( "" );
  auto int i_request;

  i_request= ( s_req.find("GET",0) != string::npos ) ? RQ_GET : RQ_POST;

  pos = s_req.find( "HTTP", 0 );

  if( i_request == RQ_GET )
    s_ret.append( s_req.substr( 5, pos-6 ) );
  else
    s_ret.append( s_req.substr( 6, pos-7 ) );

  // remove ".." from the request.
  do
  {
    pos = s_ret.find( "../", 0 );

    if ( pos == string::npos )
      break;

    s_ret.replace( pos, pos+2, "" );
  }
  while( true );

  // do not add the string behind "?" tp s_ret and add all params behind "?" to map_params.
  if( i_request == RQ_GET )
    pos = s_ret.find( "?", 0 );
  else
    pos = s_req.find("\r\n\r\n", 0);

  auto string s_params( "" );
  if ( pos != string::npos )
  {
    if( i_request == RQ_GET )
      s_params.append( s_ret.substr( pos+1, s_ret.length() -pos-1 ) );

    else
      s_params = s_req.substr( pos+4, s_req.length() -pos-1 );

    s_ret = s_ret.substr( 0, pos );
  }

  if ( i_request == RQ_POST && s_params.empty() )
  {
    char c_req[READBUF];

    if ( read ( p_thrd->get_sock() , c_req, READBUF ) <= 0 )
      return "NOBYTE";

    s_params = string( strstr( c_req, "event" ) );
  }

  auto unsigned int pos2;
  do
  {
    pos  = s_params.find( "=", 0 );
    if ( pos == string::npos )
      break;

    pos2 = s_params.find( "&", 0 );
    if ( pos2 == string::npos )
    {
      auto string sValue( s_params.substr(pos+1, s_params.length()-pos-1) );
      auto string tmpstr( url_decode(sValue) );
      map_params[ s_params.substr( 0, pos ) ] = tmpstr;
      break;
    }

    auto string s_temp= s_params.substr( pos+1, pos2-pos-1 );
    map_params[ s_params.substr( 0, pos ) ] = url_decode(s_temp);

    s_params = s_params.substr( pos2+1, s_params.length()-pos2-1 );
  }
  while( true );

#ifdef VERBOSE

  pthread_mutex_lock  ( &s_mutx::get
                          ().mut_stdout );
  cout << REQUEST << s_ret << endl;
  pthread_mutex_unlock( &s_mutx::get
                          ().mut_stdout );
#endif

  if ( s_ret.empty() )
    s_ret = s_conf::get
              ().get_val( "STARTMPL" );

  map_params["request"] = s_ret;

  return s_ret;
}
*/

string
reqp::get_content_type( string s_file )
{
  string s_ext=s_tool::getExtension( s_file );

  if(s_ext=="")
    s_ext="DEFAULT";

  return s_conf::get
           ().get_val( "CT_"+s_ext );
}
void
reqp::parse_headers( string s_req, map_string &map_params )
{
  int pos = s_req.find("\n");
  if(pos!=string::npos)
    map_params["QUERY_STRING"]=s_tool::trim(s_req.substr(0,pos-1));

  while(pos!=string::npos)
  {
    auto string s_line=s_req.substr(0,pos);
    auto int pos2=s_line.find(":");
    if(pos2!=string::npos)
    {
      auto string key=s_tool::trim(s_line.substr(0, pos2));
      auto string value=s_tool::trim(s_line.substr(pos2+1));

      map_params[key]=value;


    }
    s_req=s_req.substr(s_line.size()+1);
    pos=s_req.find("\n");
  }

}


int
reqp::htoi(string *s)
{
  int value;
  int c;

  c=s->c_str()[0];
  if(isupper(c))
    c=tolower(c);

  value=(c>='0' && c<='9'?c-'0':c-'a'+10)*16;

  c=s->c_str()[1];
  if(isupper(c))
    c=tolower(c);

  value+=c>='0' && c<='9'?c-'0':c-'a'+10;
  return value;
}

string
reqp::url_decode( string s_str )
{
  auto string sDest="";
  int len = s_str.size();

  for(int i=0;i<len;i++)
  {
    char ch = s_str.at(i);
    if(ch=='+')
    {
      sDest+=" ";
    }
    else if(ch=='%')
    {
      auto string sTmp=s_str.substr(i+1,2);
      ch=(char)htoi(&sTmp);
      sDest+=ch;
      i+=2;

    }
    else

      sDest+=ch;
  }
  return sDest;
}

string
reqp::get_from_header( string s_req, string s_hdr )
{
  auto unsigned int pos[2];
  pos[0] = s_req.find( s_hdr, 0 );
  pos[1] = s_req.find( "\n", pos[0] );

  auto int i_length = s_hdr.length();
  return s_req.substr( pos[0]+i_length, pos[1]-pos[0]-i_length-1 );
}

string
reqp::parse( thrd* p_thrd, string s_req, map_string &map_params )
{

  // store all request informations in map_params. store the url in
  // map_params["request"].

  if ( get_url( p_thrd, s_req, map_params ).compare("NOBYTE") == 0 )
    map_params["request"] = s_conf::get
                              ().get_val("NOTFOUND");

  parse_headers( s_req, map_params );
  // create the http header.
  string s_rep( HTTP_CODEOK );
  s_rep.append( HTTP_SERVER );
  s_rep.append( HTTP_CONTAC );
  s_rep.append( HTTP_CACHEC );
  s_rep.append( HTTP_CONNEC );
  s_rep.append( HTTP_COTYPE );
  s_rep.append( get_content_type( map_params["request"] ) );
  s_rep.append("\r\n\r\n");

  // check the event variable.

  string s_event( map_params["event"] );
  if ( ! s_event.empty() )
  {
    // login procedure.
    if ( s_event == "login" )
    {
      s_chat::get
        ().login( map_params );
    }

    else
    {
      bool b_found;

      //   user* p_user = s_chat::get().get_user( map_params["nick"], b_found );
      sess *sess_temp=s_sman::get
                        ().getSession( map_params["tmpid"] );
      user *p_user;
      if(sess_temp!=NULL)
      {
        string *s_nick=static_cast<string*>(sess_temp->getValue(string("nick")));
        p_user = s_chat::get
                   ().get_user( *s_nick, b_found);
      }
      else
        return s_rep;
      if ( ! b_found )
      {
        map_params["INFO"]    = E_NOTONL;
        map_params["request"] = s_conf::get
                                  ().get_val( "STARTMPL" ); // redirect to the startpage.
      }
      // if a message post.
      else if ( s_event == "post" )
        s_chat::get
          ().post( p_user, map_params );


      // if a chat stream
      else if ( s_event == "stream" )
      {
        string s_msg(s_html::get
                       ().parse( map_params ) );
        p_user->msg_post( &s_msg);
        s_sock::get
          ().chat_stream( p_thrd->get_sock(), p_user, map_params );
      }

      // if a request for the online list of the active room.
      else if ( s_event == "online" )
        s_html::get
          ().online_list( p_user, map_params );
    }
  }

  // parse and get the requested html-template and also use
  // the values stored in map_params for %%KEY%% substituations.
  s_rep.append( s_html::get
                  ().parse( map_params ) );

  // return the parsed html-template.
  return  s_rep;
}

string
reqp::remove_dots( string s_ret )
{
  // remove ".." from the request.
  unsigned pos;

  if( (pos = s_ret.find( ".." )) != string::npos)
    return s_ret.substr(0, pos);

  return s_ret;
}

void
reqp::get_request_parameters( string s_parameters, map<string,string>& map_params )
{
  string s_tmp;
  unsigned i_pos, i_pos2;

  while( (i_pos = s_parameters.find("&")) != string::npos )
  {
    s_tmp = s_parameters.substr(0, i_pos );

    if ( (i_pos2 = s_tmp.find("=")) != string::npos )
      map_params[ s_tmp.substr(0, i_pos2) ] = s_tool::replace( s_tmp.substr( i_pos2+1 ), "\\AND", "&");

    s_parameters = s_parameters.substr( i_pos + 1 );
  }

  // Get the last request parameter, which does not have a "&" on the end!
  if( (i_pos = s_parameters.find("=")) != string::npos )
    map_params[ s_parameters.substr(0, i_pos) ] = s_parameters.substr( i_pos+1 );

  //map<string,string>::iterator iter;
  //for ( iter = map_params.begin(); iter != map_params.end(); ++iter )
  //cout << ">>>" << iter->first << "=" << iter->second << endl;
}


#endif
