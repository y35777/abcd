#ifndef __FRONTSAVE_H__
#define __FRONTSAVE_H__

#include "save.h"

//ǰ����״̬�ļ����ļ�ͷ����
struct FS_STATUS_FILE_HEAD
{
    UINT4 uCheckSum;            //У���
    UINT4 uRotateCount;         //״̬�ļ���ת�Ĵ���
    UINT4 uStatusFileWritePos;  //״̬�ļ������дָ��
    UINT4 uLastPID;             //ǰ����ģ�鵱ǰ��Ҫ�����ԭʼ�������İ���
    UINT4 uFileCsn;             //����ΪuLastPid��ԭʼ���������ڵ�ԭʼ��
                                //���ļ����ļ����к�
    UINT4 uFileWritePos;        //����ΪuLastPid��ԭʼ���������ڵ�
                                //ԭʼ�����ļ���дָ��
    UINT4 uaReserved[2];        //�����ֽ�
};

//ǰ����״̬��¼�Ķ���
struct FS_STATUS_FILE_ELEMENT
{
    UINT4 uPID;         //ǰ����ģ���������ԭʼ�����İ���
    UINT4 uFileCsn;     //����ΪuPID��ԭʼ���������ڵ�ԭʼ�����ļ����ļ����к�
    UINT4 uStartPos;    //����ΪuPID��ԭʼ���������ļ��е���ʼλ��
    UINT4 uPacketLen;   //����ΪuPID��ԭʼ�������ĳ���
    UINT2 nYear;        //����ΪuPID��ԭʼ���������ڵ�ԭʼ�����ļ��Ĵ������
    BYTE  yMonth;       //����ΪuPID��ԭʼ���������ڵ�ԭʼ�����ļ��Ĵ����·�
    BYTE  yDay;         //����ΪuPID��ԭʼ���������ڵ�ԭʼ�����ļ��Ĵ�������
    UINT4 uReserved;    //�����ֶ�
};

class CFrontSave : public CSaveTransaction
{
public:
    //���졢��������
    CFrontSave();
    virtual ~CFrontSave();

    //�����ӵĽӿ�
    BOOL Init(UINT4 uAPID); //�����ʼ��ʱ�Ĺ���
    //��ȡָ���Ļ�������Χ�еĵ�һ���ܹ������Ļ���
    int GetFirstBillPacket(const UINT4 uFirstPID, const UINT4 uLastPID, 
                           UINT4& uPID, BYTE*& pPacket, UINT4&  uPacketLen, 
                           STRING& szFileFullPath, UINT4& uStartPos);
    //��ȡָ���Ļ�������Χ�е���һ���ܹ������Ļ���
    int GetNextBillPacket(const UINT4 uLastPID,
                          UINT4& uPID, 
                          BYTE*& pPacket, 
                          UINT4& uPacketLen, 
                          STRING& szFileFullPath, 
                          UINT4& uStartPos);
    //����״̬�ļ��õ����һ���������İ���
    UINT4 GetLastPID();
    //�õ���ǰ����������ز�������Щ���������򻺳������ͻ������������Ϣ
    void GetCurPacketParam(STRING& szFileFullPath, UINT4& uStartPos);
protected:
    //�ӻ���̳еĺ���
    //�õ���һ�������ļ����Ŀ¼�����Ŀ¼�����ڣ��򴴽�֮
    void GetNextFilePath();
    //ɾ�����һ����ʱ�ļ����ڽ���״̬�ļ�У��ʱ����
    BOOL DeleteTempFile();
    //�����յ��Ļ�����������״̬�ļ��������Լ�����Ա����
    void RefreshStatus(BYTE* pPacketInfo);
    //�����µĻ����ļ��Լ���ص�״̬�ļ�ˢ�²���
    BOOL CreateNewFile();
    //�رջ����ļ��ĺ�������
    BOOL DoWhenFileClose();
    //�ڱ��滰���������е�д״̬�ļ�����
    BOOL WriteStatusFile(CInfoFile* pInfoFile, BOOL bFlush);
    //�ڱ��滰���������е�д�����ļ�����
    int WriteBillFile(BYTE* pPacketInfo, BOOL bFlush);

	//����Զ2002.05.18�޸ģ����ⵥ��D13767
    //��������У��ʱ����״̬�ļ���һ�µ����
    BOOL DoWithMainDiffBak(BYTE* pMainHead, BYTE* pBakHead, 
						   BYTE* pMainLastUnit, BYTE* pBakLastUnit);
	//����Զ2002.05.18�޸Ľ���

    //����״̬�ļ������Ϣ����صĳ�Ա�������г�ʼ��
    BOOL InitByStatusFile();
	//�����յ��İ���Ϣ�ж��Ƿ�Ϊ�հ�
	BOOL IsEmptyPacket(BYTE* pInfo);

    //�����ӵĺ���
    //����ָ����״̬��¼��û����ļ���ȫ·��
    STRING GetFileFullPath(FS_STATUS_FILE_ELEMENT* pInfoUnit);
    //����ָ����״̬��¼��û����ļ���·�����ļ���
    void GetFilePathAndName(FS_STATUS_FILE_ELEMENT* pInfoUnit, 
                            STRING& szFilePath, STRING& szFileName);
    //���ݻ������ŵõ�״̬�ļ�����Ӧ��״̬��¼������Ҳ�����صļ�¼�����
    //���������������İ��Ŷ�Ӧ��״̬��¼----��״̬�ļ��д��ڣ���Ϊ���д�
    //��uPID�İ�������С�İ���
    int GetInfoUnit(const UINT4 uPID, 
                    FS_STATUS_FILE_ELEMENT* pInfoUnit, INT4& nIndex);
    //��ָ�����ļ��ж�ȡһ����������
    BOOL ReadPacket(const char* szFileFullPath, const BYTE* pPacket, 
                    const UINT4 uLen, const UINT4 uStartPos);

	virtual void DelMoreFiles();

protected:
    UINT4  m_uAPID;            //���ڵĽ�����ID��
    STRING m_szDatePath;       //����Ŀ¼��ŵ�·��

    //���µı������ڽ��ж�ȡĳ�����ŷ�Χ�ڵĻ������Ĳ���
    //���ŷ�Χ�е���С����
    UINT4  m_uFirstPID;        
    //���ŷ�Χ�е�������
    UINT4  m_uLastPID;
    //��ȡָ����Χ�Ļ������Ĳ����Ƿ��Ѿ�����
    BOOL   m_bReadComplete;    
    //��ǰ���ڶ�ȡ��״̬��¼��������
    INT4   m_nCurIndex;        
};

#endif
