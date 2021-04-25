 /**************************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：GTP'计费接口
 *文件名    : cdr_producer.h
 *描述信息  ：本文件是类CCDRProducer的定义
 *版本说明  ：V200R002B02   
 *版权信息  ：版权所有（C）2004-2005 华为技术有限公司
 *作者      ：Z30971
 *创建时间  ：2004-06-19
 *修改记录  ：
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
    time_t m_tFileTime;               //文件创建时间
    char   m_szFileName[MAX_PATH];    //话单文件路径
    
    bool operator < (_TFilesNode &rhs)
	{
		if(m_tFileTime == rhs.m_tFileTime)
        {
            return strncmp(m_szFileName, rhs.m_szFileName, sizeof(m_szFileName)) < 0;
        }
		
        return m_tFileTime < rhs.m_tFileTime;
	}
} TFileNode;


//话单包结构,打包原则: 一个话单文件中的几张话单记录
typedef struct _TBillPacket
{
    UINT4  m_nPacketID;             //话单包ID
    UINT4  m_nTaskID;               //线程ID
    char   m_szFileName[MAX_PATH];  //话单文件名
    time_t m_tCreTime;              //创建时间
    UINT4  m_nCurReadPos;           //读指针位置
    UINT4  m_nLength;               //数据区长度
    UINT4  m_nCdrCount;             //话单记录数
    BYTE*  m_pData;                 //数据区

} TBillPacket;

//打包的记录数目,当一个文件不足5条记录时,也直接打包
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
    //获取参数配置信息
    BOOL GetConfig();

    //获取状态信息
    BOOL GetStatus();
    
    //生产CDR, 线程执行函数
    void ProduceCDR();

    //清空所有信息
    void Clear();

    //准备任务的当前文件
    BOOL PrepareTaskCurFiles(UINT4 nTaskID);
    
    //处理任务的当前文件
    BOOL ProcessTaskCurFiles(UINT4 nTaskID);

    //休眠
    BOOL Sleep(int nSeconds);

protected:
    //辅助函数, 准备目录和话单文件
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
    
    STRVECTOR    m_VecTaskPath;            //任务路径向量
    CCDRBuffer*  m_pCDRBuff;               //话单缓存区
    UINT4         m_nTaskCount;             //任务数
    BOOL         m_bIsExit;                //线程退出标识

    typedef MAP<UINT4, TStatusRecord*>  TASKSTATUSMAP;
    
    TASKSTATUSMAP       m_TaskStatusMap;   //读取状态信息MAP
    CSenderStatusFile*  m_pStatusFile;     //状态文件
    ACE_Event           m_ThdExitEvt;      //退出事件

    typedef MAP<UINT4, LIST<TFileNode> >  TASKFILESMAP;
    TASKFILESMAP        m_TaskFilesMap;
    
    UINT4               m_nBillPacketID;

    static BYTE*  g_pFileBuffer;           //读话单缓冲区
    static UINT4  g_nFileBuffSize;         //缓冲区长度

};
#endif //__CDR_PRODUCER_H__