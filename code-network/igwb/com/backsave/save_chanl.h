#ifndef __SAVECHANNEL_H__
#define __SAVECHANNEL_H__

#include "../include/save_out.h"
#include "patch_encap.h"

//�����״̬�ļ����ļ�ͷ����
struct  BS_STATUS_FILE_HEAD
{
    UINT4 uCheckSum;        //У���
    UINT4 uRotateCount;     //״̬�ļ���ת�Ĵ���
    UINT4 uStatusFileWritePos;  //״̬�ļ������дָ��
    UINT4 uLastPID; //����ͨ����ǰ����������һ�����ջ����İ���
    UINT4 uFileCsn; //����ΪuLastPid�����ջ��������ڵ����ջ����ļ����ļ����к�
    UINT4 uFileWritePos; //����ΪuLastPID�����ջ��������ڵ����ջ����ļ���дָ��
    UINT4 uBillCsn;      //���һ�������Ļ�������ģ���а�ͨ������Ļ�����ˮ��
    UINT4 uaReserved[2];        //�����ֽ�
};

//�����״̬��¼�Ķ���
struct BS_STATUS_FILE_ELEMENT
{
    UINT4 uFileCsn;     //���ջ����ļ����к�
    UINT4 uFirstPID;    //�û����ļ��еĵ�һ�������İ���
    UINT4 uLastPID;     //�û����ļ��е����һ�������İ��ţ�
						//��ֵĿǰ����
    UINT4 uBillCsn;     //ǰһ�������ļ�����󻰵���¼�Ļ�����ˮ��
    UINT4 uReserved;    //�����ֶ�
};

class CSaveChannel : public CSaveTransaction  
{
public:
    //���졢��������
    CSaveChannel(CPatchEncap* pPatchEncap);
    virtual ~CSaveChannel();

    //�̳еĽӿ�
	BOOL Timer1Sec();   //���¼�������	

    //�����ӵĽӿ�
	//��ȡͨ����
	const char* GetChlName();
    //��ʼ������
    virtual BOOL Init(UINT4 uAPID, UINT4 uChannelID, STRING& szChannelName, int nBillLen = 0);
    //��������ָ�ʱ��Ҫ��״̬����
    void GetResumeParam(UINT4& uLastPID, UINT4& uLastBillCsn);
    //����״̬�ļ��õ����һ���������İ���
    UINT4 GetLastPID();
    //����״̬�ļ��õ��������к�
    UINT4 GetBillCsn();
	//���Ʒ������Ƿ��Ѿ���ʱȡ�߻����ļ���ÿ���ӱ�����һ��
	void CheckFetchingFile();
	
	//by	����	2004-6-1	���ⵥSWPD05564
	BOOL CloseCurFile();
	//the end	���ⵥSWPD05564

protected:
    //�õ���һ�������ļ����Ŀ¼�����Ŀ¼�����ڣ��򴴽�֮
    void GetNextFilePath();
    //ɾ�����һ����ʱ�ļ����ڽ���״̬�ļ�У��ʱ����
    BOOL DeleteTempFile();
    //�����յ��Ļ�����������״̬�ļ������ݼ�����Ա����
    void RefreshStatus(BYTE* pPacketInfo);
    BOOL DoWhenFileClose(); //�رջ����ļ��ĺ�������
    //�ڱ��滰���������е�д״̬�ļ�����
    BOOL WriteStatusFile(CInfoFile* pInfoFile, BOOL bFlush);
    //�ڱ��滰���������е�д�����ļ�����
    int WriteBillFile(BYTE* pPacketInfo, BOOL bFlush); 

	//����Զ2002.05.18�޸ģ����ⵥ��D13767
    //��������У��ʱ����״̬�ļ���һ�µ����
    BOOL DoWithMainDiffBak(BYTE* pMainHead, BYTE* pBakHead, 
						   BYTE* pMainLastUnit, BYTE* pBakLastUnit);
	//����Զ2002.05.18�޸Ľ���

    //����״̬�ļ������Ϣ����صĳ�Ա�������г�ʼ��
    BOOL InitByStatusFile();    
    //�����µĻ����ļ��Լ���ص�״̬�ļ�ˢ�²���
    BOOL CreateNewFile();
	//�����յ��İ���Ϣ�ж��Ƿ�Ϊ�հ�
	BOOL IsEmptyPacket(BYTE* pInfo);

	virtual void DelMoreFiles();

    //Added by ZhengYuqun SWPD03912 2004-03-03
    //���ҵڶ��ݻ���Ŀ¼�µĵ�һ���ļ�
    STRING FindFirstFileInSecondPath();

    //��ʽ��ǰ׺�ַ���
    static STRING FormatCfgStr(const char* szFormat, 
                               const tm&   tmTime, 
                               const UINT4 uChannelID,
                               const INT4  nRecordCnt = 0);

    virtual STRING GetBillFileInfo(const char* szFilePath);
    
    virtual BOOL IsRepeatLog(const char* szLogMsg, FILE* pFile);
    //���������־
    BOOL SaveCheckLog(const char* szLogMsg);
	

protected:
    UINT4  m_uAPID;               //������ID��
    UINT4  m_uChannelID;          //ͨ����	
    STRING m_szChannelName;       //ͨ����
    STRING m_szDatePath;          //ͨ���Ĵ���Ŀ¼
    STRING m_szFirstPath;         //��һ�����ڲ�ѯ����Ļ����Ĵ��·��
    STRING m_szFormalFileName;    //��ǰ����ʽ�ļ���
    char   m_szFormalPrefix[512]; //��ʽ�ļ���ǰ׺
    char   m_szFormalMidfix[512]; //��ʽ�ļ�����׺
    char   m_szFormalPostfix[512];//��ʽ�ļ��ĺ�׺
    BOOL   m_bSecondCopy;         //�Ƿ�ʹ�õڶ�����Ŀ¼�ṹ�������ջ����ļ�
    STRING m_szSecondPath;        //�ڶ��������Ʒ����ĵĻ����ļ��Ĵ��·��
    STRING m_szSecondChannelPath; //�ڶ��������Ʒ����ĵ�ͨ��·��
    
    //�����ļ����ɵ�ʱ�����ƣ���ʱ�ļ������������ʱ���ǿ��ת������ʽ�ļ�
    UINT4  m_uFileTime;          
    UINT4  m_uFileExistTime;     //��ǰ�ļ����ڵ�ʱ�䣬�����жϵ�ǰ�ļ��Ƿ�ʱ
	//���±������ڹ����Ʒ����ĳ���δȡ�������Ĺ��ϸ澯
	BOOL   m_bFetchFileAlarmSend;//�Ƿ��͹����Ʒ����ĳ���δȡ�������Ĺ��ϸ澯
	STRING m_szCheckedFilePath;//�����ӵ��ļ�������ȷ����Ʒ����ĵ������Ƿ�����
	int    m_nCheckTimeCounter;//��鱻�����ļ���ʱ�������
	UINT4    m_nFetchFileTimeOut;//�Ʒ�����ȡ�����ļ��ĳ�ʱʱ�䣬�Է���Ϊ��λ
	int    m_nDelTimeCounter;  //����ļ��Ƿ���ڵ�ʱ����

    int    m_nBillLen;               //���ջ�������

    BOOL   m_bCheckLog;              //�Ƿ�����������־����
    STRING m_szCheckedLogPath;      //������־·��
    char   m_szCheckLogPrefix[512];  //������־�ļ���ǰ׺
    char   m_szCheckLogPostfix[512]; //������־�ļ��ĺ�׺

    CPatchEncap* m_pPatchEncap;//�����������
};

#endif 
