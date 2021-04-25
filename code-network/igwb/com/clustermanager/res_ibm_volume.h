#ifndef __RESOURCE_IBM_VOLUME_H__
#define __RESOURCE_IBM_VOLUME_H__

#include "res_volume.h"

extern "C" {
typedef int (*RefreshIBMRaid)();
}

class CResIBMVolume : public CResVolume
{
public:

    CResIBMVolume(const char* const szDriveName,
                  const char* const szResName,
                  ERES_TYPE nResType,
                  int nMaxLocalRecovery);
    virtual ~CResIBMVolume();

    virtual int Active();       //��Դ�ļ����

protected:

    //��IBM��RAID�豸����ǿ�ƽӹ�};
    BOOL TakeOver();

    RefreshIBMRaid GetRefreshIBMRaid();

};


#endif //__RESOURCE_IBM_VOLUME_H__
