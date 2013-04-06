#ifndef LOGD_CXX
#define LOGD_CXX

#include "logd.h"

logd::logd( string filename )
{
	if(filename.empty())
	{
		cerr << "ycLog: No filename specified" << endl;
		exit(1);
	}
  
	s_logfile=filename;

	i_lines=s_tool::string2int( s_conf::get().get_val("LOG_LINES"));	

}
void logd::flush()
{
	 s_output.open(s_logfile.c_str(), ios::app);

        if(s_output==NULL)
        {
                cerr << "ycLog: Could not open file: " << s_logfile << endl;
                exit(1);
        }


	while(!s_queue.empty())
	{
		string s_l=s_queue.front();
		s_queue.pop();
		s_output.write(s_l.c_str(), s_l.size());

	}
	s_output.close();
}
void logd::log( map_string request )
{
	struct tm *t_m;
	time_t t_cur=time(NULL);
	t_m=gmtime(&t_cur);

	char buffer[100];
	strftime(buffer, 100, "[%d/%b/%Y:%H:%M:%S %z]", t_m);
	string s_time=buffer;
	string s_logstr = request["REMOTE_ADDR"] + " - - "+s_time+" \"" + request["QUERY_STRING"]+"\" 200 0 \""+request["request"]+"\" \""+request["User-Agent"]+"\"\n";

	s_queue.push(s_logstr);	

	if(s_queue.size()>=i_lines)
		flush();
}

logd::~logd()
{
	flush();
}





#endif
