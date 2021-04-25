#ifndef __COLLECT_DEFINE_H__
#define __COLLECT_DEFINE_H__

//�������壬���ͳһ�ŵ�Common.h�ļ���
static const  int  MAX_ERROR_LEN = 1024; //��������Ϣ���ĳ���
static const  int  FT_SUCC = 0;  //�ļ�Э������ɹ�
static const  int  FT_FAIL = -1;  //�ļ�Э�����ʧ��
static const  int  FT_INVALIDPARAM = 1000; // ���������������
static const  int  FT_INITFAIL = 2000; //��ʼ��Э��ʧ��
static const  int  FT_CONNFAIL = 2001; //�����ļ������ʧ��
static const  int  FT_COPYFAIL = 2002; //�����ļ�ʧ��
static const  int  FT_ATTRNOTSET = 2200; //δ�������������
static const  int  FT_DELFAIL = 2201;  //�ļ�ɾ��ʧ��
static const  int  FT_RENAMEFAIL = 2202;  //�������ļ�ʧ��
static const  int  FT_LISTFAIL = 2203;  //�ļ��б�ʧ��


static const  int  COL_INITFAIL = 3000;      //�ɼ���ʼ��ʧ��
static const  int  COL_CONNFAIL = 3001;      //�ɼ����ӷ����
static const  int  COL_COPYFAIL = 3002;      //�ɼ��ļ�ʧ��
static const  int  COL_LISTFAIL = 3002;      //�ɼ��ļ��б�ʧ��
static const  int  COL_COLLECTFAIL = 3003;   //�ɼ�����ʧ��


#define S1240_CFG_LEN               40
#define COM_CFG_LEN                 40       //added by chenliangwei
#define TRANS_MODE_LEN              10       //added by chenliangwei


//�ɼ�����
#define CO_TYPE_S1240               "S1240"
#define CO_TYPE_5ESS                "5ESS"
#define CO_TYPE_EWSD_MM             "EWSD_MM"   //added by chenliangwei
#define CO_TYPE_EWSD_AMA            "EWSD_AMA"   //added by chenliangwei
#define CO_TYPE_NEC                 "NEC"    //added by chenliangwei
#define CO_TYPE_F150                "F150"   //added by chenliangwei
#define CO_TYPE_ZXJ                 "ZXJ"    //added by chenliangwei
#define CO_TYPE_CC08                "CC08"    //added by chenliangwei
#define CO_TYPE_TELLIN              "TELLIN"    //added by chenliangwei, 2003-08-14
#define CO_TYPE_COM                 "com"    //added by chenliangwei
#define CO_TYPE_METER_CC08          "METER_CC08"
#define CO_TYPE_SJZ_TELLIN			"SJZ_TELLIN"

//Э������
#define PL_TYPE_VERTEL              "Vertel"
#define PL_TYPE_FTP                 "FTP"    //added by chenliangwei
#define PL_TYPE_FILESCAN            "FileScan"  //added by wangfeng

//��������
#define CO_BILL_TYPE_GB                  "GB"
#define CO_BILL_TYPE_DETAIL              "Detail"
#define CO_BILL_TYPE_METER               "Meter"   //added by chenliangwei
#define CO_BILL_TYPE_AMA                 "AMA"     //added by chenliangwei
#define CO_BILL_TYPE_TAD                 "TAD"     //added by chenliangwei
#define CO_BILL_TYPE_CC08                "CC08"    //added by chenliangwei
#define CO_BILL_TYPE_ZJX_DETAIL300       "ZJX_DETAIL300"   //added by chenliangwei
#define CO_BILL_TYPE_ZJX_DETAIL302       "ZJX_DETAIL302"   //added by chenliangwei
#define CO_BILL_TYPE_ZJX_METER303        "ZJX_METER303"   //added by chenliangwei
#define CO_BILL_TYPE_ZJX_METER304        "ZJX_METER304"   //added by chenliangwei

#define CO_BILL_TYPE_OTHER               "Other"   //added by chenliangwei



//����ģʽ
#define TANS_MODE_ASC               "asc"    //added by chenliangwei
#define TANS_MODE_BIN               "bin"    //added by chenliangwei

#define FILE_TANS_MODE_ASC          0        //added by chenliangwei
#define FILE_TANS_MODE_BIN          1        //added by chenliangwei

#define FNA_FILE_NAME               "fna_%s.txt"
#define SZ_OUTPUT_PATH              "./output"

#define BUF_SIZE                    256
#define DIR_CHAR                    "/"

#endif //__COLLECT_DEFINE_H__
