#ifndef __GLOBE_FUN_H__
#define __GLOBE_FUN_H__

extern const char* get_date_time();

/*
    ��������Ϣ�ж���������Ϣ��ֻ��ȷ�����ӣ������ʽ�����򷵻�0
    ֧�ֵĸ�ʽΪHH:MM��HH
*/
extern int GetTimeFromString(const char* const szTime);

//�õ��ļ����
extern STRING GetFileSerial(const char* const szFileName);

extern STRING Get_Hour_Min();

#endif //__GLOBE_FUN_H__
