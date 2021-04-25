#include "macro_file.h"
#include "../include/toolbox.h"

/*****************************************************************************
 *������������CMacroFile�Ĺ��캯��
 *���룺
 *		pszFileName��Macro.ini�ļ���ȫ·����
 *		szTblDir��ת�����MML�����Ĵ��Ŀ¼
 *�����
 *		��
 *���أ�
 *		��
 ******************************************************************************/
CMacroFile::CMacroFile(char *pszFileName):CINIFile(pszFileName)
{
    //��Ա������ʼ��
    m_fpCmd         = NULL;		//������
    m_fpPara	    = NULL;		//���������
    m_fpParaRange   = NULL;		//������Χ��
    m_fpChildPara   = NULL;		//�Ӳ�����
    m_fpParaEnum    = NULL;		//����ö�ٱ�
}

/***************************************************************************
 *������������CMacroFile����������
 *���룺
 *		���ȹر�MACRO.INI�ļ�ָ�룬Ȼ��ر�MML�������ļ�ָ��(�ر����ű�)
 *�����
 *		��
 *���أ�
 *		��
 ***************************************************************************/
CMacroFile::~CMacroFile()
{
    //�رմ򿪵�MACRO.INI�ļ�
    Close();
    
    //�رմ򿪵�MML������ļ�
    CloseAllTbl();
}

/***************************************************************************
 *�����������ر�����MML�������ļ�ָ��
 *���룺
 *		��
 *�����
 *		��
 *���أ�
 *		��
 **************************************************************************/
void CMacroFile::CloseAllTbl()
{
    //�ر�������
    if (m_fpCmd)	
    {
        fclose(m_fpCmd);
        m_fpCmd = NULL;
    }

    //�رղ��������
    if (m_fpPara)
 	{
        fclose(m_fpPara);
        m_fpPara = NULL;
    }

    //�رղ������䶨���
    if (m_fpParaRange)	
    {
        fclose(m_fpParaRange);
        m_fpParaRange = NULL;
    }

    //�ر��Ӳ�����
    if (m_fpChildPara)
    {
        fclose(m_fpChildPara);
        m_fpChildPara = NULL;
    }

    //�رղ���ö����Ϣ��
    if (m_fpParaEnum)
    {
        fclose(m_fpParaEnum);	
        m_fpParaEnum  = NULL;
    }
}

/*******************************************************************************
 *������������Macro.ini�ļ�ת����MML�������ű��
 *     	1��������tbl_CmdDef
 *	    2�����������tbl_ParaDef
 *	    3���Ӳ��������tbl_ChildParaDef
 *	    4������ö�ٶ����tbl_ParaEnumDef
 *	    5������ȡֵ��Χ�����tbl_ParaRangeDef
 *���룺
 *		nMaxCmdCode : ���MML������
 *�����
 *		��
 *���أ�
 *		ת���ɹ�����TRUE�����򷵻�FALSE��
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

    if (!IsOpen())		return FALSE;		//MACRO.INI�ļ���δ��

    //�����Զ����ƴ�������ļ���Ϊ��֧�ֿ�ƽ̨��ע���ļ���ȫ������Сд
    sprintf(szCmdFile, "%s",  "cmddef.dat");
    sprintf(szParaFile, "%s", "paradef.dat");
    sprintf(szChildFile, "%s", "childparadef.dat");
    sprintf(szEnumFile, "%s", "paraenumdef.dat");
    sprintf(szRangeFile, "%s", "pararangedef.dat");

    m_fpCmd       = fopen(szCmdFile, "wb");	    //�ö����Ʒ�ʽ���ļ�
    m_fpPara      = fopen(szParaFile, "wb");	//�ö����Ʒ�ʽ���ļ�
    m_fpChildPara = fopen(szChildFile, "wb");	//�ö����Ʒ�ʽ���ļ�
    m_fpParaEnum  = fopen(szEnumFile, "wb");	//�ö����Ʒ�ʽ���ļ�
    m_fpParaRange = fopen(szRangeFile, "wb");	//�ö����Ʒ�ʽ���ļ�

    if ((!m_fpCmd) || (!m_fpPara) || (!m_fpChildPara) 
     || (!m_fpParaEnum) || (!m_fpParaRange))
    {
	    CloseAllTbl();
        printf("Cannot Open Table File!\n");

	    return FALSE;		//�޷��򿪱��ļ�
    }

    //����ת������MML����
    for (UINT2 i = 1; i <= nMaxCmdCode; i ++)
    {
	    char szCmdCode[5] = SZ_EMPTY_STR;
	    sprintf(szCmdCode, "%d", i);
	
	    //�����������ҵ���������
	    GetString(SZ_ALL_MACROS, szCmdCode, SZ_EMPTY_STR, szCmdName, MAX_CMD_NAME_LEN);

	    if (strlen(szCmdName) == 0)	
        {
            //���������Ӧ�������������ڣ�������һ��
            //MACRO.INI�ļ�������MML�����벻���������
            continue;	
        }

	    printf("Convert MML Command:  %s. ", szCmdName);

	    if (!GetCmdDef(szCmdName, CmdDef))	return FALSE;

	    printf("OK!\n");
	    fwrite((void *)&CmdDef, sizeof(SCmdDef), 1, m_fpCmd);	//������������ļ���
		
	    if (!ConvertParaDef(szCmdName, i, CmdDef.uParaNum - 1))
	    {
		    CloseAllTbl();
		    return FALSE;
	    }
        
        //дMML����ġ�CONFIRM������
        WriteConfirmPara(i, CmdDef.uParaNum);
    }
	
    printf("Convert MML Command: Over!\n");

    return TRUE;	
}

/*****************************************************************************************
 *��������������MML�������ƶ�ȡ������������𡢲���������
 *���룺
 *	szCmdName:MML��������
 *�����
 *	CmdDef	 :��õ�MML�����ṹ(�����롢����������������ʾ���ơ�������)
 *���أ�
 *	FALSE - ʧ�ܣ�TRUE - �ɹ�
 *****************************************************************************************/
BOOL CMacroFile::GetCmdDef(char *szCmdName, SCmdDef& CmdDef)		
{
    char  szCmdLevel[64];

    //��������	
    strcpy(CmdDef.szCmdName, szCmdName);
    StringToUpper(CmdDef.szCmdName);

    //�õ�����������롢�����������ڵ����
    int nCmdCode = GetInt(szCmdName, SZ_MACRO_CODE, 0);
    if (nCmdCode == 0)		//MML�����������(macro_code)������Ϊ0
    {
	    printf("%d  %s\n", __LINE__, __FILE__);

	    return FALSE;
    }

    char szCmdCode[5];
    sprintf(szCmdCode, "%d", nCmdCode);
    
    //�õ����������������������ã�����Ϊ��������Ϊ0
    int nParaNum = GetInt(szCmdName, SZ_PARA_NUM, 0);	//�õ������������(Ĭ��Ϊ0)

    //int nNodeNum = GetInt(szCmdName, SZ_NODE_NUM, 0);	//�ڵ����,Ŀǰ��ʹ��

    GetString(szCmdName, SZ_MACRO_NAME, SZ_EMPTY_STR, 
                     CmdDef.szDspName, MAX_CMD_DSP_LEN);

    char szConfirm[100];
    int nConfirm = 1;
    GetString(SZ_MUST_CONFIRM, szCmdCode, SZ_EMPTY_STR,  szConfirm, 99);
    if (strlen(szConfirm) == 0)		nConfirm = 0;

    CmdDef.uCmdCode = (UINT2)nCmdCode;
    CmdDef.uParaNum = (UINT2)(nParaNum + 1);   //ÿһMML���һ����CONFIRM������

    //MML�����ȱʡΪ��ͨ�û�����
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
 * ÿһ��MML�������һ��"CONFIRM"������
 * ���������
 *          nCmdCode  : MML������  
 *  ���������
 *          �ޡ�
 *  ����ֵ:
 *          �ޡ� 
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
 * ��MACRO.INI�еõ���������
 * szCmdName: ��������
 * nCmdCode : ������
 * nParaNum : �������� 
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
    int  iParaMaxLen = 0;	//���������������󳤶�,��Ҫ�����ַ����Ͳ���
    BYTE yMustGive   = 0;

    //����һ��MML����������������ζ����������ԣ���������������������
    for (UINT2 i = 1; i <= nParaNum; i++)  
    {
        //�õ�MML�����������ͣ�MML�����������Ͷ����ʽ���£�
        //��para1_datatype=2�������в�����������Ϊö���ͣ�PARA_TYPE
        //���һ����������������û����ȷָ��������Ϊ�����͡�
	    yDataType = GetParaDataType(szCmdName, i);	
        
	    if (yDataType == 0xFF)		
	    {
	        printf("GetDataType() Error: CmdName = %s ParaCode = %d\n", szCmdName, i);

	        return FALSE;
	    }

	    //��������ļ��ж�ȡ���������������󳤶�(Ŀǰδʹ��)
        //MML�����ĳ��ȶ����ʽΪ����para3_length=15����һ�����
        //�ַ��Ͳ�������Ҫ�涨�䳤�ȣ�ȱʡΪ15���ֽ�
	    iParaMaxLen = GetParaMaxDataLen(szCmdName, i);
	    char szParaNO[8];
	    sprintf(szParaNO, "%d", i);

	    //�õ�����i�����һ���ַ�����MML���������ʽ���£�
        //1= op (�ʺ�)
 	    GetString(szCmdName, szParaNO, SZ_EMPTY_STR, szParaLine, MAX_LINE_LEN);

	    if (strlen(szParaLine) == 0)	//���󣺸ò�����MML������û�ж���
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
        //���һ��������Ҫ���ԣ���������������ʾ��������ȱʡֵ������У� 
	    if (GetParaAttribute(pszBuf, nParaCode, szParaName, szParaDspName, szDefault, nMovLength))		
	    {
	        pszBuf += nMovLength;
	        nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
		
	        yMustGive = IsParaMustGive(szCmdName, nParaCode);
            
            //�ò���û���������壬���������������������в�������
            //һ�������Ĵ���
	        if (strlen(szWord) <= 0)
	        {
		        memset((void *)&ParaDef, 0, sizeof(SParaDef));	
		
		        ParaDef.uCmdCode  = nCmdCode;
		        ParaDef.uParaCode = nParaCode;
	            ParaDef.yDataType = yDataType;
		        ParaDef.yMustGive = yMustGive;
		        
                //���ַ���ͳһת��Ϊ��д 
	            strcpy(ParaDef.szDefault, szDefault);
		        StringToUpper(ParaDef.szDefault);

	   	        strcpy(ParaDef.szDspName, szParaDspName);
		        StringToUpper(ParaDef.szDspName);

		        strcpy(ParaDef.szParaName, szParaName);
		        StringToUpper(ParaDef.szParaName);

                //��һ��������������������������
		        fwrite((void *)&ParaDef, sizeof(SParaDef), 1, m_fpPara);
		        continue;
	        }
	    }
	    else  //���󣺻�ȡ��������ʧ��
	    {
	        printf("GetParaAttribute() Error: CmdName = %s ParaCode = %d\n", 
                    szCmdName, i);

	        return FALSE;
	    }
		
        //�ɹ�ת��һ��MML����һ���������壬��������ö�ٱ��Ӳ��������������
	    strcpy(szTmpWord, szWord);
	    if (GetCommonValue(szWord, szTableName, SZ_TABLE))//�ж���������Ƿ��ϵ������
	    {
	        pszBuf += nMovLength;
	        nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);

	        //���û�ж����ֵ��Լ��
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
	    if (GetCommonValue(szTmpWord, szFieldName, SZ_FIELD))//�ж���������Ƿ��ֶ�����
	    {
	        pszBuf += nMovLength;
	        nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
	        //���û�ж����ֵ��Լ��
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

	    //�ж���������Ƿ�λ���ֶΣ����磺
        //2 = cg (������) fieldarray~1 ByteNumber~3 G_0~0(�澯��ѯ) G_1~1(�澯�޸�) G_2~2(��ͳ��ѯ) 
	    if (StringiCmp(szWord, SZ_COMPOUND_STRING) == 1)
	    {
	        pszBuf += nMovLength;
		
	        //�õ���һ������,��ʽӦ����ByteNumber~�ֽڸ���
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
			
	         yByteNum = (BYTE)uEnumValue;		//�ֽ���

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
	
                 //������������������������
	             fwrite((void *)&ParaDef, sizeof(SParaDef), 1, m_fpPara);
		         continue;
	        }
	    }
        //�����Ͳ��� 
 	    else if (StringiCmp(szWord, SZ_COMPLEX_STRING) == 1)
	    {
	        pszBuf +=  nMovLength;
	        //�õ���һ������,��ʽӦ����childpart~�Ӳ�������
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

            //��"childpart~"�з����"childpart"�ַ���
	        if ((!GetEnumValue(szWord, szEnumName, uEnumValue))
              || (StringiCmp(szEnumName, SZ_CHILDPART) != 1))
	        {
		        printf("Get childpart~ Error: CmdName = %s ParaCode = %d\n", 
                        szCmdName, i);

		        return (FALSE);
	        }
		
	        yByteNum = (BYTE)uEnumValue;	//�Ӳ�������

	        //��ȡ�ò������Ӳ���������Ϣ������Ӳ������嵽���Ӳ�����
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
	
		        ParaDef.yByteNum = yByteNum;		//�Ӳ�������

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

	    //�����ǲ�����ֵ������,��������ʽ,һ��������ʽ,һ��ö��ֵ��ʽ
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
			
	    if (ParaDef.yRangeType == ENUM_ZONE)		//����ֵ
	    {
            //������������ͣ���Ӧת�������
	        if (!WriteParaRangeTbl(pszBuf, nCmdCode, nParaCode, 0))
	        {
		        printf("WriteParaRangeTbl() Error: CmdName = %s ParaCode = %d\n", 
                        szCmdName, i);

		        return FALSE;
	        }
	    }		
        //�����ö�����ͣ���Ӧת��ö����Ϣ��
	    else if (ParaDef.yRangeType == ENUM_ARATE)	//�����ö��ȡֵ��ʽ
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
 *���������������������ơ����������õ�������������
 *���룺
 *	szCmdName��MML��������
 *      nParaNo  ��������
 *�����
 *	��
 *���أ�
 *	��������(MML_PARA_TYPE)
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

    //��MACRO.INI�ļ��ж��ò������壬��������ڣ��򷵻�0xFF
    GetString(szCmdName, szParaNo, SZ_EMPTY_STR, szParaLine, MAX_LINE_LEN);
    if (strlen(szParaLine) == 0)
    {
        return(0xFF);	//���󣺲���������
    }

    pszBuf = szParaLine;
    StringToLower(szParaLine);

    //�õ�һ���ַ����еĵ�һ�����ʣ��õ��ʿ����ǣ�
    //�������ƣ����磺1 = ParaName
    //�����룺  ���磺1 = 55
    nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
    if (strlen(szWord) == 0)	
    {
        return(0xFF);
    }

    if (IsDigitValue(szWord))   //�����"1 = 55 ParaName ....."
    {
        pszBuf += nMovLength;
        nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
        if (strlen(szWord) <= 0)
        {
            return(0xFF);	//����ֻ�в����룬û�в�����	
        }
    }

    pszBuf = pszBuf + nMovLength;
    nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);

    int iLength = strlen(szWord);
    if ((iLength > 0) && (szWord[0] == '(') && (szWord[iLength - 1] == ')'))
    {
	    //�����������ǲ�������ʾ����
	    pszBuf += nMovLength;
	    nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
    }

    //���������ȱʡֵ
    char *szDefault = szWord;
    char *pDest = NULL;
    pDest = strstr(szDefault, SZ_DEFAULT);

    if (pDest == szDefault)
    {
	    pszBuf += nMovLength;
	    nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
    }

    //�����������"paraname = attr1-0&attr2-1&attr3-1"����ô��������Ϊ��
    //"1 = paraname fieldarray~1 attr1~1 attr2~2 attr3~3....."
    //����ζ��ò����Ǹ��ϲ�����attr1,attr2...��һ���ֽ�
    //����"attr2~2"��ʾattr2�ǵڶ�������ֵ
    if (StringiCmp(szWord, SZ_COMPOUND_STRING) == 1)
    {
        return PT_BITFIELD;
    }
    else if (StringiCmp(szWord, SZ_COMPLEX_STRING) == 1)
    {
        return PT_COMPLEX;
    }

    //��������
    sprintf(szDataType, "para%d_datatype", nParaCode);
    yDataType = (BYTE)GetInt(szCmdName, szDataType, 0xFF);	//��ò�����������

    if (yDataType == 0xFF)	
    {
        //���û����ʽ����ĳ���������������ͣ���ò���Ϊ������
        yDataType = PT_DIGIT;	//����Ĭ������������������
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
 *������������һ���ַ����еõ���һ������,�ָ����ǿո�򶺺�
 * ���룺 
 *       szLine    �����ַ���
 *	 uWordSize ������szWord����󳤶�
 * �����
 *       szWord	   ��ȡ�õĵ��ʡ���������������������                                       
 * ����ֵ��
 *       ǰ�Ƶľ���ƫ��ֵ,����ǰ�õĿո�Ͷ�������
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
        //���˵�ǰ�õĿո�򶺺�
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
 * ��������: ���ݲ����������Ŵ�������ļ��еõ��������ݵ���󳤶�
 *	     ��������ʽ��: para8_length = 16
 *���룺
 *	szCmdName���������� 
 *	nParaNO	 ��ָ���������������� 
 *�����
 *	 ��
 * ����ֵ��
 *   ���ݵ���󳤶�,���û�ж���,����0                                                                       *
 **************************************************************************/
int CMacroFile::GetParaMaxDataLen(char *szCmdName, UINT2 nParaNO)
{
    char szPara[16];

    //�����ַ��Ͳ�����һ��Ҫ������ò�������󳤶�
    sprintf(szPara, "para%d_length", nParaNO);
    return (GetInt(szCmdName, szPara, 15));
}

/*****************************************************************************************
 *�����������ж�һ�������Ƿ����ڱ�����������磺"must_give = 1 2"��ʾ����1��2�������
 *���룺
 *	szCmdName��MML��������
 *	nParaCode��������
 *�����
 *	��
 *���أ�
 *	0���Ǳ������
 *	1���������
 *****************************************************************************************/
BYTE CMacroFile::IsParaMustGive(char *szCmdName, int nParaCode)
{
    char szWord[MAX_MACRO_NAME_LEN];
    char szParaLine[MAX_LINE_LEN];
    char *pszBuf;
    int nMovLength;

    GetString(szCmdName, SZ_MUST_GIVE, SZ_EMPTY_STR, szParaLine, MAX_LINE_LEN);
    //���һ��MML������и���û��"must_give"��һ�У�����Ϊ�����в�����Ϊ��ѡ
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
 *��������������ʽΪ"˵��(ȡֵ)"���ַ����з����ȡֵ�ַ���������szOutput�У�
 *���룺
 *	pszType	����ʶ�ַ�������Ϊ��
 *		  ������ȱʡֵ��default(ȡֵ)��pszType = "default(";
 *		  ������Ӧ�Ĺ�ϵ��table(ȡֵ)��pszType = "table(";
 *		  ������Ӧ���ֶΣ�field(ȡֵ)��pszType = "field(";
 *
 *	szEnumName�������ַ���
 *�����
 *	szOutput ��
 *���أ�
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

    szEnumName[iLength - 1] = '\0';		//�ص�")"�ַ�
	
    StringRTrim(szEnumName);
    strcpy(szOutput, szEnumName);

    return TRUE;
}

/*****************************************************************************************
 *������������ȡ�����Ĳ�����,������,������ʾ��,ȱʡֵ����Ϣ
 *���룺
 *  szParaLine��MML�������������
 *�����
 *  nParaCode   ������ֵ
 *  szParaName  ��������
 *  szDefault   ������ȱʡֵ
 *  szDspName   : ������ʾ����
 *  nMoveLength ���ƶ��ֽ���
 *
 *���أ�
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

    //��ȡ�����������еĵ�һ������
    nTmpLen = GetNextWord(szWord, szParaLine, MAX_MACRO_NAME_LEN);

    if (strlen(szWord) <= 0)	
    {
        return FALSE;
    }

    //��������Ķ�����ʽ��"1 = 55 paraname ....."
    if (IsDigitValue(szWord))	//���������,��ʾ���������ֵ
    {
	    nParaCode = atoi(szWord);
	    szParaLine  += nTmpLen;    //ָ��ǰ��
	    nMoveLength += nTmpLen;

	    //��ȡ��һ������,Ӧ���ǲ�����
	    nTmpLen = GetNextWord(szWord, szParaLine, MAX_MACRO_NAME_LEN);
	    if (strlen(szWord) <= 0)
        {  
           return FALSE;
        }

	    sprintf(szParaName, "%s", szWord);
    }
    else	//������,��ʾ������
    {
	    sprintf(szParaName, "%s", szWord);
    }

    szParaLine += nTmpLen;    //ָ��ǰ��
    nMoveLength += nTmpLen;

    nTmpLen = GetNextWord(szWord, szParaLine, MAX_MACRO_NAME_LEN);

    int nStrLen = strlen(szWord);
    if (nStrLen <= 0)
    {
        return TRUE;
    }

    //�ж���������Ƿ��������ʾ���ƣ��������磺cg (������)
    if ((szWord[0] == '(') && (szWord[nStrLen - 1] == ')'))
    {
	    int nLen = strlen(szWord);
	    if (nLen <= 2)
        {
            return FALSE;    //����ʽ���ƣ��������ڲ����ַ�
        }

	    for (int n = 1; n < nLen - 1; n++)
        {
	        szDspName[n-1] = szWord[n];	//��ò�������ʽ���ƣ�����������������
	    }

        szDspName[nLen - 2] = '\0';	    //�����ַ�����β��־	
		
        szParaLine += nTmpLen;          //ָ��ǰ��
        nMoveLength += nTmpLen;

        nTmpLen = GetNextWord(szWord, szParaLine, MAX_MACRO_NAME_LEN);
        if (strlen(szWord) <= 0)
        {
            return TRUE;
        }
    }

    //�ж���������Ƿ���ȱʡֵ����
    char  szTmpWord[MAX_MACRO_NAME_LEN] = SZ_EMPTY_STR;
    strcpy(szTmpWord, szWord);

    if (GetCommonValue(szTmpWord, szDefault, SZ_DEFAULT))
    {
	    nMoveLength += nTmpLen;
    }
    else   //��ȱʡֵ
    {
	    szDefault[0] = '\0';
    }

    return TRUE;
}

/******************************************************************************************
 *������������һ��ö�����ַ����л��ö�����ƺ�ö��ֵ
 *���룺
 *	szEnumString������ö����ʽ���ַ���������"G_0~0"��ö����ΪG_0��ö��ֵΪ0
 *�����
 *	szEnumName��ö������
 *  uEnumValue��ö��ֵ
 *���أ�
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

    szEnumName[i] = '\0';	//ö����

    i++;	//����"~"����"("�ַ�
    j = 0;

    while ((szEnumString[i] != '\0') && (j < (MAX_MACRO_NAME_LEN - 1)) 
        && (szEnumString[i] != '('))
    {
	    szValue[j] = szEnumString[i];	//ö��ֵ
	    i++;
	    j++;
    }

    szValue[j] = '\0';
    if (!IsDigitValue(szValue))	
    {
	    printf("The Enum Value Isn't Digit: %s\n", szEnumString);

	    return FALSE;	//ö��ֵ����������
    }

    uEnumValue = atoi(szValue);		//�����ִ�ת��������

    return(TRUE);
}


/*****************************************************************************
 *����������ת��һ��MML�������е��Ӳ���
 *���룺
 *	pszDesc�������ַ���
 *�����
 *	��
 *���أ�
 *	�������ͣ����磺 *{0~100}, �򷵻�1��ENUM_ZONE������ʾ������ֵ
 *name~2(display_name) value~3(fadsaf)���򷵻�2��ENUM_ARATE������ʾ��ö��ֵ
 *asdfasd~aa���򷵻�-1����ʾMACRO.INI��д����
 ******************************************************************************/
BYTE CMacroFile::GetRangeType(char *pszDesc)
{
    char szWord[MAX_MACRO_NAME_LEN];
    int nMovLength;
	
    nMovLength = GetNextWord(szWord, pszDesc, MAX_MACRO_NAME_LEN);
    if (strlen(szWord) <= 0)
    {
       	return ENUM_NONE;	//������ֵ����ö����
    }

    if (szWord[0] == '{')
    {
	    int nWordLen = strlen(szWord);

	    if ((szWord[nWordLen - 1] != '}') || (nWordLen < 2))
        {
	        return (0xFF);
        }
 
	    return ENUM_ZONE;	//����ֵ
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
 *����������ת��һ��MML�������е��Ӳ���
 *���룺
 *	pszDesc�������ַ��������磺year {1998~3000}
 *      nCmdCode��MML������
 *      nParaCode��������
 *	nChildParaCode���Ӳ�����
 *�����
 *	�����Ӳ���������Ϣ���������������Ϣ��
 *���أ�
 *      ���pszBuf����������Ϣ��ʾ�������򷵻�TRUE�����򷵻�FALSE��
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

	    prd.uCmdCode  = nCmdCode;			        //MML������
	    prd.uParaCode = nParaCode;			        //������
	    prd.uChildParaCode = nChildParaCode;		//�Ӳ�����
	    prd.nMinValue = (UINT4)atoi(szEnumName);	//��Сֵ
	    prd.nMaxValue = uEnumValue;			        //���ֵ

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
 *����������ת��һ��MML�������е��Ӳ���
 *���룺
 *	szCmdName��MML��������
 *	nCmdCode��MML������
 *      nParaCode��������
 *      nParaSeq���������
 *      nChildNum���Ӳ�������
 *�����
 *		��
 *���أ�
 *		ת���ɹ�����TRUE�����򷵻�FALSE��
 ******************************************************************************/
BOOL CMacroFile::ConvertChildPara(char *szCmdName, int nCmdCode, int nParaSeq, 
                                  int nParaCode, int nChildNum)
{
    char szParaSeq[16];	
    char szParaLine[MAX_LINE_LEN];
    char szWord[MAX_MACRO_NAME_LEN];

    char *pszBuf;
    SChildParaDef spd;

    //ע��MML�Ӳ������Ŵ�1��ʼ
    //�Ӳ�����Ķ������磺2.1=op (����Ա) SYS~0(ϵͳ����Ա) OPER~1(��ͨ����Ա)
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
	
	        if (strlen(szWord) <= 0)	//�Ӳ�����������Ϣ����������������Ӳ�������
	        {
	            spd.uCmdCode     = nCmdCode;
	            spd.uParaCode    = nParaCode;
	            spd.uChildParaCode = i;
	
	            strcpy(spd.szDefValue, szDefault);
	            StringToUpper(spd.szDefValue);		//�ַ���ͳһ����д����

	            strcpy(spd.szParaName, szParaName);	
	            StringToUpper(spd.szParaName);	//�ַ���ͳһ����д����	

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

	    //�õ�������ֵ��������ʽ
	    int yRangeType = GetRangeType(pszBuf);	//�Ӳ�������������
	    if (yRangeType == 0xFF)	
        {
	        return FALSE;
        }
	    else					
        {
	        spd.yRangeType = yRangeType;		//��������
        }

	    spd.uCmdCode     = nCmdCode;
	    spd.uParaCode    = nParaCode;
	    spd.uChildParaCode = i;

	    strcpy(spd.szDefValue, szDefault);
	    StringToUpper(spd.szDefValue);		//�ַ���ͳһ����д����

	    strcpy(spd.szParaName, szParaName);
	    StringToUpper(spd.szParaName);	//�ַ���ͳһ����д����

	    strcpy(spd.szDspName, szParaDspName);
        StringToUpper(spd.szDspName);

	    fwrite((void *)&spd, sizeof(spd), 1, m_fpChildPara);
	
	    if (yRangeType == ENUM_ZONE)		//�Ӳ�������������
	    {
	        if (!WriteParaRangeTbl(pszBuf, nCmdCode, nParaCode, i))
		    {
                return FALSE;
            }
	    }
	    else if (yRangeType == ENUM_ARATE)	//�Ӳ�����ö������
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
 *����������ת��һ��MML�����е�ö����Ϣ
 *���룺
 *	pszBuf        ������ö����Ϣ���ַ���
 *	nCmdCode      ��MML������
 *      nParaCode     ��������
 *      nChildParaCode: �Ӳ�����
 *�����
 *	��
 *���أ�
 *	ת���ɹ�����TRUE�����򷵻�FALSE��
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
    {	//���pszBuf�е���һ������
        nMovLength = GetNextWord(szWord, pszBuf, MAX_MACRO_NAME_LEN);
        if (strlen(szWord) <= 0)	break;

        i = 0;

        //���ö���ַ���, ö�ٲ��������ʽ��: G_0~0 G_1~1������G_0, G_1��ö���ַ���
        //0��1�ֱ���G_0��G_1��Ӧ��ö��ֵ
        while ((szWord[i] != '~')  && (szWord[i] != '\0') && (szWord[i] != '('))
        {
            szEnumName[i] = szWord[i];
	        i++;

	        if (i >= MAX_MACRO_NAME_LEN)	
            {
                return (FALSE);	//ö���ַ�������
            }
	    }

	    if (szWord[i] != '~')	return FALSE;	//������ö�ٲ����Ķ����ʽ��G_0~0��

	    szEnumName[i] = '\0';	//�����ַ����Ľ�����־

	    i++;
  	    j = 0;
	    //���ö��ֵ
	    while ((szWord[i] != '\0') && (szWord[i] != '('))
	    {
	        szValue[j] = szWord[i];
	        i++;
	        j++;
	        if (j >= MAX_MACRO_NAME_LEN)	return FALSE;
	    }

	    szValue[j] = '\0';
	    if (!IsDigitValue(szValue))	return FALSE;	//ö��ֵ����ȫ������

	    int iValue = atoi(szValue);			//���ַ�����ת��������
	
	    if (szWord[i] == '\0')	//�����name~value��ʽ
	    {
	        memset((void *)&ped, 0, sizeof(SParaEnumDef));

	        ped.uCmdCode       = nCmdCode;	       //MML������
	        ped.uParaCode      = nParaCode;	       //������
	        ped.uChildParaCode = nChildParaCode;   //�Ӳ�����
	        ped.uValue         = iValue;		   //ö��ֵ
	        strcpy(ped.szInput, szEnumName);	   //ö������
	        StringToUpper(ped.szInput);	           //���ַ���ת���ɴ�д
					
	        fwrite((void *)&ped, sizeof(ped), 1, m_fpParaEnum);		//�����ö����Ϣ��
	    }

	    //�����name~value(display_value)��ʽ����Ҫ���ö����ʽ����
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
		
        //ȡ��һ��ö��
        pszBuf = pszBuf + nMovLength;
    }while(1);

    return TRUE;
}
