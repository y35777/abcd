#ifndef __COLLECT_CONVERT_OBJ_H_
#define __COLLECT_CONVERT_OBJ_H_

#include "include/i_collect_convert.h"

class CCollectConvertObj 
{
public:
    CCollectConvertObj(int nConvertID, const char* const szCfgPath);
    virtual ~CCollectConvertObj();

    //��ʼ��
    virtual int Init();

    //ת�������ļ�
    virtual int Process(const char* szBillFile, 
                        const int& nConvertInfo);


    //ȡ�����
    inline int  GetConvertID(){return m_nConvertID;};

protected:

   //���ɼ������Ӧ�������ļ�ȫ·����
    char          m_szCfgPath[MAX_PATH];

    //�ɼ��������ӿ�ָ��
    ICollectConvert* m_pCollectConvert;

    //ת��ID�ţ����������������ļ��еı��
    int          m_nConvertID;

};

#endif //__COLLECT_CONVERT_OBJ_H_
