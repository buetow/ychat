#pragma warning(disable:4786)

#include "../incl.h"

#ifndef HMAP_H
#define HMAP_H

#include <vector>

using namespace std;

// void add_elem( obj_type x, key_type k )       --> Insert x
// void del_elem( key_type k )       --> Remove x
// obj_type get_elem( key_type k )  --> Return item that matches x
// void make_empty( )      --> Remove all items

template <class obj_type, class key_type>
class hmap
{
protected:
    enum entry_type
    {
        ACTIVE, EMPTY, DELETED
    };

    struct hash_entry
    {
        obj_type   element;
        key_type   key;
        entry_type info;
        int        hits;

        hash_entry( const obj_type &e = obj_type( ), const key_type &k = key_type( ), entry_type i = EMPTY ) : element( e ), key( k ), info( i )
        { }
    }
    ;

    int occupied;

    virtual bool is_active( int i_current_pos ) const;
    virtual void rehash( );
    virtual bool is_prime  ( int n ) const;
    virtual int  next_prime( int n ) const;
    double       i_max_occupied_percentage;

    int lookups;
    virtual unsigned int hash( const string &key ) const;
    vector<hash_entry> array;

    int get_lookups()
    {
        return lookups;
    };

    int get_capacity()
    {
        return array.size();
    };

    double get_lambda()
    {
        return static_cast<double>(get_size())/static_cast<double>(get_capacity());
    }

    obj_type& operator[]( key_type &k )
    {
        return get_elem( k );
    }

    int  find_pos  ( const key_type &k );

public:
    hmap( double moc );
    ~hmap( );

    virtual int get_size();
    virtual void make_empty( );
    virtual void make_empty( void (*func)(key_type) );
    virtual void del_elem ( const key_type &k );
    virtual obj_type set_elem ( const obj_type &x, const key_type &k );

    virtual void  run_func( void (*func)(obj_type) );
    virtual void  run_func( void (*func)(obj_type, void*), void* v_arg );
    virtual void  run_func_on( void (*func)(obj_type), const key_type & k );
    virtual vector<key_type>*  get_key_vector( );

    virtual void add_elem ( const obj_type &x, const key_type &k );

    virtual bool is_avail( const key_type &k );   
    virtual obj_type pop_elem ( const key_type &k );
    virtual obj_type get_elem ( const key_type &k );
    virtual obj_type operator[]( const key_type &k ) {
     return get_elem( k );
    }

};

#include "hmap.tmpl"

#endif
