#if !defined(_FORMAT_FORMAT_H_)
#define _FORMAT_FORMAT_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CFormat
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"

class CCondition;
class CConvert;

/****************** The CFormat class Defining **********************/
// 功能描述：话单格式类
//
/********************************************************************/
class IGWB_Svc_Export CFormat
{
public:

    //构造函数
    CFormat(const STRING& sRootDir, const STRING& sFmtName);

    //将格式配置信息读入内存函数
    int LoadConfig(void);

    //计算本格式字节的长度（该函数目前用不到）
    UINT4  CaculateFmtLen(void);

    //创建一种话单格式
    int Create(void);

    //删除一种话单格式
    int Delete(void);

    //获得本格式名
    void GetFmtName(STRING& sFmtName) const;
    const char* GetFmtName() const;

    //设置本格式对应的源格式对象
    int SetSrcFmt(CFormat*  pSrcFmt);

    //获得本格式对应的源格式对象
    CFormat* GetSrcFmt(void) const;

    //获得本格式的源格式名
    void GetSrcFmtName(STRING& sSrcFmtName) const;
    const char* GetSrcFmtName() const;


    //获得本格式对应的单张话单长度（字节数）
    UINT4 GetFmtLen(void) const;
    
	//获得本格式对应的源格式长度单张话单长度（字节数）
	UINT4 GetSrcFmtLen(void) const;

    //获得本格式域个数
    UINT4 GetFieldNum(void) const;

    //获得本格式所有话单域MAP指针
    const FIELD_MAP* GetFieldsMap(void) const;

    //获得本格式所有话单域数组指针
    const SFieldInfo** GetFieldsArray(void) const;

    //获得本格式的格式描述和源格式名
    void GetFormatGen(STRING& sFmtDesc, STRING&  sSrcFmtName) const;

    //设置本格式的格式描述和源格式名
    void SetFormatGen(const STRING& sFmtDesc,  const STRING& sSrcFmtName);

    //设置本格式的识别条件
    void SetDistingCondition(const STRING& sDistingCondition);

    //获得本格式的识别条件
    void GetDistingCondition(STRING& sDistingCondition) const;
    
    //根据格式域索引从一张话单中获得该域的值
    //2004-02-18  SWPD03631 增加参数pDestFI实现二进制取枚举值 ZhengYuqun
    int GetFieldValueByIndex(const void* pABill, const int bThisFmt, 
                             const UINT4 uIndex,
                             void* pFieldValue, UINT4& uLen, SFieldInfo* pDestFI = NULL);

    //使上一次Commit到本次Commit调用之间所修改的配置信息保存到磁盘中
    int Commit(void);

    //设置查询条件
    int SetQueryCond(const STRING& sQueryCond);

    //获取话单域类型的长度
    static int GetFieldTypeLen(const SFieldInfo* pFI);

	
	//分析表达式，提取域名
	int ParseExpress(const char* const  szExpress, LIST<STRING>& FieldList);

public:

    //析构函数
    virtual ~CFormat();

    //初始化函数
    virtual int Init(void);

    //计算一个格式中所有域的字节偏移、位偏移以及字节总长度函数
    virtual int InitFormatInfo(void);
    
    //判断传入的一张话单是否符合本格式
    virtual int IsTheFormat(const void* pABill,  const UINT4 uLen);

    //将传入的一张话单转换成目标格式
    virtual int ConvertABill(const void* pASrcBill, const UINT4 uSrcBillLen, 
                             void*& pADestBill, UINT4& uDestBillLen);

    //根据话单域名从一张话单中获得该域的值
    virtual int GetFieldValueByName(const void* pABill, const int bThisFmt,
                                    const STRING& sFieldName,  
                                    void*& pFieldValue, UINT4& uLen,
                                    int nIsQuery = 0);

	//根据话单域序号从一张话单中获得该域的值
	virtual int GetFieldContentByIndex(const void* pABill, const int bThisFmt,
		                               UINT2 nFieldIndex,  
		                               void* pFieldValue, UINT4& uLen,
		                               int nIsQuery);
    //判断一张话单是否符合指定的查询条件
    virtual int IsTheQueryCondition(const void* pASrcBill, const STRING& sQueryCond = "");

	//把Intel或者Net字节序转为本机字节序
    //Modified by XDB on 2002.10.28
	static int ConvertToHostOrder(const SFieldInfo* pFI, void* pFieldValue);

    void SetMustMerge(BOOL bMustMerge);

protected:
    STRING         m_sFmtRootDir;		//格式库的根路径
    STRING         m_sFmtName;	        //格式名称
    STRING         m_sFmtDesc;		    //格式描述
    STRING         m_sSrcFmtName;		//本格式对应的源格式名
    STRING         m_sDistingCondition;	//格式识别条件，字符串方式的中缀表达式，
                                        //从格式库中读出
    STRING         m_sQueryCond;        //查询条件，字符串方式的中缀表达式

    FIELD_MAP*     m_pFieldsMap;		//以话单域名称为索引MAP
    CCondition*    m_pDistingCondition;	//作为识别条件的后缀表达式对象
    CFormat*       m_pSrcFmt;		    //本格式的源格式对象
    CConvert*      m_pConvert;		    //格式转换对象
    CCondition*    m_pQueryCondition;	//作为查询条件的后缀表达式对象 
    UINT4          m_uFmtLen;		    //本格式所有话单域字节总长度
    UINT4          m_uFieldNum;		    //本格式话单域总个数
    SFieldInfo**   m_pFieldsArray;		//话单域描述数组，数组序号就是话单域的序号，
                                        //数组长度就是格式描述的话单域个数

	CCondition*    m_pStatCondition;	//作为统计条件的后缀表达式对象
    BOOL           m_bIsMustMerge;

	//邱意平 2002年4月4日增加如下两个成员函数
	//由于iGateway Bill V200的格式配置在保存各个配置文件时，统一按网络字节序、1字节
	//对齐方式保存，在NT平台下，格式库可以按1字节对齐编译，但在UNIX平台下，不能使用
	//1字节对齐编译，因此格式库在加载配置文件时必须处理字节序和字节对齐问题
private:
    void  SerialFieldInfo(SFieldInfo* pOut, BYTE* pIn); 
	void  SerialEnumInfo(SEnumInfo* pOut, BYTE* pIn);
};
/****************** End of The CFormat class Defining ***************/

#endif  //!defined(_FORMAT_FORMAT_H_)
