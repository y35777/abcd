#ifndef READ_CONFIG_H_
#define READ_CONFIG_H_
#include <iostream.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdio.h>
//警报属性类
class CAlarmProperty
{
public:
	void SetCfgDirPath(const char *szPath);
	CAlarmProperty();    //构造函数
	~CAlarmProperty();  //析构函数

	//字符串拷贝，将源字符串中，指定位置间的字符拷贝到目的字符串
    void strPartCpy(int fpos, int lpos, char* dest, char* source);

	//得到字符在字符串中的第一次出现的位置
    int GetFPos(char* string, char ch);

	//得到字符在字符串中最后一次出现的位置
	int GetLPos(char* string, char ch);

	//将源字符串中，指定位置间的字符数据拷贝到目的字符串,并转换为整数值
	int GetValue(int fpos, int lpos, char* dest, char* source);

    //读配置文件
    int ReadCng(char Specid[50] );

	int GetAlarmId(void) const;           //得到AlarmId
	int GetAlarmType(void) const;         //得到AlarmType
	int GetMachineType(void) const;      //得到MachineType
    const char *GetLastError()
    {
        return m_strError.c_str();
    }
protected:
	int m_alarmid;           //告警ID
	int m_alarmtype;         //告警类型
	int m_machinetype;       //服务器类型，0－IBM磁盘阵列，1－DELL磁盘阵列
    std::string m_strCfgPath; //config.ini的配置文件
    std::string m_strError;   //错误信息
};

#endif //READ_CONFIG_H_
