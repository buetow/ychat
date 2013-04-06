#ifndef LANG_CPP
#define LANG_CPP

#include <fstream>
#include "lang.h"

using namespace std;

lang::lang( string s_lang = "en" ) : nmap<string,string>::nmap(HMAPOCC), name::name( s_lang )
{
    parse( ); // parse the config file.
}

lang::~lang()
{}

void
lang::parse()
{
  //  wrap::system_message( CFILEOK + get_name() );

    string filename(wrap::CONF->get_elem("LANGUAGE_DIR"));
    filename.append(get_name()+".txt");

    ifstream fs_conf( filename.c_str() );

    if ( ! fs_conf )
    {
    //    wrap::system_message( CFILEOFF + get_name() );
        return;
    }

    char c_buf[READBUF];

    while( fs_conf.getline( c_buf, READBUF ) )
    {
        string s_token( c_buf );
        unsigned int ui_pos = s_token.find( "#", 0 );

        // if line is commented out:
        if ( ui_pos == 0 )
            continue;

        ui_pos = s_token.find( ";", 0 );

        // if token has not been found.
        if ( ui_pos == string::npos )
            continue;

        s_token = s_token.substr( 0   , --ui_pos );
        ui_pos  = s_token.find  ( "\"", 0        );

        if ( ui_pos == string::npos )
            continue;

        string s_val = s_token.substr( ui_pos+1, s_token.length() );
        string s_key = s_token.substr( 0      , --ui_pos          );

        // fill the map.
        nmap<string,string>::add_elem(s_val, s_key );
    }

    fs_conf.close();
    fs_conf.~ifstream();
}

#endif
