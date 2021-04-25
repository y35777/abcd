
/*
    ��ͷ�ļ�����V200ϵͳʹ�õ����ͱ���.
*/

#ifndef _TYPE_DEFINE_H
#define _TYPE_DEFINE_H

    #define  FALSE           0
    #define  TRUE            1
    #define  NULL            0

    /* ����ϵͳʹ�õĻ����������� */

    typedef  long               INT4;
    typedef  short              INT2;
    typedef  int                INT;

    typedef  unsigned long      UINT4;
    typedef  unsigned short     UINT2;
    typedef  unsigned char      BYTE;

    typedef     int         BOOL;

    /* Windowsϵͳ��Unixϵͳ�ֱ��� */

    #ifdef   _PLATFORM_WIN32
        typedef     __int64     INT8;
    #else
        typedef     long long   INT8;
    #endif

    #ifdef   _PLATFORM_WIN32
    #else
        #define     MAX_PATH    PATH_MAX
    #endif

#if !defined(IGWB_Svc_Export)

#if defined(_PLATFORM_WIN32) && !defined(_USRDLL) && !defined(_LIB)
#define IGWB_Svc_Export __declspec (dllimport)
#else // defined(_PLATFORM_WIN32) && !defined(_USRDLL) && !defined(_LIB)
#define IGWB_Svc_Export
#endif // defined(_PLATFORM_WIN32) && !defined(_USRDLL) && !defined(_LIB)

#endif //!defined(IGWB_Svc_Export)

enum PS_CDR_TYPE
{
    PS_GCDR             =   0,
    PS_PARTIAL_GCDR     =   1,
    PS_MCDR             =   2,
    PS_PARTIAL_MCDR     =   3,
    PS_SCDR             =   4,
    PS_PARTIAL_SCDR     =   5,
    PS_SMOCDR           =   6,
    PS_SMTCDR           =   7,
    PS_LCSMOCDR         =   8,
    PS_LCSMTCDR         =   9,
    PS_LCSNICDR         =   10
};
#endif
