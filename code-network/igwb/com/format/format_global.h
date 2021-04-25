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

//�����ʽ��ȫ�ֿռ�
namespace NS_Fmt_Global
{

/*************** The global Variable Defining ***********************/


//ջ�����ȼ�
extern SOperatorPriority g_ISP[MAX_OPERATOR_NUM];

//ջ�����ȼ�
extern SOperatorPriority g_ICP[MAX_OPERATOR_NUM];

//�������ַ���
extern STRING g_sOperator;

/*************** End of The global Variable Defining *****************/


/*************** The global Functions Defining ***********************/

//�����༶Ŀ¼
//BOOL CreateDeepDir(const char* pPath);

/*************** End of The global Functions Defining ****************/

};  //End of NS_Fmt_Global

using namespace NS_Fmt_Global;

#endif  //!defined(_FORMAT_GLOBAL_FUNCTION_VARIABLE_H_)
