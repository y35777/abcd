#ifndef __FILE_PROTOCOL_FACTORY_H__
#define __FILE_PROTOCOL_FACTORY_H__

class CFileProtocol;

//以下是是文件传输类工厂CFileProtocolFactory的定义
class  CFileProtocolFactory
{

public:
    CFileProtocolFactory();
    ~CFileProtocolFactory();

    //根据不同标识串来动态创建CFileProtocol派生类对象
      static  CFileProtocol*  CreateObject(const char* szId,
                                           int nTransSpeed,
                                           const char* szDevName); //added by chenliangwei

};

#endif //__FILE_PROTOCOL_FACTORY_H__

