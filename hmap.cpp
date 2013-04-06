#ifndef hmap_cpp
#define hmap_cpp


#include "hmap.h"

using namespace std;

bool  is_prime( int n );
int next_prime( int n );

// Construct the hash table.
template <class obj_type, class key_type>
hmap<obj_type, key_type>::hmap( double mop )
        : i_max_occupied_percentage(mop), array( next_prime( 101 ) )
{
    lookups = 0;
    make_empty( );
}

// Insert item x into the hash table. If the item is
// already present, do nothing
template <class obj_type, class key_type>
void hmap<obj_type, key_type>::add_elem( const obj_type &x, const key_type &k  )
{
    // Insert x as active
    int i_current_pos = find_pos( k );
    if( is_active( i_current_pos ) )
        return;

    array[ i_current_pos ] = hash_entry( x, k, ACTIVE );
    if( ++occupied > array.size( ) * i_max_occupied_percentage )
        rehash( );
}

// Expand the hash table.
template <class obj_type, class key_type>
void hmap<obj_type, key_type>::rehash( )
{
    vector<hash_entry> old_array = array;

    // Create new double-sized, empty table
    array.resize( next_prime( 2 * old_array.size( ) ) );
    for( int j = 0; j < array.size( ); j++ )
        array[ j ].info = EMPTY;

    // Copy table over
    make_empty( );
    for( int i = 0; i < old_array.size( ); i++ )
        if( old_array[ i ].info == ACTIVE )
            add_elem( old_array[ i ].element, old_array[ i ].key );
}

// Hash function, can only handle strings.
// If you want to hash other objects you will have to
// create a hash table for them
template <class obj_type, class key_type>
unsigned int hmap<obj_type, key_type>::hash( const string & key ) const
{
    unsigned int hash_value = 0;
    //	cout << key << "%";

    for( size_t i = 0; i < key.size(); i++ )
        hash_value =  ( hash_value << 5 ) ^ key[ i ] ^ hash_value;

    return hash_value;
}

// Method that performs quadratic probing resolution.
// Return the position where the search for x terminates.
template <class obj_type, class key_type>
int hmap<obj_type, key_type>::find_pos( const key_type &k )
{
    int i_collision_num = 0;
    int i_current_pos = hash( k ) % array.size( );
    lookups++;

    while( array[ i_current_pos ].info != EMPTY &&
            array[ i_current_pos ].key != k )
    {
        //		cout <<  array[ i_current_pos ].element << "!=" << x << endl;
        lookups++;
        i_current_pos += 2 * ++i_collision_num - 1;  // Compute ith probe

        if( i_current_pos >= array.size( ) )
            i_current_pos -= array.size( );
    }

    //	cout << i_current_pos << " ";
    return i_current_pos;
}

// Remove item x from the hash table.
template <class obj_type, class key_type>
void hmap<obj_type, key_type>::del_elem( const key_type & k )
{
    int i_current_pos = find_pos( k );
    if( is_active( i_current_pos ) )
        array[ i_current_pos ].info = DELETED;
}

// Find item x in the hash table.
// Return a pointer to the matching item or 0 if not found
template <class obj_type, class key_type>
obj_type hmap<obj_type, key_type>::get_elem( const key_type &k )
{
    int i_current_pos = find_pos( k );
    if( is_active( i_current_pos ) )
        return array[ i_current_pos ].element;
    else
        return 0;
}

// Make the hash table logically empty.
template <class obj_type, class key_type>
void hmap<obj_type, key_type>::make_empty( )
{
    occupied = 0;
    for( int i = 0; i < array.size( ); i++ )
        array[ i ].info = EMPTY;
}

// Return true if i_current_pos exists and is active.
template <class obj_type, class key_type>
bool hmap<obj_type, key_type>::is_active( int i_current_pos ) const
{
    return array[ i_current_pos ].info == ACTIVE;
}


// Internal method to test if a positive number is prime.
// Not an efficient algorithm.
template <class obj_type, class key_type>
bool hmap<obj_type, key_type>::is_prime( int n ) const
{
    if( n == 2 || n == 3 )
        return true;

    else if( n == 1 || n % 2 == 0 )
        return false;

    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;

    return true;
}

// Internal method to return a prime number at least as large as n.
// Assumes n > 0.
template <class obj_type, class key_type>
int hmap<obj_type, key_type>::next_prime( int n ) const
{
    if( n % 2 == 0 )
        n++;

    for( ; !is_prime( n ); n += 2 )
        ;

    return n;
}
template<class obj_type, class key_type>
void
hmap<obj_type, key_type>::run_func( void (*func)(obj_type) )
{
    for( int i = 0; i < array.size( ); i++ )
        if ( array[i].info == ACTIVE )
            ( *func ) ( array[i].element );
}

template<class obj_type, class key_type>
void
hmap<obj_type, key_type>::run_func( void (*func)(obj_type, void*), void* v_arg )
{
    for( int i = 0; i < array.size( ); i++ )
        if ( array[i].info == ACTIVE )
            ( *func ) ( array[i].element, v_arg );
}

template<class obj_type, class key_type>
vector<key_type>*
hmap<obj_type, key_type>::get_key_vector()
{
    vector<key_type>* p_vec = new vector<key_type>;
    for( int i = 0; i < array.size( ); i++ )
        if ( array[i].info == ACTIVE )
 	    p_vec->push_back( array[i].key );	 	

    return p_vec;
}



#endif

