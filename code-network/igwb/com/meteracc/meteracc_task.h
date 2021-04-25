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
    BYTE    nSubmitDay;         //提交的日
    BYTE    nSubmitHour;        //提交的时
};

typedef MAP<int, SSubmitTime>       SUBMIT_TIMES_MAP;   //用户计次表分类到提交时间的映射
typedef MAP<int, CMeterFile*>       METER_FILE_MAP;     //用户计次表分类到文件句柄的映射


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
    char    m_szMeterAccSrcDir[MAX_PATH];       //计次表累加输入话单存放的目录
    char    m_szMeterAccLocalDir[MAX_PATH];     //计次表累加本地存放的目录
    char    m_szMeterAccBakDir[MAX_PATH];       //计次表累加本地备份的目录
    char    m_szMeterAccBSDir[MAX_PATH];        //计次表累加提交给计费中心的目录
    char    m_szIndexFile[MAX_PATH];            //计次表索引文件名
    int     m_nMeterTables;                     //每用户计次表张数
    int     m_nSubmitTimeInd;                   //用户计次表提交时间标识数
    char    m_szPrefix[32];                     //提交给计费中心用户计次表表底文件名前缀
    char    m_szSuffix[32];                     //提交给计费中心用户计次表表底文件名后缀    
    
    METER_FILE_MAP          m_mapAccFile;       //用户计次表表底文件，由m_nSubmitTimeInd索引
    SUBMIT_TIMES_MAP        m_mapSubmitTime;    //用户计次表提交时间映射
    CMeterAccIndex*         m_pIndexFile;       //用户计次表记录在表底文件中的位置索引
    
    CFormat*    m_pSrcFmt;                      //计次表转换格式的源格式
    CFormat*    m_pDestFmt;                     //计次表转换格式的目标格式

    unsigned char*          m_pReadEntry;
    int                     m_bForTTAT;
    int                     m_bDebug;           //是否调试版本
    int                     m_nBackupDays;      //本地保存天数
	BOOL                    m_bRunAcc;          //计次表累加处理主线程循环工作

private:
    ACE_Event               m_ThreadExitEvt;    //退出线程事件
};


#endif //__METER_ACCUMULATION_TASK__
