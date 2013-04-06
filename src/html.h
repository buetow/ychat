// class html declaration. this class manages the html-template files.
#include "incl.h"

#ifndef HTML_H
#define HTML_H

#include "maps/smap.h"
#include "chat/user.h"
#include "name.h"

using namespace std;

class html : public smap<string, string>, name
{
private:

public:
    // public methods.
    html( ); // simple constructor.
    ~html( );

    // clears the template cache so that new html templates will be read
    // from hard disk. this method is needed after changeing s.t. on
    // the html-template files.
    void clear_cache( );

    // returns a parsed html-template. this method will check first if the
    // required html-template exists inside the classes template cache. if not
    // then the file will be read from file and added to the cache.
    // afterwards the html-template will be parsed and returned.
    // map_params contains the client request parameters which also will be
    // used for string substituation.
    string parse( map_string &map_params );

    void online_list( user *p_user, map_string &map_params ); //<<
#ifdef NCURSES
    void print_cached( int i_docs );
#endif
};

#endif
