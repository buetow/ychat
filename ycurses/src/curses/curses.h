#ifndef CURSES_H
#define CURSES_H

#include "attributes.h"
#include "incl.h"
#include "color.h"
#include "cursor.h"
#include "menu.h"
#include "pwindow.h"
#include "tool.h"
#include "window.h"
#include "coordinate.h"

using namespace std;

/* This class is representing the base of curses, it has to be instantiated 
 * in order to use curses.
 */
class curses 
{
 private:
  static int i_self_counter;

 public:
  /* Initializes curses before its first use */
  curses();

  /* Removes all curses elements, all objects (such as window, color etc)
   * be finished as well!
   */
  ~curses();
 
  /* Waits for user intput and returns the ASCII code being typed */ 
  static int get_char();
 
  /* Returns the string being typed until the first return */ 
  static string get_string();
 
  /* Waits intil the user hits a key */
  static void pause();

  static void disable();
  static void enable();

  /* Removes all curses elements, all objects (such as window, color etc) will
   * be finished as well! This method will be called by ~curses internally.
   * You can also use this method instead of the destructor if you wanna use
   * another shutdown routine like catching SIGINT first, deleting your own
   * stuff, and then running curses::finish();!
   */
  static void finish();
  static void clear();
};

#endif
