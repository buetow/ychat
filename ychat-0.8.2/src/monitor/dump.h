#include "../incl.h"

#ifndef DUMP_H
#define DUMP_H

using namespace std;

class dumpable
{
private:
  int i_level;
  int i_lined;
  bool b_lined;
  bool b_next_no_nl;
  string s_dump;

  static const string s_sep;
  static const int i_max_level;

  virtual void dumpit() = 0;
  void initialize(int i_level);
  void reset();

protected:
  void add
    (unsigned i_num)
  {
    add
      ("<unsigned>");
  }

  void add
    (int i_num)
  {
    add
      ("<int>");
  }

  void add
    (string s_line);
  dumpable();

  void next_no_newline()
  {
    b_next_no_nl = true;
  }

public:
  string dump();
  string dump(int i_level);
  int get_level() const;
};

class dump
{
private:
  string run(vector<string> &vec_params);
public:
  dump(vector<string> vec_params);
};

#endif
