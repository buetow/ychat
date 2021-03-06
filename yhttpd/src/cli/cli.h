#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>
#include <vector>

#ifndef	RUSAGE_SELF
#define RUSAGE_SELF 0
#endif
#ifndef	RUSAGE_CHILDREN
#define RUSAGE_CHILDREN -1
#endif

#include "../incl.h"

#ifndef CLI_H
#define CLI_H
#ifdef CLI

#ifndef NCURSES
#include "../thrd/thro.h"
#endif

#include "../monitor/dump.h"

using namespace std;

#ifndef NCURSES
class cli : public thro
{
#else
class cli
{
#endif
private:
  int parse_input(string s_input);
  vector<string> vectorize(string s_param);

public:
  cli( );
  ~cli( );

#ifdef DEBUG

  void debug_routine();
#endif

  void print_rusage();

#ifndef NCURSES

  void start(void* p_void);
#else

  void start();
#endif
};

#endif
#endif
