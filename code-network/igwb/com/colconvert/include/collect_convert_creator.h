#ifndef __COLLECT_CONVERT_CREATOR_H__
#define __COLLECT_CONVERT_CREATOR_H__

class ICollectConvert;

class CCollectConvertFactory
{
public:
    static ICollectConvert* CreateCollectConvert(int nSwitchIndex, const char* const szCfgPath);
};

#endif //__COLLECT_CONVERT_CREATOR_H__
