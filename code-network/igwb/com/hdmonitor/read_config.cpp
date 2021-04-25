#include "read_config.h"

//---------------------------------------------------
//  函数名:     CAlarmProperty
//  功能:       类CAlarmProperty的构造函数
//  输入参数:   无
//  输出参数:   无
//  调用函数：  无
//  返回值:     无
//----------------------------------------------------
CAlarmProperty::CAlarmProperty()
{
	m_alarmid = 0;                        //初始化m_alarmid
	m_alarmtype = 0;                     //初始化m_alarmtype
	m_machinetype = 0;                   //初始化m_machinetype
    m_strCfgPath = "";
    m_strError = "";
}
//---------------------------------------------------
//  函数名:     CAlarmProperty
//  功能:       类CAlarmProperty的析构函数
//  输入参数:   无
//  输出参数:   无
//  调用函数：  无
//  返回值:     无
//----------------------------------------------------
CAlarmProperty::~CAlarmProperty()
{
}
//---------------------------------------------------
//  函数名:     strPartCpy
//  功能:       将源字符串中的字符从起始位置到结束位置
//              的字符内容拷贝到目的字符串中。
//  输入参数:   int fpos, int lpos,
//              char* source, char* dest
//  输出参数:   无
//  调用函数：  无
//  返回值:     void 
//----------------------------------------------------
void CAlarmProperty::strPartCpy(int fpos, int lpos, char* dest, char* source)
{
    int i;
	
	//如果fpos小于0或fpos小于lpos,不进行拷贝
	if((fpos >= 0) && (fpos < lpos))
	{
		for (i=fpos ; i<lpos ; i++)
		{
			dest[i-fpos] = source[i];
		}
	}
}

//---------------------------------------------------
//  函数名:     GetFPos
//  功能:       得到字符串中第一个和目标字符匹配的位置
//  输入参数:   char* string, char ch
//  输出参数:   无
//  调用函数：  无
//  返回值:     int
//----------------------------------------------------
int CAlarmProperty::GetFPos(char* string, char ch)
{
	char* pdest;
	int fpos = 0;

	pdest = strchr(string, ch);
	
	if(pdest != NULL)
	{
		fpos = pdest - string ;
		return fpos;
	}

	else
	{
		return 0;
	}
}


//---------------------------------------------------
//  函数名:     GetLPos
//  功能:       得到字符串中最后一个和目标字符匹配的位置
//  输入参数:   char* string, char ch
//  输出参数:   无
//  调用函数：  无
//  返回值:     int
//----------------------------------------------------
int CAlarmProperty::GetLPos(char* string, char ch)
{
	char* pdest;
	int lpos;

	pdest = strrchr(string, ch);
	

	if(pdest != NULL)
	{
		lpos = pdest - string ;
		return lpos;
	}
	else
	{
		return 0;
	}
}

//---------------------------------------------------
//  函数名:     GetValue
//  功能:       从配置文件中得到告警ID或告警类型或
//              产生告警的服务器类型
//  输入参数:   int fpos, int lpos,
//              char* source, char* dest
//  输出参数:   无
//  调用函数：  void strPartCpy();
//  返回值:     int
//----------------------------------------------------
 
int CAlarmProperty::GetValue(int fpos, int lpos, char* dest, char* source)
{
	int result;
	fpos = fpos + 1; //跳过TAB字符

	memset(dest, 0, 50);  //注意:为了程序简洁,初始化dest为50个字节
	strPartCpy(fpos, lpos, dest, source);
	result = atoi(dest);

	return result;
}

//---------------------------------------------------
//  函数名:     ReadCng
//  功能:       从配置文件中，得到告警ID，告警类型以及
//              产生告警的服务器类型（DELL/IBM)
//  输入参数:   char* string, char ch
//  输出参数:   无
//  调用函数：  无
//  返回值:     int
//----------------------------------------------------

int CAlarmProperty::ReadCng(char Specid[50] )
{
    FILE *fp;
    char line[200];     //读取文件的行内容
    char string[50];   //存放字符串内容
    int ftabpos = 0;       //第一个TAB键位置
    int ltabpos = 0;       //第二个TAB键位置
    int commonpos = 0;     //冒号的位置
    int lenline = 0;       //line的长度
    
    
    if ((fp = fopen(m_strCfgPath.c_str(),"r")) == NULL) 
    {
        m_strError = "Can't open the file config.ini ";
        m_strError += (m_strCfgPath + "  ");
        m_strError += strerror(errno);
        return -1;
    }
    
    while (!feof(fp))
    {
        memset(line,0,200);
        
        if (fgets(line,200,fp) != NULL)
        {         
            lenline = (int)strlen(line);
            
            memset(string, 0, 50);
            
            ftabpos = GetFPos(line, '\t');
            ltabpos = GetLPos(line, '\t');
            commonpos = GetFPos(line, ':');
            
            //判断Specid是否与文件中TRAP OID的记录相匹配
            strPartCpy(0, ftabpos, string, line);
            if( strcmp( Specid, string ) == 0)
            {
                //读取m_alarmid值，赋给全局变量pwarnid
                
                m_alarmid = GetValue(ftabpos, ltabpos, string, line);
                
                
                //读取类型值，赋给全局变量ptype
                m_alarmtype = GetValue(ltabpos, commonpos, string, line);
                
                //读取服务器类型，赋给全局变量mtype,-1是为了去掉回车换行符
                m_machinetype = GetValue(commonpos, lenline-1, string, line);
                
                fclose(fp);
                return 1;
            }//end if( strcmp(specid,string) == 0)
        }//end if(fgets(line,200,fp)!= NULL)
    }// end while(!feof(fp))
    
    fclose(fp);
    return 0;
} //end ReadCng

//---------------------------------------------------
//  函数名:     CAlarmProperty::GetAlarmId()
//  功能:       得到告警ID
//  输入参数:   无
//  输出参数:   无
//  调用函数：  无
//  返回值:     int
//----------------------------------------------------
int CAlarmProperty::GetAlarmId() const
{
	return m_alarmid;
}

//---------------------------------------------------
//  函数名:     CAlarmProperty::GetAlarmType()
//  功能:       得到告警类型
//  输入参数:   无
//  输出参数:   无
//  调用函数：  无
//  返回值:     int
//----------------------------------------------------
int CAlarmProperty::GetAlarmType() const
{
	return m_alarmtype;
}

//---------------------------------------------------
//  函数名:     CAlarmProperty::GetMachineType()
//  功能:       得到服务器类型
//  输入参数:   无
//  输出参数:   无
//  调用函数：  无
//  返回值:     int
//----------------------------------------------------
int CAlarmProperty::GetMachineType()const
{
	return m_machinetype;
}

void CAlarmProperty::SetCfgDirPath(const char *szPath)
{
    m_strCfgPath = szPath;
}
