#ifndef READ_CONFIG_H_
#define READ_CONFIG_H_
#include <iostream.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdio.h>
//����������
class CAlarmProperty
{
public:
	void SetCfgDirPath(const char *szPath);
	CAlarmProperty();    //���캯��
	~CAlarmProperty();  //��������

	//�ַ�����������Դ�ַ����У�ָ��λ�ü���ַ�������Ŀ���ַ���
    void strPartCpy(int fpos, int lpos, char* dest, char* source);

	//�õ��ַ����ַ����еĵ�һ�γ��ֵ�λ��
    int GetFPos(char* string, char ch);

	//�õ��ַ����ַ��������һ�γ��ֵ�λ��
	int GetLPos(char* string, char ch);

	//��Դ�ַ����У�ָ��λ�ü���ַ����ݿ�����Ŀ���ַ���,��ת��Ϊ����ֵ
	int GetValue(int fpos, int lpos, char* dest, char* source);

    //�������ļ�
    int ReadCng(char Specid[50] );

	int GetAlarmId(void) const;           //�õ�AlarmId
	int GetAlarmType(void) const;         //�õ�AlarmType
	int GetMachineType(void) const;      //�õ�MachineType
    const char *GetLastError()
    {
        return m_strError.c_str();
    }
protected:
	int m_alarmid;           //�澯ID
	int m_alarmtype;         //�澯����
	int m_machinetype;       //���������ͣ�0��IBM�������У�1��DELL��������
    std::string m_strCfgPath; //config.ini�������ļ�
    std::string m_strError;   //������Ϣ
};

#endif //READ_CONFIG_H_
