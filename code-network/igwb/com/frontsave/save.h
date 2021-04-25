#ifndef __SAVETRANSACTION_H__
#define __SAVETRANSACTION_H__

#include "../include/precom.h"
#include "info_file.h"
#include <algorithm>
#include <deque>

#define CHAR_DIR            '/' 

typedef struct _FilesListNode
{
    time_t tFileTime;               //�ļ��޸�ʱ��
    char szFileName[MAX_PATH];      //�����ļ�����(��·����Ϣ)
} SFilesListNode, *PFilesListNode;
typedef std::deque<SFilesListNode *> LISTFILELISTNODE;

class CSaveTransaction
{
public:
/* Begin: WLAN��ֲ��Ҫ�ϸ�����ļ������Ĭ��Ϊ0����ʾ���谴ʱ�����	Adder: Gan Zhiliang   Date: 2003-4-23*/
	UINT4 m_uResFileProductTime;

/* End  : Adder: Gan Zhiliang	   Date: 2003-4-23*/
    //���졢��������
    CSaveTransaction();
    virtual ~CSaveTransaction();

    //�������úͻ�ȡ����
    UINT4 GetCurCsn(BOOL bDisplay = FALSE);

    //����ӿ�
    //���¼�������
	virtual BOOL Timer1Sec();	
    //ʵ�ֻ���������Ĵ����߼�
	int WritePacket(BYTE* pPacketInfo, UINT4& uWriteTime);	
    //�رյ�ǰ���ڲ����Ļ����ļ�
    virtual BOOL CloseCurFile();

	//added by maimaoshi at 2002-09-23
	virtual void DelMoreFiles() { }         //ɾ�������ļ�
	//end

	void GetCurrentFileName(STRING& sFileName);

protected:
    //�����ʼ��ʱ�Ĺ���
    BOOL Init();
    //������һ���»����ļ����ļ����к�
    void CalcNextCsn();
    //�õ���һ�������ļ����ļ���
    void GetNextFileName();
    //�õ���һ�������ļ����Ŀ¼�����Ŀ¼�����ڣ��򴴽�֮
    virtual void GetNextFilePath() = 0;
    //ɾ�����һ����ʱ�ļ����ڽ���״̬�ļ�У��ʱ����
    virtual BOOL DeleteTempFile() = 0;
    //�����յ��Ļ�����������״̬�ļ��������Լ�����Ա����
    virtual void RefreshStatus(BYTE* pPacketInfo) = 0;
    //�����µĻ����ļ��Լ���ص�״̬�ļ�ˢ�²���,
    //�ڻ�����ʵ�ִ������ļ��Ĳ������������и���ʵ��״̬�ļ���ˢ��
    virtual BOOL CreateNewFile();
    //�رջ����ļ��ĺ�������
    virtual BOOL DoWhenFileClose() = 0;
    //�ڱ��滰���������е�д״̬�ļ�����
    virtual BOOL WriteStatusFile(CInfoFile* pInfoFile, BOOL bFlush) = 0;
    //��һ������д�뵽��ǰ�Ļ����ļ���
    BOOL WritePacketData(BYTE* pPacket, UINT4 uPacketLen);
    //�ڱ��滰���������е�д�����ļ�����
    virtual int WriteBillFile(BYTE* pPacketInfo, BOOL bFlush) = 0;

	//����Զ2002.05.18�޸ģ����ⵥ��D13767
    //��������У��ʱ����״̬�ļ���һ�µ����
    virtual BOOL DoWithMainDiffBak(BYTE* pMainHead, BYTE* pBakHead, 
								   BYTE* pMainLastUnit, 
								   BYTE* pBakLastUnit) = 0;
	//����Զ2002.05.18�޸Ľ���

    //����״̬�ļ������Ϣ����صĳ�Ա�������г�ʼ��
    virtual BOOL InitByStatusFile() = 0;
    //������״̬�ļ�����У�鹤������Ӧ������������
    //������״̬�������ļ�����ģ������ʱ����
    BOOL StartCheck();
    //�������кŵõ��ļ����кŵ��ַ���
    STRING GetFileCsnStr(UINT4 uCsn);
	//�����յ��İ���Ϣ�ж��Ƿ�Ϊ�հ�
	virtual BOOL IsEmptyPacket(BYTE* pInfo) = 0;

	//added by maimaoshi at 2002-09-23
	//ɾ��ָ��Ŀ¼�¹��ڻ����ļ�������
    int DelOneDirBillFiles(char* szPath);
	//����·��
	STRING MakePath(const STRING &path, const char *filename);
	//׼���ļ��б�
    virtual int PrepareFiles(const char* szPath, time_t tFileTime);
	//end 

protected:
    //��Ա����
    CInfoFile m_InfoMain;         //��״̬�ļ�
    CInfoFile m_InfoBak;          //��״̬�ļ�
    BYTE*     m_pHead;            //��ǰ��״̬�ļ�ͷ��
                                  //�ɳ�ʼ���������������������ڴ��������ͷ�
    UINT4     m_uHeadLen;         //״̬�ļ�ͷ�ĳ���
    BYTE*     m_pLastInfoUnit;    //״̬�ļ������һ��״̬��¼��
                                  //�ɳ�ʼ���������������������ڴ��������ͷ�
    UINT4     m_uUnitLen;         //״̬��¼�ĳ���
    FILE*     m_pBillFile;        //��ǰ���ڴ���Ļ����ļ�����ָ��
    STRING    m_szFilePath;       //��ǰ���ļ��Ĵ��·��
    STRING    m_szFileName;       //��ǰ���ļ���
    UINT4     m_uFileLen;         //�����ļ�����󳤶ȣ���λΪ�ֽ�
    char      m_szPrefix[512];    //�����ļ�����ǰ׺
    int       m_nCsnLen;          //�����ļ��������кų���
    UINT4     m_uFileCsnStart;    //�����ļ���ʼ���к�
    UINT4     m_uFileCsnEnd;      //�����ļ�������к�
    UINT4     m_uFileCsnStep;     //�����ļ����кŵ�������
    char      m_szPostfix[512];   //�����ļ����ĺ�׺
    UINT4     m_uCurCsn;          //��ǰ�Ļ����ļ����к�
    tm        m_tmToday;          //��ǰ�����ڣ�ֻ��������Ϣ���ã�
                                  //���ڽ����ж��Ƿ����
    tm        m_tmCreateTime;     //��ǰ�����ļ�����ʱ��
    tm        m_tmLastFileTime;   //��һ���ļ��Ĵ���ʱ��
    //����Flush�����������ż������������m_uMaxFlushSpan�������������
    //�����һ��״̬�ļ���д�����Լ�״̬�ļ��ͻ����ļ���Flush����
    UINT4 m_uMaxFlushSpan;
    UINT4 m_uLastFlushPID;        //�ϴν���Flush����ʱ�Ļ�������
    UINT4 m_uPID;                 //��ǰ����İ���
	char  m_szModuName[200];      //����дTRACE��Ϣ��ʱ��ģ����

	//added by maimaoshi at 2002-09-23 
	char m_szModuNameEx[200];      //����дɾ����ʱ�ļ�Log��Ϣʱ��ģ����
	char m_szSrcBasePath[MAX_PATH]; //���ݵ�ԭʼ��·��
	UINT4 m_uBillSavedDays;         //�����ļ���������
	LISTFILELISTNODE m_FileList;    //��Ҫ���ݵĻ����ļ��б�

	STRING m_sCurFileFullPath;		//��ǰ��д���ļ���ȫ·��
	
	BOOL   m_bAlwaysGenFile;      //�Ƿ񲻼�ϵ������ļ�

	class Compare
    {
    public:
        friend class CSaveTransaction;
        bool operator ()(SFilesListNode* x, SFilesListNode *y); 
    };
	//end
};

#endif
