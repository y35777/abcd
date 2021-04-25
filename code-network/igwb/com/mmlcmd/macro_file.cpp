#include "macro_file.h"
#include "../include/toolbox.h"

/*****************************************************************************
 *功能描述：类CMacroFile的构造函数
 *输入：
 *		pszFileName：Macro.ini文件的全路径名
 *		szTblDir：转换后的MML命令表的存放目录
 *输出：
 *		无
 *返回：
 *		无
 ******************************************************************************/
CMacroFile::CMacroFile(char *pszFileName):CINIFile(pszFileName)
{
    //成员变量初始化
    m_fpCmd         = NULL;		//命令定义表
    m_fpPara	    = NULL;		//参数定义表
    m_fpParaRange   = NULL;		//参数范围表
    m_fpChildPara   = NULL;		//子参数表
    m_fpParaEnum    = NULL;		//参数枚举表
}

/***************************************************************************
 *功能描述：类CMacroFile的析构函数
 *输入：
 *		首先关闭MACRO.INI文件指针，然后关闭MML命令定义表文件指针(关闭五张表)
 *输出：
 *		无
 *返回：
 *		无
 ***************************************************************************/
CMacroFile::~CMacroFile()
{
    //关闭打开的MACRO.INI文件
    Close();
    
    //关闭打开的MML命令表文件
    CloseAllTbl();
}

/***************************************************************************
 *功能描述：关闭所有MML命令表格文件指针
 *输入：
 *		无
 *输出：
 *		无
 *返回：
 *		无
 **************************************************************************/
void CMacroFile::CloseAllTbl()
{
    //关闭命令定义表
    if (m_fpCmd)	
    {
        fclose(m_fpCmd);
        m_fpCmd = NULL;
    }

    //关闭参数定义表
    if (m_fpPara)
 	{
        fclose(m_fpPara);
        m_fpPara = NULL;
    }

    //关闭参数区间定义表
    if (m_fpParaRange)	
    {
        fclose(m_fpParaRange);
        m_fpParaRange = NULL;
    }

    //关闭子参数表
    if (m_fpChildPara)
    {
        fclose(m_fpChildPara);
        m_fpChildPara = NULL;
    }

    //关闭参数枚举信息表
    if (m_fpParaEnum)
    {
        fclose(m_fpParaEnum);	
        m_fpParaEnum  = NULL;
    }
}

/*******************************************************************************
 *功能描述：将Macro.ini文件转换成MML命令五张表格：
 *     	1、命令定义表tbl_CmdDef
 *	    2、参数定义表tbl_ParaDef
 *	    3、子参数定义表tbl_ChildParaDef
 *	    4、参数枚举定义表tbl_ParaEnumDef
 *	    5、参数取值范围定义表tbl_ParaRangeDef
 *输入：
 *		nMaxCmdCode : 最大MML命令码
 *输出：
 *		无
 *返回：
 *		转换成功返回TRUE，否则返回FALSE。
 ****************************************************************************/
BOOL CMacroFile::ConvertAllCmdToTbl(const int nMaxCmdCode)
{
    char szCmdFile[MAX_PATH];
    char szParaFile[MAX_PATH];
    char szChildFile[MAX_PATH];
    char szEnumFile[MAX_PATH];
    char szRangeFile[MAX_PATH];
    char szCmdName[MAX_CMD_NAME_LEN];

    SCmdDef	 CmdDef;

    if (nMaxCmdCode < 0)	return FALSE;

    if (!IsOpen())		return FALSE;		//MACRO.INI文件尚未打开

    //首先以二进制打开五个表文件，为了支持跨平台，注意文件名全部采用小写
    sprintf(szCmdFile, "%s",  "cmddef.dat");
    sprintf(szParaFile, "%s", "paradef.dat");
    sprintf(szChildFile, "%s", "childparadef.dat");
    sprintf(szEnumFile, "%s", "paraenumdef.dat");
    sprintf(szRangeFile, "%s", "pararangedef.dat");

    m_fpCmd       = fopen(szCmdFile, "wb");	    //用二进制方式打开文件
    m_fpPara      = fopen(szParaFile, "wb");	//用二进制方式打开文件
    m_fpChildPara = fopen(szChildFile, "wb");	//用二进制方式打开文件
    m_fpParaEnum  = fopen(szEnumFile, "wb");	//用二进制方式打开文件
    m_fpParaRange = fopen(szRangeFile, "wb");	//用二进制方式打开文件

    if ((!m_fpCmd) || (!m_fpPara) || (!m_fpChildPara) 
     || (!m_fpParaEnum) || (!m_fpParaRange))
    {
	    CloseAllTbl();
        printf("Cannot Open Table File!\n");

	    return FALSE;		//无法打开表文件
    }

    //依次转换各个MML命令
    for (UINT2 i = 1; i <= nMaxCmdCode; i ++)
    {
	    char szCmdCode[5] = SZ_EMPTY_STR;
	    sprintf(szCmdCode, "%d", i);
	
	    //根据命令码找到命令名称
	    GetString(SZ_ALL_MACROS, szCmdCode, SZ_EMPTY_STR, szCmdName, MAX_CMD_NAME_LEN);

	    if (strlen(szCmdName) == 0)	
        {
            //此命令码对应的命令名不存在，继续下一个
            //MACRO.INI文件中允许MML命令码不连续的情况
            continue;	
        }

	    printf("Convert MML Command:  %s. ", szCmdName);

	    if (!GetCmdDef(szCmdName, CmdDef))	return FALSE;

	    printf("OK!\n");
	    fwrite((void *)&CmdDef, sizeof(SCmdDef), 1, m_fpCmd);	//输出到命令定义表文件中
		
	    if (!ConvertParaDef(szCmdName, i, CmdDef.uParaNum - 1))
	    {
		    CloseAllTbl();
		    return FALSE;
	    }
        
        //写MML命令的“CONFIRM”参数
        WriteConfirmPara(i, CmdDef.uParaNum);
    }
	
    printf("Convert MML Command: Over!\n");

    return TRUE;	
}

/*****************************************************************************************
 *功能描述：根据MML命令名称读取该命令的命令吗、参数个数等
 *输入：
 *	szCmdName:MML命令名称
 *输出：
 *	CmdDef	 :获得的MML命令定义结构(命令码、参数个数、命令显示名称、命令级别等)
 *返回：
 *	FALSE - 失败；TRUE - 成功
 *****************************************************************************************/
BOOL CMacroFile::GetCmdDef(char *szCmdName, SCmdDef& CmdDef)		
{
    char  szCmdLevel[64];

    //命令名称	
    strcpy(CmdDef.szCmdName, szCmdName);
    StringToUpper(CmdDef.szCmdName);

    //得到命令的命令码、参数个数、节点个数
    int nCmdCode = GetInt(szCmdName, SZ_MACRO_CODE, 0);
    if (nCmdCode == 0)		//MML命令的命令码(macro_code)不允许为0
    {
	    printf("%d  %s\n", __LINE__, __FILE__);

	    return FALSE;
    }

    char szCmdCode[5];
    sprintf(szCmdCode, "%d", nCmdCode);
    
    //得到命令参数个数，如果不配置，则认为参数个数为0
    int nParaNum = GetInt(szCmdName, SZ_PARA_NUM, 0);	//得到命令参数个数(默认为0)

    //int nNodeNum = GetInt(szCmdName, SZ_NODE_NUM, 0);	//节点个数,目前不使用

    GetString(szCmdName, SZ_MACRO_NAME, SZ_EMPTY_STR, 
                     CmdDef.szDspName, MAX_CMD_DSP_LEN);

    char szConfirm[100];
    int nConfirm = 1;
    GetString(SZ_MUST_CONFIRM, szCmdCode, SZ_EMPTY_STR,  szConfirm, 99);
    if (strlen(szConfirm) == 0)		nConfirm = 0;

    CmdDef.uCmdCode = (UINT2)nCmdCode;
    CmdDef.uParaNum = (UINT2)(nParaNum + 1);   //每一MML命令含一个“CONFIRM”参数

    //MML命令级别，缺省为普通用户级别
    GetString(szCmdName, SZ_CMD_LEVEL, "CL_OPER", szCmdLevel, 64);

    if (StringiCmp(szCmdLevel, "CL_ADMIN") == 1)
    {
	    CmdDef.yLevel = CMD_LEVEL_ADMIN;
    }
    else
    {
	    CmdDef.yLevel = CMD_LEVEL_OPER;
    }

    return TRUE;
}

/*****************************************************************************
 * 每一个MML命令包含一个"CONFIRM"参数。
 * 输入参数：
 *          nCmdCode  : MML命令码  
 *  输出参数：
 *          无。
 *  返回值:
 *          无。 
 ******************************************************************************/
void CMacroFile::WriteConfirmPara(const int nCmdCode, const int nParaNum)
{
    char szConfirm[64];
    SParaDef pd;
    sprintf(szConfirm, "%s", SZ_CONFIRM_ASK);

    pd.uCmdCode    = nCmdCode;
    pd.uParaCode   = nParaNum;
    pd.yDataType   = PT_DIGIT;
    pd.yMustGive   = 0;
    pd.yRangeType  = ENUM_ARATE;
    pd.yByteNum    = 0;
    strcpy(pd.szParaName, "CONFIRM"	);
    strcpy(pd.szDspName, szConfirm);
	
    strcpy(pd.szDefault, "0");
    fwrite((void *)&pd, sizeof(pd), 1, m_fpPara);

    SParaEnumDef ped;
    ped.uCmdCode       = nCmdCode;
    ped.uParaCode      = nParaNum;
    ped.uChildParaCode = 0;
    ped.uValue         = 0;      
  
    strcpy(ped.szInput, "N");
    fwrite((void *)&ped, sizeof(ped), 1, m_fpParaEnum);

    ped.uValue         = 1;
    strcpy(ped.szInput, "Y");
    fwrite((void *)&ped, sizeof(ped), 1, m_fpParaEnum);   
}

/*****************************************************************************
 * 从MACRO.INI中得到参数定义
 * szCmdName: 命令名称
 * nCmdCode : 命令码
 * nParaNum : 参数个数 
 ******************************************************************************/
BOOL CMacroFile::ConvertParaDef(char *szCmdName, UINT2 nCmdCode, UINT2 nParaNum)
{
    char szWord[MAX_MACRO_NAME_LEN];
    char szTmpWord[MAX_MACRO_NAME_LEN];
    char szParaLine[MAX_LINE_LEN];
    char szFieldName[MAX_MACRO_NAME_LEN];
    char szTableName[MAX_MACRO_NAME_LEN];
    char *pszBuf = NULL;
    int nMovLength;
    SParaDef ParaDef;

    int  nParaCode;
    char szParaName[PARA_NAME_LEN] = "";
    char szParaDspName[PARA_NAME_LEN] = "";
    char szDefault[PARA_NAME_LEN] = "";

    BYTE yDataType   = 0;
    BYTE yByteNum    = 0;
    int  iParaMaxLen = 0;	//保存参数的数据最大长度,主要用于字符串型参数
    BYTE yMustGive   = 0;

    //根据一个MML命令参数个数，依次读出参数属性，将其输出到参数定义表中
    for (UINT2 i = 1; i <= nParaNum; i++)  
    {
        //得到MML参数数据类型，MML参数数据类型定义格式如下：
        //“para1_datatype=2”，其中参数数据类型为枚举型－PARA_TYPE
        //如果一个参数的数据类型没有明确指定，则认为是整型。
	    yDataType = GetParaDataType(szCmdName, i);	
        
	    if (yDataType == 0xFF)		
	    {
	        printf("GetDataType() Error: CmdName = %s ParaCode = %d\n", szCmdName, i);

	        return FALSE;
	    }

	    //从命令定义文件中读取这个参数的数据最大长度(目前未使用)
        //MML参数的长度定义格式为：“para3_length=15”，一般对于
        //字符型参数才需要规定其长度，缺省为15个字节
	    iParaMaxLen = GetParaMaxDataLen(szCmdName, i);
	    char szParaNO[8];
	    sprintf(szParaNO, "%d", i);

	    //得到参数i定义的一行字符串，MML参数定义格式如下：
        //1= op (帐号)
 	    GetString(szCmdName, szParaNO, SZ_EMPTY_STR, szParaLine, MAX_LINE_LEN);

	    if (strlen(szParaLine) == 0)	//错误：该参数在MML命令中没有定义
	    {
	        printf("Parameter Not Defined: CmdName = %s ParaCode = %d\n", szCmdName, i);

	        return(FALSE);
	    }

	    char szTemp[MAX_LINE_LEN];
 	    int iSubSeq = 1;
	    strcpy(szTemp, szParaLine);
 	
 	    do	
 	    { 
	        strcat(szTemp, " ");

 	        sprintf(szParaNO, "%d-%d", i, iSubSeq++);
 	        GetString(szCmdName, szParaNO, SZ_EMPTY_STR, szParaLine, MAX_LINE_LEN);
	        if (strlen(szParaLine) == 0)	 break;

	        strcat(szTemp, szParaLine);
  
        }while(TRUE);
  
        StringToLower(szTemp);
	    pszBuf = szTemp;

	    nParaCode = i;		
        //获得一个参数主要属性：参数名、参数显示名、参数缺省值（如果有） 
	    if (GetParaAttribute(pszBuf, nParaCode, szParaName, szParaDspName, szDefault, nMovLength))		
	    {
	        pszBuf += nMovLength;
	        nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
		
	        yMustGive = IsParaMustGive(szCmdName, nParaCode);
            
            //该参数没有其它定义，则将其输出到“参数定义表”中并继续下
            //一个参数的处理
	        if (strlen(szWord) <= 0)
	        {
		        memset((void *)&ParaDef, 0, sizeof(SParaDef));	
		
		        ParaDef.uCmdCode  = nCmdCode;
		        ParaDef.uParaCode = nParaCode;
	            ParaDef.yDataType = yDataType;
		        ParaDef.yMustGive = yMustGive;
		        
                //将字符串统一转换为大写 
	            strcpy(ParaDef.szDefault, szDefault);
		        StringToUpper(ParaDef.szDefault);

	   	        strcpy(ParaDef.szDspName, szParaDspName);
		        StringToUpper(ParaDef.szDspName);

		        strcpy(ParaDef.szParaName, szParaName);
		        StringToUpper(ParaDef.szParaName);

                //将一个参数定义输出到“参数定义表”
		        fwrite((void *)&ParaDef, sizeof(SParaDef), 1, m_fpPara);
		        continue;
	        }
	    }
	    else  //错误：获取参数属性失败
	    {
	        printf("GetParaAttribute() Error: CmdName = %s ParaCode = %d\n", 
                    szCmdName, i);

	        return FALSE;
	    }
		
        //成功转换一个MML命令一个参数定义，继续处理（枚举表、子参数表、参数区间表）
	    strcpy(szTmpWord, szWord);
	    if (GetCommonValue(szWord, szTableName, SZ_TABLE))//判断这个单词是否关系表描述
	    {
	        pszBuf += nMovLength;
	        nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);

	        //如果没有额外的值域约束
	        if (strlen(szWord) <= 0)
	        {
		        memset((void *)&ParaDef, 0, sizeof(SParaDef));	

		        strcpy(ParaDef.szDefault, szDefault);
		        StringToUpper(ParaDef.szDefault);

		        strcpy(ParaDef.szDspName, szParaDspName);
                StringToUpper(ParaDef.szDspName);
                
		        strcpy(ParaDef.szParaName, szParaName);
		        StringToUpper(ParaDef.szParaName);

		        ParaDef.uCmdCode  = nCmdCode;
		        ParaDef.uParaCode = nParaCode;
		        ParaDef.yDataType = yDataType;
	            ParaDef.yMustGive = yMustGive;
	
		        fwrite((void *)&ParaDef, sizeof(SParaDef), 1, m_fpPara);
			    continue;
	        }
	    }
	    else
        {
	        strcpy(szTableName, SZ_EMPTY_STR);
        }
	
	    strcpy(szTmpWord, szWord);
	    if (GetCommonValue(szTmpWord, szFieldName, SZ_FIELD))//判断这个单词是否字段描述
	    {
	        pszBuf += nMovLength;
	        nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
	        //如果没有额外的值域约束
	        if (strlen(szWord) <= 0)
	        {
	            memset((void *)&ParaDef, 0, sizeof(SParaDef));	

	            strcpy(ParaDef.szDefault, szDefault);
	            StringToUpper(ParaDef.szDefault);

	            strcpy(ParaDef.szDspName, szParaDspName);
                StringToUpper(ParaDef.szDspName);

	            strcpy(ParaDef.szParaName, szParaName);
	            StringToUpper(ParaDef.szParaName);
	
	            ParaDef.uCmdCode = nCmdCode;
	            ParaDef.uParaCode = nParaCode;
	            ParaDef.yDataType = yDataType;
	            ParaDef.yMustGive = yMustGive;
	
	            fwrite((void *)&ParaDef, sizeof(SParaDef), 1, m_fpPara);			
	            continue;
	        }
	    }
	    else
        {
	        strcpy(szFieldName, SZ_EMPTY_STR);
        } 

	    //判断这个单词是否位域字段，例如：
        //2 = cg (命令组) fieldarray~1 ByteNumber~3 G_0~0(告警查询) G_1~1(告警修改) G_2~2(话统查询) 
	    if (StringiCmp(szWord, SZ_COMPOUND_STRING) == 1)
	    {
	        pszBuf += nMovLength;
		
	        //得到下一个单词,形式应该是ByteNumber~字节个数
	        nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
	        if (strlen(szWord) <= 0)	
	        {
		        printf("GetNextWord() Error: CmdName = %s ParaCode = %d\n", 
                        szCmdName, i);

		        return (FALSE);
	        }

	        pszBuf = pszBuf + nMovLength;

	        char szEnumValName[MAX_MACRO_NAME_LEN];
	        UINT4 uEnumValue;

	        if (!GetEnumValue(szWord, szEnumValName, uEnumValue))
	        {
		         printf("GetEnumValue() Error: CmdName = %s ParaCode = %d\n", 
                         szCmdName, i);
		
			     return (FALSE);
	         }
		
	         if (StringiCmp(szEnumValName, SZ_BYTENUM) != 1)
	         {
		         printf("StringiCmp Error! CmdName = %s ParaCode = %d\n", 
                         szCmdName, i);

		         return (FALSE);
	         }
			
	         yByteNum = (BYTE)uEnumValue;		//字节数

	         nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
	         if (strlen(szWord) <= 0)
	         {
	             memset((void *)&ParaDef, 0, sizeof(SParaDef));	

		         strcpy(ParaDef.szDefault, szDefault);
		         StringToUpper(ParaDef.szDefault);

		         strcpy(ParaDef.szDspName, szParaDspName);
                 StringToUpper(ParaDef.szDspName);

		         strcpy(ParaDef.szParaName, szParaName);
		         StringToUpper(ParaDef.szParaName);

		         ParaDef.uCmdCode  = nCmdCode;
		         ParaDef.uParaCode = nParaCode;
		         ParaDef.yDataType = yDataType;
		         ParaDef.yMustGive = yMustGive;
	
                 //将参数输出到“参数定义表”中
	             fwrite((void *)&ParaDef, sizeof(SParaDef), 1, m_fpPara);
		         continue;
	        }
	    }
        //复合型参数 
 	    else if (StringiCmp(szWord, SZ_COMPLEX_STRING) == 1)
	    {
	        pszBuf +=  nMovLength;
	        //得到下一个单词,形式应该是childpart~子参数个数
	        nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
	        if (strlen(szWord) <= 0)
	        {
	            printf("GetNextWord() Error: CmdName = %s ParaCode = %d\n", 
                        szCmdName, i);

	            return(FALSE);
	        }

	        pszBuf +=  nMovLength;

	        char szEnumName[MAX_MACRO_NAME_LEN];
	        UINT4 uEnumValue;

            //从"childpart~"中分离出"childpart"字符串
	        if ((!GetEnumValue(szWord, szEnumName, uEnumValue))
              || (StringiCmp(szEnumName, SZ_CHILDPART) != 1))
	        {
		        printf("Get childpart~ Error: CmdName = %s ParaCode = %d\n", 
                        szCmdName, i);

		        return (FALSE);
	        }
		
	        yByteNum = (BYTE)uEnumValue;	//子参数个数

	        //读取该参数的子参数定义信息，输出子参数定义到“子参数表”
	        if (!ConvertChildPara(szCmdName, nCmdCode, i, nParaCode, yByteNum))
	        {
	 	        printf("Convert Child Para Error: CmdName = %s ParaCode = %d\n", 
                        szCmdName, i);

		        return FALSE;
	        }

	        nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
	        if (strlen(szWord) <= 0)
	        {
		        memset((void *)&ParaDef, 0, sizeof(SParaDef));	
	
		        ParaDef.yByteNum = yByteNum;		//子参数个数

		        strcpy(ParaDef.szDefault, szDefault);
		        StringToUpper(ParaDef.szDefault);

		        strcpy(ParaDef.szDspName, szParaDspName);
                StringToUpper(ParaDef.szDspName);
                 
		        strcpy(ParaDef.szParaName, szParaName);
		        StringToUpper(ParaDef.szParaName);

		        ParaDef.uCmdCode  = nCmdCode;
	            ParaDef.uParaCode = nParaCode;
		        ParaDef.yDataType = yDataType;
		        ParaDef.yMustGive = yMustGive;
			
		        fwrite((void *)&ParaDef, sizeof(SParaDef), 1, m_fpPara);
	            continue;
	        }
	    }
	    else
	    {
	        yByteNum = 0;
	    }

	    //下面是参数的值域描述,有两种形式,一是区间形式,一是枚举值形式
	    ParaDef.yRangeType = GetRangeType(pszBuf);

	    if (ParaDef.yRangeType == 0xFF)		
	    {
	        printf("GetRangeType() Error: CmdName = %s ParaCode = %d\n", 
                    szCmdName, i);
	
	        return FALSE;
	    }

	    strcpy(ParaDef.szDefault, szDefault);
	    StringToUpper(ParaDef.szDefault);

	    strcpy(ParaDef.szDspName, szParaDspName);
        StringToUpper(ParaDef.szDspName);

	    strcpy(ParaDef.szParaName, szParaName);
	    StringToUpper(ParaDef.szParaName);

	    ParaDef.uCmdCode  = nCmdCode;
	    ParaDef.uParaCode = nParaCode;
	    ParaDef.yDataType = yDataType;
	    ParaDef.yMustGive = yMustGive;
	    ParaDef.yByteNum  = yByteNum;

	    fwrite((void *)&ParaDef, sizeof(SParaDef), 1, m_fpPara);
			
	    if (ParaDef.yRangeType == ENUM_ZONE)		//区间值
	    {
            //如果是区间类型，则应转换区间表
	        if (!WriteParaRangeTbl(pszBuf, nCmdCode, nParaCode, 0))
	        {
		        printf("WriteParaRangeTbl() Error: CmdName = %s ParaCode = %d\n", 
                        szCmdName, i);

		        return FALSE;
	        }
	    }		
        //如果是枚举类型，则应转换枚举信息表
	    else if (ParaDef.yRangeType == ENUM_ARATE)	//如果是枚举取值形式
	    {
	        if (!WriteParaEnumTbl(pszBuf, nCmdCode, nParaCode, 0))
	        {
	            printf("WriteParaEnumTbl() Error: CmdName = %s ParaCode = %d\n", 
                        szCmdName, i);

	            return FALSE;
	        }
	    }	
    }
   
    return TRUE;
}

/*****************************************************************************************
 *功能描述：根据命令名称、命令参数码得到参数数据类型
 *输入：
 *	szCmdName：MML命令名称
 *      nParaNo  ：参数码
 *输出：
 *	无
 *返回：
 *	参数类型(MML_PARA_TYPE)
 *****************************************************************************************/
BYTE CMacroFile::GetParaDataType(char *szCmdName, UINT2 nParaCode)
{
    char  szDataType[128];
    BYTE  yDataType = 0xFF;
    char  szParaLine[MAX_LINE_LEN];
    char  *pszBuf = NULL;
    UINT2 nMovLength;
    char szWord[MAX_MACRO_NAME_LEN];

    char szParaNo[4];
    sprintf(szParaNo, "%d", nParaCode);

    //从MACRO.INI文件中读该参数定义，如果不存在，则返回0xFF
    GetString(szCmdName, szParaNo, SZ_EMPTY_STR, szParaLine, MAX_LINE_LEN);
    if (strlen(szParaLine) == 0)
    {
        return(0xFF);	//错误：参数不存在
    }

    pszBuf = szParaLine;
    StringToLower(szParaLine);

    //得到一行字符串中的第一个单词，该单词可能是：
    //参数名称：例如：1 = ParaName
    //参数码：  例如：1 = 55
    nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
    if (strlen(szWord) == 0)	
    {
        return(0xFF);
    }

    if (IsDigitValue(szWord))   //如果如"1 = 55 ParaName ....."
    {
        pszBuf += nMovLength;
        nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
        if (strlen(szWord) <= 0)
        {
            return(0xFF);	//错误：只有参数码，没有参数名	
        }
    }

    pszBuf = pszBuf + nMovLength;
    nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);

    int iLength = strlen(szWord);
    if ((iLength > 0) && (szWord[0] == '(') && (szWord[iLength - 1] == ')'))
    {
	    //如果这个单词是参数的显示名称
	    pszBuf += nMovLength;
	    nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
    }

    //如果单词是缺省值
    char *szDefault = szWord;
    char *pDest = NULL;
    pDest = strstr(szDefault, SZ_DEFAULT);

    if (pDest == szDefault)
    {
	    pszBuf += nMovLength;
	    nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
    }

    //如果参数类似"paraname = attr1-0&attr2-1&attr3-1"，那么参数描述为：
    //"1 = paraname fieldarray~1 attr1~1 attr2~2 attr3~3....."
    //这意味这该参数是复合参数，attr1,attr2...是一个字节
    //类似"attr2~2"表示attr2是第二个参数值
    if (StringiCmp(szWord, SZ_COMPOUND_STRING) == 1)
    {
        return PT_BITFIELD;
    }
    else if (StringiCmp(szWord, SZ_COMPLEX_STRING) == 1)
    {
        return PT_COMPLEX;
    }

    //参数数据
    sprintf(szDataType, "para%d_datatype", nParaCode);
    yDataType = (BYTE)GetInt(szCmdName, szDataType, 0xFF);	//获得参数数据类型

    if (yDataType == 0xFF)	
    {
        //如果没有显式定义某个参数的数据类型，则该参数为数字型
        yDataType = PT_DIGIT;	//参数默认数据类型是数字型
    }

    switch (yDataType)
    {
        case PT_DIGIT:
        case PT_STRING:
        case PT_BCD:
        case PT_ANYTYPE:
        {
	    return (yDataType);
        }

	default:
        {
	    return(0xFF);
        }
    }
}

/***************************************************************************
 *功能描述：从一行字符串中得到下一个单词,分隔符是空格或逗号
 * 输入： 
 *       szLine    ：行字符串
 *	 uWordSize ：参数szWord的最大长度
 * 输出：
 *       szWord	   ：取得的单词　　　　　　　　　　　                                       
 * 返回值：
 *       前移的绝对偏移值,包含前置的空格和逗号在内
***************************************************************************/
UINT2 CMacroFile::GetNextWord(char *szWord, char *szLine, UINT2 uWordSize)
{
    UINT2 nLength = 0;
    UINT2 nWordLength = 0;

    int nFlag = 0;
    szWord[0] = '\0';

    if (strlen(szLine) == 0)	return 0;

    while (*szLine)
    {
        //过滤掉前置的空格或逗号
        if (((*szLine == ' ') || (*szLine == ',')) && (nFlag == 0))
        {
            nLength++;
            szLine++;
        }
	else
	{
	    if (*szLine == '(')
	    {
	        nFlag++;
	    }
            else if (*szLine == ')')
            {
                 nFlag--;
            }

            *(szWord++) = *(szLine++);
            nWordLength ++;
            nLength ++;

            if (((*szLine == ' ') || (*szLine == ',') || (nWordLength == uWordSize -1))	
               && (nFlag == 0))
            {
                break;
            }
	}
    }
	
    *szWord = '\0';

    return(nLength);
}


/***************************************************************************
 * 功能描述: 根据参数定义的序号从命令定义文件中得到它的数据的最大长度
 *	     命令定义的形式是: para8_length = 16
 *输入：
 *	szCmdName：命令名称 
 *	nParaNO	 ：指定的命令参数的序号 
 *输出：
 *	 无
 * 返回值：
 *   数据的最大长度,如果没有定义,返回0                                                                       *
 **************************************************************************/
int CMacroFile::GetParaMaxDataLen(char *szCmdName, UINT2 nParaNO)
{
    char szPara[16];

    //对于字符型参数，一般要求给出该参数的最大长度
    sprintf(szPara, "para%d_length", nParaNO);
    return (GetInt(szCmdName, szPara, 15));
}

/*****************************************************************************************
 *功能描述：判断一个参数是否属于必须参数，例如："must_give = 1 2"表示参数1、2必须给出
 *输入：
 *	szCmdName：MML命令名称
 *	nParaCode：参数码
 *输出：
 *	无
 *返回：
 *	0：非必须参数
 *	1：必须参数
 *****************************************************************************************/
BYTE CMacroFile::IsParaMustGive(char *szCmdName, int nParaCode)
{
    char szWord[MAX_MACRO_NAME_LEN];
    char szParaLine[MAX_LINE_LEN];
    char *pszBuf;
    int nMovLength;

    GetString(szCmdName, SZ_MUST_GIVE, SZ_EMPTY_STR, szParaLine, MAX_LINE_LEN);
    //如果一个MML命令定义中根本没有"must_give"这一行，则认为其所有参数均为可选
    if (strlen(szParaLine) == 0)
    {
        return  ((BYTE)OPT_ENABLE);
    }

    strlen(szParaLine);
    pszBuf = (char *)szParaLine;

    do
    {
	nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
	pszBuf += nMovLength;
	
	if (strlen(szWord) <= 0)
        { 
       	    return (BYTE)OPT_ENABLE;
        }

        int iCode = atoi(szWord);

        if (iCode == nParaCode)		
        {
            return (BYTE)OPT_MUST;
        }
    } while(1);
	
    return (BYTE)OPT_ENABLE;
}

/*****************************************************************************************
 *功能描述：从形式为"说明(取值)"的字符串中分离出取值字符串，存入szOutput中，
 *输入：
 *	pszType	：标识字符串，分为：
 *		  参数的缺省值：default(取值)，pszType = "default(";
 *		  参数对应的关系表：table(取值)，pszType = "table(";
 *		  参数对应的字段：field(取值)，pszType = "field(";
 *
 *	szEnumName：输入字符串
 *输出：
 *	szOutput ：
 *返回：
 *	TRUE/FALSE
 *****************************************************************************************/
BOOL CMacroFile::GetCommonValue(char *szEnumName, char *szOutput, char *pszType)
{
    char szType[256];
    strcpy(szType, pszType);

    char *pFind = strstr(szEnumName, szType);	
    if (pFind != szEnumName)	
    {
        return FALSE;	
    }

    sprintf(szEnumName, "%s", &szEnumName[strlen(szType)]);

    StringLTrim(szEnumName);

    int iLength = strlen(szEnumName);

    if (iLength <= 1)
    {
    	return FALSE;
    }

    if (szEnumName[iLength - 1] != ')')	
    {
        return FALSE;
    }

    szEnumName[iLength - 1] = '\0';		//截掉")"字符
	
    StringRTrim(szEnumName);
    strcpy(szOutput, szEnumName);

    return TRUE;
}

/*****************************************************************************************
 *功能描述：读取参数的参数码,参数名,参数显示名,缺省值等信息
 *输入：
 *  szParaLine：MML命令参数定义行
 *输出：
 *  nParaCode   ：参数值
 *  szParaName  ：参数名
 *  szDefault   ：参数缺省值
 *  szDspName   : 参数显示名称
 *  nMoveLength ：移动字节数
 *
 *返回：
 *  TRUE/FALSE
 *****************************************************************************************/
BOOL CMacroFile::GetParaAttribute(char *szParaLine, int& nParaCode, char *szParaName, 
                                  char *szDspName,  char *szDefault, int& nMoveLength)
{
    UINT2 nTmpLen;
    char  szWord[MAX_MACRO_NAME_LEN];

    szParaName[0] = '\0';
    szDspName[0]  = '\0';
    szDefault[0]  = '\0';
    nMoveLength   = 0;

    //读取参数定义行中的第一个单词
    nTmpLen = GetNextWord(szWord, szParaLine, MAX_MACRO_NAME_LEN);

    if (strlen(szWord) <= 0)	
    {
        return FALSE;
    }

    //如果参数的定义形式是"1 = 55 paraname ....."
    if (IsDigitValue(szWord))	//如果是数字,表示这个参数的值
    {
	    nParaCode = atoi(szWord);
	    szParaLine  += nTmpLen;    //指针前移
	    nMoveLength += nTmpLen;

	    //读取下一个单词,应该是参数名
	    nTmpLen = GetNextWord(szWord, szParaLine, MAX_MACRO_NAME_LEN);
	    if (strlen(szWord) <= 0)
        {  
           return FALSE;
        }

	    sprintf(szParaName, "%s", szWord);
    }
    else	//非数字,表示参数名
    {
	    sprintf(szParaName, "%s", szWord);
    }

    szParaLine += nTmpLen;    //指针前移
    nMoveLength += nTmpLen;

    nTmpLen = GetNextWord(szWord, szParaLine, MAX_MACRO_NAME_LEN);

    int nStrLen = strlen(szWord);
    if (nStrLen <= 0)
    {
        return TRUE;
    }

    //判断这个单词是否参数的显示名称，即类似如：cg (命令组)
    if ((szWord[0] == '(') && (szWord[nStrLen - 1] == ')'))
    {
	    int nLen = strlen(szWord);
	    if (nLen <= 2)
        {
            return FALSE;    //无显式名称，即括号内不含字符
        }

	    for (int n = 1; n < nLen - 1; n++)
        {
	        szDspName[n-1] = szWord[n];	//获得参数的显式名称，但不包含左右括弧
	    }

        szDspName[nLen - 2] = '\0';	    //设置字符串结尾标志	
		
        szParaLine += nTmpLen;          //指针前移
        nMoveLength += nTmpLen;

        nTmpLen = GetNextWord(szWord, szParaLine, MAX_MACRO_NAME_LEN);
        if (strlen(szWord) <= 0)
        {
            return TRUE;
        }
    }

    //判断这个单词是否是缺省值描述
    char  szTmpWord[MAX_MACRO_NAME_LEN] = SZ_EMPTY_STR;
    strcpy(szTmpWord, szWord);

    if (GetCommonValue(szTmpWord, szDefault, SZ_DEFAULT))
    {
	    nMoveLength += nTmpLen;
    }
    else   //无缺省值
    {
	    szDefault[0] = '\0';
    }

    return TRUE;
}

/******************************************************************************************
 *功能描述：从一个枚举型字符串中获得枚举名称和枚举值
 *输入：
 *	szEnumString：符合枚举形式的字符串，例如"G_0~0"，枚举名为G_0，枚举值为0
 *输出：
 *	szEnumName：枚举名称
 *  uEnumValue：枚举值
 *返回：
 *	TRUE/FALSE
 ******************************************************************************************/
BOOL CMacroFile::GetEnumValue(char *szEnumString, char *szEnumName, UINT4& uEnumValue)
{
    int i;
    int j;
    char szValue[MAX_MACRO_NAME_LEN];

    i = 0;
    while ((szEnumString[i] != '~') && (szEnumString[i] != '\0') 
        && (szEnumString[i] != '('))
    {
	szEnumName[i] = szEnumString[i];
	i++;

	if (i >= (MAX_MACRO_NAME_LEN - 1))	
	{
	    printf("The Enum String Is Too Long: %s\n", szEnumString);

	    return (FALSE);
	}
    }

    szEnumName[i] = '\0';	//枚举名

    i++;	//跳过"~"或者"("字符
    j = 0;

    while ((szEnumString[i] != '\0') && (j < (MAX_MACRO_NAME_LEN - 1)) 
        && (szEnumString[i] != '('))
    {
	    szValue[j] = szEnumString[i];	//枚举值
	    i++;
	    j++;
    }

    szValue[j] = '\0';
    if (!IsDigitValue(szValue))	
    {
	    printf("The Enum Value Isn't Digit: %s\n", szEnumString);

	    return FALSE;	//枚举值不是数字型
    }

    uEnumValue = atoi(szValue);		//将数字串转换成数字

    return(TRUE);
}


/*****************************************************************************
 *功能描述：转换一条MML命令中中的子参数
 *输入：
 *	pszDesc：区间字符串
 *输出：
 *	无
 *返回：
 *	区间类型，例如： *{0~100}, 则返回1（ENUM_ZONE），表示是区间值
 *name~2(display_name) value~3(fadsaf)，则返回2（ENUM_ARATE），表示是枚举值
 *asdfasd~aa，则返回-1，表示MACRO.INI书写错误
 ******************************************************************************/
BYTE CMacroFile::GetRangeType(char *pszDesc)
{
    char szWord[MAX_MACRO_NAME_LEN];
    int nMovLength;
	
    nMovLength = GetNextWord(szWord, pszDesc, MAX_MACRO_NAME_LEN);
    if (strlen(szWord) <= 0)
    {
       	return ENUM_NONE;	//非区间值或者枚举型
    }

    if (szWord[0] == '{')
    {
	    int nWordLen = strlen(szWord);

	    if ((szWord[nWordLen - 1] != '}') || (nWordLen < 2))
        {
	        return (0xFF);
        }
 
	    return ENUM_ZONE;	//区间值
    }

    char szEnumValName[MAX_MACRO_NAME_LEN];
    UINT4 uEnumValue;

    if (!GetEnumValue(szWord, szEnumValName, uEnumValue))
    {
	    return(0xFF);
    }

    return ENUM_ARATE;
}

/*****************************************************************************
 *功能描述：转换一条MML命令中中的子参数
 *输入：
 *	pszDesc：区间字符串，例如：year {1998~3000}
 *      nCmdCode：MML命令码
 *      nParaCode：参数码
 *	nChildParaCode：子参数码
 *输出：
 *	将该子参数区间信息输出到参数区间信息表
 *返回：
 *      如果pszBuf符合区间信息表示方法，则返回TRUE，否则返回FALSE。
 ******************************************************************************/
BOOL CMacroFile::WriteParaRangeTbl(char *pszBuf, int nCmdCode, 
				   int nParaCode, int nChildParaCode)
{
    char szWord[MAX_MACRO_NAME_LEN];
    int nMovLength;
    SParaRangeDef prd;

    memset((void *)&prd, 0, sizeof(SParaRangeDef));

    nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
    if (strlen(szWord) <= 0)	return FALSE;

    if (szWord[0] == ('{'))
    {
	    char szTmpWord[MAX_MACRO_NAME_LEN];
	    int nWordLen = strlen(szWord);

	    if (szWord[nWordLen - 1] != '}' || nWordLen < 2)
		    return(FALSE);
       
	    int i = 0;
	    for (i = 0; i < nWordLen - 2; i++)
        {
	        szTmpWord[i] = szWord[i+1];
        }

	    szTmpWord[i] = '\0';

	    char szEnumName[MAX_MACRO_NAME_LEN];
	    UINT4 uEnumValue;

	    if (!GetEnumValue(szTmpWord, szEnumName, uEnumValue))
        {
	        return FALSE;
        }

	    if (!IsDigitValue(szEnumName))
        {
	        return FALSE;
        }

	    prd.uCmdCode  = nCmdCode;			        //MML命令码
	    prd.uParaCode = nParaCode;			        //参数码
	    prd.uChildParaCode = nChildParaCode;		//子参数码
	    prd.nMinValue = (UINT4)atoi(szEnumName);	//最小值
	    prd.nMaxValue = uEnumValue;			        //最大值

	    if (prd.nMinValue >= prd.nMaxValue)	
        {
            return FALSE;
        }

	    fwrite((void *)&prd, sizeof(prd), 1, m_fpParaRange);

	    return TRUE;
    }

    return FALSE;
}

/*****************************************************************************
 *功能描述：转换一条MML命令中中的子参数
 *输入：
 *	szCmdName：MML命令名称
 *	nCmdCode：MML命令码
 *      nParaCode：参数码
 *      nParaSeq：参数序号
 *      nChildNum：子参数个数
 *输出：
 *		无
 *返回：
 *		转换成功返回TRUE，否则返回FALSE。
 ******************************************************************************/
BOOL CMacroFile::ConvertChildPara(char *szCmdName, int nCmdCode, int nParaSeq, 
                                  int nParaCode, int nChildNum)
{
    char szParaSeq[16];	
    char szParaLine[MAX_LINE_LEN];
    char szWord[MAX_MACRO_NAME_LEN];

    char *pszBuf;
    SChildParaDef spd;

    //注意MML子参数码编号从1开始
    //子参数码的定义行如：2.1=op (操作员) SYS~0(系统操作员) OPER~1(普通操作员)
    for (int i = 1; i <= nChildNum; i ++)
    {
	    memset((void *)&spd, 0, sizeof(SChildParaDef));

	    sprintf(szParaSeq, "%d.%d", nParaSeq, i);	//
	    GetString(szCmdName, szParaSeq, SZ_EMPTY_STR, szParaLine, MAX_LINE_LEN);
	    if (strlen(szParaLine) == 0)	
	    {
		    return(FALSE);
	    }

	    StringToLower(szParaLine);
	    pszBuf = szParaLine;

	    char szParaName[PARA_NAME_LEN];
	    char szParaDspName[PARA_NAME_LEN];
	    char szDefault[PARA_NAME_LEN];

	    int iParaNo, nMovLength;

	    if (GetParaAttribute(pszBuf, iParaNo, szParaName, szParaDspName, szDefault, nMovLength))
	    {
	        pszBuf += nMovLength;
	        nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
	
	        if (strlen(szWord) <= 0)	//子参数无其它信息描述，则将其输出到子参数表中
	        {
	            spd.uCmdCode     = nCmdCode;
	            spd.uParaCode    = nParaCode;
	            spd.uChildParaCode = i;
	
	            strcpy(spd.szDefValue, szDefault);
	            StringToUpper(spd.szDefValue);		//字符串统一按大写保存

	            strcpy(spd.szParaName, szParaName);	
	            StringToUpper(spd.szParaName);	//字符串统一按大写保存	

	            strcpy(spd.szDspName, szParaDspName);
	            StringToUpper(spd.szDspName);

	            fwrite((void *)&spd, sizeof(spd), 1, m_fpChildPara);

	            continue;
            }
        }
	    else
        {
	         return FALSE;
        }

	    //得到参数的值域描述形式
	    int yRangeType = GetRangeType(pszBuf);	//子参数的区间类型
	    if (yRangeType == 0xFF)	
        {
	        return FALSE;
        }
	    else					
        {
	        spd.yRangeType = yRangeType;		//区间类型
        }

	    spd.uCmdCode     = nCmdCode;
	    spd.uParaCode    = nParaCode;
	    spd.uChildParaCode = i;

	    strcpy(spd.szDefValue, szDefault);
	    StringToUpper(spd.szDefValue);		//字符串统一按大写保存

	    strcpy(spd.szParaName, szParaName);
	    StringToUpper(spd.szParaName);	//字符串统一按大写保存

	    strcpy(spd.szDspName, szParaDspName);
        StringToUpper(spd.szDspName);

	    fwrite((void *)&spd, sizeof(spd), 1, m_fpChildPara);
	
	    if (yRangeType == ENUM_ZONE)		//子参数是区间类型
	    {
	        if (!WriteParaRangeTbl(pszBuf, nCmdCode, nParaCode, i))
		    {
                return FALSE;
            }
	    }
	    else if (yRangeType == ENUM_ARATE)	//子参数是枚举类型
	    {
	        if (!WriteParaEnumTbl(pszBuf, nCmdCode, nParaCode, i))
            {
		        return FALSE;
            }
	    }	
    }

    return TRUE;
}

/*****************************************************************************
 *功能描述：转换一条MML命令中的枚举信息
 *输入：
 *	pszBuf        ：包含枚举信息的字符串
 *	nCmdCode      ：MML命令码
 *      nParaCode     ：参数码
 *      nChildParaCode: 子参数码
 *输出：
 *	无
 *返回：
 *	转换成功返回TRUE，否则返回FALSE。
 ******************************************************************************/
BOOL CMacroFile::WriteParaEnumTbl(char *pszBuf, int nCmdCode, 
                                  int nParaCode, int nChildParaCode)
{
    char szWord[MAX_MACRO_NAME_LEN];
    int nMovLength, i, j;
    char szValue[MAX_MACRO_NAME_LEN];
    char szEnumName[MAX_MACRO_NAME_LEN];
    char szDspName[MAX_MACRO_NAME_LEN];
    SParaEnumDef ped;
    int iFlag = 0;

    do
    {	//获得pszBuf中的下一个单词
        nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
        if (strlen(szWord) <= 0)	break;

        i = 0;

        //获得枚举字符串, 枚举参数定义格式是: G_0~0 G_1~1，其中G_0, G_1是枚举字符串
        //0、1分别是G_0、G_1对应的枚举值
        while ((szWord[i] != '~')  && (szWord[i] != '\0') && (szWord[i] != '('))
        {
            szEnumName[i] = szWord[i];
	        i++;

	        if (i >= MAX_MACRO_NAME_LEN)	
            {
                return (FALSE);	//枚举字符串过长
            }
	    }

	    if (szWord[i] != '~')	return FALSE;	//不符合枚举参数的定义格式（G_0~0）

	    szEnumName[i] = '\0';	//设置字符串的结束标志

	    i++;
  	    j = 0;
	    //获得枚举值
	    while ((szWord[i] != '\0') && (szWord[i] != '('))
	    {
	        szValue[j] = szWord[i];
	        i++;
	        j++;
	        if (j >= MAX_MACRO_NAME_LEN)	return FALSE;
	    }

	    szValue[j] = '\0';
	    if (!IsDigitValue(szValue))	return FALSE;	//枚举值必须全是数字

	    int iValue = atoi(szValue);			//将字符串串转换成数字
	
	    if (szWord[i] == '\0')	//如果是name~value形式
	    {
	        memset((void *)&ped, 0, sizeof(SParaEnumDef));

	        ped.uCmdCode       = nCmdCode;	       //MML命令字
	        ped.uParaCode      = nParaCode;	       //参数码
	        ped.uChildParaCode = nChildParaCode;   //子参数码
	        ped.uValue         = iValue;		   //枚举值
	        strcpy(ped.szInput, szEnumName);	   //枚举名称
	        StringToUpper(ped.szInput);	           //将字符串转换成大写
					
	        fwrite((void *)&ped, sizeof(ped), 1, m_fpParaEnum);		//输出到枚举信息表
	    }

	    //如果是name~value(display_value)形式，还要获得枚举显式名称
  	    else if (szWord[i] == '(')		
	    {
	        i++;
	        j = 0;

	        while (((szWord[i] != '\0') && (szWord[i] != ')'))
	           ||  ((szWord[i] == ')')  && (iFlag != 0)))
	        {
	            if (szWord[i] == '(')
	            {
	                iFlag++;
	            }
	            else if (szWord[i] == ')')
	            {
	                iFlag--;
                }
			
	            szDspName[j] = szWord[i];
	            i++;
	            j++;
	            if (j >= MAX_MACRO_NAME_LEN)	return FALSE;
	        }
		
	        if (szWord[i] != ')')		return FALSE;

	        szDspName[j] = '\0';
	
 	        memset((void *)&ped, 0, sizeof(SParaEnumDef));

	        ped.uCmdCode       = nCmdCode;
	        ped.uParaCode      = nParaCode;
	        ped.uChildParaCode = nChildParaCode;
	        ped.uValue         = iValue;

	        strcpy(ped.szInput, szEnumName);
            StringToUpper(ped.szInput);
	    
            strcpy(ped.szDspName, szDspName);
            StringToUpper(ped.szDspName);
            
            fwrite((void *)&ped, sizeof(ped), 1, m_fpParaEnum);
        }
		
        //取下一个枚举
        pszBuf = pszBuf + nMovLength;
    }while(1);

    return TRUE;
}
