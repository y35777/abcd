//ת��S1240��ϸ�������ı���ʽ

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../comm_convert_fun.h"
#include "../include/collect_convert_define.h"
#include "../../include/collect_convert_include.h"
#include "../../../include/toolbox.h"
#include "../../../include/format_out.h"



//S1240����IDD�굥�Ļ�����ʽ
struct S1240GBIDDDetailSrcFmt
{
    unsigned char RecordType : 4;   //BCD����ʾ�û�����¼�ĺ��е�����
                                    //1�� PSTN IDD/DDD���м�¼
                                    //2�� DBO���м�¼
                                    //3�� IN���м�¼
                                    //4�� ISDN/CTX��¼
                                    //5�� TAX������¼
                                    //6-7������
    unsigned char PartialFlg : 4;   //BCD����ʾ�û����ǵ����Ļ�������һ���������е�һ����
                                    //0�� ������¼
                                    //1�� ��¼�ĵ�һ����
                                    //2�� ��¼���м䲿��
                                    //3�� ��¼����󲿷�
    unsigned char SeqNum[4];        //˳���
    unsigned char CallerLocAttr[1]; //��ʾ�����û��ĵ�ַ����
                                    //0�� �û�����
                                    //1�� ����
                                    //2�� ������Ч����
                                    //3�� ���ʺ���
    unsigned char CallerNum[10];    //���к��룬BCD������룬����λ��16���Ƶ�E���
    unsigned char CalledLocAttr[1]; //��ʾ�����û��ĵ�ַ����
    unsigned char CalledNum[10];    //���к��룬BCD������룬����λ��16���Ƶ�E���
    unsigned char StartYear[1];     //��ʼ�꣬BCD��
    unsigned char StartMonth[1];    //��ʼ�£�BCD��
    unsigned char StartDay[1];      //��ʼ�գ�BCD��
    unsigned char StartHour[1];     //��ʼʱ��BCD��
    unsigned char StartMinute[1];   //��ʼ�֣�BCD��
    unsigned char StartSecond[1];   //��ʼ�룬BCD��
    unsigned char StartTSecond : 4; //��ʼʮ��֮һ�룬BCD��
    unsigned char CallType : 4;     //��ʾ��������
                                    //0~1������
                                    //2������ת��
                                    //3�����غ���
                                    //4���豸���У�facility call��
                                    //5�����ڷ��������˳�;�֣�
                                    //6�����ʷ��������ʺͷ��˳�;�֣�
                                    //7�������������ն˳�;�֣�
                                    //8������������������ھ֣�
                                    //9��ת��������/��;�֣��������ڹ���ת����
    unsigned char EndYear[1];       //��ֹ�꣬BCD��
    unsigned char EndMonth[1];      //��ֹ�£�BCD��
    unsigned char EndDay[1];        //��ֹ�գ�BCD��
    unsigned char EndHour[1];       //��ֹʱ��BCD��
    unsigned char EndMinute[1];     //��ֹ�֣�BCD��
    unsigned char EndSecond[1];     //��ֹ�룬BCD��
    unsigned char EndTSecond : 4;   //��ֹʮ��֮һ�룬BCD��
    unsigned char TermCause : 4;    //BCD�룬��ʾ���н�����ԭ��
                                    //0�������û��һ�
                                    //1�������û��һ�
                                    //2���쳣����
                                    //3��������
    unsigned char Duration[4];      //ͨ��ʱ����BCD��
    unsigned char CallerType1[1];
    unsigned char CallerType2 : 4;  //BCD�룬�����û����
                                    //00�������û����֪��
                                    //01������Ա����
                                    //02������ԱӢ��
                                    //03������Ա����
                                    //04������Ա����
                                    //05������Ա��������
                                    //06������Ա��˫��Э�̲��õ����ԣ����
                                    //07������Ա��˫��Э�̲��õ�����
                                    //08������Ա��˫��Э�̲��õ����ԣ����
                                    //09�����ڻ���Ա
                                    //0A����ͨ�û�����-������-�м��ã�
                                    //0B�������û�����-������-�У���-�м��ã�
                                    //0C�����ݺ��У��������ݣ�
                                    //0D�����Ժ���
                                    //0E������
                                    //0F�����ѵ绰
                                    //10~BF������
                                    //E0~EF�����ڱ���
                                    //F0����ͨ����ѣ���-�����ã�
                                    //F1����ͨ���ڣ���-�����ã�
                                    //F2����ͨ�û�����������-�����ã�
                                    //F3����ͨ��ӡ����������-�����ã�
                                    //F4��������ѣ���-�����ã�
                                    //F5�����ȶ��ڣ���-�����ã�
    unsigned char ValidFlg : 1;     //��¼��Ч��ָʾ�������ƣ�0���ü�¼��Ч 1���ü�¼��Ч
    unsigned char TimerTickFlg : 1; //ʱ�ӱ仯��ʶ��0�����й�����ʱ�ӷ����˱仯 1�����й�����ʱ��û�з����仯
    unsigned char FreeFlg : 1;      //�Ʒ�/��ѱ�ʶ�������ƣ�0���Ʒ� 1�����
    unsigned char TryCallFlg : 1;   //�Ժ���� �����ã��������ƣ�0���Ժ���� 1���Ժ��Ʒ�

    unsigned char IncomingCTX[2];   //���м�Ⱥ��ʶ��0~9999����BCD��
    unsigned char OutgoingCTX[2];   //���м�Ⱥ��ʶ��0~9999����BCD��
    unsigned char CallToSS[7];      //�����е���SS��������
    unsigned char CallingType[1];   //�Ʒѷ���ݱ�ʶ
                                    //1���������û��Ʒ�
                                    //2���Ա����û��Ʒ�
                                    //3����Ŀ���ַ����Ʒ�
                                    //127���Թ涨�ļƷѺ���Ʒ�
    unsigned char RelatedNumExp[1]; //���Ӻ����ַ���ʱ�ʾ�BCD���Ҷ���
    unsigned char RelatedNum[10];   //���Ӻ��룬BCD�������
    unsigned char Fee[4];           //���ã�BCD��
    unsigned char Reserved[4];      //����
};

//S1240����ISDN�굥�Ļ�����ʽ
struct S1240GBISDNDetailSrcFmt
{
    unsigned char RecordType : 4;   //BCD����ʾ�û�����¼�ĺ��е�����
                                    //1�� PSTN IDD/DDD���м�¼
                                    //2�� DBO���м�¼
                                    //3�� IN���м�¼
                                    //4�� ISDN/CTX��¼
                                    //5�� TAX������¼
                                    //6-7������
    unsigned char PartialFlg : 4;   //BCD����ʾ�û����ǵ����Ļ�������һ���������е�һ����
                                    //0�� ������¼
                                    //1�� ��¼�ĵ�һ����
                                    //2�� ��¼���м䲿��
                                    //3�� ��¼����󲿷�
    unsigned char SeqNum[4];        //˳���
    unsigned char CallerLocAttr[1]; //��ʾ�����û��ĵ�ַ����,BCD���Ҷ���
                                    //0�� �û�����
                                    //1�� ����
                                    //2�� ������Ч����
                                    //3�� ���ʺ���
    unsigned char CallerNum[10];    //���к��룬BCD������룬����λ��16���Ƶ�E���
    unsigned char CalledLocAttr[1]; //��ʾ�����û��ĵ�ַ����
    unsigned char CalledNum[10];    //���к��룬BCD������룬����λ��16���Ƶ�E���
    unsigned char StartYear[1];     //��ʼ�꣬BCD��
    unsigned char StartMonth[1];    //��ʼ�£�BCD��
    unsigned char StartDay[1];      //��ʼ�գ�BCD��
    unsigned char StartHour[1];     //��ʼʱ��BCD��
    unsigned char StartMinute[1];   //��ʼ�֣�BCD��
    unsigned char StartSecond[1];   //��ʼ�룬BCD��
    unsigned char StartTSecond : 4; //��ʼʮ��֮һ�룬BCD��
    unsigned char CallType : 4;     //��ʾ��������
                                    //0~1������
                                    //2������ת��
                                    //3�����غ���
                                    //4���豸���У�facility call��
                                    //5�����ڷ��������˳�;�֣�
                                    //6�����ʷ��������ʺͷ��˳�;�֣�
                                    //7�������������ն˳�;�֣�
                                    //8������������������ھ֣�
                                    //9��ת��������/��;�֣��������ڹ���ת����
    unsigned char EndYear[1];       //��ֹ�꣬BCD��
    unsigned char EndMonth[1];      //��ֹ�£�BCD��
    unsigned char EndDay[1];        //��ֹ�գ�BCD��
    unsigned char EndHour[1];       //��ֹʱ��BCD��
    unsigned char EndMinute[1];     //��ֹ�֣�BCD��
    unsigned char EndSecond[1];     //��ֹ�룬BCD��
    unsigned char EndTSecond : 4;   //��ֹʮ��֮һ�룬BCD��
    unsigned char TermCause : 4;    //BCD�룬��ʾ���н�����ԭ��
                                    //0�������û��һ�
                                    //1�������û��һ�
                                    //2���쳣����
                                    //3��������
    unsigned char Duration[4];      //ͨ��ʱ����BCD��
    unsigned char CallerType1[1];
    unsigned char CallerType2 : 4;  //BCD�룬�����û����
                                    //00�������û����֪��
                                    //01������Ա����
                                    //02������ԱӢ��
                                    //03������Ա����
                                    //04������Ա����
                                    //05������Ա��������
                                    //06������Ա��˫��Э�̲��õ����ԣ����
                                    //07������Ա��˫��Э�̲��õ�����
                                    //08������Ա��˫��Э�̲��õ����ԣ����
                                    //09�����ڻ���Ա
                                    //0A����ͨ�û�����-������-�м��ã�
                                    //0B�������û�����-������-�У���-�м��ã�
                                    //0C�����ݺ��У��������ݣ�
                                    //0D�����Ժ���
                                    //0E������
                                    //0F�����ѵ绰
                                    //10~BF������
                                    //E0~EF�����ڱ���
                                    //F0����ͨ����ѣ���-�����ã�
                                    //F1����ͨ���ڣ���-�����ã�
                                    //F2����ͨ�û�����������-�����ã�
                                    //F3����ͨ��ӡ����������-�����ã�
                                    //F4��������ѣ���-�����ã�
                                    //F5�����ȶ��ڣ���-�����ã�
    unsigned char ValidFlg : 1;     //��¼��Ч��ָʾ�������ƣ�0���ü�¼��Ч 1���ü�¼��Ч
    unsigned char TimerTickFlg : 1; //ʱ�ӱ仯��ʶ��0�����й�����ʱ�ӷ����˱仯 1�����й�����ʱ��û�з����仯
    unsigned char FreeFlg : 1;      //�Ʒ�/��ѱ�ʶ�������ƣ�0���Ʒ� 1�����
    unsigned char TryCallFlg : 1;   //�Ժ���� �����ã��������ƣ�0���Ժ���� 1���Ժ��Ʒ�
    unsigned char IncomingCTX[2];   //���м�Ⱥ��ʶ��0~9999����BCD��
    unsigned char OutgoingCTX[2];   //���м�Ⱥ��ʶ��0~9999����BCD��
    unsigned char CallToSS[7];      //�����е���SS��������
    unsigned char CallingType[1];   //�Ʒѷ���ݱ�ʶ
                                    //1���������û��Ʒ�
                                    //2���Ա����û��Ʒ�
                                    //3����Ŀ���ַ����Ʒ�
                                    //127���Թ涨�ļƷѺ���Ʒ�
    unsigned char RelatedNumExp[1]; //���Ӻ����ַ���ʱ�ʾ�BCD���Ҷ���
    unsigned char RelatedNum[10];   //���Ӻ��룬BCD�������
    unsigned char Fee[4];           //���ã�BCD��
    unsigned char BearingService[1];//BCD������ҵ��0~99
                                    //1����·ģʽ��64kbit/s�����ޣ�8kHz�ṹ������ҵ��
                                    //2����·ģʽ��64kbit/s,8kHz�ṹ����������
                                    //3����·ģʽ��64kbit/s,8kHz�ṹ������3.1kHz����
                                    //4������ģʽ��ISDN����У��������·����ҵ����Bͨ���ṩ���û�����
                                    //5���û��������ҵ��
                                    //7����·ģʽ��2*64kbit/s�����ޣ�8kHz�ṹ������ҵ�����
                                    //8����·ģʽ��384kbit/s�����ޣ�8kHz�ṹ������ҵ�����
                                    //9����·ģʽ��1536kbit/s�����ޣ�8kHz�ṹ������ҵ�����
                                    //10����·ģʽ��1920kbit/s�����ޣ� 8 kHz�ṹ������ҵ�����
    unsigned char TerminalServ : 4; //�ն�ҵ��BCD:0~99
                                    //0������
                                    //1��3.1kHz����ҵ��
                                    //2��7kHz����ҵ��
                                    //3��4�ഫ��
                                    //4�������û��籨
                                    //5������ͼ��
                                    //6������͵���ҵ��
                                    //7��7kHzͼ��
    unsigned char TryCallPulse : 4; //���ã��Ժ����壩
    unsigned char UUS1Count[1];     //UUS1������������
    unsigned char UUS2Count[1];     //UUS2������������
    unsigned char CallerSpecNum[5]; //���з�ר�ú���,BCD�������
    unsigned char CalledSpecNum[5]; //���з�ר�ú���,BCD�������
    unsigned char CTXLabel[2];      //CTX��ʶ��������
    unsigned char AccountProp[1];   //�ƷѺ����ַ���ʱ�ʾ��,BCD���Ҷ���
                                    //0���û�����
                                    //1�����У����ڱ��ã�
                                    //2�����ں���
                                    //3�����ʺ���
                                    //4�����ʿ�A�࿨
                                    //5�����ʿ�B�࿨
                                    //6�����ʿ�C�࿨
                                    //7�����ʿ�D�࿨
                                    //8��VISA��
                                    //9��VPNȺ��
                                    //10��VPN�ֻ���
                                    //12�����������ʺ�
                                    //13�����������ʺ�
                                    //14���й������ʺ�
                                    //15����ͨ�����ʺ�
                                    //16��ũҵ�����ʺ�
                                    //����������
    unsigned char AccountNum[11];   //�ƷѺ���,BCD�������
    unsigned char Reserved[4];      //����
};

//S1240����IN�굥�Ļ�����ʽ
struct S1240GBINDetailSrcFmt
{
    unsigned char RecordType : 4;   //BCD����ʾ�û�����¼�ĺ��е�����
                                    //1�� PSTN IDD/DDD���м�¼
                                    //2�� DBO���м�¼
                                    //3�� IN���м�¼
                                    //4�� ISDN/CTX��¼
                                    //5�� TAX������¼
                                    //6-7������
    unsigned char PartialFlg : 4;   //BCD����ʾ�û����ǵ����Ļ�������һ���������е�һ����
                                    //0�� ������¼
                                    //1�� ��¼�ĵ�һ����
                                    //2�� ��¼���м䲿��
                                    //3�� ��¼����󲿷�
    unsigned char SeqNum[4];        //˳���
    unsigned char CallerLocAttr[1]; //��ʾ�����û��ĵ�ַ����
                                    //0�� �û�����
                                    //1�� ����
                                    //2�� ������Ч����
                                    //3�� ���ʺ���
    unsigned char CallerNum[10];    //���к��룬BCD������룬����λ��16���Ƶ�E���
    unsigned char CalledLocAttr[1]; //��ʾ�����û��ĵ�ַ����
    unsigned char CalledNum[10];    //���к��룬BCD������룬����λ��16���Ƶ�E���
    unsigned char StartYear[1];     //��ʼ�꣬BCD��
    unsigned char StartMonth[1];    //��ʼ�£�BCD��
    unsigned char StartDay[1];      //��ʼ�գ�BCD��
    unsigned char StartHour[1];     //��ʼʱ��BCD��
    unsigned char StartMinute[1];   //��ʼ�֣�BCD��
    unsigned char StartSecond[1];   //��ʼ�룬BCD��
    unsigned char StartTSecond : 4; //��ʼʮ��֮һ�룬BCD��
    unsigned char CallType : 4;     //��ʾ��������
                                    //0~1������
                                    //2������ת��
                                    //3�����غ���
                                    //4���豸���У�facility call��
                                    //5�����ڷ��������˳�;�֣�
                                    //6�����ʷ��������ʺͷ��˳�;�֣�
                                    //7�������������ն˳�;�֣�
                                    //8������������������ھ֣�
                                    //9��ת��������/��;�֣��������ڹ���ת����
    unsigned char EndYear[1];       //��ֹ�꣬BCD��
    unsigned char EndMonth[1];      //��ֹ�£�BCD��
    unsigned char EndDay[1];        //��ֹ�գ�BCD��
    unsigned char EndHour[1];       //��ֹʱ��BCD��
    unsigned char EndMinute[1];     //��ֹ�֣�BCD��
    unsigned char EndSecond[1];     //��ֹ�룬BCD��
    unsigned char EndTSecond : 4;   //��ֹʮ��֮һ�룬BCD��
    unsigned char TermCause : 4;    //BCD�룬��ʾ���н�����ԭ��
                                    //0�������û��һ�
                                    //1�������û��һ�
                                    //2���쳣����
                                    //3��������
    unsigned char Duration[4];      //ͨ��ʱ����BCD��
    unsigned char CallerType1[1];
    unsigned char CallerType2 : 4;  //BCD�룬�����û����
                                    //00�������û����֪��
                                    //01������Ա����
                                    //02������ԱӢ��
                                    //03������Ա����
                                    //04������Ա����
                                    //05������Ա��������
                                    //06������Ա��˫��Э�̲��õ����ԣ����
                                    //07������Ա��˫��Э�̲��õ�����
                                    //08������Ա��˫��Э�̲��õ����ԣ����
                                    //09�����ڻ���Ա
                                    //0A����ͨ�û�����-������-�м��ã�
                                    //0B�������û�����-������-�У���-�м��ã�
                                    //0C�����ݺ��У��������ݣ�
                                    //0D�����Ժ���
                                    //0E������
                                    //0F�����ѵ绰
                                    //10~BF������
                                    //E0~EF�����ڱ���
                                    //F0����ͨ����ѣ���-�����ã�
                                    //F1����ͨ���ڣ���-�����ã�
                                    //F2����ͨ�û�����������-�����ã�
                                    //F3����ͨ��ӡ����������-�����ã�
                                    //F4��������ѣ���-�����ã�
                                    //F5�����ȶ��ڣ���-�����ã�
    unsigned char ValidFlg : 1;     //��¼��Ч��ָʾ�������ƣ�0���ü�¼��Ч 1���ü�¼��Ч
    unsigned char TimerTickFlg : 1; //ʱ�ӱ仯��ʶ��0�����й�����ʱ�ӷ����˱仯 1�����й�����ʱ��û�з����仯
    unsigned char FreeFlg : 1;      //�Ʒ�/��ѱ�ʶ�������ƣ�0���Ʒ� 1�����
    unsigned char TryCallFlg : 1;   //�Ժ���� �����ã��������ƣ�0���Ժ���� 1���Ժ��Ʒ�
    unsigned char IncomingCTX[2];   //���м�Ⱥ��ʶ��0~9999����BCD��
    unsigned char OutgoingCTX[2];   //���м�Ⱥ��ʶ��0~9999����BCD��
    unsigned char CallToSS[7];      //�����е���SS��������
    unsigned char CallingType[1];   //�Ʒѷ���ݱ�ʶ
                                    //1���������û��Ʒ�
                                    //2���Ա����û��Ʒ�
                                    //3����Ŀ���ַ����Ʒ�
                                    //127���Թ涨�ļƷѺ���Ʒ�
    unsigned char Fee[4];           //���ã�BCD��
    unsigned char BearingService[1];//BCD������ҵ��0~99
                                    //1����·ģʽ��64kbit/s�����ޣ�8kHz�ṹ������ҵ��
                                    //2����·ģʽ��64kbit/s,8kHz�ṹ����������
                                    //3����·ģʽ��64kbit/s,8kHz�ṹ������3.1kHz����
                                    //4������ģʽ��ISDN����У��������·����ҵ����Bͨ���ṩ���û�����
                                    //5���û��������ҵ��
                                    //7����·ģʽ��2*64kbit/s�����ޣ�8kHz�ṹ������ҵ�����
                                    //8����·ģʽ��384kbit/s�����ޣ�8kHz�ṹ������ҵ�����
                                    //9����·ģʽ��1536kbit/s�����ޣ�8kHz�ṹ������ҵ�����
                                    //10����·ģʽ��1920kbit/s�����ޣ� 8 kHz�ṹ������ҵ�����
    unsigned char TerminalServ : 4; //�ն�ҵ��BCD:0~99
                                    //0������
                                    //1��3.1kHz����ҵ��
                                    //2��7kHz����ҵ��
                                    //3��4�ഫ��
                                    //4�������û��籨
                                    //5������ͼ��
                                    //6������͵���ҵ��
                                    //7��7kHzͼ��
    unsigned char TryCallPulse : 4; //���ã��Ժ����壩
    unsigned char UUS1Count[1];     //UUS1������������
    unsigned char UUS2Count[1];     //UUS2������������
    unsigned char CallerSpecNum[5]; //���з�ר�ú���,BCD�������
    unsigned char CalledSpecNum[5]; //���з�ר�ú���,BCD�������
    unsigned char CTXLabel[2];      //CTX��ʶ��������
    unsigned char AccountProp[1];   //�ƷѺ����ַ���ʱ�ʾ��,BCD���Ҷ���
                                    //0���û�����
                                    //1�����У����ڱ��ã�
                                    //2�����ں���
                                    //3�����ʺ���
                                    //4�����ʿ�A�࿨
                                    //5�����ʿ�B�࿨
                                    //6�����ʿ�C�࿨
                                    //7�����ʿ�D�࿨
                                    //8��VISA��
                                    //9��VPNȺ��
                                    //10��VPN�ֻ���
                                    //12�����������ʺ�
                                    //13�����������ʺ�
                                    //14���й������ʺ�
                                    //15����ͨ�����ʺ�
                                    //16��ũҵ�����ʺ�
                                    //����������
    unsigned char AccountNum[11];   //�ƷѺ���,BCD�������
    unsigned char TransNumExp[1];   //��������ַ���ʱ�ʾ��,BCD���Ҷ���
                                    //0���û����� 1������ 2��������Ч���� 3�����ʺ���
    unsigned char TransNum[10];     //�������,BCD�������
    unsigned char PosiNumExp[1];    //λ�ú����ַ���ʱ�ʾ��,BCD���Ҷ���
                                    //0���û����� 1������ 2��������Ч���� 3�����ʺ���
    unsigned char PosiNum[10];      //λ�ú���,BCD�������
    unsigned char ChargeRateType[2];//�Ʒ�������,BCD
    unsigned char ChargeRate1[1];   //�Ʒѵ���ϵ����BCD ���� 1���ֽڱ�ʾ�Ʒѵ�����0~255 ����ֽڼƷѵ�������1=���ʣ�2=����
    unsigned char ChargeRate2 : 4;
    unsigned char Reserved1 : 4;    //����
    unsigned char INAddition[3];    //IN���ӷѣ�BCD ����2���ֽڱ�ʾ���ӷ�ֵ 1���ֽڱ�ʾ���ӷ����ͣ�1=����Ǯ�� 2��ʾ���ӷ���
    unsigned char AddedPara[20];    //͸������,��ʾINҵ���е��õ�ҵ������
    unsigned char Reserved[20];     //����
};

//�ƴα��¼��Դ��ʽ
struct S1240GBMeterSrcFmt
{
    unsigned char RecordType : 4;   //BCD��0����ʾ�ƴλ��� 1-9������
    unsigned char PulseNum : 4;     //BCD����ʾ������ʹ�õ��������
    unsigned char SeqNum[4];        //˳��ţ�������
    unsigned char SwitchLabel[4];   //��������OPC��ʶ��������
    unsigned char StartYear[2];     //��ʼ�꣬BCD��
    unsigned char StartMonth[1];    //��ʼ�£�BCD��
    unsigned char StartDay[1];      //��ʼ�գ�BCD��
    unsigned char StartHour[1];     //��ʼʱ��BCD��
    unsigned char StartMinute[1];   //��ʼ�֣�BCD��
    unsigned char StartSecond[1];   //��ʼ�룬BCD��
    unsigned char CallerZone[4];    //�������ţ�BCD������룬���к������ڱ����������ţ�
                                    //��ϵͳ�漰�������ϵı�����Ӧ��ʱ��
                                    //��ͨ�����������ֲ�ͬ�������Ļ���
    unsigned char CallerNum[6];     //���к��룬BCD������룬����λ��16���Ƶ�E���

    unsigned char Meter1[4];
    unsigned char Meter2[4];
    unsigned char Meter3[4];
    unsigned char Meter4[4];
    unsigned char Meter5[4];
    unsigned char Reserved[14];
};

//���ͷ��¼
struct S1240GBCDRBlockHead
{
    /*
        ���ݿ�ϵ�кţ�������һ���ļ��ڡ�����ÿ�δ���һ�����ļ�ʱ����ϵ�кű���λΪ0��
        �����Ʊ��롣ȡֵ��ΧΪ0ȫ32767
    */
    unsigned char   BlockCsn[2];

    /*
        ��ʼ���ڣ�ʱ�䣺������������ļƷ��ļ������壬�ҽ�����ÿ���ļ��ĵ�һ�����ݿ��С�
        �����������ݿ���ʮ������"EEEEEEEEEEEE"��䡣���ö����Ʊ��룬���ʽΪYYYYMMDDHHMM��
        YYYY��ʾ��ݣ�ȡֵ1980-H' FFFF��MM��ʾ�£�ȡֵ01-12��DD��ʾ�գ�ȡֵ01��31��HH��ʾСʱ��
        ȡֵ00һ23��MM��ʾ���ӣ�ȡֵ00һ59
    */
    unsigned char   StartYear[2];       //��ʼ�꣬������
    unsigned char   StartMonth[1];      //��ʼ�£�������
    unsigned char   StartDay[1];        //��ʼ�գ�������
    unsigned char   StartHour[1];       //��ʼʱ��������
    unsigned char   StartMinute[1];     //��ʼ�֣�������

    /*
        �ļ����ͣ��������Ϊһ���ļ����ͣ�����ϸ����� ASCll��'D'��ʾ
    */
    unsigned char   FileType[1];        //�ļ�����

    /*
        ������OPC��ָ���������Լ��� OPC��������������ļƷ��ļ������塣
        �������ÿ���ļ��ĵ�һ�����ݿ��У������������ݿ���ʮ������"EEEEEE"��䡣
        H���Ʊ��룬ȡֵ��ΧΪ0-16777215
    */
    unsigned char   OPC[3];

    /*
        ���ݼ�¼���ͣ�������������ļƷ��ļ������壬����ָ���ļ������ݼ�¼�����ͣ�
        ȡֵͬ��1�еļ�¼���ͣ��ö����ƴ����ʾ
    */
    unsigned char   RecordType[1];

    /*
        ʵ�ʿ鳤�ȣ�������������ļƷ��ļ������壬����ָ���ڸ����ݿ���ʵ����Ч���ݵ���������
        ����20���ֽڵ�ͷ��¼�������������ݿ�β������䲿�֡������Ʊ��룬ȡֵ��Χ0��2048
    */
    unsigned char   BlockLen[2];

    /*
        �Ʒ���ϵͳ��ʶ����������������ļƷ��ļ������壬����ÿ���ļ��ĵ�һ�����ݿ��в�ʹ�������
        ���������ݿ������ʮ������"EE"�������Ʊ��룬ȡֵ��ΧΪ0-255
    */
    unsigned char   AccountLabel[1];

    /*
        �ļ�ϵ�кţ�������������ļƷ��ļ������塣����ÿ���ļ��ĵ�һ�����ݿ��в�ʹ�������
        ���������ݿ������ʮ������"EEEEEEEE"�������Ʊ���
    */
    unsigned char   FileSN[4];
};

//��¼ͷ
struct S1240GBCDRRecordHead
{
    unsigned char RecordType : 4;   //BCD����ʾ�û�����¼�ĺ��е�����
                                    //0:  �ƴα�
                                    //1�� PSTN IDD/DDD���м�¼
                                    //2�� DBO���м�¼
                                    //3�� IN���м�¼
                                    //4�� ISDN/CTX��¼
                                    //5�� TAX������¼
                                    //6-7������
    unsigned char PartialFlg : 4;

    unsigned char SeqNum[4];        //˳��ţ�������

};





int ProcessS1240GBFile(const char*   pszSrcFile,
                       VECTOR<STRING>& FinalFileNameList,
                       VECTOR<int>&    CvtResultList,
                       const int bFileLevelVerify,
                       const int bBillLevelVerify,
                       void* pIBillProcessor,
                       const int nOrigBillLen)
{
    IBillProcessor* pBillProcessor = (IBillProcessor*)pIBillProcessor;

    FILE* pSrcFile = fopen(pszSrcFile, "rb");
    if (NULL == pSrcFile)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�򿪻����ļ�(%s)ʧ��",
               pszSrcFile);

        WRITELOG_F("�򿪻����ļ�(%s)ʧ��",
                   pszSrcFile);

        return ERR_FAIL;
    }

    VECTOR<FILE*> ChlFiles;
    VECTOR<STRING>::iterator item = FinalFileNameList.begin();

    for (; item != FinalFileNameList.end(); item++)
    {
        ChlFiles.push_back(NULL);
    }

    //�����ļ���У�顣
    long nSrcFileSize = FileSize(pSrcFile);

    if (bFileLevelVerify)
    {
        if (nSrcFileSize % 2048 != 0)
        {
            if (!FileCopy(pszSrcFile, FinalFileNameList[0].c_str()))
            {
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "�����ļ�(%s)�����������ļ�(%s)ʧ��",
                       pszSrcFile,
                       FinalFileNameList[0].c_str());

                WRITELOG_F("�����ļ�(%s)�����������ļ�(%s)ʧ��",
                           pszSrcFile,
                           FinalFileNameList[0].c_str());
            }
            else
            {
                CvtResultList[0] = 1;
            }

            fclose(pSrcFile);
            return ERR_FAIL;
        }
    }

    /*
     * ��ʼ���л���ת����
     */
    long nBlockNum = (nSrcFileSize + 2048 - 1) / 2048;

    char buf[2048];
    S1240GBCDRBlockHead* CDRBlock = (S1240GBCDRBlockHead*)buf;

    int nResult = ERR_SUCCESS;
    int nBlockIndex = 0;
    short nBlockLen = 0;
    int nSeqNum = -1;
    int nRet = ERR_SUCCESS;
    while (nBlockNum-- && (nRet == ERR_SUCCESS))
    {
        //��һ���������
        int nReadLen = fread(CDRBlock, 1, 2048, pSrcFile);
        if(nReadLen <= 0)
        {
            nBlockNum = 0;
            continue;
        }
        else if(nReadLen <= 20)
        {
            continue;
        }

        memcpy(&nBlockLen, &CDRBlock->BlockLen[0], 2);
        nBlockLen = NTOHS(nBlockLen);
        nBlockLen = min(nBlockLen, nReadLen);
        if(nBlockLen <= 20)
        {
            continue;
        }

        char invalid_flag[10];
        memset(&invalid_flag, 0xEE, min(nReadLen - 20, sizeof(invalid_flag)));
        if (memcmp(CDRBlock, &invalid_flag, sizeof(invalid_flag)) == 0)
        {
            continue;
        }

        int ReadByte    = 20;
        int RecordIndex = 0;
        int RecordTypeInHead  = ((S1240GBCDRRecordHead*)(&buf[20]))->RecordType;
        int RecordLenInHead   = 0;
        switch(RecordTypeInHead)
        {
        case 0: //1�� Meter���м�¼
            RecordLenInHead = 60;
            break;

        case 1: //1�� PSTN IDD/DDD���м�¼
            RecordLenInHead = 78;
            break;
        case 3: //3�� IN���м�¼
            RecordLenInHead = 160;
            break;
        case 4: //4�� ISDN/CTX��¼
            RecordLenInHead = 106;
            break;
        default:
            continue;
        }
        while (ReadByte <= nBlockLen - 1)//��С�Ļ�������Ϊ60�ֽ�
        {
            int RecordType = buf[ReadByte] >> 4;
            int RecordLen  = 0;
            switch(RecordType)
            {
            case 0: //1�� Meter���м�¼
                RecordLen = 60;
                break;

            case 1: //1�� PSTN IDD/DDD���м�¼
                RecordLen = 78;
                break;

            case 3: //3�� IN���м�¼
                RecordLen = 160;
                break;

            case 4: //4�� ISDN/CTX��¼
                RecordLen = 106;
                break;

            default:
                RecordLen = RecordLenInHead;
                break;
            }

            if (memcmp(&buf[ReadByte], &invalid_flag, min(nReadLen - 20, sizeof(invalid_flag))) == 0)
            {
                ReadByte += RecordLen;
                continue;
            }
            S1240GBCDRRecordHead* pRecordHead = (S1240GBCDRRecordHead*)&buf[ReadByte];
/*
            if(RecordType != pRecordHead->RecordType)
            {
                if (bBillLevelVerify)
                {
                    if (ChlFiles[0] == NULL)
                    {
                        ChlFiles[0] = fopen(FinalFileNameList[0].c_str(), "wb");
                        if (ChlFiles[0] == NULL)
                        {
                            COMSGOUT(MSGOUT_LEVEL_URGENT,
                                   "���ļ�(%s)ʧ��",
                                   FinalFileNameList[0].c_str());

                            WRITELOG_F("���ļ�(%s)ʧ��",
                                       FinalFileNameList[0].c_str());
                        }
                        else
                        {
                            fwrite(&buf[ReadByte], RecordLen, 1, ChlFiles[0]);
                            CvtResultList[0] = 1;
                        }
                    }
                    else
                    {
                        fwrite(&buf[ReadByte], RecordLen, 1, ChlFiles[0]);
                        CvtResultList[0] = 1;
                    }
                }

                ReadByte += RecordLen;
                continue;
            }
*/
            if(ReadByte <= nBlockLen - 5)
            {
                //��黰�����к�����
                int nCurSeqNum = 0;
                memcpy(&nCurSeqNum, &pRecordHead->SeqNum[0], sizeof(pRecordHead->SeqNum));
                nCurSeqNum = NTOHL(nCurSeqNum);

                if(nSeqNum != -1)
                {
                    if(nCurSeqNum != nSeqNum + 1 && bBillLevelVerify)
                    {
                        COMSGOUT(MSGOUT_LEVEL_URGENT,
                               "�����ļ�(%s)�л������кŲ�����(%d-%d)",
                               pszSrcFile,
                               nSeqNum,
                               nCurSeqNum);
                        WRITELOG_F("�����ļ�(%s)�л������кŲ�����(%d-%d)",
                                   pszSrcFile,
                                   nSeqNum,
                                   nCurSeqNum);
                    }
                }
                nSeqNum = nCurSeqNum;
            }


            //�ø�ʽ�����ת��
            UINT4 uChlID;
            CFormat* pDestFmt = NULL;
            void* pDestBill = NULL;
            UINT4 uLen;
            int nReadRecordLen = min(RecordLen, nBlockLen - ReadByte);

            //��ȡ�ּ�ͨ���ź�ת����ʽ���󣬲����л����Ĵ���
            pBillProcessor->Distribute(&buf[ReadByte],
                                       nReadRecordLen,
                                       uChlID,
                                       pDestFmt);

            if ((pDestFmt != NULL) && (uChlID != 0))
            {
                nResult = pDestFmt->ConvertABill(&buf[ReadByte],
                                                 nReadRecordLen,
                                                 pDestBill, uLen);
                if (ERR_SUCCESS == nResult)
                {
                    if (ChlFiles[uChlID] == NULL)
                    {
                        FILE* pFile = fopen(FinalFileNameList[uChlID].c_str(), "wb");
                        if (pFile == NULL)
                        {
                            COMSGOUT(MSGOUT_LEVEL_URGENT,
                                   "�����ջ����ļ�(%s)ʧ��",
                                   FinalFileNameList[uChlID].c_str());

                            WRITELOG_F("�����ջ����ļ�(%s)ʧ��",
                                       FinalFileNameList[uChlID].c_str());
                            delete pDestBill;
                            nRet = ERR_FAIL;
                            break;
                        }

                        ChlFiles[uChlID] = pFile;
                    }

                    fwrite(pDestBill, uLen, 1, ChlFiles[uChlID]);
                    CvtResultList[uChlID] = 1;

                    delete pDestBill;
                }
            }
            else
            {
                nResult = ERR_FAIL;
            }

            //���ת�����ɹ��������󻰵�д������ļ�
            if (ERR_SUCCESS != nResult)
            {
                if (ChlFiles[0] == NULL)
                {
                    ChlFiles[0] = fopen(FinalFileNameList[0].c_str(), "wb");
                    if (ChlFiles[0] == NULL)
                    {
                        COMSGOUT(MSGOUT_LEVEL_URGENT,
                               "���ļ�(%s)ʧ��",
                               FinalFileNameList[0].c_str());

                        WRITELOG_F("���ļ�(%s)ʧ��",
                                   FinalFileNameList[0].c_str());
                        nRet = ERR_FAIL;
                        break;
                    }
                }

                fwrite(&buf[ReadByte], nReadRecordLen, 1, ChlFiles[0]);
                CvtResultList[0] = 1;
            }

            ReadByte += nReadRecordLen;
        }

    }

    fclose(pSrcFile);
    pSrcFile = NULL;

    for(int j = 0; j < ChlFiles.size(); j++)
    {
        if (ChlFiles[j] != NULL)
        {
            fclose(ChlFiles[j]);
            ChlFiles[j] = NULL;
        }
    }

    return nRet;
}

