#include "example2.h"

int example2()
{
  curses cur;
  cursor::set_invisible();

  window win_root(root);
  win_root.print(1, 1, "Press any key to continue...");

  coordinate coord(2, 0);;

  window win1(10, 40, coord); 
  window win2(10, 40, coord.displace(3, 6));
  window win3(10, 40, coord.displace(3, 6));

  win1.print(1, 1, "window 1");
  win2.print(1, 1, "window 2");
  win3.print(1, 1, "window 3");

  /* Initialize pwindows. */
  pwindow pwindow1(win1);
  pwindow pwindow2(win2);
  pwindow pwindow3(win3);


  /* Remove pwindow3, pwindow3 doesnt contain a valid pwindow any more! 
   * DONT USE THIS OBJECT AGAIN until re-init!!! 
   */
  pwindow3.remove();

  /* Re-init a new pwindow, using the old variable. Now the object
   * can be used again. 
   */
  pwindow3.reinit(win3);
  cur.pause();

  /* Remove the center pwindow. */
  pwindow2.remove();
  win3.print(2, 1, "pwindow 2 removed!");
  cur.pause();

  win1.print(2, 1, "Some text!");
  cur.pause();

  pwindow3.remove();
  win1.print(3, 1, "pwindow 3 removed!");
  cur.pause();

  return 0;
}

