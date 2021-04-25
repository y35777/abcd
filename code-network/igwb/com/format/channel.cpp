/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CChannel
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/

#include "format.h"
#include "dis_condition.h"
#include "format_lib.h"
#include "format_global.h"
#include "channel.h"
#include "../include/toolbox.h"

/************ The CChannel Defining ****************************/
// �������������캯��
//
// ���������
// sFmtRootDir����ʽ���Ŀ¼·��
//       uAPID����ͨ����Ӧ�Ľ����ID
//      uChlID����ͨ����Ӧ��ͨ��ID
//     pFmtLib����ʽ�����ָ��
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CChannel::CChannel(const STRING& sFmtRootDir, const UINT4 uAPID,
                   const UINT4 uChlID, CFormatLib *pFmtLib):
                   m_sFmtRootDir(sFmtRootDir), m_uAPID(uAPID),
                   m_uChlID(uChlID), m_pFmtLib(pFmtLib)
{
    //���ݲ�����ʼ����Ӧ��Ա����
    //m_sFmtRootDir = sFmtRootDir;
    //m_uAPID = uAPID;
    //m_uChlID = uChlID;
    //m_pFmtLib = (CFormatLib*)pFmtLib;

    //��ʼ������ָ���Ա����
    m_pDisCondition = NULL;
    m_pFinalFmtMaps = NULL;
    m_pFinalBrowseFmtMaps = NULL;

    m_pFinalFmtNameList = NULL;  //Added by ZhengYuqun 2004-01-18 SWPD03392
}
/************ End of The CChannel Defining *********************/


/************ The ~CChannel Defining ***************************/
// ������������������
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CChannel::~CChannel()
{
    //ɾ����ͨ����Ӧ�ķּ���������
    if(m_pDisCondition != NULL)
    {
        delete m_pDisCondition;
        m_pDisCondition = NULL;
    }

    FMT_MAP::iterator it;

    //ɾ����ͨ����Ӧ�����ջ�����ʽ�����б�
    if(m_pFinalFmtMaps != NULL)
    {
        it = m_pFinalFmtMaps->begin();
        while(it != m_pFinalFmtMaps->end())
        {
            delete it->second;  //ɾ���б��е�SFmtMap�ṹ
            it++;
        }
        delete m_pFinalFmtMaps;
        m_pFinalFmtMaps = NULL;
    }

    //ɾ����ͨ����Ӧ�����ջ��������ʽ�б�
    if(m_pFinalBrowseFmtMaps != NULL)
    {
        it = m_pFinalBrowseFmtMaps->begin();
        while(it != m_pFinalBrowseFmtMaps->end())
        {
            delete it->second;  //ɾ���б��е�SFmtMap�ṹ
            it++;
        }
        delete m_pFinalBrowseFmtMaps;
        m_pFinalBrowseFmtMaps = NULL;
    }

    //Added by ZhengYuqun 2004-01-18 SWPD03392
    if(m_pFinalFmtNameList != NULL)
    {
        delete m_pFinalFmtNameList;
        m_pFinalFmtNameList = NULL;
    }
}
/************ End of The ~CChannel Defining ********************/


/************ The Init Defining ********************************/
// ������������ʼ��
//
// ���������
//    pFmtLib����ʽ�����CChannel��Զ�����ͷŸ�ָ��
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CChannel::Init(CFormatLib *pFmtLib)
{
    //ȷ��m_pFmtLib����Ϊ��
    if((pFmtLib == NULL) && (m_pFmtLib == NULL))
    {
        return -1;
    }
    else if(pFmtLib != NULL)
    {
        m_pFmtLib = pFmtLib;
    }

    //ȱʡͨ�����޳�ʼ������
    if(m_uChlID == 0)
    {
        return 0;
    }

    //����ͨ������
    if(LoadConfig() != 0)
    {
        return -1;
    }

    CFormat* pFmt = NULL;
    FMT_MAP::iterator it;

    //��ʼ����ͨ����Ӧ�����ջ�����ʽ�����б�
    if(m_pFinalFmtMaps != NULL)
    {
        it = m_pFinalFmtMaps->begin();
        while(it != m_pFinalFmtMaps->end())
        {
			STRING sFmtName = (it->second)->sFmtName;
			int nCh = m_uChlID;
            pFmt = m_pFmtLib->GetFormat((it->second)->sFmtName, m_uAPID);
            if(pFmt == NULL)
            {
                TRACE(MTS_FORMAT, S_FMT_FINALBILL_FMT_NOT_FOUND, 
                      m_uAPID, m_uChlID, it->first.c_str());
                return -1;
            }
            (it->second)->pFmt = pFmt;

            it++;
        }
    }

    //��ʼ����ͨ����Ӧ�����ջ��������ʽ�б�
    if(m_pFinalBrowseFmtMaps != NULL)
    {
        it = m_pFinalBrowseFmtMaps->begin();
        while(it != m_pFinalBrowseFmtMaps->end())
        {
            pFmt = m_pFmtLib->GetFormat((it->second)->sFmtName, m_uAPID);
            if(pFmt == NULL)
            {
                TRACE(MTS_FORMAT, S_FMT_FINALBILL_BRW_FMT_NOT_FOUND, 
                      m_uAPID, m_uChlID, it->first.c_str());
                return -1;
            }
            (it->second)->pFmt = pFmt;

            it++;
        }
    }

    if(m_pDisCondition != NULL)
    {
        if (m_pDisCondition->Init(m_pFmtLib) != 0)
        {
            return -1;
        }
    }

    return 0;
}
/************ End of The Init Defining *************************/


/************ The LoadConfig Defining **************************/
// ��������������ͨ�������������ڴ�
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CChannel::LoadConfig(void)
{
    //��ȡͨ�������ļ���ȫ·�������磺��·��/"ap"+�����ID/ͨ��ID+".chl"
	char szChlFile[MAX_PATH];
	sprintf(szChlFile, "%s/ap%u/%u.chl", m_sFmtRootDir.c_str(), m_uAPID, m_uChlID);
	
	int nChannelFmt = VerifyFileFmt(szChlFile);
	if (nChannelFmt == FILE_FMT_BINARY)
	{
		return LoadBinaryConfig(szChlFile);
	}
	else
	{
		return LoadAsciiConfig(szChlFile);
	}

}
/************ End of The LoadConfig Defining *******************/

UINT4 CChannel::GetFinalLen() const
{
    UINT4 uLen = 0;

    //ȷ����Ӧ��Ա��������Ϊ��
    if(m_pFinalFmtMaps == NULL)
    {
        return 0;
    }

    FMT_MAP::iterator it = m_pFinalFmtMaps->begin();
    uLen = it->second->pFmt->GetFmtLen();
    while(it != m_pFinalFmtMaps->end())
    {
        //����������ε��û�õĻ������Ȳ�һ�£����ʾ
        //��ͨ�����ջ����Ǳ䳤�ģ�����0
        if(uLen != it->second->pFmt->GetFmtLen())
        {
            return 0;
        }

        it++;
    }

    return uLen;    
}

/************ The GetChlName Defining **************************/
// �������������ͨ��������
//
// �����������
//
// ���������
//   sChlName����õ�ͨ������
//
//   ����ֵ����
//
/***************************************************************/
void CChannel::GetChlName(STRING& sChlName) const
{
    sChlName = m_sChlName;
}
/************ End of The GetChlName Defining *******************/


/************ The GetChlID Defining ****************************/
// �������������ͨ����ID
//
// �����������
//
// �����������
//
//   ����ֵ����õ�ͨ��ID
//
/***************************************************************/
UINT4 CChannel::GetChlID(void) const
{
    return m_uChlID;
}
/************ End of The GetChlID Defining *********************/


/************ The GetFinalBrowseNames Defining *****************/
// ������������ñ�ͨ���������ջ��������ʽ��
//
// �����������
//
// ���������
//      Names����õ��������ջ��������ʽ���б�
//      Descs����ʽ�����б�
//
//   ����ֵ�����������ʽ����
//
/***************************************************************/
UINT4 CChannel::GetFinalBrowseNames(LIST<STRING>& Names, LIST<STRING>& Descs) const
{
    CFormat* pFMT = NULL;  //��ʽ����ָ����ʱ����
    STRING   sFmtDesc;     //��ʽ������ʱ����
    STRING   sSrcFmtName;  //����ʽ��Ӧ��ԭ��ʽ����ʱ����

    //��ʼ���������
    Names.clear();
    Descs.clear();

    //ȷ����Ӧ��Ա������Ϊ��
    if(m_pFinalBrowseFmtMaps == NULL)
    {
        return 0;
    }

    //Modified by ZhengYuqun 2004-01-18 SWPD03392
    //���б�ͨ���е����������ʽ
    /*FMT_MAP::iterator it = m_pFinalBrowseFmtMaps->begin();
    while(it != m_pFinalBrowseFmtMaps->end())
    {
        //��ȡ�����ʽ��
        Names.push_back(it->first);

        //��ȡ�����ʽ����
        pFMT = it->second->pFmt;
        pFMT->GetFormatGen(sFmtDesc, sSrcFmtName);
        Descs.push_back(sFmtDesc);

        it++;
    }*/

    //�ȴ�LIST�еõ�����Ȼ���ٴ�MAP�еõ�����
    if(m_pFinalFmtNameList != NULL)
    {
        FINAL_FMT_NAME_LIST::iterator lst_it = m_pFinalFmtNameList->begin();
        while(lst_it != m_pFinalFmtNameList->end())
        {
            //��ȡ��ǰ��ʽ���͸�ʽ����
            FMT_MAP::iterator it;
            it = m_pFinalBrowseFmtMaps->find(*lst_it);
            if(it != m_pFinalBrowseFmtMaps->end())
            {
                //��ȡ�����ʽ��
                Names.push_back(it->first);

                //��ȡ�����ʽ����
                pFMT = it->second->pFmt;
                pFMT->GetFormatGen(sFmtDesc, sSrcFmtName);
                Descs.push_back(sFmtDesc);
            }

            lst_it++;
        }
    }
    //End SWPD03392

    return Names.size();
}
/************ End of The GetFinalBrowseNames Defining **********/


/************ The IsTheChannel Defining ************************/
// �����������жϴ����һ�Ż���pABill�������ڱ�ͨ��
//
// ���������
//     pABill����������һ�Ż�������ָ��
//       uLen��pABill�����ݳ���
//
// ���������
//   pDestFmt��pABill�����ϵ����ջ�����ʽ����
//
//   ����ֵ�����ϱ�ͨ������0�����򷵻�-1
//
/***************************************************************/
int CChannel::IsTheChannel(const void* pABill, const UINT4 uLen, CFormat*& pDestFmt)
{
    //��ʼ���������
    pDestFmt = NULL;

    //ȷ������Ĳ�����ȷ
    if((pABill == NULL) || (uLen == 0))
    {
        return -1;
    }

    //����ּ���������Ϊ��
    if(m_pDisCondition == NULL)
    {
        return -1;
    }

    //�ж��Ƿ���ϱ�ͨ���ķּ�����
    CFormat* pSrcFmt = NULL;
    int nRet = m_pDisCondition->IsTheDisCondition(pABill, uLen, pSrcFmt);
    if(nRet != 0)
    {
        return -1;  //������
    }

    //���ϱ�ͨ���ķּ���������ȡ��ԭ��ʽ��
    STRING sFmtName;
    pSrcFmt->GetFmtName(sFmtName);

    //�жϱ�ͨ�������ջ�����ʽ���Ƿ��з��Ϸּ�������ȡ��ԭ��ʽ
    SFmtMap* pFmtMap = NULL;
    STRING sSrcFmtName;
    FMT_MAP::iterator it = m_pFinalFmtMaps->begin();
    while(it != m_pFinalFmtMaps->end())
    {
        pFmtMap = it->second;
        pFmtMap->pFmt->GetSrcFmtName(sSrcFmtName);
        if(!(sSrcFmtName.compare(sFmtName)))
        {
            //�ҵ��������������ջ�����ʽ
            pDestFmt = pFmtMap->pFmt;
            return 0;
        }

        it++;
    }

    //����Ļ��������ϱ�ͨ����ʽ
    return -1;
}
/************ End of The IsTheChannel Defining *****************/


/************ The SetChlName Defining **************************/
// ��������������ͨ������
//
// ���������
//   sChlName��ͨ������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
void CChannel::SetChlName(const STRING& sChlName)
{
    m_sChlName = sChlName;
}
/************ End of The SetChlName Defining *******************/


/************ The Create Defining ******************************/
// ��������������һ���µ�ͨ��
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CChannel::Create(void)
{
    FILE*   pChlFile = NULL;   //ͨ�������ļ�ָ��
    STRING  sFilePath;         //ͨ�������ļ�ȫ·��
    char    szBuff[50];        //��ʱ������
    char*   pTmpStr = szBuff;  //ָ����ʱ������ָ��
    int     nWriteLen  = 0;    //д�ļ����ݵĳ���
    char    szPath[MAX_PATH];  //�����ļ�ȫ·�����������ļ�����


    //��ȡͨ�������ļ���ȫ·�������磺��·��/"ap"+�����ID/ͨ��ID+".chl"
    sFilePath = m_sFmtRootDir + "/ap";
    sprintf(pTmpStr, "%u", m_uAPID);
    sFilePath += pTmpStr;
    strcpy(szPath, sFilePath.c_str());  //��ȡ�ļ�ȫ·�����������ļ�����
    sprintf(pTmpStr, "/%u.chl", m_uChlID);
    sFilePath += pTmpStr;

    //����ָ��ͨ��ID��ͨ�������ļ�
    pChlFile = fopen(sFilePath.c_str(), "wb");
    if(pChlFile == NULL)
    {
        //�����һ�δ��ļ�ʧ�ܣ��򴴽����ļ���Ŀ¼
        if(!CreateDeepDir(szPath))
        {
            return -1;  //�����ļ�Ŀ¼ʧ��
        }

        //�ٴ�һ���ļ�
        pChlFile = fopen(sFilePath.c_str(), "wb");
        if(pChlFile == NULL)
        {
            return -1;  //�ڶ��δ��ļ�ʧ��
        }
    }

    //����ͨ������д�������ļ�
    memset((void*)szBuff, 0, 50);
    nWriteLen = m_sChlName.length() + 1;
    if(nWriteLen > 16)
    {
        nWriteLen = 16;  //ͨ��������󳤶���16���ֽ�
    }
    memcpy((void*)szBuff, (void*)m_sChlName.c_str(), nWriteLen);
    if(fwrite((void*)szBuff, 16, 1, pChlFile) != 1)
    {
        fclose(pChlFile);
        return -1;
    }

    //�ر�ͨ�������ļ�
    fclose(pChlFile);

    return 0;
}
/************ End of The Create Defining ***********************/


/************ The SetDistingCondition Defining *****************/
// �������������ñ�ͨ�������һ�����ջ���Դ��ʽ�ķּ�����
//
// ���������
// sDisFormat����ͨ����һ�����ջ�����ʽ��Դ��ʽ�ĸ�ʽ��
//   sDisCond���ּ�����
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CChannel::SetDistingCondition(const STRING& sDisFormat, const STRING& sDisCond)
{
    //ȷ�����������ȷ
    if((sDisFormat.length() == 0) || (sDisCond.length() == 0))
    {
        return -1;
    }

    //ȷ����Ӧ��Ա��������Ϊ��
    if(m_pDisCondition == NULL)
    {
        m_pDisCondition = new CDisCondition(m_pFmtLib);
    }

    //����ָ��ԭ��ʽ�ķּ�������������ִ�н��
    int nRet = m_pDisCondition->AddDisCondition(sDisFormat, sDisCond);
    if(nRet != 0)
    {
        return -1;
    }

    //��ʼ�����ӵķּ�����
    nRet = m_pDisCondition->Init(m_pFmtLib);
    if(nRet != 0)
    {
        return -1;
    }

    return 0;
}
/************ End of The SetDistingCondition Defining **********/


/************ The AddChlFormats Defining ***********************/
// ������������׷�ӵķ�ʽ���ñ�ͨ�������ջ�����ʽ�������ջ��������ʽ��
//
// ���������
//       nFinalFmtCount���������õ����ջ�����ʽ����
//       szFinalFormats�����ջ�����ʽ�����飬������nFinalFmtCount��Ա��
//                       ÿһ����Ա����һ�����ջ�����ʽ��
// nFinalBrowseFmtCount���������õ����ջ��������ʽ����
//    szFinalBrowseFmts�����ջ��������ʽ�����飬������nFinalBrowseFmtCount
//                       ����Ա��ÿһ����Ա����һ�����ջ��������ʽ��
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CChannel::AddChlFormats(UINT4 nFinalFmtCount, const char* szFinalFormats[],
                            UINT4 nFinalBrowseFmtCount, const char* szFinalBrowseFmts[])
{
    int i = 0;  //ѭ��������


    //ȷ�������������ȷ��
    if((nFinalFmtCount == 0) || (szFinalFormats == NULL) || (nFinalBrowseFmtCount == 0)
        || (szFinalBrowseFmts == NULL))
    {
        return -1;
    }
    for(i = 0; i < nFinalFmtCount; i++)
    {
        if(szFinalFormats[i] == NULL)
        {
            return -1;
        }
    }
    for(i = 0; i < nFinalBrowseFmtCount; i++)
    {
        if(szFinalBrowseFmts[i] == NULL)
        {
            return -1;
        }
    }

    //ȷ����Ӧ��Ա��������Ϊ��
    if(m_pFinalFmtMaps == NULL)
    {
        m_pFinalFmtMaps = new FMT_MAP;
    }
    if(m_pFinalBrowseFmtMaps == NULL)
    {
        m_pFinalBrowseFmtMaps = new FMT_MAP;
    }

    //����������Ҫ�õ�����ʱ����
    SFmtMap* pFmp = NULL;
    CFormat* pFmt = NULL;
    FMT_MAP::iterator it;

    //�������ջ�����ʽ
    for(i = 0; i < nFinalFmtCount; i++)
    {
        //�ж����ջ�����ʽ���б����Ƿ��Ѿ����ڵ�ǰ�ĸ�ʽ��
        it = m_pFinalFmtMaps->find(szFinalFormats[i]);
        if(it != m_pFinalFmtMaps->end())
        {
            //����Ѿ����ڣ�����Ե�ǰ��ʽ��
            //return -1;
            continue;
        }

        //�����µ����ջ�����ʽ��Ӧ��ϵ
        pFmp = new SFmtMap;
        pFmp->sFmtName = szFinalFormats[i];

        //��ȡ��ʽ����Ӧ�ĸ�ʽ����
        pFmt = m_pFmtLib->GetFormat(pFmp->sFmtName);
        if(pFmt == NULL)
        {
            delete pFmp;
            return -1;
        }

        //�����������ջ�����ʽ��Ӧ��ϵ�������ջ�����ʽ�б���
        pFmp->pFmt = pFmt;
        (*m_pFinalFmtMaps)[pFmp->sFmtName] = pFmp;
    }

    //�������ջ��������ʽ
    for(i = 0; i < nFinalBrowseFmtCount; i++)
    {
        //�ж����ջ��������ʽ���б����Ƿ��Ѿ����ڵ�ǰ�ĸ�ʽ��
        it = m_pFinalBrowseFmtMaps->find(szFinalBrowseFmts[i]);
        if(it != m_pFinalBrowseFmtMaps->end())
        {
            //����Ѿ����ڣ�����Ե�ǰ��ʽ��
            //return -1;
            continue;
        }

        //�����µ��������������ʽ��Ӧ��ϵ
        pFmp = new SFmtMap;
        pFmp->sFmtName = szFinalBrowseFmts[i];

        //��ȡ��ʽ����Ӧ�ĸ�ʽ����
        pFmt = m_pFmtLib->GetFormat(pFmp->sFmtName);
        if(pFmt == NULL)
        {
            delete pFmp;
            return -1;
        }

        //�����������ջ��������ʽ��Ӧ��ϵ�������ջ��������ʽ�б���
        pFmp->pFmt = pFmt;
        (*m_pFinalBrowseFmtMaps)[pFmp->sFmtName] = pFmp;
    }

    return 0;
}
/************ End of The AddChlFormats Defining ****************/


/************ The GetChlFormats Defining ***********************/
// ������������ñ�ͨ�����е����ջ�����ʽ��
//           �����ջ��������ʽ��
//
// �����������
//
// ���������
//          FinalFmts����õ����ջ�����ʽ���б�
// FinalBrowseFormats����õ����ջ��������ʽ���б�
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CChannel::GetChlFormats(LIST<STRING>& FinalFmts, LIST<STRING>& FinalBrowseFormats) const
{
    //��ʼ���������
    FinalFmts.clear();
    FinalBrowseFormats.clear();
    FMT_MAP::iterator it;

    //���б�ͨ���е��������ջ�����ʽ
    if(m_pFinalFmtMaps != NULL)
    {
        it = m_pFinalFmtMaps->begin();
        while(it != m_pFinalFmtMaps->end())
        {
            //��ȡ���ջ�����ʽ��
            FinalFmts.push_back(it->first);
            it++;
        }
    }

    //���б�ͨ���е����������ʽ
    if(m_pFinalBrowseFmtMaps != NULL)
    {
        it = m_pFinalBrowseFmtMaps->begin();
        while(it != m_pFinalBrowseFmtMaps->end())
        {
            //��ȡ�����ʽ��
            FinalBrowseFormats.push_back(it->first);
            it++;
        }
    }

    return 0;
}
/************ End of The GetChlFormats Defining ****************/


/************ The Delete Defining ******************************/
// ����������ɾ��ͨ��
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CChannel::Delete(void)
{
    STRING    sFilePath;           //ͨ�������ļ�ȫ·������
    char      szBuff[50];         //��ʱ������
    char*     pTmpStr  = szBuff;   //ָ�򻺳�����ָ��

    
    //��ȡͨ�������ļ���ȫ·�������磺��·��/"ap"+�����ID/ͨ��ID+".chl"
    sFilePath = m_sFmtRootDir + "/ap";
    sprintf(pTmpStr, "%u/%u.chl", m_uAPID, m_uChlID);
    sFilePath += pTmpStr;

    //ɾ��ָ���������ļ�
    if(remove(sFilePath.c_str()) != 0)
    {
        return -1;
    }

    return 0;
}
/************ End of The Delete Defining ***********************/


/************ The Commit Defining ******************************/
// ����������ʹ��һ��Commit������Commit����֮�����޸ĵ�������Ϣ
//           ���浽������
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CChannel::Commit(void)
{
    STRING  sFilePath;                  //��������ļ�ȫ·��
    FILE*   pFile   = NULL;             //�����ļ�ָ��
    int     nWriteLen  = 0;             //д�ļ����ݵĳ���
    BYTE    pBuff[100];                 //��д�ļ�����ʱ���Ȳ���ĵط���д0�Ļ�����
    char*   pTmpStr  = (char*)pBuff;    //ָ�򻺳�����ָ��
    BYTE    yTmpNum = 0;                //дһ���ֽڳ��ȵ���������
    char    szPath[MAX_PATH];           //�����ļ�ȫ·�����������ļ�����


    //��ȡͨ�������ļ���ȫ·�������磺��·��/"ap"+�����ID/ͨ��ID+".chl"
    sFilePath = m_sFmtRootDir + "/ap";
    sprintf(pTmpStr, "%u", m_uAPID);
    sFilePath += pTmpStr;
    strcpy(szPath, sFilePath.c_str());  //��ȡ�ļ�ȫ·�����������ļ�����
    sprintf(pTmpStr, "/%u.chl", m_uChlID);
    sFilePath += pTmpStr;

    //��ָ����ͨ�������ļ�
    pFile = fopen(sFilePath.c_str(), "wb");
    if(pFile == NULL)
    {
        //�����һ�δ��ļ�ʧ�ܣ��򴴽����ļ���Ŀ¼
        if(!CreateDeepDir(szPath))
        {
            return -1;  //�����ļ�Ŀ¼ʧ��
        }

        //�ٴ�һ���ļ�
        pFile = fopen(sFilePath.c_str(), "wb");
        if(pFile == NULL)
        {
            return -1;  //�ڶ��δ��ļ�ʧ��
        }
    }

    //��ͨ������д�������ļ�
    memset((void*)pBuff, 0, 100);
    nWriteLen = m_sChlName.length() + 1;
    if(nWriteLen > LEN_CHL_NAME)
    {
        nWriteLen = LEN_CHL_NAME;  //ͨ��������󳤶���16���ֽ�
    }
    memcpy((void*)pBuff, (void*)m_sChlName.c_str(), nWriteLen);
    if(fwrite((void*)pBuff, LEN_CHL_NAME, 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //�����ջ�����ʽ����д�������ļ�
    if(m_pFinalFmtMaps == NULL)
    {
        yTmpNum = 0;
    }
    else
    {
        yTmpNum = m_pFinalFmtMaps->size();  //�ջ�����ʽ����ռ��һ���ֽ�
    }
    if(fwrite((void*)&yTmpNum, LEN_BYTE, 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //����������Ҫ�õ�����ʱ����
    FMT_MAP::iterator it;
    STRING sFmtName;

    //��ÿһ�����ջ�����ʽ������ּ�����д�������ļ�
    if(m_pFinalFmtMaps != NULL)
    {
        it = m_pFinalFmtMaps->begin();
        STRING sDisCondName;
        while(it != m_pFinalFmtMaps->end())
        {
            //����ǰ��ʽ��д�������ļ�
            memset((void*)pBuff, 0, 100);
            sFmtName = it->first;
            nWriteLen = sFmtName.length() + 1;
            if(nWriteLen > LEN_CHL_FINAL_FMT_NAME)
            {
                nWriteLen = LEN_CHL_FINAL_FMT_NAME;  //��ʽ������󳤶���16���ֽ�
            }
            memcpy((void*)pBuff, (void*)sFmtName.c_str(), nWriteLen);
            if(fwrite((void*)pBuff, LEN_CHL_FINAL_FMT_NAME, 1, pFile) != 1)
            {
                fclose(pFile);
                return -1;
            }

            //����ǰ��ʽ����Ӧ�ķּ�����д�������ļ�����>>

            //�����ǰͨ�������ڷּ���������д64�����ַ�
            if(m_pDisCondition == NULL)
            {
                memset((void*)pBuff, 0, 100);
                if(fwrite((void*)pBuff, LEN_CHL_DIS_CONDITION, 1, pFile) != 1)
                {
                    fclose(pFile);
                    return -1;
                }

                it++;
                continue;
            }

            //��ǰͨ�����ڷּ�����
            memset((void*)pBuff, 0, 100);
            if(m_pDisCondition->GetDisCondName(sFmtName, sDisCondName) != 0)
            {
                //�����ǰ��ʽ�����ڶ�Ӧ�ķּ���������д64�����ַ�
                memset((void*)pBuff, 0, 100);
                if(fwrite((void*)pBuff, LEN_CHL_DIS_CONDITION, 1, pFile) != 1)
                {
                    fclose(pFile);
                    return -1;
                }

                it++;
                continue;
            }

            //��ǰ��ʽ���ڶ�Ӧ�ķּ�����
            nWriteLen = sDisCondName.length() + 1;
            if(nWriteLen > LEN_CHL_DIS_CONDITION)
            {
                nWriteLen = LEN_CHL_DIS_CONDITION;  //�ּ�������󳤶���64���ֽ�
            }
            memcpy((void*)pBuff, (void*)sDisCondName.c_str(), nWriteLen);
            if(fwrite((void*)pBuff, LEN_CHL_DIS_CONDITION, 1, pFile) != 1)
            {
                fclose(pFile);
                return -1;
            }

            it++;
        }
    }

    //�����ջ��������ʽ����д�������ļ�
    if(m_pFinalBrowseFmtMaps == NULL)
    {
        yTmpNum = 0;
    }
    else
    {
        yTmpNum = m_pFinalBrowseFmtMaps->size();  //�ջ��������ʽ����ռ��һ���ֽ�
    }
    if(fwrite((void*)&yTmpNum, LEN_BYTE, 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //��ÿһ�����ջ��������ʽ��д�������ļ�
    if(m_pFinalBrowseFmtMaps != NULL)
    {
        it = m_pFinalBrowseFmtMaps->begin();
        while(it != m_pFinalBrowseFmtMaps->end())
        {
            //����ǰ��ʽ��д�������ļ�
            memset((void*)pBuff, 0, 100);
            sFmtName = it->first;
            nWriteLen = sFmtName.length() + 1;
            if(nWriteLen > LEN_CHL_FINALBRS_FMT_NAME)
            {
                nWriteLen = LEN_CHL_FINALBRS_FMT_NAME;  //��ʽ������󳤶���16���ֽ�
            }
            memcpy((void*)pBuff, (void*)sFmtName.c_str(), nWriteLen);
            if(fwrite((void*)pBuff, LEN_CHL_FINALBRS_FMT_NAME, 1, pFile) != 1)
            {
                fclose(pFile);
                return -1;
            }

            it++;
        }
    }

    //�رո�ʽ�����ļ�
    fclose(pFile);

    return 0;
}
/************ End of The Commit Defining ***********************/

int CChannel::LoadBinaryConfig(const STRING& sChlFile)
{    
	FILE*     pFile = NULL;
    char      szBuff[255];         //��ȡ�����ļ���Ϣ�Ļ�����
    char*     pTmpStr  = szBuff;   //ָ�򻺳�����ָ��
    BYTE      yTmpNum = 0;         //��ȡһ���ֽ���������ʱ����
    SFmtMap*  pFmtMap = NULL;      //��SFmtMap�ṹ����ʱ����

     //������Ӧ��Ա����
    if (m_pDisCondition == NULL)
    {
        m_pDisCondition = new CDisCondition;
    }
	
	if (m_pFinalFmtMaps == NULL)
    {
        m_pFinalFmtMaps = new FMT_MAP;
    }

    //������Ӧ��Ա����
    if (m_pFinalBrowseFmtMaps == NULL)
    {
        m_pFinalBrowseFmtMaps = new FMT_MAP;
    }

	//�Զ�����ֻ����ʽ��ͨ�������ļ�
    if ((pFile = fopen(sChlFile.c_str(), "rb")) == NULL)
    {
        TRACE(MTS_FORMAT, S_FMT_OPEN_FILE_FAIL, 
              sChlFile.c_str());
        return -1;
    }

    //��ȡͨ������
    if ((fread((void*)pTmpStr, LEN_CHL_NAME, 1, pFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sChlFile.c_str());
        fclose(pFile);
        return -1;
    }
    else
    {
        *(pTmpStr + LEN_CHL_NAME) = '\0';
        m_sChlName = pTmpStr;
    }

    //��ͨ�����ջ�����ʽ����
    if ((fread((void*)&yTmpNum, LEN_BYTE, 1, pFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sChlFile.c_str());
        fclose(pFile);
        return -1;
    }
    if (yTmpNum == 0)
    {
        TRACE(MTS_FORMAT, S_FMT_INVALID_FINALBILL_FMT_NUM, 
              sChlFile.c_str());

        fclose(pFile);
        return -1;
    }

    for (int i = 0; i < yTmpNum; i++)
    {
        //�����ջ�����ʽ��
        if((fread((void*)pTmpStr, LEN_CHL_FINAL_FMT_NAME, 1, pFile)) != 1)
        {
            TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                  sChlFile.c_str());
            fclose(pFile);
            return -1;
        }
        else
        {
            *(pTmpStr + LEN_CHL_FINAL_FMT_NAME) = '\0';
        }

        pFmtMap = new SFmtMap;
        pFmtMap->pFmt = NULL;
        pFmtMap->sFmtName = pTmpStr;
        (*m_pFinalFmtMaps)[pFmtMap->sFmtName] = pFmtMap;

        //����ʽ�ּ�����
        if ((fread((void*)pTmpStr, LEN_CHL_DIS_CONDITION, 1, pFile)) != 1)
        {
            TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                  sChlFile.c_str());
            fclose(pFile);
            return -1;
        }
        else
        {
            *(pTmpStr + LEN_CHL_DIS_CONDITION) = '\0';
        }
        if (strlen(pTmpStr) != 0)
        {
            m_pDisCondition->AddDisCondition(pFmtMap->sFmtName, pTmpStr);
        }
    }

	//��ͨ���������ʽ����
    if ((fread((void*)&yTmpNum, LEN_BYTE, 1, pFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sChlFile.c_str());
        fclose(pFile);
        return -1;
    }

    //������Ӧ��Ա����
    if (m_pFinalBrowseFmtMaps == NULL)
    {
        m_pFinalBrowseFmtMaps = new FMT_MAP;
    }
    
    //Added by ZhengYuqun 2004-01-18 SWPD03392
    if(m_pFinalFmtNameList == NULL)
    {
        m_pFinalFmtNameList = new FINAL_FMT_NAME_LIST;
    }
    //End SWPD03392    

    if (yTmpNum != 0)
    {
        for(int j = 0; j < yTmpNum; j++)
        {
            //�������ջ��������ʽ��
            if ((fread((void*)pTmpStr, LEN_CHL_FINALBRS_FMT_NAME, 1, pFile)) != 1)
            {
                TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                      sChlFile.c_str());
                fclose(pFile);
                return -1;
            }
            else
            {
                *(pTmpStr + LEN_CHL_FINALBRS_FMT_NAME) = '\0';
            }

            pFmtMap = new SFmtMap;
            pFmtMap->sFmtName = pTmpStr;
            pFmtMap->pFmt = NULL;
            (*m_pFinalBrowseFmtMaps)[pFmtMap->sFmtName] = pFmtMap;
            m_pFinalFmtNameList->push_back(pFmtMap->sFmtName); //Added by ZhengYuqun 2004-01-18 SWPD03392            
        }
    }

    //�رմ򿪵��ļ�
    fclose(pFile);

    return 0;
}

int CChannel::LoadAsciiConfig(const STRING& sChlFile)
{
    FILE* pFile = NULL;
    SFmtMap*  pFmtMap = NULL;      //��SFmtMap�ṹ����ʱ����

     //������Ӧ��Ա����
    if (m_pDisCondition == NULL)
    {
        m_pDisCondition = new CDisCondition;
    }
	
	if (m_pFinalFmtMaps == NULL)
    {
        m_pFinalFmtMaps = new FMT_MAP;
    }

    //������Ӧ��Ա����
    if (m_pFinalBrowseFmtMaps == NULL)
    {
        m_pFinalBrowseFmtMaps = new FMT_MAP;
    }
    
    //Added by ZhengYuqun 2004-01-18 SWPD03392
    if(m_pFinalFmtNameList == NULL)
    {
        m_pFinalFmtNameList = new FINAL_FMT_NAME_LIST;
    }
    //End SWPD03392    

    //��ֻ����ʽ��ͨ�������ļ�
    if ((pFile = fopen(sChlFile.c_str(), "rt")) == NULL)
    {
        TRACE(MTS_FORMAT, S_FMT_OPEN_FILE_FAIL, 
              sChlFile.c_str());
        return -1;
    }

    char szLine[1024];
    char szTemp[1024];
    char szKeyName[1024];
    STRING sLine;

    while (!feof(pFile))
    {
        szLine[0] = '\0';
        if (fgets(szLine, 1024, pFile) == NULL)
        {
            if (ferror(pFile))
            {
                TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                      sChlFile.c_str());
				fclose(pFile);
				return -1;
            }
        }
        
        sLine = szLine;
        StringLRTrim(szLine);
        //���У���������һ��
        if (strlen(szLine) == 0)
        {
            continue;
        }
        
        sLine = szLine;

        //ͨ��������
        if (sLine.find("ChannelName", 0) == 0)
        {
            char szChlName[128];
            
            sscanf(szLine, "%s %s", szKeyName, szChlName);
            StringLRTrim(szChlName);
            if (strlen(szChlName) == 0)
            {
                TRACE(MTS_FORMAT, S_FMT_FILED_NULL, 
                      szKeyName, sChlFile.c_str());
                fclose(pFile);
                return -1;
            }
            
            szChlName[LEN_CHL_NAME - 1] = '\0';
            m_sChlName = szChlName;
        }
        //ͨ����Ӧ�����ո�ʽ����
        else if (sLine.find("FinalFormat", 0) == 0)
        {
            char szFormatName[128];
            char szDisCondition[128];
            sscanf(szLine, "%s %s %s", szTemp, szFormatName, szDisCondition);
            
            StringLRTrim(szFormatName);
            StringLRTrim(szDisCondition);
            
            pFmtMap = new SFmtMap;
            pFmtMap->pFmt = NULL;
            pFmtMap->sFmtName = szFormatName;
            (*m_pFinalFmtMaps)[pFmtMap->sFmtName] = pFmtMap;
            
            if (strlen(szDisCondition) != 0)
            {
                m_pDisCondition->AddDisCondition(pFmtMap->sFmtName, szDisCondition);
            }
            else
            {
                TRACE(MTS_FORMAT, S_FMT_FILED_NULL, 
                      szTemp, sChlFile.c_str());
                fclose(pFile);
                return -1;
            }
        }
        else if (sLine.find("FinalBrowseFormat", 0) == 0)
        {
            sscanf(szLine, "%s %s", szKeyName, szTemp);
            StringLRTrim(szTemp);
            if (strlen(szTemp) == 0)
            {
                TRACE(MTS_FORMAT, S_FMT_FILED_NULL, 
                    szKeyName, sChlFile.c_str());
                fclose(pFile);
                return -1;
            }
            
            pFmtMap = new SFmtMap;
            pFmtMap->sFmtName = szTemp;
            pFmtMap->pFmt = NULL;
            (*m_pFinalBrowseFmtMaps)[pFmtMap->sFmtName] = pFmtMap;
            m_pFinalFmtNameList->push_back(pFmtMap->sFmtName); //Added by ZhengYuqun 2004-01-18 SWPD03392
        }
    }
    
    //�رմ򿪵��ļ�
    fclose(pFile);
    
    return 0;
}

int CChannel::VerifyFileFmt(const STRING& sChlFile)
{
	FILE* pFile = NULL;
	if ((pFile = fopen(sChlFile.c_str(), "rt")) == NULL)
	{
		return FILE_FMT_UNKNOWN;
	}
	
	BOOL bChlNameFound     = FALSE;
	BOOL bFinalFmtFound    = FALSE;
	BOOL bFinalBrwFmtFound = FALSE;

	char szLine[4096];
	while (!feof(pFile))
	{
		if (NULL == fgets(szLine, 4096, pFile))
		{
			break;
		}
		
		StringLRTrim(szLine);
		//���У���������һ��
		if (strlen(szLine) == 0)
		{
			continue;
		}

		STRING sLine = szLine;
		//ͨ��������
		if (sLine.find("ChannelName", 0) == 0)
		{
			bChlNameFound = TRUE;
		}
		else if (sLine.find("FinalFormat", 0) == 0)
		{
			bFinalFmtFound = TRUE;
		}
		else if (sLine.find("FinalBrowseFormat", 0) == 0)
		{
			bFinalBrwFmtFound = TRUE;
		}
	}

	fclose(pFile);
	if (bChlNameFound && bFinalFmtFound && bFinalBrwFmtFound)
	{
		return FILE_FMT_ASCII;
	}
	else
	{
		return FILE_FMT_BINARY;
	}
}
