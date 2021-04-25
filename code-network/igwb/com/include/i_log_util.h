#ifndef _I_LOG_UTIL_H_
#define _I_LOG_UTIL_H_

#include "../include/base_type.h"

enum E_LOG_WHENCE
{
    EL_FIRST = 0,   //ȡ��һ����־��Ϣ
    EL_NEXT,        //ȡ��һ����־��Ϣ
    EL_FINAL        //ȡ���һ����־��Ϣ
};

class ILogUtil
{
public:
    ILogUtil(){};
    virtual ~ILogUtil(){};

	//����������DOM�ĵ������ļ�������������ָ���ļ�
	virtual int OpenFile(const char * const szFilePath) = 0;

    //�ر��ļ�
	virtual int CloseFile(void) = 0;

    //���ʣ�����־��
	virtual int GetRemainCount(void) = 0;

	//дһ����־��Ϣ
	virtual int AppendLog(UINT4       uSerialNo,
                          BYTE        LogType,
                          const char* szUserName,
                          UINT4       uIPAddr,
                          time_t      tLogTime,
                          BYTE        OperResult,
                          const char* szOperCmd,
                          const char* szCmdDesc) = 0;

    //��һ����־��Ϣ
	virtual int ReadLog(int     whence,
                        UINT4&  uSerialNo,
                        BYTE&   LogType,
                        char*   szUserName,
                        int     nUserNameSize,
                        UINT4&  uIPAddr,
                        time_t& tLogTime,
                        BYTE&   OperResult,
                        char*   szOperCmd,
                        int     nOperCmdSize,
                        char*   szCmdDesc,
                        int     nCmdDescSize) = 0;
};

#endif //_I_LOG_UTIL_H_

