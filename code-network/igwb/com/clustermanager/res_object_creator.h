#ifndef __RESOURCE_OBJECT_CREATOR_H__
#define __RESOURCE_OBJECT_CREATOR_H__

#include "res_object.h"

class CResObjectCreator
{
public:
    CResObjectCreator();
    ~CResObjectCreator();

    //创建同步资源对象的MAP
    static MAP<int, CResObject*>* CreateResObject(RESOURCE_TYPE nStaticRes);

};

#endif //__RESOURCE_OBJECT_CREATOR_H__
