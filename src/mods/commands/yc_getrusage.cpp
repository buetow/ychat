#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>
#include "../../chat/user.h"

#ifndef	  RUSAGE_SELF
#define   RUSAGE_SELF 0
#endif
#ifndef	  RUSAGE_CHILDREN
#define   RUSAGE_CHILDREN -1
#endif

/*
 gcc -shared -o yc_name.so yc_name.cpp
*/

using namespace std;

extern "C" {
 int valid_color( string );
 int extern_function(void *v_arg)
 {
	container *c=(container *)v_arg;
	
	user *p_user = (user*) c->elem[1];		// the corresponding user
        rusage* p_rusage = new rusage;
        getrusage( RUSAGE_SELF, p_rusage );       
        char c_msg[1024];

        /* see man getrusage  */

        sprintf(c_msg, "getrusage:<br>\nmaxrss: %D (max resident set size)<br>\nixrss: %D (integral shared text memory size)<br>\nidrss: %D (integral unshared data size)<br>\nisrss %D (integral unshared stack size)<br>\nminflt: %D (page reclaims)<br>\nmajflt: %D (page faults)<br>\nnswap: %D (swaps)<br>\ninblock: %D (block input operations)<br>\noublock: %D (block output operations)<br>\nmsgsnd: %D (messages sent)<br>\nmsgrcv: %D (messages received)<br>\nnsignals: %D (signals received)<br>\nnvcsw: %D (voluntary context switches)<br>\nnivcsw: %D (involuntary context switches)<br>\n",
                p_rusage->ru_maxrss,
                p_rusage->ru_ixrss,
                p_rusage->ru_idrss,
                p_rusage->ru_isrss,
                p_rusage->ru_minflt,
                p_rusage->ru_majflt,
                p_rusage->ru_nswap,
                p_rusage->ru_inblock,
                p_rusage->ru_oublock,
                p_rusage->ru_msgsnd,
                p_rusage->ru_msgrcv,
                p_rusage->ru_nsignals,
                p_rusage->ru_nvcsw,
                p_rusage->ru_nivcsw
                       );  

        p_user->msg_post( new string( c_msg ) );
 }
}

