#ifndef __SYSTEM1_H__
#define __SYSTEM1_H__

#ifdef _PLATFORM_SOLARIS

int system1(const char* s );

#else 


#define system1 system


#endif //_PLATFORM_SOLARIS

#endif // __SYSTEM1_H__

