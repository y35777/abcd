#ifndef __I_COLLECT_CONVERT_H__
#define __I_COLLECT_CONVERT_H__

class ICollectConvert
{
public:
    ICollectConvert(){};
    virtual ~ICollectConvert(){};

    //���������ļ�ȫ·����
    virtual void SetCfgPath(const char* const szCfgPath) = 0;

    //��ʼ���ɼ�����
    virtual int Init() = 0;

    //ת�������ļ�
    virtual int Process(const char* szBillFile, 
                        const int& nConvertInfo) = 0;

};

#endif //__I_COLLECT_CONVERT_H__
