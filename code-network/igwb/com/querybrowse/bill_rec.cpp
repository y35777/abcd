#include "../include/frame.h"
#include "bill_rec.h"
#include "../include/frame.h"
#include "resource.h"

CBillRecord::CBillRecord()
{

}

CBillRecord::~CBillRecord()
{

}

/**********************************************************************
功能描述    设置话单记录数据内容函数，根据话单记录得到转换后的文本描述
输入参数    pData----话单记录指针，在为空时表示一张错误话单
            uLen----话单记录长度
            pMutex---格式对象的互斥量
            pFormat----话单格式
输出参数    无
返回值      成功与否
抛出异常    无
修改记录
    2002.3.30   颜文远      修改代码，在话单内容错误时设置错误信息
**********************************************************************/
BOOL CBillRecord::SetContent(BYTE* pData, UINT4 uLen, CFormat* pFormat, ACE_Mutex* pMutex)
{
    ASSERT(NULL != pFormat);

    //颜文远2002.3.30添加，问题单号D12265
    if (NULL == pData)
    {
        if(NULL != pMutex)
            pMutex->acquire();
        SetErrorContent(pFormat);
        if(NULL != pMutex)
            pMutex->release();

        return TRUE;
    }
    //颜文远2002.3.30添加结束
    void* pDstData;
    UINT4 uDstLen;
    if(NULL != pMutex)
        pMutex->acquire();
    int nResult = pFormat->ConvertABill(pData, uLen, pDstData, uDstLen);
    if (0 != nResult)
    {
        if(NULL != pMutex)
            pMutex->release();
        return FALSE;
    }
    //从返回的话单内容中，根据分隔符，将话单内容分割成各个域，
    //并加入到m_ContentList中
    UINT4 uFieldCount = pFormat->GetFieldNum();
    const SFieldInfo** pFieldArr = pFormat->GetFieldsArray();
    if(NULL != pMutex)
        pMutex->release();

    int nFieldStartPos = 0;
    for (int i = 0; i < uFieldCount; i++)
    {
        STRING szField;
        char cSeparator;
        cSeparator = pFieldArr[i]->cSeparator;
        int j;
        for (j = nFieldStartPos; j < uDstLen; j++)
        {
            if (cSeparator == ((char* )pDstData)[j])
            {
                ((char* )pDstData)[j] = '\0';
                break;
            }
        }
        //最后一个域的后面没有跟分隔符，必须单独进行处理
        if (i < uFieldCount - 1)
        {
            szField = (char* )pDstData + nFieldStartPos;

            int nFieldLen = strlen(szField.c_str());
            if (!IsValidStr(szField.c_str(), nFieldLen))
            //在MML报告中话单域的内容不能为空，用"-"代替
            {
                m_ContentList.push_back("-");
            }

            //by ldf 2003-11-05 对应问题单 SWPD00365
            //把原来显示为255字节改为显示1000字节
            else if (nFieldLen > 1000)
            //在MML报告中单个话单域的长度不能太长，太长时只显示1000个字符，
            //然后再加"..."
            {
                //颜文远2002.03.06修改，问题单号D11482
                char szTmpField[1005];
                memset(szTmpField, 0, 1005);
                memcpy(szTmpField, szField.c_str(), 1000);
                strcat(szTmpField, "...");
                m_ContentList.push_back(szTmpField);
                //修改结束，颜文远2002.03.06
            //SWPD00365 end
            }
            else
            {
                m_ContentList.push_back(szField);
            }
            nFieldStartPos = j + 1;
        }
        else
        {
            if ((int)uDstLen - nFieldStartPos < 0)
            {
                return FALSE;
            }

            char* szLastField = new char[uDstLen - nFieldStartPos + 1];
            memcpy(szLastField, (char* )pDstData + nFieldStartPos,
                    uDstLen - nFieldStartPos);
            szLastField[uDstLen - nFieldStartPos] = '\0';

            int nFieldLen = strlen(szLastField);
            if (!IsValidStr(szLastField, nFieldLen))
            //在MML报告中话单域的内容不能为空，用"-"代替
            {
                m_ContentList.push_back("-");
            }

            //by ldf 2003-11-05 对应问题单 SWPD00365
            //把原来显示为255字节改为显示1000字节
            else if (nFieldLen > 1000)
            //在MML报告中单个话单域的长度不能太长，太长时只显示1000个字符，
            //然后再加"..."
            {
                //颜文远2002.03.06修改，问题单号D11482
                char szTmpField[1005];
                memset(szTmpField, 0, 1005);
                memcpy(szTmpField, szLastField, 1000);
                strcat(szTmpField, "...");
                m_ContentList.push_back(szTmpField);
                //修改结束，颜文远2002.03.06
            //SWPD00365 end
            }
            else
            {
                m_ContentList.push_back(szLastField);
            }

            delete[] szLastField;
        }
    }

    delete[] pDstData;

    return TRUE;
}

/**********************************************************************
功能描述    获取话单记录内容
输入参数    无
输出参数    ContentList----话单域内容的列表
返回值      ContentList中包含的个数
抛出异常    无
**********************************************************************/
int CBillRecord::GetContent(LIST<STRING>& ContentList)
{
    int nCount = 0;

    LIST<STRING>::iterator i;
    for (i = m_ContentList.begin(); i != m_ContentList.end(); i++)
    {
        ContentList.push_back(*i);
        nCount++;
    }

    return nCount;
}

//颜文远2002.3.30添加，问题单号D12265
/**********************************************************************
功能描述    在话单内容中设置错误信息
输入参数    pFormat——话单格式对象
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillRecord::SetErrorContent(CFormat* pFormat)
{
    UINT4 uFieldCount = pFormat->GetFieldNum();

    ASSERT(uFieldCount != 0);

    m_ContentList.push_back(S_QB_ERROR_FIELD);

    for (int i = 1; i < uFieldCount; i++)
    {
        m_ContentList.push_back("-");
    }
}
//颜文远2002.3.30添加结束


BOOL CBillRecord::IsValidStr(const char* szStr, int nStrLen)
{
    BOOL bValid = FALSE;

    for (int i = 0; i < nStrLen; i++)
    {
        if (0 == szStr[i])
        {
            break;
        }

        if ((' ' != szStr[i]) && ('\r' != szStr[i])
            && ('\n' != szStr[i]) && ('\t' != szStr[i]))
        {
            bValid = TRUE;
            break;
        }
    }

    return bValid;
}