#ifndef _C_M2000_LOG_XML_H_
#define _C_M2000_LOG_XML_H_

#include "../include/i_log_util.h"
#include "xml_file_opr.h"

class CM2000LogXml : public ILogUtil
{
public:
    CM2000LogXml();
    virtual ~CM2000LogXml();

	//����������DOM�ĵ������ļ�������������ָ���ļ�
	virtual int OpenFile(const char * const szFilePath);

    //�ر��ļ�
	virtual int CloseFile(void);

    //���ʣ�����־��
    virtual int GetRemainCount(void);

	//дһ����־��Ϣ
	virtual int AppendLog(UINT4       uSerialNo,
                          BYTE        LogType,
                          const char* szUserName,
                          UINT4       uIPAddr,
                          time_t      tLogTime,
                          BYTE        OperResult,
                          const char* szOperCmd,
                          const char* szCmdDesc);

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
                        int     nCmdDescSize);
protected:
    //��һ����־��Ϣ
	virtual int ReadOneLog(int     nIndex,
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
                           int     nCmdDescSize);

    CXmlFileOpr m_xmlFileLog; //������־�ļ�(.xml)�����Ķ���
    int         m_nReadIndex;
    STRING      m_strStartTime;
    STRING      m_strEndTime;
};

#endif //_C_M2000_LOG_XML_H_

