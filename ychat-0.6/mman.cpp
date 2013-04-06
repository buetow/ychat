#ifndef MMAN_CXX
#define MMAN_CXX

#include "mman.h"
#include "mcon.h"
#include "mcon.h"
#include "s_mutx.h"
#include "s_ncur.h"
#include "s_tool.h"

using namespace std;

mman::mman(int i_initial, int i_max)
{
    pthread_mutex_init( &mut_i_used_con  , NULL);

    this->i_used_connections=0;

    if( i_max > MAXMSQL )
    {
#ifdef VERBOSE
    cerr << MYLIMIT << MAXMSQL << endl;
#endif

    i_max = MAXMSQL;

    }

    if( i_initial > i_max )
        i_initial = i_max;

    this->i_initial_connections = i_initial;
    this->i_max_connections = i_max;

#ifdef VERBOSE
    cout << MYINITC << i_initial << endl
         << MYINITM << i_max << endl;
#endif
}
void mman::init( string host, string user, string passwd, string db, unsigned int port)
{
    this->s_host=host;
    this->s_user=user;
    this->s_pass=passwd;
    this->s_db=db;
    this->i_port=port;

    for(int i=0; i<this->i_initial_connections;i++)
        this->mysql.push_back(new_connection( ));

}

#ifdef NCURSES
void
mman::print_init_ncurses()
{
        string s_tmp( MYINITC );
        s_tmp.append( s_tool::int2string( i_initial_connections ) );
        s_ncur::get
            ().print( s_tmp );

        string s_tmp2( MYINITM );
        s_tmp2.append( s_tool::int2string( i_max_connections ) );
        s_ncur::get
            ().print( s_tmp2 );
        print_used_connections(1);
}
void
mman::print_used_connections( bool b_refresh )
{
    pthread_mutex_lock  ( &s_mutx::get().mut_stdout );
    mvprintw( NCUR_MYSQL_X,NCUR_MYSQL_Y, "MySQL: %d ", i_used_connections);
    if ( b_refresh )
     refresh();
    pthread_mutex_unlock( &s_mutx::get().mut_stdout );
}
#endif

mman::~mman()
{

    for(int i = 0; i < this->mysql.size();i++)
    {
        if(this->mysql[i] != NULL)
            mysql_close(this->mysql[i]);
    }

    pthread_mutex_destroy( &mut_i_used_con );
}
MYSQL *mman::get_connection()
{
    if(!this->mysql.empty())
    {
        MYSQL *x=this->mysql[this->mysql.size()-1];
        this->mysql.pop_back();
        if(mysql_ping(x)==0)
        {
            pthread_mutex_lock  ( &mut_i_used_con );
            this->i_used_connections--;
#ifdef NCURSES
            print_used_connections(1);
#endif
            pthread_mutex_unlock( &mut_i_used_con );
            mysql_close(x);
            return new_connection();
        }
        return x;
    }

#ifdef NCURSES
    {
        s_ncur::get
            ().print( MYERROR );
    }
#endif
#ifdef SERVMSG
    cerr << MYERROR << endl;
#endif

    return NULL;
}
MYSQL *mman::new_connection( )
{

    pthread_mutex_lock  ( &mut_i_used_con );
    if(this->i_used_connections>this->i_max_connections)
    {
#ifdef NCURSES
	s_ncur::get().shutdown();
    	cerr << MYERRO2 << endl;
#endif
#ifdef SERVMSG
    	cerr << MYERRO2 << endl;
#endif
        return NULL;
    }
    pthread_mutex_unlock( &mut_i_used_con );

    MYSQL *ms = mysql_init(NULL);
    if(!ms)
    {
#ifdef NCURSES
	s_ncur::get().shutdown();
    	cerr << MYERRO1 << endl;
#endif
#ifdef SERVMSG
    	cerr << MYERRO1 << endl;
#endif
        exit(1);
    }
    if(mysql_real_connect( ms, this->s_host.c_str(), this->s_user.c_str(), this->s_pass.c_str(), this->s_db.c_str()
                           , this->i_port, NULL, 0 )==NULL)
    {
#ifdef NCURSES
	s_ncur::get().shutdown();
        cerr << MYMANAG << mysql_error(ms) << endl;
#endif
#ifdef SERVMSG
        cerr << MYMANAG << mysql_error(ms) << endl;
#endif
        exit(1);
    }

    pthread_mutex_lock  ( &mut_i_used_con );
    this->i_used_connections++;
#ifdef NCURSES
    print_used_connections(0);
#endif
    pthread_mutex_unlock( &mut_i_used_con );
    return ms;
}
void mman::free_connection( MYSQL *msql)
{
    pthread_mutex_lock  ( &mut_i_used_con );
    this->i_used_connections--;
#ifdef NCURSES
    print_used_connections(1);
#endif
    pthread_mutex_unlock( &mut_i_used_con );

    if(mysql_ping( msql )==0)
    {
        mysql_close( msql );
        this->mysql.push_back( msql );
    }
    else
        this->mysql.push_back( new_connection() );
}

#endif
