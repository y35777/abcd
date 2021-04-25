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
//ջ�����ȼ�
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

//ջ�����ȼ�
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

//�������ַ���
STRING g_sOperator = "(+-*/%<>=!&^|";

/*************** End of The global Variable Defining *****************/


/********************* The CreateDeepDir Defining *********************/
//�������ܣ������༶(����һ��)Ŀ¼
//
//���������
//     pPath���ļ�����ȫ·����
//
//�����������
//
//����ֵ���ɹ�����TRUE�����򷵻�FALSE
//
/**********************************************************************/
/*
BOOL CreateDeepDir(const char* pPath)
{
    char  szPath[MAX_PATH];  //·����ʱ������

    //Ŀ¼���в����ڳ��ֵ��ַ�
    const char pInvalidChars[] =
        { '\\', '/', ':', '*', '?', '"', '<', '>', '|', '\0'};

    //ȷ�����������ȷ
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
            //����ַ����Ѿ�����
            if(szPath[i] == '\0')
            {
                nStop = 1;
            }

            //���Ŀ¼���ĺϷ���
            if (strchr(pInvalidChars, szPath[i-1]))
            {
                continue;
            }

            szPath[i] = 0;    //�����ַ���������

            //�ڴ����༶Ŀ¼ʱ�����Ŀ¼�Ѵ��ڣ�����Ϊ�Ǵ���
            //����������һ��Ŀ¼���������Ŀ¼ʧ�ܣ���ʧ��ԭ��
            //������Ŀ¼�Ѵ��ڣ�����ֹ��������������FALSE��
            if ((ACE_OS::mkdir(szPath) != 0) && (errno != EEXIST))
            {
                return FALSE;
            }

            //����ַ�����û�н���
            if(nStop != 1)
            {
                szPath[i] = '/';  //�ָ��ַ���
            }
        }
    }

    return TRUE;
}
*/
/************ End of The CreateDeepDir Defining ***********************/
};
