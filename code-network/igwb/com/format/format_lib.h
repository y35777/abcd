#if !defined(_FORMAT_FORMAT_LIB_H_)
#define _FORMAT_FORMAT_LIB_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CFormatLib
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"
#include "ace/DLL.h"

class CFormat;

//��ʽ��Ϊ���ʽ��ӳ���ϵ���Ͷ���
typedef  MAP<STRING,  CFormat*> FORMAT_MAP;

/****************** The CFormatLib class Defining *******************/
// ������������ʽ����
//
/********************************************************************/
class CFormatLib
{
public:

    //���캯��
    CFormatLib(const STRING& sRootDir);

    //��������
    ~CFormatLib();

    //��ʼ������
    int Init(int bCfg = -1);

    //��õ�ǰ��ʽ�������и�ʽ��
    UINT4 GetFormats(LIST<STRING>& FmtNamesList) const;

    //���ָ����ʽ���ĸ�ʽ����
    CFormat* GetFormat(const STRING& sFmtName, int nAP = -1);

    //����һ���»�����ʽ����
    int AddFormat(const STRING& sFmtName);

    //ɾ��һ�ֻ�����ʽ����
    int DelFormat(const STRING& sFmtName);

    //ʹ��һ��Commit������Commit����֮�����޸ĵ�������Ϣ���浽������
    int Commit(void);
    
    void SetMustMerge(BOOL bMustMerge);

protected:

    CFormat* CreateFormatObject(const char* szDllName,
                                const char* szRootDir, 
                                const char* szFmtName,
                                int nAP);

protected:

    STRING              m_sFmtRootDir;      //��ʽ���ÿ�ĸ�Ŀ¼
    FORMAT_MAP*         m_pFmtsMap;         //�Ը�ʽ��Ϊ������MAPָ��
    
    BOOL                m_bIsMustMerge;

    MAP<int,ACE_DLL*>   m_mapAPToFmtSpDll;

    //�����ŵ���ʽ�����Զ�̬������MAP
    MAP<int,STRING>     m_mapAPToFmtSpName;
};
/****************** End of The CFormatLib class Defining ************/


#endif  //!defined(_FORMAT_FORMAT_LIB_H_)
