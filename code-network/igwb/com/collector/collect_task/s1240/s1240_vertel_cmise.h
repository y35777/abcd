#ifndef __VERTEL_S1240_CMISE_H__
#define __VERTEL_S1240_CMISE_H__

#include "s1240_cmise.h"
#include "../../include/i_cmise_fun.h"

//以下是文件传输基类的定义
class CS1240VertelCmise : public CS1240Cmise
{
public:

    CS1240VertelCmise();
    virtual  ~CS1240VertelCmise();

    //初始化本地服务, 是虚函数, 由派生类具体实现该方法
    virtual int  Init();

    virtual int  GetFilePool(SS1240FilePoolMsg& pool_msg);

    virtual int  SetFilePool(int nAlarmLimit);

    virtual int  GetFile(const char* const szFileId,
                         SS1240FileMsg&    file_msg);

    virtual int  SetFile(const char* const szFileId,
                         int               nFilestatus);

    //获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
    virtual int  GetFileList(int nFileStatus,
                             VECTOR<SS1240FileMsg> &Out);
protected:
};

#endif //__VERTEL_S1240_CMISE_H__
