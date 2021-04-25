 #ifndef __FILE_SCAN_PROTOCOL_H__
#define __FILE_SCAN_PROTOCOL_H__

#include "file_protocol.h"

//以下是文件传输基类的定义
class  CFileScanProtocol : public CFileProtocol
{

public:

    CFileScanProtocol();
    virtual  ~CFileScanProtocol();

    //实现拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现
    virtual  int  GetFile(const SGetFileReq &pReq);

    //获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
    virtual  int  GetFileList( const  char* szSrcDir, SListOut &pOut );

    //删除文件服务端的指定文件
    virtual  int  DelFile( const char* szFileName );

    //重命名指定文件
    virtual  int  RenameFile( const char *szOld, const char *szNew );

protected:
};

#endif //__FILE_SCAN_PROTOCOL_H__
