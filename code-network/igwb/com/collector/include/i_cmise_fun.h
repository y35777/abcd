#ifndef __I_CMISE_FUN_H__
#define __I_CMISE_FUN_H__

#define CMISE_STATUS_FREE                   1
#define CMISE_STATUS_SELECTED               2
#define CMISE_STATUS_RECORDING              3
#define CMISE_STATUS_READYTOBETRANSFERRED   4
#define CMISE_STATUS_TRANSFERRING           5

#define CMISE_MODE_OK                       1
#define CMISE_MODE_ORIGOK                   2
#define CMISE_MODE_DUPOK                    3
#define CMISE_MODE_NOTOK                    4

class ICmiseFun
{
public:
	ICmiseFun(){};
    virtual ~ICmiseFun(){};

    //≥ı ºªØ
    virtual int Init(int argc = 0, char** argv = 0) = 0;

    virtual int Release() = 0;

    virtual int CreateAssociate() = 0;

    virtual int SetTPMsg(const char* const szAssPwd,
                         const char* const szActPwd,
                         const char* const szNename,
                         const char* const szTpid) = 0;

    virtual int GetFileList(int nFileStatus) = 0;

    virtual int GetFilePool(char*& szFilePoolId,
                            int&   nFileCount,
                            int&   nFileFullCount,
                            int&   nAlarmLimit) = 0;

    virtual int GetNextFile(char*&         szFileId, 
                            int&           nFileStatus,
                            int&           nFileMode,
                            char*&         szFileIdDup, 
                            int&           nFileCounter,
                            BOOL&          bFileLocalDumped,
                            unsigned long& uFileSize) = 0;

    virtual int SetFilePool(int nAlarmLimit) = 0;                                     

    virtual int GetFile(const char* const szFileId,
                        int&              nFileStatus, 
                        int&              nFileMode,
                        char*&            szFileIdDup, 
                        int&              nFileCounter,
                        BOOL&             bFileLocalDumped,
                        unsigned long&    uFileSize) = 0;

    virtual int SetFile(const char* const szFileId,
                        int               nFilestatus) = 0;
};

#endif //__I_CMISE_FUN_H__
