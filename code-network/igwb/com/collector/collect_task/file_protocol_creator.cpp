#include "../../include/base_type.h"
#include "collect_include.h"
#include "collect_define.h"
#include "collect_struct.h"
#include "collect_base.h"
#include "file_protocol.h"
#include "file_protocol_creator.h"

//added by chenliangwei
#include "vertel_ftam.h"
#include "ftp_protocol.h"
#include "file_scan_protocol.h"
//added end

#include "../include/adapter_include.h"
#include "../../include/toolbox.h"

//以下是文件传输协议类的实现
//构造函数
CFileProtocolFactory::CFileProtocolFactory( )
{
    //NULL
}

//析构函数
CFileProtocolFactory::~CFileProtocolFactory( )
{
    //NULL
}

//added by chenliangwei
CFileProtocol*  CFileProtocolFactory::CreateObject(const char* szId,
                                                   int nTransSpeed,
                                                   const char* szDevName)
{
    CFileProtocol* pFileProtocol = NULL;

    if (StringiCmp(szId, PL_TYPE_VERTEL))
    {
        pFileProtocol = new CVertelFtam(nTransSpeed, szDevName);
    }
    else if (StringiCmp(szId, PL_TYPE_FTP))
    {
        pFileProtocol = new CFtpProtocol();
    }
    else if (StringiCmp(szId, PL_TYPE_FILESCAN))
    {
        pFileProtocol = new CFileScanProtocol();
    }

    return pFileProtocol;
}
//added end

