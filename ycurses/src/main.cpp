#include <iostream>
#include <signal.h>

#include "curses/ycurses.h"
#include "examples/example1.h"
#include "examples/example2.h"
#include "examples/example3.h"
#include "examples/example4.h"

using namespace std;

static void finish_demo(int i_signal);

int main(int argc, char* argv[])
{
  /* Catch SIGINT for proper shutdown */
  signal(SIGINT, finish_demo);

  curses cur;

  color col(Red,Black);
  attributes attr(col); 
  attr.set_bold(true);

  coordinate coord(5,5);

  for (;;)
  {
    cursor::set_invisible();

    window win_root(root);
    win_root.print(1, 2, "Examples of ycurses", attr);
    win_root.print(2, 2, "See src/examples/* for the sample code!");

    vector<string> vec_choices; 
    vec_choices.push_back("1. Example with mixed stuff");
    vec_choices.push_back("2. Class pwindow basic usage");
    vec_choices.push_back("3. Class pwindow usage #2");
    vec_choices.push_back("4. Class menu usage");
    vec_choices.push_back("5. Move menu left");
    vec_choices.push_back("6. Move menu right");
    vec_choices.push_back("7. Move menu up");
    vec_choices.push_back("8. Move menu down");
    vec_choices.push_back("9. Exit");
 
    menu menu(vec_choices);
    window& menu_window = menu.get_window();
    pwindow pan(menu_window);
    pan.move(coord);

    switch ( menu.run() )
    {
      case 0:
	cur.clear();
        pan.hide();
	example1();
	break;

      case 1:
        pan.hide();
	cur.clear();
	example2();
	break;

      case 2:
        pan.hide();
	cur.clear();
	example3();
	break;

      case 3:
        pan.hide();
	cur.clear();
	example4();
	break;

      case 4: 
        pan.move(coord.left());
	break;

      case 5: 
        pan.move(coord.right());
	break;

      case 6: 
        pan.move(coord.up());
	break;

      case 7: 
        pan.move(coord.down());
	break;

      case 8:
  	return 0;
    }

    cur.clear();
    pan.show();
  }

  return 0;
}

static void finish_demo(int i_signal)
{
  curses::finish();

  cout << "Process received SIGINT" << endl;
  exit(0);
}
