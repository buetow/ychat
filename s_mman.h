#ifndef GMMAN_H
#define GMMAN_H

#include "mman.h"
#include "s_conf.h"
#include "s_tool.h"

using namespace std;

class s_mman
{
private:
    static mman* obj;

public:
    static void init()
    {
        obj = new mman( s_tool::string2int(s_conf::get
                                               ().get_val( "MIN_CONNECTIONS" )), s_tool::string2int(s_conf::get
                                                           ().get_val( "MAX_CONNECTIONS" )));
        obj->init( s_conf::get
                       ().get_val( "MYSQL_HOST" ), s_conf::get
                           ().get_val( "MYSQL_USER" ), s_conf::get
                               ().get_val( "MYSQL_PASS" ),
                               s_conf::get
                                   ().get_val( "MYSQL_DB" ), s_tool::string2int(s_conf::get
                                               ().get_val( "MYSQL_PORT" )) );
    }

    static mman& get
        ()
    {
        return *obj;
    }
};


#endif
