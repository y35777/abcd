 /**************************************************************************
 *��Ʒ��    ��iGateway Bill V200        
 *ģ����    ��GTP'�Ʒѽӿ�
 *�ļ���    : cdr_producer.h
 *������Ϣ  �����ļ�����CCDRProducer�Ķ���
 *�汾˵��  ��V200R002B02   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2004-2005 ��Ϊ�������޹�˾
 *����      ��Z30971
 *����ʱ��  ��2004-06-19
 *�޸ļ�¼  ��
 **************************************************************************/

#ifndef __CDR_PRODUCER_H__
#define __CDR_PRODUCER_H__

#include "ace/Task.h"
#include "sender_status.h"
#include "../include/base_type.h"

//////////////////////////////////////////////////////////////////////////
#ifdef _PLATFORM_WIN32
#define S_ISDIR(mode)   (((mode) & S_IFMT) == S_IFDIR)
#endif

typedef struct _TFilesNode
{
    time_t m_tFileTime;               //�ļ�����ʱ��
    char   m_szFileName[MAX_PATH];    //�����ļ�·��
    
    bool operator < (_TFilesNode &rhs)
	{
		if(m_tFileTime == rhs.m_tFileTime)
        {
            return strncmp(m_szFileName, rhs.m_szFileName, sizeof(m_szFileName)) < 0;
        }
		
        return m_tFileTime < rhs.m_tFileTime;
	}
} TFileNode;


//�������ṹ,���ԭ��: һ�������ļ��еļ��Ż�����¼
typedef struct _TBillPacket
{
    UINT4  m_nPacketID;             //������ID
    UINT4  m_nTaskID;               //�߳�ID
    char   m_szFileName[MAX_PATH];  //�����ļ���
    time_t m_tCreTime;              //����ʱ��
    UINT4  m_nCurReadPos;           //��ָ��λ��
    UINT4  m_nLength;               //����������
    UINT4  m_nCdrCount;             //������¼��
    BYTE*  m_pData;                 //������

} TBillPacket;

//����ļ�¼��Ŀ,��һ���ļ�����5����¼ʱ,Ҳֱ�Ӵ��
#define CDRCOUNTPERPACKET    5

class CCDRBuffer;

class CCDRProducer : public ACE_Task_Base
{
#define MAX_INFO_LEN  256

public:
    CCDRProducer(CCDRBuffer* pBuffer, CSenderStatusFile* pStatusFile);
    virtual ~CCDRProducer();

    virtual int open(void* args = 0);
    virtual int svc(void);
    virtual void stop();

    inline BOOL IsExit()
    {
        return m_bIsExit;
    };

protected:
    //��ȡ����������Ϣ
    BOOL GetConfig();

    //��ȡ״̬��Ϣ
    BOOL GetStatus();
    
    //����CDR, �߳�ִ�к���
    void ProduceCDR();

    //���������Ϣ
    void Clear();

    //׼������ĵ�ǰ�ļ�
    BOOL PrepareTaskCurFiles(UINT4 nTaskID);
    
    //��������ĵ�ǰ�ļ�
    BOOL ProcessTaskCurFiles(UINT4 nTaskID);

    //����
    BOOL Sleep(int nSeconds);

protected:
    //��������, ׼��Ŀ¼�ͻ����ļ�
    BOOL PrepareSubDirs(const char* pszDir, LIST<STRING> &SubDirList);
    
    void PrepareSubDirs_Impl(const char* pszDir, LIST<STRING>& SubDirList);
    
    int  PrepareFiles(const char* szPath, time_t tFileTime,
                       LIST<TFileNode>& FileList);
    void GetCreateTime(const char* szPath, time_t st_cTime);

protected:
    class Compare
    {    
    public:
        bool operator ()(const STRING &lhs, const STRING &rhs)
        {        
            return (strcmp(lhs.c_str(),rhs.c_str()) < 0);
        }
    };

protected:
    typedef VECTOR<STRING> STRVECTOR;
    
    STRVECTOR    m_VecTaskPath;            //����·������
    CCDRBuffer*  m_pCDRBuff;               //����������
    UINT4         m_nTaskCount;             //������
    BOOL         m_bIsExit;                //�߳��˳���ʶ

    typedef MAP<UINT4, TStatusRecord*>  TASKSTATUSMAP;
    
    TASKSTATUSMAP       m_TaskStatusMap;   //��ȡ״̬��ϢMAP
    CSenderStatusFile*  m_pStatusFile;     //״̬�ļ�
    ACE_Event           m_ThdExitEvt;      //�˳��¼�

    typedef MAP<UINT4, LIST<TFileNode> >  TASKFILESMAP;
    TASKFILESMAP        m_TaskFilesMap;
    
    UINT4               m_nBillPacketID;

    static BYTE*  g_pFileBuffer;           //������������
    static UINT4  g_nFileBuffSize;         //����������

};
#endif //__CDR_PRODUCER_H__