#ifndef hmap_cpp
#define hmap_cpp

#include "hmap.h"

using namespace std;

bool  isPrime( int n );
int nextPrime( int n );

// Construct the hash table.
template <class obj_type, class key_type>
hmap<obj_type, key_type>::hmap( double mop )
              : maxOccupiedPercentage(mop), array( nextPrime( 101 ) )
{
 cout << "hmap Constructor" << endl;
 lookups = 0;
 make_empty( );
}

// Insert item x into the hash table. If the item is
// already present, do nothing
template <class obj_type, class key_type>
void hmap<obj_type, key_type>::add_elem( const obj_type &x, const key_type &k  )
{
 // Insert x as active
 int currentPos = findPos( k );
 if( isActive( currentPos ) )
  return;

 array[ currentPos ] = hash_entry( x, k, ACTIVE );
 //	cout << "Inserted=" << x << "= at " << currentPos << endl;
 if( ++occupied > array.size( ) * maxOccupiedPercentage )
  rehash( );
}

// Expand the hash table.
template <class obj_type, class key_type>
void hmap<obj_type, key_type>::rehash( )
{
 vector<hash_entry> oldArray = array;

 // Create new double-sized, empty table
 array.resize( nextPrime( 2 * oldArray.size( ) ) );
 for( int j = 0; j < array.size( ); j++ )
  array[ j ].info = EMPTY;

 // Copy table over
 make_empty( );
 for( int i = 0; i < oldArray.size( ); i++ )
  if( oldArray[ i ].info == ACTIVE )
   add_elem( oldArray[ i ].element, oldArray[ i ].key );
}

// Hash function, can only handle strings.
// If you want to hash other objects you will have to 
// create a hash table for them
template <class obj_type, class key_type>
unsigned int hmap<obj_type, key_type>::hash( const string & key ) const
{
 unsigned int hashVal = 0;
 //	cout << key << "%";

 for( size_t i = 0; i < key.size(); i++ )
  hashVal =  ( hashVal << 5 ) ^ key[ i ] ^ hashVal;

 return hashVal;
}

// Method that performs quadratic probing resolution.
// Return the position where the search for x terminates.
template <class obj_type, class key_type>
int hmap<obj_type, key_type>::findPos( const key_type &k )
{
 int collisionNum = 0;
 int currentPos = hash( k ) % array.size( );
 lookups++;

 while( array[ currentPos ].info != EMPTY &&
        array[ currentPos ].key != k )
 {
 //		cout <<  array[ currentPos ].element << "!=" << x << endl;
  lookups++;
  currentPos += 2 * ++collisionNum - 1;  // Compute ith probe

  if( currentPos >= array.size( ) )
   currentPos -= array.size( );
 }

 //	cout << currentPos << " ";
 return currentPos;
}

// Remove item x from the hash table.
template <class obj_type, class key_type>
void hmap<obj_type, key_type>::del_elem( const key_type & k )
{
    int currentPos = findPos( k );
    if( isActive( currentPos ) )
        array[ currentPos ].info = DELETED;
}

// Find item x in the hash table.
// Return a pointer to the matching item or 0 if not found
template <class obj_type, class key_type>
obj_type hmap<obj_type, key_type>::get_elem( const key_type &k )
{
 int currentPos = findPos( k );
 if( isActive( currentPos ) )
  return array[ currentPos ].element;
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

// Return true if currentPos exists and is active.
template <class obj_type, class key_type>
bool hmap<obj_type, key_type>::isActive( int currentPos ) const
{
 return array[ currentPos ].info == ACTIVE;
}


// Internal method to test if a positive number is prime.
// Not an efficient algorithm.
template <class obj_type, class key_type>
bool hmap<obj_type, key_type>::isPrime( int n ) const 
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
int hmap<obj_type, key_type>::nextPrime( int n ) const
{
 if( n % 2 == 0 )
  n++;

 for( ; !isPrime( n ); n += 2 );

 return n;
}
template<class obj_type, class key_type> void
hmap<obj_type, key_type>::run_func( void (*func)(obj_type, void*), void* v_arg )
{
 for( int i = 0; i < array.size( ); i++ )
  if ( array[i].info == ACTIVE )
   ( *func ) ( array[i].element, v_arg );
}

#endif

