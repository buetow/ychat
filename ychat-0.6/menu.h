#ifndef MENU_H
#define MENU_H

#include <ncurses.h>
#include "incl.h"

using namespace std;

class menu
{
private:
    char **choices;
    char *c_header;

    int i_startx, i_starty, i_width, i_height, i_highlight, i_choice,
    i_numchoices, c;

    WINDOW *win;

    void initialize( const chtype ch );

public:
    explicit menu( int i_startx, int i_starty, int i_width, int i_height, char *c_header, char **choices, int i_numchoices, const chtype ch );
    ~menu( );

    void display();
    void start( void (*swich_case_menu_action)(int) );
};

#endif
