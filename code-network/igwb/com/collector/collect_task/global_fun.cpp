#include "../../include/toolbox.h"
#include "global_fun.h"

/*************************************************
  Function:		char * get_date_time()
  Description:  ���ص�ǰϵͳ����,��ʽΪ:YYYYMMDDHHMISS
  Calls:          
  Called By:      all
  Input:          
                  
  Output:         
  Return:        YYYYMMDDHHMISS
  Others:        test by wangyefeng 20020818;ok;
*************************************************/
const char* get_date_time()
{
	/*@ ���ص�ǰϵͳ����ʱ��,��ʽΪ:YYYYMMDDHHMISS*/
  
    static char datetime[50];

    time_t timer;
    struct tm t;
    
    timer = time(NULL);
    t = *localtime(&timer);
    
    sprintf(datetime,"%d%02d%02d%02d%02d%02d",
    	t.tm_year+1900,t.tm_mon+1,t.tm_mday, 
    	t.tm_hour, t.tm_min, t.tm_sec);
    
    return datetime;
}


/*
    ��������Ϣ�ж���������Ϣ��ֻ��ȷ�����ӣ������ʽ�����򷵻�0
    ֧�ֵĸ�ʽΪHH:MM��HH
*/
int GetTimeFromString(const char* const szTime)
{
    if(szTime == 0 || szTime[0] == '\0')
    {
        return 0;
    }

    int nTime = 0;
    char* p = (char*)szTime;
    char* p1 = strchr(p, ':');
    if(p1 == NULL)
    //���ֻ��ʱ
    {
        if(IsDigitValue(p))
        {
            nTime = atoi(p) * 3600;
        }
    }
    else
    {
        char szBuf[10];
        strncpy(szBuf, p, p1 - p);
        szBuf[p1 - p] = '\0';
        if(IsDigitValue(szBuf))
        {
            nTime = atoi(szBuf) * 3600;
        }

        p = p1 + 1;
        p1 = strchr(p, ':');
        if(p1 == NULL)
        {
            if(IsDigitValue(p))
            {
                nTime += atoi(p) * 60;
            }
        }
        else
        {
            char szBuf[10];
            strncpy(szBuf, p, p1 - p);
            szBuf[p1 - p] = '\0';
            if(IsDigitValue(szBuf))
            {
                nTime += atoi(szBuf) * 60;
            }
        }
    }

    return nTime;
}

/*************************************************
  Function:     char * Get_Hour_Min()
  Description: ��õ�ǰϵͳʱ��ģ�Сʱ����
  Calls:          
  Called By:      all
  Input:          
			   
			   
  Output:         
  Return:        HHMI:Сʱ����
  Others:         test by wangyefeng 20020818;ok;
*************************************************/
STRING Get_Hour_Min()
{
    struct tm t;
    time_t curr_time;
    char buf[20];

    time(&curr_time);
    t = *localtime(&curr_time);
    sprintf(buf,"%02d%02d",t.tm_hour,t.tm_min);
    buf[4]=0;
    return buf;
}

//�õ��ļ����
STRING GetFileSerial(const char* const szFileName)
{

    char tmpfileserial[20] = ""; //�ļ���ţ�����ֻȡ4λ?

	char fileserial[20];
	BOOL isFlag = TRUE;
	
	int len = strlen(szFileName); 
	int i;
	int j;

    /*
	for ( i=0, j=0 ; i<=len-1; i++ )
	{
		
		if(szFileName[i] !='.'&& isFlag ==TRUE)
		{
			continue;
		}
		if(szFileName[i] =='.')
	    	isFlag = FALSE;
		
        // get serial
		tmpfileserial [j] = szFileName[i+1];
		j++;	

	}
    */

    memset( tmpfileserial, '0', 4 ); //��0������
    tmpfileserial[4] = '\0';
    for(i=len-1, j=3; i>=0; i-- )//���ļ����һ��"."��ȡ4λ?
	{
		if( szFileName[i] !='.' && isFlag )
		{
            // get serial
		    tmpfileserial [j] = szFileName[i];
		    j--;	
            if( j < 0 )
            {
                 break; //ֻȡ4λ��Ȼ���˳�
            }
		}

		if( szFileName[i] =='.')
        {
            isFlag = FALSE;
            break;
        }
	}
    
    memset(fileserial, 0, sizeof(fileserial));
	sprintf(fileserial,
            "%s%s",
            Get_Hour_Min().c_str(),
            tmpfileserial);

    return  fileserial;
}

