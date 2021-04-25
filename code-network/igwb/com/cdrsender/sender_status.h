/**************************************************************************
 *��Ʒ��    ��iGateway Bill V200        
 *ģ����    ��GTP'�Ʒѽӿ�
 *�ļ���    : sender_status.h
 *������Ϣ  �����ļ�����CSenderStatus�Ķ���
 *�汾˵��  ��V200R002B02   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2004-2005 ��Ϊ�������޹�˾
 *����      ��Z30971
 *����ʱ��  ��2004-06-19
 *�޸ļ�¼  ��
 **************************************************************************/
#ifndef __SENDER_STATUS_H__
#define __SENDER_STATUS_H__

#include "../include/base_type.h"

//////////////////////////////////////////////////////////////////////////

//״̬�ļ���¼
typedef struct _TStatusRecord
{
    UINT4   m_nTaskID;                //����ID
    char    m_szFileName[MAX_PATH];   //��ǰ��ȡ���ļ�
    time_t  m_tCreTime;               //�ļ�����ʱ��
    UINT4   m_nCurPos;                //�ļ�ָ���λ��

} TStatusRecord;

#define  STATUSINFOLENGTH   sizeof(TStatusRecord)

class CSenderStatusFile
{
public:
    //����/��������
    CSenderStatusFile();
    virtual ~CSenderStatusFile();

public:
    //��/�ر� ״̬�ļ�
    BOOL Open(const char* szFileName);
    void Close();
    
    //��/д ����
    BOOL Write(void* pData, UINT4 uLen = STATUSINFOLENGTH);
    BOOL Read(void* pData, UINT4 uLen = STATUSINFOLENGTH);

    //���״̬��Ϣ
    BOOL GetRecord(UINT4 nTaskID, TStatusRecord* pRecInfo);

public:
    INT4  GetOffset(UINT4 nTaskID);
    BOOL  SetOffset(UINT4 nOffset, INT POS = SEEK_SET);
    
protected:
    char          m_szFilePath[MAX_PATH];     //״̬�ļ�·��
    FILE*         m_pFile;                    //�ļ�ָ��

};
#endif  //__SENDER_STATUS_H__
