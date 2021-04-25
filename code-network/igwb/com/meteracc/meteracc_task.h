#ifndef __METER_ACCUMULATION_TASK__
#define __METER_ACCUMULATION_TASK__

#include "../include/toolbox.h"
#include "ace/Task.h"
#include "meter_index.h"

class CFormat;
class CMeterAccIndex;
class CMeterFile;

struct SSubmitTime
{
    BYTE    nSubmitDay;         //�ύ����
    BYTE    nSubmitHour;        //�ύ��ʱ
};

typedef MAP<int, SSubmitTime>       SUBMIT_TIMES_MAP;   //�û��ƴα���ൽ�ύʱ���ӳ��
typedef MAP<int, CMeterFile*>       METER_FILE_MAP;     //�û��ƴα���ൽ�ļ������ӳ��


class CMeterAccTask : public ACE_Task_Base
{
public:
    CMeterAccTask();
    ~CMeterAccTask();

public:
    int  open(void* args = 0);
    void StopTask(void);

protected:
    int Init(const char* pszRootDir);
    int ProcessThread(void);
    int svc();
    
private:
    int Sleep(const int nSeconds);
    int ProcessAFile(const char* pszSrcFile);
    int SubmitAFile(const int nIndex);
    int SubmitTimeCheck(void);
    int DelOutdatedMeterFile(void);

private:
    char    m_szMeterAccSrcDir[MAX_PATH];       //�ƴα��ۼ����뻰����ŵ�Ŀ¼
    char    m_szMeterAccLocalDir[MAX_PATH];     //�ƴα��ۼӱ��ش�ŵ�Ŀ¼
    char    m_szMeterAccBakDir[MAX_PATH];       //�ƴα��ۼӱ��ر��ݵ�Ŀ¼
    char    m_szMeterAccBSDir[MAX_PATH];        //�ƴα��ۼ��ύ���Ʒ����ĵ�Ŀ¼
    char    m_szIndexFile[MAX_PATH];            //�ƴα������ļ���
    int     m_nMeterTables;                     //ÿ�û��ƴα�����
    int     m_nSubmitTimeInd;                   //�û��ƴα��ύʱ���ʶ��
    char    m_szPrefix[32];                     //�ύ���Ʒ������û��ƴα����ļ���ǰ׺
    char    m_szSuffix[32];                     //�ύ���Ʒ������û��ƴα����ļ�����׺    
    
    METER_FILE_MAP          m_mapAccFile;       //�û��ƴα����ļ�����m_nSubmitTimeInd����
    SUBMIT_TIMES_MAP        m_mapSubmitTime;    //�û��ƴα��ύʱ��ӳ��
    CMeterAccIndex*         m_pIndexFile;       //�û��ƴα��¼�ڱ���ļ��е�λ������
    
    CFormat*    m_pSrcFmt;                      //�ƴα�ת����ʽ��Դ��ʽ
    CFormat*    m_pDestFmt;                     //�ƴα�ת����ʽ��Ŀ���ʽ

    unsigned char*          m_pReadEntry;
    int                     m_bForTTAT;
    int                     m_bDebug;           //�Ƿ���԰汾
    int                     m_nBackupDays;      //���ر�������
	BOOL                    m_bRunAcc;          //�ƴα��ۼӴ������߳�ѭ������

private:
    ACE_Event               m_ThreadExitEvt;    //�˳��߳��¼�
};


#endif //__METER_ACCUMULATION_TASK__
