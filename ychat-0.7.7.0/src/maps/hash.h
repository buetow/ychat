#ifndef HASH_H
#define HASH_H

#include <string>

using namespace std;

struct hash
{
  int operator()(string s_key) const;
};

struct hashmap_allocator
{
  bool operator()(string s_key_1, string s_key_2) const;
};

#endif
