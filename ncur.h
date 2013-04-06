#ifndef NCUR_H
#define NCUR_H

#include <ncurses.h>
#include <list>
#include "s_mutx.h"
#include "incl.h"
#include "menu.h"

using namespace std;

class ncur
{
private:
    menu*   p_menu;
    WINDOW* p_serveroutput;
    list<char*>* p_messagelist;   // contains the messages for p_serveroutput!
    pthread_mutex_t mut_messages; // for syncronization of p_serveroutput!
    int  i_message_length;        // the maximum length of a system message!
    bool b_is_ready;	       // is set to TRUE if the admin interface is initialized.

public:
    explicit ncur( );      	      // a standard constructor.
    ~ncur( );

    static void* start( void *v_pointer );
    void print( char* c_print );
    void print( string  s_msg );
    void print( string* p_msg );
    void is_ready( bool b_is_ready )
    {
        this->b_is_ready = b_is_ready;
    }
    bool is_ready() const
    {
        return b_is_ready;
    }

    static void switch_main_menu_( int i_choice );
    void shutdown();
};

#endif
