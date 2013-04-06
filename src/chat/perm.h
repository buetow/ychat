#include "../incl.h"
#include "../maps/smap.h"

#ifndef PERM_H
#define PERM_H

using namespace std;

template<class type_>
struct valwrap 
{ 
 type_ val; 
 valwrap(type_ i) 
 {
  val = i;
 }
 type_ get_val() 
 {
  return val;
 }
};

class perm
{
private:
 smap< valwrap<int>*, string>* p_command_status;
 smap< valwrap<bool>*, string>* p_command_disabled;

 void set_standard_command_permissions();
 
public:
 perm( );
 ~perm( );

 virtual int  get_command_status( string s_command );
 virtual bool get_command_disabled( string s_command );
 virtual void set_command_status( string s_command, int i_int );
 virtual void set_command_disabled( string s_command, bool b_bool );
};

#endif
