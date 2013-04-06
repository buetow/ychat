#ifndef HTML_CPP
#define HTML_CPP

#include <fstream>
#include "html.h"

using namespace std;

html::html( ) : smap<string,string>::smap(HMAPOCC)
{
    set_name( wrap::CONF->get_elem( "httpd.templatedir" ) );
}

html::~html( )
{
}

void
html::clear_cache( )
{
    make_empty();
    wrap::system_message( CLRHTML );

#ifdef NCURSES 
    print_cached( 0 );
#endif
}

string
html::parse( map_string &map_params )
{
    string s_file = map_params["request"];

    // check if s_file is in the container.
    string s_templ;

    // if not, read file.
    if ( ! smap<string,string>::is_avail( s_file ) )
    {
        auto string   s_path  = get_name();
        auto ifstream fs_templ( s_path.append( s_file ).c_str(), ios::binary );

        if ( ! fs_templ )
        {
    	    wrap::system_message( OFFFOUND + s_path );
            if(map_params["request"]== wrap::CONF->get_elem( "httpd.html.notfound"  ))
                    return "";

            map_params["request"] = wrap::CONF->get_elem( "httpd.html.notfound" );
            return parse( map_params );

        }

        auto char c_buf;
        while( !fs_templ.eof() )
        {
            fs_templ.get( c_buf );
            s_templ+=c_buf;
        }

        fs_templ.close();

	wrap::system_message( TECACHE + s_path );

        // cache file.
        smap<string,string>::add_elem( s_templ, s_file ); 
#ifdef NCURSES 
        print_cached( smap<string,string>::get_size() );
#endif
    }

    else
    {
        s_templ = smap<string,string>::get_elem( s_file );
    }

    // find %%KEY%% token and substituate those.
    auto unsigned int pos[2];
    pos[0] = pos[1] = 0;

    for(;;)
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
        auto string s_val = wrap::CONF->get_elem( s_key );

        // if s_val is empty use map_params.
        if ( s_val.empty() )
            s_val = map_params[ s_key ];
        
        // substituate key with val.
        s_templ.replace( pos[0]-2, pos[1]-pos[0]+4, s_val );

        // calculate the string displacement.
        auto int i_dif = s_val.length() - ( pos[1] - pos[0] + 4);

        pos[1] += 2 + i_dif;

    };

    return s_templ;
}

//<<*
void
html::online_list( user *p_user, map_string &map_params )
{
    // prepare user_list.
    string s_list;

    room* p_room = p_user->get_room();

    p_room->get_user_list( s_list );

    map_params["ROOMNAME"] = p_room->get_name();
    map_params["ROOMTOPIC"] = p_room->get_topic();
    map_params["USERLIST"] = s_list;
}
//*>>

#ifdef NCURSES
void
html::print_cached( int i_docs )
{
    if ( !wrap::NCUR->is_ready() )
      return;

    mvprintw( NCUR_CACHED_DOCS_X, NCUR_CACHED_DOCS_Y, "Docs: %d ", i_docs);
    refresh();
}
#endif

#endif

