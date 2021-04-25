#include "../../include/toolbox.h"
#include "global_fun.h"

/*************************************************
  Function:		char * get_date_time()
  Description:  返回当前系统日期,格式为:YYYYMMDDHHMISS
  Calls:          
  Called By:      all
  Input:          
                  
  Output:         
  Return:        YYYYMMDDHHMISS
  Others:        test by wangyefeng 20020818;ok;
*************************************************/
const char* get_date_time()
{
	/*@ 返回当前系统日期时间,格式为:YYYYMMDDHHMISS*/
  
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
    从配置信息中读出日期信息，只精确到分钟，如果格式错误则返回0
    支持的格式为HH:MM或HH
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
    //如果只有时
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
  Description: 获得当前系统时间的：小时分钟
  Calls:          
  Called By:      all
  Input:          
			   
			   
  Output:         
  Return:        HHMI:小时分钟
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

//得到文件序号
STRING GetFileSerial(const char* const szFileName)
{

    char tmpfileserial[20] = ""; //文件序号，现在只取4位?

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

    memset( tmpfileserial, '0', 4 ); //以0填充序号
    tmpfileserial[4] = '\0';
    for(i=len-1, j=3; i>=0; i-- )//从文件最后一个"."后取4位?
	{
		if( szFileName[i] !='.' && isFlag )
		{
            // get serial
		    tmpfileserial [j] = szFileName[i];
		    j--;	
            if( j < 0 )
            {
                 break; //只取4位，然后退出
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

