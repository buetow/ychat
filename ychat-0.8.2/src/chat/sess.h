#include "../incl.h"

#ifndef SESS_H
#define SESS_H

#include <string>
#include "user.h"

class cont;

using namespace std;

class sess
{
private:
  string s_tmpid;
  user  *p_user;

public:
  sess(string s_tmpid);
  ~sess();

  string get_tmpid();

  void set_user(user* p_user);
  user* get_user();
  void set_name(string s_name);
  string get_name();
  void invalidate();
};
#endif
