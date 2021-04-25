#ifndef _COLLECT_FACTORY_H__
#define _COLLECT_FACTORY_H__

class CCollectBase;
class CLinkGroup;  //added by chenliangwei
 
/*
CCollectFactory���ļ��ɼ������๤�������ڶ�̬������ͬ���ļ��ɼ���
������C5ESSCollect, CBamCollect����ȣ�
*/
//�������ļ��ɼ������๤��CCollectFactory�Ķ���

class  CCollectFactory
{

public:
    CCollectFactory();
    ~CCollectFactory();

    //���ݲ�ͬ��ʶ������̬����CCollectBase�����������
    static CCollectBase* CreateCollectObject(const char* const szCfgPath,
                                             const SCreateCollect& stObj);

    static CCollectBase* CreateS1240Object(const char* const szCfgPath,
                                           const SCreateCollect& stObj);

    static CCollectBase* Create5ESSObject(const char* const szCfgPath,
                                          const SCreateCollect& stObj);
    //added by chenliangwei
    static CCollectBase* CreateEWSD_MMObject(const char* const szCfgPath,
                                             const SCreateCollect& stObj);

    static CCollectBase* CreateEWSD_AMAObject(const char* const szCfgPath,
                                             const SCreateCollect& stObj);


    static CCollectBase* CreateComObject(const char* const szCfgPath,
                                         const SCreateCollect& stObj);

protected:
    static CLinkGroup* CreateLinkGroup(const char* const szCfgPath,
                                       const SCreateCollect& stObj);

};

#endif //_COLLECT_FACTORY_H__
