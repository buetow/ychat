#include "../incl.h"

#ifndef USER_H
#define USER_H

#include "../maps/smap.h"
#include "../name.h"
#include "../time/timo.h"

class room;

using namespace std;

class user : public name, public timo
{
private:

    // private members:
    bool   b_online; // true if user is online.
    bool   b_has_sess; // true if user already has a session! 
    bool   b_is_reg; // true if user is registered 
    bool   b_away;   // true if user is away.
    bool   b_fake;   // true if user hides his status logo (does not work for guest)
    bool   b_invisible;   // true if user hides his status logo (does not work for guest)
    int   i_status;   // user's rang ( see enum rang @ globals.h ).
    int   i_old_status;   // user's previous status.
    string s_tmpid;
    string s_agnt;   // user's http user agent.
    string s_away;   // user's last away message.
    string s_col1;   // user's nick color.
    string s_col2;   // user's text color.
    string s_email;  // user's email addres 
    string s_mess;   // message string which has to be sent to the user.
    string s_pass;   // password
    room*  p_room;   // pointer to the user's room.

    map<string,string> map_changed_data; // Needed to tell yChat which data to change after user is removed!
    bool             b_set_changed_data; // Only set change data if required! 

    // Modules which are allowed to be executed by the user.

   // smap<dynmod*,string>* map_mods;

  //  pthread_mutex_t mut_map_mods;
    pthread_mutex_t mut_away;
    pthread_mutex_t mut_b_online;
    pthread_mutex_t mut_b_fake;
    pthread_mutex_t mut_b_invisible;
    pthread_mutex_t mut_b_has_sess;
    pthread_mutex_t mut_b_is_reg;
    pthread_mutex_t mut_s_mess;
    pthread_mutex_t mut_s_pass;
    pthread_mutex_t mut_p_room;
    pthread_mutex_t mut_s_col1;
    pthread_mutex_t mut_s_col2;
    pthread_mutex_t mut_s_email;
    pthread_mutex_t mut_s_tmpid;
    pthread_mutex_t mut_i_status;
    pthread_mutex_t mut_map_changed_data;

    void initialize();
    void   set_changed_data( string s_varname, string s_value );

 public:
   pthread_cond_t  cond_message;
   pthread_mutex_t mut_message;

   user();
   user(string s_name);
   ~user();

    void clean();

    // gets specific data of this user und stores it in
    // (*p_map<string,string>)["nick"]. this method will be used
    // every time data has to be got from every user of a room
    // or even of the system.
    void get_data( map<string,string> *p_map_data );

    string get_colored_name();
    string get_colored_bold_name()
    { 
     return "<b>" + get_colored_name() + "</b>";
    }
    bool  get_online();
    bool  get_fake();
    bool  get_invisible();
    bool  get_has_sess();
    bool  get_is_reg();
    void  set_online( bool b_online );
    void  set_fake( bool b_fake );
    void  set_invisible( bool b_invisible );
    void  set_has_sess( bool b_has_sess );
    void  set_is_reg( bool b_is_reg );
    void  set_changed_data_on() { b_set_changed_data = 1; }
    bool  get_away( );
    string  get_away_msg( );
    void  set_away( bool b_away, string s_away );
    void  set_away( bool b_away );
    room* get_room();
    void  set_p_room( room* p_room );
    string get_pass();
    string get_col1();
    string get_col2();
    string get_email();
    string get_tmpid();
    void   set_tmpid( string s_tmpid );
    void   set_pass( string s_col1 );
    void   set_col1( string s_col1 );
    void   set_col2( string s_col2 );
    void   set_email( string s_email );
    int    get_status( );
    void   set_status( int i_status );
    bool   new_msgs  ( );
    void   check_timeout( int* i_idle_timeout );

    // executes a command.
    void command( string &s_command );

    // gets the message and clears s_mess;
    string get_mess();


    // Here are starting methods which are mainly needed by the data<type> class.
    // appends a string to s_mess including br.
    void msg_post( string s_msg ) { msg_post( &s_msg ); }
    void msg_post( string *p_msg );
    void s_mess_delete( );
    void get_user_list( string &s_list );
    void check_restore_away();
    void reconf();
};

#endif
