#ifndef __COLLECT_CONVERT_OBJ_FACTORY_H__
#define __COLLECT_CONVERT_OBJ_FACTORY_H__

class CCollectConvertObj;

class CCollectConvertObjFactory
{
public:
    static VECTOR<CCollectConvertObj*>* CreateCollectConvertObj(const char* const szCfgPath);
};

#endif //__COLLECT_CONVERT_OBJ_FACTORY_H__
