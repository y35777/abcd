#if !defined(_FORMAT_CONVERTFUN_H_)
#define _FORMAT_CONVERTFUN_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CConvertFun
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"
#include "ace/Synch.h"

class CFormat;

#define MAX_PARA_LEN  255  //定义一个函数参数的最大长度

/****************** The CConvertFun class Defining ******************/
// 功能描述：转换函数类，该类是一个工具类（Utility Class），它的所有属性
//           和方法都是静态的
//
/********************************************************************/
class CConvertFun
{
public:

    static void AddRef(void);     //引用计数器加1函数
    static void ReleaseRef(void); //引用计数器减1函数

    /************ The ConvertAnalysis Defining *********************/
    // 功能描述：从格式域转换函数中分离出库名、函数名、参数列表、域缺省值
    //
    // 输入参数：
    // sConvertFun：转换函数，对应SFieldInfo中的szConvertFun
    //
    // 输出参数：
    //        sLib：库名（如果不使用动态连接库，则返回的sLib为空字符串）
    //        sFun：转换函数名
    //    sDefault：域缺省值
    //    ParaList：参数列表，按函数参数从左到右排列
    //
    //   返回值：如果由于sConvertFun配置出错，则函数返回-1，否则返回0
    //
    /***************************************************************/
    static int ConvertAnalysis(const STRING& sConvertFun, STRING& sLib, 
                               STRING& sFun, STRING& sDefault, 
                               LIST<STRING>& ParaList);
    /************ End of The ConvertAnalysis Defining **************/

    /************ The BlockCopy Defining ***************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现数据块的拷贝
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向源数据块的内存，
    //           nSrcFieldLen[0]表示源数据块的长度
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int BlockCopy(void* pCurField, const UINT4 nSrcFields,
                         const void* pSrcFields[],
                         const UINT4 nSrcFieldLen[],
                         const SFieldInfo* pFieldInfo,
                         const void* pDefault, const UINT4 nDefaultLen,
                         int nIsQuery = 0);
    /************ End of The BlockCopy Defining ********************/

    //块拷贝函数，支持Intel/网络字节序不同长度整数的互相拷贝 ZhengYuqun 2004-04-28
    static int BlockCopyEx(void* pCurField, const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);

    /************ The GetBinYear Defining **************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现将BYTE型或者SHORT型的两位年转换成二进制四位年
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向数据类型为BYTE或SHORT型的2位“年”，
    //           如果nSrcFieldLen[0]为1，表示pSrcFields[0]为BYTE型的年，
    //           如果nSrcFieldLen[0]为2，表示pSrcFields[0]为SHORT型的年
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int GetBinYear(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[], 
                          const UINT4 nSrcFieldLen[], 
                          const SFieldInfo* pFI, 
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery = 0);
    /************ End of The GetBinYear Defining *******************/

    /************ The GetTextYear Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现将BYTE或SHORT型的两位年转换成文本格式的四位年
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向数据类型为BYTE或SHORT型的2位“年”，
    //           如果nSrcFieldLen[0]为1，表示pSrcFields[0]为BYTE型的年，
    //           如果nSrcFieldLen[0]为2，表示pSrcFields[0]为SHORT型的年
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int GetTextYear(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The GetTextYear Defining ******************/

    /************ The GetTextTime Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现年、月、日、时、分、秒转换成文本格式的YYYY-MM-DD HH:NN:SS
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为6，
    //           pSrcFields[0]指向数据类型为BYTE或SHORT型的2位“年”，
    //           pSrcFields[1]、pSrcFields[2]、pSrcFields[3]、pSrcFields[4]、
    //           pSrcFields[5]分别指向BYTE型月、日、时、分、秒，
    //           如果nSrcFieldLen[0]为1，表示pSrcFields[0]为BYTE型的年，
    //           如果nSrcFieldLen[0]为2，表示pSrcFields[0]为SHORT型的年
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int GetTextTime(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The GetTextTime Defining ******************/

	/************ The GetTextTimeEx Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现年、月、日、时、分、秒转换成文本格式的YYYY-MM-DD HH:NN:SS
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为6或者7
    //           pSrcFields[0]指向数据类型为BYTE或SHORT型的2位“年”，
    //           pSrcFields[1]、pSrcFields[2]、pSrcFields[3]、pSrcFields[4]、
    //           pSrcFields[5]分别指向BYTE型月、日、时、分、秒
    //           pSrcFields[6]如果存在，则为毫秒 
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int GetTextTimeEx(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The GetTextTime Defining ******************/

    /************ The Int2Text Defining ****************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现将各种整型数据转换成文本
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           nSrcFieldLen[0]为1、2、4、8时分别表示pSrcFields[0]
    //           指向数据类型为BYTE、SHORT、LONG、INT8型的数据
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int Int2Text(void* pCurField, const UINT4 nSrcFields, 
                        const void* pSrcFields[],
                        const UINT4 nSrcFieldLen[],
                        const SFieldInfo* pFI,
                        const void* pDefault, const UINT4 nDefaultLen,
                        int nIsQuery = 0);
    /************ End of The Int2Text Defining *********************/

    //add by zkq 2004-09-28
    /************ The BCD2RBCD Defining ****************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           1.将反向BCD码转换正向BCD码，
    //           转换时在两种情况下认为BCD数组结束：
    //           1、BCD码等于0xF，2、达到BCD码数组的最大长度
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           pSrcFields[0]指向正向BCD码数组， 
    //           nSrcFieldLen[0]指示该正向BCD码数组的长度
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
/***************************************************************/
static int BCD2RBCD(void* pCurField, const UINT4 nSrcFields, 
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery);
    /************ End of The BCD2RBCD Defining *********************/

    /************ The BCD2String Defining **************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           1.将正向BCD码转换成字符串，
	//           modify by zkq 2004-05-27
    //           2.将正向BCD码中B,C转换成字符'*','#'，
    //           add end
    //           转换时在两种情况下认为BCD数组结束：
    //           1、BCD码等于0xF，2、达到BCD码数组的最大长度
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向正向BCD码数组， 
    //           nSrcFieldLen[0]指示该正向BCD码数组的长度
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int BCD2String(void* pCurField, const UINT4 nSrcFields, 
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery = 0);
    /************ End of The BCD2String Defining *******************/

    /************ The BCD2String2 Defining **************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           将n.5个字节的正向BCD码转换成字符串，
    //           转换时在两种情况下认为BCD数组结束：
    //           1、BCD码等于0xF，2、达到BCD码数组的最大长度
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向正向BCD码数组， 
    //           nSrcFieldLen[0]指示该正向BCD码数组的长度
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int BCD2String2(void* pCurField, const UINT4 nSrcFields, 
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery = 0);
    /************ End of The BCD2String2 Defining *******************/

    /************ The SBCD2String Defining **************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           将正向SBCD码转换成字符串，
    //           转换时在两种情况下认为BCD数组结束：
    //           1、SBCD码大于等于0xA，2、达到BCD码数组的最大长度
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向正向BCD码数组， 
    //           nSrcFieldLen[0]指示该正向BCD码数组的长度
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int SBCD2String(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The BCD2String Defining *******************/

    /************ The RBCD2String Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           将反向BCD码转换成字符串，
    //           转换时在两种情况下认为BCD数组结束：
    //           1、BCD码等于0xF，2、达到BCD码数组的最大长度
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向反向BCD码数组， 
    //           nSrcFieldLen[0]指示该反向BCD码数组的长度
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int RBCD2String(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The RBCD2String Defining ******************/

    /************ The ZXRBCD2String Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           将反向BCD码转换成字符串，
    //           转换时在两种情况下认为BCD数组结束：
    //           1、BCD码等于0xF，2、达到BCD码数组的最大长度
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向反向BCD码数组， 
    //           nSrcFieldLen[0]指示该反向BCD码数组的长度
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int ZXRBCD2String(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery = 0);
    /************ End of The ZXRBCD2String Defining ******************/
    
    /************ The GetEnumString Defining ***********************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           根据一个格式域的枚举值获得其对应的枚举字符串
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向枚举型的格式域的取值
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int GetEnumString(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery = 0);
    /************ End of The GetEnumString Defining ****************/

    //Add by zkq for TT&T 2004-05-27

	/************ The BCDTime2Sec Defining **************************/
	// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
	//           将正向BCD码转换成字符串，并根据时分秒的BCD码转换成以秒显示的字符串
	//           转换时在两种情况下认为BCD数组结束：
	//           1、BCD码等于0xF，2、达到BCD码数组的最大长度
	//
	// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
	//           需要补充的是调用此函数时nSrcFields必须为1，
	//           pSrcFields[0]指向正向BCD码数组， 
	//           nSrcFieldLen[0]指示该正向BCD码数组的长度
	//
	// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
	//
	//   返回值：成功返回0，失败返回-1
	//
	/***************************************************************/
    static int CConvertFun::BCDTime2Sec(void* pCurField, const UINT4 nSrcFields, 
		                                const void* pSrcFields[],
		                                const UINT4 nSrcFieldLen[],
		                                const SFieldInfo* pFI,
		                                const void* pDefault, const UINT4 nDefaultLen,
		                                int nIsQuery);
   /************ End of The BCDTime2Sec Defining *******************/

	/************ The EBCD2String Defining **************************/
	// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
	//           将正向BCD码转换成字符串，
	//           转换时在两种情况下认为BCD数组结束：
	//           1、BCD码等于0xE，2、达到BCD码数组的最大长度
	//
	// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
	//           需要补充的是调用此函数时nSrcFields必须为1，
	//           pSrcFields[0]指向正向BCD码数组， 
	//           nSrcFieldLen[0]指示该正向BCD码数组的长度
	//
	// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
	//
	//   返回值：成功返回0，失败返回-1
	//
	/***************************************************************/
	static int CConvertFun::EBCD2String(void* pCurField, const UINT4 nSrcFields, 
                                        const void* pSrcFields[],
                                        const UINT4 nSrcFieldLen[],
                                        const SFieldInfo* pFI,
                                        const void* pDefault, const UINT4 nDefaultLen,
                                        int nIsQuery);
    /************ End of The EBCD2String Defining *******************/
	
	//End by zkq for TT&T 2004-05-27
	//Added by maimaoshi at 2002-10-16

	/************ The GetTextTimeInBCD Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现把BCD的年月日时分秒转换成文本格式的YYYY-MM-DD HH:NN:SS
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
	static int GetTextTimeInBCD(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery = 0);
	/************ End of The GetTextTimeInBCD Defining **********************/

	/************ The GetTextTimeInBCD_4Y Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现把4位年的BCD的年月日时分秒转换成文本格式的YYYY-MM-DD HH:NN:SS
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
	static int GetTextTimeInBCD_4Y(void* pCurField, const UINT4 nSrcFields, 
                                 const void* pSrcFields[],
                                 const UINT4 nSrcFieldLen[],
                                 const SFieldInfo* pFI,
                                 const void* pDefault, const UINT4 nDefaultLen,
                                 int nIsQuery = 0);
	/************ End of The GetTextTimeInBCD_4Y Defining **********************/

	/************ The GetTextTimeInBCD_T Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现把6.5个字节的BCD的时间转换成文本格式的YYYY-MM-DD HH:NN:SS.T
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
	static int GetTextTimeInBCD_T(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery = 0);
	/************ End of The GetTextTimeInBCD_T Defining **********************/

	/************ The Int2IpText Defining ****************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现将各种整型数据转换成文本
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           nSrcFieldLen[0]为1、2、4、8时分别表示pSrcFields[0]
    //           指向数据类型为BYTE、SHORT、LONG、INT8型的数据
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
	static int Int2IpText(void* pCurField, const UINT4 nSrcFields, 
                        const void* pSrcFields[],
                        const UINT4 nSrcFieldLen[],
                        const SFieldInfo* pFI,
                        const void* pDefault, const UINT4 nDefaultLen,
                        int nIsQuery = 0);
	/************ End of The Int2IpText Defining **********************/

	//end by maimaoshi at 2002-05-18

    //by ldf 2003-01-08 对应问题单D05886 D05556（同D05415 D5150）
    /************ The Int2IpTextDirect Defining ****************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现将各种整型数据转换成文本
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           nSrcFieldLen[0]为1、2、4、8时分别表示pSrcFields[0]
    //           指向数据类型为BYTE、SHORT、LONG、INT8型的数据
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
	static int Int2IpTextDirect(void* pCurField, const UINT4 nSrcFields, 
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery);
    /************ End of The Int2IpTextDirect Defining **********************/

    /************ The Array2Text Defining ****************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现将各种整型数据转换成文本
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           指向数据类型为BYTE型的数据
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int Array2Text(void* pCurField, const UINT4 nSrcFields, 
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery);
    /************ End of The Array2Text Defining **********************/
    //修改完毕


	/************ The BCDToSecond Defining **************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           将正向BCD码转换成字符串，
    //           转换时在两种情况下认为BCD数组结束：
    //           1、BCD码等于0xF，2、达到BCD码数组的最大长度
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向正向BCD码数组， 
    //           nSrcFieldLen[0]指示该正向BCD码数组的长度
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int BCDToSecond(void* pCurField, const UINT4 nSrcFields, 
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery = 0);
    /************ End of The BCDToSecond Defining *******************/

	/************ The GetTextTimeNoSp Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现年、月、日、时、分、秒转换成文本格式的YYYY-MM-DD HH:NN:SS
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为6，
    //           pSrcFields[0]指向数据类型为BYTE或SHORT型的2位“年”，
    //           pSrcFields[1]、pSrcFields[2]、pSrcFields[3]、pSrcFields[4]、
    //           pSrcFields[5]分别指向BYTE型月、日、时、分、秒，
    //           如果nSrcFieldLen[0]为1，表示pSrcFields[0]为BYTE型的年，
    //           如果nSrcFieldLen[0]为2，表示pSrcFields[0]为SHORT型的年
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int GetTextTimeNoSp(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The GetTextTimeNoSp Defining ******************/

	/************ The FillFF Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现对某字段填"FF"
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向数据类型为BYTE，
    //           nSrcFieldLen[0]为1
    //           
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int FillFF(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The FillFF Defining ******************/

   	/************ The ChangeGSVNandCallType Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现对SoftX300固网详细话单的呼叫类别和业务类型进行转换
    //           业务属性为本地长途的话单，业务属性都要改为本局，
    //           呼叫类型原来是出局改为本局，原来是汇接的改为入局
	//           此函数实现转换为二进制话单  
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为2，
    //          
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int ChangeGSVNandCallType(void* pCurField, 
                           const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery);
    /************ End of The ChangeGSVNandCallType Defining ******************/

	/************ The ChangeGSVNandCallType Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现对SoftX300固网详细话单的呼叫类别和业务类型进行转换
    //           业务属性为本地长途的话单，业务属性都要改为本局，
    //           呼叫类型原来是出局改为本局，原来是汇接的改为入局
    //           此函数实现转换为
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为2，
    //          
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int ChangeGSVNandCallType2Text(void* pCurField, 
                           const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery);
    /************ End of The ChangeGSVNandCallType Defining ******************/

	/************ The Array2HexString Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现对字节数组转换为十六进制字符串
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //          
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int Array2HexString(void* pCurField, 
                           const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery);
    /************ End of The Array2HexString Defining ******************/

	/************ The FillInvaidValue Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现对字节填充FF
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //          
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int FillInvaidValue(void* pCurField, 
                           const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery);
    /************ End of The FillInvaidValue Defining ******************/
    
    	//Added by maimaoshi at 2002-10-16
	
	//added maimaoshi,2003-03-10
	/************ The ToLRLN Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现对某字段填"\r\n"
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向数据类型为BYTE，
    //           nSrcFieldLen[0]为1
    //           
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
	static int ToLRLN(void* pCurField, const UINT4 nSrcFields, 
                       const void* pSrcFields[],
                       const UINT4 nSrcFieldLen[],
                       const SFieldInfo* pFI,
                       const void* pDefault, const UINT4 nDefaultLen,
                       int nIsQuery = 0);
	/************ End of The ToLRLN Defining ****************/

	/************ The ToLR Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现对某字段填"\r"
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向数据类型为BYTE，
    //           nSrcFieldLen[0]为1
    //           
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
	static int ToLR(void* pCurField, const UINT4 nSrcFields, 
                       const void* pSrcFields[],
                       const UINT4 nSrcFieldLen[],
                       const SFieldInfo* pFI,
                       const void* pDefault, const UINT4 nDefaultLen,
                       int nIsQuery = 0);
	/************ End of The ToLR Defining ****************/

	/************ The ToLN Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现对某字段填"\n"
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向数据类型为BYTE，
    //           nSrcFieldLen[0]为1
    //           
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
	static int ToLN(void* pCurField, const UINT4 nSrcFields, 
                   const void* pSrcFields[],
                   const UINT4 nSrcFieldLen[],
                   const SFieldInfo* pFI,
                   const void* pDefault, const UINT4 nDefaultLen,
                   int nIsQuery = 0);
	/************ End of The ToLN Defining ****************/

	/************ The StringCopy Defining ***************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现字段按照字符串拷贝
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向源数据块的内存，
    //           nSrcFieldLen[0]表示源数据块的长度
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int StringCopy(void* pCurField, const UINT4 nSrcFields,
                         const void* pSrcFields[],
                         const UINT4 nSrcFieldLen[],
                         const SFieldInfo* pFieldInfo,
                         const void* pDefault, const UINT4 nDefaultLen,
                         int nIsQuery = 0);
    /************ End of The StringCopy Defining ********************/
    
    /************ The ByteText2 Defining ****************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现将各种整型数据转换成文本
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           nSrcFieldLen[0]为1、2、4、8时分别表示pSrcFields[0]
    //           指向数据类型为BYTE、SHORT、LONG、INT8型的数据
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int ByteText2(void* pCurField, const UINT4 nSrcFields, 
                        const void* pSrcFields[],
                        const UINT4 nSrcFieldLen[],
                        const SFieldInfo* pFI,
                        const void* pDefault, const UINT4 nDefaultLen,
                        int nIsQuery = 0);
    /************ End of The ByteText2 Defining *********************/

	/************ The GetTextTimeInString Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现年、月、日、时、分、秒转换成文本格式的YYYY-MM-DD HH:NN:SS
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为6，
    //           pSrcFields[0]指向数据类型为BYTE或SHORT型的2位“年”，
    //           pSrcFields[1]、pSrcFields[2]、pSrcFields[3]、pSrcFields[4]、
    //           pSrcFields[5]分别指向BYTE型月、日、时、分、秒，
    //           如果nSrcFieldLen[0]为1，表示pSrcFields[0]为BYTE型的年，
    //           如果nSrcFieldLen[0]为2，表示pSrcFields[0]为SHORT型的年
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int GetTextTimeInString(void* pCurField, const UINT4 nSrcFields, 
                                   const void* pSrcFields[],
                                   const UINT4 nSrcFieldLen[],
                                   const SFieldInfo* pFI,
                                   const void* pDefault, const UINT4 nDefaultLen,
                                   int nIsQuery = 0);
    /************ End of The GetTextTimeInString Defining ******************/
    
    //end maimaoshi
    //by ldf 2003-04-01
    /************ The CheckSum Defining *****************************/
    // 功能描述：校验函数
    /***************************************************************/
	static int CheckSum(void* pCurField, const UINT4 nSrcFields,
		                const void* pSrcFields[], 
		                const UINT4 nSrcFieldLen[], 
		                const SFieldInfo* pFI, 
		                const void* pDefault, const UINT4 nDefaultLen,
		                int nIsQuery);
    //end
    //从中继群号到电话号码的转换.
    static int TrkGrp2CallNo(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery = 0);  
    
    /************ The FindFun Defining *****************************/
    // 功能描述：根据格式域名的转换函数，找到对应的转换函数指针
    //
    // 输入参数：
    // szConvertFun：格式域中的转换函数字符串
    //      pSrcFmt：源格式对象
    //
    // 输出参数：无
    //
    //   返回值：转换函数指针
    //
    /***************************************************************/
    static CONVERTFUN FindFun(const char* szConvertFun, 
                              const CFormat *pSrcFmt );
    /************ End of The FindFun Defining **********************/

    /************ The FillWithStr Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现对某字段填充一个常量字符串，实现对话单格式的灵活配置
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为1，
    //           pSrcFields[0]指向数据类型为BYTE，
    //           nSrcFieldLen[0]为1
    //           
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int FillWithStr(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The FillWithStr Defining ******************/
    
    /************ The FillByIniFile Defining *************************/
    // 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
    //           实现往格式库中添加一个INI文件中的指定键值
    //
    // 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
    //           需要补充的是调用此函数时nSrcFields必须为2，
    //           pSrcFields[0]指向数据类型为char，
    //           pSrcFields[1]指向数据类型为char
    //           
    //
    // 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
    //
    //   返回值：成功返回0，失败返回-1
    //
    /***************************************************************/
    static int FillByIniFile(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);    
    
    // 查找条件函数指针的函数
    static CONDITIONFUN FindConFun(const STRING& sFun);


    //获取电话号码的前n位，并转换成相应的整数,
    //该函数是CONDITIONFUN类型的工具函数
    static int Prefix(const UINT4 nParasNum, 
                      const void* pParas[],
                      const UINT4 nParasLen[], 
                      const STRING& pDefault,
                      SVariant& stVar);

    //实现从主叫号码中返回区号ID
    //该函数是CONDITIONFUN类型的工具函数
    static int GetAreaID(const UINT4 nParasNum, 
                         const void* pParas[],
                         const UINT4 nParasLen[], 
                         const STRING& pDefault,
                         SVariant& stVar);

	/************ The AddFillerSeparator Defining ******************/
    // 功能描述：为文本格式的目标格式域增加填充符和分隔符
    //
    // 输入参数：
    //   pszField：未增加填充符和分隔符的目标格式域内容
    //        pFI：该目标格式域的描述
    //
    // 输出参数：
    //   pszField：已经加填充符和分隔符后的目标格式域内容
    //
    //   返回值：成功返回0，失败返回-1
    //
    /****************************************************************/
    static int AddFillerSeparator(char* pszField, 
                                  const SFieldInfo* pFI);
    /************ End of The AddFillerSeparator Defining ************/

    //add by lucy 2005-5-25
    /************ The LinkText Defining *****************************/
    // 功能描述：用连接符连接两个字段
    /***************************************************************/
    static int LinkText(void* pCurField, const UINT4 nSrcFields,
                        const void* pSrcFields[], 
                        const UINT4 nSrcFieldLen[], 
                        const SFieldInfo* pFI, 
                        const void* pDefault, const UINT4 nDefaultLen,
                        int nIsQuery);
    /************ End of The LinkText Defining *********************/


    //Added By Lucy 2004-07-12
    /************ The GetRecordType  Defining *********************/
    // 功能描述：马来TM格式转换需求，处理流程为：
    // 1.   当原始话单中的free_indicator为0是，Record Type字段填写为04
    // 2.   否则，原始话单中的gsvn，通过枚举转换
    // 3.   支持没有取值时，用默认值代替
    // 转换举例：GetRecordType(free_ind, gsvn, "0")
    /***************************************************************/    
    static int GetRecordType(void* pCurField, const UINT4 nSrcFields,
                            const void* pSrcFields[], 
                            const UINT4 nSrcFieldLen[], 
                            const SFieldInfo* pFI, 
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery);    
    /************ End of The GetRecordType Defining ****************/
    

    //add by lucy 2004-07-09
    /************ The GetSubCategory  Defining *********************/
    // 功能描述：马来TM格式转换需求，处理流程为：
    // 1.	如果原始话单中的caller_ctx_ number字段有效，则此字段填写为03。
    // 2、	如果原始话单中的caller_did为H.323或者SIP，并且trunk_group _in
    //      不等于0xFFFF，则此字段填写为05。
    // 3、	其它情况，根据原始话单中的caller_category字段来转换，枚举取值        
    /***************************************************************/
    static int GetSubCategory(void* pCurField, const UINT4 nSrcFields,
                              const void* pSrcFields[], 
                              const UINT4 nSrcFieldLen[], 
                              const SFieldInfo* pFI, 
                              const void* pDefault, const UINT4 nDefaultLen,
                              int nIsQuery);
    /************ End of The GetSubCategory Defining ****************/
    
    //add by lucy 2004-07-09
    /************ The ISUPChargeNumber Defining *********************/
    // 功能描述：马来TM格式转换需求，处理流程为：
    //     根据原始话单中的ISUP charge number indicator字段的取值转换。
    //      1、	如果取值为0，将caller_number转换到此字段；
    //      2、	如果取值为1，将called_number转换到此字段；
    //      3、	如果取值为2，将connected_number转换到此字段。
    //      其它情况填写为空字符串。
    static int ISUPChargeNumber(void* pCurField, const UINT4 nSrcFields,
                                const void* pSrcFields[], 
                                const UINT4 nSrcFieldLen[], 
                                const SFieldInfo* pFI, 
                                const void* pDefault, const UINT4 nDefaultLen,
                                int nIsQuery); 
    /************ End of The ISUPChargeNumber Defining ****************/
    
    //add by lucy 2004-07-09
    /************ The ISUPChargeNumberNOA Defining *********************/
    // 功能描述：马来TM格式转换需求，处理流程为：
    //     根据原始话单中的ISUP charge number indicator字段的取值转换。
    //      1、	如果取值为0，将caller_address_nature转换到此字段；
    //      2、	如果取值为1，将called_address_nature转换到此字段；
    //      3、	如果取值为2，将connected_address_nature转换到此字段。
    //      其它情况填写为空字符串。
    static int ISUPChargeNumberNOA(void* pCurField, const UINT4 nSrcFields,
                                   const void* pSrcFields[], 
                                   const UINT4 nSrcFieldLen[], 
                                   const SFieldInfo* pFI, 
                                   const void* pDefault, const UINT4 nDefaultLen,
                                   int nIsQuery); 
    /************ End of The ISUPChargeNumberNOA Defining ****************/  
    
    //add by lucy 2004-08-05
    /************ The VideoTime Defining *********************/
    // 功能描述：联通三期格式转换需求，处理流程为：
    //     根据视频编解码字段判断，如果有视频编解码，则使用"应答日期时间"
    // 使用示例：
    // VideoTime(video_code_type, ans_year,ans_mon,ans_day,ans_hour,ans_min,ans_sec)
    /********************************************************/
    static int VideoTime(void* pCurField, const UINT4 nSrcFields,
                        const void* pSrcFields[], 
                        const UINT4 nSrcFieldLen[], 
                        const SFieldInfo* pFI, 
                        const void* pDefault, const UINT4 nDefaultLen,
                        int nIsQuery); 

    //add by lucy 2004-08-05
    /************ The VideoDuration Defining *********************/
    // 功能描述：联通三期格式转换需求，处理流程为：
    //     根据视频编解码字段判断，如果有视频编解码，则使用"通话时长"
    // 使用示例：
    // VideoDuration(video_code_type,conversation_time)
    /********************************************************/
    static int VideoDuration(void* pCurField, const UINT4 nSrcFields,
                        const void* pSrcFields[], 
                        const UINT4 nSrcFieldLen[], 
                        const SFieldInfo* pFI, 
                        const void* pDefault, const UINT4 nDefaultLen,
                        int nIsQuery); 
    
    //added by lucy 2004-08-16
    //实现 整型域和整数常量相加
    static int Add(void* pCurField, const UINT4 nSrcFields,
                    const void* pSrcFields[], 
                    const UINT4 nSrcFieldLen[], 
                    const SFieldInfo* pFI, 
                    const void* pDefault, const UINT4 nDefaultLen,
                    int nIsQuery);
private:

    /************ The GetParas Defining *****************************/
    // 功能描述：从转换函数配置的参数字符串中拆分出单个函数参数，
    //           参数之间用','分隔
    //
    // 输入参数：
    //     sParas：参数字符串
    //
    // 输出参数：
    //   ParaList：参数列表，按函数参数顺序排列
    //
    //   返回值：如果由于配置出错，则函数返回-1，否则返回0
    //
    /****************************************************************/
    static int GetParas(const STRING& sParas, LIST<STRING>& ParaList);
    /************ End of The GetParas Defining **********************/

    /************ The ConvertFunDiv Defining ************************/
    // 功能描述：从格式域转换函数中分离出函数名、参数列表、域缺省值
    //
    // 输入参数：
    //    sSubStr：转换函数字符串
    //
    // 输出参数：
    //       sFun：转换函数名
    //   sDefault：域缺省值
    //   ParaList：参数列表，按函数参数从左到右排列
    //
    //   返回值：如果由于配置出错，则函数返回-1，否则返回0
    //
    /****************************************************************/
    static int ConvertFunDiv(const STRING& sSubStr, STRING& sFun, 
                             STRING& sDefault, LIST<STRING>& ParaList);
    /************ End of The CConvertFunDiv Defining ****************/

    //Added by ZhengYuqun 2003-08-27 SWPD01198
    static int PadAreacode(void* pCurField, const UINT4 nSrcFields,
                         const void* pSrcFields[],
                         const UINT4 nSrcFieldLen[],
                         const SFieldInfo* pFieldInfo,
                         const void* pDefault, const UINT4 nDefaultLen,
                         int nIsQuery = 0);
    //End

    //用新的前缀替换电话号码中旧的前缀
    static int ReplacePrefix(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFieldInfo,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery);

    //added by lucy 2004-06-25 
    //功能：将原始话单的某个字段从数字替换成字符串,数字和字符串对应关系在文件中配置
    static int ReplaceNumByStr(void* pCurField, const UINT4 nSrcFields,
                               const void* pSrcFields[],
                               const UINT4 nSrcFieldLen[],
                               const SFieldInfo* pFieldInfo,
                               const void* pDefault, const UINT4 nDefaultLen,
                               int nIsQuery);


    //邱意平 2004-02-14 添加转换函数 Duration2HHmmSS(...)
    static int Duration2HHmmSS(void* pCurField, 
                               const UINT4 nSrcFields,
                               const void* pSrcFields[],
                               const UINT4 nSrcFieldLen[],
                               const SFieldInfo* pFI,
                               const void* pDefault, 
                               const UINT4 nDefaultLen,
                               int nIsQuery);

    /************ The Convert10ms Defining ************************/
    // 功能描述：将10ms为单位的时间转换成S, 或HHMMSS
    //
    // 转换HHMMSS需带格式，例如：12800, 参数"HH:MM:SS" -> 02:08:00
    //
    // 返回值：如果由于转换失败，则函数返回-1，否则返回0
    //
    // Added By Lucy 2004-07-16 
    /****************************************************************/
    static int Convert10ms(void* pCurField, 
                           const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, 
                           const UINT4 nDefaultLen,
                           int nIsQuery);
    /*************** End of Convert10ms Defining ********************/

    static int Int2TextEx(void* pCurField, 
                          const UINT4 nSrcFields, 
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, 
                          const UINT4 nDefaultLen,
                          int nIsQuery);

    //Added by ZhengYuqun SWPD03631 2004-02-18
    //MSoftX3000的话单类型转换为2G的话单类型时需要根据话单类型和中间话单原因值生成试呼话单类型
    static int CDRType2G(void* pCurField, 
        const UINT4 nSrcFields, 
        const void* pSrcFields[],
        const UINT4 nSrcFieldLen[],
        const SFieldInfo* pFI,
        const void* pDefault, 
        const UINT4 nDefaultLen,
        int nIsQuery);

    static int IsInNumSeg(const UINT4 nParasNum, 
                      const void* pParas[],
                      const UINT4 nParasLen[], 
                      const STRING& pDefault,
                      SVariant& stVar);

    static int IsInEnum(const UINT4 nParasNum, 
                        const void* pParas[],
                        const UINT4 nParasLen[], 
                        const STRING& pDefault,
                        SVariant& stVar);

   static int PadNumber(void* pCurField, 
                        const UINT4 nSrcFields, 
                        const void* pSrcFields[],
                        const UINT4 nSrcFieldLen[],
                        const SFieldInfo* pFI,
                        const void* pDefault, 
                        const UINT4 nDefaultLen,
                        int nIsQuery);
                        
    //将短消息中的主被叫号码转换为文本  Added by ZhengYuqun
    static int SMSNum2Text(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);

    static int TermCause2G(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);

    static int PeerMnc2G(void* pCurField, const UINT4 nSrcFields, 
                         const void* pSrcFields[],
                         const UINT4 nSrcFieldLen[],
                         const SFieldInfo* pFI,
                         const void* pDefault, const UINT4 nDefaultLen,
                         int nIsQuery = 0);

    static int VarLen2Text(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    
    static int VarValue2Text(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery = 0);

    //Added by ZhengYuqun SWPD06279 2004-06-26
    static int GetVarValue(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);

    static int GetVarLen(void* pCurField, const UINT4 nSrcFields, 
                        const void* pSrcFields[],
                        const UINT4 nSrcFieldLen[],
                        const SFieldInfo* pFI,
                        const void* pDefault, const UINT4 nDefaultLen,
                        int nIsQuery = 0);

    //根据号码的长度来转换BCD码  //SWPD06703 Zheng Yuqun 2004-06-19
    static int BCD2StringByLen(void* pCurField, const UINT4 nSrcFields,
                                const void* pSrcFields[],
                                const UINT4 nSrcFieldLen[],
                                const SFieldInfo* pFI,
                                const void* pDefault, const UINT4 nDefaultLen,
                                int nIsQuery = 0);

    static int GetArgSpecField(void* pCurField, const UINT4 nSrcFields, 
                                const void* pSrcFields[],
                                const UINT4 nSrcFieldLen[],
                                const SFieldInfo* pFI,
                                const void* pDefault, const UINT4 nDefaultLen,
                                int nIsQuery = 0);    
                                                   
private:

    static int m_nRefCount;	      //引用计数器，必须互斥访问
    static ACE_Mutex m_hMutex;    //ACE互斥锁，用于同步对m_nRefCount的访问

};
/****************** End of The CConvertFun class Defining ***********/

#endif  //!defined(_FORMAT_CONVERTFUN_H_)
