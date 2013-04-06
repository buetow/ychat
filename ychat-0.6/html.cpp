// class html implementation.

#ifndef s_html_CXX
#define s_html_CXX

#include <fstream>
#include "html.h"
#include "s_chat.h"
#include "s_mutx.h"
#include "s_ncur.h"

using namespace std;

html::html( )
{
    set_name( s_conf::get
                  ().get_val( "HTMLTEMP" ) );
    pthread_mutex_init( &mut_map_vals, NULL );
}

html::~html( )
{
    pthread_mutex_destroy( &mut_map_vals );
}

void
html::clear_cache( )
{
    pthread_mutex_lock  ( &mut_map_vals );
    clear_vals();
    pthread_mutex_unlock( &mut_map_vals );
}

string
html::parse( map_string &map_params )
{
    string s_file = map_params["request"];

    // check if s_file is in the container.
    pthread_mutex_lock  ( &mut_map_vals );
    string s_templ = get_val( s_file );
    pthread_mutex_unlock( &mut_map_vals );

    // if not, read file.
    if ( s_templ.empty() )
    {
        auto string   s_path  = get_name();
        auto ifstream fs_templ( s_path.append( s_file ).c_str(), ios::binary );

        if ( ! fs_templ )
        {

#ifdef VERBOSE
            cerr << NOFOUND << s_path << endl;
#endif
#ifdef NCURSES

            {
                string s_tmp( NOFOUND );
                s_tmp.append( s_path );
                s_ncur::get
                    ().print( s_tmp );
            }
#endif
            if(map_params["request"]==s_conf::get
                        ().get_val( "NOTFOUND"  ))
                    return "";

            map_params["request"] = s_conf::get
                                        ().get_val( "NOTFOUND" );
            return parse( map_params );

        }

        auto char c_buf;
        while( !fs_templ.eof() )
        {
            fs_templ.get( c_buf );
            s_templ+=c_buf;
        }

        fs_templ.close();

#ifdef VERBOSE

        pthread_mutex_lock  ( &s_mutx::get
                                  ().mut_stdout );
        cout << TECACHE << s_path << endl;
        pthread_mutex_unlock( &s_mutx::get
                                  ().mut_stdout );
#endif
#ifdef NCURSES

        {
            string s_tmp( TECACHE );
            s_tmp.append( s_path );
            s_ncur::get
                ().print( s_tmp.c_str() );
        }
#endif



        // cache file.
        pthread_mutex_lock  ( &mut_map_vals );
        map_vals[ s_file ] = s_templ;
        pthread_mutex_unlock( &mut_map_vals );
    }

    // find %%KEY%% token and substituate those.
    auto unsigned int pos[2];
    pos[0] = pos[1] = 0;

    do
    {
        pos[0] = s_templ.find( "%%", pos[1] );

        if ( pos[0] == string::npos )
            break;

        pos[0] += 2;
        pos[1]  = s_templ.find( "%%", pos[0] );

        if ( pos[0] == string::npos )
            break;

        // get key and val.
        auto string s_key = s_templ.substr( pos[0], pos[1]-pos[0] );
        auto string s_val = s_conf::get
                                ().get_val( s_key );

        // if s_val is empty use map_params.
        if ( s_val.empty() )
            s_val = map_params[ s_key ];
        
        // substituate key with val.
        s_templ.replace( pos[0]-2, pos[1]-pos[0]+4, s_val );

        // calculate the string displacement.
        auto int i_dif = s_val.length() - ( pos[1] - pos[0] + 4);

        pos[1] += 2 + i_dif;

    }
    while( true );

    return s_templ;
}

void
html::online_list( user *p_user, map_string &map_params )
{
    // prepare user_list.
    string s_list     ( ""     );
    string s_seperator( "<br>" );

    p_user->get_room()->get_user_list( s_list, s_seperator );

    // use the collected data as a message in html-templates.
    map_params["MESSAGE"] = s_list;

    // renew the timestamp.
    p_user->renew_stamp();

    // send a ping to the client chat stream.
    p_user->msg_post( new string("\n") );
}

#endif

