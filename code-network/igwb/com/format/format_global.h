#if !defined(_FORMAT_GLOBAL_FUNCTION_VARIABLE_H_)
#define _FORMAT_GLOBAL_FUNCTION_VARIABLE_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the Global Functions and Varibles
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"
#include "../include/toolbox.h"


#define MAX_OPERATOR_NUM 50

//定义格式库全局空间
namespace NS_Fmt_Global
{

/*************** The global Variable Defining ***********************/


//栈内优先级
extern SOperatorPriority g_ISP[MAX_OPERATOR_NUM];

//栈外优先级
extern SOperatorPriority g_ICP[MAX_OPERATOR_NUM];

//操作符字符串
extern STRING g_sOperator;

/*************** End of The global Variable Defining *****************/


/*************** The global Functions Defining ***********************/

//创建多级目录
//BOOL CreateDeepDir(const char* pPath);

/*************** End of The global Functions Defining ****************/

};  //End of NS_Fmt_Global

using namespace NS_Fmt_Global;

#endif  //!defined(_FORMAT_GLOBAL_FUNCTION_VARIABLE_H_)
