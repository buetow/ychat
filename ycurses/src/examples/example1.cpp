#include "example1.h"

int example1()
{
  /* Initialize the ycurses object at first! You MUST also delete this as the last!  */
  curses cur;

  /* Make the cursor invisible. 
   * All available options are: 
   * 	Invisible
   *	Visible
   *	VeryVisible
   */
  cursor::set_invisible();

  /* Get the root-window */
  window win_root(root);

  /* 
   * There are standard colors defined which can be mixed together to
   * get foreground/background color pairs:
   *   Red, Green, Yellow, Blue, Cyan, Magenta and White
   */ 
  color col1(Cyan, Blue); 		// 1. With implicit Name.
  // color col1("col1", Cyan, Blue); 	// 2. With explicit Name.

  /* Now we define attributes, and pass them the color */
  attributes attr1(col1);	
  win_root.set_attributes(attr1); 	

  /* Print out some text on the cursor location, with line break */ 
  win_root.println(0, 0, "Hello, welcome to ncurses in cyan-blue");

  win_root.unset_attributes(attr1); 

  /* Create a new centeralized window with the specified height and width */ 
  window win1(20, 78);

  color col2(Yellow, Black); 
  attributes attr2(col2);
  attr1.set_bold(true);

  win1.set_attributes(attr2); 
  win1.print(2, 2, "Print into window at pos 2-2 w/out newline in yellow-black-bold!");

  /* Create yet another window */
  window win2(10, 20);

  /* Print out the y and the x component of the absolute coordinates of win1 */
  coordinate coord_win(Absolutecoord,win1);
  coordinate coord_text(10,11);

  /* Other available coordinates 
   *
   * coordinate(Absolutecoord,win1); // Gets window's absolute position.
   * coordinate(Relativecoord,win1); // Gets window's relative position.
   * coordinate(Beginningcoord,win1); // Gets window's beginning coords.
   * coordinate(windowSize,win1); // Gets window's size.
   * coordinate(TerminalSize); // Gets the total terminal size.
   * coordinate(TerminalCenter); // Gets the center of the terminal 
   */

  attributes attr_bold(Bold);
  // Hint: you can use Normal instead of Bold to set all attributes to default.

  win_root.print(coord_text, "Demonstration output", attr_bold);
  win_root.print(coord_text.down(), "coordinate y:");
  win_root.print(coord_text.displace(1,1), coord_win.y, attr_bold);
  win_root.print(coord_text.displace(1,-1), "coordinate x:");
  win_root.print(coord_text.displace(1,1), coord_win.x, attr_bold);

  /* Wait for user key stroke */
  cur.pause();

  /* Deletes all curses elements from memory, also all pointers 
     Because the ycurses Objekt will get out of scope now.
 */
  return 0;
}

