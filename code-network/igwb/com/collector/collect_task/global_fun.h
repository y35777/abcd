#ifndef __GLOBE_FUN_H__
#define __GLOBE_FUN_H__

extern const char* get_date_time();

/*
    从配置信息中读出日期信息，只精确到分钟，如果格式错误则返回0
    支持的格式为HH:MM或HH
*/
extern int GetTimeFromString(const char* const szTime);

//得到文件序号
extern STRING GetFileSerial(const char* const szFileName);

extern STRING Get_Hour_Min();

#endif //__GLOBE_FUN_H__
