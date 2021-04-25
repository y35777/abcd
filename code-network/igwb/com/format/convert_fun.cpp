/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CConvertFun
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/

#include "format.h"
#include "format_global.h"
#include "ace/INET_Addr.h"
#include "utility_ex.h"
#include "../include/toolbox.h"
#include "convert_fun.h"

//��ʼ����̬��Ա����
int CConvertFun::m_nRefCount = 0;
ACE_Mutex CConvertFun::m_hMutex;


/************ The AddRef Defining ******************************/
// �������������ü�������1
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
void CConvertFun::AddRef(void)
{
    m_hMutex.acquire();
    m_nRefCount++;
    m_hMutex.release();
}
/************ End of The AddRef Defining ***********************/


/************ The ReleaseRef Defining **************************/
// �������������ü�������1
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
void CConvertFun::ReleaseRef(void)
{
    m_hMutex.acquire();
    m_nRefCount--;
    m_hMutex.release();
}
/************ End of The ReleaseRef Defining *******************/


/************ The ConvertAnalysis Defining *********************/
// �����������Ӹ�ʽ��ת�������з�����������������������б���ȱʡֵ
//
// ���������
// sConvertFun��ת����������ӦSFieldInfo�е�szConvertFun
//
// ���������
//        sLib�������������ʹ�ö�̬���ӿ⣬�򷵻ص�sLibΪ���ַ�����
//        sFun��ת��������
//    sDefault����ȱʡֵ
//    ParaList�������б���������������������
//
//   ����ֵ���������sConvertFun���ó�����������-1�����򷵻�0
//
/***************************************************************/
int CConvertFun::ConvertAnalysis(const STRING& sConvertFun, STRING& sLib,
                                 STRING& sFun, STRING& sDefault,
                                 LIST<STRING>& ParaList)
{
    int     nStart = 0;               //�����ַ����е�һ��'('��λ��
    int     nEnd = 0;                 //�����ַ��������һ��')'��λ��
    int     nLib = 0;                 //�����ַ�����default��λ��
    STRING  sTmpStr;                  //�����ַ�����ʱ�������
    char    szTmpPara[MAX_PARA_LEN];  //һ����������ʱ������
    char*   pTmp = NULL;              //������ʱ�������ĵ�ǰλ��
    int     i = 0;                    //��ʱ����������


    //��ʼ���������
    sLib = "";
    sFun = "";
    sDefault = "";
    ParaList.clear();

    //������������ַ����ĳ��ȵ�����
    if(sConvertFun.length() == 0)
    {
        return -1;
    }

    //��ȡ�����ַ����е���������λ��
    nStart = sConvertFun.find_first_of('(');
    nEnd = sConvertFun.find_last_of(')');

    //���ͬʱ�����������ţ����ʾ�����ĺ����ַ���
    if((nStart != -1) && (nEnd != -1))
    {
        //����"DllLoad"λ��
        nLib = sConvertFun.find(VAR_DLL_LOAD);

        //���"DllLoad"���ڣ����ʾ�����ж�̬���ӿ���
        if(nLib != -1)
        {
            //��ȡ��̬���ӿ���
            pTmp = szTmpPara;  //��ʼ����̬���ӿ�����������ǰλ��
            i = nStart + 1;    //�ӵ�һ��������һ��λ�ÿ�ʼ

            //�����ǰ�ַ����ڶ��ţ���ʾ��������
            while((sConvertFun[i] != ',') && (sConvertFun[i] != '\0'))
            {
                //���Կո�
                if(sConvertFun[i] == ' ')
                {
                    i++;
                    continue;
                }

                *pTmp++ = sConvertFun[i++];
            }
            *pTmp = '\0';
            sLib = szTmpPara;
            if(sConvertFun[i] == '\0')
            {
                return -1;
            }

            //��ȡ�����ַ������Ӷ��ŵ���һ��λ�ÿ�ʼ�������һ������֮ǰ����
            int nTmp = nEnd - i -1;
            if(nTmp < 0)
            {
                nTmp = 0;
            }
            sTmpStr = sConvertFun.substr(i+1, nTmp);

            //�Ѻ����ַ����ֽ�ɺ�������ȱʡֵ�������б�
            if(ConvertFunDiv(sTmpStr, sFun, sDefault, ParaList) != 0)
            {
                return -1;
            }
        }

        //���"DllLoad"�����ڣ����ʾû�������ж�̬���ӿ���
        else
        {
            //�Ѻ����ַ����ֽ�ɺ�������ȱʡֵ�������б�
            if(ConvertFunDiv(sConvertFun, sFun, sDefault, ParaList) != 0)
            {
                return -1;
            }
        }
    }

    //����������Ŷ������ڣ��������ַ�����ʾһ������
    else if((nStart == -1) && (nEnd == -1))
    {
        ParaList.push_back(sConvertFun);
    }

    //������������ַ�����ֻ���������Ż�������
    else
    {
        return -1;
    }

    return 0;
}
/************ End of The ConvertAnalysis Defining **************/


/************ The BlockCopy Defining ***************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ�����ݿ�Ŀ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           pSrcFields[0]ָ��Դ���ݿ���ڴ棬
//           nSrcFieldLen[0]��ʾԴ���ݿ�ĳ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::BlockCopy(void* pCurField, const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_BLOCK_COPY, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //��ԭ��ʽ��Ӧ���е��������ݿ�����Ŀ���ʽ��ǰ����
    memcpy(pCurField, pSrcFields[0], nSrcFieldLen[0]);

    return 0;
}
/************ End of The BlockCopy Defining ********************/


/************ The BlockCopy Defining ***************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ�����ݿ�Ŀ���
//           �߱�BlockCopy�����й��ܣ�����֧��Intel/�����ֽ���ͬ���������Ļ��࿽��
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           pSrcFields[0]ָ��Դ���ݿ���ڴ棬
//           nSrcFieldLen[0]��ʾԴ���ݿ�ĳ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::BlockCopyEx(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFieldInfo,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
 {
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_BLOCK_COPY, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }
    
    if(pFieldInfo->nFieldLength == 1)
    {
        INT8 nTemp = 0;
        
        //����Ҫ�任������ת��ΪINT8���м����
        switch (nSrcFieldLen[0])
        {
        case LEN_BYTE:
            nTemp = *(BYTE*)pSrcFields[0];
            break;
            
        case LEN_INT2:
            nTemp = *(UINT2*)pSrcFields[0];
            break;
            
        case LEN_INT4:
            nTemp = *(UINT4*)pSrcFields[0];
            break;
                
        case LEN_INT8:
            nTemp = *(INT8*)pSrcFields[0];
            break;
                    
        default:
            memcpy(pCurField, pSrcFields[0], nSrcFieldLen[0]);
            return 0;
        }
        
        //��INT8���м������ת��Ϊ��Ҫ����������
        switch (pFieldInfo->nDataType)
        {
        case FT_BYTE:
            *(BYTE*)pCurField = nTemp;
            break;
            
        case FT_SHORT:
        case FT_USHORT:
            *(UINT2*)pCurField = nTemp;
            break;
            
        case FT_LONG:
        case FT_ULONG:
            *(UINT4*)pCurField = nTemp;
            break;
            
        case FT_INT8:
            *(INT8*)pCurField = nTemp;
            break;
            
        default:
            memcpy(pCurField, pSrcFields[0], nSrcFieldLen[0]);
            break;
        }
        
        return 0;
    }
    
    //��ԭ��ʽ��Ӧ���е��������ݿ�����Ŀ���ʽ��ǰ����
    memcpy(pCurField, pSrcFields[0], nSrcFieldLen[0]);
    
    return 0;
}
/************ End of The BlockCopy Defining ********************/

/************ The GetBinYear Defining **************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ�ֽ�BYTE�ͻ���SHORT�͵���λ��ת���ɶ�������λ��
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           pSrcFields[0]ָ����������ΪBYTE��SHORT�͵�2λ���ꡱ��
//           ���nSrcFieldLen[0]Ϊ1����ʾpSrcFields[0]ΪBYTE�͵��꣬
//           ���nSrcFieldLen[0]Ϊ2����ʾpSrcFields[0]ΪSHORT�͵���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::GetBinYear(void* pCurField, const UINT4 nSrcFields,
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_BIN_YEAR, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    INT2 nTmp = 0;    //�����λ�����ʱ����

    //�����Ϊ��ΪSHORT����
    if(nSrcFieldLen[0] == LEN_INT2)
    {
        //nTmp = *(INT2*)pSrcFields[0];
        memcpy((void*)&nTmp, pSrcFields[0], LEN_INT2);

        //CC08�������ж������ֽڵ�2λ�꣬��ݱ����ڸ��ֽڣ�
        //������Ҫ�Ƶ����ֽ�
        nTmp = nTmp >> 8;
    }

    //�����Ϊ��ΪBYTE����
    else if(nSrcFieldLen[0] == LEN_BYTE)
    {
        //nTmp = *(BYTE*)pSrcFields[0];
        BYTE yTmp = *(BYTE*)pSrcFields[0];
        nTmp = yTmp;
    }

    //�Ƿ�����
    else
    {
        return -1;
    }

    nTmp += 2000;
    //*(INT2*)pCurField = nTmp;
    memcpy(pCurField, &nTmp, LEN_INT2);

    return 0;
}
/************ End of The GetBinYear Defining *******************/


/************ The GetTextYear Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ�ֽ�BYTE��SHORT�͵���λ��ת�����ı���ʽ����λ��
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           pSrcFields[0]ָ����������ΪBYTE��SHORT�͵�2λ���ꡱ��
//           ���nSrcFieldLen[0]Ϊ1����ʾpSrcFields[0]ΪBYTE�͵��꣬
//           ���nSrcFieldLen[0]Ϊ2����ʾpSrcFields[0]ΪSHORT�͵���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::GetTextYear(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_TEXT_YEAR, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    INT2 nTmp = 0;    //�����λ�����ʱ����

    //�����Ϊ��ΪSHORT����
    if(nSrcFieldLen[0] == LEN_INT2)
    {
        //nTmp = *(INT2*)pSrcFields[0];
        memcpy((void*)&nTmp, pSrcFields[0], LEN_INT2);

        //CC08�������ж������ֽڵ�2λ�꣬��ݱ����ڸ��ֽڣ�
        //������Ҫ�Ƶ����ֽ�
        nTmp = nTmp >> 8;
    }

    //�����Ϊ��ΪBYTE����
    else if(nSrcFieldLen[0] == LEN_BYTE)
    {
        //nTmp = *(BYTE*)pSrcFields[0];
        BYTE yTmp = *(BYTE*)pSrcFields[0];
        nTmp = yTmp;
    }

    //�Ƿ�����
    else
    {
        return -1;
    }

    if(nTmp == 0xFF)
    {
        ((char*)pCurField)[0] = '\0';
    }
    else
    {
        //����Ϊ�����2000��������Ŀ���ʽ��ǰ��
        nTmp += 2000;

        //����λ��ת������Ӧ���ַ���
        sprintf((char*)pCurField, "%4d", nTmp);
    }

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The GetTextYear Defining ******************/


/************ The GetTextTime Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ���ꡢ�¡��ա�ʱ���֡���ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS.MS
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ6
//           pSrcFields[0]ָ����������ΪBYTE��SHORT�͵�2λ���ꡱ��
//           pSrcFields[1]��pSrcFields[2]��pSrcFields[3]��pSrcFields[4]��
//           pSrcFields[5]�ֱ�ָ��BYTE���¡��ա�ʱ���֡��룬
//           ���nSrcFieldLen[0]Ϊ1����ʾpSrcFields[0]ΪBYTE�͵��꣬
//           ���nSrcFieldLen[0]Ϊ2����ʾpSrcFields[0]ΪSHORT�͵���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::GetTextTime(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    INT2  nYear =  0;    //�����λ�����ʱ����
    BYTE  nMon =   0;    //����µ���ʱ����
    BYTE  nDate =  0;    //����յ���ʱ����
    BYTE  nHour =  0;    //���ʱ����ʱ����
    BYTE  nMin =   0;    //��ŷֵ���ʱ����
    BYTE  nSec =   0;    //��������ʱ����
    BYTE  nMilliSec = 0; //��ź������ʱ����

    //ȷ�����������ȷ
    if((pCurField == NULL) || (nSrcFields != 6 && nSrcFields != 7) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //�����Ϊ��ΪSHORT����
    if(nSrcFieldLen[0] == LEN_INT2)
    {
        //nYear = *(INT2*)pSrcFields[0];
        memcpy((void*)&nYear, pSrcFields[0], LEN_INT2);

        //CC08�������ж������ֽڵ�2λ�꣬��ݱ����ڸ��ֽڣ�
        //������Ҫ�Ƶ����ֽ�
        nYear = nYear >> 8;
    }

    //�����Ϊ��ΪBYTE����
    else if(nSrcFieldLen[0] == LEN_BYTE)
    {
        //nYear = *(BYTE*)pSrcFields[0];
        BYTE yTmp = *(BYTE*)pSrcFields[0];
        nYear = yTmp;
    }

    //�Ƿ�����
    else
    {
        return -1;
    }

    //����Ϊ�����2000��������Ŀ���ʽ��ǰ��
    nYear += 2000;

    //��ȡ�¡��ա�ʱ���֡����ֵ
    nMon   =  *(BYTE*)pSrcFields[1];
    nDate  =  *(BYTE*)pSrcFields[2];
    nHour  =  *(BYTE*)pSrcFields[3];
    nMin   =  *(BYTE*)pSrcFields[4];
    nSec   =  *(BYTE*)pSrcFields[5];

    //Added by ZhengYuqun SWPD02506 2003-11-24
    if((nYear - 2000) == 0xFF || nMon == 0xFF || nDate == 0xFF || nHour == 0xFF || nMin == 0xFF || nSec == 0xFF)
    {
        ((char*)pCurField)[0] = '\0';
    } //End SWPD02506
    else if(nSrcFields == 7)
    {
        nMilliSec = *(BYTE*)pSrcFields[6];
        //�����͵��ꡢ�¡��ա�ʱ���֡���ת����"YYYY-MM-DD HH:NN:SS"���ַ���
        sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d.%d",
            nYear, nMon, nDate, nHour, nMin, nSec, nMilliSec);
    }
    else
    {
        //�����͵��ꡢ�¡��ա�ʱ���֡���ת����"YYYY-MM-DD HH:NN:SS"���ַ���
        sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d",
                nYear, nMon, nDate, nHour, nMin, nSec);
    }

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The GetTextTime Defining ******************/


/************ The Int2Text Defining ****************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ�ֽ�������������ת�����ı�
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           nSrcFieldLen[0]Ϊ1��2��4��8ʱ�ֱ��ʾpSrcFields[0]
//           ָ����������ΪBYTE��SHORT��LONG��INT8�͵�����
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
//     ע�⣺��ʱ��֧��INT8����ת��
/***************************************************************/
int CConvertFun::Int2Text(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    INT4  nTmp = 0;    //���BYTE��SHORT��LONG����ʱ����
    //INT8  nNum8 = 0;   //���INT8����ʱ����


    //ȷ�����������ȷ
    if(!(nSrcFields == 1 || nSrcFields == 2))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_INT_TO_TEXT, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //by lx SWPD03800
    int nRadix = 10; //ȱʡ10����
    if(nSrcFields == 2 && pSrcFields[1] != NULL)
    {
        char cRadix;
        memcpy(&cRadix, pSrcFields[1], 1);
        if (cRadix == 'H')
        {
            nRadix = 16;
        }
    }
    //end by lx

    //��ȡԭ��ʽ�������ֵ
    if(nSrcFieldLen[0] == LEN_BYTE)
    {
        //nTmp = *(BYTE*)pSrcFields[0];
        BYTE uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_BYTE);
        nTmp = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT2)
    {
        //nTmp = *(INT2*)pSrcFields[0];
        UINT2 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT2);
        nTmp = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT4)
    {
        //nTmp = *(INT4*)pSrcFields[0];
        UINT4 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT4);
        nTmp = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_INT8)
    {
        //nTmp = *(INT4*)pSrcFields[0];
        INT8 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_INT8);

        #ifdef _PLATFORM_WIN32
        sprintf((char*)pCurField, "%I64u", uTmp);
        #else
        sprintf((char*)pCurField, "%llu", uTmp);
        #endif

        //��Ŀ���ʽ�����������ͷָ���
        if(!nIsQuery)
        {
            //�������Ϊ��ѯ����ת��
            return AddFillerSeparator((char*)pCurField, pFI);
        }
        else
        {
            return 0;
        }

    }
    else
    {
        return -1;
    }
    
    //by lx SWPD03800
    if(nRadix == 16)
    {
        char szFmt[10] = "";
        sprintf(szFmt, "%%0%dx", nSrcFieldLen[0] * 2);  //ת���������ַ����������ֶ��ֽ�����2����ǰ�油0
        sprintf((char*)pCurField, szFmt, nTmp);
    }
    else
    {    
    //������ֵת������Ӧ���ַ���
/* Begin:�޸Ĳ������޷����������     Modifier: Gan Zhiliang   Date: 2003-4-22*/
    //sprintf((char*)pCurField, "%d", nTmp);
        sprintf((char*)pCurField, "%u", nTmp);
    }
    //end by lx SWPD03800
/* End  : Modifier: Gan Zhiliang   Date: 2003-4-22*/

    //�����ǰ����ö������
    INT4 nEnumVal = 0;
    ENUMINFO_LIST::iterator item;       //ö�����������
    ENUMINFO_LIST* pEnumInfos = NULL;   //��ʽ��ö������ָ�����
    if(pFI->flgEnum != 0)
    {
        StringLRTrim((char*)pCurField);
        nEnumVal = atol((char*)pCurField);

        //��ȡ��ʽ��ö������
        pEnumInfos = (ENUMINFO_LIST*)(pFI->pEnumInfos);
        if(pEnumInfos == NULL)
        {
            return -1;
        }

        //˳��������ʽ��ö�������е�ÿһ���ڵ�
        item = pEnumInfos->begin();
        while(item != pEnumInfos->end())
        {
            //�����ǰ�ڵ��ö��ֵ������Ҫ��ö��ֵ
            if(item->nEnumValue == nEnumVal)
            {
                //��ȡ��ǰö��ֵ��Ӧ��ö���ַ�����Ϣ
                strcpy((char*)pCurField, item->szEnumValue);
                break;
            }

            item++;
        }

    }

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The Int2Text Defining *********************/

//add by zkq 2004-09-28
/************ The BCD2RBCD Defining **************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           1.������BCD��ת������BCD�룬
//           ת��ʱ�������������ΪBCD���������
//           1��BCD�����0xF��2���ﵽBCD���������󳤶�
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����//          
//           pSrcFields[0]ָ������BCD�����飬 
//           nSrcFieldLen[0]ָʾ������BCD������ĳ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun:: BCD2RBCD(void* pCurField, const UINT4 nSrcFields, 
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    int    nBcdIndex = 0;      //BCD����������
    BYTE*  pBcdArray = NULL;   //BCD������
    BYTE*  pCurStr   = NULL;   //����BCD���ַ���ָ��


    //ȷ�����������ȷ
    if((pCurField == NULL) || (nSrcFields != 1 ) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    pBcdArray = (BYTE *)pSrcFields[0];
   
    //��ʼ��BCD������ָ��
    pCurStr = (BYTE*)pCurField;   
    for(;nBcdIndex  < nSrcFieldLen[0]; nBcdIndex ++)
    {
        pCurStr[nBcdIndex] = ((pBcdArray[nBcdIndex] << 4) | (pBcdArray[nBcdIndex] >> 4));
    }
    pCurStr[nBcdIndex + 1] = 0xFF;    

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��        
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;

}
/************ End of The BCD2String Defining *******************/
//end 

/************ The BCD2String Defining **************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           1.������BCD��ת�����ַ�����
//           modify by zkq 2004-05-27
//           2.������BCD����B,Cת�����ַ�'*','#'��
//           add end
//           ת��ʱ�������������ΪBCD���������
//           1��BCD�����0xF��2���ﵽBCD���������󳤶�
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           pSrcFields[0]ָ������BCD�����飬
//           nSrcFieldLen[0]ָʾ������BCD������ĳ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::BCD2String(void* pCurField, const UINT4 nSrcFields,
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    int    nIndex    = 0;      //�ı���ʽ��BCD���ַ�������
    int    nBcdIndex = 0;      //BCD����������
    BYTE   yCurBcd   = 0;      //��ǰBCD��ֵ
    BYTE*  pBcdArray = NULL;   //BCD������ָ��
    char*  pCurStr   = NULL;   //�ı���ʽ��BCD���ַ���ָ��



    //ȷ�����������ȷ
    if(nSrcFields != 1 && nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_INVALID_PARA_NUM,
              FUN_BCD_TO_STR, nSrcFields);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //��װBCD��
    BYTE pNumber[40];
    int  nSrcNumLen = 0;
    if(nSrcFields == 1)
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];
    }
    else
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];

        memcpy(pNumber + nSrcNumLen, pSrcFields[1], nSrcFieldLen[1]);
        nSrcNumLen += nSrcFieldLen[1];        
    }

    //��ʼ��BCD������ָ����ı���ʽ��BCD���ַ���ָ��
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pNumber;

    //˳��ɨ��BCD�������ÿһ��Ԫ��
    while(nBcdIndex < nSrcNumLen)
    {
        //��ȡ��ǰBCD���ֵ
        yCurBcd = pBcdArray[nBcdIndex++];

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0xF0)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
				//add by zkq 2004-05-27
				if((yCurBcd & 0xF0) == 0xB0)
                {
                   pCurStr[nIndex++] = '*';
                }
				else if((yCurBcd & 0xF0) == 0xC0)
                {
                   pCurStr[nIndex++] = '#';
                }//add end 
				else
				{
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
				}
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }

        //���ε�ǰBCD������ֵ�ĸ���λ
        yCurBcd &= 0x0F;

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0x0F)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
				//add by zkq 2004-05-27
				if(yCurBcd == 0x0B)
                {
                   pCurStr[nIndex++] = '*';
                }
				else if(yCurBcd == 0x0C)
                {
                   pCurStr[nIndex++] = '#';
                }//add end
				else
				{
                pCurStr[nIndex++] = yCurBcd - 10 + 'A';
				}
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }
    }

    //�����ַ����Ľ�����
    pCurStr[nIndex] = '\0';

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The BCD2String Defining *******************/

/************ The BCD2String2 Defining **************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ��������(��һ����Ϊn���ֽڣ��ڶ�����Ϊ0.5���ֽ�)������BCD��ת�����ַ�����
//           ת��ʱ�������������ΪBCD���������
//           1��BCD�����0xF��2���ﵽBCD���������󳤶�
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           pSrcFields[0]ָ������BCD�����飬
//           nSrcFieldLen[0]ָʾ������BCD������ĳ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::BCD2String2(void* pCurField, const UINT4 nSrcFields,
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    int    nIndex    = 0;      //�ı���ʽ��BCD���ַ�������
    int    nBcdIndex = 0;      //BCD����������
    BYTE   yCurBcd   = 0;      //��ǰBCD��ֵ
    BYTE*  pBcdArray = NULL;   //BCD������ָ��
    char*  pCurStr   = NULL;   //�ı���ʽ��BCD���ַ���ָ��


    //ȷ�����������ȷ
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_BCD_TO_STR_2, 2);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0)
       || (pSrcFields[1] == NULL) || (nSrcFieldLen[1] == 0))
    {
        return -1;
    }

    //��ʼ��BCD������ָ����ı���ʽ��BCD���ַ���ָ��
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    //˳��ɨ��BCD�������ÿһ��Ԫ��
    while(nBcdIndex < nSrcFieldLen[0])
    {
        //��ȡ��ǰBCD���ֵ
        yCurBcd = pBcdArray[nBcdIndex++];

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0xF0)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }

        //���ε�ǰBCD������ֵ�ĸ���λ
        yCurBcd &= 0x0F;

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0x0F)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = yCurBcd - 10 + 'A';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }
    }

    yCurBcd = *(BYTE*)pSrcFields[0];
    //�����ǰBCD��ֵС��10
    if(yCurBcd < 0x0A)
    {
        pCurStr[nIndex++] = yCurBcd + '0';
    }

    //�����ǰBCD��ֵ����10
    else
    {
        pCurStr[nIndex++] = yCurBcd - 10 + 'A';
    }

    //�����ַ����Ľ�����
    pCurStr[nIndex] = '\0';

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The BCD2String2 Defining *******************/

/************ The RBCD2String Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ������BCD��ת�����ַ�����
//           ת��ʱ�������������ΪBCD���������
//           1��BCD�����0xF��2���ﵽBCD���������󳤶�
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           pSrcFields[0]ָ����BCD�����飬
//           nSrcFieldLen[0]ָʾ�÷���BCD������ĳ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::RBCD2String(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    int    nIndex    = 0;      //�ı���ʽ��BCD���ַ�������
    int    nBcdIndex = 0;      //BCD����������
    BYTE   yCurBcd   = 0;      //��ǰBCD��ֵ
    BYTE   yTmpBcd   = 0;      //��ʱBCD��ֵ
    BYTE*  pBcdArray = NULL;   //BCD������ָ��
    char*  pCurStr   = NULL;   //�ı���ʽ��BCD���ַ���ָ��


    //ȷ�����������ȷ
    if(nSrcFields != 1 && nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_INVALID_PARA_NUM,
              FUN_RBCD_TO_STR, nSrcFields);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }
    
    //��װRBCD��
    BYTE pNumber[40];
    int  nSrcNumLen = 0;
    if(nSrcFields == 1)
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];
    }
    else
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];

        memcpy(pNumber + nSrcNumLen, pSrcFields[1], nSrcFieldLen[1]);
        nSrcNumLen += nSrcFieldLen[1];        
    }



    //��ʼ��BCD������ָ����ı���ʽ��BCD���ַ���ָ��
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pNumber;

    //˳��ɨ��BCD�������ÿһ��Ԫ��
    while(nBcdIndex < nSrcNumLen)
    {
        //��ȡ��ǰBCD���ֵ
        yCurBcd = pBcdArray[nBcdIndex++];

        //���ε�ǰBCD������ֵ�ĸ���λ
        yTmpBcd = yCurBcd & 0x0F;

        //�������λ��������BCD��ֵ����С��16
        if(yTmpBcd < 0x0F)
        {
            //�����ǰBCD��ֵС��10
            if(yTmpBcd < 0x0A)
            {
                pCurStr[nIndex++] = yTmpBcd + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = yTmpBcd - 10 + 'A';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0xF0)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }
    }

    //�����ַ����Ľ�����
    pCurStr[nIndex] = '\0';

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The RBCD2String Defining ******************/

/************ The SBCD2String Defining **************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ������BCD��ת�����ַ�����
//           ת��ʱ�������������ΪBCD���������
//           1��BCD����ڵ���0xA��2���ﵽBCD���������󳤶�
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           pSrcFields[0]ָ������BCD�����飬
//           nSrcFieldLen[0]ָʾ������BCD������ĳ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::SBCD2String(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    int    nIndex    = 0;      //�ı���ʽ��BCD���ַ�������
    int    nBcdIndex = 0;      //BCD����������
    BYTE   yCurBcd   = 0;      //��ǰBCD��ֵ
    BYTE*  pBcdArray = NULL;   //BCD������ָ��
    char*  pCurStr   = NULL;   //�ı���ʽ��BCD���ַ���ָ��


    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_SBCD_TO_STR, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //��ʼ��SBCD������ָ����ı���ʽ��SBCD���ַ���ָ��
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    bool bPrefixZero = true;
    //˳��ɨ��SBCD�������ÿһ��Ԫ��
    while(nBcdIndex < nSrcFieldLen[0])
    {
        //��ȡ��ǰSBCD���ֵ
        yCurBcd = pBcdArray[nBcdIndex++];

        if((yCurBcd >> 4) == 0x0 && bPrefixZero)
        {
            //��������ȥ��ǰ���0
        }
        //�������λ��������BCD��ֵ����С��10
        else if(yCurBcd < 0xA0)
        {
            //�����ǰBCD��ֵС��10
            pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            bPrefixZero = false;
        }
        //����SBCD���������λ��
        else
        {
            break;
        }

        //���ε�ǰSBCD������ֵ�ĸ���λ
        yCurBcd &= 0x0F;

        if(yCurBcd == 0x0 && bPrefixZero)
        {
            //��������ȥ��ǰ���0
        }
        //�������λ��������BCD��ֵ����С��16
        else if(yCurBcd < 0x0A)
        {
            //�����ǰBCD��ֵС��10
            pCurStr[nIndex++] = yCurBcd + '0';
            bPrefixZero = false;
        }
        //����BCD���������λ��
        else
        {
            break;
        }
    }

    //�����ַ����Ľ�����
    if(nIndex == 0)
    {
        pCurStr[nIndex++] = '0';
    }
    pCurStr[nIndex] = '\0';

    //�����ǰ����ö������
    INT4 nEnumVal = 0;
    ENUMINFO_LIST::iterator item;       //ö�����������
    ENUMINFO_LIST* pEnumInfos = NULL;   //��ʽ��ö������ָ�����
    if(pFI->flgEnum != 0)
    {
        StringLRTrim((char*)pCurStr);
        nEnumVal = atol((char*)pCurStr);

        //��ȡ��ʽ��ö������
        pEnumInfos = (ENUMINFO_LIST*)(pFI->pEnumInfos);
        if(pEnumInfos == NULL)
        {
            return -1;
        }

        //˳��������ʽ��ö�������е�ÿһ���ڵ�
        item = pEnumInfos->begin();
        while(item != pEnumInfos->end())
        {
            //�����ǰ�ڵ��ö��ֵ������Ҫ��ö��ֵ
            if(item->nEnumValue == nEnumVal)
            {
                //��ȡ��ǰö��ֵ��Ӧ��ö���ַ�����Ϣ
                strcpy((char*)pCurStr, item->szEnumValue);
                break;
            }

            item++;
        }

    }

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}

/************ The ZXRBCD2String Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ������BCD��ת�����ַ�����
//           ת��ʱ�������������ΪBCD���������
//           1��BCD�����0xF��2���ﵽBCD���������󳤶�
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           pSrcFields[0]ָ����BCD�����飬
//           nSrcFieldLen[0]ָʾ�÷���BCD������ĳ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::ZXRBCD2String(void* pCurField, const UINT4 nSrcFields,
                               const void* pSrcFields[],
                               const UINT4 nSrcFieldLen[],
                               const SFieldInfo* pFI,
                               const void* pDefault, const UINT4 nDefaultLen,
                               int nIsQuery)
{
    int    nIndex    = 0;      //�ı���ʽ��BCD���ַ�������
    int    nBcdIndex = 0;      //BCD����������
    BYTE   yCurBcd   = 0;      //��ǰBCD��ֵ
    BYTE   yTmpBcd   = 0;      //��ʱBCD��ֵ
    BYTE*  pBcdArray = NULL;   //BCD������ָ��
    char*  pCurStr   = NULL;   //�ı���ʽ��BCD���ַ���ָ��


    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_ZXRBCD_TO_STR, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //��ʼ��BCD������ָ����ı���ʽ��BCD���ַ���ָ��
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    //˳��ɨ��BCD�������ÿһ��Ԫ��
    while(nBcdIndex < nSrcFieldLen[0])
    {
        //��ȡ��ǰBCD���ֵ
        yCurBcd = pBcdArray[nBcdIndex++];

        //���ε�ǰBCD������ֵ�ĸ���λ
        yTmpBcd = yCurBcd & 0x0F;

        //�������λ��������BCD��ֵ����С��16
        if(yTmpBcd < 0x0F)
        {
            //�����ǰBCD��ֵС��10
            if(yTmpBcd < 0x0A)
            {
                pCurStr[nIndex++] = yTmpBcd + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = yTmpBcd - 10 + '0';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0xF0)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + '0';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }
    }

    //�����ַ����Ľ�����
    pCurStr[nIndex] = '\0';

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The ZXRBCD2String Defining ******************/

/************ The GetEnumString Defining ***********************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ����һ����ʽ���ö��ֵ������Ӧ��ö���ַ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           pSrcFields[0]ָ��ö���͵ĸ�ʽ���ȡֵ
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::GetEnumString(void* pCurField, const UINT4 nSrcFields,
                               const void* pSrcFields[],
                               const UINT4 nSrcFieldLen[],
                               const SFieldInfo* pFI,
                               const void* pDefault, const UINT4 nDefaultLen,
                               int nIsQuery)
{
    INT4 nEnumVal = -1;                 //ö��ֵ��ʱ����
    ENUMINFO_LIST::iterator item;       //ö�����������
    ENUMINFO_LIST* pEnumInfos = NULL;   //��ʽ��ö������ָ�����


    //ȷ�����������ȷ
    if((nSrcFields != 1) && (nSrcFields != 2))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_ENUM_STR, nSrcFields);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    strcpy((char*)pCurField, " ");
    //�����ǰ����ö������
    if(pFI->flgEnum != 0)
    {
        //��ȡö��ֵ
        if(nSrcFieldLen[0]  == LEN_BYTE)
        {
            //nEnumVal = *(BYTE*)pSrcFields[0];
            BYTE uTmp = 0;
            memcpy(&uTmp, pSrcFields[0], LEN_BYTE);
            nEnumVal = uTmp;
        }
        else if(nSrcFieldLen[0]  == LEN_UINT2)
        {
            //nEnumVal = *(INT2*)pSrcFields[0];
            UINT2 uTmp = 0;
            memcpy(&uTmp, pSrcFields[0], LEN_UINT2);
            nEnumVal = uTmp;
        }
        else if(nSrcFieldLen[0]  == LEN_UINT4)
        {
            //nEnumVal = *(INT4*)pSrcFields[0];
            UINT4 uTmp = 0;
            memcpy(&uTmp, pSrcFields[0], LEN_UINT4);
            nEnumVal = uTmp;
        }
        else
        {
            return -1;
        }

        //��ȡ��ʽ��ö������
        pEnumInfos = (ENUMINFO_LIST*)(pFI->pEnumInfos);
        if(pEnumInfos != NULL)
        {
        
            //˳��������ʽ��ö�������е�ÿһ���ڵ�
            item = pEnumInfos->begin();
            while(item != pEnumInfos->end())
            {
                //�����ǰ�ڵ��ö��ֵ������Ҫ��ö��ֵ
                if(item->nEnumValue == nEnumVal)
                {
                    //��ȡ��ǰö��ֵ��Ӧ��ö���ַ�����Ϣ
                    strcpy((char*)pCurField, item->szEnumValue);
                    break;
                }

                item++;
            }
        }

        //û�ж�Ӧ��ö���ַ�����Ϣ added by lucy 2004-06-28
        if(((pEnumInfos == NULL) ||(item == pEnumInfos->end())) 
           && (nSrcFields == 2))
        {
            memcpy(pCurField, pSrcFields[1], nSrcFieldLen[1]);            
        }
        //end added

        //��Ŀ���ʽ�����������ͷָ���
        if(!nIsQuery)
        {
            //�������Ϊ��ѯ����ת��
            return AddFillerSeparator((char*)pCurField, pFI);
        }

    }

    return 0;
}
/************ End of The GetEnumString Defining ****************/


/************ The FindFun Defining *****************************/
// �������������ݸ�ʽ������ת���������ҵ���Ӧ��ת������ָ��
//
// ���������
// szConvertFun����ʽ���е�ת�������ַ���
//      pSrcFmt��Դ��ʽ����
//
// �����������
//
//   ����ֵ��ת������ָ��
//
/***************************************************************/
CONVERTFUN CConvertFun::FindFun(const char* szConvertFun, const CFormat *pSrcFmt)
{
    STRING  sLib;               //����
    STRING  sFun;               //������
    STRING  sDefault;           //ȱʡֵ
    LIST<STRING>  ParaList;     //�����б�
    CONVERTFUN pRetFun = NULL;  //ת������ָ��


    //ȷ�������������ȷ��
    if(szConvertFun == NULL)
    {
        return NULL;
    }

    //�����ȷ�ؽ�������������������ȱʡֵ�������б�
    if(ConvertAnalysis(szConvertFun, sLib, sFun, sDefault, ParaList) == 0)
    {
        //���ݿ鿽����ת������
        if(!(sFun.compare(FUN_BLOCK_COPY)))
        {
            pRetFun = BlockCopy;
        }
		
        //�鿽��������֧��Intel/�����ֽ���ͬ���������Ļ��࿽�� ZhengYuqun 2004-04-28
        else if(!(sFun.compare(FUN_BLOCK_COPY_EX)))
        {
            pRetFun = BlockCopyEx;
        }

        //SoftX3000����ҵ������Ϊ���س�;������ת��
        else if(!(sFun.compare(FUN_CHANGE_GSVN_AND_CALLTYPE)))
        {
            pRetFun = ChangeGSVNandCallType;
        }
        else if(!(sFun.compare(FUN_CHANGE_GSVN_AND_CALLTYPE_TEXT)))
        {
            pRetFun = ChangeGSVNandCallType2Text;
        }

        //ʵ�ֽ�BYTE�ͻ���SHORT�͵���λ��ת���ɶ�������λ���ת������
        else if(!(sFun.compare(FUN_GET_BIN_YEAR)))
        {
            pRetFun = GetBinYear;
        }

        //ʵ�ֽ�BYTE��SHORT�͵���λ��ת�����ı���ʽ����λ���ת������
        else if(!(sFun.compare(FUN_GET_TEXT_YEAR)))
        {
            pRetFun = GetTextYear;
        }
       //ʵ������ʽ������ӹ̶����ȵ��ַ���
        else if(!(sFun.compare(FUN_FILL_WITH_STR)))
        {
            pRetFun = FillWithStr;
        }
        //ʵ������ʽ�������һ��INI�ļ��е�ָ����ֵ
	    else if(!(sFun.compare(FUN_FILL_BY_INI_FILE)))
        {
            pRetFun = FillByIniFile;
        }        

        //ʵ���ꡢ�¡��ա�ʱ���֡���ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS��ת������
        else if(!(sFun.compare(FUN_GET_TEXT_TIME)))
        {
            pRetFun = GetTextTime;
        }

        //ʵ�ֽ�������������ת�����ı���ת������
        else if(!(sFun.compare(FUN_INT_TO_TEXT)))
        {
            pRetFun = Int2Text;
        }

        //������SBCD��ת�����ַ�����ת������
        else if(!(sFun.compare(FUN_SBCD_TO_STR)))
        {
            pRetFun = SBCD2String;
        }

        //������BCD��ת�����ַ�����ת������
        else if(!(sFun.compare(FUN_BCD_TO_STR)))
        {
            pRetFun = BCD2String;
        }

        //��n.5���ֽڵ�����BCD��ת�����ַ�����ת������
        else if(!(sFun.compare(FUN_BCD_TO_STR_2)))
        {
            pRetFun = BCD2String2;
        }

        //������BCD��ת�����ַ�����ת������
        else if(!(sFun.compare(FUN_RBCD_TO_STR)))
        {
            pRetFun = RBCD2String;
        }

        //�����˻����еķ���BCD��ת�����ַ�����ת������
        else if(!(sFun.compare(FUN_ZXRBCD_TO_STR)))
        {
            pRetFun = ZXRBCD2String;
        }

        //Added by maimaoshi at 2002-05-17

        //ʵ�ֽ�UINT4����ת�����ı�IP��ַ��ת������
        else if(!(sFun.compare(FUN_INT_TO_IP_TEXT)))
        {
            pRetFun = Int2IpText;
        }
        //by ldf 2003-01-08 ��Ӧ���ⵥD05886 D05556��ͬD05415 D5150��
        //ʵ�ְ�UINT4����ת��λ�ı�IP��ַ�ĺ���������˳����Ϸ����෴��
        //���Ϸ���ʾΪ1.2.3.4�򱾺������Ϊ 4.3.2.1
        else if(!(sFun.compare(FUN_INT_TO_IP_TEXT_DIRECT)))
        {
            pRetFun = Int2IpTextDirect;
        }
        //ʵ�ְ������������� ��"00 13"������Ϊ"00 19"
        else if(!(sFun.compare(FUN_ARRAY_TO_TEXT)))
        {
            pRetFun = Array2Text;
        }
        //�޸����
        //ʵ�ְ��ַ��͵��ꡢ�¡��ա�ʱ���֡���ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS��ת������
        else if(!(sFun.compare(FUN_GET_TEXT_TIME_IN_BCD)))
        {
            pRetFun = GetTextTimeInBCD;
        }

        //ʵ�ְ�BCD�͵�4λ����ꡢ�¡��ա�ʱ���֡���ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS��ת������
        else if(!(sFun.compare(FUN_GET_TEXT_TIME_IN_BCD_4Y)))
        {
            pRetFun = GetTextTimeInBCD_4Y;
        }

        //ʵ�ְ��ַ��͵�6.5���ֽڵ�BCD������ʱ��ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS��ת������
        else if(!(sFun.compare(FUN_GET_TEXT_TIME_IN_BCD_T)))
        {
            pRetFun = GetTextTimeInBCD_T;
        }

        //End by maimaoshi at 2002-05-17
        //ʵ�ְ��ַ��͵��ꡢ�¡��ա�ʱ���֡���ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS��ת������
        else if(!(sFun.compare(FUN_BCD_TO_SECOND)))
        {
            pRetFun = BCDToSecond;
        }
        //ʵ���ꡢ�¡��ա�ʱ���֡���ת�����ı���ʽ��YYYYMMDDHHNNSS��ת������
        else if(!(sFun.compare(FUN_GET_TEXT_TIME_NO_SP)))
        {
            pRetFun = GetTextTimeNoSp;
        }
        //��ĳ�ֶ�ȫ����FF��ת������
        else if(!(sFun.compare(FUN_GET_TEXT_FILLFF)))
        {
            pRetFun = FillFF;
        }
        //����һ����ʽ���ö��ֵ������Ӧ��ö���ַ�����ת������
        else if(!(sFun.compare(FUN_GET_ENUM_STR)))
        {
            pRetFun = GetEnumString;
        }
        else if(!(sFun.compare(FUN_ARRAY_TO_HEX_STRING)))
        {
            pRetFun = Array2HexString;
        }
        else if(!(sFun.compare(FUN_FILL_INVALID_VALUE)))
        {
            pRetFun = FillInvaidValue;
        }
        else if(!(sFun.compare(FUN_GET_TEXT_TIME_EX)))
        {
            pRetFun = GetTextTimeEx;
        }
        //ʵ����д"\r\n"��ת������
        else if(!(sFun.compare(FUN_TO_LRLN)))
        {
            pRetFun = ToLRLN;
        }
        //ʵ����д"\r"��ת������
        else if(!(sFun.compare(FUN_TO_LR)))
        {
            pRetFun = ToLR;
        }
        //ʵ����д"\n"��ת������
        else if(!(sFun.compare(FUN_TO_LN)))
        {
            pRetFun = ToLN;
        }
        //�ַ���������ת������
        else if(!(sFun.compare(FUN_STRING_COPY)))
        {
            pRetFun = StringCopy;
        }
        //byteתΪ�ı���ת������
        else if(!(sFun.compare(FUN_BYTE_TEXT2)))
        {
            pRetFun = ByteText2;
        }
        //by ldf 2003-04-01 , У�麯��
        else if(!(sFun.compare(FUN_CHECK_SUM)))
        {
            pRetFun = CheckSum;
        }
        //end
        //���ַ�������ʱ��ת��Ϊ�ַ�����ת������
        else if(!(sFun.compare(FUN_GET_TEXT_TIME_INSTRING)))
        {
            pRetFun = GetTextTimeInString;
        }
        //��ú����ŵĵ绰����
        else if(!(sFun.compare(FUN_PAD_AREACODE))) //Added by ZhengYuqun SWPD01198,
        {
            pRetFun = PadAreacode;
        }
        //�滻�绰�����е�ǰ׺
        else if(!(sFun.compare(FUN_REPLACE_PREFIX))) //Added by ZhengYuqun SWPD01198, 
        {
            pRetFun = ReplacePrefix;
        }        
        else if(!(sFun.compare(FUN_REPLACE_NUM_BY_STR))) //added by lucy 2004-06-25
        {
            pRetFun = ReplaceNumByStr;
        }
        else if (!(sFun.compare(FUN_DURATION_TO_HHMMSS)))
        {
            pRetFun = Duration2HHmmSS;
        }
        else if (!(sFun.compare(FUN_INT_TO_TEXT_EX)))
        {
            pRetFun = Int2TextEx;
        }
        //MSoftXB03����ת��Ϊ2G����ʱ����Ҫ�����Ժ���������
        else if (!(sFun.compare(FUN_CDRTYPE_2G)))  //Added by ZhengYuqun SWPD03631 2004-02-18
        {
            pRetFun = CDRType2G;
        }
        //����ƽ 2004-04-19 ���
        else if (!(sFun.compare(FUN_PAD_NUMBER)))
        {
        	pRetFun = PadNumber;
        }        
        else if (!(sFun.compare(FUN_SMS_NUM_TO_TEXT)))
        {
            pRetFun = SMSNum2Text;
        }
		//���Ⱥ 2004-05-27 ���
		else if (!(sFun.compare(FUN_EBCD_TO_STR)))
        {
            pRetFun = EBCD2String;
        }
		else if (!(sFun.compare(FUN_BCD_TIME_TO_SEC)))
        {
            pRetFun = BCDTime2Sec;
        }
		//����
        //��MSoftX3000��Diagnosticsת��Ϊ2G��Cause for temination
        else if (!(sFun.compare(FUN_TERM_CAUSE_2G)))  //Added by ZhengYuqun SWPD05305 2004-05-19
        {
            pRetFun = TermCause2G;
        }
        //��MSoftX3000��Last MCCMNCת��Ϊ2G��Peer MNC
        else if (!(sFun.compare(FUN_PEER_MNC_2G)))  //Added by ZhengYuqun SWPD05306 2004-05-19
        {
            pRetFun = PeerMnc2G;
        }
        else if (!(sFun.compare(FUN_VAR_LEN_TO_TEXT)))
        {
            pRetFun = VarLen2Text;
        }
        else if (!(sFun.compare(FUN_VAR_VALUE_TO_TEXT)))
        {
            pRetFun = VarValue2Text;
        }
        //Added by ZhengYuqun SWPD06279 2004-06-26
        else if (!(sFun.compare(FUN_GET_VAR_VALUE)))
        {
            pRetFun = GetVarValue;
        }
        else if (!(sFun.compare(FUN_GET_VAR_LEN)))
        {
            pRetFun = GetVarLen;
        }
        //Added by lucy 2005-5-25 OPT version
        else if (!(sFun.compare(FUN_LINK_TEXT)))
        {
            pRetFun = LinkText;
        }
        else if(!(sFun.compare(FUN_GET_RECORDTYPE)))
        {
            pRetFun = GetRecordType;
        }
        else if(!(sFun.compare(FUN_GET_SUBCATEGORY)))
        {
            pRetFun = GetSubCategory;
        }
        else if(!(sFun.compare(FUN_ISUP_CHARGE_NUMBER)))
        {
            pRetFun = ISUPChargeNumber;
        }
        else if(!(sFun.compare(FUN_ISUP_CHARGE_NUMBER_NOA)))
        {
            pRetFun = ISUPChargeNumberNOA;
        }
        else if (!(sFun.compare(FUN_CONVERT10MS)))
        {
            pRetFun = Convert10ms;
        }
        else if(!(sFun.compare(FUN_VIDEOTIME)))
        {
            pRetFun = VideoTime;
        }                
        else if(!(sFun.compare(FUN_VIDEODURATION)))
        {
            pRetFun = VideoDuration;
        }        
        else if(!(sFun.compare(FUN_ADD)))
        {
            pRetFun = Add;
        }                
        //���ݺ���ĳ�����ת��BCD�� //SWPD06703 Zheng Yuqun 2004-06-19
        else if(!(sFun.compare(FUN_BCD_TO_STR_BY_LEN)))
        {
            pRetFun = BCD2StringByLen;
        }
        //add by zkq 2004-09-28 //SWPD09010
        else if(!(sFun.compare(FUN_BCD_TO_BCD)))
        {
            pRetFun = BCD2RBCD;
        }
        else if(!(sFun.compare(FUN_GET_ARG_SPEC_FIELD)))
        {
            pRetFun = GetArgSpecField;
        }
        else
        {
            pRetFun = NULL;
        }
    }

    //������������ַ�������
    else
    {
        return NULL;
    }

    //���ػ�ȡ��ת������ָ��
    return pRetFun;
}
/************ End of The FindFun Defining **********************/


/************ The AddFillerSeparator Defining ******************/
// ����������Ϊ�ı���ʽ��Ŀ���ʽ�����������ͷָ���
//
// ���������
//   pszField��δ���������ͷָ�����Ŀ���ʽ������
//        pFI����Ŀ���ʽ�������
//
// ���������
//   pszField���Ѿ��������ͷָ������Ŀ���ʽ������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/****************************************************************/
int CConvertFun::AddFillerSeparator(char* pszField,
                                    const SFieldInfo* pFI)
{
    UINT4  nScrLen = strlen(pszField);   //Ŀ���ʽ���ԭʼ����������
    UINT4  nFieldLen = 0;                //Ŀ���ʽ���ʵ�ʳ���
    UINT4  nLeftFill = 0;                //�����������Ҫ����ַ��ĳ���
    UINT4  nRightFill = 0;               //�������ұ���Ҫ����ַ��ĳ���
    char*  pszTmp = NULL;                //ԭʼ���ݻ�����
    BOOL   bIsBlankSepartor = FALSE;

    //ȷ�������������ȷ
    if((pszField == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if (pFI->cSeparator == ' ')
    {
        bIsBlankSepartor = TRUE;
    }

    nFieldLen = pFI->nFieldLength;  //Ŀ���ʽ���ʵ�ʳ���

    //�޸��ֽ�������(D12711),��ïʶ��2002-04-13
    //���Ŀ���ʽ��ĳ��Ȳ���
    //if(nFieldLen < (nScrLen + 2))
    if(nFieldLen < (nScrLen + 1))
    {
        if(!bIsBlankSepartor)
        {
            pszField[nFieldLen - 1] = pFI->cSeparator;  //���Ϸָ��� ZhengYuqun SWPD03695 2004-02-20
        }
        return 0;// Gan Zhiliang modified
    }
    //�޸Ľ�������ïʶ��2002-04-13

    //������Ҷ���
    if(pFI->nAlignMode == RIGHT_ALIGN)
    {
        //����ԭʼ����������
        pszTmp = new char[nScrLen + 1];
        strcpy(pszTmp, pszField);


        if (bIsBlankSepartor)
        {
            nLeftFill = nFieldLen - nScrLen;  //��ȡ���Ӧ������ַ��ĳ���
        }
        else
        {
            nLeftFill = nFieldLen - nScrLen - 1;  //��ȡ���Ӧ������ַ��ĳ���
        }

        //�����������������ַ�
        memset((void*)pszField, pFI->cFillChar, nLeftFill);
        memcpy((void*)&pszField[nLeftFill], (void*)pszTmp, nScrLen);
    }

    //����Ǿ��ж���
    else if(pFI->nAlignMode == MID_ALIGN)
    {
        //����ԭʼ����������
        pszTmp = new char[nScrLen + 1];
        strcpy(pszTmp, pszField);

        nLeftFill = (nFieldLen - nScrLen - 1) / 2;

        if (bIsBlankSepartor)
        {
            nLeftFill = (nFieldLen - nScrLen) / 2;
            nRightFill = nFieldLen - nScrLen - nLeftFill;
        }
        else
        {
            nLeftFill = (nFieldLen - nScrLen - 1) / 2;
            nRightFill = nFieldLen - nScrLen - 1 - nLeftFill;
        }


        //�������������ҷֱ�����ַ�
        memset((void*)pszField, pFI->cFillChar, nLeftFill);
        memcpy((void*)&pszField[nLeftFill], (void*)pszTmp, nScrLen);
        memset((void*)&pszField[nLeftFill + nScrLen], pFI->cFillChar, nRightFill);
    }

    //Ĭ���������
    else
    {
        if (bIsBlankSepartor)
        {
            nRightFill = nFieldLen -  nScrLen;  //��ȡ�ұ�Ӧ������ַ��ĳ���
        }
        else
        {
            nRightFill = nFieldLen -  nScrLen - 1;  //��ȡ�ұ�Ӧ������ַ��ĳ���
        }


        //�����������ұ�����ַ�
        memset((void*)&pszField[nScrLen], pFI->cFillChar, nRightFill);
    }

    //Ŀ���ʽ���������ָ���
    if (!bIsBlankSepartor)
    {
        pszField[nFieldLen - 1] = pFI->cSeparator;
    }

    if(pszTmp != NULL)
    {
        delete[] pszTmp;  //�ͷ���ʱ�������ռ�
    }

    return 0;
}
/************ End of The AddFillerSeparator Defining ************/

/************ The LinkText Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//          ����ServedMSISDN����'@'������Domain�������ΪChange_dn
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ2,
//           �������ServedMSISDNMSISDN��Domain
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
// 2005-5-25 lucy  OPT�汾 ��MSISDN������'@'��������Domain
/***************************************************************/
int CConvertFun::LinkText(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    char sztmp[63];
    int nLen;
    
    //��֤���������ȷ
    if((pCurField == NULL) || (nSrcFields != 2) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }
    //end of ��֤
    
    //��ԭServedMSISDN����RBCD��,�Ƚ�֮תΪ�ַ���
    nLen = RBCDtoString((BYTE*)pSrcFields[0], sztmp, nSrcFieldLen[0],0);
    //����Domain��
    memcpy(sztmp + nLen, pSrcFields[1], nSrcFieldLen[1]);
    //������'\0'
    sztmp[nLen + nSrcFieldLen[1] + 1] = '\0';
    
    memcpy(pCurField, sztmp, nLen + nSrcFieldLen[1] + 1);
    
    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;
}
/************ End of The LinkText Defining ****************/

/************ The GetRecordType  Defining *********************/
// ��������������TM��ʽת�����󣬴�������Ϊ��
// 1.   ��ԭʼ�����е�free_indicatorΪ0�ǣ�Record Type�ֶ���дΪ04
// 2.   ����ԭʼ�����е�gsvn��ͨ��ö��ת��
// 3.   ֧��û��ȡֵʱ����Ĭ��ֵ����
//
// ����������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           �������Ϊ���������еĲ��������ͼ�˳��
//           free_ind     gsvn
//            FT_BYTE     FT_BYTE
// ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
// ������GetRecordType(free_ind, gsvn, "0")
// Added By Lucy 2004-07-12
/***************************************************************/    
int CConvertFun::GetRecordType(void* pCurField, const UINT4 nSrcFields,
                         const void* pSrcFields[], 
                         const UINT4 nSrcFieldLen[], 
                         const SFieldInfo* pFI, 
                         const void* pDefault, const UINT4 nDefaultLen,
                         int nIsQuery)
{
    //��֤���������ȷ
    if((pCurField == NULL) || ((nSrcFields != 2) && (nSrcFields != 3))
        || (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_GET_SUBCATEGORY, nSrcFields);
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    BYTE nFreeInd = 0;
    memcpy(&nFreeInd, pSrcFields[0], LEN_BYTE);

    if(0 == nFreeInd)
    {
        sprintf((char*)pCurField, "04");
    }
    else
    {
        BYTE nGsvn = 0;
        memcpy(&nGsvn, pSrcFields[1], LEN_BYTE);

        //�����ǰ����ö������
        if(pFI->flgEnum != 0)
        {
            ENUMINFO_LIST::iterator item;       //ö�����������
            ENUMINFO_LIST* pEnumInfos = NULL;   //��ʽ��ö������ָ�����    
            
            //��ȡ��ʽ��ö������
            pEnumInfos = (ENUMINFO_LIST*)(pFI->pEnumInfos);
            if(pEnumInfos != NULL)
            {                
                //˳��������ʽ��ö�������е�ÿһ���ڵ�
                item = pEnumInfos->begin();
                while(item != pEnumInfos->end())
                {
                    //�����ǰ�ڵ��ö��ֵ������Ҫ��ö��ֵ
                    if(item->nEnumValue == nGsvn)
                    {
                        //��ȡ��ǰö��ֵ��Ӧ��ö���ַ�����Ϣ
                        strcpy((char*)pCurField, item->szEnumValue);
                        break;
                    }
                    
                    item++;
                }
            }
            
            //û�ж�Ӧ��ö���ַ�����Ϣ 
            if(((pEnumInfos == NULL) ||(item == pEnumInfos->end())) 
                && (nSrcFields == 3))
            {
                memcpy(pCurField, pSrcFields[2], nSrcFieldLen[2]);
            }
        }           
    }    
    
    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;    
}
/************ End of The GetRecordType Defining ****************/

/************ The GetSubCategory  Defining *********************/
// ��������������TM��ʽת�����󣬴�������Ϊ��
// 1.	���ԭʼ�����е�caller_ctx_ number�ֶ���Ч������ֶ���дΪ03��
// 2��	���ԭʼ�����е�caller_didΪH.323����SIP������trunk_group _in
//      ������0xFFFF������ֶ���дΪ05��
// 3��	�������������ԭʼ�����е�caller_category�ֶ���ת����ö��ȡֵ        
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           �������Ϊ���������еĲ��������ͼ�˳��
//           caller_ctx_ number��caller_did�� trunk_group _in, caller_category           
//           FT_BCD	           , FT_BYTE   ,  FT_USHORT	     , FT_BYTE	
//
//           ��5������Ϊû�ж�Ӧö��ʱ��Ĭ���������
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
// ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
// �������ӣ�GetSubCategory(caller_ctx_number, caller_did, trunk_group_in, caller_category, "01")
//
// Added By Lucy 2004-07-09
/***************************************************************/
int CConvertFun::GetSubCategory(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[], 
                          const UINT4 nSrcFieldLen[], 
                          const SFieldInfo* pFI, 
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    //��֤���������ȷ
    if((pCurField == NULL) || ((nSrcFields != 4) && (nSrcFields != 5))
       || (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_SUBCATEGORY, nSrcFields);
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //1. ���ԭʼ�����е�caller_ctx_ number�ֶ���Ч������ֶ���дΪ03��
    char szResult[32];

    BCDtoString((BYTE*)pSrcFields[0], (char*)szResult, 
        nSrcFieldLen[0], sizeof(szResult));
    
    if(szResult[0] != 'F')
    {
        strcpy((char*)pCurField, "03");        
    }
    else
    {
        //2. ���ԭʼ�����е�caller_didΪH.323 (118) ����SIP (119)��
        //   ����trunk_group_in������0xFFFF������ֶ���дΪ05��
        BYTE nCallerDid = 0;
        UINT2 nTrunkGroupIn = 0;

        memcpy(&nCallerDid, pSrcFields[1], LEN_BYTE);
        memcpy(&nTrunkGroupIn, pSrcFields[2], LEN_UINT2);

        if((nCallerDid == 118 || nCallerDid == 119) 
           && (nTrunkGroupIn != 0xFFFF))
        {
            strcpy((char*)pCurField, "05");
        }
        else
        {
            //3. ����ԭʼ�����е�caller_category�ֶ���ת����ö��ȡֵ
            BYTE nCallerCategory;
            ENUMINFO_LIST::iterator item;       //ö�����������
            ENUMINFO_LIST* pEnumInfos = NULL;   //��ʽ��ö������ָ�����    

            strcpy((char*)pCurField, " ");        
            memcpy(&nCallerCategory, pSrcFields[3], LEN_BYTE);

            //�����ǰ����ö������
            if(pFI->flgEnum != 0)
            {
                //��ȡ��ʽ��ö������
                pEnumInfos = (ENUMINFO_LIST*)(pFI->pEnumInfos);
                if(pEnumInfos != NULL)
                {
            
                    //˳��������ʽ��ö�������е�ÿһ���ڵ�
                    item = pEnumInfos->begin();
                    while(item != pEnumInfos->end())
                    {
                        //�����ǰ�ڵ��ö��ֵ������Ҫ��ö��ֵ
                        if(item->nEnumValue == nCallerCategory)
                        {
                            //��ȡ��ǰö��ֵ��Ӧ��ö���ַ�����Ϣ
                            strcpy((char*)pCurField, item->szEnumValue);
                            break;
                        }
                
                        item++;
                    }
                }
        
                //û�ж�Ӧ��ö���ַ�����Ϣ 
                if(((pEnumInfos == NULL) ||(item == pEnumInfos->end())) 
                    && (nSrcFields == 5))
                {
                    memcpy(pCurField, pSrcFields[4], nSrcFieldLen[4]);
                }
            }        
        }
    }

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;
}
/************ End of The GetSubCategory Defining ****************/


/************ The ISUPChargeNumber Defining *********************/
// ��������������TM��ʽת�����󣬴�������Ϊ��
//     ����ԭʼ�����е�ISUP charge number indicator�ֶε�ȡֵת����
//      1��	���ȡֵΪ0����caller_numberת�������ֶΣ�
//      2��	���ȡֵΪ1����called_numberת�������ֶΣ�
//      3��	���ȡֵΪ2����connected_numberת�������ֶΡ�
//      ���������дΪ���ַ�����
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           �������Ϊ���������еĲ��������ͼ�˳��
//           ISUPchargenumber   caller_number  called_number  connected_number
//           FT_BYTE           , FT_BCD  10      ,  FT_BCD	     , FT_BCD	
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
// ����ֵ���ɹ�����0��ʧ�ܷ���-1
// ���ӣ� ISUPChargeNumber(isup_charge_num, caller_number, called_number, connected_number)
// Added By Lucy 2004-07-09
/***************************************************************/
int CConvertFun::ISUPChargeNumber(void* pCurField, const UINT4 nSrcFields,
                            const void* pSrcFields[], 
                            const UINT4 nSrcFieldLen[], 
                            const SFieldInfo* pFI, 
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    //��֤���������ȷ
    if((pCurField == NULL) || (nSrcFields != 4) || (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_GET_SUBCATEGORY, nSrcFields);
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //���Ĭ�Ͽ�ֵ
    strcpy((char*)pCurField, "");        

    BYTE  nISUPNumber = 0;

    memcpy(&nISUPNumber, pSrcFields[0], LEN_BYTE);

    if((0 == nISUPNumber) || (1 == nISUPNumber) || (2 == nISUPNumber))
    {
        char szResult[32];
        
        int nResLen = BCDtoString((BYTE*)pSrcFields[nISUPNumber + 1], (char*)szResult, 
                              nSrcFieldLen[nISUPNumber + 1]);
        //ȥ��β����FFFF
        char* pF = strchr(szResult, 'F');
        if(pF != NULL)
        {
            *pF = '\0';
        }

        if (nResLen > 0)
        {            
            strcpy((char*)pCurField, szResult);
        }        
    }

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    return 0;
}
/************ End of The ISUPChargeNumber Defining ****************/


/************ The ISUPChargeNumberNOA Defining *********************/
// ��������������TM��ʽת�����󣬴�������Ϊ��
//     ����ԭʼ�����е�ISUP charge number indicator�ֶε�ȡֵת����
//      1��	���ȡֵΪ0����caller_address_natureת�������ֶΣ�
//      2��	���ȡֵΪ1����called_address_natureת�������ֶΣ�
//      3��	���ȡֵΪ2����connected_address_natureת�������ֶΡ�
//      ���������дΪ���ַ�����
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           �������Ϊ���������еĲ��������ͼ�˳��, ���Ͷ�Ϊ��FT_BYTE	
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
// ����ֵ���ɹ�����0��ʧ�ܷ���-1
// ���ӣ� ISUPChargeNumberNOA(isup_charge_num, caller_addr_nature,called_addr_nature,connected_addr_nature)
// Added By Lucy 2004-07-09
/***************************************************************/
int CConvertFun::ISUPChargeNumberNOA(void* pCurField, const UINT4 nSrcFields,
                               const void* pSrcFields[], 
                               const UINT4 nSrcFieldLen[], 
                               const SFieldInfo* pFI, 
                               const void* pDefault, const UINT4 nDefaultLen,
                               int nIsQuery)
{
    //��֤���������ȷ
    if((pCurField == NULL) || (nSrcFields != 4) || (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_GET_SUBCATEGORY, nSrcFields);
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }
    
    //���Ĭ�Ͽ�ֵ
    strcpy((char*)pCurField, "");
    
    BYTE nISUPNumber = 0;
    int nResult = 0;
    memcpy(&nISUPNumber, pSrcFields[0], LEN_BYTE);
    
    if((0 == nISUPNumber) || (1 == nISUPNumber) || (2 == nISUPNumber))
    {
        memcpy(&nResult, pSrcFields[nISUPNumber + 1], nSrcFieldLen[nISUPNumber + 1]);
        sprintf((char*)pCurField, "%d", nResult);
    }
    
    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;    
}
/************ End of The ISUPChargeNumberNOA Defining ****************/    

//add by lucy 2004-08-05
/************ The VideoTime Defining *********************/
// ������������ͨ���ڸ�ʽת�����󣬴�������Ϊ��
//     ������Ƶ������ֶ��жϣ��������Ƶ����룬��ʹ��"Ӧ������ʱ��"
// ʹ��ʾ����
// VideoTime(video_code_type, ans_year,ans_mon,ans_day,ans_hour,ans_min,ans_sec)
/********************************************************/
int CConvertFun::VideoTime(void* pCurField, const UINT4 nSrcFields,
                     const void* pSrcFields[], 
                     const UINT4 nSrcFieldLen[], 
                     const SFieldInfo* pFI, 
                     const void* pDefault, const UINT4 nDefaultLen,
                     int nIsQuery)
{
    INT2  nYear =  0;    //�����λ�����ʱ����
    BYTE  nMon =   0;    //����µ���ʱ����
    BYTE  nDate =  0;    //����յ���ʱ����
    BYTE  nHour =  0;    //���ʱ����ʱ����
    BYTE  nMin =   0;    //��ŷֵ���ʱ����
    BYTE  nSec =   0;    //��������ʱ����
    
    //��֤���������ȷ
    if((pCurField == NULL) || (nSrcFields != 7) || (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_VIDEOTIME, nSrcFields);
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }
    
    //���Ĭ�Ͽ�ֵ
    ((char*)pCurField)[0] = '\0';
    
    BYTE VideoType = 0;
    memcpy(&VideoType, pSrcFields[0], LEN_BYTE);
    
    if(VideoType != 0)
    {
        //�����Ϊ��ΪSHORT����
        if(nSrcFieldLen[1] == LEN_INT2)
        {
            memcpy((void*)&nYear, pSrcFields[1], LEN_INT2);           
            //CC08�������ж������ֽڵ�2λ�꣬��ݱ����ڸ��ֽڣ�
            //������Ҫ�Ƶ����ֽ�
            nYear = nYear >> 8;
        }       
        //�����Ϊ��ΪBYTE����
        else if(nSrcFieldLen[1] == LEN_BYTE)
        {
            BYTE yTmp = *(BYTE*)pSrcFields[1];
            nYear = yTmp;
        }
        
        //�Ƿ�����
        else
        {
            return -1;
        }
        
        //����Ϊ�����2000��������Ŀ���ʽ��ǰ��
        nYear += 2000;
        
        //��ȡ�¡��ա�ʱ���֡����ֵ
        nMon   =  *(BYTE*)pSrcFields[2];
        nDate  =  *(BYTE*)pSrcFields[3];
        nHour  =  *(BYTE*)pSrcFields[4];
        nMin   =  *(BYTE*)pSrcFields[5];
        nSec   =  *(BYTE*)pSrcFields[6];
        
        if((nYear - 2000) == 0xFF || nMon == 0xFF || nDate == 0xFF || nHour == 0xFF || nMin == 0xFF || nSec == 0xFF)
        {
            ((char*)pCurField)[0] = '\0';
        }
        else
        {
            //�����͵��ꡢ�¡��ա�ʱ���֡���ת����"YYYYMMDDHHNNSS"���ַ���
            sprintf((char*)pCurField, "%4d%02d%02d%02d%02d%02d",
                nYear, nMon, nDate, nHour, nMin, nSec);
        }
        
    }
    
    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;        
}

//add by lucy 2004-08-05
/************ The VideoDuration Defining *********************/
// ������������ͨ���ڸ�ʽת�����󣬴�������Ϊ��
//     ������Ƶ������ֶ��жϣ��������Ƶ����룬��ʹ��"ͨ��ʱ��"
// ʹ��ʾ����
// VideoDuration(video_code_type,conversation_time)
/********************************************************/
int CConvertFun::VideoDuration(void* pCurField, const UINT4 nSrcFields,
                         const void* pSrcFields[], 
                         const UINT4 nSrcFieldLen[], 
                         const SFieldInfo* pFI, 
                         const void* pDefault, const UINT4 nDefaultLen,
                         int nIsQuery)
{
    INT4  nTmp = 0;    //���BYTE��SHORT��LONG����ʱ����

    //��֤���������ȷ
    if((pCurField == NULL) || (nSrcFields != 2) || (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_VIDEOTIME, nSrcFields);
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }
    
    //���Ĭ�Ͽ�ֵ
    ((char*)pCurField)[0] = '\0';    
    
    BYTE VideoType = 0;
    memcpy(&VideoType, pSrcFields[0], LEN_BYTE);
    
    if(VideoType != 0)
    {
        //��ȡԭ��ʽ�������ֵ
        if(nSrcFieldLen[1] == LEN_BYTE)
        {
            BYTE uTmp = 0;
            memcpy(&uTmp, pSrcFields[1], LEN_BYTE);
            nTmp = uTmp;
        }
        else if(nSrcFieldLen[1] == LEN_UINT2)
        {
            UINT2 uTmp = 0;
            memcpy(&uTmp, pSrcFields[1], LEN_UINT2);
            nTmp = uTmp;
        }
        else if(nSrcFieldLen[1] == LEN_UINT4)
        {
            UINT4 uTmp = 0;
            memcpy(&uTmp, pSrcFields[1], LEN_UINT4);
            nTmp = uTmp;
        }
        else
        {
            return -1;
        }

        sprintf((char*)pCurField, "%u", nTmp);
    }
    
    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;            
}


/************ The GetParas Defining *****************************/
// ������������ת���������õĲ����ַ����в�ֳ���������������
//           ����֮����','�ָ�
//
// ���������
//     sParas�������ַ���
//
// ���������
//   ParaList�������б�����������˳������
//
//   ����ֵ������������ó�����������-1�����򷵻�0
//
/****************************************************************/
int CConvertFun::GetParas(const STRING& sParas, LIST<STRING>& ParaList)
{
    int     nStart = 0;               //���������ַ����е�һ��'('��λ��
    int     nEnd = 0;                 //���������ַ��������һ��')'��λ��
    STRING  sSubStr;                  //�����ַ�����ʱ�������
    char    szTmpPara[MAX_PARA_LEN];  //һ����������ʱ������
    char*   pTmp = NULL;              //������ʱ�������ĵ�ǰλ��
    const char*  pStr = NULL;         //�����ַ����ĵ�ǰλ��


    // ��ʼ���������
    ParaList.clear();

    //������������ַ����ĳ��ȵ�����
    if(sParas.length() == 0)
    {
        return 0;
    }

    //��ȡ���������ַ����е���������λ��
    nStart = sParas.find_first_of('(');
    nEnd = sParas.find_last_of(')');

    //������������ַ�����ͬʱ������������
    if((nStart != -1) && (nEnd != -1))
    {
        //��ȡ���������еĲ����ַ���
        sSubStr = sParas.substr(nStart+1, nEnd - nStart - 1);
    }

    //������������ַ����в�������������
    else if((nStart == -1) && (nEnd == -1))
    {
        //�������������ַ������ǲ���
        sSubStr = sParas;
    }

    //������������ַ�����ֻ���������Ż�������
    else
    {
        return -1;
    }

    //���ò����ַ����ĵ�ǰλ��
    pStr = sSubStr.c_str();

    //��ʼ�������Ļ��������䵱ǰλ��
    szTmpPara[0] = '\0';
    pTmp = szTmpPara;

    //�жϲ����ַ����е�ÿһ���ַ�
    while(*pStr != '\0')
    {
        //�����ǰ�ַ����ڿո�
        if(*pStr == ' ')
        {
            pStr++;
            continue;
        }

        //�����ǰ�ַ����ڶ���
        else if(*pStr == ',')
        {
            //�����ǰ������Ϊ��
            if(szTmpPara[0] != '\0')
            {
                //�ѵ�ǰ�������������������
                *pTmp = '\0';
                ParaList.push_back(szTmpPara);

                //���³�ʼ�������Ļ��������䵱ǰλ��
                szTmpPara[0] = '\0';
                pTmp = szTmpPara;
            }

            //�����ǰ����Ϊ��
            else
            {
                return -1;
            }

            pStr++;
            continue;
        }
        else
        {
            //�ѵ�ǰ�ַ����������������ǰλ��
            *pTmp++ = *pStr++;
        }
    }

    //�����ǰ������Ϊ��
    if(szTmpPara[0] != '\0')
    {
        //�ѵ�ǰ�������������������
        *pTmp = '\0';
        ParaList.push_back(szTmpPara);
    }

    return 0;
}
/************ End of The GetParas Defining **********************/

/************ The ConvertFunDiv Defining ************************/
// �����������Ӹ�ʽ��ת�������з�����������������б���ȱʡֵ
//
// ���������
//    sSubStr��ת�������ַ���
//
// ���������
//       sFun��ת��������
//   sDefault����ȱʡֵ
//   ParaList�������б���������������������
//
//   ����ֵ������������ó�����������-1�����򷵻�0
//
/****************************************************************/
int CConvertFun::ConvertFunDiv(const STRING& sSubStr, STRING& sFun,
                               STRING& sDefault, LIST<STRING>& ParaList)
{
    int     nStart = 0;               //�����ַ����е�һ��'('��λ��
    int     nEnd = 0;                 //�����ַ��������һ��')'��λ��
    int     nDefault = 0;             //�����ַ�����default��λ��
    STRING  sTmpStr;                  //�����ַ�����ʱ�������
    char    szTmpPara[MAX_PARA_LEN];  //һ����������ʱ������
    char*   pTmp = NULL;              //������ʱ�������ĵ�ǰλ��
    int     i = 0;                    //��ʱ����������


    // ��ʼ���������
    sFun = "";
    sDefault = "";
    ParaList.clear();

    //������������ַ����ĳ��ȵ�����
    if(sSubStr.length() == 0)
    {
        return -1;
    }

    //��ȡ�����ַ����е���������λ��
    nStart = sSubStr.find_first_of('(');
    nEnd = sSubStr.find_last_of(')');

    //��������ַ����в����������Ż������ţ��򶼲����ڣ�
    //���ߵ�һ���ַ��������ţ��������������ڣ�
    if((nStart == -1) || (nEnd == -1) || (nStart == 0))
    {
        return -1;
    }

    //��ȡ��������
    //sFun = sSubStr.substr(0, nStart);
    pTmp = szTmpPara;    //��ʼ���������ƻ�������ǰλ��
    for(i = 0; i < nStart; i++)
    {
        //���Կո�
        if(sSubStr[i] == ' ')
        {
            continue;
        }

        *pTmp++ = sSubStr[i];
    }
    *pTmp = '\0';
    sFun = szTmpPara;

    //��ȡȱʡֵλ��
    nDefault = sSubStr.find(VAR_DEFAULT);

    //���ȱʡֵ����
    if(nDefault != -1)
    {
        pTmp = szTmpPara;   //��ʼ��ȱʡֵ��������ǰλ��

        //��ȡȱʡֵ
        i = nDefault + strlen(VAR_DEFAULT) + 1;  //��"default"�����¸�λ�ÿ�ʼ
        while((sSubStr[i] != ')') && (sSubStr[i] != '\0'))  //����')'��ʾȱʡֵ����
        {
            //���Կո�͵Ⱥ�
            if((sSubStr[i] == ' ') || (sSubStr[i] == '='))
            {
                i++;
                continue;
            }

            *pTmp++ = sSubStr[i++];
        }
        *pTmp = '\0';
        sDefault = szTmpPara;

        //��ȡ�����ַ������ӵ�һ��������һ��λ�ÿ�ʼ��
        //��"default"��ǰǰ��λ�ý���
        int nTmp = nDefault - nStart - 2;
        if(nTmp < 0)
        {
            nTmp = 0;
        }
        sTmpStr = sSubStr.substr(nStart + 1, nTmp);
    }

    //���ȱʡֵ������
    else
    {
        //��ȡ�����ַ������ӵ�һ�����ŵ���һ��λ�ÿ�ʼ��
        //�����һ������֮ǰ����
        sTmpStr = sSubStr.substr(nStart + 1, nEnd - nStart - 1);
    }

    //���������ַ���Ϊ�����б�
    if (GetParas(sTmpStr, ParaList) != 0)
    {
        return -1;
    }

    return 0;
}
/************ End of The ConvertFunDiv Defining *****************/


/****************** The Prefix Defining *************************/
// ������������ȡ�绰�����ַ�����ǰnλ����ת������Ӧ���ַ���,
//           �ú�����CONDITIONFUN���͵Ĺ��ߺ���
//
// ���������
//  nParasNum�������������˺����Ĳ�������Ӧ��Ϊ2
//     pParas���������飬�˺�����������������pParas[0]��ʾ�绰�����ַ�����BCD�룩
//             pParas[1]��ʾҪ��ȡ�绰�����ַ�����ǰn���ַ�
//  nParasLen����������������
//   pDefault������ȱʡֵ
//
// ���������SVariant���͵Ľ��
//
//   ����ֵ������ɹ����򷵻�0����������ط���
//
/****************************************************************/
int CConvertFun::Prefix(const UINT4 nParasNum,
                        const void* pParas[],
                        const UINT4 nParasLen[],
                        const STRING& pDefault,
                        SVariant& stVar)
{
    char*      pCallNum = NULL;           //�绰�����ַ���
    UINT4      uPreNum  = 0;              //ȡ�绰�����ַ�����ǰuPreNum������
    char szTmpStr[FMT_MAX_CALL_NUM_LEN];  //�绰�����ǰuPreNum��������ʱ�ַ���

    //ȷ�������������ȷ��
    if((nParasNum != 2) || (pParas == NULL))
    {
        return -1;
    }
    int i = 0;
    //ȷ��������ȷ
    for(i = 0; i < nParasNum; i++)
    {
        if((pParas[i] == NULL) || nParasLen[i] == 0)
        {
            return -1;
        }
    }

    //��ȡ�绰�����ַ���
    pCallNum = (char*)pParas[0];

    /*
    //��ȡҪ��ȡ��ǰn�����ֵĸ���
        if(nParasLen[1] == LEN_BYTE)
        {
            //uPreNum = *(BYTE*)pParas[1];
            BYTE uTmp = 0;
            memcpy(&uTmp, pParas[1], LEN_BYTE);
            uPreNum = uTmp;
        }
        else if(nParasLen[1] == LEN_UINT2)
        {
            //uPreNum = *(INT2*)pParas[1];
            UINT2 uTmp = 0;
            memcpy(&uTmp, pParas[1], LEN_UINT2);
            uPreNum = uTmp;
        }
        else if(nParasLen[1] == LEN_UINT4)
        {
            //uPreNum = *(INT4*)pParas[1];
            UINT4 uTmp = 0;
            memcpy(&uTmp, pParas[1], LEN_UINT4);
            uPreNum = uTmp;
        }
        else
        {
            return NULL;
        }


    //��󳤶�Ϊ�绰����ĳ���
    if(uPreNum > strlen(pCallNum))
    {
        uPreNum = strlen(pCallNum);
    }*/
    memcpy(&uPreNum, pParas[1], 4);
    if(uPreNum > nParasLen[0])
    {
        uPreNum = nParasLen[0];
    }
    int nCount = 0;

    for (i = 0; i < nParasLen[0]; i++)
    {
        unsigned char ch;

        ch = pCallNum[ i ];
        if ((ch < 0xA0) && (nCount < uPreNum))
        {
            szTmpStr[nCount] = (((ch >> 4) & 0x0F) + '0');
            nCount++;
        }
        else
            break;

        ch &= 0x0F;
        if ((ch < 0x0A) && (nCount < uPreNum))
        {
            szTmpStr[nCount] = (ch + '0');
            nCount++;
        }
        else
            break;
    }

    szTmpStr[uPreNum] = '\0';

    //�Ѻ����ַ�����ǰuPreNum������ת��������
    stVar.yVariantType = FT_STRING;
    memcpy(stVar.szValue, szTmpStr,  uPreNum + 1);

    return 0;
}
/****************** End of The Prefix Defining ******************/


/****************** The FINDCONDFUN Defining ********************/
// ����������������������ָ��ĺ������Ͷ���
//
// ���������
//       sFun����������
//
// �����������
//
//   ����ֵ��CONDITIONFUN���͵ĺ���ָ��
//
//   �ؼ��㣺��ʱֻ��Prefixһ������
//
/****************************************************************/
CONDITIONFUN CConvertFun::FindConFun(const STRING& sFun)
{
    CONDITIONFUN pRetFun = NULL;


    //ȷ�������������ȷ��
    if(sFun.length() == 0)
    {
        return NULL;
    }

    //��ȡ�绰�����ǰnλ����
    if(!(sFun.compare(FUN_PREFIX)))
    {
        pRetFun = Prefix;
    }

    else if(!(sFun.compare(FUN_ISINNUMSEG)))
    {
        pRetFun = IsInNumSeg;
    }

    else if(!(sFun.compare(FUN_ISINENUM)))
    {
        pRetFun = IsInEnum;
    }

    //ʵ�ִ����к����з�������ID
    else if(!(sFun.compare(FUN_GET_AREA_ID_INSTRING)))
    {
        pRetFun = GetAreaID;
    }

    //�����Ƿ��ĺ�����
    else
    {
        pRetFun = NULL;
    }

    //���ػ�ȡ��ת������ָ��
    return pRetFun;
}
/****************** End of The FINDCONDFUN Defining *************/


/************ The GetTextTimeInBCD Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ�ְ�BCD��������ʱ����ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS.MS
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1
//
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
// Added by maimaoshi at 2002-10-15
/***************************************************************/
int CConvertFun::GetTextTimeInBCD(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    int nYear, nMon, nDay, nHour, nMin, nSec;
    char szTemp[10];

    int    nIndex    = 0;      //�ı���ʽ��BCD���ַ�������
    int    nBcdIndex = 0;      //BCD����������
    BYTE   yCurBcd   = 0;      //��ǰBCD��ֵ
    BYTE*  pBcdArray = NULL;   //BCD������ָ��
    char*  pCurStr   = NULL;   //�ı���ʽ��BCD���ַ���ָ��


    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_TEXT_TIME_IN_BCD, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //��ʼ��BCD������ָ����ı���ʽ��BCD���ַ���ָ��
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    //˳��ɨ��BCD�������ÿһ��Ԫ��
    while(nBcdIndex < nSrcFieldLen[0])
    {
        //��ȡ��ǰBCD���ֵ
        yCurBcd = pBcdArray[nBcdIndex++];

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0xF0)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }

        //���ε�ǰBCD������ֵ�ĸ���λ
        yCurBcd &= 0x0F;

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0x0F)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = yCurBcd - 10 + 'A';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }
    }

    //�����ַ����Ľ�����
    pCurStr[nIndex] = '\0';

    //�����꣬����λ������λ��
    memcpy(szTemp, pCurStr, 2);
    szTemp[2] = 0;
    nYear = 2000 + atoi(szTemp);

    //������
    memcpy(szTemp, pCurStr + 2, 2);
    szTemp[2] = 0;
    nMon = atoi(szTemp);

    //������
    memcpy(szTemp, pCurStr + 4, 2);
    szTemp[2] = 0;
    nDay = atoi(szTemp);

    //����ʱ
    memcpy(szTemp, pCurStr + 6, 2);
    szTemp[2] = 0;
    nHour = atoi(szTemp);

    //�����
    memcpy(szTemp, pCurStr + 8, 2);
    szTemp[2] = 0;
    nMin = atoi(szTemp);

    //������
    memcpy(szTemp, pCurStr + 10, 2);
    szTemp[2] = 0;
    nSec = atoi(szTemp);

    //�����͵��ꡢ�¡��ա�ʱ���֡���ת����"YYYY-MM-DD HH:NN:SS"���ַ���
    sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d",
                nYear, nMon, nDay, nHour, nMin, nSec);

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The GetTextTimeInBCD Defining ******************/

/************ The GetTextTimeInBCD_4Y Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ�ְ�BCD�ģ���λ�꣩������ʱ����ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS.MS
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1
//
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
// Added by maimaoshi at 2002-10-15
/***************************************************************/
int CConvertFun::GetTextTimeInBCD_4Y(void* pCurField, const UINT4 nSrcFields,
                                     const void* pSrcFields[],
                                     const UINT4 nSrcFieldLen[],
                                     const SFieldInfo* pFI,
                                     const void* pDefault, const UINT4 nDefaultLen,
                                     int nIsQuery)
{
    int nYear, nMon, nDay, nHour, nMin, nSec;
    char szTemp[10];

    int    nIndex    = 0;      //�ı���ʽ��BCD���ַ�������
    int    nBcdIndex = 0;      //BCD����������
    BYTE   yCurBcd   = 0;      //��ǰBCD��ֵ
    BYTE*  pBcdArray = NULL;   //BCD������ָ��
    char*  pCurStr   = NULL;   //�ı���ʽ��BCD���ַ���ָ��


    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_TEXT_TIME_IN_BCD_4Y, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //��ʼ��BCD������ָ����ı���ʽ��BCD���ַ���ָ��
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    //˳��ɨ��BCD�������ÿһ��Ԫ��
    while(nBcdIndex < nSrcFieldLen[0])
    {
        //��ȡ��ǰBCD���ֵ
        yCurBcd = pBcdArray[nBcdIndex++];

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0xF0)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }

        //���ε�ǰBCD������ֵ�ĸ���λ
        yCurBcd &= 0x0F;

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0x0F)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = yCurBcd - 10 + 'A';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }
    }

    //�����ַ����Ľ�����
    pCurStr[nIndex] = '\0';

    //������
    memcpy(szTemp, pCurStr, 4);
    szTemp[4] = 0;
    nYear = atoi(szTemp);

    //������
    memcpy(szTemp, pCurStr + 4, 2);
    szTemp[2] = 0;
    nMon = atoi(szTemp);

    //������
    memcpy(szTemp, pCurStr + 6, 2);
    szTemp[2] = 0;
    nDay = atoi(szTemp);

    //����ʱ
    memcpy(szTemp, pCurStr + 8, 2);
    szTemp[2] = 0;
    nHour = atoi(szTemp);

    //�����
    memcpy(szTemp, pCurStr + 10, 2);
    szTemp[2] = 0;
    nMin = atoi(szTemp);

    //������
    memcpy(szTemp, pCurStr + 12, 2);
    szTemp[2] = 0;
    nSec = atoi(szTemp);

    //�����͵��ꡢ�¡��ա�ʱ���֡���ת����"YYYY-MM-DD HH:NN:SS"���ַ���
    sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d",
                nYear, nMon, nDay, nHour, nMin, nSec);

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The GetTextTimeInBCD_4Y Defining ******************/

/************ The GetTextTimeInBCD_T Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ�ְ�BCD��YYMMDDHHMMSST��6.5���ֽ�ת�����ı���ʽ��
//           YYYY-MM-DD HH:NN:SS.MS
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1
//
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//   Added by ���� at 2003-05-22
/***************************************************************/
int CConvertFun::GetTextTimeInBCD_T(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    int nYear, nMon, nDay, nHour, nMin, nSec, nTSec;
    char szTemp[10];

    int    nIndex    = 0;      //�ı���ʽ��BCD���ַ�������
    int    nBcdIndex = 0;      //BCD����������
    BYTE   yCurBcd   = 0;      //��ǰBCD��ֵ
    BYTE*  pBcdArray = NULL;   //BCD������ָ��
    char*  pCurStr   = NULL;   //�ı���ʽ��BCD���ַ���ָ��


    //ȷ�����������ȷ
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_TEXT_TIME_IN_BCD_T, 2);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0)
       || (pSrcFields[1] == NULL) || (nSrcFieldLen[1] == 0))
    {
        return -1;
    }

    //��ʼ��BCD������ָ����ı���ʽ��BCD���ַ���ָ��
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    //˳��ɨ��BCD�������ÿһ��Ԫ��
    while(nBcdIndex < nSrcFieldLen[0])
    {
        //��ȡ��ǰBCD���ֵ
        yCurBcd = pBcdArray[nBcdIndex++];

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0xF0)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }

        //���ε�ǰBCD������ֵ�ĸ���λ
        yCurBcd &= 0x0F;

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0x0F)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = yCurBcd - 10 + 'A';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }
    }

    //�����ַ����Ľ�����
    pCurStr[nIndex] = '\0';

    //�����꣬����λ������λ��
    memcpy(szTemp, pCurStr, 2);
    szTemp[2] = 0;
    nYear = 2000 + atoi(szTemp);

    //������
    memcpy(szTemp, pCurStr + 2, 2);
    szTemp[2] = 0;
    nMon = atoi(szTemp);

    //������
    memcpy(szTemp, pCurStr + 4, 2);
    szTemp[2] = 0;
    nDay = atoi(szTemp);

    //����ʱ
    memcpy(szTemp, pCurStr + 6, 2);
    szTemp[2] = 0;
    nHour = atoi(szTemp);

    //�����
    memcpy(szTemp, pCurStr + 8, 2);
    szTemp[2] = 0;
    nMin = atoi(szTemp);

    //������
    memcpy(szTemp, pCurStr + 10, 2);
    szTemp[2] = 0;
    nSec = atoi(szTemp);

    //����ʮ��֮һ��
    BYTE yTemp = *(BYTE*)pSrcFields[1];

    //�����ǰBCD��ֵС��10
    if(yTemp < 0x0A)
    {
        nTSec = yTemp;
    }
    //�����ǰBCD��ֵ����10
    else
    {
        nTSec = 0;
    }

    //�����͵��ꡢ�¡��ա�ʱ���֡���ת����"YYYY-MM-DD HH:NN:SS.T"���ַ���
    sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d.%02d",
                nYear, nMon, nDay, nHour, nMin, nSec, nTSec);

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The GetTextTimeInBCD_T Defining ******************/

/************ The Int2IpText Defining ****************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ�ֽ�������������ת�����ı�
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           nSrcFieldLen[0]ֻ��Ϊ5����ʾpSrcFields[0]
//           ָ����������ΪUINT4�͵�����
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
// Added by maimaoshi at 2002-05-17
/***************************************************************/
int CConvertFun::Int2IpText(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    UINT4 nTmp = 0;

    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_INT_TO_IP_TEXT, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //��ȡԭ��ʽ�������ֵ
    if(nSrcFieldLen[0] == LEN_UINT4)
    {
        UINT4 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT4);
        nTmp = uTmp;
    }
    else
    {
        return -1;
    }

    //������ֵת������Ӧ���ַ���
    ACE_INET_Addr addr((u_short)0,(ACE_UINT32)nTmp);

    strcpy((char*)pCurField, addr.get_host_addr());

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The Int2IpText Defining *********************/

/************ The BCDToSecond Defining **************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ������BCD���ͨ��ʱ��ת�����룬
//           (4���ֽ�BCD���ʽΪ:HHHMMSST)
//           (HHH:0~255Сʱ;MM:0~59����;SS:0~59��;T:0-9����)
//           ת��ʱ�������������ΪBCD���������
//           1��BCD�����0xF��2���ﵽBCD���������󳤶�
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           pSrcFields[0]ָ������BCD�����飬
//           nSrcFieldLen[0]ָʾ������BCD������ĳ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::BCDToSecond(void* pCurField, const UINT4 nSrcFields,
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    int    nIndex    = 0;      //�ı���ʽ��BCD���ַ�������
    int    nBcdIndex = 0;      //BCD����������
    BYTE   yCurBcd   = 0;      //��ǰBCD��ֵ
    BYTE*  pBcdArray = NULL;   //BCD������ָ��
    char*  pCurStr   = NULL;   //�ı���ʽ��BCD���ַ���ָ��


    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_BCD_TO_SECOND, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //��ʼ��BCD������ָ����ı���ʽ��BCD���ַ���ָ��
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    //˳��ɨ��BCD�������ÿһ��Ԫ��
    while(nBcdIndex < nSrcFieldLen[0])
    {
        //��ȡ��ǰBCD���ֵ
        yCurBcd = pBcdArray[nBcdIndex++];

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0xF0)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }

        //���ε�ǰBCD������ֵ�ĸ���λ
        yCurBcd &= 0x0F;

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0x0F)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = yCurBcd - 10 + 'A';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }
    }

    //�����ַ����Ľ�����
    pCurStr[nIndex] = '\0';

    UINT4 uSecondCount = 0;
    UINT4 uTemp = 0;

    if (nIndex == 8)
    {
        //ת��Сʱ���룬ǰ��3���ַ�
        uTemp = uTemp + (pCurStr[0] - '0')*100;
        uTemp = uTemp + (pCurStr[1] - '0')*10;
        uTemp = uTemp + (pCurStr[2] - '0');

        uSecondCount = uSecondCount + uTemp * 3600;
        uTemp = 0;

        //ת�����ӵ��룬����2���ַ�
        uTemp = uTemp + (pCurStr[3] - '0')*10;
        uTemp = uTemp + (pCurStr[4] - '0');

        uSecondCount = uSecondCount + uTemp * 60;
        uTemp = 0;

        //ת���������룬����2���ַ�
        uTemp = uTemp + (pCurStr[5] - '0')*10;
        uTemp = uTemp + (pCurStr[6] - '0');

        uSecondCount = uSecondCount + uTemp;
        uTemp = 0;

        //ת�����뵽�룬���1���ַ�
        //uTemp = uTemp + (pCurStr[7] - '0')*0.1;

        //uSecondCount = uSecondCount + uTemp;
        //uTemp = 0;

    }
    else
    {
        uTemp = 0;
    }

    //��ת�������ַ���
    sprintf((char*)pCurField, "%d", uSecondCount);

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The BCDToSecond Defining *********************/

/************ The GetTextTimeNoSp Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ���ꡢ�¡��ա�ʱ���֡���ת�����ı���ʽ��YYYYMMDDHHNNSS
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ6
//           pSrcFields[0]ָ����������ΪBYTE��SHORT�͵�2λ���ꡱ��
//           pSrcFields[1]��pSrcFields[2]��pSrcFields[3]��pSrcFields[4]��
//           pSrcFields[5]�ֱ�ָ��BYTE���¡��ա�ʱ���֡��룬
//           ���nSrcFieldLen[0]Ϊ1����ʾpSrcFields[0]ΪBYTE�͵��꣬
//           ���nSrcFieldLen[0]Ϊ2����ʾpSrcFields[0]ΪSHORT�͵���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::GetTextTimeNoSp(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    INT2  nYear =  0;    //�����λ�����ʱ����
    BYTE  nMon =   0;    //����µ���ʱ����
    BYTE  nDate =  0;    //����յ���ʱ����
    BYTE  nHour =  0;    //���ʱ����ʱ����
    BYTE  nMin =   0;    //��ŷֵ���ʱ����
    BYTE  nSec =   0;    //��������ʱ����

    //ȷ�����������ȷ
    if(nSrcFields != 6)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_TEXT_TIME_NO_SP, 6);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //�����Ϊ��ΪSHORT����
    if(nSrcFieldLen[0] == LEN_INT2)
    {
        //nYear = *(INT2*)pSrcFields[0];
        memcpy((void*)&nYear, pSrcFields[0], LEN_INT2);

        //CC08�������ж������ֽڵ�2λ�꣬��ݱ����ڸ��ֽڣ�
        //������Ҫ�Ƶ����ֽ�
        nYear = nYear >> 8;
    }

    //�����Ϊ��ΪBYTE����
    else if(nSrcFieldLen[0] == LEN_BYTE)
    {
        //nYear = *(BYTE*)pSrcFields[0];
        BYTE yTmp = *(BYTE*)pSrcFields[0];
        nYear = yTmp;
    }

    //�Ƿ�����
    else
    {
        return -1;
    }

    //����Ϊ�����2000��������Ŀ���ʽ��ǰ��
    nYear += 2000;

    //��ȡ�¡��ա�ʱ���֡����ֵ
    nMon   =  *(BYTE*)pSrcFields[1];
    nDate  =  *(BYTE*)pSrcFields[2];
    nHour  =  *(BYTE*)pSrcFields[3];
    nMin   =  *(BYTE*)pSrcFields[4];
    nSec   =  *(BYTE*)pSrcFields[5];

    //Added by ZhengYuqun SWPD02506 2003-11-24
    if((nYear - 2000) == 0xFF || nMon == 0xFF || nDate == 0xFF || nHour == 0xFF || nMin == 0xFF || nSec == 0xFF)
    {
        ((char*)pCurField)[0] = '\0';
    }
    else
    { //End SWPD02506
        //�����͵��ꡢ�¡��ա�ʱ���֡���ת����"YYYYMMDDHHNNSS"���ַ���
        sprintf((char*)pCurField, "%4d%02d%02d%02d%02d%02d",
                    nYear, nMon, nDate, nHour, nMin, nSec);
    }

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The GetTextTimeNoSp Defining ******************/

/************ The FillFF Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ��ĳ�ֶ���"FF"
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::FillFF(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_TEXT_FILLFF, 1);
        return -1;
    }

    if((pCurField == NULL) || (nSrcFields != 1) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //Modified by ZhengYuqun SWPD02477 2003-11-21

    //����Ŀ���ֶε��������;���������ַ���������ת����ʮ�����Ƶ��ֽڴ�
    switch(pFI->nDataType)
    {
    case FT_STRING:
        sprintf((char*)pCurField, "%s", "FF");
        //��Ŀ���ʽ�����������ͷָ���
        if(!nIsQuery)
        {
            //�������Ϊ��ѯ����ת��
            return AddFillerSeparator((char*)pCurField, pFI);
        }
        break;

    //�������Ͱ���ʵ�ʳ������0xFF
    case FT_CHAR:
    case FT_BYTE:
    case FT_BCD:
    case FT_RBCD:
        memset(pCurField, 0xFF, pFI->nFieldLength);
        break;

    case FT_SHORT:
    case FT_USHORT:
        memset(pCurField, 0xFF, 2 * pFI->nFieldLength);
        break;

    case FT_LONG:
    case FT_ULONG:
        memset(pCurField, 0xFF, 4 * pFI->nFieldLength);
        break;

    case FT_INT8:
        memset(pCurField, 0xFF, 8 * pFI->nFieldLength);
        break;

    default:
        memset(pCurField, 0xFF, pFI->nFieldLength);
        break;
    }
    //End SWPD02477


    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The FillFF Defining ******************/

/************ The ChangeGSVNandCallType Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ��SoftX300������ϸ�����ĺ�������ҵ�����ͽ���ת��
//           ҵ������Ϊ���س�;�Ļ�����ҵ�����Զ�Ҫ��Ϊ���֣�
//           ��������ԭ���ǳ��ָ�Ϊ���֣�ԭ���ǻ�ӵĸ�Ϊ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ2
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
#define GSVN_LOCAL_OFFICE           0   //����
#define GSVN_LOCAL                  1   //����
#define GSVN_LOCAL_TOLL             2   //���س�;
#define GSVN_NATIONAL_TOLL          3   //���ڳ�;

#define CALL_TYPE_INTRA_OFFICE      1   //����
#define CALL_TYPE_INCOMING_OFFICE   2   //���
#define CALL_TYPE_OUTGOING_OFFICE   3   //����
#define CALL_TYPE_TANDEM            4   //���

int CConvertFun::ChangeGSVNandCallType(void* pCurField, const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_CHANGE_GSVN_AND_CALLTYPE, 1);
        return -1;
    }
    if((pCurField == NULL) || (nSrcFields != 1) || (pSrcFields == NULL))
    {
        return -1;
    }

    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }


    BYTE nGSVN;          //���ҵ������
    BYTE nCallType;      //��ź������
    BYTE nTemp;          //���ҵ�����ͺͺ������ҵ�����ͺͺ�������ռ����ֽ�

    memcpy(&nTemp, pSrcFields[0], nSrcFieldLen[0]);
    nGSVN = nTemp >> 4;
    nCallType = nTemp & 0x0f;

    if(nGSVN == GSVN_LOCAL_TOLL)  //ҵ������Ϊ���س�;
    {
        nGSVN = GSVN_LOCAL_OFFICE;
        if(nCallType == CALL_TYPE_OUTGOING_OFFICE)
        {
            nCallType = CALL_TYPE_INTRA_OFFICE;
        }

        if(nCallType == CALL_TYPE_TANDEM)
        {
            nCallType = CALL_TYPE_INCOMING_OFFICE;
        }

        nTemp = nGSVN << 4;
        nTemp += nCallType;
    }

    memcpy(pCurField, &nTemp, nSrcFieldLen[0]);

    return 0;
}

int CConvertFun::ChangeGSVNandCallType2Text(void* pCurField, const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_CHANGE_GSVN_AND_CALLTYPE_TEXT, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL))
    {
        return -1;
    }

    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }


    BYTE nGSVN;          //���ҵ������
    BYTE nCallType;      //��ź������
    BYTE nTemp;          //���ҵ�����ͺͺ������ҵ�����ͺͺ�������ռ����ֽ�

    memcpy(&nTemp, pSrcFields[0], nSrcFieldLen[0]);
    nGSVN = nTemp >> 4;
    nCallType = nTemp & 0x0f;

    if(nGSVN == GSVN_LOCAL_TOLL)  //ҵ������Ϊ���س�;
    {
        nGSVN = GSVN_LOCAL_OFFICE;
        if(nCallType == CALL_TYPE_OUTGOING_OFFICE)
        {
            nCallType = CALL_TYPE_INTRA_OFFICE;
        }

        if(nCallType == CALL_TYPE_TANDEM)
        {
            nCallType = CALL_TYPE_INCOMING_OFFICE;
        }
    }

    sprintf((char*)pCurField, "%-24d;%-24d", nCallType, nGSVN);

    //��Ŀ���ʽ�����������ͷָ���
    return AddFillerSeparator((char*)pCurField, pFieldInfo);
}

/************ End of The ChangeGSVNandCallType Defining ******************/

//by ldf 2003-01-08 ��Ӧ���ⵥD05886 D05556��ͬD05415 D5150��
/************ The Int2IpTextDirect Defining ****************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ�ֽ�������������ת�����ı�,���������ֽ�������⣬��Ҫ��������һ��������
//           �������� (10)(11)(243)(197) ������ ���Ϊ 197.243.11.10
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           nSrcFieldLen[0]ֻ��Ϊ5����ʾpSrcFields[0]
//           ָ����������ΪUINT4�͵�����
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
/***************************************************************/
int CConvertFun::Int2IpTextDirect(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    UINT4 nTmp = 0;
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_INT_TO_IP_TEXT_DIRECT, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //��ȡԭ��ʽ�������ֵ
    if(nSrcFieldLen[0] == LEN_UINT4)
    {
        UINT4 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT4);
        nTmp = uTmp;
    }
    else
    {
        return -1;
    }

    //������ֵת������Ӧ���ַ���
    BYTE pTmpBuf[4];

    //�Ȱ�UINT4��4�ֽڷֲ𵽻���������
    memset((void*)pTmpBuf, 0, 4);
    memcpy((void*)pTmpBuf, &nTmp, 4);

    char szIpAddr[256];

    sprintf(szIpAddr, "%d.%d.%d.%d\0", pTmpBuf[3],pTmpBuf[2],pTmpBuf[1],pTmpBuf[0]);

    strcpy((char*)pCurField, szIpAddr);

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The Int2IpTextDirect Defining *********************/

/************ The Array2Text Defining ****************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ�ֽ�������������ת�����ı�
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ָ����������ΪBYTE�͵�����
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::Array2Text(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_ARRAY_TO_TEXT, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    int nBufLen = nSrcFieldLen[0];

    char*  pCurStr   = NULL;
    pCurStr = (char*)pCurField;

    BYTE*  pArray = NULL;
    pArray = (BYTE*)pSrcFields[0];

    for(int nIndex=0; nIndex < nBufLen; nIndex++)
    {
        int yCurNum = (int)pArray[nIndex];
        sprintf ( (char *)&pCurStr[nIndex*2], "%02d", yCurNum );
    }

    pCurStr[nBufLen*2] ='\0';


    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}

/************ End of The Array2Text Defining *********************/
//�޸����


/************ The Array2HexString Defining ****************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ�ֽ�������������ת�����ı�
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ָ����������ΪBYTE�͵�����
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::Array2HexString(void* pCurField, const UINT4 nSrcFields,
                                 const void* pSrcFields[],
                                 const UINT4 nSrcFieldLen[],
                                 const SFieldInfo* pFI,
                                 const void* pDefault, const UINT4 nDefaultLen,
                                 int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_ARRAY_TO_HEX_STRING, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    UINT4 nByteCount = nSrcFieldLen[0];
    for(UINT4 nIndex=0; nIndex < nByteCount; nIndex++)
    {
        BYTE yCurNum = ((BYTE*)pSrcFields[0])[nIndex];
        sprintf (&((char *)pCurField)[nIndex*3], "%02x ", yCurNum );
    }

    ((char *)pCurField)[nByteCount*3] ='\0';


    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The Array2HexString Defining *********************/

/************ The FillInvaidValue Defining ****************************/
// �������������ֽ����FF,Ϊת����ʹ��
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ָ����������ΪBYTE�͵�����
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::FillInvaidValue(void* pCurField, const UINT4 nSrcFields,
                                 const void* pSrcFields[],
                                 const UINT4 nSrcFieldLen[],
                                 const SFieldInfo* pFI,
                                 const void* pDefault, const UINT4 nDefaultLen,
                                 int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_FILL_INVALID_VALUE, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //Ŀ���ʽ����ĳ���
    UINT4 nByteCount = pFI->nFieldLength;

    for(UINT4 nIndex=0; nIndex < nByteCount; nIndex++)
    {
        ((BYTE *)pCurField)[nIndex] = 0XFF;
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The FillInvaidValue Defining *********************/


/************ The GetTextTimeEx Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ���ꡢ�¡��ա�ʱ���֡���ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS.MS
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ6����7
//           pSrcFields[0]ָ����������ΪSHORT�͵�4λ���ꡱ��
//           pSrcFields[1]��pSrcFields[2]��pSrcFields[3]��pSrcFields[4]��
//           pSrcFields[5]�ֱ�ָ��BYTE���¡��ա�ʱ���֡���
//           pSrcFields[6]�������,���ʾ����
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::GetTextTimeEx(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    INT2  nYear =  0;    //�����λ�����ʱ����
    BYTE  nMon =   0;    //����µ���ʱ����
    BYTE  nDate =  0;    //����յ���ʱ����
    BYTE  nHour =  0;    //���ʱ����ʱ����
    BYTE  nMin =   0;    //��ŷֵ���ʱ����
    BYTE  nSec =   0;    //��������ʱ����
    BYTE  nMilliSec = 0; //��ź������ʱ����

    //ȷ�����������ȷ
    if((pCurField == NULL) || (nSrcFields != 6 && nSrcFields != 7) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //��ȡ�ꡢ�¡��ա�ʱ���֡����ֵ
    nYear  =  *(INT2*)pSrcFields[0];
    nMon   =  *(BYTE*)pSrcFields[1];
    nDate  =  *(BYTE*)pSrcFields[2];
    nHour  =  *(BYTE*)pSrcFields[3];
    nMin   =  *(BYTE*)pSrcFields[4];
    nSec   =  *(BYTE*)pSrcFields[5];

    //Added by ZhengYuqun SWPD02506 2003-11-24
    if(nYear == 0xFFFF || nMon == 0xFF || nDate == 0xFF || nHour == 0xFF || nMin == 0xFF || nSec == 0xFF)
    {
        ((char*)pCurField)[0] = '\0';
    }//End SWPD02506
    else if(nSrcFields == 7)
    {
        nMilliSec = *(BYTE*)pSrcFields[6];
        //�����͵��ꡢ�¡��ա�ʱ���֡���ת����"YYYY-MM-DD HH:NN:SS"���ַ���
        sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d.%d",
            nYear, nMon, nDate, nHour, nMin, nSec, nMilliSec);
    }
    else
    {
        //�����͵��ꡢ�¡��ա�ʱ���֡���ת����"YYYY-MM-DD HH:NN:SS"���ַ���
        sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d",
                nYear, nMon, nDate, nHour, nMin, nSec);
    }

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The GetTextTime Defining ******************/

//added by maimaoshi ,2003-03-10
/************ The ToLRLN Defining ****************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ����д"\r\n"
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
//     ע�⣺��ʱ��֧��INT8����ת��
/***************************************************************/
int CConvertFun::ToLRLN(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_TO_LRLN, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //�����ֶ���"\r\n"
    sprintf((char*)pCurField, "%s", "\r\n");

    return 0;
}
/************ End of The ToLRLN Defining *********************/

/************ The ToLR Defining ****************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ����д"\r"
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
//     ע�⣺��ʱ��֧��INT8����ת��
/***************************************************************/
int CConvertFun::ToLR(void* pCurField, const UINT4 nSrcFields,
                      const void* pSrcFields[],
                      const UINT4 nSrcFieldLen[],
                      const SFieldInfo* pFI,
                      const void* pDefault, const UINT4 nDefaultLen,
                      int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_TO_LR, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //�����ֶ���"\r"
    sprintf((char*)pCurField, "%s", "\r");

    return 0;
}
/************ End of The ToLR Defining *********************/


/************ The ToLN Defining ****************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ����д"\n"
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
//     ע�⣺��ʱ��֧��INT8����ת��
/***************************************************************/
int CConvertFun::ToLN(void* pCurField, const UINT4 nSrcFields,
                      const void* pSrcFields[],
                      const UINT4 nSrcFieldLen[],
                      const SFieldInfo* pFI,
                      const void* pDefault, const UINT4 nDefaultLen,
                      int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_TO_LN, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //�����ֶ���"\r"
    sprintf((char*)pCurField, "%s", "\n");

    return 0;
}
/************ End of The ToLN Defining *********************/


/************ The StringCopy Defining ***************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ���ַ����Ŀ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           pSrcFields[0]ָ��Դ���ݿ���ڴ棬
//           nSrcFieldLen[0]��ʾԴ���ݿ�ĳ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::StringCopy(void* pCurField, const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery)
{
    INT4 nEnumVal = 0;
    ENUMINFO_LIST::iterator item;       //ö�����������
    ENUMINFO_LIST* pEnumInfos = NULL;   //��ʽ��ö������ָ�����

    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_STRING_COPY, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //��ԭ��ʽ��Ӧ���е��������ݿ�����Ŀ���ʽ��ǰ����
    strncpy((char*)pCurField, (const char*)pSrcFields[0], nSrcFieldLen[0]);
    ((char*)pCurField)[nSrcFieldLen[0]] = '\0';

    //�����ǰ����ö������
    if(pFieldInfo->flgEnum != 0)
    {
        StringLRTrim((char*)pCurField);
        nEnumVal = atol((char*)pCurField);

        //��ȡ��ʽ��ö������
        pEnumInfos = (ENUMINFO_LIST*)(pFieldInfo->pEnumInfos);
        if(pEnumInfos == NULL)
        {
            return -1;
        }

        //˳��������ʽ��ö�������е�ÿһ���ڵ�
        item = pEnumInfos->begin();
        while(item != pEnumInfos->end())
        {
            //�����ǰ�ڵ��ö��ֵ������Ҫ��ö��ֵ
            if(item->nEnumValue == nEnumVal)
            {
                //��ȡ��ǰö��ֵ��Ӧ��ö���ַ�����Ϣ
                strcpy((char*)pCurField, item->szEnumValue);
                break;
            }

            item++;
        }

    }

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFieldInfo);
    }


    return 0;
}
/************ End of The StringCopy Defining ********************/
/************ The ByteText2 Defining ****************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ�ֽ�������������ת�����ı�
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           nSrcFieldLen[0]Ϊ1��2��4��8ʱ�ֱ��ʾpSrcFields[0]
//           ָ����������ΪBYTE��SHORT��LONG��INT8�͵�����
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
//     ע�⣺��ʱ��֧��INT8����ת��
/***************************************************************/
int CConvertFun::ByteText2(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    INT4  nTmp = 0;    //���BYTE��SHORT��LONG����ʱ����



    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_BYTE_TEXT2, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //��ȡԭ��ʽ�������ֵ
    if(nSrcFieldLen[0] == LEN_BYTE)
    {
        BYTE uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_BYTE);
        nTmp = uTmp;
    }
    else
    {
        return -1;
    }

    //������ֵת������Ӧ���ַ���
    sprintf((char*)pCurField, "%02d", nTmp);

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The ByteText2 Defining *********************/


/************ The GetTextTimeInString Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ���ꡢ�¡��ա�ʱ���֡���ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS.MS
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ6
//           pSrcFields[0]ָ����������ΪBYTE��SHORT�͵�2λ���ꡱ��
//           pSrcFields[1]��pSrcFields[2]��pSrcFields[3]��pSrcFields[4]��
//           pSrcFields[5]�ֱ�ָ��BYTE���¡��ա�ʱ���֡��룬
//           ���nSrcFieldLen[0]Ϊ1����ʾpSrcFields[0]ΪBYTE�͵��꣬
//           ���nSrcFieldLen[0]Ϊ2����ʾpSrcFields[0]ΪSHORT�͵���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::GetTextTimeInString(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    INT2  nYear =  0;    //�����λ�����ʱ����
    BYTE  nMon =   0;    //����µ���ʱ����
    BYTE  nDate =  0;    //����յ���ʱ����
    BYTE  nHour =  0;    //���ʱ����ʱ����
    BYTE  nMin =   0;    //��ŷֵ���ʱ����
    BYTE  nSec =   0;    //��������ʱ����
    char  szTmp[5];

    //ȷ�����������ȷ
    if((pCurField == NULL) || (nSrcFields != 3 && nSrcFields != 6) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }

    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    if (nSrcFields == 6)
    {
        //��ȡ�ꡢ�¡��ա�ʱ���֡����ֵ
        strncpy(szTmp, (const char*)pSrcFields[0], nSrcFieldLen[0]);
        szTmp[nSrcFieldLen[0]] = '\0';
        nYear = atoi(szTmp);

        strncpy(szTmp, (const char*)pSrcFields[1], nSrcFieldLen[1]);
        szTmp[nSrcFieldLen[1]] = '\0';
        nMon = atoi(szTmp);

        strncpy(szTmp, (const char*)pSrcFields[2], nSrcFieldLen[2]);
        szTmp[nSrcFieldLen[2]] = '\0';
        nDate = atoi(szTmp);

        strncpy(szTmp, (const char*)pSrcFields[3], nSrcFieldLen[3]);
        szTmp[nSrcFieldLen[3]] = '\0';
        nHour = atoi(szTmp);

        strncpy(szTmp, (const char*)pSrcFields[4], nSrcFieldLen[4]);
        szTmp[nSrcFieldLen[4]] = '\0';
        nMin = atoi(szTmp);

        strncpy(szTmp, (const char*)pSrcFields[5], nSrcFieldLen[5]);
        szTmp[nSrcFieldLen[5]] = '\0';
        nSec = atoi(szTmp);


        //�����͵��ꡢ�¡��ա�ʱ���֡���ת����"YYYY-MM-DD HH:NN:SS"���ַ���
        sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d",
                nYear, nMon, nDate, nHour, nMin, nSec);
    }
    else if (nSrcFields == 3)
    {
        strncpy(szTmp, (const char*)pSrcFields[0], nSrcFieldLen[0]);
        szTmp[nSrcFieldLen[0]] = '\0';
        nHour = atoi(szTmp);

        strncpy(szTmp, (const char*)pSrcFields[1], nSrcFieldLen[1]);
        szTmp[nSrcFieldLen[1]] = '\0';
        nMin = atoi(szTmp);

        strncpy(szTmp, (const char*)pSrcFields[2], nSrcFieldLen[2]);
        szTmp[nSrcFieldLen[2]] = '\0';
        nSec = atoi(szTmp);


        //�����͵��ꡢ�¡��ա�ʱ���֡���ת����"YYYY-MM-DD HH:NN:SS"���ַ���
        sprintf((char*)pCurField, "%02d:%02d:%02d", nHour, nMin, nSec);
    }
    else
    {
        return -1;
    }


    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The GetTextTimeInString Defining ******************/
//end by maimashi 2003-03-10
//by ldf 2003-04-01
int CConvertFun::CheckSum(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    if (!pCurField || nSrcFieldLen <= 0 || !pSrcFields)
    {
        return -1;
    }

    UINT4 nFieldLen = nSrcFieldLen[0];
    unsigned char* pData = (unsigned char*)pSrcFields[0];
    unsigned char uCheckSum = 0;

    UINT4 nIndex = 0;
    //����У��ͣ���128����У��͵ķ�����ֻ�������ֽ��ۼ�
    for(UINT4 i = 0; i < nSrcFields; i++)
    {
        pData = (unsigned char*)pSrcFields[i];
        nIndex = 0;
        nFieldLen = nSrcFieldLen[i];
        while (nIndex < nFieldLen)
        {
            uCheckSum += pData[nIndex];
            nIndex ++;
        }
    }

    memcpy(pCurField, &uCheckSum, 1);

    return 0;
}
//end

/************ The FillWithStr Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ��ĳ�ֶ���дĳһ���������ַ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//�޸ļ�¼��
//2003-11-20 ֣��Ⱥ  ʹ��֧�ֶ�����Ҳ֧���ı�
/***************************************************************/
int CConvertFun::FillWithStr(void* pCurField, const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_FILL_WITH_STR, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //Modified by ZhengYuqun SWPD02477 2003-11-21

    //����Ŀ���ֶε��������;���������ַ���������ת����ʮ�����Ƶ��ֽڴ�
    if(pFI->nDataType == FT_STRING) //ֻ��STRING���Ͱ��ַ���ת
    {
        int nLen = min(nSrcFieldLen[0], pFI->nFieldLength);
        memcpy(pCurField, pSrcFields[0], nLen);
        //��Ŀ���ʽ�����������ͷָ���
        if(!nIsQuery)
        {
            //�������Ϊ��ѯ����ת��
            return AddFillerSeparator((char*)pCurField, pFI);
        }
    }
    else
    {
        int nLen = pFI->nFieldLength;
        switch(pFI->nDataType)
        {
            case FT_SHORT:
            case FT_USHORT:
                nLen *= 2;
                break;

            case FT_LONG:
            case FT_ULONG:
                nLen *= 4;
                break;

            case FT_INT8:
                nLen *= 8;
                break;

            default:
                break;
                //��������1�ֽڳ��ȴ���
        }

        //����ϳ��ַ���Ȼ����ת��Ϊʮ�����ƴ�
        char szTemp[FMT_FIELD_MAX_LEN * 2 + 1];
        char cFillChar = pFI->cFillChar;

        memcpy(szTemp, pSrcFields[0], nSrcFieldLen[0]);
        if(cFillChar == ' ')  //��������ǿո���ô��Ϊ0x00
        {
            cFillChar = '0';
        }

        //����Ŀ���ֶγ��ȵ�2���������������szTemp
        if(nSrcFieldLen[0] - 1 < nLen * 2)
        {
            memset(szTemp + nSrcFieldLen[0] - 1, cFillChar, nLen * 2 - nSrcFieldLen[0] + 1);
            szTemp[nLen * 2] = '\0';
        }

        StringtoBCD(szTemp, (BYTE*)pCurField, nLen);
    }

    //End SWPD02477

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;

}
/************ End of The FillWithStr Defining ******************/
/************ The FillByIniFile Defining *************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ʵ������ʽ�������һ��INI�ļ��е�ָ����ֵ
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ2��
//           pSrcFields[0]ָ����������Ϊchar��
//           pSrcFields[1]ָ����������Ϊchar
//           
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::FillByIniFile(void* pCurField, const UINT4 nSrcFields, 
                               const void* pSrcFields[],
                               const UINT4 nSrcFieldLen[],
                               const SFieldInfo* pFI,
                               const void* pDefault, const UINT4 nDefaultLen,
                               int nIsQuery)
{
	//ȷ�����������ȷ
    if((pCurField == NULL) || (nSrcFields != 2) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //����Ŀ���ֶε��������;����Ƿ����INI�ļ��е�ֵ
    if((pFI->nDataType == FT_STRING)  || (pFI->nDataType == FT_CHAR))
    {
        static MAP<STRING, STRING> iniFileMap;
        static ACE_Mutex   mutex;  

        STRING strKeyName = (char*)pSrcFields[0];
        strKeyName += (char*)pSrcFields[1];
        
        mutex.acquire();

        MAP<STRING, STRING>::iterator iter;
        iter = iniFileMap.find(strKeyName);
        if(iter == iniFileMap.end())
        {
            //������Map��û�����ݣ����ļ��м���
            CINIFile iniFile("./config/ini/igwb.ini");
            if(!iniFile.Open())
            {
                mutex.release();
                return -1;
            }

            char szValue[64] = "";
            iniFile.GetString((char*)pSrcFields[0],              
                              (char*)pSrcFields[1],                      //APName
                              "",
                              szValue,
                              sizeof(szValue));
            szValue[sizeof(szValue) - 1] = '\0';

            iniFileMap[strKeyName] = szValue;
        }

	    int nLen = min(iniFileMap[strKeyName].length() + 1, pFI->nFieldLength);
        memcpy(pCurField, iniFileMap[strKeyName].c_str(), nLen);
        
        mutex.release();
	    //��Ŀ���ʽ�����������ͷָ���
        if(!nIsQuery)
        {
            //�������Ϊ��ѯ����ת��
            return AddFillerSeparator((char*)pCurField, pFI);
        }
    }
    else
    {
        return -1;
    }


    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;

}
/************ End of The FillByIniFile Defining ******************/


#define AREA_CODE_COUNT 108
const char* g_szAreaCode[AREA_CODE_COUNT] = {
        "022  1 ���.���",
        "024  5 ����.����",
        "0310 2 �ӱ�.����",
        "0311 2 �ӱ�.ʯ��ׯ",
        "0312 2 �ӱ�.����",
        "0313 2 �ӱ�.�żҿ�",
        "0314 2 �ӱ�.�е�",
        "0315 2 �ӱ�.��ɽ",
        "0316 2 �ӱ�.�ȷ�",
        "0317 2 �ӱ�.����",
        "0318 2 �ӱ�.��ˮ",
        "0319 2 �ӱ�.��̨",
        "0335 2 �ӱ�.�ػʵ�",
        "0349 8 ɽ��.˷��",
        "0350 8 ɽ��.����",
        "0351 8 ɽ��.̫ԭ",
        "0352 8 ɽ��.��ͬ",
        "0353 8 ɽ��.��Ȫ",
        "0354 8 ɽ��.�ܴ�",
        "0355 8 ɽ��.����",
        "0356 8 ɽ��.����",
        "0357 8 ɽ��.�ٷ�",
        "0358 8 ɽ��.��ʯ",
        "0359 8 ɽ��.�˳�",
        "0370 9 ����.����",
        "0371 9 ����.֣��",
        "0372 9 ����.����",
        "0373 9 ����.����",
        "0374 9 ����.���",
        "0375 9 ����.ƽ��ɽ",
        "0376 9 ����.����",
        "0377 9 ����.����",
        "0378 9 ����.����",
        "0379 9 ����.����",
        "0391 9 ����.����",
        "0392 9 ����.�ױ�",
        "0393 9 ����.���",
        "0394 9 ����.�ܿ�",
        "0395 9 ����.���",
        "0396 9 ����.פ���",
        "0397 9 ����.�괨",
        "0398 9 ����.����Ͽ",
        "0410 5 ����.����",
        "0411 5 ����.����",
        "0412 5 ����.��ɽ",
        "0413 5 ����.��˳",
        "0414 5 ����.��Ϫ",
        "0415 5 ����.����",
        "0416 5 ����.����",
        "0417 5 ����.Ӫ��",
        "0418 5 ����.����",
        "0419 5 ����.����",
        "0421 5 ����.����",
        "0427 5 ����.�̽�",
        "0429 5 ����.��«��",
        "0431 6 ����.����",
        "0432 6 ����.����",
        "0433 6 ����.�ӱ�",
        "0434 6 ����.��ƽ",
        "0435 6 ����.ͨ��",
        "0436 6 ����.�׳�",
        "0437 6 ����.��Դ",
        "0438 6 ����.��ԭ",
        "0439 6 ����.��ɽ",
        "0440 6 ����.����",
        "0448 6 ����.÷�ӿ�",
        "0451 7 ������.������",
        "0452 7 ������.�������",
        "0453 7 ������.ĵ����",
        "0454 7 ������.��ľ˹",
        "0455 7 ������.�绯",
        "0456 7 ������.�ں�",
        "0457 7 ������.���˰���",
        "0458 7 ������.����",
        "0459 7 ������.����",
        "0464 7 ������-��̨��",
        "0467 7 ������-����",
        "0468 7 ������-�׸�",
        "0469 7 ������-˫Ѽɽ",
        "0470 4 ���ɹ�.���ױ�����",
        "0471 4 ���ɹ�.���ͺ���",
        "0472 4 ���ɹ�.��ͷ",
        "0473 4 ���ɹ�.�ں�",
        "0474 4 ���ɹ�.�����鲼��",
        "0475 4 ���ɹ�.ͨ��",
        "0476 4 ���ɹ�.���",
        "0477 4 ���ɹ�.��������",
        "0478 4 ���ɹ�.����׿����",
        "0479 4 ���ɹ�.���ֺ���",
        "0482 4 ���ɹ�-�˰���",
        "0483 4 ���ɹ�-��������",
        "0530 3 ɽ��.����",
        "0531 3 ����",
        "0532 3 ɽ��.�ൺ",
        "0533 3 ɽ��.�Ͳ�",
        "0534 3 ɽ��.����",
        "0535 3 ɽ��.��̨",
        "0536 3 ɽ��.Ϋ��",
        "0537 3 ɽ��.����",
        "0538 3 ɽ��.̩��",
        "0539 3 ɽ��.����",
        "0543 3 ɽ��.����",
        "0546 3 ɽ��.��Ӫ",
        "0631 3 ɽ��.����",
        "0632 3 ɽ��.��ׯ",
        "0633 3 ɽ��.����",
        "0634 3 ɽ��.����",
        "0635 3 ɽ��.�ĳ�"
    };

/****************** The GetAreaID Defining *************************/
// ������������ȡ�绰�����ǰnλ����ת������Ӧ������,
//           ʵ�ִ����к����з�������ID
//
// ���������
//  nParasNum�������������˺����Ĳ�������Ӧ��Ϊ2
//     pParas���������飬�˺�����������������pParas[0]��ʾ�绰�����ַ���
//             pParas[1]��ʾҪ��ȡ�绰�����ǰn������
//  nParasLen����������������
//   pDefault������ȱʡֵ
//
// ���������SVariant���͵Ľ��
//
//   ����ֵ������ɹ����򷵻�0����������ط���
//
/****************************************************************/
int CConvertFun::GetAreaID(const UINT4 nParasNum,
                           const void* pParas[],
                           const UINT4 nParasLen[],
                           const STRING& pDefault,
                           SVariant& stVar)
{
    //ȷ�������������ȷ��
    if((nParasNum != 1) || (pParas == NULL))
    {
        return -1;
    }

    //ȷ��������ȷ
    for(int i = 0; i < nParasNum; i++)
    {
        if((pParas[i] == NULL) || nParasLen[i] == 0)
        {
            return -1;
        }
    }

    static MAP<STRING, int> map_code_to_id;
    static ACE_Mutex mutex;

    mutex.acquire();
    if(map_code_to_id.size() == 0)
    {
        char szAreaCode[10];
        char szTemp[100];
        int  nAreaID;

        FILE* f = fopen("./config/area/area.map", "rt");
        if(f != NULL)
        {
            char szLine[1024];
            while(!feof(f))
            {
                char* p = fgets(szLine, sizeof(szLine), f);
                if(p != NULL)
                {
                    int nRead = sscanf(szLine, "%s%d%s", szAreaCode, &nAreaID, szTemp);
                    if(nRead >= 2)
                    {
                        szAreaCode[sizeof(szAreaCode) - 1] = '\0';
                        map_code_to_id[szAreaCode] = nAreaID;
                    }
                }
            }
            fclose(f);
        }

        if(map_code_to_id.size() == 0)
        {
            for(int i = 0; i < AREA_CODE_COUNT; i++)
            {
                int nRead = sscanf(g_szAreaCode[i], "%s%d%s", szAreaCode, &nAreaID, szTemp);
                if(nRead >= 2)
                {
                    szAreaCode[sizeof(szAreaCode) - 1] = '\0';
                    map_code_to_id[szAreaCode] = nAreaID;
                }
            }
        }
    }

    char szTemp[10];
    memcpy(szTemp, (char*)pParas[0], 4);
    szTemp[4] = '\0';
    int nAreaID = 0;
    if(map_code_to_id.find(szTemp) != map_code_to_id.end())
    {
        nAreaID = map_code_to_id[szTemp];
    }
    else
    {
        memcpy(szTemp, (char*)pParas[0], 3);
        szTemp[3] = '\0';
        if(map_code_to_id.find(szTemp) != map_code_to_id.end())
        {
            nAreaID = map_code_to_id[szTemp];
        }
    }
    mutex.release();

    //�Ѻ����ַ�����ǰuPreNum������ת��������
    stVar.yVariantType = FT_ULONG;
    stVar.u4Value = nAreaID;

    return 0;
}
/************ End of The GetAreaID Defining ******************/

/************ The PadAreacode Defining ***************************/
// �������������û�����ǰ��������ţ���������Ѿ����ڣ�����ӡ�
//            ���ԭ��:
//            1. ���ڻ����е����к��룬�����ַ����Ϊ���غ��룬�͸������б��غ��׼���
//               ���м������ţ������ַ����Ϊ����ֵ���Ͳ����κδ���
//            2. ���ڻ����еı��к��룬�����ַ����Ϊ���غ��룬�͸������б��غ��׼���
//               ���м������ţ������ַ����Ϊ����ֵ���Ͳ����κδ������⣬����13�ƶ�
//               ���룬��Ҫ������;           
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ4��
//           pSrcFields[0]/nSrcFieldLen[0] : �û�����/�û������ֶεĳ���(���л��߱���)
//           pSrcFields[1]/nSrcFieldLen[1] : ��������/�������Զ�Ӧ���ֶγ���
//           pSrcFields[2]/nSrcFieldLen[2] : �����û������Ӧ�ĺ��׼�/���׼��ֶγ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
// ����ֵ���ɹ�����0��ʧ�ܷ���-1
// 
// Added by ZhengYuqun , SWPD01198, ΪSoftX3000�����Ļ����еĺ�����������, 2003-08-27 
//�޸ļ�¼��
//2004-02-23 ֣��Ⱥ �޸ı�����֧��Ŀ���ֶ�Ϊ�ı�
/***************************************************************/
int CConvertFun::PadAreacode(void* pCurField, const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery)
{
#define ADDRESS_NATURE_SUBSCRIBER         0      //�û�����
#define ADDRESS_NATURE_RESERVED           1      //����
#define ADDRESS_NATURE_DOMESTIC           2      //���ں���
#define ADDRESS_NATURE_INTERNATIONAL      3      //���ʺ���

    static MAP<UINT4, STRING> mapDNSet2AreaCode;
	static BOOL bIsFirstRun = TRUE;

    char* pszCurField = (char *)pCurField;

    if (bIsFirstRun && mapDNSet2AreaCode.size() == 0)
    {
        //��ȡ��������׼������ŵĶ��ձ�
		FILE* f = fopen("./config/area/area.map", "rt");
        if(f != NULL)
        {
            char szLine[1024];
            while(!feof(f))
            {
                char szAreaCode[128];
                UINT4 nDNSet;
                char* p = fgets(szLine, sizeof(szLine), f);
                if(p != NULL)
                {
                    int nRead = sscanf(szLine, "%d%s", &nDNSet, szAreaCode);
                    if(nRead >= 2)
                    {
                        szAreaCode[sizeof(szAreaCode) - 1] = '\0';
                        mapDNSet2AreaCode[nDNSet] = szAreaCode;
                    }
                }
            }
            fclose(f);
        }

		bIsFirstRun = FALSE; //��ֹmapDNSet2AreaCodeΪ�յ�ʱ������ͼ���ļ�
    }

    //ȷ�����������ȷ
    if ((pCurField == NULL) || (nSrcFields != 3) || (pSrcFields == NULL))
    {
        return -1;
    }

    if ((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0)
     || (pSrcFields[1] == NULL) || (nSrcFieldLen[1] == 0)
     || (pSrcFields[2] == NULL) || (nSrcFieldLen[2] == 0))
    {
        return -1;
    }

    //�绰���������BCD������ı�
    if (pFieldInfo[0].nDataType != FT_BCD && pFieldInfo[0].nDataType != FT_STRING) 
    {
        return -1;
    }

    //��ú�������
    BYTE nAddrNature;
    if (nSrcFieldLen[1] == LEN_BYTE)
    {
        memcpy(&nAddrNature, pSrcFields[1], LEN_BYTE);
    }
    else
    {
        return -1;
    }

    //��ú��׼�
    UINT2 nDNSet = 0;
    if (nSrcFieldLen[2] == LEN_BYTE)
    {
        nDNSet = (UINT2)*(BYTE*)pSrcFields[2];
    }
    else if(nSrcFieldLen[2] == LEN_INT2) //���ݽ������׼�Ϊ2���ֽ�
    {
        memcpy((void*)&nDNSet, pSrcFields[2], LEN_INT2);
        //nDNSet  = (UINT2)ITOHS(nDNSet);
    }
    else
    {
        return -1;
    }

    char szSubscriber[128];
    BCDtoString((BYTE*)pSrcFields[0], szSubscriber, nSrcFieldLen[0]);
    char* pF = strchr(szSubscriber, 'F');
    if(pF != NULL)
    {
        *pF = '\0';
    }

    //���ں��롢��0����13��ͷ�ĺ��룬������
    if ((nAddrNature != ADDRESS_NATURE_SUBSCRIBER) 
     || (strncmp(szSubscriber, "13", 2) == 0)
     || (szSubscriber[0] == '0')) 
    {
        if (pFieldInfo[0].nDataType == FT_STRING)
        {
            if (pFieldInfo->nFieldLength < strlen(szSubscriber))
            {
                strncpy(pszCurField, szSubscriber, pFieldInfo->nFieldLength);                
                pszCurField[pFieldInfo->nFieldLength - 1] = '\0';
            }
            else
            {
                strcpy(pszCurField, szSubscriber);
            }           
 
            if (nIsQuery == 0)
            {
                return AddFillerSeparator(pszCurField, pFieldInfo);        
            }
        }
        else
        {
            memcpy(pCurField, pSrcFields[0], nSrcFieldLen[0]);            
        }

        return 0;
    }

    //�ȸ��ݺ��׼���������
	if (mapDNSet2AreaCode.find(nDNSet) == mapDNSet2AreaCode.end())
    {
        //����Ҳ�����Ӧ�ĺ��׼���ֱ�ӿ����绰����
        if (pFieldInfo[0].nDataType == FT_STRING)
        {
            if (pFieldInfo->nFieldLength < strlen(szSubscriber))
            {
                strncpy(pszCurField, szSubscriber, pFieldInfo->nFieldLength);
                pszCurField[pFieldInfo->nFieldLength - 1] = '\0';
            }
            else
            {
                strcpy(pszCurField, szSubscriber);
            }    
            
            if (nIsQuery == 0)
            {
                return AddFillerSeparator(pszCurField, pFieldInfo);        
            }
        }
        else
        {
            memcpy(pCurField, pSrcFields[0], nSrcFieldLen[0]);            
        }
    }
    else
    {
        int i = 0;
        char szAreaCode[128];
        szAreaCode[0] = '\0';

        //��MAP�л�ñ����׼�����Ӧ������
        strncpy(szAreaCode, mapDNSet2AreaCode[nDNSet].c_str(), sizeof(szAreaCode));
        szAreaCode[sizeof(szAreaCode) - 1] = '\0';


        //�жϺ����ǰ��λ�Ƿ��������
        for (i = 0; ((i < strlen(szAreaCode)) && (i < strlen(szSubscriber))); i++)
        {
            if (szAreaCode[i] != szSubscriber[i])
            {
                break;
            }
        }

        if (i == strlen(szAreaCode))
        {
            //����ǰ���Ѿ��������ţ�ֱ�ӿ����Ϳ�����
            if (pFieldInfo[0].nDataType == FT_STRING)
            {
                if (pFieldInfo->nFieldLength < strlen(szSubscriber))
                {
                    strncpy(pszCurField, szSubscriber, pFieldInfo->nFieldLength);
                    pszCurField[pFieldInfo->nFieldLength - 1] = '\0';
                }
                else
                {
                    strcpy(pszCurField, szSubscriber);
                }    

                if (nIsQuery == 0)
                {
                    AddFillerSeparator(pszCurField, pFieldInfo);        
                }
            }
            else
            {
                memcpy(pCurField, pSrcFields[0], nSrcFieldLen[0]);            
            }
        }
        else
        {
            //����û�а������ţ��Ƚ����ź͵绰��������ַ���ƴ��
            strncat(szAreaCode, szSubscriber, sizeof(szAreaCode) - strlen(szAreaCode));
            
            if (pFieldInfo[0].nDataType == FT_STRING)
            {
                if (pFieldInfo->nFieldLength < strlen(szAreaCode))
                {
                    strncpy(pszCurField, szAreaCode, pFieldInfo->nFieldLength);
                    pszCurField[pFieldInfo->nFieldLength - 1] = '\0';
                }
                else
                {
                    strcpy(pszCurField, szAreaCode);
                }    

                if (nIsQuery == 0)
                {
                    AddFillerSeparator(pszCurField, pFieldInfo);        
                }
            }
            else
            {
                //��ƴ�Ӻ�Ĵ����ŵĵ绰����ת��ΪBCD��ֱ��д����������Ȳ�����Դ�ֶεĳ���
                //StringtoBCD�������޸ģ����Խ�������������&ȥ�� ZhengYuqun 2003-11-21
                StringtoBCD(szAreaCode, (BYTE*)pCurField, nSrcFieldLen[0]);  
            }
        }
    }

    return 0;
}

/************ End of The PadAreacode Defining ******************/


/************ The ReplacePrefix Defining ***************************/
// �������������µ�ǰ׺�滻�绰�����оɵ�ǰ׺�����������þ����Ƿ����Ӻ��׼���
//           �滻ԭ��:
//           �绰����ǰ׺�����ƥ��ԭ����ǰ׺���е�ǰ׺����ƥ�䡣                
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1����2��
// 
//           pSrcFields[0]/nSrcFieldLen[0] : �û�����ǰһ����/�û�����ǰһ���ֵĳ���/��ѡ����
//           pSrcFields[1]/nSrcFieldLen[1] : ���׼��ֶ�/���׼��ֶεĳ���/��ѡ����
//           pSrcFields[2]/nSrcFieldLen[2] : ȱʡ�Ƿ���������/��ѡ����
//           pSrcFields[3]/nSrcFieldLen[3] : �û������һ����/�û������һ���ֵĳ���/��ѡ����
//           ��Ϊ�������������ģ�������ĸ�����ʱ���û������һ������pSrcFields[2]��
//           ��ȱʡ�Ƿ�����������pSrcFields[3]
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
// ����ֵ���ɹ�����0��ʧ�ܷ���-1
// 
// Added by ZhengYuqun , SWPD01198, ΪSoftX3000�����Ļ����еĺ�����������, 2003-08-27 
// Modify by chenliangwei, 2003-03-20
//�޸ļ�¼��
/***************************************************************/
typedef struct _SPrefixCvtInfo
{
    STRING  strNewPrefix;
    int     bPadAreaCode;    
} SPrefixCvtInfo;

typedef MAP<STRING, SPrefixCvtInfo> PREFIX_MAP;

int CConvertFun::ReplacePrefix(void* pCurField, const UINT4 nSrcFields,
                               const void* pSrcFields[],
                               const UINT4 nSrcFieldLen[],
                               const SFieldInfo* pFieldInfo,
                               const void* pDefault, const UINT4 nDefaultLen,
                               int nIsQuery)
{

    //ȷ�����������ȷ
    if(nSrcFields < 1 || nSrcFields > 4)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_INVALID_PARA_NUM,
              FUN_BCD_TO_STR, nSrcFields);
        return -1;
    }
    
    if((pCurField == NULL) || (pSrcFields == NULL))
    {
        return -1;
    }

    if ((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //�绰���������BCD������ı�
    if (pFieldInfo[0].nDataType != FT_BCD && pFieldInfo[0].nDataType != FT_STRING) 
    {
        return -1;
    }

    //��ú��׼�
    UINT2 nDNSet = 0;
    if(nSrcFields > 1)
    {
        if (nSrcFieldLen[1] == LEN_BYTE)
        {
            nDNSet = (UINT2)*(BYTE*)pSrcFields[1];
        }
        else if(nSrcFieldLen[1] == LEN_INT2) //���ݽ������׼�Ϊ2���ֽ�
        {
            memcpy((void*)&nDNSet, pSrcFields[1], LEN_INT2);
            //nDNSet  = (UINT2)ITOHS(nDNSet);
        }
        else
        {
            return -1;
        }
    }

    //ȱʡ�Ƿ���������
    BOOL bDefPadAreaCode = 0; 
    if((nSrcFields == 3) || (nSrcFields == 4))
    {
        bDefPadAreaCode = atoi((char*)pSrcFields[nSrcFields - 1]);
    }

    //��װ�绰����
    BYTE pNumber[40];
    int  nSrcNumLen = 0;
    if(nSrcFields != 4)
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];

    }
    else
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];

        memcpy(pNumber + nSrcNumLen, pSrcFields[2], nSrcFieldLen[2]);
        nSrcNumLen += nSrcFieldLen[1];
        
    }




    static PREFIX_MAP mapPrefix;
    static MAP<UINT4, STRING> mapDNSet2AreaCode;
	static BOOL bIsFirstRun = TRUE;
    static ACE_Mutex mutex;    

    /*
     * ����ǰ׺��������ļ���
     */
    mutex.acquire();
    if(bIsFirstRun)
    {
        //��ȡ��������׼������ŵĶ��ձ�
		FILE* f = fopen("./config/area/prefix.map", "rt");
        if(f != NULL)
        {
            char szLine[1024];
            while(!feof(f))
            {
                char szOldPrefix[128] = "";
                char szNewPrefix[128] = "";
                char szbAddAreaCode[3] = "";

                char* p = fgets(szLine, sizeof(szLine), f);
                if((p != NULL) && (szLine[0] != '\0'))
                {
                    //��"��ǰ׺,��ǰ׺,�Ƿ������"���н���
                    char* pDiv = strchr(szLine, ',');
                    if(pDiv == NULL)
                    {
                        //û�ж��Ž��зָ���˵��ֻ��һ����ǰ׺���������
                        strncpy(szOldPrefix, szLine, sizeof(szOldPrefix));
                        szOldPrefix[sizeof(szOldPrefix) - 1] = '\0';
                    }
                    else
                    {
                        //��þ�ǰ׺
                        *pDiv = '\0';
                        strncpy(szOldPrefix, szLine, sizeof(szOldPrefix));
                        szOldPrefix[sizeof(szOldPrefix) - 1] = '\0';

                        char* pNewPrefix = pDiv + 1;
                        pDiv = strchr(pNewPrefix, ',');
                        if(pDiv == NULL)
                        {
                            //ֻ��һ�����ŷָ���˵��ֻ�о�ǰ׺����ǰ׺�������ǰ׺
                            strncpy(szNewPrefix, pNewPrefix, sizeof(szNewPrefix));
                            szNewPrefix[sizeof(szNewPrefix) - 1] = '\0';
                        }
                        else
                        {
                            //�������ŷָ���˵����ǰ׺\��ǰ׺\�Ƿ�����Ŷ��У�
                            //����Ƿ�����ű�ʾ
                            *pDiv = '\0';
                            strncpy(szNewPrefix, pNewPrefix, sizeof(szNewPrefix));
                            szNewPrefix[sizeof(szNewPrefix) - 1] = '\0';

                            char* pbAddAreaCode = pDiv + 1;
                            strncpy(szbAddAreaCode, pbAddAreaCode, sizeof(szbAddAreaCode));
                            szbAddAreaCode[sizeof(szbAddAreaCode) - 1] = '\0';
                        }
                    }

                    //�Ѿ�ǰ׺��Ϊ�ؼ��ּ���Map
                    StringLRTrim(szOldPrefix);
                    StringLRTrim(szNewPrefix);
                    StringLRTrim(szbAddAreaCode);

                    SPrefixCvtInfo stInfo;
                    stInfo.strNewPrefix = szNewPrefix;
                    if(szbAddAreaCode[0] == '\0')
                    {
                        stInfo.bPadAreaCode = bDefPadAreaCode;
                    }
                    else
                    {
                        stInfo.bPadAreaCode = atoi(szbAddAreaCode);
                    }

                    mapPrefix[szOldPrefix] = stInfo;
                }
            }
            fclose(f);
        }

        //��ȡ��������׼������ŵĶ��ձ�
        if(nSrcFields > 1)
        {
		    f = fopen("./config/area/area.map", "rt");
            if(f != NULL)
            {
                char szLine[1024];
                while(!feof(f))
                {
                    char szAreaCode[128];
                    UINT4 nDNSet;
                    char* p = fgets(szLine, sizeof(szLine), f);
                    if(p != NULL)
                    {
                        int nRead = sscanf(szLine, "%d%s", &nDNSet, szAreaCode);
                        if(nRead >= 2)
                        {
                            szAreaCode[sizeof(szAreaCode) - 1] = '\0';
                            mapDNSet2AreaCode[nDNSet] = szAreaCode;
                        }
                    }
                }
                fclose(f);
            }
        }
        
        //��ֹmapPrefixΪ�յ�ʱ������ͼ���ļ�
		bIsFirstRun = FALSE; 
    }
    mutex.release();


    //���绰�����BCD��ת��Ϊ�ı�
    char szNumber[128] = "";
    BCDtoString(pNumber, szNumber, nSrcNumLen);
    char* pF = strchr((char*)szNumber, 'F');
    if(pF != NULL)
    {
        *pF = '\0';
    }


    /*
     * ����ǰ׺���еľ�ǰ׺�Ե绰�������ƥ�䣬�����ƥ��ԭ��
     * ��ΪMap�е����ݶ�����ֵ������������˴Ӻ���ǰ�飬�ͷ������ԭ��
     */

    mutex.acquire();
    PREFIX_MAP::reverse_iterator rit= mapPrefix.rbegin();
    while(rit != mapPrefix.rend())
    {
        STRING sOldPrefix = rit->first;
        if(strncmp(sOldPrefix.c_str(), szNumber, sOldPrefix.length()) == 0)
        {
            break;
        }            
        rit++;
    }


    char szNewNumber[256] = "";

    //����Ҳ�����Ӧ��ǰ׺
    if(rit == mapPrefix.rend())
    {
        if((nSrcFields > 1) && bDefPadAreaCode)
        {
            //���ݺ��׼���������
            MAP<UINT4, STRING>::iterator itDNSet = 
                                mapDNSet2AreaCode.find(nDNSet);
	        if ( itDNSet != mapDNSet2AreaCode.end())
            {
                strncpy(szNewNumber, itDNSet->second.c_str(), sizeof(szNewNumber));
                szNewNumber[sizeof(szNewNumber) - 1] = '\0';
            }
        }
        strcat(szNewNumber, szNumber);
    }
    else
    {
        //�ȼ�����
        if((nSrcFields > 1) && rit->second.bPadAreaCode)
        {
            //���ݺ��׼���������
            MAP<UINT4, STRING>::iterator itDNSet = 
                                mapDNSet2AreaCode.find(nDNSet);
	        if ( itDNSet != mapDNSet2AreaCode.end())
            {
                strncpy(szNewNumber, itDNSet->second.c_str(), sizeof(szNewNumber));
                szNewNumber[sizeof(szNewNumber) - 1] = '\0';
            }
        }

        //���滻ǰ׺
        strcat(szNewNumber, rit->second.strNewPrefix.c_str());
        strcat(szNewNumber, &szNumber[rit->first.length()]);
    }

    if(pFieldInfo[0].nDataType == FT_STRING)
    {
        strcpy((char*)pCurField, szNewNumber);
        if (nIsQuery == 0)
        {
            AddFillerSeparator((char*)pCurField, pFieldInfo);        
        }
    }
    else
    {
        //���滻ǰ׺��ĵ绰����ת��ΪBCD��ֱ��д�����
        StringtoBCD(szNewNumber, (BYTE*)pCurField, pFieldInfo->nFieldLength);
    }

    mutex.release();

    return 0;
}

/************ End of The ReplacePrefix Defining ******************/

/************ The ReplaceNumByStr Defining ***************************/
// ������������ĳ���ֶδ������滻���ַ���,���ֺ��ַ�����Ӧ��ϵ���ļ�������
//           ԭʼ�ֶε�ֵ		�滻���ֵ
//           1        			abc
//           2      			def
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1����2��
// 
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
// ����ֵ���ɹ�����0��ʧ�ܷ���-1
// 
// Added By Lucy 2004-06-25
/***************************************************************/
typedef MAP<int, STRING>         NUMSTRMAP;
typedef MAP<STRING, NUMSTRMAP>   NUMSTRFILE;

//���������ļ�
BOOL LoadNumStr(NUMSTRMAP& NumStrMap, const char* szFileName)
{
    STRING sFileName = "./config/area/" + STRING(szFileName);
    //�ļ��Ƿ���ڣ��������򴴽�
    FILE* pFile = fopen(sFileName.c_str(), "r");
    if( pFile == NULL)
    {
        CreateDeepDir("./config/area");
        pFile = fopen(sFileName.c_str(), "w");
        if(pFile == NULL)
        {
            return FALSE;
        }
    }
    else
    //��ȡ������Ϣ
    {
        char szLine[1024];
        int  nNum;        //����
        char szStr[1024]; //�ַ���
        while(!feof(pFile))
        {           
            char* p = fgets(szLine, sizeof(szLine), pFile);
            if((p != NULL) && (szLine[0] != '\0'))
            {
                sscanf(szLine,"%d %s", &nNum, szStr);
                //����MAP��
                NumStrMap[nNum] = STRING(szStr);
            }
        }
    }
    fclose(pFile);
    
    return TRUE;
}

int CConvertFun::ReplaceNumByStr(void* pCurField, const UINT4 nSrcFields,
                                 const void* pSrcFields[],
                                 const UINT4 nSrcFieldLen[],
                                 const SFieldInfo* pFieldInfo,
                                 const void* pDefault, const UINT4 nDefaultLen,
                                 int nIsQuery)
{
    INT4 uSrcNum;

    //ȷ�����������ȷ
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_INVALID_PARA_NUM,
            FUN_REPLACE_NUM_BY_STR, nSrcFields);
        return -1;
    }    
    if((pCurField == NULL) || (pSrcFields == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0)||(pSrcFields[1] == NULL) || (nSrcFieldLen[1] == 0))
    {
        return -1;
    }
    
    //��ȡԭ��ʽ�������ֵ
    if(nSrcFieldLen[0] == LEN_BYTE)
    {
        BYTE uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_BYTE);
        uSrcNum = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT2)
    {
        UINT2 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT2);
        uSrcNum = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT4)
    {
        UINT4 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT4);
        uSrcNum = uTmp;
    }    
    else if(nSrcFieldLen[0] == LEN_INT8)
    {
        TRACE(MTS_FORMAT, S_CFG_SET_ITEMVALUE_TOOLONG,
            FUN_REPLACE_NUM_BY_STR, nSrcFieldLen[0]);
        return -1;
    }    
    else
    {
        return -1;
    }

    //��ȡ�����еĶ�����SegName
    const char* szFileName = (const char*)pSrcFields[1];

    static ACE_Mutex mutex;
    mutex.acquire();

    static NUMSTRFILE  SegFile;
    NUMSTRFILE::iterator iterFile = SegFile.find(szFileName);

    //û�м��ع��������ݣ����ļ��м���
    if(iterFile == SegFile.end())
    {
        NUMSTRMAP NumStrMap;
        if(!LoadNumStr(NumStrMap, szFileName))
        {
            mutex.release();
            return -1;
        }

        SegFile[szFileName] = NumStrMap;        
    }

    //ȡ�ö�Ӧ�ε��б�
    NUMSTRMAP& NumStrMap = SegFile[szFileName];

    //�ҵ���Ӧ������ �� ����ַ���
    NUMSTRMAP::iterator iterMap = NumStrMap.find(uSrcNum);
    if(iterMap != NumStrMap.end())
    {
        strncpy((char*)pCurField, (NumStrMap[uSrcNum]).c_str(), (NumStrMap[uSrcNum]).length() + 1); 
    }
    else
    {
        sprintf((char*)pCurField, "%u", uSrcNum);
    }
    
    mutex.release();

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFieldInfo);
    }
    
    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;    
}
//end


// ������������ԭʼ�����еġ�ͨ��ʱ�����ֶ�ת����HHmmSS��ʽ�����磬ͨ��ʱ��Ϊ128�룬��ת����ĸ�ʽ
//           Ϊ000208��
// ���������
//           pCurField      : ת����Ĵ��HHmmSS��ʽ���ַ���ָ��
//           nSrcFields     : �ڱ�������Ӧ�õ���1
//           pSrcFields[0]  : ͨ��ʱ��(UINT4)
//           nSrcFieldLen[0]: ͨ��ʱ���ֶεĳ���
int CConvertFun::Duration2HHmmSS(void* pCurField,
                                 const UINT4 nSrcFields,
                                 const void* pSrcFields[],
                                 const UINT4 nSrcFieldLen[],
                                 const SFieldInfo* pFI,
                                 const void* pDefault,
                                 const UINT4 nDefaultLen,
                                 int nIsQuery)
{
    if ((pCurField == NULL) || (nSrcFieldLen[0] <= 0) || (pSrcFields == NULL))
    {
        return -1;
    }

    //��ȡ��������е�ͨ��ʱ��
    UINT4 nDuration;
    memcpy(&nDuration, pSrcFields[0], LEN_UINT4);
    //nDuration = ITOHL(nDuration);

    UINT4 nHours    = 0;
    UINT4 nMinutes  = 0;
    UINT4 nSeconds  = 0;

    nHours    = nDuration/3600;
    nMinutes  = (nDuration - (nHours * 3600))/60;
    nSeconds  = (nDuration % 60);

    sprintf((char*)pCurField, "%02d%02d%02d", nHours, nMinutes, nSeconds);
    if (nIsQuery == 0)
    {
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    return 0;
}

/***************  The Define of  Convert10ms   ************************/
// ���ܣ���10msΪ��λ��ʱ��ת����S, ��HHMMSS��ת����HHMMSSʱ��Ҫ���ϸ�ʽ
//
// ���磺12800, "HH:MM:SS" -> 02:08:00
//
// ���������
//           pCurField      : ת����Ĵ��HHmmSS��ʽ���ַ���ָ��
//           nSrcFields     : �ڱ�������Ӧ�õ���1
//           pSrcFields[0]  : ͨ��ʱ��(UINT4)
//           nSrcFieldLen[0]: ͨ��ʱ���ֶεĳ���
// ����� -1��ʧ��, 0: �ɹ�
//
// Added By Lucy 2004-07-16 
/**********************************************************************/
int CConvertFun::Convert10ms(void* pCurField, 
                       const UINT4 nSrcFields,
                       const void* pSrcFields[],
                       const UINT4 nSrcFieldLen[],
                       const SFieldInfo* pFI,
                       const void* pDefault, 
                       const UINT4 nDefaultLen,
                       int nIsQuery)
{
    //��֤���������ȷ
    if((pCurField == NULL) || ((nSrcFields != 1) && (nSrcFields != 2))|| 
       (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_CONVERT10MS, nSrcFields);
        
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }
    //end of ��֤
    
    //��ȡ�������: ʱ��
    UINT4 nDuration = 0;
    
    //��ȡԭ��ʽ�������ֵ
    if(nSrcFieldLen[0] == LEN_BYTE)
    {
        BYTE uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_BYTE);
        nDuration = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT2)
    {
        UINT2 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT2);
        nDuration = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT4)
    {
        UINT4 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT4);
        nDuration = uTmp;
    }    
    else 
    {
        return -1;
    }
    
    //��10ms��λת����sΪ��λ,�������1
    nDuration = (nDuration + 99) / 100;
    
    //��Ŀ������(������/�ı�)ת������
    if(1 == nSrcFields)
    {
        if((FT_CHAR == pFI->nDataType) || (FT_STRING == pFI->nDataType))
        {
            sprintf((char*)pCurField, "%u", nDuration);
        }
        else
        {
            switch(pFI->nDataType) 
            {
            case FT_BYTE:
                {                
                    BYTE uTmp = nDuration;
                    memcpy(pCurField, &uTmp, LEN_BYTE);
            	    break;
                }
            case FT_SHORT:
            case FT_USHORT:
                {
                    UINT2 uTmp = nDuration;
                    memcpy(pCurField, &uTmp, LEN_INT2);
            	    break;
                }
            case FT_LONG:
            case FT_ULONG:   
                {
                    UINT4 uTmp = nDuration;
                    memcpy(pCurField, &uTmp, LEN_INT4);
                    break;
                }
            case FT_INT8:
                {
                    INT8 uTmp = nDuration;
                    memcpy(pCurField, &uTmp, LEN_INT8);
                    break;
                }
            default:
                return -1;
            }
        }
    }
    //����ʽת����HHMMSS
    else
    {               
        UINT4 nHours    = nDuration/3600;
        UINT4 nMinutes  = (nDuration - (nHours * 3600))/60;
        UINT4 nSeconds  = (nDuration % 60);

        char szTmp[32] = "";
        char szSep1[8] = "";
        char szSep2[8] = "";
        
        memcpy(szTmp, pSrcFields[1], nSrcFieldLen[1]);
        //���ڷָ���HH:MM:SS
        if(strlen(szTmp) > 6)
        {
            sscanf(szTmp, "HH%cMM%cSS", szSep1, szSep2);
        }

        STRING sFormat = "%02d" + STRING(szSep1) + "%02d" + STRING(szSep2) + "%02d";

        sprintf((char*)pCurField, sFormat.c_str(), nHours, nMinutes, nSeconds);
    }

    if (nIsQuery == 0)
    {
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;    
}


//��������Int2Text(...)��������ǿ�棬������������ȫΪ0xFF����ת���ɿ��ַ���
int CConvertFun::Int2TextEx(void* pCurField,
                            const UINT4 nSrcFields,
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault,
                            const UINT4 nDefaultLen,
                            int nIsQuery)
{
    INT4  nTmp = 0;    //���BYTE��SHORT��LONG����ʱ����
    BOOL bIsAllF = TRUE;  //�Ƿ�ȫΪ0xF

    //ȷ�����������ȷ
    if((pCurField == NULL) || !(nSrcFields == 1 || nSrcFields == 2)  || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }

    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    for(int i = 0; i < nSrcFieldLen[0]; i++)
    {
        if(*((BYTE*)(pSrcFields[0]) + i) != 0xFF)
        {
            bIsAllF = FALSE;
            break;
        }
    }

    if(bIsAllF)
    {
        *(char*)pCurField = '\0';
    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    }
    else
    {
        return Int2Text(pCurField, nSrcFields, pSrcFields, nSrcFieldLen,
                        pFI, pDefault, nDefaultLen, nIsQuery);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}

int CConvertFun::TrkGrp2CallNo(void* pCurField,
                               const UINT4 nSrcFields, 
                               const void* pSrcFields[],
                               const UINT4 nSrcFieldLen[],
                               const SFieldInfo* pFI,
                               const void* pDefault,
                               const UINT4 nDefaultLen,
                               int nIsQuery)
{
    static MAP<UINT2, STRING> mapTrunkGroup2CallNo;
    static BOOL bIsFirstRun = TRUE;
    
    //ȷ�����������ȷ
    if((pCurField == NULL) || (nSrcFields != 1) || (pSrcFields == NULL))
    {
        return -1;
    }
    
    if ((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }    
    
    static ACE_Mutex mutex;
    
    mutex.acquire();
    if(bIsFirstRun)
    {
        //��ȡ��������׼������ŵĶ��ձ�
        FILE* f = fopen("./config/distinction/trk2callno.map", "rt");
        if(f != NULL)
        {
            char szLine[1024];
            char szCallNo[40];
            UINT2 uTrunkGroup = 0;
            while(!feof(f))
            {
                if( fgets(szLine, sizeof(szLine), f) != NULL)
                {
                    StringLRTrim(szLine);
                    int nRead = sscanf(szLine, "%d%s", &uTrunkGroup, szCallNo);
                    if(nRead == 2)
                    {
                        mapTrunkGroup2CallNo[uTrunkGroup] = szCallNo;
                    }
                }
            }
            fclose(f);
        }
        
        bIsFirstRun = FALSE; //��ֹ�����ͼ���ļ�
    }  

    if(nSrcFieldLen[0] == LEN_UINT2)
    {
        UINT2 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT2);
        MAP<UINT2, STRING>::iterator pMap = mapTrunkGroup2CallNo.find(uTmp);
        if (pMap != mapTrunkGroup2CallNo.end())
        {
            strncpy((char*)pCurField,
                     mapTrunkGroup2CallNo[uTmp].c_str(),
                     pFI->nFieldLength - 1);
            memset((char*)pCurField + pFI->nFieldLength - 1, '\0', 1);
        }
        else
        {
            sprintf((char*)pCurField,"%d\0",uTmp);
        }
    }
    else
    {
        memset(pCurField, 0, 1);
    }
    mutex.release();

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    return 0;
}

//Added by ZhengYuqun SWPD03631 2004-02-18
//MSoftX3000�Ļ�������ת��Ϊ2G�Ļ�������ʱ��Ҫ���ݻ������ͺ��м仰��ԭ��ֵ�����Ժ���������
int CConvertFun::CDRType2G(void* pCurField,
                            const UINT4 nSrcFields,
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault,
                            const UINT4 nDefaultLen,
                            int nIsQuery)
{
    INT4  nTmp = 0;    //���BYTE��SHORT��LONG����ʱ����
    BOOL bIsAllF = TRUE;  //�Ƿ�ȫΪ0xF
    int i;

    //ȷ�����������ȷ
    if((pCurField == NULL) || (nSrcFields != 2) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }

    for(i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    if(nSrcFieldLen[0] != LEN_BYTE || nSrcFieldLen[1] != LEN_BYTE)
    {
        return -1;
    }

    BYTE byCDRType = *(BYTE*)pSrcFields[0];
    BYTE byCause = *(BYTE*)pSrcFields[1];

    //�м仰��ԭ��ֵΪ3(ʧ��)���һ�������Ϊ2(��ؿھ�/����)��4(���)��10(���ؿھ�)ʱ�����������޸�Ϊ9(�Ժ�)
    if(byCause == 3 && (byCDRType == 2 || byCDRType == 4 || byCDRType == 10))
    {
        byCDRType = 9;
    }

    memcpy(pCurField, &byCDRType, LEN_BYTE);

    return 0;
}


/****************** The IsInNumSeg Defining *************************/
// �����������ж�һ�������Ƿ���ָ���ĺŶ���
//
// ���������
//  nParasNum�������������˺����Ĳ�������Ӧ��Ϊ2
//     pParas���������飬�˺�����������������
//             pParas[0]��ʾTBCD����ĵ绰����
//             pParas[1]��ʾ�Ŷ��������
//  nParasLen����������������
//   pDefault������ȱʡֵ
//
// �����������
//
//   ����ֵ������ɹ����򷵻�SVariant*���͵Ľ�������������NULL
//
/****************************************************************/
typedef struct _SNUMSEG
{
    UINT2  nNumberCount;        //pNumSeg�ж��ٸ���������Ч��
    BYTE   pNumSeg[20];         //�ŶεĴ������
} SNUMSEG;

#define   NUM_SEG_SINGLE    0
#define   NUM_SEG_SCOPE     1
typedef struct _SNUMSEGSCOPE
{
    BYTE      byScopeTag;          //��ʾ�ǵ����Ŷλ���һ���Ŷη�Χ
    SNUMSEG   stMinSeg;            //�Ŷη�Χ����С�ĶΣ������Ŷ�ʱ���Ǵ˺Ŷ�
    SNUMSEG   stMaxSeg;            //�Ŷη�Χ�����Ķ�
} SNUMSEGSCOPE;


typedef LIST<SNUMSEGSCOPE>          NUMSEGTABLE;
typedef MAP<STRING, NUMSEGTABLE>    NUMSEGFILE;

#define NUM_SEG_COUNT               "NumSegCount"
#define NUM_SEG_COUNT_DEFAULT       0

#define NUM_SEG_KEY                 "NumSeg"
#define NUM_SEG_VALUE_DEFAULT       ""
//���غŶα�
BOOL LoadNumSegTabel(NUMSEGTABLE& SegTable, const char* szNumSegName)
{
    //�����ļ��Ƿ���ڣ��������򴴽�
    FILE* pFile = fopen("./config/distination/number_seg.ini", "r");
    if( pFile == NULL)
    {
        CreateDeepDir("./config/distination");
        pFile = fopen("./config/distination/number_seg.ini", "w");
        if(pFile == NULL)
        {
            return FALSE;
        }
    }
    fclose(pFile);
    pFile = NULL;

    char szNumSegKey[15] = "";
    char szNumSeg[40]    = "";
    BYTE pBCD[64]        = "";
    BYTE pRBCD[64]       = "";

    //�������õĺŶ�
    CINIFile iniFile("./config/distination/number_seg.ini");
    iniFile.Open();
    UINT4 nNumSegCnt = iniFile.GetInt(szNumSegName,
                                      NUM_SEG_COUNT,
                                      NUM_SEG_COUNT_DEFAULT);
    for(int i = 1; i <= nNumSegCnt; i++)
    {
        //����һ���Ŷ�
        SNPRINTF(szNumSegKey,
                 sizeof(szNumSegKey),
                 "%s%d",
                 NUM_SEG_KEY,
                 i);
        szNumSegKey[sizeof(szNumSegKey) - 1] = '\0';

        iniFile.GetString(szNumSegName,
                          szNumSegKey,
                          NUM_SEG_VALUE_DEFAULT,
                          szNumSeg,
                          sizeof(szNumSeg));
        szNumSeg[sizeof(szNumSeg) - 1] = '\0';

        //û�����õĻ�����������һ��
        StringLRTrim(szNumSeg);
        if(szNumSeg[0] == '\0')
        {
            continue;
        }

        SNUMSEGSCOPE stNumSegScope;
        char* pDiv = strchr(szNumSeg, '-');

        if (pDiv == NULL)
        {
            //�����Ŷε����
            stNumSegScope.byScopeTag = NUM_SEG_SINGLE;
            int nByteLen = StringtoBCD(szNumSeg, pBCD);
            int nRBCDLen = BCDtoRBCD(stNumSegScope.stMinSeg.pNumSeg, pBCD, nByteLen);
            if(nRBCDLen == 0)
            {
                continue;
            }
            if(pRBCD[nRBCDLen - 1] & 0x0F == 0x0F)
            {
                stNumSegScope.stMinSeg.nNumberCount = 2 * nRBCDLen - 1;
            }
            else
            {
                stNumSegScope.stMinSeg.nNumberCount = 2 * nRBCDLen;
            }
        }
        else
        {
            //�Ŷη�Χ�����
            stNumSegScope.byScopeTag = NUM_SEG_SCOPE;

            pDiv[0] = '\0';
            char* szMinSeg = szNumSeg;
            char* szMaxSeg = pDiv + 1;
            StringLRTrim(szMinSeg);
            StringLRTrim(szMaxSeg);

            //ת����С�Ŷ�Ϊ����BCD��
            int nByteLen = StringtoBCD(szMinSeg, pBCD);
            int nRBCDLen = BCDtoRBCD(stNumSegScope.stMinSeg.pNumSeg, pBCD, nByteLen);
            if(nRBCDLen == 0)
            {
                continue;
            }
            if(pRBCD[nRBCDLen - 1] & 0x0F == 0x0F)
            {
                stNumSegScope.stMinSeg.nNumberCount = 2 * nRBCDLen - 1;
            }
            else
            {
                stNumSegScope.stMinSeg.nNumberCount = 2 * nRBCDLen;
            }

            //ת�����Ŷ�Ϊ����BCD��
            nByteLen = StringtoBCD(szMaxSeg, pBCD);
            nRBCDLen = BCDtoRBCD(stNumSegScope.stMaxSeg.pNumSeg, pBCD, nByteLen);
            if(nRBCDLen == 0)
            {
                continue;
            }
            if(pRBCD[nRBCDLen - 1] & 0x0F == 0x0F)
            {
                stNumSegScope.stMaxSeg.nNumberCount = 2 * nRBCDLen - 1;
            }
            else
            {
                stNumSegScope.stMaxSeg.nNumberCount = 2 * nRBCDLen;
            }
        }

        //����һ�ֲ�Ʒ�ĺŶ��б�
        SegTable.push_back(stNumSegScope);
    }

    return TRUE;
}

//�Ƚ�ָ��λ���ķ���BCD��
int RBCDCmp(BYTE* pRBCD1, BYTE* pRBCD2, int nCmpCodeLen)
{
    BOOL bIsOdd = (nCmpCodeLen % 2 != 0);
    int  nByteLen = nCmpCodeLen / 2;
    int  i;

    for(i = 0; i < nByteLen; i++)
    {
        BYTE byBCD1 = pRBCD1[i] << 4 || pRBCD1[i] >> 4;
        BYTE byBCD2 = pRBCD2[i] << 4 || pRBCD2[i] >> 4;

        if(byBCD1 > byBCD2)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    if(bIsOdd)
    {
        BYTE byBCD1 = pRBCD1[i] & 0xF;
        BYTE byBCD2 = pRBCD2[i] & 0xF;
        if(byBCD1 > byBCD2)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    return 0;
}

//�ж�һ�������Ƿ���ָ���ĺŶ���
int CConvertFun::IsInNumSeg(const UINT4 nParasNum,
                            const void* pParas[],
                            const UINT4 nParasLen[],
                            const STRING& pDefault,
                            SVariant& stVar)
{
    if((nParasNum != 2) || (pParas == NULL))
    {
        return -1;
    }

    //ȷ��������ȷ
    for(int i = 0; i < nParasNum; i++)
    {
        if((pParas[i] == NULL) || nParasLen[i] == 0)
        {
            return -1;
        }
    }

    const char* szNumSegName = (const char*)pParas[1];
    static ACE_Mutex   mutex;
    mutex.acquire();

    static NUMSEGFILE  SegFile;
    NUMSEGFILE::iterator iterFile = SegFile.find(szNumSegName);

    //û�м��ع��������ݣ����ļ��м���
    if(iterFile == SegFile.end())
    {
        NUMSEGTABLE SegTable;
        if(!LoadNumSegTabel(SegTable, szNumSegName))
        {
            mutex.release();
            return -1;
        }

        SegFile[szNumSegName] = SegTable;
    }

    /*
     *  ����Ϊ�жϺ����Ƿ��ںŶα��ڡ�
     */
    int nRet = 0;
    NUMSEGTABLE& SegTable = SegFile[szNumSegName];
    BYTE* pByNumber = (BYTE*)pParas[0];

    NUMSEGTABLE::iterator iterTab = SegTable.begin();
    while(iterTab != SegTable.end())
    {
        SNUMSEGSCOPE& NumScope = (*iterTab);

        //�ж��Ƿ���ϵ����Ŷ�
        if(NumScope.byScopeTag == NUM_SEG_SINGLE)
        {
            if(RBCDCmp(pByNumber, NumScope.stMinSeg.pNumSeg,
                       NumScope.stMinSeg.nNumberCount) == 0)
            {
                nRet = 1;
                break;
            }
        }
        else
        {
            if(( RBCDCmp(pByNumber, NumScope.stMinSeg.pNumSeg, NumScope.stMinSeg.nNumberCount) >= 0)
              &&(RBCDCmp(pByNumber, NumScope.stMaxSeg.pNumSeg, NumScope.stMaxSeg.nNumberCount) <= 0))
            {
                nRet = 1;
                break;
            }
        }
    }
    mutex.release();

    stVar.yVariantType = FT_ULONG;
    stVar.u4Value = nRet;

    return 0;

}

//�ж�һ�������Ƿ���ָ���ĺŶ���
int CConvertFun::IsInEnum(const UINT4 nParasNum,
                          const void* pParas[],
                          const UINT4 nParasLen[],
                          const STRING& pDefault,
                          SVariant& stVar)
{
    if((nParasNum != 2) || (pParas == NULL))
    {
        return -1;
    }

    //ȷ��������ȷ
    for(int i = 0; i < nParasNum; i++)
    {
        if((pParas[i] == NULL) || nParasLen[i] == 0)
        {
            return -1;
        }
    }

    //��ȡԭ��ʽ�������ֵ
    UINT4 uSrcNum = 0;
    if(nParasLen[0] == LEN_BYTE)
    {
        BYTE uTmp = 0;
        memcpy(&uTmp, pParas[0], LEN_BYTE);
        uSrcNum = uTmp;
    }
    else if(nParasLen[0] == LEN_UINT2)
    {
        UINT2 uTmp = 0;
        memcpy(&uTmp, pParas[0], LEN_UINT2);
        uSrcNum = uTmp;
    }
    else if(nParasLen[0] == LEN_UINT4)
    {
        UINT4 uTmp = 0;
        memcpy(&uTmp, pParas[0], LEN_UINT4);
        uSrcNum = uTmp;
    }    
    else
    {
        return -1;
    }
    
    const char* szFileName = (const char*)pParas[1];
    static ACE_Mutex   mutex;
    mutex.acquire();

    static MAP<STRING, MAP<UINT4, UINT4> >  EnumFile;
    MAP<STRING, MAP<UINT4, UINT4> >::iterator iterFile = EnumFile.find(szFileName);

    //û�м��ع��������ݣ����ļ��м���
    if(iterFile == EnumFile.end())
    {
        MAP<UINT4, UINT4> EnumTable;
        char  szTemp[100];
        UINT4 uOrgNum;
        UINT4 uDstNum;

        STRING strFileName = "./config/distinction/";
        strFileName += szFileName;
        FILE* f = fopen(strFileName.c_str(), "rt");
        if(f != NULL)
        {
            char szLine[1024];
            while(!feof(f))
            {
                char* p = fgets(szLine, sizeof(szLine), f);
                if(p != NULL)
                {
                    int nRead = sscanf(szLine, "%d%d%s", &uOrgNum, &uDstNum, szTemp);
                    if(nRead >= 2)
                    {
                        EnumTable[uOrgNum] = uDstNum;
                    }
                }
            }
            fclose(f);
        }

        if(EnumTable.size() > 0)
        {
            EnumFile[szFileName] = EnumTable;
        }
    }

    /*
     *  ����Ϊ�жϺ����Ƿ��ںŶα��ڡ�
     */
    UINT4 uRet = 0;
    MAP<UINT4, UINT4>& EnumTable = EnumFile[szFileName];
    if (EnumTable.find(uSrcNum) != EnumTable.end())
    {
        uRet = EnumTable[uSrcNum];
    }

    mutex.release();

    stVar.yVariantType = FT_ULONG;
    stVar.u4Value = uRet;

    return 0;

}

/*
  �������ܣ������Ȳ���ĺ����ָ�����ֹڡ�
  ���������
          pSrcFields[0]/nSrcFieldLen[0] : ������ĺ��루BCD�룩
          pSrcFields[1]/nSrcFieldLen[1] : ���볤��Ҫ��
          pSrcFields[2]/nSrcFieldLen[2] : ��Ҫ��ӵ��ֹ�
      
*/
int CConvertFun::PadNumber(void* pCurField, 
                           const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, 
                           const UINT4 nDefaultLen,
                           int nIsQuery)
{
    //ȷ�����������ȷ
    if ((pCurField == NULL) || (nSrcFields != 3) || (pSrcFields == NULL))
    {
        return -1;
    }

    if ((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0)
     || (pSrcFields[1] == NULL) || (nSrcFieldLen[1] == 0)
     || (pSrcFields[2] == NULL) || (nSrcFieldLen[2] == 0))
    {
        return -1;
    }

    if ((pFI->nDataType != FT_BCD) && (pFI->nDataType != FT_STRING))
    {
        return -1;
    }
	
	char szNumber[32];
	char szLen[16];
	
	strncpy(szLen, (char *)pSrcFields[1], 16);
	szLen[nSrcFieldLen[1]] = '\0';
	
	int  nLen = atoi(szLen);
	
	strncpy(szNumber, (char *)pSrcFields[2], nSrcFieldLen[2]);
	szNumber[nSrcFieldLen[2]] = '\0';
	
	char szTemp[32];
    BCDtoString((BYTE*)pSrcFields[0], szTemp, nSrcFieldLen[0]);
    char* pF = strchr(szTemp, 'F');
    if (pF != NULL)
    {
        *pF = '\0';
    }
    //��Ϊ����ΪnLen����ǰ�����Ӧ���ַ� 20004-06-02 by zkq
    if ((strlen(szTemp) == nLen) && (strlen(szTemp) != 0))
    {
    	strncat(szNumber, szTemp, 32 - strlen(szNumber));
    } 
    else
    {
    	strcpy(szNumber, szTemp); 
    }
    //end 
    char* pszCurField = (char *)pCurField;
   
    if (pFI->nDataType == FT_STRING)
    {
        if (pFI->nFieldLength < strlen(szNumber))
        {
            strncpy(pszCurField, szNumber, pFI->nFieldLength);                
            pszCurField[pFI->nFieldLength - 1] = '\0';
        }
        else
        {
            strcpy(pszCurField, szNumber);
        }           
 
        if (nIsQuery == 0)
        {
            return AddFillerSeparator(pszCurField, pFI);        
        }
    }
    else if (strlen(szTemp) != 0)
    {
        StringtoBCD(szNumber, (BYTE*)pCurField, nSrcFieldLen[0]); 
    }
    else
    {
    	memset(pCurField, 0xFF, pFI->nFieldLength);
    }      	
    
    return 0;
}

/*
  �������ܣ��������ƶ���SMS�����е������к��밴��������ת��Ϊ�ı�
            �����������Ϊ5�������ı����ͣ�����ΪBCD������
  ���������
          pSrcFields[0]/nSrcFieldLen[0] : ������ĺ���
          pSrcFields[1]/nSrcFieldLen[1] : ���������ֶ�
          pSrcFields[2]/nSrcFieldLen[2] : ���볤���ֶ�

  Added by ZhengYuqun      
*/
int CConvertFun::SMSNum2Text(void* pCurField, const UINT4 nSrcFields, 
                                    const void* pSrcFields[],
                                    const UINT4 nSrcFieldLen[],
                                    const SFieldInfo* pFI,
                                    const void* pDefault, const UINT4 nDefaultLen,
                                    int nIsQuery)
{
    int    nIndex    = 0;      //�ı���ʽ��BCD���ַ�������
    int    nBcdIndex = 0;      //BCD����������
    BYTE   yCurBcd   = 0;      //��ǰBCD��ֵ
    BYTE*  pBcdArray = NULL;   //BCD������ָ��
    char*  pCurStr   = NULL;   //�ı���ʽ��BCD���ַ���ָ��
    

    if(nSrcFields != 3)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_SMS_NUM_TO_TEXT, 1);
        return -1;
    }
    
    //ȷ�����������ȷ
    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }
    
    BYTE byNAI = *(BYTE*)pSrcFields[1];
    if(byNAI == 5) //��������Ϊ5��ʱ�����ASCII��
    {
        pCurStr = (char*)pCurField;
        BYTE byNumLen = *(BYTE*)pSrcFields[2];
        memcpy(pCurStr, (BYTE*)pSrcFields[0], byNumLen / 2); //��������Լ������Ҫ���ȳ���2
        pCurStr[byNumLen / 2] = '\0';
    }
    else
    {
        //��ʼ��BCD������ָ����ı���ʽ��BCD���ַ���ָ��
        pCurStr = (char*)pCurField;
        pBcdArray = (BYTE*)pSrcFields[0];
    
        //˳��ɨ��BCD�������ÿһ��Ԫ��
        while(nBcdIndex < nSrcFieldLen[0])
        {
            //��ȡ��ǰBCD���ֵ
            yCurBcd = pBcdArray[nBcdIndex++];
        
            //�������λ��������BCD��ֵ����С��16
            if(yCurBcd < 0xF0)
            {
                //�����ǰBCD��ֵС��10
                if(yCurBcd < 0xA0)
                {
                    pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
                }
            
                //�����ǰBCD��ֵ����10
                else
                {
                    pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
                }
            }
        
            //����BCD���������λ��
            else
            {
                break;
            }
        
            //���ε�ǰBCD������ֵ�ĸ���λ
            yCurBcd &= 0x0F;
        
            //�������λ��������BCD��ֵ����С��16
            if(yCurBcd < 0x0F)
            {
                //�����ǰBCD��ֵС��10
                if(yCurBcd < 0x0A)
                {
                    pCurStr[nIndex++] = yCurBcd + '0';
                }
            
                //�����ǰBCD��ֵ����10
                else
                {
                    pCurStr[nIndex++] = yCurBcd - 10 + 'A';
                }
            }
        
            //����BCD���������λ��
            else
            {
                break;
            }
        }
    
        //�����ַ����Ľ�����
        pCurStr[nIndex] = '\0';
    }
    
    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��        
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}

/*
  �������ܣ�����Ƕ���Ϣ��������diagnostics������Ϊgsm0902MapError��ֱ����normal release��ֵ
            ����ֱ�ӿ���diagnostics��ֵ
  ���������
          pSrcFields[0]/nSrcFieldLen[0] : ��������
          pSrcFields[1]/nSrcFieldLen[1] : Diagnostics

  Added by ZhengYuqun SWPD05305 2004-05-19     
*/
int CConvertFun::TermCause2G(void* pCurField, const UINT4 nSrcFields, 
                       const void* pSrcFields[],
                       const UINT4 nSrcFieldLen[],
                       const SFieldInfo* pFI,
                       const void* pDefault, const UINT4 nDefaultLen,
                       int nIsQuery)
{
/*
SDiagnostics�Ľṹ���£�
    typedef struct
    {
        BYTE   CauseType;
        union
        {
            UINT4     gsm0408Cause;                // CauseType == 0
            UINT4     gsm0902MapErrorValue;        // CauseType == 1
            UINT4     ccittQ767Cause;              // CauseType == 2
        } Cause;
    } SDiagnostics;
*/
#define CDRTYPE_MT_SMS	6
#define CDRTYPE_MO_SMS	7
#define NORMAL_CALL_RELEASE	144

    //ȷ�����������ȷ
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_TERM_CAUSE_2G, 1);
        return -1;
    }    
    
    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    
    if(nSrcFieldLen[0] != LEN_BYTE || nSrcFieldLen[1] != 5)
    {
        return -1;
    }
    
    BYTE byTermCause = NORMAL_CALL_RELEASE;  //���յĻ���ԭ��ֵ

    BYTE byCDRType = *(BYTE*)pSrcFields[0];
    BYTE byCauseType = *(BYTE*)pSrcFields[1];
    UINT4 uCause = 0;

    memcpy(&uCause, (BYTE*)pSrcFields[1] + 1, LEN_UINT4);
    //�϶��������ͻ������ֽ���ΪIntel�ֽ���
    //uCause = ITOHL(uCause);
    
    //Diagnostics������Ϊgsm0902MapError���һ�������Ϊ����Ϣ��ֱ����NORMAL_CALL_RELEASE
    if(byCauseType == 1 && (byCDRType == CDRTYPE_MT_SMS || byCDRType == CDRTYPE_MO_SMS))
    {
        byTermCause = NORMAL_CALL_RELEASE;
    }
    else
    {
        byTermCause = uCause;
    }
    
    memcpy(pCurField, &byTermCause, LEN_BYTE);
    
    return 0;
}

/*
  �������ܣ��������ƶ���HO�����е�PeerMnc��Ϊԭʼ������last mccmnc�ĸ��ֽڲ��ҷ���
            ���໰����PeerMnc��0xFF
  ���������
          pSrcFields[0]/nSrcFieldLen[0] : ��������
          pSrcFields[1]/nSrcFieldLen[1] : last mccmnc

  Added by ZhengYuqun SWPD05306 2004-05-19     
*/
int CConvertFun::PeerMnc2G(void* pCurField, const UINT4 nSrcFields, 
                     const void* pSrcFields[],
                     const UINT4 nSrcFieldLen[],
                     const SFieldInfo* pFI,
                     const void* pDefault, const UINT4 nDefaultLen,
                     int nIsQuery)
{
#define CDRTYPE_MO_HO	20
#define CDRTYPE_MT_HO	21
    
    //ȷ�����������ȷ
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_PEER_MNC_2G, 1);
        return -1;
    }    
    
    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    
    if(nSrcFieldLen[0] != LEN_BYTE && nSrcFieldLen[1] != 3)
    {
        return -1;
    }
    
    BYTE byCDRType = *(BYTE*)pSrcFields[0];
    BYTE byPeerMNC = *((BYTE*)pSrcFields[1] + 2);  //��mncȡ��ߵ�һ���ֽ�
    byPeerMNC = byPeerMNC >> 4 | byPeerMNC << 4; //���һ�Ҫ��һ��
    
    //����������Ͳ���2PLMN�л���������ôPeerMNC����0xFF
    if(byCDRType != CDRTYPE_MO_HO && byCDRType != CDRTYPE_MT_HO)
    {
        byPeerMNC = 0xFF;
    }
    
    memcpy(pCurField, &byPeerMNC, LEN_BYTE);
    
    return 0;
}

/*
  �������ܣ����䳤�ֶ���ĳ���ֶεĳ���תΪ�ı�
  ���������
          pSrcFields[0]/nSrcFieldLen[0] : �䳤�ֶ�
          pSrcFields[1]/nSrcFieldLen[1] : "m/n"��m��ʾ�ڼ����ֶΣ�n��ʾ�䳤�ֶ��й��������ֶ�

    Added by ZhengYuqun
*/
int CConvertFun::VarLen2Text(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[], 
                             const UINT4 nSrcFieldLen[], 
                             const SFieldInfo* pFI, 
                             const void* pDefault, const UINT4 nDefaultLen, 
                             int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_VAR_LEN_TO_TEXT, 2);
        return -1;
    }  

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (pSrcFields[1] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }
    
    int nLenIndOffset = 0;  //�ڼ�����ų���ָʾ���ֽ�
    UINT4 nLenInd = 0;      //��ų���ָʾ���ֽڵ�����
    
    char szPara2[128];
    char* pPos;
    strncpy(szPara2, (char*)pSrcFields[1], 128);
    pPos = strchr(szPara2, '/');
    if(pPos != NULL)
    {
        *pPos = '\0';
        nLenIndOffset = atoi(szPara2); 
        nLenIndOffset--;
    }
    else
    {
        return -1;
    }
    nLenInd = ((BYTE*)pSrcFields[0])[nLenIndOffset];

    ACE_OS_String::itoa(nLenInd, (char *)pCurField, 10);
    
    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��        
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}


/*
  �������ܣ����䳤�ֶ���ĳ���ֶε����ݰ�ʮ�����ƴ�תΪ�ı�
  ���������
          pSrcFields[0]/nSrcFieldLen[0] : �䳤�ֶ�
          pSrcFields[1]/nSrcFieldLen[1] : "m/n"��m��ʾ�ڼ����ֶΣ�n��ʾ�䳤�ֶ��й��������ֶ�

    Added by ZhengYuqun
*/
int CConvertFun::VarValue2Text(void* pCurField, const UINT4 nSrcFields, 
                               const void* pSrcFields[], 
                               const UINT4 nSrcFieldLen[], 
                               const SFieldInfo* pFI, 
                               const void* pDefault, const UINT4 nDefaultLen, 
                               int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_VAR_VALUE_TO_TEXT, 2);
        return -1;
    }  

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (pSrcFields[1] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    UINT4 nLenIndOffset = 0;  //�ڼ�����ų���ָʾ���ֽ�
    UINT4 nIndex = 0;         //Ҫת�����ֽڴ�����ʼƫ����

    //�ֱ�����������������ֵ
    char szPara2[128];
    char* pPos;
    strncpy(szPara2, (char*)pSrcFields[1], 128);
    pPos = strchr(szPara2, '/');
    if(pPos != NULL)
    {
        *pPos = '\0';
        nLenIndOffset = atoi(szPara2); 
        nIndex = atoi(pPos + 1);
        nLenIndOffset--;
    }
    else
    {
        return -1;
    }

    for(int i = 0; i < nLenIndOffset; i++)
    {
        nIndex += ((BYTE*)pSrcFields[0])[i];
    }
        
    //ȡ����ȷ�Ĳ���ת��Ϊ�ı�
    UINT4 nTotalByteCount = nSrcFieldLen[0];
    UINT4 nCopyByteCount = ((BYTE*)pSrcFields[0])[nLenIndOffset];
    UINT4 nCount = 0;
    for(nCount = 0; nIndex < nTotalByteCount && nCount < nCopyByteCount; nIndex++, nCount++)
    {
        BYTE yCurNum = ((BYTE*)pSrcFields[0])[nIndex];
        if(nCount * 3 + 4 >= FMT_FIELD_MAX_LEN)
        {
            break;
        }
        sprintf (&((char *)pCurField)[nCount * 3], "%02x ", yCurNum );
    }
    ((char *)pCurField)[nCount*3] ='\0';
    
    
    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��        
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}

/*
  �������ܣ��õ��䳤�ֶ���ĳ���ֶεĶ���������
  ���������
          pSrcFields[0]/nSrcFieldLen[0] : �䳤�ֶ�
          pSrcFields[1]/nSrcFieldLen[1] : "m/n"��m��ʾ�ڼ����ֶΣ�n��ʾ�䳤�ֶ��й��������ֶ�

    Added by ZhengYuqun
*/
int CConvertFun::GetVarValue(void* pCurField, const UINT4 nSrcFields, 
                               const void* pSrcFields[], 
                               const UINT4 nSrcFieldLen[], 
                               const SFieldInfo* pFI, 
                               const void* pDefault, const UINT4 nDefaultLen, 
                               int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_GET_VAR_VALUE, 2);
        return -1;
    }  

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (pSrcFields[1] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    UINT4 nLenIndOffset = 0;  //�ڼ�����ų���ָʾ���ֽ�
    UINT4 nIndex = 0;         //Ҫת�����ֽڴ�����ʼƫ����

    //�ֱ�����������������ֵ
    char szPara2[128];
    char* pPos;
    strncpy(szPara2, (char*)pSrcFields[1], 128);
    pPos = strchr(szPara2, '/');
    if(pPos != NULL)
    {
        *pPos = '\0';
        nLenIndOffset = atoi(szPara2); 
        nIndex = atoi(pPos + 1);
        nLenIndOffset--;
    }
    else
    {
        return -1;
    }

    for(int i = 0; i < nLenIndOffset; i++)
    {
        nIndex += ((BYTE*)pSrcFields[0])[i];
    }
        
    //ȡ����ȷ�Ĳ���
    UINT4 nTotalByteCount = nSrcFieldLen[0];
    UINT4 nCopyByteCount = min(((BYTE*)pSrcFields[0])[nLenIndOffset], nTotalByteCount - nIndex);
    memset((BYTE *)pCurField, 0xFF, FMT_FIELD_MAX_LEN);
    if(nCopyByteCount > 0)
    {
        memcpy((BYTE *)pCurField, (BYTE*)pSrcFields[0] + nIndex, min(nCopyByteCount, FMT_FIELD_MAX_LEN));
    }
   
    return 0;
}

/*
  �������ܣ���ȡ�䳤�ֶ���ĳ���ֶεĳ���
  ���������
          pSrcFields[0]/nSrcFieldLen[0] : �䳤�ֶ�
          pSrcFields[1]/nSrcFieldLen[1] : "m/n"��m��ʾ�ڼ����ֶΣ�n��ʾ�䳤�ֶ��й��������ֶ�

    Added by ZhengYuqun
*/
int CConvertFun::GetVarLen(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[], 
                             const UINT4 nSrcFieldLen[], 
                             const SFieldInfo* pFI, 
                             const void* pDefault, const UINT4 nDefaultLen, 
                             int nIsQuery)
{
    //ȷ�����������ȷ
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_GET_VAR_LEN, 2);
        return -1;
    } 

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (pSrcFields[1] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }
    
    int nLenIndOffset = 0;  //�ڼ�����ų���ָʾ���ֽ�
    INT8 nLenInd = 0;      //��ų���ָʾ���ֽڵ�����
    
    char szPara2[128];
    char* pPos;
    strncpy(szPara2, (char*)pSrcFields[1], 128);
    pPos = strchr(szPara2, '/');
    if(pPos != NULL)
    {
        *pPos = '\0';
        nLenIndOffset = atoi(szPara2); 
        nLenIndOffset--;
    }
    else
    {
        return -1;
    }
    nLenInd = ((BYTE*)pSrcFields[0])[nLenIndOffset];
    
    //��INT8���м������ת��Ϊ��Ҫ����������
    switch (pFI->nDataType)
    {
    case FT_BYTE:
        *(BYTE*)pCurField = nLenInd;
        break;
        
    case FT_SHORT:
    case FT_USHORT:
        *(UINT2*)pCurField = nLenInd;
        break;
        
    case FT_LONG:
    case FT_ULONG:
        *(UINT4*)pCurField = nLenInd;
        break;
        
    case FT_INT8:
        *(INT8*)pCurField = nLenInd;
        break;
        
    default:
        memcpy(pCurField, ((BYTE*)pSrcFields[0]) + nLenIndOffset, sizeof(BYTE));
        break;
    }
    
    return 0;
}

/************ The EBCD2String Defining **************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           1��������BCD��ת�����ַ�����
//           2��������BCD����B,Cת�����ַ�'*','#'��
//           ת��ʱ�������������ΪBCD���������
//           1��BCD�����0xE��2���ﵽBCD���������󳤶�
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           pSrcFields[0]ָ������BCD�����飬 
//           nSrcFieldLen[0]ָʾ������BCD������ĳ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::EBCD2String(void* pCurField, const UINT4 nSrcFields, 
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    int    nIndex    = 0;      //�ı���ʽ��BCD���ַ�������
    int    nBcdIndex = 0;      //BCD����������
    BYTE   yCurBcd   = 0;      //��ǰBCD��ֵ
    BYTE*  pBcdArray = NULL;   //BCD������ָ��
    char*  pCurStr   = NULL;   //�ı���ʽ��BCD���ַ���ָ��


    //ȷ�����������ȷ
    if((pCurField == NULL) || (nSrcFields != 1 && nSrcFields != 2) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //��װBCD��
    BYTE pNumber[40];
    int  nSrcNumLen = 0;
    if(nSrcFields == 1)
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];
    }
    else
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];

        memcpy(pNumber + nSrcNumLen, pSrcFields[1], nSrcFieldLen[1]);
        nSrcNumLen += nSrcFieldLen[1];        
    }

    //��ʼ��BCD������ָ����ı���ʽ��BCD���ַ���ָ��
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pNumber;

    //˳��ɨ��BCD�������ÿһ��Ԫ��
    while(nBcdIndex < nSrcNumLen)
    {
        //��ȡ��ǰBCD���ֵ
        yCurBcd = pBcdArray[nBcdIndex++];

        //�������λ��������BCD��ֵ����С��15
        if((yCurBcd & 0xF0)< 0xE0)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
				if((yCurBcd & 0xF0) == 0xB0)
                {
					pCurStr[nIndex++] = '*';
                }
				else if((yCurBcd & 0xF0) == 0xC0)
                {
					pCurStr[nIndex++] = '#';
                }
				else
				{
					pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
				}
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }

        //���ε�ǰBCD������ֵ�ĸ���λ
        yCurBcd &= 0x0F;

        //�������λ��������BCD��ֵ����С��15
        //��sofx3000��designated charging_num ��ʾ��EΪ����       
        if(yCurBcd < 0x0E)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
				if(yCurBcd == 0x0B)
                {
					pCurStr[nIndex++] = '*';
                }
				else if(yCurBcd == 0x0C)
                {
					pCurStr[nIndex++] = '#';
                }
                else
				{
					pCurStr[nIndex++] = yCurBcd - 10 + 'A';
				}
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }
    }

    //�����ַ����Ľ�����
    pCurStr[nIndex] = '\0';

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��        
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The EBCD2String Defining *******************/

/************ The BCDTime2Sec Defining **************************/
// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
//           ������BCD��ת�����ַ�����������ʱ�����BCD��ת����������ʾ���ַ���
//           ת��ʱ�������������ΪBCD���������
//           1��BCD�����0xF��2���ﵽBCD���������󳤶�
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
//           pSrcFields[0]ָ������BCD�����飬 
//           nSrcFieldLen[0]ָʾ������BCD������ĳ���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CConvertFun::BCDTime2Sec(void* pCurField, const UINT4 nSrcFields, 
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    
    if(0 != BCD2String(pCurField,  nSrcFields, pSrcFields, nSrcFieldLen, 
                       pFI, pDefault,  nDefaultLen,nIsQuery))
    {
        return -1;
    }
    
    //������ʱ�����BCD��ת����������ʾ���ַ���
    char*  pCurStr   = NULL;   //�ı���ʽ��BCD���ַ���ָ��   
	char   szTmp[4];
    pCurStr = (char*)pCurField;        
    UINT4 uSec = 0;
    UINT2 uTmp = 0;
    int  yMsec =0;  
    int   nIndex = 0;   
	
    memcpy(szTmp, pCurStr, 3 * sizeof(char)); //Сʱ
	szTmp[3] = '\0';
	uTmp = atoi(szTmp);
    uSec += uTmp * 3600;    
    pCurStr += 3; //���������ַ�    
    
    memcpy(szTmp, pCurStr, 2 * sizeof(char)); //����
	szTmp[2] = '\0';
	uTmp = atoi(szTmp);
    uSec += uTmp * 60;    
    pCurStr += 2; //���������ַ�    
   
    memcpy(szTmp, pCurStr, 2 * sizeof(char)); //��
    szTmp[2] = '\0';
	uTmp = atoi(szTmp);
    uSec += uTmp;
    pCurStr += 2; //���������ַ�     
    
    memcpy(szTmp, pCurStr, sizeof(char)); //100����    
    szTmp[1] = '\0';
	yMsec = atoi(szTmp);
    pCurStr[0] = '\0';
    sprintf((char*)pCurField, "%u.%u", uSec,yMsec); //ת�����ַ�����
    
    
    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}
/************ End of The BCDTime2Sec Defining *******************/

//End  by zkq for TT&T 2004.5.27


//SWPD06703 Zheng Yuqun 2004-06-19
//���ݺ����ʵ�ʳ�����ת��BCD
//��һ������ΪBCD���ֶ�
//�ڶ�������Ϊ�����ֶΣ�����Ϊת��Ϊ�ַ�֮��ĳ��ȣ���BCD���ֽ���
int CConvertFun::BCD2StringByLen(void* pCurField, const UINT4 nSrcFields,
                                const void* pSrcFields[],
                                const UINT4 nSrcFieldLen[],
                                const SFieldInfo* pFI,
                                const void* pDefault, const UINT4 nDefaultLen,
                                int nIsQuery)
{
    int    nIndex    = 0;      //�ı���ʽ��BCD���ַ�������
    int    nBcdIndex = 0;      //BCD����������
    BYTE   yCurBcd   = 0;      //��ǰBCD��ֵ
    BYTE*  pBcdArray = NULL;   //BCD������ָ��
    char*  pCurStr   = NULL;   //�ı���ʽ��BCD���ַ���ָ��



    //ȷ�����������ȷ
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_INVALID_PARA_NUM,
              FUN_BCD_TO_STR_BY_LEN, nSrcFields);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0) 
        || (pSrcFields[1] == NULL) || (nSrcFieldLen[1] == 0))
    {
        return -1;
    }

    int  nSrcNumLen = 0;
    nSrcNumLen = nSrcFieldLen[0];

    //��ȡʵ�ʳ���
    INT4 nActulLen = 0;
    if(nSrcFieldLen[1]  == LEN_BYTE)
    {
        BYTE uTmp = 0;
        memcpy(&uTmp, pSrcFields[1], LEN_BYTE);
        nActulLen = uTmp;
    }
    else if(nSrcFieldLen[1]  == LEN_UINT2)
    {
        UINT2 uTmp = 0;
        memcpy(&uTmp, pSrcFields[1], LEN_UINT2);
        //nActulLen = ITOHS(uTmp);
        nActulLen = uTmp;
    }
    else if(nSrcFieldLen[1]  == LEN_UINT4)
    {
        UINT4 uTmp = 0;
        memcpy(&uTmp, pSrcFields[1], LEN_UINT4);
        //nActulLen = ITOHL(uTmp);
        nActulLen = uTmp;
    }
    else
    {
        return -1;
    }

    //��ʼ��BCD������ָ����ı���ʽ��BCD���ַ���ָ��
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    //˳��ɨ��BCD�������ÿһ��Ԫ��
    while(nBcdIndex < nSrcNumLen && nActulLen > 0)
    {
        //��ȡ��ǰBCD���ֵ
        yCurBcd = pBcdArray[nBcdIndex++];

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0xF0)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
				//add by zkq 2004-05-27
				if((yCurBcd & 0xF0) == 0xB0)
                {
                   pCurStr[nIndex++] = '*';
                }
				else if((yCurBcd & 0xF0) == 0xC0)
                {
                   pCurStr[nIndex++] = '#';
                }//add end 
				else
				{
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
				}
            }

            nActulLen--;

            //Added by ZhengYuqun SWPD06312 2004-06-28
            if(nActulLen == 0)
            {
                break;
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }

        //���ε�ǰBCD������ֵ�ĸ���λ
        yCurBcd &= 0x0F;

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0x0F)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
				//add by zkq 2004-05-27
				if(yCurBcd == 0x0B)
                {
                   pCurStr[nIndex++] = '*';
                }
				else if(yCurBcd == 0x0C)
                {
                   pCurStr[nIndex++] = '#';
                }//add end
				else
				{
                    pCurStr[nIndex++] = yCurBcd - 10 + 'A';
				}
            }

            nActulLen--;

            //Added by ZhengYuqun SWPD06312 2004-06-28
            if(nActulLen == 0)
            {
                break;
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }
    }

    //�����ַ����Ľ�����
    pCurStr[nIndex] = '\0';

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //��ѯ����ת��ʱ����Ҫ�����κ������ͷָ���
    return 0;
}


/************ The GetArgSpecField Defining ***************************/
// �������������µ�ǰ׺�滻�绰�����оɵ�ǰ׺�����������þ����Ƿ����Ӻ��׼���
//           �滻ԭ��:
//           �绰����ǰ׺�����ƥ��ԭ����ǰ׺���е�ǰ׺����ƥ�䡣                
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ2����3��
// 
//           pSrcFields[0]/nSrcFieldLen[0] : �м�Ⱥ��/�м�Ⱥ�ŵĳ���/��ѡ����
//           ��Ϊ�������������ģ���������һ�������������յ��ֶ����ƣ�
//           ����λ��pSrcFields[1]��pSrcFields[2]��
//           ��2�����ʱ��pSrcFields[1]���ֶ�Ϊ���к���
//
// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
//
// ����ֵ���ɹ�����0��ʧ�ܷ���-1
// 
// Added by chenliangwei , 2004-8-3
//�޸ļ�¼��
/***************************************************************/
typedef struct _STrunkGroupAttr
{
    UINT2     nTrunkGroupID;
    UINT4     nCarrierID;
    char      szCountry[10];
    UINT4     nCountryID;
} STrunkGroupAttr;

typedef MAP<UINT2, STrunkGroupAttr> TRUNK_TABLE;


int CConvertFun::GetArgSpecField(void* pCurField, const UINT4 nSrcFields,
                              const void* pSrcFields[],
                              const UINT4 nSrcFieldLen[],
                              const SFieldInfo* pFI,
                              const void* pDefault, const UINT4 nDefaultLen,
                              int nIsQuery)
{
    //ȷ�����������ȷ
    if((pCurField == NULL) || nSrcFields < 2 || nSrcFields > 3 || (pSrcFields == NULL))
    {
        return -1;
    }

    if ((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }


    static TRUNK_TABLE trunkTable;
	static BOOL bIsFirstRun = TRUE;
    static ACE_Mutex mutex;    

    /*
     * ����ǰ׺��������ļ���
     */
    mutex.acquire();
    if(bIsFirstRun)
    {
        //��ȡ�������м�Ⱥ���Ա�
		FILE* f = fopen("./config/area/trunktable.cfg", "rt");
        if( f == NULL)
        {
            CreateDeepDir("./config/area");
            f = fopen("./config/area/trunktable.cfg", "w");
            if(f == NULL)
            {
                mutex.release();
                return -1;
            }
            fclose(f);
        }
        else
        {
            char szLine[1024];
            while(!feof(f))
            {
                char* p = fgets(szLine, sizeof(szLine), f);
                if((p != NULL) && (szLine[0] != '\0'))
                {
                    STrunkGroupAttr stTrunkAttr;
                    memset(&stTrunkAttr, 0, sizeof(STrunkGroupAttr));

                    int nRead = sscanf(szLine, "%d%d%s%d", 
                                       &stTrunkAttr.nTrunkGroupID,
                                       &stTrunkAttr.nCarrierID, 
                                       stTrunkAttr.szCountry,
                                       &stTrunkAttr.nCountryID);
                    if(nRead != 4)
                    {
                        continue;
                    }

                    trunkTable[stTrunkAttr.nTrunkGroupID] = stTrunkAttr;
                }
            }
            fclose(f);
        }
		bIsFirstRun = FALSE; 
    }
    mutex.release();

    /*
     * ���ݲ�ͬ��Ŀ���ֶ�������ȡ��ͬ��ֵ��
     */
    mutex.acquire();
    char* szDstField = (char*)pSrcFields[nSrcFields - 1];
    UINT2 nTrunkGroupID = *(UINT2*)pSrcFields[0];

    BOOL bFind = (trunkTable.find(nTrunkGroupID) != trunkTable.end());
    if(StringiCmp(szDstField, "SubscriberB"))
    {
        //���绰�����BCD��ת��Ϊ�ı�
        char szNumber[31] = "";
        BCDtoString((BYTE*)pSrcFields[1], szNumber, nSrcFieldLen[1], sizeof(szNumber) - 1);
        szNumber[sizeof(szNumber) - 1] = '\0';
        char* pF = strchr((char*)szNumber, 'F');
        if(pF != NULL)
        {
            *pF = '\0';
        }                

        if(bFind)
        {
            STrunkGroupAttr& stTrunkAttr = trunkTable[nTrunkGroupID];
            if(StringiCmp(stTrunkAttr.szCountry, "USA"))
            {
                char szTemp[31];
                SNPRINTF(szTemp, sizeof(szTemp), 
                        "1%s", szNumber);
                szTemp[sizeof(szTemp) - 1] = '\0';
                strcpy(szNumber, szTemp);
            }
        }
        strcpy((char*)pCurField, szNumber);
    }
    else if(StringiCmp(szDstField, "DestType"))
    {
        UINT4 nDestType = 0;
        if(bFind)
        {
            STrunkGroupAttr& stTrunkAttr = trunkTable[nTrunkGroupID];
            nDestType = stTrunkAttr.nCountryID;
            sprintf((char*)pCurField, "%d", nDestType);
        }
        else
        {
            sprintf((char*)pCurField, "FFF");
        }
    }
    else if(StringiCmp(szDstField, "CarrierID"))
    {
        UINT4 nCarrierID = 0;
        if(bFind)
        {
            STrunkGroupAttr& stTrunkAttr = trunkTable[nTrunkGroupID];
            nCarrierID = stTrunkAttr.nCarrierID;
            sprintf((char*)pCurField, "%d", nCarrierID);
        }
        else
        {
            sprintf((char*)pCurField, "FFF");
        }
    }
    else
    {
        mutex.release();
        return -1;
    }
    mutex.release();

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    return 0;
}



/***************      The Define of  IntPlus   ************************/
// ���ܣ�������ͳ������ӷ�����
//
// ���������
//           pCurField      : ��Ž��
//           nSrcFields     : �ڱ�������Ӧ�õ���2
//           pSrcFields     : ���ӷ�������򣬻���
//           nSrcFieldLen   : ��ӦpSrcFields�ĳ���
//
// ����� -1��ʧ��, 0: �ɹ�
//
// Added By Lucy 2004-08-16
/**********************************************************************/
int CConvertFun::Add(void* pCurField, const UINT4 nSrcFields,
                   const void* pSrcFields[], 
                   const UINT4 nSrcFieldLen[], 
                   const SFieldInfo* pFI, 
                   const void* pDefault, const UINT4 nDefaultLen,
                   int nIsQuery)
{
    //�������
    UINT4 nSrc[2] = {0, 0};

    //��֤���������ȷ
    if((pCurField == NULL) || (nSrcFields != 2)|| 
        (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_ADD, nSrcFields);        
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }
    //end of ��֤
    
    //��ȡԭ��ʽ�������ֵ
    if(nSrcFieldLen[0] == LEN_BYTE)
    {
        BYTE uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_BYTE);
        nSrc[0] = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT2)
    {
        UINT2 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT2);
        nSrc[0] = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT4)
    {
        UINT4 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT4);
        nSrc[0] = uTmp;
    }    
    else
    {
        return -1;
    }

    BOOL bIsAllF = TRUE;  //�Ƿ�ȫΪ0xF
    
    for(int j = 0; j < nSrcFieldLen[0]; j++)
    {
        if(*((BYTE*)(pSrcFields[0]) + j) != 0xFF)
        {
            bIsAllF = FALSE;
            break;
        }
    }
    
    //���ȫ��ΪF
    if(bIsAllF)
    {
        sprintf((char*)pCurField, "%u", nSrc[0]);
    }
    else
    {    
        const char* szSrc = (const char*)pSrcFields[1];
        nSrc[1] = atoi(szSrc);
    
        sprintf((char*)pCurField, "%u", nSrc[0] + nSrc[1]);
    }

    //��Ŀ���ʽ�����������ͷָ���
    if(!nIsQuery)
    {
        //�������Ϊ��ѯ����ת��
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;
}


















