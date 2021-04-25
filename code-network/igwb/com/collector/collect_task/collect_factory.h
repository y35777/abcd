#ifndef _COLLECT_FACTORY_H__
#define _COLLECT_FACTORY_H__

class CCollectBase;
class CLinkGroup;  //added by chenliangwei
 
/*
CCollectFactory是文件采集操作类工厂，用于动态创建不同的文件采集类
对象，如C5ESSCollect, CBamCollect对象等；
*/
//以下是文件采集操作类工厂CCollectFactory的定义

class  CCollectFactory
{

public:
    CCollectFactory();
    ~CCollectFactory();

    //根据不同标识串来动态创建CCollectBase的派生类对象
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
