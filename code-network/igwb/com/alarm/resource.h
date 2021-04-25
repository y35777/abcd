/*
    �ļ���:resource.h
    ���ļ��ж����˸澯ģ����ʹ�õ��ַ�������.
*/
#ifndef _RESOURCE_ALARM_H_
#define _RESOURCE_ALARM_H_
#include <string>

#define SZ_IGATEWAY_BILL          S_IGATEWAY_BILL
#define S_ALARM_CONFIG_PATH       "config/alarm/alarmconfig.cfg"

#ifdef _PLATFORM_WIN32
#define S_ALARM_ALMHISTORY              "d:\\other\\alarm"
#define S_ALARM_BOX_PORT_NAME_DEFAULT        "com2"
#else
#define S_ALARM_ALMHISTORY              "/var/other/alarm"
#define S_ALARM_BOX_PORT_NAME_DEFAULT   "/dev/term/b"
#endif //_PLATFORM_WIN32 

#define S_ALARM_ACCESSPOINT         "AccessPoint"
#define ALM_DEFAPTYPE               0
#define ALM_DEFAPCOUNT              1
#define ALM_DEFSERVER               0
#define ALM_BASEID                  0
#define ALM_DEFBINSEND              0
#define ALM_DEFAPID                 1
#define ALM_DEFALARMSHIELD          "11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"

//by ����	2004-5-19	��Ӧ���ⵥSWPD5267
#define ALM_DEFMMLALARMSHIELD		"11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"
//the end	2004-5-19	��Ӧ���ⵥSWPD5267

#define ONE_SECOND              1000
#define ONE_HOUR                (60*60)
#define BOX_COMM_BUFFER_SIZE    30

//������ 2002-04-17
#define DEFAULT_ACK_CNT             200
#define DEFAULT_CMD_DEL_LIMIT       500

#endif  //_RESOURCE_DEFINE_H_



