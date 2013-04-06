// class html declaration. this class manages the html-template files.

#ifndef s_html_H
#define s_html_H

#include "incl.h"
#include "cont.h"
#include "s_conf.h"
#include "user.h"
#include "name.h"


using namespace std;

class html : public cont, name
{
private:
    // needed for synchronizing the map_vals.
    pthread_mutex_t mut_map_vals;

public:
    // public methods.
    explicit html( ); // simple constructor.
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

    void online_list( user *p_user, map_string &map_params );
};

#endif
