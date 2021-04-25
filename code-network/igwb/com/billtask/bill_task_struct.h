#ifndef __BILL_TASK_STRUCT_H__
#define __BILL_TASK_STRUCT_H__

//������������
enum TASK_TYPE
{
    TK_UNKNOWN = -1,        //δ֪����
    TK_SCP_RVY              //SCP�����ָ�����
};

#define SCP_QUERY_DST_FMT   "X3KF_In_BSCP_D"

//SCP�Ļ����ָ�����Ĺؼ�����
struct SSCPBillRcvrApply
{
	char szOp[17];		    //FTP�û���
	char szPwd[17];		    //FTP����
	char szDir[33];		    //�ļ�Ŀ¼	
	char szFileName[33];	//�ļ���
	char szIP[17];		    //FTP��IP��ַ
	int  nNI;               //����ʶ��0����; 1���ʱ���; 2����; 3���ڱ���
	char szDpc[7];		    //���ֶ�Ӧ��DPC��, 16���Ƶ��ַ���, ��"AAAAAA"
};

//���滰���ָ�������й���Ϣ�����ر���ʱ��
struct SRcvrCmd
{
	char* szCmdLine;	//MML���
	UINT2 uAppType;     //Ӧ������
	UINT2 uClientNo;    //ĳ�����͵�MML�ͻ��˵ı��
	UINT4 uSerialNo;	//������ˮ��
};

#define  MAX_NUM_LEN	22
//�쳣�����ṹ
struct SEcptBillRec
{
	int  nRecNo;		                //��ˮ��
	int  nServKey;		                //ҵ���
	char szCallingNum[MAX_NUM_LEN + 2];	//�����û�����
	char szTransNum[MAX_NUM_LEN + 2];	//Ŀ���ַ����
	char szDateTime[15];			    //�쳣��������ʼʱ�䴮
	int  nDecCost;				        //�쳣��������ֵ
	tm   tmStartTime;				    //��szDateTimeת����ֵ
};

//���ʱ�ʶ��������
struct SScpApplyKey
{
    int nNI;
    char szDpc[7];
    bool operator<(const struct SScpApplyKey& right) const
    {
        if(nNI == right.nNI)
        {
            return ACE_OS::strcasecmp(szDpc, right.szDpc) < 0; 
        }

        return (nNI < right.nNI);
    }
};

//�����롢ҵ���
struct SCodeKey
{
	char szAccCode[MAX_NUM_LEN + 1];
	int nServKey;
};

enum EBill_RVY_RETCODE
{	//�˷����붨���Ӧ��Դ�еĶ���, �������ģ�����ʱ������
	BR_SUCCESS   = 0,	    //�ɹ���������ϡ�
	BR_CODEBEGIN = 18540,
	BR_ACCEPTED  = 18540,	//ϵͳ���У����󱻽��ܡ�
	BR_PROCESSING,		    //���������ڴ���
	BR_DPCERR,			    //Ŀ������㲻�������󱻾ܾ���
	BR_PROCPREV,		    //���ڴ�����һ������������������󱻾ܾ���
	BR_SYSECPT,			    //ϵͳ����//�������

	BR_FTPCOMFAIL,		    //FTP COM��ʼ��ʧ�ܡ�
	BR_FTPGETFAIL,		    //FTP��SCPȡ�ļ�ʧ�ܡ�
	BR_WRITEFILEFAIL,	    //�쳣�����ļ�дʧ�ܡ�
	BR_FTPPUTFAIL,		    //FTP���ļ���SCPʧ�ܳ���n(3)�Ρ�
	
	BR_GETDPCFAIL,		    //ȡĿ�������ʧ�ܡ�
	BR_THDFAIL,			    //�����ָ������̴߳���ʧ�ܡ�
	
	BR_FTPERRBEGIN,
	BR_FTP_SUCCESS = BR_FTPERRBEGIN,	//�ļ�����ɹ���
	BR_FTP_WAIT,			//���ڽ��д���,���Ժ�(��ʼ��ʱʹ��)
	BR_FTP_CONNFAIL,		//���ӵ�FTP Serverʧ�ܡ�
	BR_FTP_FINDTRANEXCEP,	//���һ����ļ��쳣
	BR_FTP_FILENOTEXIST,	//�ļ������ڡ�
	BR_FTP_TRANNOFILE,		//�����ļ���δ����ɹ�
	BR_FTP_TRANSOMEFILE,	//�����ļ�δ����ɹ�
	BR_FTP_FAIL,			//FTP����ʧ�ܡ�//�뿪����Ա��ϵ
    
	BR_CODEKEYFAIL,		    //�����롢ҵ�������ʧ�ܡ�
	BR_ECPTFILEOPENFAIL,    //�쳣�����ļ��򿪴���
	BR_RCVRFILECRTFAIL,	    //�ָ������ļ�����ʧ�ܡ�
	BR_NOBILLRECMATCH,	    //û���ҵ�ƥ��Ļ����ɻָ���

	//���������Ƿ����룬��������LoadString
	BR_SUCCESSSTR,	        //success
	BR_RETCODESTR,	        //RETCODE = %d
	BR_DPCSTR,		        //\r\nDPC  =  %s
    
	BR_CODEEND
};

#endif //__BILL_TASK_STRUCT_H__
