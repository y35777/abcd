#ifndef __INFOFILE_H__
#define __INFOFILE_H__

#include "../include/precom.h"
#include "../include/frame.h"

#define MAX_UNIT_NUM 150000

struct SInfoHead
{
    UINT4 uCheckSum;            //�����ļ�ͷУ���У���
    UINT4 uRotateCount;         //״̬�ļ���ת�Ĵ���
    UINT4 uStatusFileWritePos;  //��ǰ״̬��¼��дָ��
    UINT4 Data[1];              //ҵ���еľ���ṹ���ɲ�ͬ��ģ����Ծ���
};

//Ϊָ�����ļ�����ָ����С�Ŀռ�
BOOL AskFileSpace(FILE* f, int nFileSize);

class CInfoFile
{
public:
    //���졢��������
    CInfoFile();
    virtual ~CInfoFile();

    //�������úͻ�ȡ����
    STRING GetFileFullPath();
    UINT4 GetUnitNum();
    UINT4 GetWriteIndex();
    //����״̬�ļ����ʹ��·��
    void SetInfoFilePath(const STRING& szFileName, 
                         const STRING& szFilePath);   
    //����״̬�����ļ����ʹ��·��
    void SetBackupFilePath(const STRING& szFileName, 
                           const STRING& szFilePath);
    //�����ļ�ͷ���ȣ���¼��Ԫ���Ⱥͼ�¼��Ԫ����
    void SetFileStruct(const UINT4 uHeadLen, const UINT4 uUnitLen, 
                              const UINT4 uUnitNum);

    //����ӿ�
    //��ָ�����ļ�ͷ����У���
    void CalcCheckSum(BYTE* pHead);
    //ʹpHead�е�uStatusFileWritePos����һ��״̬��¼�ĳ���
    void IncreaseIndex(BYTE* pHead);
    //����������ָ����·�����ļ����ͳ��ȴ������״̬�ļ�
    BOOL OpenInfoFile(BOOL bNeedCreate = FALSE, BYTE* pDefaultHead = NULL);
    //�ر�״̬�ļ�
    BOOL Close();
    //ɾ��״̬�ļ�
    BOOL Delete();
    //��δд�뵽Ӳ�̵�״̬����д�뵽Ӳ����
    BOOL Flush();
    //�ú���ʵ�ֽ�״̬�ļ���������Ӧ�ı���Ŀ¼��
    BOOL CopyFileForBackup();
    //�ú���ʵ��д�ļ�ͷ�Ĺ���
    BOOL WriteHead(const BYTE* pData, const UINT4 uLen);
    //д��m_InfoWriteIndex����¼��Ԫ��Ҳ�������һ����¼��Ԫ
    BOOL WriteUnit(const BYTE* pData, const UINT4 uLen);
    //����ʵ�ֶ�ȡ�ļ�ͷ�Ĺ���
    BOOL ReadHead(BYTE* pData);
    //�ú���ʵ�ֶ�ȡָ����¼��Ԫ�Ĺ���
    int ReadUnit(UINT4 uIndex, BYTE* pData);
    //�ú���ʵ�ֶ�ȡ���һ����¼��Ԫ�Ĺ���
    int ReadLastUnit(BYTE* pData);
    //У���ļ�ͷ�Ƿ���Ч
    BOOL CheckHead();

protected:
    //�ṩ״̬�ļ�д��Ľӿ�
    BOOL WriteInfo(const UINT4 uOffset, const BYTE* pData, 
                   const UINT4 uLen);    
    //�ṩ��ȡ״̬�ļ����ݵĽӿ�
    BOOL ReadInfo(const UINT4 uOffset, const BYTE* pData, 
                  const UINT4 uLen);

protected:
    //��Ա����
    FILE*  m_pInfoFile;           //��Ӧ�ļ����󣬹�������ļ���д
    STRING m_szInfoFileName;      //״̬�ļ���
    STRING m_szInfoFilePath;      //�ļ����·��
    STRING m_szBackupName;        //�����ļ���
    STRING m_szBackupPath;        //�����ļ����·��
    UINT4  m_uHeadLen;            //�ļ�ͷ���ȣ����ֽ�Ϊ��λ
    UINT4  m_uUnitLen;            //״̬��¼��Ԫ���ȣ����ֽ�Ϊ��λ
    UINT4  m_uUnitNum;            //״̬��¼������Ĭ��ȡֵΪMAX_UNIT_NUM
    UINT4  m_uFileLen;            //�ļ��ܳ���
    UINT4  m_uInfoWriteIndex;     //��ʾ��ǰд����״̬��¼������
                                  //ȡֵΪ1��m_uUnitNum
    int    m_nRotateCount;        //״̬�ļ��Ļ��ƴ���
};

#endif
