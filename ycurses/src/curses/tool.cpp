#ifndef TOOL_CPP
#define TOOL_CPP

#include "tool.h"

string
tool::int2string(int i_int)
{
  char buf[64];
  sprintf(buf, "%d", i_int);
  return buf;
}

int
tool::string2int(string s)
{
  const char *p = s.c_str();
  int i_res = 0;

  // Convert each digit char and add into result.
  while (*p >= '0' && *p <='9')
  {
    i_res = (i_res * 10) + (*p - '0');
    p++;
  }

  // Check that there were no non-digits at end.
  if (*p != 0)
  {
    return -1;
  }

  return i_res;
}

#endif

