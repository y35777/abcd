/**************************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：GTP'计费接口
 *文件名    : sender_status.h
 *描述信息  ：本文件是类CSenderStatus的定义
 *版本说明  ：V200R002B02   
 *版权信息  ：版权所有（C）2004-2005 华为技术有限公司
 *作者      ：Z30971
 *创建时间  ：2004-06-19
 *修改记录  ：
 **************************************************************************/
#ifndef __SENDER_STATUS_H__
#define __SENDER_STATUS_H__

#include "../include/base_type.h"

//////////////////////////////////////////////////////////////////////////

//状态文件记录
typedef struct _TStatusRecord
{
    UINT4   m_nTaskID;                //任务ID
    char    m_szFileName[MAX_PATH];   //当前读取的文件
    time_t  m_tCreTime;               //文件创建时间
    UINT4   m_nCurPos;                //文件指针的位置

} TStatusRecord;

#define  STATUSINFOLENGTH   sizeof(TStatusRecord)

class CSenderStatusFile
{
public:
    //构造/析构函数
    CSenderStatusFile();
    virtual ~CSenderStatusFile();

public:
    //打开/关闭 状态文件
    BOOL Open(const char* szFileName);
    void Close();
    
    //读/写 数据
    BOOL Write(void* pData, UINT4 uLen = STATUSINFOLENGTH);
    BOOL Read(void* pData, UINT4 uLen = STATUSINFOLENGTH);

    //获得状态信息
    BOOL GetRecord(UINT4 nTaskID, TStatusRecord* pRecInfo);

public:
    INT4  GetOffset(UINT4 nTaskID);
    BOOL  SetOffset(UINT4 nOffset, INT POS = SEEK_SET);
    
protected:
    char          m_szFilePath[MAX_PATH];     //状态文件路径
    FILE*         m_pFile;                    //文件指针

};
#endif  //__SENDER_STATUS_H__
