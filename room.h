// class room declaration.

#ifndef ROOM_H
#define ROOM_H

#include "incl.h"
#include "data.h"
#include "name.h"
#include "user.h"

using namespace std;

class room : public data<user>, public name
{
private:
 
public:
 void add_user( user* p_user  )
 {
  p_user->set_p_room( this );
  add_elem( p_user );
 }

 user* get_user( string &s_name, bool &b_found )
 {
  return static_cast<user*>( get_elem( s_name, b_found ) );
 }

 // public methods: 
  explicit room( string s_name );      // a constructor.
 ~room();  			       // room destructor.	
};

#endif
