#ifndef HASH_CPP
#define HASH_CPP

#include "hash.h"

int
hash::operator()(string s_key) const
{
  int i_hash = 0;
  //  cout << key << "%";

  int i_size = s_key.size();

  for( size_t i = 0; i < i_size; ++i )
    i_hash =  ( i_hash << 5 ) ^ s_key.at(i) ^ i_hash;

  return i_hash;
}

bool
hashmap_allocator::operator()(string s_key_1, string s_key_2) const
{
  return s_key_1.compare(s_key_2) == 0;
}

#endif
