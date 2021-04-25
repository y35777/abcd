#ifndef __BK_LINK_MGR_H__
#define __BK_LINK_MGR_H__

#include "link_mgr.h"

//以下是文件传输基类的定义
class  CBKLinkMgr : public CLinkMgr
{

public:

    CBKLinkMgr(CLinkGroup* pLinkGroup)
              :CLinkMgr(pLinkGroup)
    {
        //
    }

    virtual  ~CBKLinkMgr()
    {
        //
    }

    //初始化本地服务, 是虚函数, 由派生类具体实现该方法
    virtual int  Init(void *pPara = NULL){return ERR_SUCCESS;}

    //实现拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现
    virtual int  GetFile(const SGetFileReq &pReq);

    //获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
    virtual int  GetFileList(const char* szSrcDir, SListOut &pOut);

    //删除文件服务端的指定文件
    virtual int  DelFile(const char* szFileName);

    //取文件服务端的指定文件的长度
    virtual int  GetFileLen(const char* szFileName,
                            unsigned long& uFileLen);

    //重命名指定文件
    virtual int  RenameFile(const char *szOld, const char *szNew);

    //连接文件服务端
    virtual int  Connect();

    //断开连接
    virtual int  Disconnect();

    //设置文件传输模式
    virtual int  SetMode(int nMode);

protected:

    //int m_nActiveLink;
};

#endif //__BK_LINK_MGR_H__
