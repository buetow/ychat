#ifndef MENU_H
#define MENU_H

#include "incl.h"
#include "window.h"

using namespace std;

class menu 
{
 private:
  void init(vector<string>& vec_choices);  

  int i_choices, i_current;
  ITEM** p_items;
  MENU* p_menu;
  window* p_win;   

 public:
  menu(vector<string>& vec_choices);
  menu(string s_name,  vector<string>& vec_choices);
  ~menu();

  window& get_window();
  int run();
};

#endif
