#ifndef DUMP_CPP
#define DUMP_CPP

#include "dump.h"

using namespace std;

const string dumpable::s_sep = "->";
const int dumpable::i_max_level = 100;

dumpable::dumpable()
{
  initialize(0);
}

void
dumpable::initialize(int i_level)
{
  this->i_level = i_level;
  this->i_lined = i_level;
  this->b_lined = false;
  this->b_next_no_nl = false;
  this->s_dump = "";
}

string
dumpable::dump()
{
  return dump(0);
}

string
dumpable::dump(int i_level)
{
  initialize(i_level);
  dumpit();
  return s_dump;
}

void
dumpable::add
  (string s_line)
{
  if ( i_lined > i_max_level )
    i_lined = i_max_level;

  if (!b_lined)
    s_dump.append(s_sep);

  else
    for ( int i = 0; i < i_lined; ++i )
      s_dump.append(" ");

  s_dump.append(s_line);

  if (b_next_no_nl)
    b_next_no_nl = false;

  else
    s_dump.append("\n");

  if (!b_lined)
  {
    b_lined = true;
    i_lined = i_level + s_sep.length();
  }
}

int
dumpable::get_level() const
{
  return i_lined;
}

dump::dump(vector<string> vec_params)
{
  if (vec_params.empty())
  {
    cout << CLIPRMO << "all conf sock";
    cout << endl;
    return;
  }

  cout << run(vec_params);
}

string
dump::run(vector<string> &vec_params)
{
  string s_ret("");
  string s_part;

  vector<string>::iterator iter;
  for (iter = vec_params.begin(); iter != vec_params.end(); ++iter)
  {
    s_part = *iter;

    if (!s_part.compare("conf") || !s_part.compare("all"))
      s_ret.append(wrap::CONF->dump());

    if (!s_part.compare("sock") || !s_part.compare("all"))
      s_ret.append(wrap::SOCK->dump());

  }

  return s_ret;
}

#endif
