// class html declaration. this class manages the html-template files.
#include "incl.h"

#ifndef HTML_H
#define HTML_H

#include "maps/shashmap.h"
#include "chat/user.h"
#include "name.h"

using namespace std;

class html : public shashmap<string>, name
{
public:
  html( );
  ~html( );

  // Clears the template cache so that new html templates will be read
  // from hard disk. This method is needed after changeing s.t. on
  // the html-template files.
  void clear_cache( );

  // Returns a parsed html-template. this method will check first if the
  // required html-template exists inside the classes template cache. if not
  // then the file will be read from file and added to the cache.
  // afterwards the html-template will be parsed and returned.
  // map_params contains the client request parameters which also will be
  // used for string substituation.
  string parse( hashmap<string> &map_params );

  void online_list( user *p_user, hashmap<string> &map_params ); //<<
#ifdef NCURSES

  void print_cached( int i_docs );
#endif

};

#endif
