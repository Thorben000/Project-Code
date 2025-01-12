#ifndef DEBUG_H
#define DEBUG_H

#include <cstring>

#ifdef __WIN32__
    #define __PATH_SEP__ '\\'
#else
    #define __PATH_SEP__ '/'
#endif

#define __FILENAME__ (strrchr(__FILE__, __PATH_SEP__) ? strrchr(__FILE__, __PATH_SEP__) + 1 : __FILE__)

#define LOC "\x1b[1;35m[" << __FILENAME__ << ":" << __LINE__ << "]\x1b[0m "

#define panic(...) do { std::cout << LOC << "\x1b[1;31mERROR:\x1b[0m " << __VA_ARGS__ << std::endl; exit(1); } while (0)

#define assert_file_valid(file, path) do { if (!file.is_open()) { panic("Could not open file '" << path << "'."); } } while (0)

#endif