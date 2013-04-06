// contains header files which are included by all classes.

// include some std headers. 
#include <iostream>

// since thread synchronization is a big issue this header needs
// to be included  by every other file too.
#include <pthread.h>

// std::string.
#include <string>

// include all the custom global variables.
#include "glob.h"
// include all the custom messages. 
#include "msgs.h"
