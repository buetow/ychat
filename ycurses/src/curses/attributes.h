#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include <set>
#include "incl.h"
#include "color.h"

using namespace std;

const int Normal = A_NORMAL;
const int Standout = A_STANDOUT;
const int Underline = A_UNDERLINE; 
const int Reverse = A_REVERSE; 
const int Blink = A_BLINK;
const int Dim = A_DIM;
const int Bold = A_BOLD;
const int Protect = A_PROTECT;
const int Invis = A_INVIS;
const int AltCharSet = A_ALTCHARSET;
const int CharText = A_CHARTEXT;

class attributes 
{
private:
  void init();  
  set<int> set_attr;
  bool get(int i_attr);
  void set(bool b, int i_attr);
  color* p_color;

  friend class window;
  void use_wattron(WINDOW* p_window);
  void use_wattroff(WINDOW* p_window);

public:
  attributes();
  attributes(int i_attr);
  attributes(color& r_color);

  void set(int i_attr) { set(true, i_attr); }
  void unset(int i_attr ) { set(false, i_attr); }
  void unset_all();
  void set_color(color& r_color);

  void set_normal(bool b) { set(b, Normal); }
  void set_standout(bool b) { set(b, Standout); }
  void set_underline(bool b) { set(b, Underline); }
  void set_reverse(bool b) { set(b, Reverse); }
  void set_blink(bool b) { set(b, Blink); }
  void set_dim(bool b) { set(b, Dim); }
  void set_bold(bool b) { set(b, Bold); }
  void set_protect(bool b) { set(b, Protect); }
  void set_invisible(bool b) { set(b, Invis); }
  void set_altcharset(bool b) { set(b, AltCharSet); }
  void set_chartext(bool b) { set(b, CharText); }

  bool get_normal() { return get(Normal); }
  bool get_standout() { return get(Standout); }
  bool get_underline() { return get(Underline); }
  bool get_reverse() { return get(Reverse); }
  bool get_blink() { return get(Blink); }
  bool get_dim() { return get(Dim); }
  bool get_bold() { return get(Bold); }
  bool get_protect() { return get(Protect); }
  bool get_invisible() { return get(Invis); }
  bool get_altcharset() { return get(AltCharSet); }
  bool get_chartext() { return get(CharText); }
};

#endif
