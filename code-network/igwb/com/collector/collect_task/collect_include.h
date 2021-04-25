#ifndef __COLLECT_INCLUDE_H__
#define __COLLECT_INCLUDE_H__

#include <sys/types.h>  /* required for some of our prototypes; wait(), time()*/
#include <stdio.h>      /*always stdio.h!*/


#include <stdlib.h>
#include <string.h>     /*memeset(), strerror()*/



#include <stdarg.h>
#include <string.h>     /*memeset(), strerror()*/
#include <errno.h>      /*errno*/

#include <time.h>       /*time()*/
#include <sys/stat.h>   /*chmod*/

#include <string>
#include <vector>
#include <map>

#ifdef _PLATFORM_WIN32
    #include <direct.h>     /*get current directory*/
#else  //_PLATFORM_WIN32
    #include <unistd.h>
#endif //_PLATFORM_WIN32

using namespace std;

#define LESS        std::less
#define STRING      std::string
#define MAP         std::map
#define LIST        std::list
#define VECTOR      std::vector
#define SET         std::set
#define PAIR        std::pair
#define STACK       std::stack

#endif //__COLLECT_INCLUDE_H__
