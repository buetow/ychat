#include "../incl.h"

#ifndef NMAP_H
#define NMAP_H

#include "smap.h"

template <class obj_type, class key_type>
class nmap : public smap<obj_type, key_type>
{
 public:
    nmap( double moc );
    ~nmap();

    // Override the get_elem method so that a new object
    // will be returned if the required element is not in
    // the map!
    obj_type get_elem ( const key_type &k );
};

#include "nmap.tmpl"
#endif 
