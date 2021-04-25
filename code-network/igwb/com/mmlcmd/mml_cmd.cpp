/*****************************************************************************************
 *产品名	：iGateway Bill V200		
 *模块名	：公用功能
 *文件名	：MMLCmd.cpp
 *描述信息	：完成从MACRO.INI到MML命令表的转换，测试功能。
 *版本说明	：V200R001i00M00B00D1130A	
 *版权信息	：版权所有（C）2001-2002 华为技术有限公司
 *作者		：邱意平
 *创建时间	：2001-07-30
 *修改记录	：
 *****************************************************************************************/

#include "mml_cmd.h"

void TestEnumTbl();
void TestCmdTbl();
void TestParaTbl();
void Convert(int nMaxCmdCode);
void TestRangeTbl();
void TestChildParaTbl();

int main(int argc, char* argv[])
{
	if (argc < 2)
    {
		printf("Usage: MMLCmd op  [maxcmd]\n");
        printf("op = 1: Convert All MML Commond\n");
        printf("op = 2: Test MML Command Table cmddef.dat\n");
        printf("op = 3: Test MML Parameter Table paradef.dat\n");
        printf("op = 4: Test MML Parameter Enum Table paraenumDef.dat\n");
        printf("op = 5: Test MML Parameter Range Table pararange.dat\n"); 
		printf("op = 6: Test MML Child Parameter Table childparaDef.dat\n");

        return 0;
    }
        
    int i =  atoi(argv[1]);
    switch (i)
    {
    case 1:
		if (argc < 3)
		{
			printf("Usage: MMLCmd 1 maxcmd\n");
			return 0;
		}

        Convert(atoi(argv[2]));
        break;

	case 2:
		TestCmdTbl();
		break;
       
    case 3:
		TestParaTbl();
        break;
 
    case 4:
        TestEnumTbl();
        break;
     
    case 5:
        TestRangeTbl();
        break;

	case 6:
		TestChildParaTbl();

    default:
        break;
    }

    return 0;
}


void TestRangeTbl()
{
	SParaRangeDef prd;
	FILE  *fp = NULL;

	fp = fopen("pararangedef.dat", "rb");
	if (fp == NULL)
	{
		printf("Open Failed!\n");
		return;
	}

	while (!feof(fp))
	{
		int nCount = fread((void *)&prd, sizeof(prd), 1, fp);
		if (ferror(fp))	break;
		
		if (nCount == 1)
		{
			printf("CmdCode     = %d\n", prd.uCmdCode);
			printf("ParaCode    = %d\n", prd.uParaCode);
			printf("SubParaCode = %d\n", prd.uChildParaCode);
			printf("MaxValue    = %d\n", prd.nMaxValue);
			printf("MinValue    = %d\n\n", prd.nMinValue);
			getchar();
		}
	}
	
	fclose(fp);	
}

void TestEnumTbl()
{
	SParaEnumDef spd;
	FILE  *fp = NULL;

	fp = fopen("paraenumdef.dat", "rb");
	if (fp == NULL)
	{
		printf("Open Failed!\n");
		return;
	}

	while (!feof(fp))
	{
		int nCount = fread((void *)&spd, sizeof(spd), 1, fp);
		if (ferror(fp))	break;
		
		if (nCount)
		{
			printf("CmdCode     = %d\n", spd.uCmdCode);
			printf("ParaCode    = %d\n", spd.uParaCode);
			printf("SubParaCode = %d\n", spd.uChildParaCode);
			printf("EnumValue   = %d\n", spd.uValue);
			printf("szEnumName  = %s\n\n", spd.szInput);
			getchar();
		}
	}
	
	fclose(fp);	
}

void TestCmdTbl()
{
	FILE *fp = NULL;
	int i = 0;

	fp = fopen("cmddef.dat", "rb");
	if (fp == NULL)
	{
		printf("Open Failed!\n");
		return;
	}

	while (!feof(fp))
	{
		SCmdDef cmddef;
		int nCount = fread((void *)&cmddef, sizeof(cmddef), 1, fp);
		
		if (nCount == 1)
		{
			printf("CmdCode    = %d\n", cmddef.uCmdCode);
			printf("CmdName    = %s\n", cmddef.szCmdName);
            printf("CmdDspName = %s\n", cmddef.szDspName); 
			printf("ParaNum    = %d\n", cmddef.uParaNum);
			printf("CmdLevel   = %d\n\n", cmddef.yLevel);

			getchar();
		}
	}
	
	fclose(fp);
}

void TestChildParaTbl()
{
	FILE *fp = NULL;

	fp = fopen("childparadef.dat", "rb");
	if (fp == NULL)
	{
		printf("Open Failed!\n");
		return;
	}

	while (!feof(fp))
	{
		SChildParaDef spd;

		int nCount = fread((void *)&spd, sizeof(spd), 1, fp);
		if (nCount == 1)
		{
			printf("CmdCode        = %d\n", spd.uCmdCode);
			printf("ParaCode       = %d\n", spd.uParaCode);
			printf("SubParaCode    = %d\n", spd.uChildParaCode);
			printf("SubParaName    = %s\n", spd.szParaName);
			printf("Default        = %s\n", spd.szDefValue);
			printf("RangeType      = %d\n\n", spd.yRangeType);

			getchar();
		}
	}
	
	fclose(fp);	

}

void TestParaTbl()
{
	FILE *fp = NULL;

	fp = fopen("paradef.dat", "rb");
	if (fp == NULL)
	{
		printf("Open Failed!\n");
		return;
	}

	while (!feof(fp))
	{
		SParaDef pd;

		int nCount = fread((void *)&pd, sizeof(pd), 1, fp);
		if (nCount == 1)
		{
			printf("CmdCode        = %d\n", pd.uCmdCode);
			printf("ParaCode       = %d\n", pd.uParaCode);
			printf("ParaName       = %s\n", pd.szParaName);
 			printf("ParaDspName    = %s\n", pd.szDspName); 
			printf("ChildParaNum   = %d\n", pd.yByteNum);
			printf("Default        = %s\n", pd.szDefault);
			printf("DataType       = %d\n", pd.yDataType);
			printf("MustGive       = %d\n", pd.yMustGive);
			printf("RangeType      = %d\n\n", pd.yRangeType);
			getchar();
		}
	}
	
	fclose(fp);	
}

void Convert(int nMaxCmdCode)
{
	CMacroFile *pFile = new CMacroFile("macro.ini");
	
	if (pFile->Open())
	{
		pFile->ConvertAllCmdToTbl(nMaxCmdCode);
		delete pFile;
	}
	else
	{
		printf("Open MACRO.INI Error!\n");
	}
}
