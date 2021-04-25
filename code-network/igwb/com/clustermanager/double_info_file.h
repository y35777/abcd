//���ļ�������Զ��2002.05.08����

#ifndef _DOUBLE_INFO_FILE_H_
#define _DOUBLE_INFO_FILE_H_

#include "../frontsave/info_file.h"

//�����ṩһ����һ��״̬�ļ���д�ӿڣ�������״̬�ļ�������ϸ��
class CDoubleInfoFile
{
public:
    CDoubleInfoFile();
    virtual ~CDoubleInfoFile();

	//��ʼ������
    void Init(const char* const szMainFilePath,
              const char* const szMainFileName,
              const char* const szBakFilePath,
              const char* const szBakFileName);

    //�����ļ�ͷ���ȣ���¼��Ԫ���Ⱥͼ�¼��Ԫ����
    void SetFileStruct(const UINT4 uHeadLen, const UINT4 uUnitLen, 
                       const UINT4 uUnitNum);

    //����������ָ����·�����ļ����ͳ��ȴ������״̬�ļ�
    BOOL OpenInfoFile(BOOL bNeedCreate = FALSE, BYTE* pDefaultHead = NULL);

    //�ر�״̬�ļ�
    BOOL Close();

    //��δд�뵽Ӳ�̵�״̬����д�뵽Ӳ����
    BOOL Flush();

    //�ú���ʵ��д�ļ�ͷ�Ĺ���
    BOOL WriteHead(const BYTE* pData, const UINT4 uLen);

    //����ʵ�ֶ�ȡ�ļ�ͷ�Ĺ���
    BOOL ReadHead(BYTE* pData);


protected:

    //��״̬�ļ�
    CInfoFile   m_InfoMain;

	//��״̬�ļ�
	CInfoFile   m_InfoBak;

	//״̬�ļ�ͷ�ĳ���
	UINT4 m_uHeadLen;
};

#endif