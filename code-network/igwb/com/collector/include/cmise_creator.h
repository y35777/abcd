#ifndef __CMISE_CREATOR_H__
#define __CMISE_CREATOR_H__

#include "i_cmise_fun.h"

#ifdef _PLATFORM_WIN32
    #define  Export_Flag    __declspec (dllexport)
  
#else  //UNIX
    #define  Export_Flag     
#endif

extern "C" {

Export_Flag	ICmiseFun *CmiseCreator();

}

#endif //__CMISE_CREATOR_H__
