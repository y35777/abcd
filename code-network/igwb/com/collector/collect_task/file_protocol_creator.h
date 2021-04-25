#ifndef __FILE_PROTOCOL_FACTORY_H__
#define __FILE_PROTOCOL_FACTORY_H__

class CFileProtocol;

//���������ļ������๤��CFileProtocolFactory�Ķ���
class  CFileProtocolFactory
{

public:
    CFileProtocolFactory();
    ~CFileProtocolFactory();

    //���ݲ�ͬ��ʶ������̬����CFileProtocol���������
      static  CFileProtocol*  CreateObject(const char* szId,
                                           int nTransSpeed,
                                           const char* szDevName); //added by chenliangwei

};

#endif //__FILE_PROTOCOL_FACTORY_H__

