/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the Global Functions and Variables
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/

#include "format_global.h"


/*************** The global Variable Defining ***********************/

namespace NS_Fmt_Global
{
//栈内优先级
SOperatorPriority g_ISP[MAX_OPERATOR_NUM] = 
{ 
        { '#',  '#',  0}, 
        { '(',  '#',  1}, 
        { '!', '#',  23},
        { '*',  '#',  21}, 
        { '/',  '#',  21}, 
        { '%', '#', 21}, 
        { '+',  '#', 19}, 
        { '-', '#', 19}, 
        { '<', '<', 17 }, 
        { '>', '>', 17 }, 
        { '<', '#', 15 }, 
        { '<', '=', 15 }, 
        { '>', '#', 15 }, 
        { '>', '=', 15 }, 
        { '=', '=', 13 }, 
        { '!', '=', 13 }, 
        { '&', '#', 11 }, 
        { '^', '#', 9 }, 
        { '|', '#', 7 }, 
        { '&', '&', 5 }, 
        { '|', '|', 3 }, 
        { ')', '#', 24 } 
};

//栈外优先级
SOperatorPriority g_ICP[MAX_OPERATOR_NUM] = 
{ 
        { '#', '#', 0 }, 
        { '(', '#', 24 },    
        { '!', '#', 22 },
        { '*', '#', 20 }, 
        { '/', '#', 20 }, 
        { '%', '#', 20 }, 
        { '+', '#', 18 }, 
        { '-', '#', 18 }, 
        { '<', '<', 16 }, 
        { '>', '>', 16 }, 
        { '<', '#', 14 }, 
        { '<', '=', 14 }, 
        { '>', '#', 14 }, 
        { '>', '=', 14 }, 
        { '=', '=', 12 }, 
        { '!', '=', 12 }, 
        { '&', '#', 10 }, 
        { '^', '#', 8 }, 
        { '|', '#', 6 }, 
        { '&', '&', 4 }, 
        { '|', '|', 2 }, 
        { ')', '#', 1 } 
};

//操作符字符串
STRING g_sOperator = "(+-*/%<>=!&^|";

/*************** End of The global Variable Defining *****************/


/********************* The CreateDeepDir Defining *********************/
//函数功能：创建多级(包括一级)目录
//
//输入参数：
//     pPath：文件名（全路径）
//
//输出参数：无
//
//返回值：成功返回TRUE，否则返回FALSE
//
/**********************************************************************/
/*
BOOL CreateDeepDir(const char* pPath)
{
    char  szPath[MAX_PATH];  //路径临时缓冲区

    //目录名中部用于出现的字符
    const char pInvalidChars[] =
        { '\\', '/', ':', '*', '?', '"', '<', '>', '|', '\0'};

    //确保输入参数正确
    if((pPath == NULL) || (strlen(pPath) == 0))
    {
        return FALSE;
    }

    strcpy(szPath, pPath);
    int nLen = strlen(szPath);
    int nStop = 0;

    for (int i = 0; i <= nLen; i++)
    {
        if (((szPath[i] == '\\') || (szPath[i] == '/') || (szPath[i] == '\0'))
            && (i > 0))
        {
            //如果字符串已经结束
            if(szPath[i] == '\0')
            {
                nStop = 1;
            }

            //检查目录名的合法性
            if (strchr(pInvalidChars, szPath[i-1]))
            {
                continue;
            }

            szPath[i] = 0;    //设置字符串结束符

            //在创建多级目录时，如果目录已存在，则不认为是错误，
            //继续创建下一级目录；如果创建目录失败，但失败原因
            //并不是目录已存在，则终止创建，函数返回FALSE。
            if ((ACE_OS::mkdir(szPath) != 0) && (errno != EEXIST))
            {
                return FALSE;
            }

            //如果字符串还没有结束
            if(nStop != 1)
            {
                szPath[i] = '/';  //恢复字符串
            }
        }
    }

    return TRUE;
}
*/
/************ End of The CreateDeepDir Defining ***********************/
};
