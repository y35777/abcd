/*
    �ļ���:base_type.h
    �汾:0.1
    ����:2001-10-11

    ���ļ��ж�����ϵͳ��ʹ��ȫ����������.
*/

#ifndef __BASE_TYPE_H__
#define __BASE_TYPE_H__

#ifdef _PLATFORM_WIN32
    #pragma warning(disable:4786)
    #pragma warning(disable:4786)
    #pragma warning(disable:4503)
    #pragma warning(disable:4511)
    #pragma warning(disable:4663)
    #pragma warning(disable:4244)
    #pragma warning(disable:4100)
    #pragma warning(disable:4127)
    #pragma warning(disable:4706)
    #pragma warning(disable:4701)
#endif

/* ����ϵͳʹ�õ����ͱ��� */

#include "type_def.h"

/* ��׼C֧��ͷ�ļ� */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* STL֧��ͷ�ļ� */

#include <list>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <stack>
//#include <fstream>
#include <algorithm>

/* ����Windows��Unixϵͳͷ�ļ� */

#ifdef _PLATFORM_WIN32
#else

    /* Unix��׼ͷ�ļ� */

    #include <unistd.h>
    #include <sys/types.h>
    #include <limits.h>
    #include <strings.h>

#endif

/* ����ACE֧�ֿ� */

#include "ace/Process_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Thread.h"
#include "ace/OS.h"
#include "ace/Synch_T.h"
#include "ace/Synch.h"
#include "ace/Thread_Manager.h"
#include "ace/Message_Queue.h"
#include "ace/Reactor.h"
#include "ace/Dirent.h"
#include "ace/Profile_Timer.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"


#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))


#ifdef _PLATFORM_WIN32
    #define SNPRINTF    _snprintf
    #define VSNPRINTF   _vsnprintf
#else
    #define SNPRINTF    snprintf
    #define VSNPRINTF   vsnprintf
#endif

#define STRNCPY(szDst, szSrc, nLen)     strncpy(szDst, szSrc, nLen); \
                                        (szDst)[nLen - 1] = '\0'
#define STRNCAT(szDst, szSrc, nLen)     strncat(szDst, szSrc, nLen - strlen(szDst) - 1); \
                                        (szDst)[nLen - 1] = '\0'

/* ����ϵͳʹ�õĻ������ݽṹ */

#define LESS        std::less
#define STRING      std::string
#define MAP         std::map
#define LIST        std::list
#define VECTOR      std::vector
#define SET         std::set
#define PAIR        std::pair
#define STACK       std::stack
#define IOS_BASE    std::ios_base
/* ��Ϣ����Ľ����̶� */

#define MSGOUT_LEVEL_NONE       0x00    //Forbid
#define MSGOUT_LEVEL_URGENT     0x01    //����
#define MSGOUT_LEVEL_IMPORTANT  0x02    //��Ҫ
#define MSGOUT_LEVEL_SIDLY      0x04    //��Ҫ
#define MSGOUT_LEVEL_NORMAL     0x08    //һ��

/* ����ͳ����ID */
#define PERF_CPU1_USAGE         1       //CPU1��������			��1
#define PERF_CPU2_USAGE         2       //CPU2��������			��2
#define PERF_PHYSICAL_MEM_AVA   3       //�����ڴ���ÿռ�		��3
#define PERF_PHYSICAL_MEM_TOTAL 4       //�����ڴ���ܿռ�		��4
#define PERF_FRONT_SAVE_AVA     5       //ǰ���̿��ÿռ�		��5
#define PERF_FRONT_SAVE_TOTAL   6       //ǰ�����ܿռ�			��6
#define PERF_BACK_SAVE_AVA      7       //����̿��ÿռ�		��7
#define PERF_BACK_SAVE_TOTAL    8       //����̵��ܿռ�		��8

//by ldf 2004-06-01 Perf����CDRͳ��
#define PERF_TOTAL_SCDR			9		//���յ�S-CDR����		��9
#define PERF_TOTAL_PART_SCDR	10		//���յĲ���S-CDR����	��10
#define PERF_TOTAL_GCDR			11		//���յ�G-CDR����		��11
#define PERF_TOTAL_PART_GCDR	12		//���յ�G-CDR����		��12
#define PERF_TOTAL_MCDR			13		//���յ�M-CDR����		��13
#define PERF_TOTAL_PART_MCDR	14		//���յĲ���M-CDR����	��14
#define PERF_TOTAL_SMOCDR		15		//���յ�S-SMO-CDR����	��15
#define PERF_TOTAL_SMTCDR		16		//���յ�S-SMT-CDR����	��16
#define PERF_TOTAL_LCSMOCDR		17		//���յ�LCS-MO-CDR����	��17
#define PERF_TOTAL_LCSMTCDR		18		//���յ�LCS-MT-CDR����	��18
#define PERF_TOTAL_LCSNICDR		19		//���յ�LCS-NI-CDR����	��19
#define PERF_RECEIVED_CDR       20       //���ջ�������			��20
#define PERF_PROCESSED_CDR      21      //����������			��21


#define PERF_MANAGER_ID         100     //�������еĹ���ģ��
#define PERF_HEARTBEAT          101     //��������
#define PERF_NETWORK_ID         200     //����ģ��
#define GTP_REPEATDATA_FRAME    201     //GTP'�ظ���������֡��
#define GTP_NORMALDATA_FRAME    202     //GTP'������������֡��
#define GTP_ALL_FRAME           203     //GTP'��������֡��
#define GTP_ALLGSN_CONNECT      204     //������GSN��������
#define PERF_GTP_QUEUE_SIZE     205     //ͳ��GTP'�Ķ��г���
#define PERF_GTP_BANDWIDTH      206     //ͳ��GTP'���ô���
#define PERF_GTP_THROUGHPUT     207     //ͳ��GTP'֡��������

#define PERF_GTP_RESTIME        208     //ͳ��Ga�ӿ�ƽ����Ӧʱ��
#define PERF_MERGE_QUEUE        209     //ͳ�ƺϲ�����

#define SLD_REPEATDATA_FRAME    251     //���������ظ���������֡��
#define SLD_NORMALDATA_FRAME    252     //��������������������֡��
#define SLD_ALL_FRAME           253     //����������������֡��
#define GTP_ALLPRI_CONNECT      254     //��������MEM/SPC���������
#define GTP_ALLSEC_CONNECT      255     //�����б�MEM/SPC���������

#define PERF_FRONTSAVE_ID       300     //ǰ����ģ��
#define FS_PACKET_COUNT         301
#define FS_PACKET_SIZE          302
#define FS_WRITE_TIME           303
#define FS_FILE_CSN             304
#define FS_LAST_PID             305

#define PERF_BILLPROCESSOR_ID   400     //��������ģ��
#define BP_PACKET_COUNT         401
#define BP_PACKET_SIZE          402
#define BP_MERGE_TIME           403
#define BP_FRONT_COUNT          404
#define BP_PROCESS_TIME         405
#define BP_BACK_COUNT           406

#define PERF_BACKSAVE_ID        3500    //�����ģ��
#define BS_PACKET_COUNT         3501
#define BS_PACKET_SIZE          3502
#define BS_WRITE_TIME           3503
#define BS_FILE_CSN             3504
#define BS_LAST_PID             3505

#define PERF_MONITOR_ID         3600    //���ģ��
#define PERF_CLSTRINTERFACE_ID  3700    //˫���ӿ�ģ��
#define PERF_ALARM_ID           3800    //�澯ģ��
#define PERF_BACKUP_ID          3900    //����ģ��
#define PERF_MMLSERVER_ID       4000    //MMLģ��
#define PERF_BILLBROWSER_ID     4100    //���������ѯģ��
#define PERF_LOG_ID             4200    //��־����ģ��
#define PERF_MSGDIST_ID         4300    //��Ϣ��תģ��
#define PERF_CLSTR_ID           4400    //UNIX�µ�˫������ģ��,
                                        //��WINNT�µ�˫������ģ��
#define PERF_OMCSERVERINTERFACE_ID  4500    //OMCSERVER�Ľӿ�ģ��
#define PERF_PERF_ID            4600        //����ģ��

/*�澯ģ����Ҫ�Ķ���*/
#define MAX_ALARM_PARA          80          //��������
#define MAX_ALARM_CSN           999999999   //�澯��ˮ�ż���ֵ

/* ����ϵͳ���̺� */

enum PROCESS_TYPE
{
    PT_BROADCAST    = 255,  //�㲥��Ϣ�Ľ��ս��̺�
    PT_UNKNOWN      = 0,    //���̺Ų�ȷ��
    PT_OUTSIDE      = 1,    //�Ʒ�����֮��Ľ���
    PT_SELF         = 2,    //��������Լ�
    PT_KERNEL       = 3,    //KERNEL����
    PT_CLSTR        = 4,    //˫������
    PT_OM           = 5,    //����ά������
    PT_CFGSVR       = 6,    //Զ�̲������ý���(�����)
    PT_METERACC     = 7,    //�ƴα��ۼӽ���
    
    //GTP'�Ʒѽӿ����� Z30971 2004/06
    PT_BS           = 8,    //�Ʒѽӿڽ���
    
    PT_AP_BASE      = 10,   //���������̵���ʼ���
    PT_CO_BASE      = 50    //�ɼ����̵���ʼ���
};
#define MAX_PT_AP_TYPE      (PT_CO_BASE - 1)
#define MAX_PT_CO_TYPE      (PT_CO_BASE + 50)
/* ������������ */
enum  AP_TYPE
{
    AP_128,                 //128��B�ͻ���Ľ����
    AP_GSM,                 //GSM������
    AP_STP,                 //STP������
    AP_PS,                  //���������͵Ľ����
    AP_PS_RESERVED,         //���������͵Ľ����
    AP_SOFTX,               //SoftX300������
    AP_WLAN,                //WLAN������
    AP_OTHER = 20           //�������ͽ����
};

/* ����ϵͳģ��� */

enum MODULE_TYPE
{
    MT_UNKNOWN                  = 0,    //ģ��Ų�ȷ��
    MT_MANAGER                  = 1,    //�������еĹ���ģ��
    MT_NETWORK                  = 2,    //����ģ��
    MT_FRONTSAVE                = 3,    //ǰ����ģ��
    MT_BILLPROCESSOR            = 4,    //��������ģ��
    MT_BACKSAVE                 = 5,    //�����ģ��
    MT_MONITOR                  = 6,    //���ģ��
    MT_CLSTRINTERFACE           = 7,    //˫���ӿ�ģ��
    MT_ALARM                    = 8,    //�澯ģ��
    MT_BACKUP                   = 9,    //����ģ��
    MT_MMLSERVER                = 10,   //MMLģ��
    MT_BILLBROWSER              = 11,   //���������ѯģ��
    MT_LOG                      = 12,   //��־����ģ��
    MT_MSGDIST                  = 13,   //��Ϣ��תģ��

    MT_CLSTR                    = 14,   //UNIX�µ�˫������ģ��,
                                        //��WINNT�µ�˫������ģ��

    MT_OMCSERVERINTERFACE       = 15,   //OMCSERVER�Ľӿ�ģ��
    MT_PERF                     = 16,   //����ģ��
    MT_MMLDEBUG                 = 17,   //MML������Ϣ�����
    MT_BILLTASK                 = 18,   //SCP�����ָ�ģ��
    MT_CFGSVR                   = 19,   //Զ�̲�������ģ��(�����)
    MT_CDRSTAT                  = 20,   //����ͳ��ģ��
    MT_LICENSE_MANAGER          = 21,   //License Manager
    MT_COLLECTOR                = 22,   //�ɼ�ģ��
    MT_COLAGENT                 = 23,   //�ɼ�����ģ��
    MT_COLCONVERTOR             = 24,   //�ɼ�ת��ģ��
    MT_METERACC                 = 25,    //�ƴα��ۼ�ģ�� 

    ///////////////////////////////////////////////////////////////////////////////
    //GTP'�Ʒѽӿ����� Z30971 2004/06
    MT_CDRSENDER                = 26    //��������ģ��
};

enum CMD_TYPE
{
    /* Networkģ�鷢�͵������� */

    CMD_QUIT                    = 1,    //�˳�
    CMD_MSG_OUT                 = 2,    //MessageOut���
    CMD_PROTOCOL_TRACK          = 3,    //Э��������
    CMD_INNER_ALARM             = 4,    //�澯��Ϣ
    CMD_SYNC_DATA               = 5,    //ͬ��������Ϣ

    CMD_ACTIVE                  = 6,    //��������
    CMD_DEACTIVE                = 7,    //ȥ��������
    CMD_ANS_DEACTIVE            = 8,    //ȥ����Ӧ��
    CMD_ANS_INIT                = 9,    //��ʼ��Ӧ��

    CMD_AUXUPGRADE              = 10,   //������������
    CMD_ANS_AUXUPGRADE          = 11,   //��������Ӧ��
    CMD_SENDEMPTYPACKET         = 12,   //����ģ���Ѿ���ʼ���Ϳհ�
    CMD_DISKSPACERECOVERY       = 13,   //���̿ռ�ָ�
    CMD_AUXUPGRADE_FAIL         = 14,   //��������ʧ��   //by ldf 2002.4.8 ��Ӧ���ⵥ D12433

    /* ��������������̵�����ģ����ͣ�շ����� */
    CMD_STOP_NETWORK            = 15,

    /* ������ֹͣ������ģ�����¿�ʼ�շ����� */
    CMD_RESUME_NETWORK          = 16,

    CMD_REG_PROCESS             = 17,   //ҵ��ģ��ע����Ϣ
    CMD_HOST_DATA               = 18,   //��������������
    CMD_MEM_HANDSHAKE           = 19,   //��MEM���������Ϣ

    /* ˫���ӿ�ģ������ģ�鷢��Ϣ,���˳�  */
    CMD_EXIT                    = 20,

    CMD_TRACKMSG_ENABLE         = 21,   //����Ϣ����
    CMD_TRACKMSG_DISABLE        = 22,   //ȡ����Ϣ����
    CMD_TRACKPROTOCOL_ENABLE    = 23,   //��Э�����
    CMD_TRACKPROTOCOL_DISABLE   = 24,   //ȡ��Э�����

    //���ⵥ��SWPD00562��2003��7��23�գ�Zou Yongsheng�޸�
    CMD_SET_MSGOUT_LEVEL        = 25,   //������Ϣ���뼶��
    //�޸Ľ���

    CMD_PERFITERM_ENABLE        = 30,   //������ͳ��
    CMD_PERFITERM_DISABLE       = 31,   //ȡ������ͳ��
    CMD_TRACKMSG                = 32,   //������Ϣ(��Ϣ����)
    CMD_WRITE_LOG               = 33,   //��־��Ϣ
    CMD_READY                   = 34,   //ģ���ѳ�ʼ���ɹ�
    CMD_MODULE_ACTIVE           = 35,   //����ҵ��ģ��

    //�ӵ㣨����㡢�ɼ��㣩����������ģ�鷢�ͽӵ�����
    CMD_AP_NAME                 = 36,

    /* Agentģ�鷢�͵������� */
    CMD_MANUALOVER              = 50,   //�ֹ�����
    CMD_ANS_ACTIVE              = 51,   //����Ӧ��
    CMD_ASK_FAILOVER            = 52,   //�����Զ�����
    CMD_REQ_STATUS              = 53,   //��ѯ״̬
    CMD_ANS_HB_STATUS           = 54,   //Ӧ������״̬��ѯ

    /* MML SERVERģ�鷢�͵������� */
    CMD_USER_DOWN               = 69,   //�û�����������
    CMD_MML                     = 70,

    /*  ����ģ�鷢�͵����� */
    CMD_SAVE_PERF               = 71,   //
    CMD_ANS_CONNECT_STATUS      = 72,   //

    CMD_GET_APNAME              = 73,   //����ģ����ӵ��������ӵ�����
    CMD_GET_APLINK_STATE        = 74,   //����ģ����ӵ��������ӵ���·״̬
    CMD_ANS_PROC_STATUS         = 75,   //����ģ���monitorģ��Ľ���


    /*ǰ����̡���������֮���������*/
    CMD_SAVE_PACKET             = 100,  //���󱣴�ԭʼ������
    CMD_SYNC_PACKETID           = 101,  //����ͬ��ԭʼ������
    CMD_SAVE_SUCCEED            = 102,  //�ɹ�����Ӧ��
    CMD_BILL_PROCESS            = 103,  //�����ԭʼ���������мƷ�Ԥ����
    CMD_RESUME                  = 104,  //���������ָ�
    CMD_RESUME_OVER             = 105,  //֪ͨ�����ָ��Ѿ����
    CMD_SAVE_END_BILL           = 106,  //���󱣴����ջ�����
    CMD_INFORM_STATUS           = 107,  //֪ͨ״̬
    CMD_ALARM_RESPONSE          = 110,  //�澯ģ����Ӧ����

    /*ͨѶ��ģ�鴦�������*/
    CMD_BREAK_OPERATOR          = 130,  //ͨѶ���ж�ָ���û�

    /* �澯ģ���õ��������� */
    CMD_BIN_ALARM               = 150,

    //monitorģ���˫�����̵�������
    CMD_ANS_CLSTR_STATUS        = 160,
    
    /* �ɼ�ģ���õ��������� */
    CMD_CONVERT_BILLFILE        = 180,  //�ɼ�ģ�����뻰��ת��
    CMD_ANS_CONVERT_BILLFILE    = 181,  //ת��ģ��Բɼ�ģ���ת��Ӧ��
    CMD_REPORT_COLLECTOR_STATUS = 182,  //�ϱ��ɼ������״̬
    
    CMD_KILL_PROCESS            = 200,   //��Kernel���̷��͹ر�����

    /* License Manager�õ���������*/
    CMD_LICENSE_REQUEST         = 220,  //����License
    CMD_LICENSE_RESPONSE        = 221,  //Ӧ��License

    ///////////////////////////////////////////////////////////////////////////////
    //GTP'�Ʒѽӿ����� Z30971 2004/06/10
    CMD_BS_DATA                 = 222   //BS������������
};

/* MML�����붨�� */
enum MML_CMD_CODE
{
    /* MML SERVER (100 - 130) */

    #define MML_MAX_CMDCODE_NETWORK     100
    #define MML_MIN_CMDCODE_NETWORK     100
    #define MML_MAX_CMDCODE_MMLSERVER   130
    #define MML_MIN_CMDCODE_MMLSERVER   101

    //by ldf 2003-03-07 ��Ӧ���ⵥD20443
    MML_SHK_HAND        = 100,      //��������
    MML_ADD_OP          = 101,      //���Ӳ���Ա�ʺ�
    MML_RMV_OP          = 102,      //ɾ������Ա�ʺ�
    MML_MOD_OP          = 103,      //�޸��û�����
    MML_MOD_PWD         = 104,      //�޸Ĳ���Ա�Ŀ���
    MML_LST_OP          = 105,      //��ѯ����Ա�ĸ�����Ϣ
    MML_LGI             = 106,      //����Ա��¼
    MML_LGO             = 107,      //����Աע��
    MML_SHK             = 108,      //��������(��M2000V1����SHK:;����)
    MML_SET_OPCG        = 109,      //���ò���Ա��Ȩ��������
    MML_SET_OPTM        = 110,      //���ò���Ա�Ĳ���ʱ��
    MML_LST_CMDS        = 111,      //��ѯ��ǰ��ִ�е�����
    MML_ADD_CCG         = 112,      //����������������������
    MML_RMV_CCG         = 113,      //ɾ��������������������
    MML_LST_CCG         = 114,      //��ѯ������������������
    MML_QUERY_OP        = 115,      //��ѯ�ض��û�
    MML_ADD_IP          = 116,      //����IP����
    MML_LST_OPTM        = 117,      //��ѯ����ʱ��
    //by ldf 2004-01-30 OM����,�����������������M2000 CM ���μ�OM���İ�ȫƪ��
	MML_RVI_OP			= 118,		//M2000������Ԫ��֤����Ա���
	MML_SND_CRG			= 119,		//M2000�������ĵ���Ԫ
	//end
	MML_DLD_SOFTWARE    = 120,      //�������
    //by zkq 2004-07-31,������������̨�ĵ�½����
    MML_LGI_CFG         = 121,      //����̨�ĵ�½����  

    /* ��־����ģ�� (131 - 150) */

    #define MML_MAX_CMDCODE_LOG     150
    #define MML_MIN_CMDCODE_LOG     131

    MML_LST_LOG         = 131,      //��ѯ��־
    MML_SET_LOG_SDS     = 132,      //������־�Զ�ɾ������
    MML_RMV_LOG         = 133,      //�ֹ�ɾ����־
    MML_ARC_LOG         = 134,      //�ֹ��鵵��־
    MML_SET_LOG_ARCTIME = 135,      //������־�Զ��鵵ʱ��
    MML_GET_LOG_SDS     = 136,      //��ѯ��־�Զ�ɾ������
    MML_GET_LOG_ARCTIME = 137,      //��ѯ��־�Զ��鵵ʱ��
    MML_EXP_LOG			= 138,		//��־���
    MML_ULD_LOG			= 139,		//��־�ϴ�

    /* ����ģ�� (151 - 170) */

    #define MML_MAX_CMDCODE_BACKUP      170
    #define MML_MIN_CMDCODE_BACKUP      151

    MML_LST_AUTOBAK     = 151,      //��ѯ�Զ���������
    MML_SET_AUTOBAK     = 152,      //�����Զ���������
    MML_BAK_CDR         = 153,      //���ݻ����ļ�
    MML_DEL_CDR         = 154,      //ɾ�������ļ�
    
	/* om modify add by yangguang ---begin*/

	MML_BKP_CFGFILE    = 155,     //���������ļ�
	MML_ULD_CFGFILE    = 156,     //���������ļ�
	MML_DLD_CFGFILE    = 157,	  //���������ļ�
	MML_RTR_CFGFILE	   = 158,     //�ָ������ļ�

	/* om modify add by yangguang ---end*/    

    /*
        ���. (171 - 190)
        ��MML SERVER�Ƚ���,Ȼ������Ϣ��תģ��㲥.
    */

    #define MML_MAX_CMDCODE_FRAME       190
    #define MML_MIN_CMDCODE_FRAME       171

    //by ldf 2003-03-07 ��Ӧ���ⵥD20443
    MML_SET_PTRACK      = 171,      //����Э����ٿ���
    MML_SET_STRACK      = 172,      //����ҵ�����̸��ٿ���
    MML_SET_DBG         = 173,      //���õ����������
    MML_LST_DBG         = 174,      //��ѯ�����������
    MML_LST_VERSION     = 175,      //��ѯϵͳ�汾��Ϣ�����̰汾�ţ�
    MML_SET_PERF        = 176,      //��������ͳ�ƿ���
    MML_LST_VER         = 177,      //��ѯϵͳ�汾��Ϣ���̰汾�ţ�
    MML_LST_INTVER      = 178,      //��ѯϵͳ�汾��Ϣ�����汾�ţ�
    MML_BRK_OP          = 179,      //ǿ��ɱ������Ա

    /* ����ģ�� (191 - 200) */

    #define MML_MAX_CMDCODE_PERF        200
    #define MML_MIN_CMDCODE_PERF        191

    MML_LST_PERF        = 192,      //��ѯ����ͳ������
    MML_LST_LINK        = 193,      //��ѯGSN��IP��ַ
    MML_CRE_ALM         = 194,     //����ģ��澯
    MML_LST_PROC        = 195,      //
    MML_LST_ALLAP       = 196,      //�������нӵ�����
    MML_LST_APLINK      = 197,      //������·״̬
    MML_LST_COL_FILE    = 198,      //��ѯ�ɼ�����
	MML_SET_PERF_INV    = 199,      //��������ͳ��ʱ����
	MML_LST_PERF_INV    = 200,      //��ʾ����ͳ��ʱ����

    /* ���ģ��,KERNEL���� (201 - 205) */

    #define MML_MAX_CMDCODE_MONITOR     205
    #define MML_MIN_CMDCODE_MONITOR     201

    MML_AUP             = 201,      //��������
    MML_PRODUCE_CDR     = 202,    

    /* ˫��ģ��,Agent���� (206 - 210) */

    #define MML_MAX_CMDCODE_CLSTR       210
    #define MML_MIN_CMDCODE_CLSTR       206

    MML_SWP             = 206,      //�ֹ�����
    MML_RESET           = 207,      //�ֹ�����

    /* SCP�����ָ�ģ�� (221 - 300) */

    #define MML_MAX_CMDCODE_BILLTASK    300
    #define MML_MIN_CMDCODE_BILLTASK    211

    MML_SCP_RECOVERY    = 211,     //SCP�����ָ�����
    MML_SCP_CLOSE_ALM   = 212,     //SCP�رո澯����

    /* ����ͳ��ģ�� (301-320) */

    #define MML_MAX_CMDCODE_CDRSTAT    320
    #define MML_MIN_CMDCODE_CDRSTAT    301

    MML_LST_STATITEM    = 301,     //����ͳ���������������
    MML_SET_STATITEM    = 302,     //����ͳ����������������
    MML_BRW_STATINFO    = 303,     //����ͳ�����ͳ����Ϣ����
    MML_LST_STATPARA    = 304,     //����ͳ�������ѡ����������

    /* �ɼ�ģ�� (2001 - 2060) */
    #define MML_MAX_CMDCODE_COLLECTOR   500
    #define MML_MIN_CMDCODE_COLLECTOR   401

    MML_LST_COL_INFO    = 401,      //��ȡָ���Ĳɼ�������Ϣ
    MML_LST_COL_STATUS  = 402,      //��ѯ�ɼ������״̬
    MML_SET_COL_STATUS  = 403,      //���òɼ������״̬�������ȥ���
    MML_SET_ALARM_LIMIT = 404,      //���ø澯����
    MML_LST_MODULE_INFO = 405,      //��ѯָ��ģ��ļƷ���Ϣ
    MML_GET_FILE        = 406,      //�ֶ��ɼ�ָ�����ļ�
    MML_LST_COL_SCHDL   = 407,      //��ѯ�ɼ��ƻ�
    MML_SET_COL_SCHDL   = 408,      //���òɼ��ƻ�
    MML_LST_A_COL_SCHDL = 409,      //��ѯ���βɼ��ƻ�
    MML_SET_A_COL_SCHDL = 410,      //���õ��βɼ��ƻ�
    MML_SET_CCSCFGINFO  = 411,      //���òɼ�������Ϣ
    MML_GET_CCSCFGINFO  = 412,      //��ȡ�ɼ�������Ϣ
    MML_DEL_CCSCFGITEM  = 413,      //ɾ���ɼ�������Ϣ
    MML_MOD_CCSCFGINFO  = 414,      //�޸Ĳɼ�������Ϣ

    /* License Manager (501-550) */
    #define MML_MAX_CMDCODE_LICENSE_MANAGER    550
    #define MML_MIN_CMDCODE_LICENSE_MANAGER    501

    MML_REFRESH_LICENSE = 501,
    //add by zkq 2004-06-19
    MML_SHOW_LICENSE = 502,
    //end

    /* ��ѯ���ģ�� (1001 - 1500) */

    #define MML_MAX_CMDCODE_BROWSER     1500
    #define MML_MIN_CMDCODE_BROWSER     1001

    MML_LST_AP          = 1001,     //��ѯ����������б�
    MML_LST_BILLTYPE    = 1002,     //��ѯ��������
    MML_LST_CHL         = 1003,     //��ѯͨ����Ϣ
    MML_LST_ORGDATE     = 1004,     //��ѯԭʼ����������Ϣ
    MML_LST_FINALDATE   = 1005,     //��ѯ���ջ���������Ϣ
    MML_LST_FILES       = 1006,     //��ѯ�����ļ�
    MML_LST_FMT         = 1007,     //��ѯ��ʽ��Ϣ
    MML_LST_QUERYPARA   = 1008,     //�����ѯ��������
    MML_BRW_BILL        = 1009,     //�����������
    MML_LST_BILL        = 1010,     //��ѯ��������
    MML_STP_TASK        = 1011,     //��������������ѯ
    MML_SHK_WND         = 1012,     //��������
    MML_GET_PROGRESS    = 1013,     //����������
    MML_GET_PAGE_DATA   = 1014,     //���ҳ����
    MML_CLS_TASK        = 1015,     //�رջ���������ѯ
    //Added by MMS on 2002.11.21
    //���Ӳɼ������ļ����������
    MML_LST_COLROOT     = 1101,      //����ɼ������ļ���·��
    MML_LST_COLFILE     = 1102,      //����ָ��Ŀ¼�µ��ļ�������Ŀ¼
    //End

    /* Զ������ģ�� (1601 - 1700) */
    #define MML_MAX_CMDCODE_CFGSVR      1700
    #define MML_MIN_CMDCODE_CFGSVR      1601

    MML_LST_FILECLASS   = 1601,     //��ѯ���������ļ�����
    MML_LST_PARASECT    = 1602,     //��ѯ������Ϣ�ļ������ж�����
    MML_LST_PARAINFO    = 1603,     //��ѯ������Ϣ�ļ�ָ���ε����в�������Ϣ
    MML_LST_CFGSECT     = 1604,     //��ѯָ�������ļ������ж�����
    MML_LST_CFGINFO     = 1605,     //��ѯָ�������ļ���ָ���ε�������������Ϣ
    MML_SET_CFGINFO     = 1606,     //����ָ�������ļ�����������Ϣ
    MML_OPR_SYS         = 1607,     //����̨����������ر�iGWB����

    /* �澯ģ�� (2001 - 2060) */

    #define MML_MAX_CMDCODE_ALARM       2060
    #define MML_MIN_CMDCODE_ALARM       2001
    //by ldf 2004-01-30 OM���ģ�LST ALMFE ��Ϊ�� LST ALMAF���г���ǰ��澯��ֻ�г����ϸ澯��
    MML_LST_ALMAF		= 2001,     //��ѯϵͳ�еĵ�ǰ�澯
    //end    
    MML_LST_ALMLOG      = 2002,     //��ѯ��ʷ�澯,�����¼�
                                    //�澯�͹��ϸ澯
    MML_LST_IGWALMINF   = 2003,     //��ѯĳ���澯��ˮ�ŵ�
                                    //��ϸ�澯��Ϣ
    MML_RMV_ALMFLT      = 2005,     //�ֹ��ָ�ϵͳ�еĵ�ǰ�澯
    MML_RMV_IGWALMLOG   = 2006,     //ɾ������ʱ���֮�����ʷ�澯
    MML_SET_IGWALMACK   = 2007,     //�Թ��ϸ澯����ȷ��
    MML_SET_IGWEVTACK   = 2008,     //���¼��澯����ȷ��

    MML_SET_ALMDBG      = 2009,     //���ø澯������Ϣ�������
    MML_SET_ALMLVL      = 2010,     //���ø澯����
    MML_SET_ALMMSK      = 2011,     //���ø澯���ο���
    MML_SET_CLTRPT      = 2012,     //���ø澯�ϱ�����
    MML_SET_BOXRPT      = 2013,     //���ø澯���ϱ�����
    MML_LST_BOXLGT      = 2014,     //��ѯ�澯��״̬
    MML_LST_BOXVER      = 2015,     //��ѯ�澯�ư汾
    MML_CLR_BOXLGT      = 2016,     //�˹�Ϩ��澯��
    MML_STP_BOXSND      = 2017,     //ͣ�澯��
    MML_RST_ALMBOX      = 2018,     //�����澯��
    //by ldf 2004-01-30 OM���ģ������������ΪM2000����Ԫ�·�ͬ����׼�ţ���Ԫ������ֻ����һ�Σ��ٴν���ʱֱ�ӷ��سɹ������κδ���
    MML_CHK_ALM 		= 2019,		//�˶Ը澯��׼ͬ����
    MML_LST_IGWALMFE    = 2020,
    MML_LST_IGWALMLOG   = 2021,
    MML_RMV_IGWALMFLT   = 2022,
    //end  
    MML_LST_ALM      = 2023,
    MML_SET_ALMSHIELD  = 2024,

    //������ 2003-01-15 ����ģ������
    #define MML_MAX_CMDCODE_M2000       3060
    #define MML_MIN_CMDCODE_M2000       3001
    MML_CRE_PRFTSK      = 3001,     //�������ܲ�������
    MML_RMV_PRFTSK      = 3002,     //ɾ�����ܲ�������
    MML_LST_PRFTSK      = 3003,     //��ѯ���ܲ�������
    MML_ACT_PRFTSK      = 3004,     //��������
    MML_SPD_PRFTSK      = 3005,     //��������
    MML_LST_TSKRST      = 3006      //��ѯ������
};

/* ����MML�ͻ��˷�����MML���� */
#define MML_CMD_SHK         "SHK"       //MML�ͻ�������������������

/* ����ϵͳ��ʹ�õĴ������ */
enum  ERROR_CODE
{
    ERR_SUCCESS                 = 0,    //ִ�гɹ�
    ERR_FAIL                    = -1,   //����ʧ��
    ERR_HANDLE_INVALID          = -2,   //��Ч���

    /* ���´���������ļ������Ĵ�����Ϣ */

    ERR_FILE_NOT_EXIST          = -3,   //�ļ�������

    ERR_OPEN_CFG_FILE           = -7,   //���������ļ��޷���
    ERR_OPEN_FMT_FILE           = -8,   //��ʽ�����ļ��޷���
    ERR_OPEN_AP_FILE            = -9,   //����������ļ��޷���
    ERR_OPEN_CHL_FILE           = -10,  //ͨ�������ļ��޷���

    /*����Ϊ�澯ģ���õ��Ĵ�����*/

    ERR_OPEN_ALM_CFG_FILE       = -11,  //�澯�����ļ��޷���
    ERR_OPEN_HISTORY_ALARM      = -12,  //�޷�����ʷ�澯�ļ�
    ERR_WRITE_HISTORY_ALARM     = -13,  //д��ʷ�澯�ļ�ʧ��
    ERR_READ_HISTORY_ALARM      = -14,  //�޷���ȡ��ʷ�澯�ļ�
    ERR_FAIL_WARN               = -15,  //��Ч�澯����
    ERR_FAIL_MALLOC             = -16,  //�����ڴ�ʧ��
    ERR_FILE_NULL               = -17,  //�ļ�ָ����Ч
    ERR_WRITE_HISTORYALARM      = -18,  //д��ʷ�澯ʧ��
    ERR_POINTER_NULL            = -19,  //ָ��ΪNULL

    /* ���´��������MML SERVER�﷨������Ȩ�޹���ʱ�Ĵ�����Ϣ */

    ERR_SYNTAX_ERROR            = -20,  //�﷨����
    ERR_USER_NOT_EXIST          = -21,  //�û�������
    ERR_USER_ALREADY_EXIST      = -22,  //�û��Ѵ���
    ERR_NOT_ENOUGH_RIGHT        = -23,  //Ȩ�޲���
    ERR_INCORRECT_PASSWD        = -24,  //�������
    ERR_CANNOT_DEL_ADMIN        = -25,  //����ɾ������Ա
    ERR_MULTI_LOGIN             = -26,  //�ظ���½
    ERR_TOO_MUCH_USER           = -27,  //�û�������
    ERR_NOT_LOG_IN              = -28,  //�û�δ��½
    ERR_CMD_NOT_DEFINE          = -29,  //δ���������
    ERR_PASSWD_LEN_EXCCED       = -30,  //���볤�ȳ����涨ֵ
    /*�����Ǳ���ģ���õ��Ĵ�����*/
    ERR_HAVE_FILES_NOT_BACKUP   = -31,
    ERR_TERMINATED              = -32,


    /* ���´���������ڴ�����Ĵ�����Ϣ */

    ERR_INVALID_POINTER         = -50,  //ָ��Ƿ�
    ERR_MEM_ALLOCATION_ERROR    = -51,  //�ڴ�����ʧ��

    /* ���´���������ڲ�ģ��ͨ�ŵĴ�����Ϣ */

    ERR_INVALID_CMD_CODE        = -70,  //���ʶ��
    ERR_NOT_ENOUGH_PARA         = -71,  //�����������
    ERR_QUEUE_FULL              = -72,  //��Ϣ��������

    /* ���´����������������еĴ�����Ϣ */

    ERR_INVALID_IP_ADDR         = -100, //IP��ַ�Ƿ�
    ERR_BIND_PORT               = -101, //�޷��󶨶˿�
    ERR_LONG_FRAME              = -102, //��Ϣ֡����
    ERR_PEER_NOT_REACHABLE      = -103, //�Զ˲��ɴ�
    ERR_APP_TYPE_UNKNOWN        = -104, //Ӧ�����Ͳ���ʶ��
    ERR_SOCKET_NOT_PRESENT      = -105, //���Ӳ�����
    ERR_REACTOR_FULL            = -106, //ACE_Reactor������������
    ERR_SAME_ROUTER_REGISTERED  = -107, //ע��CommRouterʱ��ͻ
    ERR_COMMROUTER_NOT_FOUND    = -108, //���CFrmSocketʱ,�����ڶ�Ӧ���͵�CommRouter
    ERR_APTYPE_NOT_MATCH        = -109, //��������Ͳ�ƥ��
    ERR_SEND_QUEUE_FULL         = -110, //���Ͷ�������
    ERR_SOCKET_SEND_MSG         = -111, //��������ʱ,SOCKET����ʧ��
    ERR_MML_SOCKET_LIST_FULL    = -112, //MML�ͻ�SOCKET��������
    ERR_COMM_TASK_OPEN          = -113, //����CCommTask�߳�ʧ��
    ERR_LICENSE_EXPIRED         = -114, //License���ڻ�License��ʹ����������Ȩ��
    ERR_APP_NOT_ACTIVED         = -115, //Ӧ�ò�δ����

    /* ���´�����������ڹ���ҵ��ģ����д�����Ϣ */

    ERR_SUBMODULE_EXIT          = -130, //ҵ��ģ���쳣�˳�
    ERR_START_SUBMODULE         = -131, //�޷�����ҵ�����

    /*���´�����������ģ������еĴ�����Ϣ*/

    ERR_WRITE_MAIN_STATUS_FAIL  = -150, //д��״̬�ļ�ʧ��
    ERR_WRITE_BAK_STATUS_FAIL   = -151, //д��״̬�ļ�ʧ��
    ERR_READ_STATUS_FAIL        = -152, //��״̬�ļ�ʧ��
    ERR_READ_BILLFILE_FAIL      = -153, //�������ļ�ʧ��
    ERR_WRITE_BILLFILE_FAIL     = -154, //д�����ļ�ʧ��
    ERR_CREATE_BILLFILE_FAIL    = -155, //�����»����ļ�ʧ��
    ERR_PID_TOO_LARGE           = -156, //��ȡ״̬��¼ʱҪ��İ���̫��
    ERR_PID_NOT_FOUND           = -157, //��ȡ״̬��¼ʱҪ��İ���û���ҵ�
    ERR_PIDS_NOT_EXIST          = -158, //Ҫ��İ��ŷ�Χ��״̬�ļ��ж�������
    ERR_ALREADY_FINISH          = -159, //���ŷ�Χ�ڵĻ������Ѿ���ȡ�����
    ERR_INDEX_INVALID           = -160, //Ҫ��ȡ��״̬��¼����Ų��Ϸ�
    ERR_FAIL_READ               = -161, //��״̬��¼ʧ��
    ERR_NO_UNIT_EXIST           = -162, //״̬�ļ�Ϊ�´������ļ�,��״̬��¼����
    ERR_CLOSE_BILLFILE_FAIL     = -163, //�رյ�ǰ�����ļ�ʧ��
    
    /*�ֹ�����ģ��Ĵ�����Ϣ*/
    
	#define ERR_START_TOOMANY_TIMES -200
	#define ERR_BAK_FAIL -201
	#define ERR_EVT_EXIT -202
	#define ERR_DEL_FILE -203
	//#define ERR_LICENSE_EXPIRED -204   //license ����
	#define ERR_CRE_DIR         -205   //����Ŀ��Ŀ¼ʧ��
	#define ERR_CONNECT_FAILED  -206   //����Ŀ�����ʧ�� 
	#define ERR_SET_DIR         -207   //����Ŀ¼Ŀ¼ʧ��
	#define ERR_TRANS_FILE      -208   //���仰���ļ�ʧ��    

    /* MML SERVERʹ�õĴ����� */

    ERR_PARA_NOT_EXIST          = -501,  //����������
    ERR_CHILD_PARA_NOT_EXIST    = -502,  //�Ӳ���������
    ERR_NO_CRU_USER             = -503,  //�û�������
    ERR_FULL_USER               = -504,  //�û�����
    ERR_NOT_ENOUGHT_RIGHT       = -505,  //�û�Ȩ�޲���
    ERR_PASSWORD_ERR            = -506,  //�������
    ERR_PWD_UNMATCH             = -507,  //
    ERR_NOT_LOGIN               = -508,  //�û�δ��¼
    ERR_ACCOUNT_IS_EMPTY        = -509,  //�ʺ���Ϊ��
    ERR_USER_IN_LINE            = -510,  //�û�����
    ERR_USER_LGO_AGAIN          = -511,  //�ظ�LOGOUT
    ERR_PWD_OUTDATED            = -512,  //�������
    ERR_EXPIRE_USER				= -514,	 //�û����޹���
    //by ldf 2004-01-06 OM����
    ERR_ADD_M2000_ACCOUNT		= -513,  //���������M2000��ͷ���û�
    //end


    /* CLUSTER MANAGERʹ�õĴ����� */
    ERR_CLS_STATUS_TRANSFER     = -601, //״̬Ǩ�Ʋ����Ϲ���
    ERR_FAILURE_STATUS          = -602, //�Ѵ��ڹ���״̬
    ERR_HEARTBEAT_CREATE        = -603, //����������·ʧ��
    ERR_COMMTASK_OPEN           = -604, //��CommTaskʧ��
    ERR_HB_THREAD_SPAWN         = -605, //��������߳�����ʧ��
    ERR_SEND_LENGTH_EXCCEED     = -606, //��������֡�����ߴ�����
    ERR_NO_AVAILABLE_HB         = -607, //�����ڿ��õ�������·
    ERR_INIT_CLUSTER_NETWORK    = -608, //��ʼ��˫��ͨѶ��ʧ��
    ERR_INIT_CLUSTER_RECOURSE   = -609, //��ʼ��˫����Դ��ʧ��
    ERR_HEART_BEAT_BROKEN       = -610, //�������жϣ����ܽ���FailOver����
    ERR_SHARED_DISK_FAIL        = -611, //�������й��ϣ����ܽ���FailOver����
    ERR_SEND_FORBIDDED          = -612, //����Ҫ�˳������Ͳ����ѳ���ֹ
    ERR_CREATE_RES_OBJECT       = -613, //������Դ����ʧ��
    ERR_FILE_NOT_PRESENT        = -614, //�ļ�������
    ERR_BUFFER_NOT_ENOUGH       = -615, //���������Ȳ���
    ERR_GET_CHL_NUM             = -616, //��ȡͨ����ʧ��
    ERR_INIT_COM_CLUSTER        = -617, //��ʼ��CComCluster����ʧ��
    ERR_INIT_SYNC_MGR           = -618, //��ʼ��CSyncManager����ʧ��
    ERR_INIT_ASYN_OBJ           = -619, //��ʼ��CAsynResKernel����ʧ��
    ERR_INIT_KERNEL_EXIST       = -620, //��ʼ��ʱ����KERNEL���ڣ���ɱ����

    /* Collectorʹ�õĴ����� */
    ERR_FILE_BE_DELETED         = -621, //�����ļ��ѱ�ɾ���������ٲ�
    ERR_FILE_BEING_COLLECTED    = -622, //�����ļ����ڱ��ɼ��������ٲ�
    ERR_NEW_FILE_CANNOT_BE_CO   = -623, //ϵͳ������ɼ����µĿ���δ�رյĻ����ļ�
    ERR_FILE_CANNOT_BE_FOUND    = -624, //ָ�����ļ�δ�ҵ�
    ERR_FILE_STATUS_NOT_CORRECT = -625, //�ļ�״̬����ȷ�����ܽ����ֹ��ɼ�
    ERR_FILE_NAME_NOT_CORRECT   = -626, //Ŀ���ļ������Ϸ�
    ERR_FILE_BE_PUT_IN_LIST     = -627,  //ָ�����ļ�������ɼ��Զ���
    
    /*����ͳ��ģ��ʹ�õĴ�����*/
    ERR_STAT_FAIL               = -700,    //ͳ�ƻ������ɹ�
    ERR_STAT_OPEN_FILE_FAIL     = -701     //�����ļ����ܴ�
};


/* ����˫������ԭ�� */

enum EFAILOVERCAUSE_TYPE
{
    ERROR_WRITEFILE                 = 0, //д�ļ�ʧ��
    ERROR_NOENOUGHDISK_PRIMARY      = 1, //ǰ���̿ռ䲻��
    ERROR_NOENOUGHDISK_SECONDARY    = 2, //����̿ռ䲻��
    ERROR_PROCESS_EXCEPTION         = 3, //ҵ������쳣�˳�
    ERROR_ACTIVE                    = 4, //����ʧ��
    ERROR_RESOURCE                  = 5, //��ԴʧЧ
    ERROR_HARDDISK                  = 6  //����Ӳ�̻�
};

/* ϵͳ��ʹ�õ���Ϣ�ṹ. */

struct MSG_INTRA
{
    BYTE    SenderPid;                  //����ģ�����ڵĽ��̺�
    BYTE    SenderMid;                  //����ģ���ģ���
    BYTE    ReceiverPid;                //����ģ�����ڵĽ��̺�
    BYTE    ReceiverMid;                //����ģ���ģ���
    UINT2   AppType;                    //Ӧ������
    UINT2   ClientNo;                   //ĳ�����͵�MML�ͻ��˵ı��
    char    UserName[48];               //MML�û���
    UINT4   IPAddr;                     //MML�ͻ��˵�IP��ַ
    UINT2   CmdCode;                    //������
    UINT4   Len;                        //Data��ĳ���
    BYTE    Data[1];                    //Data��

    /* ������Ϣͷ����. */

#define MSG_INTRA_HEADLENGTH  (offsetof(MSG_INTRA,Data))

    /*
        ���嵥����Ϣ��󳤶�.
        ��Ϊ��Ϣ����ͨ��UDPЭ�鷢��,������󳤶�Ϊ512K.
    */

#define MSG_INTRA_MAXSIZE     512 * 1024

    void* operator new(size_t classSize, size_t DataLength = 1)
    {
        void *ptr = NULL;
        UINT4 uLen = (DataLength > 1) ? DataLength:1;

        UINT4 length = uLen + MSG_INTRA_HEADLENGTH;
        if (length > MSG_INTRA_MAXSIZE)
        {
            return NULL;
        }

        ptr = ::new char[length];
        if(ptr)
        {
            memset(ptr, 0, length);
            ((MSG_INTRA*)ptr)->Len = length - MSG_INTRA_HEADLENGTH;
        }

        return ptr;
    };
};

#define FAILOVERCAUSE_MAXSIZE      MSG_INTRA_MAXSIZE

//���嵹��ԭ��ṹ
struct SFailOverCause
{
    PROCESS_TYPE PID;           //����ID
    MODULE_TYPE MID;            //ģ��ID
    EFAILOVERCAUSE_TYPE Cause;  //����ԭ��
    UINT4 Length;               //���������ݳ���
    BYTE Para[1];

#define FAILOVERCAUSE_HEADLENTGH   (offsetof(SFailOverCause,Para))

    //����new������
    void* operator new(size_t classSize, size_t DataLength = 1)
    {
        void *ptr = NULL;
        UINT4 uDataLength = max(DataLength, 1);
        UINT4 length = uDataLength + FAILOVERCAUSE_HEADLENTGH;

        if (length > FAILOVERCAUSE_MAXSIZE)
        {
             return NULL;
        }

        ptr = ::new char[length];
        if (ptr)
        {
            memset(ptr, 0, length);
            ((SFailOverCause *)ptr)->Length = uDataLength;
        }

        return ptr;
    };
};


#define NO_DISK_FULL          0x00    //�޴��̿ռ䲻����쳣
#define PRIMARY_DISK_FULL     0x01    //ǰ���̿ռ䲻��
#define SECONDARY_DISK_FULL   0x02    //����̿ռ䲻��


enum APP_TYPE
{
    APP_MSG = 0,        //��Ϣ��ת
    APP_MAIN,           //����Ӧ������
    APP_MSC_PRIMARY,    //��������Э������˿�
    APP_MSC_SECOND,     //�������ڵı��˿�
    APP_SOFTX_PRIMARY,  //SoftX3000�еĻ�������Э������˿�
    APP_SOFTX_SECOND,   //SoftX3000�еĻ�������Э��ı��˿�
    APP_MML_WARN,       //MML�ͻ��˵ĸ澯�˿�
    APP_MML_NORM,       //MML�ͻ��˵���ͨ����ά���˿�
    APP_MML_DEBUG,      //MML�ĵ�����Ϣ����˿ڣ�Э���
                        //�١�������Ϣ��ͨ���ö˿������MML�ͻ���
    APP_MML_WHGUI,      //MML��ά��̨GUI֡��ʽ�˿ڣ���ʱ��ʹ��
    APP_ALARMBOX,       //�澯�����
    APP_GTP_TCP,        //TCP��ʽ��GTP'
    APP_GTP_UDP,        //UDP��ʽ��GTP'
	//by ldf 2004-02-12
	APP_MML_PERF,      //MML���ܶ˿�
    APP_SYN_CONFIG,       //��������ͬ���˿�   

    ///////////////////////////////////////////////////////////////////////////////
    //GTP'�Ʒѽӿ����� Z30971 2004/06/10
    APP_BS_GTP_TCP,    //�Ʒѽӿ�TCP ��ʽ��GTP'
    APP_BS_GTP_UDP     //�Ʒѽӿ�UDP ��ʽ��GTP' 
};

//����澯��
enum ALARM_ID
{
    //����ģ��澯��
    ALARM_ID_DISK_ROOM           =  1,   //���̿ռ䲻��

    //ǰ�����ģ��澯��
    ALARM_ID_NO_FETCH_FILE_FAULT =  3,   //��ʱ��δȡ�����ļ��Ĺ��ϸ澯

    //˫��ģ��澯��
    ALARM_ID_HEARTBEAT           =  5,   //�����жϸ澯
    ALARM_ID_FAILOVER            =  7,   //˫�������澯
    ALARM_ID_PRVNET              =  9,   //˽���жϸ澯
    ALARM_ID_SVCRES              =  11,  //��ԴʧЧ�澯
    ALARM_ID_IPRES               =  13,  //IP��ԴʧЧ
    ALARM_ID_VOLUME              =  15,  //����ԴʧЧ
    ALARM_ID_DISK                =  17,  //Ӳ�̹���

    //�����澯��
    ALARM_ID_WRITE_FILE_FAILED   =  19,  //д�ļ�ʧ�ܵ��¼��澯

    //�쳣����
    ALARM_ID_ABNORMAL_BILL       =  21,  //�쳣����

    ALARM_ID_BACKUP_FAIL         =  23,  //�����ļ�ʧ��
    //������ 2002-12-07
    ALARM_ID_BACKUP_CONNECT_FAIL =  25,   //����ʧ��
    ALARM_ID_CPU_REACH_LIMIT     =  27,   //CPU ����
    ALARM_ID_MEM_REACH_LIMIT     =  29,   //Memeory����(major)    
    ALARM_ID_MML_LOGIN_FAIL_MUCH =  31,   //MML�û���½ʧ�ܹ���
    ALARM_ID_LICENSE_MANAGEMENT  =  33,   //License����澯
    ALARM_ID_MANUAL_SWITCHOVER   =  35,   //�ֹ�����
    ALARM_ID_HARDWARE_ALARM      =  37,   //Ӳ���澯
    ALARM_ID_RESET               =  39,   //ϵͳ����
    ALARM_ID_X25_LINK_FAIL       =  41,   //�ɼ���·����
    ALARM_ID_FILE_SN_CHECK_FAULT =  42,   //�ļ����к�У�����
    ALARM_ID_COLLECT_FILE_FAULT  =  45,   //�ɼ��ļ�ʧ��
    ALARM_ID_COLLECT_FILE_LOST   =  47,   //�ļ�©�ɸ澯
    ALARM_ID_CONVERT_FILE_FAULT  =  49,   //ת���ļ�ʧ��
    ALARM_ID_ALARMBOX_CONNECT_FAIL = 51,    //�澯�����  
    ALARM_ID_USEREINFO_CHANGED   = 53,    //�û���Ϣ����
    ALARM_ID_DISK_ROOM_MINOR     = 55,    //���̿ռ䲻���Ҫ�澯
    ALARM_ID_MEM_REACH_MID_LIMIT = 57,     //�ڴ����(minor)
    ALARM_ID_REQUSET_TIMEOUT     =59      //����֡GSN����Ӧ
   
};

/******** ����澯���� *******************/

//�����澯����
#define ALARM_PARA_NONE           0xFF   //0xFF��ʾ�澯�޲���

//����֡GSN����Ӧ
#define ALARM_PATA_GSN_NO_REQUEST       0

//�ռ䲻��澯����
#define ALARM_PARA_FRONT_SAVE     0      //ǰ����
#define ALARM_PARA_BACK_SAVE      1      //�����
#define ALARM_PARA_BACKUP_DEV     2      //���ݽ���

//д�ļ�ʧ�ܸ澯����
#define ALARM_PARA_ORIG_BILL      0      //дԭʼ�����ļ�ʧ��
#define ALARM_PARA_FINAL_BILL     1      //д���ջ����ļ�ʧ��
#define ALARM_PARA_BACKUP         2      //�����ļ�ʧ��

//˫��ģ��澯����
#define ALARM_PARA_FRONT_VOLUME   0
#define ALARM_PARA_BACK_VOLUME    1

//����δ���ݻ����澯����
#define ALARM_PARA_ORIG_BILL_FILE   0    //ԭʼ�����ļ�
#define ALARM_PARA_FINAL_BILL_FILE  1    //���ջ����ļ�

//�쳣�����澯����
#define ALARM_PARA_FRAME_ERR      0     //�������ȳ���
#define ALARM_PARA_ENCODING_ERR   1     //�����������
#define ALARM_PARA_DECODING_ERR   2     //�����������
#define ALARM_PARA_TYPE_ERR       3     //�������ͳ���
#define ALARM_PARA_DUPLICATED_BILL  4   //�ظ�����

//license����
#define ALARM_PARA_LICENSE_INVALID  0    //��Ч��license
#define ALARM_PARA_LICENSE_EXPIRE   1    //��ʱlicense��Ҫ����

//�û���Ϣ���¸澯
#define ALARM_PARA_ADD_USERINFO       0
#define ALARM_PARA_DEL_USERINFO       1  
#define ALARM_PARA_MOD_USERINFO       2 

//�û��Ƿ���¼
#define ALARM_PARA_LOGIN_FAIL_TOO_MUCH      0
#define ALARM_PARA_UNAUTHORIZED_LOGIN       1
/******** �澯����������� ***************/


/*�澯���Ͷ���*/
enum ALARM_TYPE
{
    AT_EVENT,           //�¼��澯
    AT_FAULT,           //���ϸ澯
    AT_RESTORE,         //�ָ��澯
    AT_HISTORY,         //��ʷ�澯
    AT_NOT_DEFINE       //δ��������
};

enum ALARM_ADD_RESULT
{
    ADD_OK,             //Add a Alarm to list ok
    ADD_REPEATED,       //Add a repeated alarm to list
    ADD_NOT_MATCH,      //Alarm which to be add to list
                        //is not a standard alarm
    INVALID_ALARM       //Is not a alar
};

/* �澯��Ϣ�ṹ */
typedef struct  _InnerAlarm
{
    BYTE        yAlarmType;     //�澯����,ΪALARM_TYPEö������
    UINT2       uAlarmID;       //�ڲ��澯ID
    BYTE        yAlarmPara[MAX_ALARM_PARA];     //�澯����(0xFF��ʾ�޸澯����)
    _InnerAlarm()
    {
        memset(yAlarmPara, 0xff, sizeof(yAlarmPara));
    };
} SInnerAlarm,  *PInnerAlarm;

struct SLog
{
#define LOG_USR_NAME_LEN 48
#define LOG_OPER_CMD_LEN 20
#define LOG_CMD_DESC_LEN 256
    UINT4 uSerialNo;            //��־���кţ�����־ģ���Զ�����

    BYTE LogType;               //��־����

    /* ��־��Ϣ���� */

    enum LOG_TYPE               //��־����ö��
    {
        LOG_SYS     = 0,        //ϵͳ��־
        LOG_OPER    = 1         //������־
    };

    char szUserName[LOG_USR_NAME_LEN];        //������־���û���
                                //�����ϵͳ�ڲ�����־��
                                //��Ϊ������־��ģ������

    UINT4 uIPAddr;              //MML�ͻ��˵�IP��ַ��
                                //�����ϵͳ�ڲ�����־����Ϊ��

    time_t tLogTime;            //��־����ʱ�䣬����־ģ����д

    BYTE OperResult;            //�������

    /* ��������� */

    enum  OPER_RESULT           //�������ö��
    {
        OPER_SUCCESS    = 0,    //�ɹ�
        OPER_FAIL       = 1,    //ʧ��
        OPER_UNKNOWN    = 2     //δ֪
    };

    char szOperCmd[LOG_OPER_CMD_LEN];         //MML��������������ϵͳ�ڲ���־�򸳿�
    char szCmdDesc[LOG_CMD_DESC_LEN];        //MML����������������ϸ����
};

/* ����ģ���״̬���� */

enum EWKMGR_STATUS
{
    WKMGR_INIT_PENDING,     //Module Manager���ڳ�ʼ��״̬,
                            //�������ȴ�ҵ����̷���

    WKMGR_READY,            //Module Manager��ʼ���ɹ�,
                            //���ڵȴ�Kernel�ļ�������

    WKMGR_ACTIVE,           //Module Manager���ڼ���״̬
    WKMGR_FAILURE           //Module Manager���ڹ��ϵ�״̬
};

//�ļ����ƽڵ�ṹ
struct SFileNameNode
{
    time_t tFileTime;  //�ļ��޸�ʱ��
    //�ļ����ƣ����������ļ����ƣ�������Ŀ¼�ͻ����ļ�����
    char szFileName[MAX_PATH];
    bool operator > (const SFileNameNode& y) const
    {
        if (tFileTime == y.tFileTime)
        {
            return (strcmp(szFileName, y.szFileName) > 0);
        }
        else
        {
            return (tFileTime > y.tFileTime);
        }
    }
    bool operator < (const SFileNameNode& y) const
    {
        if (tFileTime == y.tFileTime)
        {
            return (strcmp(szFileName, y.szFileName) < 0);
        }
        else
        {
            return (tFileTime < y.tFileTime);
        }
    }
};

//License��Ӧ
struct SLicenseResp
{
    int     nLicenseCount;      //License��Ȩ����
};

/* ����ģ��ʹ�õı�׼ʱ�� */

#define ONE_MINUTE_IN_SECOND    60          //1����,����Ϊ��λ
#define FIVE_MINUTE_IN_SECOND   (5 * 60)    //5����,����Ϊ��λ
#define FIVE_SECOND_IN_MILISEC  (5 * 1000)  //5����,�Ժ���Ϊ��λ
#define ONE_SECOND_IN_MILISEC   1000        //1����,�Ժ���Ϊ��λ
#define ONE_DAY_IN_SECOND       (24 * 3600) //1��,����Ϊ��λ

/* Backup use */
#define CHANNEL0        "default"
struct SLinkStat
{
    char  szLocalAddr[50];    //����IP��ַ
    char  szModuleLabel[50];  //ģ��ţ��Ի�������Э����ģ��ţ���GTP'Э����ȱʡ1
    char  szLinkLabel[50];    //��·��,����BAM��,IP��ַ
    BYTE  yStat;              //����״̬
};
#define    ONLINK  0
#define    OFFLINK 1

typedef struct _ProcStatus
{
    char szProcName[32];
    int  nPid;
    char szStatus[16];
} SProcStatus,*PProcStatus;

enum  COLLECT_TYPE
{
    AP_IGWB = AP_OTHER,     //�Ʒ��������ͽ���㣬kernel������ap_proc
	AP_S1240,               //S1240J��������ͣ�kernel������co_proc
	AP_5ESS,                //5ESS��������ͣ�kernel������co_proc
	AP_EWSD,                //EWSD��������ͣ�kernel������co_proc
	AP_CC08,                //CC08��������ͣ�kernel������co_proc
	AP_ZXJ,                 //ZXJ��������ͣ�kernel������co_proc
	AP_TEXT_IN              //������������������ͣ�kernel������co_proc
};

//�ɼ����������״̬
enum E_CO_STATUS
{
    CS_ACTIVED     = 0,     //����״̬
    CS_DEACTIVED   = 1,     //δ����״̬
    CS_LIC_EXPIRED = 2      //δ��Ȩ
};

#define ASN1_BUF_SIZE (256 * 1024)
//�������͵�ö�ٶ���
enum BillType
{
    ORIG_BILL = 0,      //ԭʼ����
    FINAL_BILL = 1      //���ջ���
};

//added by lucy 2004-6-11 WLAN����̶�̬�����ƶ���
#define WLAN_BACKSAVE_DLL_NAME "wlan_cmcc_s"
//WLAN ��̬�����ƵĶ���
#define WLAN_DLL_NAME "wlan"
//end


//GTP'Э���Э�鷽ʽ
enum GTP_MODE
{
    TCP_MODE = 0,    //TCPЭ��
    UDP_MODE = 1     //UDPЭ��
};
#endif

