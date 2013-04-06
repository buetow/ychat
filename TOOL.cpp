#ifndef TOOL_CXX
#define TOOL_CXX

#include <time.h>
#include "TOOL.h"

int
TOOL::string2int( string s_digit )
{
 auto const char *digit = s_digit.c_str();
 int result = 0;

 // Convert each digit char and add into result.
 while (*digit >= '0' && *digit <='9') {
  result = (result * 10) + (*digit - '0');
  digit++;
 }

 // Check that there were no non-digits at end.
 if (*digit != 0) {
  return -1;
 }

 return result;
}

long
TOOL::unixtime()
{
 return (long) time( NULL );
}

#endif
