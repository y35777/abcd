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
��������    ���û�����¼�������ݺ��������ݻ�����¼�õ�ת������ı�����
�������    pData----������¼ָ�룬��Ϊ��ʱ��ʾһ�Ŵ��󻰵�
            uLen----������¼����
            pMutex---��ʽ����Ļ�����
            pFormat----������ʽ
�������    ��
����ֵ      �ɹ����
�׳��쳣    ��
�޸ļ�¼
    2002.3.30   ����Զ      �޸Ĵ��룬�ڻ������ݴ���ʱ���ô�����Ϣ
**********************************************************************/
BOOL CBillRecord::SetContent(BYTE* pData, UINT4 uLen, CFormat* pFormat, ACE_Mutex* pMutex)
{
    ASSERT(NULL != pFormat);

    //����Զ2002.3.30��ӣ����ⵥ��D12265
    if (NULL == pData)
    {
        if(NULL != pMutex)
            pMutex->acquire();
        SetErrorContent(pFormat);
        if(NULL != pMutex)
            pMutex->release();

        return TRUE;
    }
    //����Զ2002.3.30��ӽ���
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
    //�ӷ��صĻ��������У����ݷָ��������������ݷָ�ɸ�����
    //�����뵽m_ContentList��
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
        //���һ����ĺ���û�и��ָ��������뵥�����д���
        if (i < uFieldCount - 1)
        {
            szField = (char* )pDstData + nFieldStartPos;

            int nFieldLen = strlen(szField.c_str());
            if (!IsValidStr(szField.c_str(), nFieldLen))
            //��MML�����л���������ݲ���Ϊ�գ���"-"����
            {
                m_ContentList.push_back("-");
            }

            //by ldf 2003-11-05 ��Ӧ���ⵥ SWPD00365
            //��ԭ����ʾΪ255�ֽڸ�Ϊ��ʾ1000�ֽ�
            else if (nFieldLen > 1000)
            //��MML�����е���������ĳ��Ȳ���̫����̫��ʱֻ��ʾ1000���ַ���
            //Ȼ���ټ�"..."
            {
                //����Զ2002.03.06�޸ģ����ⵥ��D11482
                char szTmpField[1005];
                memset(szTmpField, 0, 1005);
                memcpy(szTmpField, szField.c_str(), 1000);
                strcat(szTmpField, "...");
                m_ContentList.push_back(szTmpField);
                //�޸Ľ���������Զ2002.03.06
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
            //��MML�����л���������ݲ���Ϊ�գ���"-"����
            {
                m_ContentList.push_back("-");
            }

            //by ldf 2003-11-05 ��Ӧ���ⵥ SWPD00365
            //��ԭ����ʾΪ255�ֽڸ�Ϊ��ʾ1000�ֽ�
            else if (nFieldLen > 1000)
            //��MML�����е���������ĳ��Ȳ���̫����̫��ʱֻ��ʾ1000���ַ���
            //Ȼ���ټ�"..."
            {
                //����Զ2002.03.06�޸ģ����ⵥ��D11482
                char szTmpField[1005];
                memset(szTmpField, 0, 1005);
                memcpy(szTmpField, szLastField, 1000);
                strcat(szTmpField, "...");
                m_ContentList.push_back(szTmpField);
                //�޸Ľ���������Զ2002.03.06
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
��������    ��ȡ������¼����
�������    ��
�������    ContentList----���������ݵ��б�
����ֵ      ContentList�а����ĸ���
�׳��쳣    ��
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

//����Զ2002.3.30��ӣ����ⵥ��D12265
/**********************************************************************
��������    �ڻ������������ô�����Ϣ
�������    pFormat����������ʽ����
�������    ��
����ֵ      ��
�׳��쳣    ��
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
//����Զ2002.3.30��ӽ���


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