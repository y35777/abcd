/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/
#include "mml_report.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//功能描述  构造函数完成成员变量的初始化.
//输入参数  szName用于指示MML命令中的源标识符
//张玉新 2001-10-13
/////////////////////////////////////////////////////////////////////
CMMLReport::CMMLReport(const char* szName)
{
   //初始化各变量成员;
    m_nReportLen    = 0;
    m_nHeadPosition = 0;
    m_nBufLen       = 64*1024;
    m_pszBuf        = new char[m_nBufLen];

    //以<起始符标识 +++><4SP><源标识符'L32><8SP>格式初始化部分报文头;
    AddString(MML_HEAD);
    FillSpace(4);
    AddString(szName, 32, ALIGN_LEFT);
    FillSpace(8);

    //根据当前填充情况,重新计算m_nHeadPostion;
    m_nHeadPosition = m_nReportLen;
}

CMMLReport::~CMMLReport()
{
    if (m_pszBuf != NULL)
    delete[] m_pszBuf;
}
///////////////////////////////////////////////////////////////////////////
//功能描述  将pString中数据拼接到MML报文中.
//输入参数
//pString  -加入报文中的数据;
//nWidth   -该报文数据应占的宽度,当nWidth长度大于pString时,
//          用空格填充剩余部分缓冲区;
//alignment-填写pString报文数据时的对齐方式,
//          取值包括: ALIGN_LEFT,ALIGN_RIGHT,ALIGN_NONE.
//张玉新2001-10-13
///////////////////////////////////////////////////////////////////////////
void CMMLReport::AddString(const char* pString,
                           int nWidth,
                           int alignment)
{
   //如果pString字符串长度大于等于nWidth,拷贝pString的前
    //nWidth个字符到报文中;
    if(pString == NULL)
    {
        return;
    }
    //张玉新 2002-07-30 D15553
    if(m_nReportLen + nWidth >= m_nBufLen)
    {
        return;
    }//结束
    int nstrlen = strlen(pString);
    if(nstrlen >= nWidth)
    {
        memcpy(m_pszBuf + m_nReportLen, pString, nWidth);
        m_nReportLen += nWidth;
    }
    //否则根据对齐方式要求拷贝pString中数据到报文中;
    else if(alignment == ALIGN_LEFT)//左对齐方式
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
//功能描述  将pString中数据拼接到MML报文中.
//输入参数  pString-加入报文中的数据;
//张玉新    2001-10-13
//////////////////////////////////////////////
void CMMLReport::AddString(const char* pString)
{
    //拼接pString到MML报文中;
    if(pString == NULL)
    {
        return;
    }
    int nstrlen = strlen(pString);
    //张玉新 2002-07-30 问题单D15553
    if(m_nReportLen + nstrlen >= m_nBufLen)
    {
        return;
    }//修改结束
    memcpy(m_pszBuf + m_nReportLen, pString, nstrlen);
    m_nReportLen +=  nstrlen;

}
///////////////////////////////////////////////
//功能描述  从当前位置填充nSp个空格到MML报文中.
//输入参数  nSp -要加入的空格数
//张玉新2001-10-13
//////////////////////////////////////////////
void CMMLReport::FillSpace(int nSp)
{
    //张玉新 2002-07-30 D15553
    if(m_nReportLen + nSp >= m_nBufLen)
    {
        return;
    }//结束
    //从m_nReportLen开始填充空格
    for(int i = 0; i < nSp; i++)
    {
        m_pszBuf[m_nReportLen++] = CHAR_SPACE;
    }
}
/////////////////////////////////////////////////////////////////////////
//功能描述  根据报告体标题和内容组装类似
//          <告警名称'L16A1><2SP><=><2SP><告警名称'L50A0>\r\n的报告体项
//输入参数  STRING szTitle  -等号左边的字符
//      STRING szContent    -等号右边的字符
//      int nFirstFormatLen -第一个参数的字符宽度
//      int nFirstAign      -第一个参数的对齐方式
//      int nSecondFormatLen-第二个参数的字符宽度
//      int nSecondAlign    -第二个参数的对齐方式
//张玉新2001-10-13
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

    //如果长度太长,则要多行显示
    if(nContentLen >= nSecondFormatLen || nSecondFormatLen > 50)
    {
        char tempContent[1024];
        char tempTitle[512];
        BYTE nSecondSpace = 0;

        //将整个显示给客户的信息组成一个整体,然后组成类似如下的报文:
        //      定位信息 ＝  定为信息1 = 定位信息1的值  定位信息2 = 定位信息2
        //                   值定位信息  定位信息3 = 定位信息3的值
        if(nTitleLen >= nFirstFormatLen)
        {
            memcpy(tempTitle, szTitle, nFirstFormatLen);
            tempTitle[nFirstFormatLen] = '\0';
        }
        else if(nFirstAign == ALIGN_LEFT)//左对齐方式
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
    //长度足够的话,简单拼接
    else
    {
    //拼接szTitle中数据
        AddString(szTitle,  nFirstFormatLen,nFirstAign);
    //添加<2SP><=><2SP>报文数据中
        AddString(MML_FILL1);
    //拼接szContent中数据,可能多行,续行后第一个等号右边左对齐
        AddString(szContent,nSecondFormatLen,nSecondAlign);
    }

    //添加回车换行
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
//功能描述  根据报告头格式组装报告头,格式为
//          <起始符标识 +++><4SP><源标识符'L32><8SP><报告输出日期'L10><1SP><报告输出时间'L8>\r\n
//张玉新2001-10-13
////////////////////////////////////////////////////////
void CMMLReport::MakeReportHead()
{
    //置m_nReportLen = m_nHeadPosition,表示开始从m_nHeadPosition重新
    //拼接报文数据,并将从m_nHeadPosition开始的数据清0
    m_nReportLen = m_nHeadPosition;
    memset(m_pszBuf + m_nHeadPosition,0,m_nBufLen - m_nHeadPosition);
    //通过GetStrDate获得当前日期字符串,并添加到报文中
    time_t  tt;
    struct tm* t;
    time(&tt);
    t = localtime(&tt);
    char szTime[20];
    sprintf(szTime, "%04d-%02d-%02d",t->tm_year+1900, t->tm_mon+1, t->tm_mday);
    AddString(szTime, 10, ALIGN_LEFT);
    FillSpace(1);

    //通过GetStrTime获得当前时间字符串,并添加到报文中
    sprintf(szTime, "%02d:%02d:%02d",t->tm_hour, t->tm_min, t->tm_sec);
    AddString(szTime, 8, ALIGN_LEFT);
    AddString(STR_NEWLINE);
}

////////////////////////////////////////////////////
//功能描述  根据报告尾格式组装报告尾，格式为
//                  <结束符标识 ---><4SP><END>
//张玉新2001-10-13
//////////////////////////////////////////////////
void CMMLReport::MakeReportTail()
{
    AddString(MML_END);
    //by ldf 2002.1.8
    AddString(STR_NEWLINE);
    //change over
}
////////////////////////////////////////////////////
//功能描述  将输入的一行报文裁成多行例如:
//    定位信息 =  定为信息1 = 定位信息1的值  定位信息2 = 定位信息2
//             =  值定位信息  定位信息3 = 定位信息3的值
//输入参数
//      szInput     -输入的报文
//      LEFT_MARGIN -左边留下的空格数
//      ROW_LENGTH  -每一行的长度
//输出参数
//      sOutPut     -输出的报文
//张玉新2001-10-13
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
        if ((*pszLast) > 0xA0)   //最后一个字符是汉字
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
		//如果是空格，则回退到上一个非空格然后再换行
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
        //张玉新 2002-05-25 问题单D13958
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
        //结束
        nLineLen = ROW_LENGTH - LEFT_MARGIN;
    }
    memcpy(pszOutput, pszInput, nLen);
    *(pszOutput + nLen) = '\0';
    sOutput = (char *)szTmp;
}
///////////////////////////////////////////////////////////////////////////////////
//功能描述  计算纵向报文中每个域的长度
//参数
//szTitleList         -要显示的表格标题的链表
//szRecordContentList -要显示的所有表格内容的链表,多条记录的内容顺序存放
//pLenArray           -返回的长度数组
//张玉新 2002－07－30
///////////////////////////////////////////////////////////////////////////////////
int CMMLReport::CalcHoriTableFieldLens(LIST<STRING>& szTitleList,
                                       LIST<STRING>& szRecContentList,
                                       int *pLenArray)
{


    int nLineLen = 0;
    //纵向横向分开处理,判断显示方式
    int nTitleCnt = szTitleList.size();
    LIST<STRING>::iterator ContentIter,TitleIter;

    //横向的以如下形式:
    //          主叫号码      被叫号码       通话起始时间      通话时长
    //          1111              2222               20010101               30
    //          3333              4444               20010101               30
    //          5555              6666               20010101               30
    //          来组织报文
    //分配足够的长度用于存储每个域的长度,并用标题字符串链表中字符串的长度,初始化

    STRING temp;
    TitleIter = szTitleList.begin();
    int i;
    for(i=0 ; i < nTitleCnt; i++,TitleIter++)
    {
        pLenArray[i] = (*TitleIter).size()+2;
    }

    //遍历记录内容字符串链表,并且以标题链表的长度为组长,每遍历一组,更新一次,
    //域长度数组
    int j;
    ContentIter = szRecContentList.begin();
    while(ContentIter != szRecContentList.end())
    {
        for(j=0 ;j<nTitleCnt && ContentIter != szRecContentList.end();j++)
        {
            temp = *ContentIter;
            if(pLenArray[j]<temp.size() + 2)
            {
                pLenArray[j] = temp.size()+2;//域和域之间有两个空格
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
//功能描述  计算纵向报文中每个域的长度
//参数
//szTitleList         -要显示的表格标题的链表
//szRecordContentList -要显示的所有表格内容的链表,多条记录的内容顺序存放
//pLenArray           -返回的长度数组
//张玉新 2002－07－30
///////////////////////////////////////////////////////////////////////////////////
int  CMMLReport::CalcVertTableRecordLen(LIST<STRING> &szTitleList,
                                        LIST<STRING> &szRecContentList,
                                        LIST<STRING> &szRecTitle)
{
    int nRecordLen = 0;
    int nFieldCount = szTitleList.size(); //一条记录中的域个数
    //张玉新 2002-07-30 D15553
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
//功能描述  生成类似如下的横向报文：
//主叫号码      被叫号码       通话起始时间      通话时长
//1111          2222           20010101          30
//3333          4444           20010101          30
//5555          6666           20010101          30
//
//输入参数  szTitleList         -要显示的表格标题的链表
//          szRecordContentList -要显示的所有表格内容的链表,多条记录的内容顺序存放
//          style               -表示显示方式,1-横向 ,0-纵向,default = 0
//          pLenArray           -长度数组
//          bLineWrap           -表示在长度过长时,是否换行,0 - NO,1- YES,default = 0
//          LineLen             -表示多长时换行,default = 80
//张玉新2001-10-13
////////////////////////////////////////////////////////////////////////////////
void CMMLReport::AddHoriTable(LIST<STRING>& szTitleList,
                              LIST<STRING> &szRecContentList,
                              const char* szReportTitle,
                              int *pLenArray,
                              int bLineWrap,
                              int LineLen)
{
    //一行表格标题不能大于255
    if (LineLen > 250)
    {
        LineLen = 250;
    }
    int *pArray = NULL;
    //纵向横向分开处理,判断显示方式
    int TitleCnt = szTitleList.size();
    LIST<STRING>::iterator ContentIter,TitleIter;

    //如果是横向的话以如下形式:
    //          主叫号码      被叫号码       通话起始时间      通话时长
    //          1111          2222           20010101          30
    //          3333          4444           20010101          30
    //          5555          6666           20010101          30
    //          来组织报文
    //分配足够的长度用于存储每个域的长度,并用标题字符串链表中字符串的长度,初始化
    //张玉新D15553 2002-07-30
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
    //修改结束
    AddString(szReportTitle);
    AddString(STR_NEWLINE);
    for(i = 0; i < strlen(szReportTitle); i++)
    {
        AddString("-");
    }
    //AddString(STR_TITLE_SPIN);
    AddString(STR_NEWLINE);
    //确定每个域的长度后,开始形成报文头部,类似如下报文
    //主叫号码      被叫号码       通话起始时间
    //通话时长
    int row_len = 0;//定义一个行的累计长度值,用于换行
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
        //修改 by ldf 2002.1.8 (asked by mms)
        FillSpace(2);
        //FillSpace(pArray[i]-temp.size());
        //修改完毕
        row_len = row_len + pArray[i];
    }
    AddString(STR_NEWLINE);
    AddString(STR_NEWLINE);
    //形成报文体的结构
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
            //H 形成一行记录  1111              2222               20010101
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
//功能描述:
//纵向报文:
//RecordTitle1
//-----------------------
//主叫号码  =  1111
//被叫号码  =  2222
//通话起始时间  =  20010101
//通话时长  =  30
//RecordTitle2
//-----------------------
//主叫号码  =  1111
//被叫号码  =  2222
//通话起始时间  =  20010101
//通话时长  =  30
/////////////////////////////////////////////////////////////////////////
void CMMLReport::AddVertTable(LIST<STRING>& szTitleList,
                              LIST<STRING>& szRecContentList,
                              LIST<STRING>& szRecTitle,
                              int bLineWrap,
                              int LineLen)
{
        //如果是纵向的话,以
        //              <主叫号码><2SP><=><2SP><1111>
        //              <被叫号码><2SP><=><2SP><2222>
        //              <通话起始时间><2SP><=><2SP><20010101>
        //              <通话时长><2SP><=><2SP><30>
        //              来组织报文
        //注意这里的记录的报文长度是写死的，所以如果
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
//功能描述  获得生成的报文
//输入参数  无
//输出参数  无
//返回值    返回报文缓冲区m_pszBuf的指针
//张玉新2001-10-13
const char* CMMLReport::GetReport()
{
    //返回报文
    m_pszBuf[m_nReportLen]='\0';
    return m_pszBuf;
}

void CMMLReport::Clear(const char* szName)
{
    //初始化各变量成员
    m_nReportLen = 0;
    m_nHeadPosition = 0;

    //以<起始符标识 +++><4SP><源标识符'L32><8SP>格式初始化部分报文头
    AddString(MML_HEAD);
    FillSpace(4);
    AddString(szName, 32, ALIGN_LEFT);
    FillSpace(8);

    //根据当前填充情况,重新计算m_nHeadPostion
    m_nHeadPosition = m_nReportLen;
}

void CMMLReport::MakeCmdDisplay(const char* szCmd,
                                UINT4 uCsn,
                                int ret,
                                const char* szhead)
{
    //构造  head #报文流水号
    AddString(szhead);
    FillSpace(4);
    AddString("#");
    char buffer[16];
    sprintf(buffer, "%d", uCsn);
    AddString(buffer);
    AddString(STR_NEWLINE);

    //构造  %%回显命令%%
    AddString("%%");
    //FillSpace(2);
    AddString(szCmd);
    //by ldf 2002.1.9
    AddString(";");
    //change over
    //FillSpace(2);
    AddString("%%");
    AddString(STR_NEWLINE);

    //构造  RETCODE = ret   命令操作结果
    //by ldf 2002.8.13 修改
    AddString("RETCODE = ");
    //修改完毕

    //ldf 修改 2002.1.8
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
    //修改完毕

    AddString(STR_NEWLINE);
}



int CMMLReport::GetReportLen()
{
    return m_nReportLen;
}
