   #ifndef __COM_DIR_COLLECT_CONVERT_H__
#define __COM_DIR_COLLECT_CONVERT_H__

/*
CComDirCollectConvert������Com�ɼ�������صĻ������Ժͻ�������
*/
#include "../collect_convert_subobj.h"

//�����ǲɼ���������CComDirCollectConvert�Ķ���
class  CComDirCollectConvert : public CCollectConvertSubObj
{

public:
    CComDirCollectConvert(int nSwitchIndex, 
                          int nDirIndex,
                          IBillProcessor* pIBillProcessor = NULL); //���캯������Э�����ָ��
    virtual ~CComDirCollectConvert();

    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Init() ;

};

//�����ǲɼ���������CTellinDirConvert�Ķ���
class  CTellinDirConvert : public CComDirCollectConvert
{

public:
    CTellinDirConvert(int nSwitchIndex, 
                      int nDirIndex,
                      IBillProcessor* pIBillProcessor = NULL); //���캯������Э�����ָ��
    virtual ~CTellinDirConvert();

    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Init() ;
    
    //����Ŀ���ļ���·��
    virtual void ConstuctDestFileLst(SConvertFileReq& stReq);

    //�����ڶ����ļ�
    virtual int  CopySecondFile();

};

class  CSJZTellinDirConvert : public CTellinDirConvert
{
	
public:
    CSJZTellinDirConvert(int nSwitchIndex, 
		int nDirIndex,
		IBillProcessor* pIBillProcessor = NULL); //���캯������Э�����ָ��
    virtual ~CSJZTellinDirConvert();
	
    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Init() ;
    
    //����Ŀ���ļ���·��
    virtual void ConstuctDestFileLst(SConvertFileReq& stReq);
	virtual int  loadstatus(int nChannel);
	virtual int  SaveStatus(int nChlNo);

    virtual void OnFileConvertSucc(int nChannel);

    STRING GetFileCsnStr(UINT4 uCsn, int nChannel);

    //��ʽ���ַ���
    static STRING FormatCfgStr(const char* szFormat, 
                               const tm&   tmTime, 
                               const UINT4 uChannelID);

    virtual STRING GetBillFileInfo(const char* szFilePath, int nChannel);
    
    virtual BOOL IsRepeatLog(const char* szLogMsg, FILE* pFile);
    //���������־
    BOOL SaveCheckLog(const char* szLogMsg, int nChannel);

public:
    struct SChlInfo 
    {
        int    nCsnLen;          //�����ļ��������кų���
        UINT4  uFileCsnStart;    //�����ļ���ʼ���к�
        UINT4  uFileCsnEnd;      //�����ļ�������к�
        UINT4  uFileCsnStep;     //�����ļ����кŵ�������
        char   szFormalPrefix[512];        //��ʽ�ļ���ǰ׺
        char   szFormalMidfix[512];        //��ʽ�ļ�����׺
        char   szFormalPostfix[512];       //��ʽ�ļ��ĺ�׺
        time_t tFileCreateTime;            //�ļ�����ʱ��

        char   szStatusFile[MAX_PATH];     //״̬�ļ�·��
        int    uCurCsn;                    //�ļ����к�

        BOOL   bCheckLog;                  //�Ƿ�����������־����
        char   szCheckedLogPath[MAX_PATH]; //������־·��
        char   szCheckLogPrefix[512];      //������־�ļ���ǰ׺
        char   szCheckLogPostfix[512];     //������־�ļ��ĺ�׺
        int    nBillLen;                   //���ջ�������

        
        SChlInfo()
        {
            memset(this, 0, sizeof(*this));
        }
    };

protected:
	VECTOR<SChlInfo>  m_ChlInfoList;          //ͨ����Ϣ�б�

};

//�����ǲɼ���������CMeterSumDirConvert�Ķ���
class  CMeterSumDirConvert : public CComDirCollectConvert
{

public:
    CMeterSumDirConvert(int nSwitchIndex, 
                       int nDirIndex,
                       IBillProcessor* pIBillProcessor = NULL); //���캯������Э�����ָ��
    virtual ~CMeterSumDirConvert();

    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Init() ;

    //����Ŀ���ļ���·��
//    virtual void ConstuctDestFileLst(SConvertFileReq& stReq);

    //����ʱĿ¼�µ��ļ��ƶ�����ʽĿ¼��
    virtual int RenameDestFile();

    //�����ڶ����ļ�
    virtual int  CopySecondFile();

    //ת�������ļ�
    virtual int ConvertFile(SConvertFileReq& stReq);
protected:

    virtual BOOL isLeapYear(int nYear);
    virtual int GetDaysOfMonth(int nYear, int nMonth);
    virtual int GetLastDate(int& nYear, int& nMonth, int& nDay);
    virtual int GetLastMonth(int& nYear, int& nMonth, int& nDay);
    virtual void initialize(STRING& sTodayMeterFile,
                            STRING& sLastMeterFile,
                            STRING& sLastMonthMeterFile,
                            const char* pszSrcFile);

    virtual int Process_cc08_special_meter_file(const char* pszSrcFile,
                                                VECTOR<STRING>& FinalFileNameList,
                                                VECTOR<int>&    CvtResultList,
                                                const int bFileLevelVerify,
                                                const int bBillLevelVerify,
                                                void* pIBillProcessor = NULL,
                                                const int nOrigBillLen = 0);

    virtual int process_and_build_finalfile(const char* pszSrcFile, //����
                                            const char*   pszDestFile,//����
                                            VECTOR<STRING>& FinalFileNameList,
                                            VECTOR<int>&    CvtResultList,
                                            void* pIBillProcessor = NULL,
                                            const int nOrigBillLen = 0
                                            );
                                            
	virtual int Proce_Other_File(STRING& sSrcFile,
     				 			 VECTOR<STRING>& FinalFileNameList, 
    				 			 VECTOR<int>& CvtResultList,
    				 			 void* pIBillProcessor = NULL,
    				 			 const int	nOrigBillLen = 0);
    				 					                                              
    int m_BillDate,m_IsMonthBill,m_IsDayBill;
    unsigned long  m_nMeterUpLimit;
    struct SMetSumKey
    {
        BYTE byChgObj;
        char szCallNo[10];
        UINT2 uTrunkGrp;
        bool operator<(const struct SMetSumKey& right) const
        {
            char szLeft[13];
            memcpy(szLeft, &byChgObj, sizeof(byChgObj));
            memcpy(szLeft+1, szCallNo, sizeof(szCallNo));
            memcpy(szLeft+11, &uTrunkGrp, sizeof(uTrunkGrp));

            char szRight[13];
            memcpy(szRight, &right.byChgObj, sizeof(byChgObj));
            memcpy(szRight+1, right.szCallNo, sizeof(szCallNo));
            memcpy(szRight+11, &right.uTrunkGrp, sizeof(uTrunkGrp));
            return memcmp(szLeft,szRight,sizeof(szLeft)) < 0;
        }
    };
    
};


#endif //__COM_DIR_COLLECT_CONVERT_H__

