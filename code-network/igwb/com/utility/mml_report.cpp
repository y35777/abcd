/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/
#include "mml_report.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//��������  ���캯����ɳ�Ա�����ĳ�ʼ��.
//�������  szName����ָʾMML�����е�Դ��ʶ��
//������ 2001-10-13
/////////////////////////////////////////////////////////////////////
CMMLReport::CMMLReport(const char* szName)
{
   //��ʼ����������Ա;
    m_nReportLen    = 0;
    m_nHeadPosition = 0;
    m_nBufLen       = 64*1024;
    m_pszBuf        = new char[m_nBufLen];

    //��<��ʼ����ʶ +++><4SP><Դ��ʶ��'L32><8SP>��ʽ��ʼ�����ֱ���ͷ;
    AddString(MML_HEAD);
    FillSpace(4);
    AddString(szName, 32, ALIGN_LEFT);
    FillSpace(8);

    //���ݵ�ǰ������,���¼���m_nHeadPostion;
    m_nHeadPosition = m_nReportLen;
}

CMMLReport::~CMMLReport()
{
    if (m_pszBuf != NULL)
    delete[] m_pszBuf;
}
///////////////////////////////////////////////////////////////////////////
//��������  ��pString������ƴ�ӵ�MML������.
//�������
//pString  -���뱨���е�����;
//nWidth   -�ñ�������Ӧռ�Ŀ��,��nWidth���ȴ���pStringʱ,
//          �ÿո����ʣ�ಿ�ֻ�����;
//alignment-��дpString��������ʱ�Ķ��뷽ʽ,
//          ȡֵ����: ALIGN_LEFT,ALIGN_RIGHT,ALIGN_NONE.
//������2001-10-13
///////////////////////////////////////////////////////////////////////////
void CMMLReport::AddString(const char* pString,
                           int nWidth,
                           int alignment)
{
   //���pString�ַ������ȴ��ڵ���nWidth,����pString��ǰ
    //nWidth���ַ���������;
    if(pString == NULL)
    {
        return;
    }
    //������ 2002-07-30 D15553
    if(m_nReportLen + nWidth >= m_nBufLen)
    {
        return;
    }//����
    int nstrlen = strlen(pString);
    if(nstrlen >= nWidth)
    {
        memcpy(m_pszBuf + m_nReportLen, pString, nWidth);
        m_nReportLen += nWidth;
    }
    //������ݶ��뷽ʽҪ�󿽱�pString�����ݵ�������;
    else if(alignment == ALIGN_LEFT)//����뷽ʽ
    {
        memcpy(m_pszBuf + m_nReportLen, pString, nstrlen);
        m_nReportLen +=  nstrlen;
        FillSpace(nWidth - nstrlen);
    }
    else if(alignment == ALIGN_RIGHT)
    {
        FillSpace(nWidth - nstrlen);
        memcpy(m_pszBuf + m_nReportLen, pString, nstrlen);
        m_nReportLen +=  nstrlen;
    }
    else
    {
        memcpy(m_pszBuf + m_nReportLen, pString, nstrlen);
        m_nReportLen +=  nstrlen;
    }
}

//////////////////////////////////////////////
//��������  ��pString������ƴ�ӵ�MML������.
//�������  pString-���뱨���е�����;
//������    2001-10-13
//////////////////////////////////////////////
void CMMLReport::AddString(const char* pString)
{
    //ƴ��pString��MML������;
    if(pString == NULL)
    {
        return;
    }
    int nstrlen = strlen(pString);
    //������ 2002-07-30 ���ⵥD15553
    if(m_nReportLen + nstrlen >= m_nBufLen)
    {
        return;
    }//�޸Ľ���
    memcpy(m_pszBuf + m_nReportLen, pString, nstrlen);
    m_nReportLen +=  nstrlen;

}
///////////////////////////////////////////////
//��������  �ӵ�ǰλ�����nSp���ո�MML������.
//�������  nSp -Ҫ����Ŀո���
//������2001-10-13
//////////////////////////////////////////////
void CMMLReport::FillSpace(int nSp)
{
    //������ 2002-07-30 D15553
    if(m_nReportLen + nSp >= m_nBufLen)
    {
        return;
    }//����
    //��m_nReportLen��ʼ���ո�
    for(int i = 0; i < nSp; i++)
    {
        m_pszBuf[m_nReportLen++] = CHAR_SPACE;
    }
}
/////////////////////////////////////////////////////////////////////////
//��������  ���ݱ���������������װ����
//          <�澯����'L16A1><2SP><=><2SP><�澯����'L50A0>\r\n�ı�������
//�������  STRING szTitle  -�Ⱥ���ߵ��ַ�
//      STRING szContent    -�Ⱥ��ұߵ��ַ�
//      int nFirstFormatLen -��һ���������ַ����
//      int nFirstAign      -��һ�������Ķ��뷽ʽ
//      int nSecondFormatLen-�ڶ����������ַ����
//      int nSecondAlign    -�ڶ��������Ķ��뷽ʽ
//������2001-10-13
/////////////////////////////////////////////////////////////////////////
void CMMLReport::MakeReportItem(const char* szTitle,
                                const char* szContent,
                                int nFirstFormatLen,
                                int nFirstAign,
                                int nSecondFormatLen,
                                int nSecondAlign,
                                BYTE bNormal)
{
    int nTitleLen = strlen(szTitle);
    int nConLen = strlen(szContent);
    int nContentLen = nConLen + nTitleLen + strlen(MML_FILL1);

    //�������̫��,��Ҫ������ʾ
    if(nContentLen >= nSecondFormatLen || nSecondFormatLen > 50)
    {
        char tempContent[1024];
        char tempTitle[512];
        BYTE nSecondSpace = 0;

        //��������ʾ���ͻ�����Ϣ���һ������,Ȼ������������µı���:
        //      ��λ��Ϣ ��  ��Ϊ��Ϣ1 = ��λ��Ϣ1��ֵ  ��λ��Ϣ2 = ��λ��Ϣ2
        //                   ֵ��λ��Ϣ  ��λ��Ϣ3 = ��λ��Ϣ3��ֵ
        if(nTitleLen >= nFirstFormatLen)
        {
            memcpy(tempTitle, szTitle, nFirstFormatLen);
            tempTitle[nFirstFormatLen] = '\0';
        }
        else if(nFirstAign == ALIGN_LEFT)//����뷽ʽ
        {
            memcpy(tempTitle, szTitle, nTitleLen);
            memset(tempTitle + nTitleLen, CHAR_SPACE, nFirstFormatLen-nTitleLen);
            tempTitle[nFirstFormatLen] = '\0';
        }
        else if(nFirstAign == ALIGN_RIGHT)
        {
            memset(tempTitle, CHAR_SPACE, nFirstFormatLen - nTitleLen);
            memcpy(tempTitle + nFirstFormatLen - nTitleLen, szTitle, nTitleLen);
            tempTitle[nFirstFormatLen] = '\0';
        }
        else
        {
            strcpy(tempTitle, szTitle);
        }
        if(nSecondAlign == ALIGN_LEFT)
            sprintf(tempContent, "%s%s%s", tempTitle, MML_FILL1, szContent);
        else
        {
            char temp[128];
            memset(temp,CHAR_SPACE,nSecondFormatLen);
            temp[nSecondFormatLen] = '\0';
            nSecondSpace = nSecondFormatLen;
            sprintf(tempContent,"%s%s%s%s",tempTitle,MML_FILL1,temp,szContent);
        }
        STRING sOutput;
        MultiLine(tempContent,sOutput,strlen(tempTitle)+strlen(MML_FILL1)+nSecondSpace,80,bNormal);
        AddString(sOutput.c_str());
    }
    //�����㹻�Ļ�,��ƴ��
    else
    {
    //ƴ��szTitle������
        AddString(szTitle,  nFirstFormatLen,nFirstAign);
    //���<2SP><=><2SP>����������
        AddString(MML_FILL1);
    //ƴ��szContent������,���ܶ���,���к��һ���Ⱥ��ұ������
        AddString(szContent,nSecondFormatLen,nSecondAlign);
    }

    //��ӻس�����
    AddString(STR_NEWLINE);
}

void CMMLReport::MakeReportItem2(const char* szTitle,
                                 const char* szContent,
                                 int nFirstFormatLen,
                                 int nFirstAign,
                                 int nSecondFormatLen,
                                 int nSecondAlign)
{
    MakeReportItem(szTitle,
                   szContent,
                   nFirstFormatLen,
                   nFirstAign,
                   nSecondFormatLen,
                   nSecondAlign,
                   1);
}
////////////////////////////////////////////////////////
//��������  ���ݱ���ͷ��ʽ��װ����ͷ,��ʽΪ
//          <��ʼ����ʶ +++><4SP><Դ��ʶ��'L32><8SP><�����������'L10><1SP><�������ʱ��'L8>\r\n
//������2001-10-13
////////////////////////////////////////////////////////
void CMMLReport::MakeReportHead()
{
    //��m_nReportLen = m_nHeadPosition,��ʾ��ʼ��m_nHeadPosition����
    //ƴ�ӱ�������,������m_nHeadPosition��ʼ��������0
    m_nReportLen = m_nHeadPosition;
    memset(m_pszBuf + m_nHeadPosition,0,m_nBufLen - m_nHeadPosition);
    //ͨ��GetStrDate��õ�ǰ�����ַ���,����ӵ�������
    time_t  tt;
    struct tm* t;
    time(&tt);
    t = localtime(&tt);
    char szTime[20];
    sprintf(szTime, "%04d-%02d-%02d",t->tm_year+1900, t->tm_mon+1, t->tm_mday);
    AddString(szTime, 10, ALIGN_LEFT);
    FillSpace(1);

    //ͨ��GetStrTime��õ�ǰʱ���ַ���,����ӵ�������
    sprintf(szTime, "%02d:%02d:%02d",t->tm_hour, t->tm_min, t->tm_sec);
    AddString(szTime, 8, ALIGN_LEFT);
    AddString(STR_NEWLINE);
}

////////////////////////////////////////////////////
//��������  ���ݱ���β��ʽ��װ����β����ʽΪ
//                  <��������ʶ ---><4SP><END>
//������2001-10-13
//////////////////////////////////////////////////
void CMMLReport::MakeReportTail()
{
    AddString(MML_END);
    //by ldf 2002.1.8
    AddString(STR_NEWLINE);
    //change over
}
////////////////////////////////////////////////////
//��������  �������һ�б��Ĳóɶ�������:
//    ��λ��Ϣ =  ��Ϊ��Ϣ1 = ��λ��Ϣ1��ֵ  ��λ��Ϣ2 = ��λ��Ϣ2
//             =  ֵ��λ��Ϣ  ��λ��Ϣ3 = ��λ��Ϣ3��ֵ
//�������
//      szInput     -����ı���
//      LEFT_MARGIN -������µĿո���
//      ROW_LENGTH  -ÿһ�еĳ���
//�������
//      sOutPut     -����ı���
//������2001-10-13
////////////////////////////////////////////////////////
void CMMLReport::MultiLine(const char* szInput,
                           STRING &sOutput,
                           int LEFT_MARGIN,
                           int ROW_LENGTH,
                           BYTE bNormal)
{
    int nLen = strlen(szInput);
    unsigned char szTmp[1024];
    unsigned char* pszOutput = szTmp;
    int nLineLen;
    int nIndex = 0;
    int nCharNum;
    unsigned char* pszLast;
    unsigned char* pszInput = (unsigned char *)szInput;
    nLineLen = ROW_LENGTH;
    while ((nLen + nIndex) > ROW_LENGTH)
    {
        pszLast = (unsigned char *)(pszInput + nLineLen);
        nCharNum = 0;
        if ((*pszLast) > 0xA0)   //���һ���ַ��Ǻ���
        {
            do
            {
                pszLast--;
                if (*pszLast > 0xA0)
                    nCharNum++;
                else
                    break;
            } while (pszLast > pszInput);

            if ((nCharNum % 2) != 0)
                nLineLen --;
        }
		//����ǿո�����˵���һ���ǿո�Ȼ���ٻ���
		else if(*pszLast == ' ')
		{
			do
			{
				if (*pszLast == ' ')
				{
                    nCharNum++;
				}
                else
				{
                    break;
				}
				pszLast--;
			} while(pszLast > pszInput);
			nLineLen -= nCharNum;
		}
        else if (isalpha(*pszLast) || isdigit(*pszLast))
        {
            do
            {
                pszLast--;
                if (isalpha(*pszLast) || isdigit(*pszLast))
                    nCharNum++;
                else
                    break;
            } while (pszLast > pszInput);
            nLineLen -= nCharNum;
        }
        if (nLineLen == 0)
            nLineLen = ROW_LENGTH - LEFT_MARGIN;
        memcpy(pszOutput, pszInput, nLineLen);
        pszOutput += nLineLen;
        pszInput  += nLineLen;
        nLen      -= nLineLen;
        while (*pszInput == ' ')
        {
            pszInput++;
            nLen--;
        }

        if (nLen == 0)
            break;
        pszOutput += sprintf((char *)pszOutput, STR_NEWLINE);
        memset(pszOutput, ' ', LEFT_MARGIN - strlen(MML_FILL1));
        pszOutput += LEFT_MARGIN - strlen(MML_FILL1);
        //������ 2002-05-25 ���ⵥD13958
        nIndex = LEFT_MARGIN;
        if(bNormal == 0)
        {
            memcpy(pszOutput, MML_FILL1, strlen(MML_FILL1));
            pszOutput += strlen(MML_FILL1);
        }
        else
        {
            memcpy(pszOutput,MML_FILL2,strlen(MML_FILL2));
            pszOutput += strlen(MML_FILL2);
        }
        //����
        nLineLen = ROW_LENGTH - LEFT_MARGIN;
    }
    memcpy(pszOutput, pszInput, nLen);
    *(pszOutput + nLen) = '\0';
    sOutput = (char *)szTmp;
}
///////////////////////////////////////////////////////////////////////////////////
//��������  ������������ÿ����ĳ���
//����
//szTitleList         -Ҫ��ʾ�ı����������
//szRecordContentList -Ҫ��ʾ�����б�����ݵ�����,������¼������˳����
//pLenArray           -���صĳ�������
//������ 2002��07��30
///////////////////////////////////////////////////////////////////////////////////
int CMMLReport::CalcHoriTableFieldLens(LIST<STRING>& szTitleList,
                                       LIST<STRING>& szRecContentList,
                                       int *pLenArray)
{


    int nLineLen = 0;
    //�������ֿ�����,�ж���ʾ��ʽ
    int nTitleCnt = szTitleList.size();
    LIST<STRING>::iterator ContentIter,TitleIter;

    //�������������ʽ:
    //          ���к���      ���к���       ͨ����ʼʱ��      ͨ��ʱ��
    //          1111              2222               20010101               30
    //          3333              4444               20010101               30
    //          5555              6666               20010101               30
    //          ����֯����
    //�����㹻�ĳ������ڴ洢ÿ����ĳ���,���ñ����ַ����������ַ����ĳ���,��ʼ��

    STRING temp;
    TitleIter = szTitleList.begin();
    int i;
    for(i=0 ; i < nTitleCnt; i++,TitleIter++)
    {
        pLenArray[i] = (*TitleIter).size()+2;
    }

    //������¼�����ַ�������,�����Ա�������ĳ���Ϊ�鳤,ÿ����һ��,����һ��,
    //�򳤶�����
    int j;
    ContentIter = szRecContentList.begin();
    while(ContentIter != szRecContentList.end())
    {
        for(j=0 ;j<nTitleCnt && ContentIter != szRecContentList.end();j++)
        {
            temp = *ContentIter;
            if(pLenArray[j]<temp.size() + 2)
            {
                pLenArray[j] = temp.size()+2;//�����֮���������ո�
            }
            ContentIter++;
        }
    }
    for(i=0 ; i < nTitleCnt; i++)
    {
        nLineLen += pLenArray[i] + 2;
    }
    nLineLen += 6;
    return nLineLen;
}
///////////////////////////////////////////////////////////////////////////////////
//��������  ������������ÿ����ĳ���
//����
//szTitleList         -Ҫ��ʾ�ı����������
//szRecordContentList -Ҫ��ʾ�����б�����ݵ�����,������¼������˳����
//pLenArray           -���صĳ�������
//������ 2002��07��30
///////////////////////////////////////////////////////////////////////////////////
int  CMMLReport::CalcVertTableRecordLen(LIST<STRING> &szTitleList,
                                        LIST<STRING> &szRecContentList,
                                        LIST<STRING> &szRecTitle)
{
    int nRecordLen = 0;
    int nFieldCount = szTitleList.size(); //һ����¼�е������
    //������ 2002-07-30 D15553
    for(LIST<STRING>::iterator it = szRecTitle.begin();it != szRecTitle.end();it++)
    {
        if(nRecordLen < it->length())
        {
            nRecordLen = it->length();
        }
    }
    nRecordLen += (nFieldCount * (50 + 24));
    nRecordLen += (3 * strlen(STR_NEWLINE) + strlen(STR_TITLE_SPIN));
    return nRecordLen;
}
///////////////////////////////////////////////////////////////////////////////////
//��������  �����������µĺ����ģ�
//���к���      ���к���       ͨ����ʼʱ��      ͨ��ʱ��
//1111          2222           20010101          30
//3333          4444           20010101          30
//5555          6666           20010101          30
//
//�������  szTitleList         -Ҫ��ʾ�ı����������
//          szRecordContentList -Ҫ��ʾ�����б�����ݵ�����,������¼������˳����
//          style               -��ʾ��ʾ��ʽ,1-���� ,0-����,default = 0
//          pLenArray           -��������
//          bLineWrap           -��ʾ�ڳ��ȹ���ʱ,�Ƿ���,0 - NO,1- YES,default = 0
//          LineLen             -��ʾ�೤ʱ����,default = 80
//������2001-10-13
////////////////////////////////////////////////////////////////////////////////
void CMMLReport::AddHoriTable(LIST<STRING>& szTitleList,
                              LIST<STRING> &szRecContentList,
                              const char* szReportTitle,
                              int *pLenArray,
                              int bLineWrap,
                              int LineLen)
{
    //һ�б����ⲻ�ܴ���255
    if (LineLen > 250)
    {
        LineLen = 250;
    }
    int *pArray = NULL;
    //�������ֿ�����,�ж���ʾ��ʽ
    int TitleCnt = szTitleList.size();
    LIST<STRING>::iterator ContentIter,TitleIter;

    //����Ǻ���Ļ���������ʽ:
    //          ���к���      ���к���       ͨ����ʼʱ��      ͨ��ʱ��
    //          1111          2222           20010101          30
    //          3333          4444           20010101          30
    //          5555          6666           20010101          30
    //          ����֯����
    //�����㹻�ĳ������ڴ洢ÿ����ĳ���,���ñ����ַ����������ַ����ĳ���,��ʼ��
    //������D15553 2002-07-30
    if(pLenArray == NULL)
    {
        pArray = new int[TitleCnt];
        CalcHoriTableFieldLens(szTitleList,szRecContentList,pArray);
    }
    else
    {
        pArray = pLenArray;
    }
    STRING temp;
    int i;
    //�޸Ľ���
    AddString(szReportTitle);
    AddString(STR_NEWLINE);
    for(i = 0; i < strlen(szReportTitle); i++)
    {
        AddString("-");
    }
    //AddString(STR_TITLE_SPIN);
    AddString(STR_NEWLINE);
    //ȷ��ÿ����ĳ��Ⱥ�,��ʼ�γɱ���ͷ��,�������±���
    //���к���      ���к���       ͨ����ʼʱ��
    //ͨ��ʱ��
    int row_len = 0;//����һ���е��ۼƳ���ֵ,���ڻ���
    TitleIter = szTitleList.begin();
    for(i=0;TitleIter!=szTitleList.end(); i++,TitleIter++)
    {
        temp = *TitleIter;
        if((bLineWrap == 1)
           && (row_len + pArray[i] > LineLen))
        {
            AddString(STR_NEWLINE);
            row_len = 0;
        }
        if(row_len == 0)
        {
            FillSpace(4);
            row_len += 4;
        }
        AddString(temp.c_str());
        //�޸� by ldf 2002.1.8 (asked by mms)
        FillSpace(2);
        //FillSpace(pArray[i]-temp.size());
        //�޸����
        row_len = row_len + pArray[i];
    }
    AddString(STR_NEWLINE);
    AddString(STR_NEWLINE);
    //�γɱ�����Ľṹ
    //          1111              2222               20010101
    //          30
    //          3333              4444               20010101
    //          30
    //          5555              6666               20010101
    //          30

    row_len = 0;
    ContentIter = szRecContentList.begin();
    while(ContentIter!=szRecContentList.end())
    {
        row_len = 0;
        for(i=0; i<TitleCnt && ContentIter!=szRecContentList.end(); i++)
        {
            //H �γ�һ�м�¼  1111              2222               20010101
            //                30
            temp = *ContentIter;
            if(row_len == 0)
            {
                FillSpace(4);
                row_len += 4;
            }
            AddString(temp.c_str());
            ContentIter++;
            if(i < TitleCnt - 1)
            {
                row_len += pArray[i];
                if((bLineWrap == 1)
                   && (row_len + pArray[i + 1] > LineLen))
                {
                    AddString(STR_NEWLINE);
                    row_len = 0;
                }
                else
                {
                    FillSpace(pArray[i] - temp.size());
                }
            }
        }

        AddString(STR_NEWLINE);
    }
    if(pLenArray == NULL)
    {
        delete[] pArray;
    }
}
/////////////////////////////////////////////////////////////////////////
//��������:
//������:
//RecordTitle1
//-----------------------
//���к���  =  1111
//���к���  =  2222
//ͨ����ʼʱ��  =  20010101
//ͨ��ʱ��  =  30
//RecordTitle2
//-----------------------
//���к���  =  1111
//���к���  =  2222
//ͨ����ʼʱ��  =  20010101
//ͨ��ʱ��  =  30
/////////////////////////////////////////////////////////////////////////
void CMMLReport::AddVertTable(LIST<STRING>& szTitleList,
                              LIST<STRING>& szRecContentList,
                              LIST<STRING>& szRecTitle,
                              int bLineWrap,
                              int LineLen)
{
        //���������Ļ�,��
        //              <���к���><2SP><=><2SP><1111>
        //              <���к���><2SP><=><2SP><2222>
        //              <ͨ����ʼʱ��><2SP><=><2SP><20010101>
        //              <ͨ��ʱ��><2SP><=><2SP><30>
        //              ����֯����
        //ע������ļ�¼�ı��ĳ�����д���ģ��������
        LIST<STRING>::iterator ContentIter,TitleIter,RecTitleIter;
        ContentIter = szRecContentList.begin();
        RecTitleIter = szRecTitle.begin();

        while(ContentIter != szRecContentList.end())
        {
            TitleIter = szTitleList.begin();

            AddString((*RecTitleIter).c_str());
            AddString(STR_NEWLINE);
            AddString(STR_TITLE_SPIN);
            AddString(STR_NEWLINE);
            while(TitleIter != szTitleList.end() && ContentIter != szRecContentList.end())
            {
                MakeReportItem((*TitleIter).c_str(),
                               (*ContentIter).c_str(),
                               24,
                               ALIGN_LEFT,
                               (*TitleIter).size() + 1,
                               ALIGN_LEFT);
                TitleIter ++;
                ContentIter++;
            }
            RecTitleIter++;
            AddString(STR_NEWLINE);
        }
}
/////////////////////////////////////////////////////////////////////////
//��������  ������ɵı���
//�������  ��
//�������  ��
//����ֵ    ���ر��Ļ�����m_pszBuf��ָ��
//������2001-10-13
const char* CMMLReport::GetReport()
{
    //���ر���
    m_pszBuf[m_nReportLen]='\0';
    return m_pszBuf;
}

void CMMLReport::Clear(const char* szName)
{
    //��ʼ����������Ա
    m_nReportLen = 0;
    m_nHeadPosition = 0;

    //��<��ʼ����ʶ +++><4SP><Դ��ʶ��'L32><8SP>��ʽ��ʼ�����ֱ���ͷ
    AddString(MML_HEAD);
    FillSpace(4);
    AddString(szName, 32, ALIGN_LEFT);
    FillSpace(8);

    //���ݵ�ǰ������,���¼���m_nHeadPostion
    m_nHeadPosition = m_nReportLen;
}

void CMMLReport::MakeCmdDisplay(const char* szCmd,
                                UINT4 uCsn,
                                int ret,
                                const char* szhead)
{
    //����  head #������ˮ��
    AddString(szhead);
    FillSpace(4);
    AddString("#");
    char buffer[16];
    sprintf(buffer, "%d", uCsn);
    AddString(buffer);
    AddString(STR_NEWLINE);

    //����  %%��������%%
    AddString("%%");
    //FillSpace(2);
    AddString(szCmd);
    //by ldf 2002.1.9
    AddString(";");
    //change over
    //FillSpace(2);
    AddString("%%");
    AddString(STR_NEWLINE);

    //����  RETCODE = ret   ����������
    //by ldf 2002.8.13 �޸�
    AddString("RETCODE = ");
    //�޸����

    //ldf �޸� 2002.1.8
    sprintf(buffer, "%d    ", ret);  //add ret & 4 space
    AddString(buffer);
    if(ret == 0)
    {
        AddString(STR_OPRSUCCESS);
    }
    else
    {
        AddString(STR_OPRFAIL);
    }
    //�޸����

    AddString(STR_NEWLINE);
}



int CMMLReport::GetReportLen()
{
    return m_nReportLen;
}
