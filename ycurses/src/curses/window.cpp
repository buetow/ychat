#ifndef WINDOW_CPP
#define WINDOW_CPP

#include "window.h"

bool window::b_is_rootoot = false;

window::window(window_type t)
{
  b_is_rootoot = true;
  init();
  p_window = stdscr;
}

window::window(int i_height, int i_width) 
{
  init();

  this->i_height = i_height;
  this->i_width = i_width;
  this->i_start_y = (LINES - i_height) / 2; 
  this->i_start_x = (COLS - i_width) / 2; 

  p_window = create_new_win(i_height, i_width, i_start_y, i_start_x);
}

window::window(int i_height, int i_width, coordinate& r_coord) 
{
  init();

  this->i_height = i_height;
  this->i_width = i_width;
  this->i_start_y = r_coord.y; 
  this->i_start_x = r_coord.x;

  p_window = create_new_win(i_height, i_width, i_start_y, i_start_x);
}

window::window(int i_height, int i_width, int i_start_y, int i_start_x) 
{
  init();

  this->i_height = i_height;
  this->i_width = i_width;
  this->i_start_y = i_start_y;
  this->i_start_x = i_start_x;

  p_window = create_new_win(i_height, i_width, i_start_y, i_start_x);
}

window::~window()
{
  if (p_window)
  {
    if(!is_root_win())
      destroy_win(p_window);

    p_window = 0;
  }
}

bool
window::is_root_win()
{
  return b_is_rootoot; 
}

void
window::init()
{
}

WINDOW*
window::create_new_win(int i_height, int i_width, int i_start_y, int i_start_x)
{
  WINDOW *p_local_win = newwin(i_height, i_width, i_start_y, i_start_x);
  box(p_local_win, 0 , 0);		/* 0, 0 gives default characters 
      				 * for the vertical and horizontal
      				 * lines			*/
  wrefresh(p_local_win);		/* Show that box 		*/

  return p_local_win;
}

void
window::destroy_win(WINDOW *p_local_win)
{	
  /* box(local_win, ' ', ' '); : This won't produce the desired
   * result of erasing the window. It will leave it's four corners 
   * and so an ugly remnant of window. 
   */

  wborder(p_local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  /* The parameters taken are 
   * 1. win: the window on which to operate
   * 2. ls: character to be used for the left side of the window 
   * 3. rs: character to be used for the right side of the window 
   * 4. ts: character to be used for the top side of the window 
   * 5. bs: character to be used for the bottom side of the window 
   * 6. tl: character to be used for the top left corner of the window 
   * 7. tr: character to be used for the top right corner of the window 
   * 8. bl: character to be used for the bottom left corner of the window 
   * 9. br: character to be used for the bottom right corner of the window
   */

  wrefresh(p_local_win);
  delwin(p_local_win);
}

void
window::hide()
{
  hidden::hide();

  if (p_window)
   destroy_win(p_window);
}

void
window::show()
{
  if (!is_hidden())
    return;

  hidden::show();

  p_window = create_new_win(i_height, i_width, i_start_y, i_start_x);

  refresh();
}

void
window::refresh()
{
  wrefresh(p_window);
}

void
window::set_attributes(attributes& r_attr)
{
  r_attr.use_wattron(p_window);
}

void
window::unset_attributes(attributes& r_attr)
{
  r_attr.use_wattroff(p_window);
}

int
window::save_attributes()
{
  return attr_get(&sr_attributes, &sr_pair, sr_options);
}

int
window::restore_attributes()
{
  return attr_set(sr_attributes, sr_pair, sr_options);
}

void
window::print(int i)
{
  print(tool::int2string(i));
}

void
window::print(coordinate &r_coord, string s)
{
  print(r_coord.y, r_coord.x, s);
}

void
window::print(int i_y, int i_x, int i)
{
  print(i_y, i_x, tool::int2string(i));
}

void
window::print(coordinate &r_coord, int i)
{
  print(r_coord, tool::int2string(i));
}

void
window::println(int i)
{
  println(tool::int2string(i));
}

void
window::println(coordinate &r_coord, string s)
{
  println(r_coord.y, r_coord.x, s);
}

void
window::println(int i_y, int i_x, int i)
{
  println(i_y, i_x, tool::int2string(i));
}

void
window::println(coordinate &r_coord, int i)
{
  println(r_coord, tool::int2string(i));
}

void
window::print(string s)
{
  wprintw(p_window, const_cast<char*>(s.c_str()));
  refresh();
}

void
window::print(int i_y, int i_x, string s)
{
  mvwprintw(p_window, i_y, i_x, const_cast<char*>(s.c_str()));
  refresh();
}

void
window::println(string s)
{
  wprintw(p_window, const_cast<char*>((s+"\n").c_str()));
  refresh();
}

void
window::println(int i_y, int i_x, string s)
{
  mvwprintw(p_window, i_y, i_x, const_cast<char*>((s+"\n").c_str()));
  refresh();
}

void
window::print(int i, attributes& r_attr)
{
  save_attributes();
  set_attributes(r_attr);
  print(tool::int2string(i));
  restore_attributes();
}

void
window::print(coordinate &r_coord, string s, attributes& r_attr)
{
  save_attributes();
  set_attributes(r_attr);
  print(r_coord.y, r_coord.x, s);
  restore_attributes();
}

void
window::print(int i_y, int i_x, int i, attributes& r_attr)
{
  save_attributes();
  set_attributes(r_attr);
  print(i_y, i_x, tool::int2string(i));
  restore_attributes();
}

void
window::print(coordinate &r_coord, int i, attributes& r_attr)
{
  save_attributes();
  set_attributes(r_attr);
  print(r_coord, tool::int2string(i));
  restore_attributes();
}

void
window::println(int i, attributes& r_attr)
{
  save_attributes();
  set_attributes(r_attr);
  println(tool::int2string(i));
  restore_attributes();
}

void
window::println(coordinate &r_coord, string s, attributes& r_attr)
{
  save_attributes();
  set_attributes(r_attr);
  println(r_coord.y, r_coord.x, s);
  restore_attributes();
}

void
window::println(int i_y, int i_x, int i, attributes& r_attr)
{
  save_attributes();
  set_attributes(r_attr);
  println(i_y, i_x, tool::int2string(i));
  restore_attributes();
}

void
window::println(coordinate &r_coord, int i, attributes& r_attr)
{
  save_attributes();
  set_attributes(r_attr);
  println(r_coord, tool::int2string(i));
  restore_attributes();
}

void
window::print(string s, attributes& r_attr)
{
  save_attributes();
  set_attributes(r_attr);
  wprintw(p_window, const_cast<char*>(s.c_str()));
  refresh();
  restore_attributes();
}

void
window::print(int i_y, int i_x, string s, attributes& r_attr)
{
  save_attributes();
  set_attributes(r_attr);
  mvwprintw(p_window, i_y, i_x, const_cast<char*>(s.c_str()));
  refresh();
  restore_attributes();
}

void
window::println(string s, attributes& r_attr)
{
  save_attributes();
  set_attributes(r_attr);
  wprintw(p_window, const_cast<char*>((s+"\n").c_str()));
  refresh();
  restore_attributes();
}

void
window::println(int i_y, int i_x, string s, attributes& r_attr)
{
  save_attributes();
  set_attributes(r_attr);
  mvwprintw(p_window, i_y, i_x, const_cast<char*>((s+"\n").c_str()));
  refresh();
  restore_attributes();
}

void
window::move_cursor(coordinate& r_coord)
{
  move_cursor(r_coord.y, r_coord.x);
}

void
window::move_cursor(int i_y, int i_x)
{
  if (!p_window)
    return;

  wmove(p_window, i_y, i_x);  
  refresh();
}


#endif
