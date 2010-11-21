#ifndef PANEL_CPP
#define PANEL_CPP

#include "pwindow.h"

pwindow::pwindow(window& r_win)
{
  p_window = &r_win;
  init();
}

void
pwindow::reinit(window& r_win)
{
  p_window = &r_win;
  init();
}

pwindow::~pwindow()
{
  remove();
}

void
pwindow::remove()
{
  if (p_panel)
  {
    del_panel(p_panel);
    p_panel = 0;
  }

  update();
}

void
pwindow::init()
{
  p_panel = new_panel(p_window->get_WINDOW());
  update();
}

void
pwindow::update()
{
  update_panels();
  doupdate();
}

void
pwindow::on_bottom()
{
  bottom_panel(p_panel);
  update();
}

void
pwindow::on_top()
{
  top_panel(p_panel);
  update();
}

void
pwindow::hide()
{
  hidden::hide();
  hide_panel(p_panel);
  update();
}

void
pwindow::show()
{
  hidden::show();
  show_panel(p_panel);
  update();
}

void
pwindow::move(coordinate& r_coord)
{
  move(r_coord.y, r_coord.x);
}

void
pwindow::move(int y, int x)
{
  move_panel(p_panel, y, x);
  update();
}

PANEL*
pwindow::get_PANEL()
{
  return p_panel;
}

#endif
