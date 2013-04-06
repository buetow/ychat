#ifndef CLI_CPP
#define CLI_CPP

#include "cli.h"

#ifdef CLI
using namespace std;

cli::cli( )
{
#ifdef NCURSES
  start();
#endif
}

cli::~cli()
{}

int
cli::parse_input( string s_input )
{
  string s_param = "";
  unsigned i_pos = s_input.find_first_of(" ");

  if ( i_pos != string::npos )
  {
    s_param = s_input.substr(i_pos+1);
    s_input = s_input.substr(0, i_pos);
  }

  if ( s_input.compare("help") == 0 || s_input.compare("h") == 0)
  {
    cout << CLIPRMO << "COMMAND LINE INTERFACE HELP MENU" << endl
         << CLIPRMO << " !command      - Uses system to run a command" << endl;
#ifdef DEBUG

    cout << CLIPRMO << " (d)ebug       - Starts debug routine (cli.cpp)" << endl;
#endif

    cout << CLIPRMO << " (du)mp [part] - Prints out a dump of the data structure" << endl;
    cout << CLIPRMO << "		    (Run without part to list all possibilities)" << endl;
    cout << CLIPRMO << " (e)cho VAR    - Prints out configuration value of VAR" << endl;
    cout << CLIPRMO << "                 Wildcards can be used too, example: echo http*" << endl;
#ifdef NCURSES

    cout << CLIPRMO << " (ex)it        - Quits CLI mode and respawns ncurses mode" << endl;
#endif

    cout << CLIPRMO << " (h)elp        - Prints out this help!" << endl;
#ifdef EXPERIM

    cout << CLIPRMO << " (re)conf      - Reloads configuration (EXPERIMENTAL)" << endl;
#endif

    cout << CLIPRMO << " (r)usage      - Shows current resource usage" << endl
    << CLIPRMO << " (ru)sageh     - Shows resource usage history (yhttpd needs to run > 1 day)" << endl
    << CLIPRMO << " (set) VAR VAL - Sets configuration value VAR to VAL" << endl
    << CLIPRMO << " (sh)ell       - Runs a system shell" << endl
    << CLIPRMO << " (s)hutdown    - Shuts down the whole server" << endl
    << CLIPRMO << " (t)ime        - Prints out time and uptime" << endl;
    cout << CLIPRMO << " (u)nset VAR   - Deletes configuration value VAR" << endl
    << CLIPRMO << " (v)ersion     - Prints out version" << endl;
    cout << CLIPRMI;
  }
  else if( s_input.at(0) == '!' )
  {
    system( s_input.substr(1).c_str() );
    cout << CLIPRMI;
  }

#ifdef DEBUG
  else if ( s_input.compare("d") == 0 || s_input.compare("debug") == 0 )
  {
    debug_routine();
    cout << CLIPRMI;
  }
#endif
  else if ( s_input.compare("du") == 0 || s_input.compare("dump") == 0 )
  {
    dump d(vectorize(s_param));
    cout << CLIPRMI;
  }
  else if( s_input.compare("echo") == 0 || s_input.compare("e") == 0 )
  {
    string s_val;

    // Check wildcards
    unsigned i_pos = s_param.find("*");
    if ( i_pos != string::npos )
    {
      s_param = s_param.substr( 0, i_pos );
      vector<string>* p_vec = wrap::CONF->get_key_vector();
      sort(p_vec->begin(), p_vec->end());
      vector<string>::iterator iter;

      for ( iter = p_vec->begin(); iter != p_vec->end(); iter++ )
        if ( iter->find(s_param) == 0 )
          s_val.append( *iter + " := " +  wrap::CONF->get_elem(*iter) + "\n" + CLIPRMO );
      delete p_vec;
    }
    else
    {
      s_val = wrap::CONF->get_elem(s_param);
    }

    if( s_val.empty() )
      s_val = "Value not set";

    cout << CLIPRMO << s_val << endl;
    cout << CLIPRMI;
  }


#ifdef NCURSES
  else if( s_input.compare("exit") == 0 || s_input.compare("ex") == 0 )
  {
    return 0;
  }
#endif


#ifdef EXPERIM
  else if( s_input.compare("reconf") == 0 || s_input.compare("re") == 0 )
  {
    wrap::HTTPD->reconf();
    cout << CLIPRMI;
  }
#endif

  else if( s_input.compare("rusage") == 0 || s_input.compare("r") == 0 )
  {
    print_rusage();
    cout << CLIPRMI;
  }
  else if( s_input.compare("ru") == 0 || s_input.compare("rusageh") == 0 )
  {
    cout << wrap::STAT->get_rusage_history( "ru_maxrss", string(CLIPRMO) + " " );
    cout << CLIPRMI;
  }
  else if( s_input.compare("set") == 0 )
  {
    string s_varname = "";
    i_pos = s_param.find_first_of(" ");

    if ( i_pos != string::npos )
    {
      s_varname = s_param.substr(0, i_pos);

      if ( s_param.length() > i_pos+1 )
        s_param = s_param.substr(i_pos+1);

      else
        s_param = "";
    }

    string s_old_val = wrap::CONF->get_elem(s_varname);

    if ( !s_old_val.empty() )
    {
      cout << CLIPRMO << "Old value: " << s_old_val << endl;
      wrap::CONF->del_elem(s_varname);
    }

    wrap::CONF->add_elem(s_param, s_varname);
    cout << CLIPRMO << "New value: " << s_param << endl;
    cout << CLIPRMI;
  }
  else if( s_input.compare("shell") == 0 || s_input.compare("sh") == 0 )
  {
    cout << CLIPRMO << CLISHEL << endl;
    system(wrap::CONF->get_elem("httpd.system.shell").c_str());
    cout << CLIPRMO << CLIWELC << endl;
    cout << CLIPRMI;
  }
  else if( s_input.compare("shutdown") == 0 || s_input.compare("s") == 0 )
  {
    exit(0);
  }
  else if( s_input.compare("t") == 0 || s_input.compare("time") == 0 )
  {
    cout << CLIPRMO  << "Time:   " << wrap::TIMR->get_time() << endl
    << CLIPRMO  << "Uptime: " << wrap::TIMR->get_uptime() << endl;
    cout << CLIPRMI;
  }


  else if( s_input.compare("u") == 0 || s_input.compare("unset") == 0 )
  {
    if (!s_param.empty())
    {
      string s_old_val = wrap::CONF->get_elem(s_param);
      if ( !s_old_val.empty() )
      {
        cout << CLIPRMO << "Deleted variable " << s_param << " with value: " << s_old_val << endl;
        wrap::CONF->del_elem(s_param);
      }
      else
      {
        cout << CLIPRMO << "Variable " << s_param << " was not set" << endl;
      }
      cout << CLIPRMI;
    }
  }
  else if( s_input.compare("v") == 0 || s_input.compare("version") == 0 )
  {
    cout << CLIPRMO  << tool::yhttpd_version() << " " << UNAME << endl;
    cout << CLIPRMI;
  }
  else
  {
    cout << CLIPRMO << CLIHELP << "\"" << s_input << "\"" << endl;
    cout << CLIPRMI;
  }

  return 1;
}

void
#ifndef NCURSES
cli::start(void* p_void)
#else
cli::start()
#endif
{
  cout << CLIPRMO << CLIWELC << endl;

  string s_input;
  cout << CLIPRMI;

  do
  {
    getline(cin, s_input);
  }
  while( parse_input(s_input) );
}

void
cli::print_rusage()
{
  rusage* p_rusage = new rusage;
  getrusage( RUSAGE_SELF, p_rusage );

  cout << CLIPRMO << "ru_maxrss:   " << p_rusage->ru_maxrss   << "\t(max resident set size)" << endl
  << CLIPRMO << "ru_ixrss:    " << p_rusage->ru_ixrss    << "\t(integral shared text memory size)" << endl
  << CLIPRMO << "ru_idrss:    " << p_rusage->ru_idrss    << "\t(integral unshared data size)" << endl
  << CLIPRMO << "ru_isrss:    " << p_rusage->ru_isrss    << "\t(integral unshared stack size)" << endl
  << CLIPRMO << "ru_minflt:   " << p_rusage->ru_minflt   << "\t(page reclaims)" << endl
  << CLIPRMO << "ru_majflt:   " << p_rusage->ru_majflt   << "\t(page faults)" << endl
  << CLIPRMO << "ru_nswap:    " << p_rusage->ru_nswap    << "\t(swaps)" << endl
  << CLIPRMO << "ru_inblock:  " << p_rusage->ru_inblock  << "\t(block input operations)" << endl
  << CLIPRMO << "ru_oublock:  " << p_rusage->ru_oublock  << "\t(block output operations)" << endl
  << CLIPRMO << "ru_msgsnd:   " << p_rusage->ru_msgsnd   << "\t(messages sent)" << endl
  << CLIPRMO << "ru_msgrcv:   " << p_rusage->ru_msgrcv   << "\t(messages received)" << endl
  << CLIPRMO << "ru_nsignals: " << p_rusage->ru_nsignals << "\t(signals received)" << endl
  << CLIPRMO << "ru_nvcsw:    " << p_rusage->ru_nvcsw    << "\t(voluntary context switches)" << endl
  << CLIPRMO << "ru_nivcsw:   " << p_rusage->ru_nivcsw   << "\t(involuntary context switches)" << endl;

  delete p_rusage;
}

vector<string>
cli::vectorize(string s_param) 
{
  vector<string> vec_ret;
  unsigned i_pos;

  for (i_pos = s_param.find(" ");
       i_pos != string::npos;
       i_pos = s_param.find(" "))
  {
    vec_ret.push_back(s_param.substr(0, i_pos));
    s_param = s_param.substr(i_pos+1);
  }

  if (!s_param.empty())
    vec_ret.push_back(s_param);

  return vec_ret;
}

#ifdef DEBUG
void
cli::debug_routine()
{
  rusage* p_rusage = new rusage;
  for(;;)
  {
    /*
      ossl *p_tmp = new ossl;
      getrusage( RUSAGE_SELF, p_rusage );      
       cout << CLIPRMO << "ru_maxrss:   " << p_rusage->ru_maxrss   << "\t(max resident set size)" << endl;
      delete p_tmp;	
     */
  }
  delete p_rusage;
}
#endif

#endif
#endif
