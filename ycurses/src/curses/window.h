#ifndef WINDOW_H
#define WINDOW_H

#include "attributes.h"
#include "color.h"
#include "coordinate.h"
#include "dummy.h"
#include "hidden.h"
#include "incl.h"
#include "tool.h"

using namespace std;

enum window_type
{
  root
};

class window : public dummy_window, hidden
{
 private:
  WINDOW* p_window;
  int i_height, i_width, i_start_y, i_start_x;
  static bool b_is_rootoot;

  /* For save_attributes and restore_attributes */
  attr_t sr_attributes; short sr_pair; void *sr_options; 

  WINDOW *create_new_win(int i_height, int i_width, int i_start_y, int i_start_x);
  void destroy_win(WINDOW *p_local_win); 

  /* Called by the constructors */
  void init();

 public:
  window(window_type t); 
  window(int i_height, int i_width, coordinate& r_coord); 
  window(int i_height, int i_width, int i_start_y, int i_start_x); 
  window(int i_height, int i_width);
  ~window();

  void hide();
  void show();
  void refresh();
  void set_attributes(attributes& r_attr);
  void unset_attributes(attributes& r_attr);
  bool is_root_win();

  WINDOW* get_WINDOW() { return p_window; }

  int save_attributes();
  int restore_attributes();

  void move_cursor(coordinate& r_coord);
  void move_cursor(int i_y, int i_x);

  void print(string s);
  void print(int i_y, int i_x, string s);
  void println(string s);
  void println(int i_y, int i_x, string s);
  void print(int i);
  void print(coordinate &r_coord, string s);
  void print(int i_y, int i_x, int i);
  void print(coordinate &r_coord, int i);
  void println(int i);
  void println(coordinate &r_coord, string s);
  void println(int i_y, int i_x, int i);
  void println(coordinate &r_coord, int i);

  void print(string s, attributes& r_attr);
  void print(int i_y, int i_x, string s, attributes& r_attr);
  void println(string s, attributes& r_attr);
  void println(int i_y, int i_x, string s, attributes& r_attr);
  void print(int i, attributes& r_attr);
  void print(coordinate &r_coord, string s, attributes& r_attr);
  void print(int i_y, int i_x, int i, attributes& r_attr);
  void print(coordinate &r_coord, int i, attributes& r_attr);
  void println(int i, attributes& r_attr);
  void println(coordinate &r_coord, string s, attributes& r_attr);
  void println(int i_y, int i_x, int i, attributes& r_attr);
  void println(coordinate &r_coord, int i, attributes& r_attr);
};

#endif

