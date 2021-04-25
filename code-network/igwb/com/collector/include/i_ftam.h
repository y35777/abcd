#ifndef __I_FTAM_H__
#define __I_FTAM_H__

#ifdef _PLATFORM_WIN32
#include <windows.h>
//#define MAX_PATH 256
#else
#define  MAX_PATH    PATH_MAX
#endif //_UNIX
#include <vector>

#ifdef _PLATFORM_WIN32
    #define  ZYX_Export_Flag    __declspec (dllexport)
#else  //UNIX
    #define  ZYX_Export_Flag     
#endif

class IFtam;

#ifdef __cplusplus
extern "C" {
#endif
	ZYX_Export_Flag	IFtam *MakeFtam();
#ifdef __cplusplus
}
#endif

class  IFtam
{

public:

    IFtam(){};
    virtual  ~IFtam(){};
	//返回错误信息
	virtual const char* GetLastError()=0;
    //初始化本地服务, 是虚函数, 由派生类具体实现该方法
    virtual  int  InitProtocal() = 0;

    //设置远程主机信息
    virtual  int  SetRemoteHostMsg(const char* const szHost,
                                   const char* const szUser,
                                   const char* const szPwd) = 0;

    //设置本地主机信息
    virtual  int  SetLocalHostMsg(const char* const szHost,
                                  const char* const szUser,
                                  const char* const szPwd) = 0;

    //测试连接文件服务端
    virtual  int  TryConnect() = 0;

    //连接文件服务端
    virtual  int  Connect() = 0;

    //断开连接
    virtual  int  Disconnect() = 0;

    //实现拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现
    virtual  int  CopyFile(const char* const szSrcPath, 
                           const char* const szSrcName, 
                           const char* const szDstPath, 
                           const char* const szDstName,
                           int               nTimeOut) = 0;

    //实现移动文件服务端的单个文件到本地, 虚函数, 由派生类具体实现
    virtual  int  MoveFile(const char* const szSrcPath, 
                           const char* const szSrcName, 
                           const char* const szDstPath, 
                           const char* const szDstName,
                           int               nTimeOut) = 0;

    //获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
    virtual  int  GetFileList(const  char* szSrcDir) = 0;

    //删除文件服务端的指定文件
    virtual  int  RemoveFile(const char* szFileName) = 0;

    //重命名指定文件
    virtual  int  RenameFile(const char *szOld, 
		                     const char *szNew) = 0;

	virtual  int  GetItemByIndex(int            nIndex,
                                 const char*&   szFileName, 
				                 int&           nAttr,
                                 unsigned long& nFileSize) = 0;

	virtual  int  GetFileAttr(const char* const szFileName, 
				              time_t&           tCreateTime,
				              time_t&           tModTime,
				              time_t&           tReadTime,
				              time_t&           tAttrModTime,
                              unsigned long&    nFileSize,
                              unsigned long&    nFileFutureSize) = 0;
};


#endif //__I_FTAM_H__
