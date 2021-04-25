#ifndef __VERTEL_S1240_CMISE_H__
#define __VERTEL_S1240_CMISE_H__

#include "s1240_cmise.h"
#include "../../include/i_cmise_fun.h"

//�������ļ��������Ķ���
class CS1240VertelCmise : public CS1240Cmise
{
public:

    CS1240VertelCmise();
    virtual  ~CS1240VertelCmise();

    //��ʼ�����ط���, ���麯��, �����������ʵ�ָ÷���
    virtual int  Init();

    virtual int  GetFilePool(SS1240FilePoolMsg& pool_msg);

    virtual int  SetFilePool(int nAlarmLimit);

    virtual int  GetFile(const char* const szFileId,
                         SS1240FileMsg&    file_msg);

    virtual int  SetFile(const char* const szFileId,
                         int               nFilestatus);

    //����ļ���������ǰĿ¼�µ��ļ��б�, �麯��, �����������ʵ�ָ���
    virtual int  GetFileList(int nFileStatus,
                             VECTOR<SS1240FileMsg> &Out);
protected:
};

#endif //__VERTEL_S1240_CMISE_H__
