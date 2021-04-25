#include "read_config.h"

//---------------------------------------------------
//  ������:     CAlarmProperty
//  ����:       ��CAlarmProperty�Ĺ��캯��
//  �������:   ��
//  �������:   ��
//  ���ú�����  ��
//  ����ֵ:     ��
//----------------------------------------------------
CAlarmProperty::CAlarmProperty()
{
	m_alarmid = 0;                        //��ʼ��m_alarmid
	m_alarmtype = 0;                     //��ʼ��m_alarmtype
	m_machinetype = 0;                   //��ʼ��m_machinetype
    m_strCfgPath = "";
    m_strError = "";
}
//---------------------------------------------------
//  ������:     CAlarmProperty
//  ����:       ��CAlarmProperty����������
//  �������:   ��
//  �������:   ��
//  ���ú�����  ��
//  ����ֵ:     ��
//----------------------------------------------------
CAlarmProperty::~CAlarmProperty()
{
}
//---------------------------------------------------
//  ������:     strPartCpy
//  ����:       ��Դ�ַ����е��ַ�����ʼλ�õ�����λ��
//              ���ַ����ݿ�����Ŀ���ַ����С�
//  �������:   int fpos, int lpos,
//              char* source, char* dest
//  �������:   ��
//  ���ú�����  ��
//  ����ֵ:     void 
//----------------------------------------------------
void CAlarmProperty::strPartCpy(int fpos, int lpos, char* dest, char* source)
{
    int i;
	
	//���fposС��0��fposС��lpos,�����п���
	if((fpos >= 0) && (fpos < lpos))
	{
		for (i=fpos ; i<lpos ; i++)
		{
			dest[i-fpos] = source[i];
		}
	}
}

//---------------------------------------------------
//  ������:     GetFPos
//  ����:       �õ��ַ����е�һ����Ŀ���ַ�ƥ���λ��
//  �������:   char* string, char ch
//  �������:   ��
//  ���ú�����  ��
//  ����ֵ:     int
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
//  ������:     GetLPos
//  ����:       �õ��ַ��������һ����Ŀ���ַ�ƥ���λ��
//  �������:   char* string, char ch
//  �������:   ��
//  ���ú�����  ��
//  ����ֵ:     int
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
//  ������:     GetValue
//  ����:       �������ļ��еõ��澯ID��澯���ͻ�
//              �����澯�ķ���������
//  �������:   int fpos, int lpos,
//              char* source, char* dest
//  �������:   ��
//  ���ú�����  void strPartCpy();
//  ����ֵ:     int
//----------------------------------------------------
 
int CAlarmProperty::GetValue(int fpos, int lpos, char* dest, char* source)
{
	int result;
	fpos = fpos + 1; //����TAB�ַ�

	memset(dest, 0, 50);  //ע��:Ϊ�˳�����,��ʼ��destΪ50���ֽ�
	strPartCpy(fpos, lpos, dest, source);
	result = atoi(dest);

	return result;
}

//---------------------------------------------------
//  ������:     ReadCng
//  ����:       �������ļ��У��õ��澯ID���澯�����Լ�
//              �����澯�ķ��������ͣ�DELL/IBM)
//  �������:   char* string, char ch
//  �������:   ��
//  ���ú�����  ��
//  ����ֵ:     int
//----------------------------------------------------

int CAlarmProperty::ReadCng(char Specid[50] )
{
    FILE *fp;
    char line[200];     //��ȡ�ļ���������
    char string[50];   //����ַ�������
    int ftabpos = 0;       //��һ��TAB��λ��
    int ltabpos = 0;       //�ڶ���TAB��λ��
    int commonpos = 0;     //ð�ŵ�λ��
    int lenline = 0;       //line�ĳ���
    
    
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
            
            //�ж�Specid�Ƿ����ļ���TRAP OID�ļ�¼��ƥ��
            strPartCpy(0, ftabpos, string, line);
            if( strcmp( Specid, string ) == 0)
            {
                //��ȡm_alarmidֵ������ȫ�ֱ���pwarnid
                
                m_alarmid = GetValue(ftabpos, ltabpos, string, line);
                
                
                //��ȡ����ֵ������ȫ�ֱ���ptype
                m_alarmtype = GetValue(ltabpos, commonpos, string, line);
                
                //��ȡ���������ͣ�����ȫ�ֱ���mtype,-1��Ϊ��ȥ���س����з�
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
//  ������:     CAlarmProperty::GetAlarmId()
//  ����:       �õ��澯ID
//  �������:   ��
//  �������:   ��
//  ���ú�����  ��
//  ����ֵ:     int
//----------------------------------------------------
int CAlarmProperty::GetAlarmId() const
{
	return m_alarmid;
}

//---------------------------------------------------
//  ������:     CAlarmProperty::GetAlarmType()
//  ����:       �õ��澯����
//  �������:   ��
//  �������:   ��
//  ���ú�����  ��
//  ����ֵ:     int
//----------------------------------------------------
int CAlarmProperty::GetAlarmType() const
{
	return m_alarmtype;
}

//---------------------------------------------------
//  ������:     CAlarmProperty::GetMachineType()
//  ����:       �õ�����������
//  �������:   ��
//  �������:   ��
//  ���ú�����  ��
//  ����ֵ:     int
//----------------------------------------------------
int CAlarmProperty::GetMachineType()const
{
	return m_machinetype;
}

void CAlarmProperty::SetCfgDirPath(const char *szPath)
{
    m_strCfgPath = szPath;
}
