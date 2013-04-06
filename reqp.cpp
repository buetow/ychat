// class reqp implementation.

#ifndef REQP_CXX
#define REQP_CXX

#include "reqp.h"
#include "CHAT.h"
#include "HTML.h"
#include "MUTX.h"

using namespace std;

// inititialization of static members.
string reqp::HTTP_CODEOK = "HTTP/1.1 200 OK\n";
string reqp::HTTP_SERVER = "Server: yChat (Unix)\n";
string reqp::HTTP_CONTAC = "Contact: www.yChat.org\n";
string reqp::HTTP_CACHEC = "Cash-control: no-cache\n";
string reqp::HTTP_CONNEC = "Connection: keep-alive\n";
string reqp::HTTP_COTYPE = "Content-Type: text/html\n\n";

reqp::reqp( )
{
#ifdef VERBOSE
 cout << "reqp::reqp()" << endl;
#endif
}

string
reqp::get_url( string s_req, map_string &map_params )
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "reqp::get_url( s_req )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 auto unsigned int pos = s_req.find( "HTTP", 0 );
 string s_ret = s_req.substr( 5, pos-6 );

 // remove ".." from the request.
 do
 {
  pos = s_ret.find( "..", 0 );

  if ( pos == string::npos )
   break; 

  s_ret.replace( pos, pos+2, "" );
 }
 while( true );

 // do not add the string behind "?" tp s_ret and add all params behind "?" to map_params. 
 pos = s_ret.find( "?", 0 );
 if ( pos != string::npos )
 {
  auto string s_params = s_ret.substr( pos+1, s_ret.length() -pos-1 ); 
  s_ret = s_ret.substr( 0, pos );

  auto unsigned int pos2;
  do
  {
   pos  = s_params.find( "=", 0 );
   if ( pos == string::npos )
    break;

   pos2 = s_params.find( "&", 0 );
   if ( pos2 == string::npos )
   {
    map_params[ s_params.substr( 0, pos ) ] = s_params.substr( pos+1, s_params.length()-pos-1 );
    break;
   }

   map_params[ s_params.substr( 0, pos ) ] = s_params.substr( pos+1, pos2-pos-1 );
   s_params = s_params.substr( pos2+1, s_params.length()-pos2-1 );
  }
  while( true );

 }

#ifdef _VERBOSE
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << REQUEST << s_ret << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 map_params["request"] = s_ret;

 return s_ret;
}

string
reqp::get_from_header( string s_req, string s_hdr )
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "reqp::get_from_header( s_req, \"" << s_hdr << "\" )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 auto unsigned int pos[2];
 pos[0] = s_req.find( s_hdr, 0 );
 pos[1] = s_req.find( "\n", pos[0] );

 auto int i_length = s_hdr.length();
 return s_req.substr( pos[0]+i_length, pos[1]-pos[0]-i_length-1 );
}

string
reqp::parse( string s_req, map_string &map_params )
{
#ifdef VERBOSE_
 pthread_mutex_lock  ( &MUTX::get().mut_stdout );
 cout << "reqp::parse( s_req )" << endl;
 pthread_mutex_unlock( &MUTX::get().mut_stdout );
#endif

 // create the http header.
 string s_rep( HTTP_CODEOK ); s_rep.append( HTTP_SERVER );
 s_rep.append( HTTP_CONTAC ); s_rep.append( HTTP_CACHEC );
 s_rep.append( HTTP_CONNEC ); s_rep.append( HTTP_COTYPE );

 // store all request informations in map_params. store the url in 
 // map_params["request"].
 get_url( s_req, map_params ); 

 // check the event variable.
 string s_event( map_params["event"] );
 if ( ! s_event.empty() )
 {
  // login procedure.
  if ( s_event == "login" )
  {
   CHAT::get().login( map_params ); 
  }

  else
  {
   bool b_found;
   user* u_user = CHAT::get().get_user( map_params["nick"], b_found );

   if ( ! b_found )
   {
     map_params["INFO"]    = E_NOTONL;
     map_params["request"] = CONF::get().get_val( "STARTMPL" ); // redirect to the startpage.
   }

   // if a message post.
   else if ( s_event == "post" )
    CHAT::get().post( u_user, map_params );

   // if a request for the online list of the active room.
   else if ( s_event == "online" )
    HTML::get().online_list( u_user, map_params );
  }
 }

 // parse and get the requested html-template and also use
 // the values stored in map_params for %%KEY%% substituations. 
 s_rep.append( HTML::get().parse( map_params ) );

 // return the parsed html-template.
 return  s_rep;
}

#endif
