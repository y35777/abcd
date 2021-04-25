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
	//���ش�����Ϣ
	virtual const char* GetLastError()=0;
    //��ʼ�����ط���, ���麯��, �����������ʵ�ָ÷���
    virtual  int  InitProtocal() = 0;

    //����Զ��������Ϣ
    virtual  int  SetRemoteHostMsg(const char* const szHost,
                                   const char* const szUser,
                                   const char* const szPwd) = 0;

    //���ñ���������Ϣ
    virtual  int  SetLocalHostMsg(const char* const szHost,
                                  const char* const szUser,
                                  const char* const szPwd) = 0;

    //���������ļ������
    virtual  int  TryConnect() = 0;

    //�����ļ������
    virtual  int  Connect() = 0;

    //�Ͽ�����
    virtual  int  Disconnect() = 0;

    //ʵ�ֿ����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��
    virtual  int  CopyFile(const char* const szSrcPath, 
                           const char* const szSrcName, 
                           const char* const szDstPath, 
                           const char* const szDstName,
                           int               nTimeOut) = 0;

    //ʵ���ƶ��ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��
    virtual  int  MoveFile(const char* const szSrcPath, 
                           const char* const szSrcName, 
                           const char* const szDstPath, 
                           const char* const szDstName,
                           int               nTimeOut) = 0;

    //����ļ���������ǰĿ¼�µ��ļ��б�, �麯��, �����������ʵ�ָ���
    virtual  int  GetFileList(const  char* szSrcDir) = 0;

    //ɾ���ļ�����˵�ָ���ļ�
    virtual  int  RemoveFile(const char* szFileName) = 0;

    //������ָ���ļ�
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
