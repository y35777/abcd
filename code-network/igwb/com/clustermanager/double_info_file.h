//本文件由颜文远于2002.05.08创建

#ifndef _DOUBLE_INFO_FILE_H_
#define _DOUBLE_INFO_FILE_H_

#include "../frontsave/info_file.h"

//本类提供一个单一的状态文件读写接口，屏蔽了状态文件的主备细节
class CDoubleInfoFile
{
public:
    CDoubleInfoFile();
    virtual ~CDoubleInfoFile();

	//初始化函数
    void Init(const char* const szMainFilePath,
              const char* const szMainFileName,
              const char* const szBakFilePath,
              const char* const szBakFileName);

    //设置文件头长度，记录单元长度和记录单元个数
    void SetFileStruct(const UINT4 uHeadLen, const UINT4 uUnitLen, 
                       const UINT4 uUnitNum);

    //根据属性中指定的路径、文件名和长度创建或打开状态文件
    BOOL OpenInfoFile(BOOL bNeedCreate = FALSE, BYTE* pDefaultHead = NULL);

    //关闭状态文件
    BOOL Close();

    //将未写入到硬盘的状态数据写入到硬盘中
    BOOL Flush();

    //该函数实现写文件头的功能
    BOOL WriteHead(const BYTE* pData, const UINT4 uLen);

    //函数实现读取文件头的功能
    BOOL ReadHead(BYTE* pData);


protected:

    //主状态文件
    CInfoFile   m_InfoMain;

	//备状态文件
	CInfoFile   m_InfoBak;

	//状态文件头的长度
	UINT4 m_uHeadLen;
};

#endif