/**************************************************************************
 *产品名    ：iGateway Bill V200
 *模块名    ：采集转换模块(collect_convertor)
 *文件名    : colloect_convertor.h
 *描述信息  ：本文件是类CCollectConvertorModule的实现
 *版本说明  ：V200R001i00M00B00D1130A
 *版权信息  ：版权所有（C）2001-2002 华为技术有限公司
 *作者      ：陈凉威
 *创建时间  ：2003-03-19
 *修改记录  ：
 **************************************************************************/

#ifndef __COLLECT_CONVERTOR_H__
#define __COLLECT_CONVERTOR_H__

#include "../include/frame.h"
#include "../include/toolbox.h"
#include "../include/collect_msg_def.h"

#include "collect_convert_obj_factory.h"
#include "collect_convert_obj.h"

class CCollectConvertorModule : public CWorkModule
{
public:
    // 构造函数
    CCollectConvertorModule(void);

    // 析构函数
    virtual ~CCollectConvertorModule();

    static char g_szColAppName[MAX_PATH];

protected:

    // 命令解析与分发函数
    virtual void ParseMsg (MSG_INTRA* pMsgParsed);

    // 初始化函数
    virtual int InitInstance(void);

    // 结束工作处理函数
    virtual void ExitInstance(void);

    // 获取本模块类型函数
    virtual MODULE_TYPE GetModuleType(void);

    virtual void OnTimer(const long nTimerID);


    //处理话单文件的转换
    int ProcessBillConvert();

    //发送转换结果到采集模块
    void SendConvertResult(int nCmdCode);

    int SaveStatus();

    int LoadStatus();

    VECTOR<CCollectConvertObj*>* m_pConvertObjList;

    enum CO_MD_STATUS
    {
        CM_FAILURE  = -1,
        CM_DEACTIVE = 0,
        CM_RESUME   = 1,
        CM_ACTIVE   = 2
    };

    CO_MD_STATUS  m_nStatus;

    enum FILE_CONVERT_STATUS
    {
        CS_NO_CONVERT      = 0,
        CS_ALREADY_CONVERT = 1
    };

    int m_nConvertStatus;

    int m_nConvertResult;

    int m_nFileNum;

    SSwitchFileInfo m_FileInfo;

    char  m_szStatusFile[MAX_PATH];
    FILE* m_pStatusFile;

};

#endif //__COLLECT_CONVERTOR_H__


