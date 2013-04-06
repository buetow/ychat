#ifndef NCUR_CXX
#define NCUR_CXX

#include "ncur.h"
#include "s_mman.h"
#include "s_sman.h"
#include "s_sock.h"

using namespace std;

ncur::ncur( )
{
    p_messagelist = new list<char*>;
    pthread_mutex_init( &mut_messages, NULL );
    i_message_length = 41;
    b_is_ready = false;
}

ncur::~ncur()
{
    pthread_mutex_destroy( &mut_messages );
}

void*
ncur::start( void *v_pointer )
{
    pthread_mutex_lock  ( &s_mutx::get().mut_stdout );
    ncur* admin_interface = static_cast<ncur*>(v_pointer);

    initscr();
    start_color();
    clear();
    noecho();
    cbreak();       // Line buffering disabled. pass on everything

    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    mvprintw(0, 2, DESCRIP );
    mvprintw(1, 2, CONTACT );

    refresh();

    char *choices[] = {
                          "Chat manager            NI",
                          "Config manager          NI",
                          "HTML-template manager   NI",
                          "Language manager        NI",
                          "Module-loader manager   NI",
                          "MySQL connection manag. NI",
                          "Session manager         NI",
                          "Socket manager          NI",
                          "Shut down server",
                      };

    admin_interface->p_serveroutput = newwin( 17, 45, 3, 34 );
    wbkgd(admin_interface->p_serveroutput, COLOR_PAIR(1));

    box      ( admin_interface->p_serveroutput, 0, 0 );
    mvwprintw( admin_interface->p_serveroutput, 2, 2, "SERVER SYSTEM MESSAGES" );
    wrefresh ( admin_interface->p_serveroutput );


    admin_interface->print( VERSION );
    pthread_mutex_unlock( &s_mutx::get().mut_stdout );

#ifdef NCURSES
    s_mman::get().print_init_ncurses(); 
    s_sman::get().print_init_ncurses(); 
#endif

    admin_interface->is_ready( true );
    admin_interface->p_menu = new menu( 1, 3, 32, 17, "ADMINISTRATOR's MAIN MENU", choices, 9, COLOR_PAIR(1));
    admin_interface->p_menu->start( &switch_main_menu_ );

    admin_interface->shutdown();	
}

void
ncur::shutdown()
{
    endwin();
}


void
ncur::print( string *p_msg )
{
    print( (char*)p_msg->c_str() );
}

void
ncur::print( string s_msg )
{
    print( (char*)s_msg.c_str() );
}

void
ncur::print( char* c_print )
{
    if ( strlen( c_print ) > i_message_length )
    {
	string s_tmp( c_print );
	print( s_tmp.substr( 0, i_message_length ) );
        print( s_tmp.substr( i_message_length, s_tmp.length() ) ); 

	return;
    }	

    char* c_temp = new char[i_message_length];
    int i;
    for ( i = 0; i < i_message_length; i++ )
        c_temp[i] = ' ';
    c_temp[i] = '\0';

    memcpy( c_temp, c_print, strlen(c_print) );

    pthread_mutex_lock( &mut_messages );

    if ( p_messagelist->size() > 10 )
        p_messagelist->pop_front();

    p_messagelist->push_back( c_temp );

    list<char*>::iterator iter;
    iter = p_messagelist->begin();

    for ( i=4; i<16 && iter != p_messagelist->end(); i++, iter++ )
        mvwprintw( p_serveroutput, i, 2, *iter );

    wrefresh ( p_serveroutput );

    pthread_mutex_unlock( &mut_messages );
}

void
ncur::switch_main_menu_( int i_choice )
{
    if( i_choice != 0 )
        switch ( i_choice )
        {
        case 9: // Shut down server
	    s_mman::get().~mman();
            mvprintw( 21,2, "Good bye !");
            refresh();
            clrtoeol();
            refresh();
            endwin();
            exit(0);
            break;

        default:
            pthread_mutex_lock  ( &s_mutx::get().mut_stdout );
            mvprintw( 20,2, "Selection # %d not yet implemented!", i_choice-1);
            refresh();
            pthread_mutex_lock  ( &s_mutx::get().mut_stdout );
            break;
        }
}
#endif
