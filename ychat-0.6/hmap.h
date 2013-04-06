#pragma warning(disable:4786)

#ifndef hmap_h
#define hmap_h

#include <vector>
#include "incl.h"

using namespace std;

// void add_elem( obj_type x, key_type k )       --> Insert x
// void del_elem( key_type k )       --> Remove x
// obj_type get_elem( key_type k )  --> Return item that matches x
// void make_empty( )      --> Remove all items

template <class obj_type, class key_type>
class hmap
{
private:
    enum entry_type
    {
        ACTIVE, EMPTY, DELETED
    };

    struct hash_entry
    {
        obj_type   element;
        key_type   key;
        entry_type info;

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

protected:
    int lookups;
    unsigned int hash( const string &key ) const;
    vector<hash_entry> array;

public:
    hmap( double moc );

    virtual int  find_pos  ( const key_type &k );
    virtual void make_empty( );
    virtual void add_elem ( const obj_type &x, const key_type &k );
    virtual void del_elem ( const key_type &k );
    virtual obj_type get_elem ( const key_type &k );

    virtual void  run_func( void (*func)(obj_type) );
    virtual void  run_func( void (*func)(obj_type, void*), void* v_arg );

    virtual vector<key_type>*  get_key_vector( );
    
    // inline:
    void get_size()
    {
        int size = 0;
        for( int j = 0; j < array.size( ); j++ )
            if (array[ j ].info == ACTIVE)
                size++;
        return size;
    };

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

};

template <class obj_type, class key_type>
class linearhmap : public hmap<obj_type, key_type>
{
public:
    linearhmap(double moc) : hmap<obj_type, key_type>(moc)
    {}
    ;

    virtual int find_pos( const key_type &k )
    {
        int i_collision_num = 0;
        int i_current_pos = hash( k ) % array.size( );
        lookups++;

        while( array[ i_current_pos ].info != EMPTY &&
                array[ i_current_pos ].key  !=  k )
        {
            lookups   ++;
            i_current_pos++;

            if( i_current_pos >= array.size( ) )
                i_current_pos -= array.size( );
        }

        return i_current_pos;
    }
};

#include "hmap.cpp"

#endif
