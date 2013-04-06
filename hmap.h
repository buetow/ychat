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

  hash_entry( const obj_type &e = obj_type( ), const key_type &k = key_type( ), entry_type i = EMPTY ) : element( e ), key( k ), info( i ) { }
 };
    
 int occupied;
	
 virtual bool isActive( int currentPos ) const;
 virtual void rehash( );
 virtual bool isPrime  ( int n ) const;
 virtual int  nextPrime( int n ) const;
 double       maxOccupiedPercentage; 

protected:
 int lookups;
 unsigned int hash( const string &key ) const;
 vector<hash_entry> array;

public:
 hmap( double moc );

 virtual int  findPos  ( const key_type &k );
 virtual void make_empty( );
 virtual void add_elem ( const obj_type &x, const key_type &k );
 virtual void del_elem ( const key_type &k );
 virtual obj_type get_elem ( const key_type &k );

 virtual void  run_func( void (*func)(obj_type, void*), void* v_arg );

 // inline:
 void getSize() 
 {
  int size = 0;
  for( int j = 0; j < array.size( ); j++ )
   if (array[ j ].info == ACTIVE)
    size++;
   return size;	
 };

 int getLookups()
 {
  return lookups;
 };

 int getCapacity()
 {
  return array.size();
 };

 double getLambda()
 {
  return static_cast<double>(getSize())/static_cast<double>(getCapacity());
 }

};

template <class obj_type, class key_type>
class linearhmap : public hmap<obj_type, key_type> {
public:
 linearhmap(double moc) : hmap<obj_type, key_type>(moc) {};

 virtual int findPos( const key_type &k )
 {
  int collisionNum = 0;
  int currentPos = hash( k ) % array.size( );
  lookups++;

  while( array[ currentPos ].info != EMPTY &&
         array[ currentPos ].key  !=  k )
  {	
   lookups   ++;
   currentPos++; 

   if( currentPos >= array.size( ) )
    currentPos -= array.size( );
  }

  return currentPos;
 }
};

#include "hmap.cpp"

#endif
