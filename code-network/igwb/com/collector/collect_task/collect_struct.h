#ifndef __COLLECT_STRUCT_H__
#define __COLLECT_STRUCT_H__


//�������ݽṹ����

/*
�ṹ����SCreateCollect���ڴ�����ͬ�Ĳɼ�����Ȼ��ɼ�����
�ٴ�����ͬ���ļ�����Э����󣬶������£�
*/
struct SCreateCollect
{
    int  nSwitchIndex;
    char szSwitchName[40];  //��������
    char szCollectId[40];   //�ɼ������ʶ��,"5ESS"��ʾ����C5ESSCollect����
    char szProtocolId[40];  /*�ļ�Э���ʶ��,"Vertel"��ʾ����Vertel��FTAM����*/

};

//���ݽṹ����
//�ṹ����SColFileReq���ڶ���ɼ������ļ�ʱ������������������£�
struct SColFileReq
{
    char  szSrcName[MAX_PATH];  //Դ�ļ���
    char  szDstName[MAX_PATH];  //Ŀ���ļ���
    int   nFileSize;            //�ļ���С�����ڿ��Ʋɼ��ļ�ʱ�����ȴ�ʱ��(5ESSҪ�õ�)
};

//���ݽṹ����

/*
�ṹ����SGetFileReq �����ļ�������Ա�������������ݣ���������
*/
struct SGetFileReq
{
    char   szSrcPath[256];  //Դ�ļ�·��
    char   szSrcName[100];  //Դ�ļ���
    char   szDstPath[256];  //Ŀ���ļ�·��
    char   szDstName[100];  //Ŀ���ļ���
    int    nFileSize;       //�ļ���С�������������Ƶȴ�ʱ��(optional)
};

enum E_COLLECT_STATUS
{
    CS_NOT_COLLECTED        = 0,
    CS_COLLECTING           = 1,
    CS_SETTING_COLLECTED    = 2,
    CS_COLLECTED            = 3,
};

#define S_CS_NOT_COLLECTED      "δ�ɼ�"
#define S_CS_COLLECTING         "���ڲɼ�"
#define S_CS_COLLECTED          "�Ѳɼ�"

// �ļ���Ϣ���ݽṹ����
struct  SFileInfo
{
    char            szFileName[MAX_PATH]; //�ļ���
    char            szModTime[10];        //�޸�ʱ��
    int             nAttr;                //�ļ����ԣ�ATTR_DIR, ATTR_FILE
    unsigned long   nFileSize;            //�ļ���С
    int             nCollectStatus;       //�ļ��ɼ�״̬���μ�E_COLLECT_STATUS
};

/*
����SListOut, ����ļ����������õ��ļ��б�ĺ�����
�����������,��������:
*/
struct SListOut
{
    VECTOR <SFileInfo> FileList;  //���ص��ļ���Ϣ�б�
};

//SS1240TPInfo����ÿ��S1240������TP�Ļ�����Ϣ
struct  SS1240TPInfo
{
    char  szNename[S1240_CFG_LEN];             //Nename
    char  szTPID[S1240_CFG_LEN];               //TP��
    char  szBilltype[S1240_CFG_LEN];           //��������
    BOOL  bDumped;                             //�ɼ����ļ��Ƿ���DUMPED
    int   nFinalStatus;                        //Ŀ��״̬(-1:null|dumped|release)
    char  szLocalPath[MAX_PATH];               //���ظ�Ŀ¼
};

struct SSwitchBaseInfo
{
    char  szRemoteAddr[S1240_CFG_LEN];         //��������ַ
    char  szRemoteUsr[S1240_CFG_LEN];          //�������û���
    char  szRemotePwd[S1240_CFG_LEN];          //����������

    char  szLocalAddr[S1240_CFG_LEN];          //���ص�ַ
    char  szLocalUsr[S1240_CFG_LEN];           //�����û���
    char  szLocalPwd[S1240_CFG_LEN];           //��������
};

struct SS1240SwitchInfo
{
    char  szSwitchName[S1240_CFG_LEN];
    char  szOfficeNum[S1240_CFG_LEN];
    char  szSwitchCode[S1240_CFG_LEN];
    char  szRemotePath[MAX_PATH];              //TP��Ŀ¼
    char  szLocalPath[MAX_PATH];               //���ظ�Ŀ¼
    char  nActivedTP;                          //���õ���·0-primary;1-backup

    SSwitchBaseInfo         primary;
    SSwitchBaseInfo         backup;
    MAP <int, SS1240TPInfo> S1240TPMap;
};

struct SS1240FilePoolMsg
{
    char szFilePoolId[255];
    int  nFileCount;
    int  nFileFullCount;
    int  nAlarmLimit;
};

struct SS1240FileMsg
{
    char          szFileId[255];
    int           nFileStatus;
    int           nFileMode;
    char          szFileIdDup[255];
    int           nFileCounter;
    int           nFileLocalDumped;
    unsigned long uFileSize;
};

struct SLinkStatusMsg
{
    int  nStatus;
    int  nAlarmPara;
    char szLocalAddr[50];
    char szRemoteAddr[50];
};

#endif //__COLLECT_STRUCT_H__
