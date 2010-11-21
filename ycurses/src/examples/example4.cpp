#include "example4.h"

int example4()
{
  curses cur;
  cursor::set_invisible();

  window win_root(root);
  win_root.print(1, 1, "Class menu example...");

  vector<string> vec_choices;
  vec_choices.push_back("Hallo das ist asd asdas d asd asda sd asd asd a");
  vec_choices.push_back("Bdsfsdsa das d asd asd asd as das djf");
  vec_choices.push_back("s das d asd saa sdCsdfsdf");
  menu menu1(vec_choices);
 
  menu1.run(); 

  return 0;
}


