#include "example3.h"

int example3()
{
  curses cur;
  cursor::set_invisible();

  window win_root(root);
  win_root.print(1, 1, "Press any key to continue...");

  coordinate coord(2, 0);;
  window win1(10, 30, coord);
  window win2(10, 30, coord.displace(3, 6));

  win1.print(1, 1, "pwindow 1");
  win2.print(1, 1, "pwindow 2");

  pwindow pwindow1(win1);
  pwindow pwindow2(win2);

  cur.pause();

  pwindow1.hide();
  win2.print(2, 1, "pwindow 1 hidden");
  cur.pause();

  pwindow1.show();
  pwindow1.on_bottom(); 
  win2.print(3, 1, "pwindow 1 back");
  cur.pause();

  win1.print(2, 1, "Some pwindow 1 text");
  cur.pause();

  pwindow1.on_top();
  win1.print(3, 1, "pwindow 2 is on bottom");
  cur.pause();

  win1.print(4, 1, "Moved pwindow 2 to 10-10");
  pwindow2.move(10,10);
  cur.pause();

  return 0;
}

