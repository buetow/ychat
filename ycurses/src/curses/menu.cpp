#ifndef MENU_CPP
#define MENU_CPP

#include "menu.h"

menu::menu(vector<string>& vec_choices) 
{
  init(vec_choices);
}

menu::~menu()
{
  if (p_items)
  {
    for (int i = 0; i < i_choices; ++i)
      free_item(p_items[i]);

    delete [] p_items;
    p_items = 0; 
  }  

  if (p_menu)
  {
    free_menu(p_menu);
    p_menu = 0;
  }

  if (p_win)
  {
    delete p_win; 
    p_win = 0;
  }
}

void
menu::init(vector<string>& vec_choices)
{
  i_choices = vec_choices.size();
  i_current = 0;

  int i_choices = vec_choices.size();
  int iMaxLen = 0;
  p_items = new ITEM*[i_choices];

  for (int i = 0; i < i_choices; ++i)
  {
    p_items[i] = new_item(vec_choices.at(i).c_str(), "");
    int iLen = vec_choices.at(i).length();
    if (iLen > iMaxLen)
      iMaxLen = iLen;
  }

  p_menu = new_menu(p_items);

  p_win = new window(i_choices+2, iMaxLen+4, 4, 4);
  WINDOW* p_window = p_win->get_WINDOW();

  set_menu_win(p_menu, p_window);
  set_menu_sub(p_menu, derwin(p_window, i_choices, iMaxLen+2, 1, 1));

  refresh();
  post_menu(p_menu);
  wrefresh(p_window);
}

window&
menu::get_window()
{
  return *p_win; 
}

int
menu::run()
{
  WINDOW* p_window = p_win->get_WINDOW();
  keypad(p_window, 1);
  int i_choice;

  while( (i_choice = wgetch(p_window)) )
  { 
    switch(i_choice)
    {	
      case KEY_DOWN:
      case 'j': // Vi mode
        if ( i_current < i_choices-1 )
 	  ++i_current;
	menu_driver(p_menu, REQ_DOWN_ITEM);
	break;

      case KEY_UP:
      case 'k': // Vi mode
	if ( i_current > 0 )
	  --i_current;
	menu_driver(p_menu, REQ_UP_ITEM);
	break;

      case 'h': // Vi mode
      case 'q': // Vi mode
      case 27: // ESC
	i_current = i_choices-1; // Select last entry.

      case 'l': // Vi mode
      case 10:  // Enter
      case 32:  // Space
        keypad(p_window, 0);
	return i_current;
    }

//    window win_root(root);
//    win_root.print(i_choice);   
 
    wrefresh(p_window);
  }	
}

#endif

