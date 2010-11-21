#ifndef PWINDOW_H
#define PWINDOW_H

using namespace std;

#include "incl.h"
#include "coordinate.h"
#include "hidden.h"
#include "window.h"

class pwindow : public hidden
{

 private:
  PANEL* p_panel;
  window* p_window;
  void init();

 public:
  pwindow(window& r_win);
  ~pwindow();
  void init(window& r_win);
  void reinit(window& r_win);

  void remove();
  void on_bottom();
  void on_top();
  void hide();
  void show();
  void move(int y, int x);
  void move(coordinate& r_coord);

  static void update();
  PANEL* get_PANEL();
};

#endif
