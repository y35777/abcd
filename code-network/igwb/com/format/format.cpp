/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CFormat
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/

#include "convert.h"
#include "condition.h"
#include "format_global.h"
#include "../include/toolbox.h"
#include "../include/export_dll.h"
#include "format.h"

extern char szFormatCfgErr[2048];

/************ The CFormat Defining *****************************/
// �������������캯��
//
// ���������
//   sRootDir����ʽ���ÿ��·��
//   sFmtName����ʽ��
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CFormat::CFormat(const STRING& sRootDir, const STRING& sFmtName):
                 m_sFmtRootDir(sRootDir), m_sFmtName(sFmtName)
{
    //ȷ��������ȷ
    assert(m_sFmtRootDir.length() != 0);
    assert(m_sFmtName.length() != 0);


    //���ݲ�����ʼ����Ӧ��Ա����
    //m_sFmtRootDir = sRootDir;
    //m_sFmtName = sFmtName;

    //��ʼ������ָ���Ա������������Ա����
    m_pFieldsMap = NULL;
    m_pDistingCondition = NULL;
    m_pSrcFmt = NULL;
    m_pConvert = NULL;
    m_uFmtLen = 0;
    m_uFieldNum = 0;
    m_pFieldsArray = NULL;
    m_pQueryCondition = new CCondition(this);

    //������������ĳ�Ա��־Ϊ���㣬��ʾ�����е�����Ҫת����Ŀ���ʽ
    m_pQueryCondition->SetFlag(-1);
    m_pQueryCondition->SetQueryFlag(1);  //���ò�ѯ��־

    m_pStatCondition = new CCondition(this);

    //������������ĳ�Ա��־Ϊ���㣬��ʾ�����е�����Ҫת����Ŀ���ʽ
    m_pStatCondition->SetFlag(-1);
    m_pStatCondition->SetQueryFlag(1);  //���ò�ѯ��־
}
/************ End of The CFormat Defining **********************/


/************ The ~CFormat Defining ****************************/
// ������������������
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CFormat::~CFormat()
{
    SFieldInfo*  pFI = NULL;     //����������ָ��


    //ɾ����Ϊʶ�������ĺ�׺���ʽ����
    if(m_pDistingCondition != NULL)
    {
        delete m_pDistingCondition;
        m_pDistingCondition = NULL;
    }

    //ɾ����Ϊ��ѯ�����ĺ�׺���ʽ����
    if(m_pQueryCondition != NULL)
    {
        delete m_pQueryCondition;
        m_pQueryCondition = NULL;
    }

    //ɾ����ʽת������
    if(m_pConvert != NULL)
    {
        delete m_pConvert;
        m_pConvert = NULL;
    }

    //ɾ��������������Ϣ
    if(m_pFieldsArray != NULL)
    {
        for(int i = 0; i < m_uFieldNum; i++)
        {
            pFI = m_pFieldsArray[i];
            if(pFI != NULL)
            {
                if(pFI->pEnumInfos != NULL)
                {
                    delete pFI->pEnumInfos;
                }
                delete pFI;
            }
        }

        //ɾ����������������
        delete[] m_pFieldsArray;
        m_pFieldsArray = NULL;
    }

    //ɾ���Ի���������Ϊ����MAP
    if(m_pFieldsMap != NULL)
    {
        delete m_pFieldsMap;
        m_pFieldsMap = NULL;
    }

    if(m_pStatCondition != NULL)
    {
        delete m_pStatCondition;
        m_pStatCondition = NULL;
    }
}
/************ End of The ~CFormat Defining *********************/

void CFormat::SetMustMerge(BOOL bMustMerge)
{
     m_bIsMustMerge = bMustMerge;	
}

/************ The Init Defining ********************************/
// ������������ʼ������
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormat::Init(void)
{
    //�����ʽ��������Ϣ
    if(LoadConfig() != 0)
    {
        SNPRINTF(szFormatCfgErr,
                 sizeof(szFormatCfgErr),
                 S_FMT_LOAD_FMT_CONFIG_FAIL,
                 m_sFmtName.c_str());
        TRACE(MTS_FORMAT, szFormatCfgErr);

        return -1;
    }

    return 0;
}
/************ End of The Init Defining *************************/


/************ The LoadConfig Defining **************************/
// ��������������ʽ������Ϣ�����ڴ溯��
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormat::LoadConfig(void)
{
    STRING  sFmtFile;            //��������ļ�ȫ·��
    char    szBuff[255];         //��ȡ�����ļ���Ϣ�Ļ�����
    char*   pTmpStr  = szBuff;   //ָ�򻺳�����ָ��
    FILE*   pFmtFile = NULL;     //�����ļ�ָ��
    SFieldInfo*  pFI = NULL;     //����������ָ��
    UINT2   uTmpNum = 0;         //��ȡ�����ֽڵ�������ʱ����
    SEnumInfo  EIF;              //������ö��ֵ�ṹ����


    //��ȡ�����ļ���ȫ·��
    sFmtFile = m_sFmtRootDir + "/" + m_sFmtName + ".fmt";

    //��ֻ����ʽ���ļ���ΪsFmtFile�ĸ�ʽ�����ļ�
    pFmtFile = fopen(sFmtFile.c_str(), "rb");
    if(pFmtFile == NULL)
    {
        TRACE(MTS_FORMAT, S_FMT_OPEN_FILE_FAIL, 
              sFmtFile.c_str());
        return -1;
    }

    //��ȡ��ʽ����
    if((fread((void*)pTmpStr, LEN_FMT_DESC, 1, pFmtFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sFmtFile.c_str());
        fclose(pFmtFile);
        return -1;
    }
    else
    {
        *(pTmpStr + LEN_FMT_DESC) = '\0';
        m_sFmtDesc = pTmpStr;
    }

    //��ȡԴ��ʽ��
    if((fread((void*)pTmpStr, LEN_FMT_SRC_FMT_NAME, 1, pFmtFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
            sFmtFile.c_str());
        fclose(pFmtFile);
        return -1;
    }
    else
    {
        *(pTmpStr + LEN_FMT_SRC_FMT_NAME) = '\0';
        m_sSrcFmtName = pTmpStr;
    }

    //��ȡ��ʽʶ������
    if((fread((void*)pTmpStr, LEN_FMT_DIS_CONDITION, 1, pFmtFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sFmtFile.c_str());
        fclose(pFmtFile);
        return -1;
    }
    else
    {
        *(pTmpStr + LEN_FMT_DIS_CONDITION) = '\0';
        if(strlen(pTmpStr) != 0)
        {
            m_sDistingCondition = pTmpStr;
        }
    }

    //����ƽ 2002��4��4���޸����´���Ϊ�˴����ʽ�����ļ��е��ֽ�����ֽڶ�������
    //������ʽ�����ļ��л��������ָʾ
    if((fread((void*)&uTmpNum, 2, 1, pFmtFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sFmtFile.c_str());
        fclose(pFmtFile);
        return -1;
    }
    uTmpNum = NTOHS(uTmpNum);   //�����ֽ�������

    //��ʼ����Ӧ��Ա����
    m_uFieldNum = uTmpNum;
    m_pFieldsArray = new SFieldInfo*[m_uFieldNum];
    m_pFieldsMap = new FIELD_MAP;

    //��ʼ�����������������ÿһ��Ϊ�գ��Ա��������������ܹ���ȷɾ��
    for(int k = 0; k < m_uFieldNum; k++)
    {
        m_pFieldsArray[k] = NULL;
    }

    BYTE pFIBuf[SIZEOF_FIELDINFO];
    BYTE pEIBuf[SIZEOF_ENUMINFO];
    BYTE* pIn = NULL;

    //��ȡ������ʽ��ÿһ��������
    for(int i = 0; i < m_uFieldNum; i++)
    {
        //����һ�������������ڵ�
        pFI = new SFieldInfo;
        pFI->pEnumInfos = NULL;
        m_pFieldsArray[i] = pFI;

        //����һ�Ż���������SFieldInfo��pFI
        if ((fread(pFIBuf, SIZEOF_FIELDINFO, 1, pFmtFile)) != 1)
        {
            TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                  sFmtFile.c_str());
            fclose(pFmtFile);
            return -1;
        }

        pIn = pFIBuf;
        SerialFieldInfo(pFI, pIn);

        pFI->pEnumInfos = NULL;

        //���������ö����
        if(pFI->flgEnum != 0)
        {
            //����ö����Ϣ�ṹ����
            pFI->pEnumInfos = new ENUMINFO_LIST;

            //������ǰ���ö����Ϣ����
            if ((fread((void*)&uTmpNum, 2, 1, pFmtFile)) != 1)
            {
                TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                      sFmtFile.c_str());
                fclose(pFmtFile);
                return -1;
            }

            uTmpNum = NTOHS(uTmpNum);  //�����ֽ�������

            //��ȡ��ǰ�������ö����Ϣ
            for(int j = 0; j < uTmpNum; j++)
            {
                //����һ��ö����Ϣ����
                if ((fread(pEIBuf, SIZEOF_ENUMINFO, 1, pFmtFile)) != 1)
                {
                    TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                          sFmtFile.c_str());
                    fclose(pFmtFile);
                    return -1;
                }

                pIn = pEIBuf;
                SerialEnumInfo(&EIF, pIn);

                //����ȡ��ö����Ϣ���뵽��ǰ���ö��������
                pFI->pEnumInfos->push_back(EIF);
            }
        }

        //�ѵ�ǰ���������뵽�Ի���������Ϊ����MAP
        m_pFieldsMap->insert(FIELD_MAP::value_type(pFI->szFieldName, pFI));
    }
    //�޸���� ����ƽ2002��4��4��

	if(!m_sDistingCondition.empty())
	{
		m_pDistingCondition = new CCondition(this, m_sDistingCondition);
	}

    //�رմ򿪵ĸ�ʽ�����ļ�
    fclose(pFmtFile);

    //��ʼ�������򣬼��㻰����λƫ�ơ��ֽ�ƫ�ơ���������
    if((InitFormatInfo()) != 0)
    {
        return -1;
    }

    return 0;
}
/************ End of The LoadConfig Defining *******************/


/************ The InitFormatInfo Defining **********************/
// ��������������һ����ʽ����������ֽ�ƫ�ơ�λƫ���Լ��ֽ��ܳ��Ⱥ���
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormat::InitFormatInfo(void)
{
    UINT4  nByteOffset = 0;      //��������ʽ���ֽڼ���ĳ��ȼ�����
    UINT4  nBitOffset  = 0;      //��������ʽ��λ����ĳ��ȼ�����
    SFieldInfo*  pFI   = NULL;   //��ǰ������ָ��

    UINT4  nBitNum     = NULL;   //����λ����λ����ĳ��ȼ�����
    UINT4  nTotalBit   = NULL;   //����λ����λ����ĵ��ܳ���


    //ȷ������ʽ�����������
    if(m_uFieldNum == 0)
    {
        TRACE(MTS_FORMAT, S_FMT_FIELD_NUM_EMPTY, 
              m_sFmtName.c_str());
        return -1;
    }

    m_uFmtLen = 0;

    //��ʼ������ʽ�е�ÿһ������ֽ�ƫ������λƫ����
    for(int i = 0; i < m_uFieldNum; i++)
    {
        //���õ�ǰ����ֽ�ƫ������λƫ����
        pFI = m_pFieldsArray[i];
        pFI->nByteOffset = nByteOffset;
        pFI->nBitOffset = nBitOffset;

        //�����ǰ��������λ����
        if(pFI->flgBit == 0)
        {
            //������һ������ֽ�ƫ������λƫ����
            nByteOffset += GetFieldTypeLen(pFI) * pFI->nFieldLength;
            nBitOffset += GetFieldTypeLen(pFI) * pFI->nFieldLength * 8;
        }

        //����û�������λ����
        else
        {
            //������һ�����λƫ����
            nBitOffset  += pFI->nFieldLength;  //����λ���ͣ�ֱ���Ǹ�λ��ĳ���

            //������һ��λ���λƫ����������λ�����λ����
            nBitNum = pFI->nFieldLength;
            nTotalBit = GetFieldTypeLen(pFI) * 8;

            //��������λ���λƫ����
            while((nBitNum < nTotalBit) && i < m_uFieldNum)
            {
                pFI = m_pFieldsArray[++i];

                //�����ǰ����λ��
                if(pFI->flgBit == 0)
                {
                    TRACE(MTS_FORMAT, S_FMT_NOT_SEQUENTIAL_BITFLD, 
                          pFI->szFieldName, m_sFmtName.c_str());
                    return -1;
                }
                nBitNum += pFI->nFieldLength;

                //���λ�򳤶ȼ�����ֵ��������λ�����λ����
                if(nBitNum > nTotalBit)
                {
                    TRACE(MTS_FORMAT, S_FMT_BITFLD_LEN_OVERFLOW, 
                          pFI->szFieldName, m_sFmtName.c_str());
                    return -1;
                }

                //���õ�ǰ����ֽ�ƫ������λƫ����
                pFI->nByteOffset = nByteOffset;
                pFI->nBitOffset = nBitOffset;

                //������һ�����λƫ����
                nBitOffset += pFI->nFieldLength;
            }

            //������һ������ֽ�ƫ����
            nByteOffset += GetFieldTypeLen(pFI);
        }
    }

    //���ñ���ʽ�����ֽڳ���
    m_uFmtLen = nByteOffset;

    return 0;
}
/************ End of The InitFormatInfo Defining ***************/


/************ The CaculateFmtLen Defining **********************/
// �������������㱾��ʽ�ֽڵĳ��ȣ��ú���Ŀǰ�ò�����
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ����ر���ʽ���ֽ��ܳ��ȣ�ʧ�ܷ���-1
//
/***************************************************************/
UINT4 CFormat::CaculateFmtLen(void)
{
    SFieldInfo*  pFI   = NULL;   //��ǰ������ָ��
    UINT4  uFmtLen = 0;          //����ʽ���ȼ�����


    //��������ʽ�е������򣬼�����λ��ʾ�ı���ʽ�ܳ���
    for(int i = 0; i < m_uFieldNum; i++)
    {
        pFI = m_pFieldsArray[i];

        //����û�������λ����
        if(pFI->flgBit == 0)
        {
            uFmtLen += GetFieldTypeLen(pFI) * pFI->nFieldLength * 8;
        }

        //����û�������λ����
        else
        {
            uFmtLen  += pFI->nFieldLength;  //����λ���ͣ�ֱ���Ǹ�λ��ĳ���
        }
    }

    //ȷ�����������λ������8��������
    if((uFmtLen % 8) != 0)
    {
        TRACE(MTS_FORMAT, S_FMT_INVALID_FMT_LEN, 
              m_sFmtName.c_str());
         return -1;
    }

    //���ܳ��ȴ�BIT��λת��ΪBYTE��λ
    uFmtLen /= 8;
    m_uFmtLen = uFmtLen;

    return m_uFmtLen;
}
/************ End of The CaculateFmtLen Defining ***************/


/************ The Create Defining ******************************/
// ��������������һ�ֻ�����ʽ
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormat::Create(void)
{
    STRING  sFmtFile;           //��������ļ�ȫ·��
    FILE*   pFmtFile   = NULL;  //�����ļ�ָ��
    int     nWriteLen  = 0;     //д�ļ����ݵĳ���
    BYTE    pBuff[50];          //��д�ļ�����ʱ���Ȳ���ĵط���д0�Ļ�����
    char    szPath[MAX_PATH];   //�����ļ�ȫ·�����������ļ�����


    //��ȡ�����ļ���ȫ·��
    sFmtFile = m_sFmtRootDir + "/" + m_sFmtName + ".fmt";
    strcpy(szPath, m_sFmtRootDir.c_str());

    //����ָ����ʽ�������ļ�
    pFmtFile = fopen(sFmtFile.c_str(), "wb");
    if(pFmtFile == NULL)
    {
        //�����һ�δ��ļ�ʧ�ܣ��򴴽����ļ���Ŀ¼
        if(!CreateDeepDir(szPath))
        {
            return -1;  //�����ļ�Ŀ¼ʧ��
        }

        //�ٴ�һ���ļ�
        pFmtFile = fopen(sFmtFile.c_str(), "wb");
        if(pFmtFile == NULL)
        {
            return -1;  //�ڶ��δ��ļ�ʧ��
        }
    }

    //������ʽ����д�������ļ�
    memset((void*)pBuff, 0, 50);
    nWriteLen = m_sFmtDesc.length() + 1;
    if(nWriteLen > 48)
    {
        nWriteLen = 48;  //��ʽ������󳤶���48���ֽ�
    }
    memcpy((void*)pBuff, (void*)m_sFmtDesc.c_str(), nWriteLen);
    if(fwrite((void*)pBuff, 48, 1, pFmtFile) != 1)
    {
        fclose(pFmtFile);
        return -1;
    }

    //��ԭ��ʽ��д�������ļ�
    memset((void*)pBuff, 0, 50);
    nWriteLen = m_sSrcFmtName.length() + 1;
    if(nWriteLen > 16)
    {
        nWriteLen = 16;  //ԭ��ʽ����󳤶���16���ֽ�
    }
    memcpy((void*)pBuff, (void*)m_sSrcFmtName.c_str(), nWriteLen);
    if(fwrite((void*)pBuff, 16, 1, pFmtFile) != 1)
    {
        fclose(pFmtFile);
        return -1;
    }

    //�رո�ʽ�����ļ�
    fclose(pFmtFile);

    return 0;
}
/************ End of The Create Defining ***********************/


/************ The Delete Defining ******************************/
// ����������ɾ��һ�ֻ�����ʽ
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormat::Delete(void)
{
    STRING  sFmtFile;    //��������ļ�ȫ·��


    //��ȡ�����ļ���ȫ·��
    sFmtFile = m_sFmtRootDir + "/" + m_sFmtName + ".fmt";

    //ɾ��ָ���������ļ�
    if(remove(sFmtFile.c_str()) != 0)
    {
        return -1;
    }

    return 0;
}
/************ End of The Delete Defining ***********************/


/************ The GetFmtName Defining **************************/
// ������������ñ���ʽ��
//
// �����������
//
// ���������
//   sFmtName������ʽ������
//
//   ����ֵ����
//
/***************************************************************/
void CFormat::GetFmtName(STRING& sFmtName) const
{
    sFmtName = m_sFmtName;
}
/************ End of The GetFmtName Defining *******************/

const char* CFormat::GetFmtName() const
{
    return (m_sFmtName.c_str());
}


/************ The SetSrcFmt Defining ***************************/
// �������������ñ���ʽ��Ӧ��Դ��ʽ����
//
// ���������
//    pSrcFmt������ʽ��Ӧ��Դ��ʽ������CFormat�ڲ������ͷŸ�ָ��
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormat::SetSrcFmt(CFormat* pSrcFmt)
{
    m_pSrcFmt = pSrcFmt;

    //�������ʽ�����ڱ���ʽ��Ӧ��ԭ��ʽ
    if(m_sFmtName != m_sSrcFmtName)
    {
        //ֻ��Ŀ���ʽ��������Դ��ʽ��ʱ������Ҫ��ʽת������
        if(m_pConvert != NULL)
        {
            delete m_pConvert;
            m_pConvert = NULL;
        }
        m_pConvert = new CConvert;
        int nRet = m_pConvert->Init(this, m_pSrcFmt);
        if(nRet != 0)
        {
            delete m_pConvert;
            m_pConvert = NULL;

            return -1;
        }
    }

    return 0;
}
/************ End of The SetSrcFmt Defining ********************/


/************ The GetSrcFmt Defining ***************************/
// ������������ñ���ʽ��Ӧ��Դ��ʽ����
//
// �����������
//
// �����������
//
//   ����ֵ������ʽ��Ӧ��Դ��ʽ���󣬱������ĵ����߲����ͷ�
//
/***************************************************************/
CFormat* CFormat::GetSrcFmt(void) const
{
    return m_pSrcFmt;
}
/************ End of The GetSrcFmt Defining ********************/


/************ The GetSrcFmtName Defining ***********************/
// ������������ñ���ʽ��Դ��ʽ��
//
// �����������
//
// ���������
//  sSrcFmtName������ʽ��Դ��ʽ��
//
//   ����ֵ����
//
/***************************************************************/
void CFormat::GetSrcFmtName(STRING& sSrcFmtName) const
{
    sSrcFmtName = m_sSrcFmtName;
}
/************ End of The GetSrcFmtName Defining ****************/

const char* CFormat::GetSrcFmtName() const
{
    return (m_sSrcFmtName.c_str());
}
/************ The GetFmtLen Defining ***************************/
// ������������ñ���ʽ��Ӧ�ĵ��Ż������ȣ��ֽ�����
//
// �����������
//
// �����������
//
//   ����ֵ������ʽ��Ӧ�ĵ��Ż������ȣ��ֽ�����
//
/***************************************************************/
UINT4 CFormat::GetFmtLen(void) const
{
    return m_uFmtLen;
}

UINT4 CFormat::GetSrcFmtLen(void) const
{
    if (m_pSrcFmt != NULL)
    {
        return m_pSrcFmt->GetFmtLen();
    }

    return 0;
}

/************ End of The GetFmtLen Defining ********************/


/************ The GetFieldNum Defining *************************/
// ������������ñ���ʽ�����
//
// �����������
//
// �����������
//
//   ����ֵ������ʽ�����
//
/***************************************************************/
UINT4 CFormat::GetFieldNum(void) const
{
    return m_uFieldNum;
}
/************ End of The GetFieldNum Defining ******************/


/************ The GetFieldsMap Defining ************************/
// ������������ñ���ʽ���л�����MAPָ��
//
// �����������
//
// �����������
//
//   ����ֵ������ʽ������MAPָ�룬�����߲����ͷŻ�õ�FIELD_MAP*ָ��
//
/***************************************************************/
const FIELD_MAP* CFormat::GetFieldsMap(void) const
{
    return m_pFieldsMap;
}
/************ End of The GetFieldsMap Defining *****************/


/************ The GetFieldsArray Defining **********************/
// ������������ñ���ʽ���л���������ָ��
//
// �����������
//
// �����������
//
//   ����ֵ������ʽ����������ָ�룬�����߲����ͷŻ�õ�SFieldInfo*ָ��
//
/***************************************************************/
const SFieldInfo** CFormat::GetFieldsArray(void) const
{
    return (const SFieldInfo**)m_pFieldsArray;
}
/************ End of The GetFieldsArray Defining ***************/


/************ The GetFormatGen Defining ************************/
// ������������ñ���ʽ�ĸ�ʽ������Դ��ʽ��
//
// �����������
//
// ���������
//    sFmtDesc������ʽ�ĸ�ʽ����
// sSrcFmtName������ʽ��Ӧ��Դ��ʽ��
//
//   ����ֵ����
//
/***************************************************************/
void CFormat::GetFormatGen(STRING& sFmtDesc, STRING& sSrcFmtName) const
{
    sFmtDesc = m_sFmtDesc;
    sSrcFmtName = m_sSrcFmtName;
}
/************ End of The GetFormatGen Defining *****************/


/************ The SetFormatGen Defining ************************/
// �������������ñ���ʽ�ĸ�ʽ������Դ��ʽ��
//
// ���������
//    sFmtDesc������ʽ�ĸ�ʽ����
// sSrcFmtName������ʽ��Ӧ��Դ��ʽ��
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
void CFormat::SetFormatGen(const STRING& sFmtDesc,  const STRING& sSrcFmtName)
{
    m_sFmtDesc = sFmtDesc;
    m_sSrcFmtName = sSrcFmtName;
}
/************ End of The SetFormatGen Defining *****************/


/************ The SetDistingCondition Defining *****************/
// �������������ñ���ʽ��ʶ������
//
// ���������
//  sDistingCondition����ʽʶ������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
void CFormat::SetDistingCondition(const STRING& sDistingCondition)
{
    m_sDistingCondition = sDistingCondition;
}
/************ End of The SetDistingCondition Defining **********/


/************ The GetDistingCondition Defining *****************/
// ������������ñ���ʽ��ʶ������
//
// �����������
//
// ���������
//  sDistingCondition����ʽʶ������
//
//   ����ֵ����
//
/***************************************************************/
void CFormat::GetDistingCondition(STRING&  sDistingCondition) const
{
    sDistingCondition = m_sDistingCondition;
}
/************ End of The GetDistingCondition Defining **********/


/************ The IsTheFormat Defining *************************/
// �����������жϴ����һ�Ż����Ƿ���ϱ���ʽ
//
// ���������
//     pABill������ĵ��Ż���
//       uLen��һ�Ż����ĳ��ȣ���pABill��ռ�ڴ��С
//
// �����������
//
//   ����ֵ�����ϱ���ʽ����0�������ϱ���ʽ����1��������-1
//
/***************************************************************/
int CFormat::IsTheFormat(const void* pABill,  const UINT4 uLen)
{
    SVariant  Var;   //�������ʽ����������

    if (m_pSrcFmt != this)
    {
        return m_pSrcFmt->IsTheFormat(pABill, uLen);
    }

    //�����ϱ���ʽ
    if((pABill == NULL) || (m_uFmtLen != uLen))
    {
        return 1;
    }

    //����ʶ���������ʽ
    if(m_pDistingCondition->Caculate(pABill, uLen, Var) != 0)
    {
        //assert(0);
        return -1;
    }

    //�����ϱ���ʽ
    if(Var.n4Value == 0)
    {
        return 1;
    }

    //���ϱ���ʽ
    return 0;
}
/************ End of The IsTheFormat Defining ******************/


/************ The ConvertABill Defining ************************/
// �����������������һ�Ż���ת����Ŀ���ʽ
//
// ���������
//   pASrcBill������ĵ��Ż���
// uSrcBillLen��һ�Ż����ĳ��ȣ���pABill��ռ�ڴ��С
//
// ���������
//   pADestBill��ת����Ļ�����ָ����ڴ��ɱ��������䣬�ɱ������ĵ����߸����ͷ�
// uDestBillLen��ת����Ļ�������
//
//   ����ֵ����ȷת������0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormat::ConvertABill(const void* pASrcBill, const UINT4 uSrcBillLen,
                          void*& pADestBill, UINT4& uDestBillLen)
{
    //��ʼ���������
    pADestBill = NULL;
    uDestBillLen  = 0;

    //ȷ����������ĺϷ���
    if((pASrcBill == NULL) || (uSrcBillLen == 0))
    {
        return -1;
    }

    //���Ŀ���ʽ����ԭ��ʽ
    if(m_sFmtName == m_sSrcFmtName)
    {
        pADestBill = new BYTE[uSrcBillLen];
        uDestBillLen = uSrcBillLen;
        memcpy(pADestBill, pASrcBill, uSrcBillLen);
    }

    //Ŀ���ʽ������ԭ��ʽ
    else
    {
        //����Convert�����ԭ��ʽת����Ŀ���ʽ
        int nRet = m_pConvert->ConvertABill(pASrcBill, uSrcBillLen, pADestBill, uDestBillLen);
        if(nRet != 0)
        {
            return -1;
        }
    }

    return 0;
}
/************ End of The ConvertABill Defining *****************/


/************ The GetFieldValueByIndex Defining ****************/
// �������������ݸ�ʽ��������һ�Ż����л�ø����ֵ
//
// ���������
//     pABill������ĵ��Ż���
//   bThisFmt������Ļ����Ƿ���ϱ���ʽ��0��ʾ���ϱ���ʽ��
//             �����ʾ���ϱ���ʽ��ԭ��ʽ
//     uIndex����ʽ����������SFieldInfo�ṹ�е�nFieldIndex��Ӧ
//     pDestFI��Ŀ���ʽ���ֶ���Ϣ��Ϊ���ܹ�ʵ�ֶ�����ȡö��ֵ�Ĺ��ܣ�ȱʡֵΪNULL
//
// ���������
// pFieldValue����õ���ֵ
//        uLen�����򳤶�
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
//�޸ļ�¼��
// 2004-02-18  SWPD03631 ���Ӳ���pDestFIʵ�ֶ�����ȡö��ֵ ZhengYuqun
/***************************************************************/
int CFormat::GetFieldValueByIndex(const void* pABill, const int bThisFmt,
                                  const UINT4 uIndex,
                                  void* pFieldValue, UINT4& uLen, SFieldInfo* pDestFI)
{
    SFieldInfo*  pFI   = NULL;   //������ָ��
    BYTE* pTmpStr = NULL;        //ָ�򻰵����ݵ���ʱָ��


    //��ʼ����������ͱ���
    //pFieldValue = NULL;
    uLen = 0;
    pTmpStr = (BYTE*)pABill;

    //���Ǳ���ʽ
    if(bThisFmt != 0)
    {
        //����Դ��ʽ
        int retVal = m_pSrcFmt->GetFieldValueByIndex(pABill, 0, uIndex, pFieldValue, uLen);
        return retVal;
    }

    //���ָ����������������Χ�������뻰������Ϊ��
    if((uIndex >= m_uFieldNum) || (pABill == NULL))
    {
        return -1;
    }

    pFI = m_pFieldsArray[uIndex];

    //������λ����
    if(pFI->flgBit == 0)
    {
        uLen = GetFieldTypeLen(pFI) * pFI->nFieldLength;
        //pFieldValue = new BYTE[uLen];
        memcpy(pFieldValue, (void*)(pTmpStr + pFI->nByteOffset), uLen);
        //added by maimaoshi,2002-10-26
        ConvertToHostOrder(pFI, pFieldValue);
        //end
    }

    //������λ����
    else
    {
        uLen = GetFieldTypeLen(pFI);
        //pFieldValue = new BYTE[uLen];
        memcpy(pFieldValue, (void*)(pTmpStr + pFI->nByteOffset), uLen);

        //added by maimaoshi,2002-10-26
        ConvertToHostOrder(pFI, pFieldValue);
        //end
        //�����λ��Ӧ������λ������λ��λ��
        //Modified by XDB on 2002.10.28
        //int nRShift = pFI->nBitOffset - (pFI->nByteOffset * 8);  //����λ��
        int nRShift = 0;  //����λ��
        if(pFI->nOrderMode == INTEL_ORDER)
        {
            nRShift = pFI->nBitOffset - (pFI->nByteOffset * 8);
        }
        else
        {
            nRShift = (uLen * 8) - (pFI->nBitOffset - (pFI->nByteOffset * 8))
                      - pFI->nFieldLength;
        }
        //End of Modify by XDB

        //������BYTE����
        if(uLen == LEN_BYTE)
        {
            //����nRShiftλ
            //BYTE nTmpNum = *(BYTE*)pFieldValue;
            BYTE nTmpNum = 0;
            memcpy(&nTmpNum, pFieldValue, LEN_BYTE);
            nTmpNum = nTmpNum >> nRShift;

            //ֻȡ��pFI->nFieldLengthλ
            nTmpNum &= ((1 << pFI->nFieldLength) -1);
            memcpy(pFieldValue, &nTmpNum, LEN_BYTE);
            //*(BYTE*)pFieldValue = nTmpNum;
        }

        //������UINT2��INT2����
        else if(uLen == LEN_UINT2)
        {
            //����nRShiftλ
            //UINT2 nTmpNum = *(UINT2*)pFieldValue;
            UINT2 nTmpNum = 0;
            memcpy(&nTmpNum, pFieldValue, LEN_UINT2);
            nTmpNum = nTmpNum >> nRShift;

            //ֻȡ��pFI->nFieldLengthλ
            nTmpNum &= ((1 << pFI->nFieldLength) -1);
            memcpy(pFieldValue, &nTmpNum, LEN_UINT2);
            //*(UINT2*)pFieldValue = nTmpNum;
        }

        //������UINT4��INT4����
        else if(uLen == LEN_UINT4)
        {
            //����nRShiftλ
            //UINT4 nTmpNum = *(UINT4*)pFieldValue;
            UINT4 nTmpNum = 0;
            memcpy(&nTmpNum, pFieldValue, LEN_UINT4);
            nTmpNum = nTmpNum >> nRShift;

            //ֻȡ��pFI->nFieldLengthλ
            nTmpNum &= ((1 << pFI->nFieldLength) -1);
            memcpy(pFieldValue, &nTmpNum, LEN_UINT4);
            //*(UINT4*)pFieldValue = nTmpNum;
        }

        //�����ǷǷ�ö������
        else
        {
            //delete[] (BYTE*)pFieldValue;
            //pFieldValue = NULL;
            uLen = 0;

            return -1;
        }
    }

    //Added by ZhengYuqun 2004-02-18 SWPD03631 ������ȡö��
    if(pDestFI)
    {
        ENUMINFO_LIST* pEnumInfos = pDestFI->pEnumInfos;
        if(pEnumInfos && pFI->nDataType != FT_CHAR && pFI->nDataType != FT_STRING)
        {
            if(uLen <= sizeof(INT4))
            {
                INT4 nEnumVal = 0;
                memcpy((BYTE*)&nEnumVal, pFieldValue, uLen);
                ENUMINFO_LIST::iterator item;
                //˳��������ʽ��ö�������е�ÿһ���ڵ�
                item = pEnumInfos->begin();
                while(item != pEnumInfos->end())
                {
                    //�����ǰ�ڵ��ö��ֵ������Ҫ��ö��ֵ
                    if(item->nEnumValue == nEnumVal)
                    {
                        //����ǰö��ֵ��Ӧ��ö���ַ���ת��Ϊ����
                        nEnumVal = atoi(item->szEnumValue);
                        memcpy(pFieldValue, (BYTE*)&nEnumVal, uLen);
                        break;
                    }
                    item++;
                }         
            }    
        }
    }
    //End SWPD03631

    return 0;
}
/************ End of The GetFieldValueByIndex Defining *********/


/************ The GetFieldValueByName Defining *****************/
// �������������ݻ���������һ�Ż����л�ø����ֵ
//
// ���������
//     pABill������ĵ��Ż���
//   bThisFmt������Ļ����Ƿ���ϱ���ʽ��0��ʾ���ϱ���ʽ��
//             ��������Ҫת����Ŀ���ʽ
//     uIndex����ʽ��������SFieldInfo�ṹ�е�nFieldIndex��Ӧ
//   nIsQuery���Ƿ��ѯ��־��1��ʾ�ú���Ϊ��ѯ����ת����
//             0��ʾ����Ϊ��ѯ����ת��
//
// ���������
// pFieldValue����õ���ֵ
//        uLen�����򳤶�
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormat::GetFieldValueByName(const void* pABill, const int bThisFmt,
                                 const STRING& sFieldName,
                                 void*& pFieldValue, UINT4& uLen,
                                 int nIsQuery)
{
    SFieldInfo*  pFI   = NULL;   //������ָ��
    BYTE*  pTmpStr     = NULL;   //ָ�򻰵����ݵ���ʱָ��
    FIELD_MAP::iterator item;    //ָ��������������ӳ���еĽڵ�


    //��ʼ����������ͱ���
    pFieldValue = NULL;
    uLen = 0;
    pTmpStr = (BYTE*)pABill;

    //ȷ������Ļ������ݲ�Ϊ��
    if((pABill == NULL) || (sFieldName.length() == 0))
    {
        return -1;
    }

    //���Ǳ���ʽ,�������ʽת������Ϊ�գ���ʾԭ��ʽ����Ŀ���ʽ
    if((bThisFmt != 0) && (m_pConvert != NULL))
    {
        //ת����Ŀ���ʽ
        int nRet = m_pConvert->ConvertAField(pABill, sFieldName, pFieldValue, uLen, nIsQuery);
        if(nRet != 0)
        {
            return -1;
        }

        return 0;
    }

    //��������ӳ���ϵ������������ָ������ڵ�
    item = m_pFieldsMap->find(sFieldName);
    if(item == m_pFieldsMap->end())
    {
        //����ʽ�����в�����ָ��������
        return -1;
    }

    //���ݸ����Ӧ����������ȡ��ֵ
    pFI = item->second;
    uLen = GetFieldTypeLen(pFI) * pFI->nFieldLength;
    pFieldValue = new BYTE[uLen];
    if(GetFieldValueByIndex(pABill, 0, pFI->nFieldIndex, pFieldValue, uLen) != 0)
    {
        delete[] (BYTE*)pFieldValue;
        pFieldValue = NULL;
        uLen = 0;
        return -1;
    }

    return 0;
}
/************ End of The GetFieldValueByName Defining **********/

/***************************************************************/
// �������������ݻ�����������һ�Ż����л�ø����ֵ
//
// ���������
//     pABill������ĵ��Ż���
//   bThisFmt������Ļ����Ƿ���ϱ���ʽ��0��ʾ���ϱ���ʽ��
//             ��������Ҫת����Ŀ���ʽ
//     uIndex����ʽ��������SFieldInfo�ṹ�е�nFieldIndex��Ӧ
//   nIsQuery���Ƿ��ѯ��־��1��ʾ�ú���Ϊ��ѯ����ת����
//             0��ʾ����Ϊ��ѯ����ת��
//
// ���������
// pFieldValue����õ���ֵ�����ⲿ�����ڴ�
//        uLen�����򳤶�
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormat::GetFieldContentByIndex(const void* pABill, const int bThisFmt,
					   			    UINT2 nFieldIndex,  
								    void* pFieldValue, UINT4& uLen,
								    int nIsQuery)
{
    SFieldInfo*  pFI   = NULL;   //������ָ��
    BYTE*  pTmpStr     = NULL;   //ָ�򻰵����ݵ���ʱָ��	
	
    //��ʼ����������ͱ���
    uLen = 0;
    pTmpStr = (BYTE*)pABill;
	
    //ȷ������Ļ������ݲ�Ϊ��
    if(pABill == NULL)
    {
        return -1;
    }
	
    //���Ǳ���ʽ,�������ʽת������Ϊ�գ���ʾԭ��ʽ����Ŀ���ʽ
    if((bThisFmt != 0) && (m_pConvert != NULL))
    {
        //ת����Ŀ���ʽ
        int nRet = m_pConvert->ConvertAFieldByIndex(pABill, nFieldIndex,
                                                    pFieldValue, uLen, nIsQuery);
        if(nRet != 0)
        {
            return -1;
        }
		
        return 0;
    }
	
    //��������ӳ���ϵ��������Ų���ָ������ڵ�
    pFI = m_pFieldsArray[nFieldIndex];
    if(NULL == pFI)
    {
        //����ʽ�����в�����ָ��������
        return -1;
    }
	
    //���ݸ����Ӧ����������ȡ��ֵ 
    uLen = GetFieldTypeLen(pFI) * pFI->nFieldLength;
    if(GetFieldValueByIndex(pABill, 0, nFieldIndex, pFieldValue, uLen) != 0)
    {
        uLen = 0;
        return -1;
    }
	
    return 0;
}
/***************************************************************/


/************ The IsTheQueryCondition Defining *****************/
// �����������ж�һ�Ż����Ƿ����ָ���Ĳ�ѯ����
//
// ���������
//   pASrcBill������ĵ��Ż���
//  sQueryCond����ѯ��������׺���ʽ
//
// �����������
//
//   ����ֵ�����ϸ�ʽ����0�������Ϸ���1��������-1
//
/***************************************************************/
int CFormat::IsTheQueryCondition(const void* pASrcBill,const STRING& sQueryCond)
{
    SVariant RetVar;    //�������ʽ����������

    //ȷ�������������ȷ��
    if((pASrcBill == NULL)|| ((m_sQueryCond.length() == 0)
        && (sQueryCond.length() == 0)))
    {
        return -1;
    }

    //����������ʽ������Ϊ�գ�����׺���ʽת��Ϊ��׺���ʽ
    if(sQueryCond.length() != 0)
    {
        if(m_pQueryCondition->Infix2Postfix(sQueryCond.c_str()) != 0)
        {
            return -1;
        }
        m_sQueryCond = sQueryCond;
    }

    //�����������ʽ
    if(m_pQueryCondition->Caculate(pASrcBill, m_uFmtLen, RetVar) != 0)
    {
        //assert(0);
        return -1;
    }

    //����������
    if(RetVar.n4Value == 0)
    {
        return 1;
    }

    //��������
    else
    {
        return 0;
    }
}
/************ End of The IsTheQueryCondition Defining **********/


/************ The GetFieldTypeLen Defining **********************/
// ������������ȡ���������͵ĳ���
//
// ���������
//   pFI��������ʽ������
//
// �����������
//
//   ����ֵ���ɹ����ػ��������͵ĳ��ȣ�ʧ�ܷ���-1
//
/***************************************************************/
int CFormat::GetFieldTypeLen(const SFieldInfo* pFI)
{
    int nFieldTypeLen = 0;   //�����ͳ��ȱ���

    //��ȡ��ǰ�����͵ĳ���
    if(pFI->nDataType == FT_CHAR)
    {
        nFieldTypeLen = sizeof(char);
    }
    else if(pFI->nDataType == FT_BYTE)
    {
        nFieldTypeLen = sizeof(BYTE);
    }
    else if(pFI->nDataType == FT_SHORT)
    {
        nFieldTypeLen = sizeof(INT2);
    }
    else if(pFI->nDataType == FT_USHORT)
    {
        nFieldTypeLen = sizeof(UINT2);
    }
    else if(pFI->nDataType == FT_LONG)
    {
        nFieldTypeLen = sizeof(INT4);
    }
    else if(pFI->nDataType == FT_ULONG)
    {
        nFieldTypeLen = sizeof(UINT4);
    }
    else if(pFI->nDataType == FT_INT8)
    {
        nFieldTypeLen = sizeof(INT8);
    }
    else if((pFI->nDataType == FT_STRING) || (pFI->nDataType == FT_BCD)
            || (pFI->nDataType == FT_RBCD))
    {
        nFieldTypeLen = sizeof(BYTE);
    }

    //������ǺϷ���������
    else
    {
        return -1;
    }

    return nFieldTypeLen;
}
/************ End of The GetFieldTypeLen Defining ***********/


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
int CFormat::Commit(void)
{
    STRING  sFmtFile;           //��������ļ�ȫ·��
    FILE*   pFmtFile   = NULL;  //�����ļ�ָ��
    int     nWriteLen  = 0;     //д�ļ����ݵĳ���
    BYTE    pBuff[100];         //��д�ļ�����ʱ���Ȳ���ĵط���д0�Ļ�����
    UINT2   uTmpNum = 0;        //д�����ֽڳ��ȵ���������
    char    szPath[MAX_PATH];   //�����ļ�ȫ·�����������ļ�����


    //��ȡ�����ļ���ȫ·��
    sFmtFile = m_sFmtRootDir + "/" + m_sFmtName + ".fmt";
    strcpy(szPath, m_sFmtRootDir.c_str());

    //��ָ����ʽ�������ļ�
    pFmtFile = fopen(sFmtFile.c_str(), "wb");
    if(pFmtFile == NULL)
    {
        //�����һ�δ��ļ�ʧ�ܣ��򴴽����ļ���Ŀ¼
        if(!CreateDeepDir(szPath))
        {
            return -1;  //�����ļ�Ŀ¼ʧ��
        }

        //�ٴ�һ���ļ�
        pFmtFile = fopen(sFmtFile.c_str(), "wb");
        if(pFmtFile == NULL)
        {
            return -1;  //�ڶ��δ��ļ�ʧ��
        }
    }

    //������ʽ����д�������ļ�
    memset((void*)pBuff, 0, 100);
    nWriteLen = m_sFmtDesc.length() + 1;
    if(nWriteLen > LEN_FMT_DESC)
    {
        nWriteLen = LEN_FMT_DESC;  //��ʽ������󳤶���48���ֽ�
    }
    memcpy((void*)pBuff, (void*)m_sFmtDesc.c_str(), nWriteLen);
    if(fwrite((void*)pBuff, LEN_FMT_DESC, 1, pFmtFile) != 1)
    {
        fclose(pFmtFile);
        return -1;
    }

    //��ԭ��ʽ��д�������ļ�
    memset((void*)pBuff, 0, 100);
    nWriteLen = m_sSrcFmtName.length() + 1;
    if(nWriteLen > LEN_FMT_SRC_FMT_NAME)
    {
        nWriteLen = LEN_FMT_SRC_FMT_NAME;  //ԭ��ʽ����󳤶���16���ֽ�
    }
    memcpy((void*)pBuff, (void*)m_sSrcFmtName.c_str(), nWriteLen);
    if(fwrite((void*)pBuff, LEN_FMT_SRC_FMT_NAME, 1, pFmtFile) != 1)
    {
        fclose(pFmtFile);
        return -1;
    }

    //��ʶ������д�������ļ�
    memset((void*)pBuff, 0, 100);
    nWriteLen = m_sDistingCondition.length() + 1;
    if(nWriteLen > LEN_FMT_DIS_CONDITION)
    {
        nWriteLen = LEN_FMT_DIS_CONDITION;  //ʶ��������󳤶���64���ֽ�
    }
    memcpy((void*)pBuff, (void*)m_sDistingCondition.c_str(), nWriteLen);
    if(fwrite((void*)pBuff, LEN_FMT_DIS_CONDITION, 1, pFmtFile) != 1)
    {
        fclose(pFmtFile);
        return -1;
    }

    //������������д�������ļ�
    uTmpNum = m_uFieldNum;  //����������ռ�������ֽ�
    if(fwrite((void*)&uTmpNum, LEN_UINT2, 1, pFmtFile) != 1)
    {
        fclose(pFmtFile);
        return -1;
    }

    //��ÿһ��������������Ϣд�������ļ�
    SFieldInfo* pFI = NULL;
    ENUMINFO_LIST::iterator it;
    SEnumInfo SNIF;
    for(int i = 0; i < m_uFieldNum; i++)
    {
        pFI = m_pFieldsArray[i];

        //���浱ǰ��SFieldInfo�ṹ��Ϣ
        if(fwrite((void*)pFI, sizeof(SFieldInfo), 1, pFmtFile) != 1)
        {
            fclose(pFmtFile);
            return -1;
        }

        //�����ǰ����ö�����ͣ��򱣴���������ö��ֵ
        if((pFI->flgEnum != 0) && (pFI->pEnumInfos != NULL))
        {
            //����ǰ���ö�ٸ���д�������ļ�
            uTmpNum = pFI->pEnumInfos->size();  //ö�ٸ���ռ�������ֽ�
            if(fwrite((void*)&uTmpNum, LEN_UINT2, 1, pFmtFile) != 1)
            {
                fclose(pFmtFile);
                return -1;
            }

            it = pFI->pEnumInfos->begin();
            while(it != pFI->pEnumInfos->end())
            {
                //���浱ǰö��ֵ
                SNIF = *it;
                if(fwrite((void*)&SNIF, sizeof(SEnumInfo), 1, pFmtFile) != 1)
                {
                    fclose(pFmtFile);
                    return -1;
                }

                it++;
            }
        }
    }

    //�رո�ʽ�����ļ�
    fclose(pFmtFile);

    return 0;
}
/************ End of The Commit Defining ***********************/


/************ The SetQueryCond Defining ************************/
// �������������ò�ѯ����
//
// ���������
//  sQueryCond����ѯ����
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormat::SetQueryCond(const STRING& sQueryCond)
{
    //ȷ�������������ȷ��
    if(sQueryCond.length() == 0)
    {
        return -1;
    }

    //����׺���ʽת��Ϊ��׺���ʽ
    if(m_pQueryCondition->Infix2Postfix(sQueryCond.c_str()) != 0)
    {
        //assert(0);
        return -1;
    }

    m_sQueryCond = sQueryCond;
    return 0;
}
/************ End of The SetQueryCond Defining *****************/

/***************************************************************************************************
 *�������ܣ��������ֽ���1�ֽڶ����ö����ϢpInת����SEnumInfo�ṹ��
 *����ƽ 2002��4��4�����Ӵ˺���
 *���룺
 *    pIn  ���������ļ��ж�ȡ�İ������ֽ���1�ֽڶ��뱣���SEnumInfo��Ϣ��
 *
 *�����
 *    pOut �� ת�������ƽ̨��ص�SEnumInfo��Ϣ��
 *���أ�
 *    �ޡ�
 ***************************************************************************************************/
void CFormat::SerialEnumInfo(SEnumInfo* pOut, BYTE* pIn)
{
    if ((pOut == NULL) || (pIn == NULL))
    {
        return;
    }

    memcpy(&(pOut->nEnumValue), pIn, 2);
    pIn += 2;

    memcpy(pOut->szEnumValue, pIn, 30);

    pOut->nEnumValue = NTOHS(pOut->nEnumValue);
}

/***************************************************************************************************
 *�������ܣ��������ֽ���1�ֽڶ����ö����ϢpInת����SFieldInfo�ṹ��
 *����ƽ 2002��4��4�����Ӵ˺���
 *���룺
 *    pIn  ���������ļ��ж�ȡ�İ������ֽ���1�ֽڶ��뱣���SFieldInfo��Ϣ��
 *
 *�����
 *    pOut �� ת�������ƽ̨��ص�SFieldInfo��Ϣ��
 *���أ�
 *    �ޡ�
 ***************************************************************************************************/
void CFormat::SerialFieldInfo(SFieldInfo* pOut, BYTE* pIn)
{
    if ((pOut == NULL) || (pIn == NULL))
    {
        return;
    }

    memcpy(&(pOut->nFieldIndex), pIn, 2);
    pIn += 2;

    memcpy(pOut->szFieldName, pIn, 64);
    pIn += 64;

    memcpy(pOut->szFieldDescribe, pIn, 64);
    pIn += 64;

    memcpy(&(pOut->nDataType), pIn, 1);
    pIn += 1;

    memcpy(&(pOut->nQueryCondDataType), pIn, 1);
    pIn += 1;

    memcpy(&(pOut->flgBit), pIn, 1);
    pIn += 1;

    memcpy(&(pOut->nFieldLength), pIn, 2);
    pIn += 2;

    memcpy(&(pOut->flgEnum), pIn, 1);

    pIn += (1+4);

    memcpy(pOut->szConvertFun, pIn, 128);
    pIn += 128;

    memcpy(&(pOut->nAlignMode), pIn, 1);
    pIn += 1;

    memcpy(&(pOut->cFillChar), pIn, 1);
    pIn += 1;

    memcpy(&(pOut->cSeparator), pIn, 1);
    pIn += 1;

    memcpy(&(pOut->nBitOffset), pIn, 4);
    pIn += 4;

    memcpy(&(pOut->nByteOffset), pIn, 4);
    pIn += 4;

    memcpy(&(pOut->flgQuery), pIn, 1);
    pIn += 1;

    memcpy(&(pOut->nOperator), pIn, 1);
    pIn += 1;

    memcpy(pOut->szUnit, pIn, 5);
    pIn += 5;

    memcpy(&(pOut->nOrderMode), pIn, 1);

    pOut->nFieldIndex   = NTOHS(pOut->nFieldIndex);
    pOut->nFieldLength  = NTOHS(pOut->nFieldLength);

    pOut->nBitOffset    = NTOHL(pOut->nBitOffset);
    pOut->nByteOffset   = NTOHL(pOut->nByteOffset);
}

//�������ʽ����ȡ����
int CFormat::ParseExpress(const char* const  szExpress, LIST<STRING>& FieldList)
{
    return m_pStatCondition->ParseExpress(szExpress, FieldList);
}

/******************************************************************************
 *�������ܣ����ֽ�����޸ģ���INTEL���ߵ����籾���ֽ���
 *
 *���룺
 *    SFieldInfo *pFI  :SFieldInfo��Ϣ
 *    void *pFieldValue:ת��ǰ��ֵ
 *
 *�����
 *    void *pFieldValue:ת��ǰ��ֵ
 *���أ�
 *    �ޡ�
 *added by maimaoshi,2002-10-24
 *Modified by XDB on 2002.10.28
 ******************************************************************************/
int CFormat::ConvertToHostOrder(const SFieldInfo* pFI, void* pFieldValue)
{
    //��Ҫ���϶��ֽ�����޸ģ���INTEL���ߵ����籾���ֽ���
    //INTEL_ORDER   0;��ʾ��ʽΪINTEL�ֽ���
    //NET_ORDER     1;��ʾ��ʽΪ�����ֽ���
    INT2  nShortValue = 0;
    INT4  nIntValue = 0;

    if((pFI == NULL) || (pFieldValue == NULL))
    {
        return -1;
    }

    if ((pFI->nDataType == FT_SHORT) || (pFI->nDataType == FT_USHORT))
    {
        //����2�ֽڵ����ʹ���
        if (pFI->nOrderMode == INTEL_ORDER)
        {
            //ԭ��ʽΪINTEL�ֽ�����Ҫ��INTEL�ֽ���תΪ�����ֽ���
            memcpy(&nShortValue, pFieldValue, LEN_INT2);
            nShortValue = ITOHS(nShortValue);
            memcpy(pFieldValue, &nShortValue, LEN_INT2);
        }
        else
        {
            //ԭ��ʽΪ�����ֽ�����Ҫ�������ֽ���תΪ�����ֽ���
            memcpy(&nShortValue, pFieldValue, LEN_INT2);
            nShortValue = NTOHS(nShortValue);
            memcpy(pFieldValue, &nShortValue, LEN_INT2);
        }
    }

    else if ((pFI->nDataType == FT_LONG) || (pFI->nDataType == FT_ULONG))
    {
        //����4�ֽڵ����ʹ���
        if (pFI->nOrderMode == INTEL_ORDER)
        {
            //ԭ��ʽΪINTEL�ֽ�����Ҫ��INTEL�ֽ���תΪ�����ֽ���
            memcpy(&nIntValue, pFieldValue, LEN_INT4);
            nIntValue = ITOHL(nIntValue);
            memcpy(pFieldValue, &nIntValue, LEN_INT4);
        }
        else
        {
            //ԭ��ʽΪ�����ֽ�����Ҫ�������ֽ���תΪ�����ֽ���
            memcpy(&nIntValue, pFieldValue, LEN_INT4);
            nIntValue = NTOHL(nIntValue);
            memcpy(pFieldValue, &nIntValue, LEN_INT4);
        }
    }

    else if (pFI->nDataType == FT_INT8)
    {
        INT8 nIntValue8;
        //����8�ֽڵ����ʹ���
        if (pFI->nOrderMode == INTEL_ORDER)
        {
            //ԭ��ʽΪINTEL�ֽ�����Ҫ��INTEL�ֽ���תΪ�����ֽ���
            memcpy(&nIntValue8, pFieldValue, LEN_INT8);
            nIntValue8 = ITOHLL(nIntValue8);
            memcpy(pFieldValue, &nIntValue8, LEN_INT8);
        }
        else
        {
            //ԭ��ʽΪ�����ֽ�����Ҫ�������ֽ���תΪ�����ֽ���
            memcpy(&nIntValue8, pFieldValue, LEN_INT8);
            nIntValue8 = NTOHLL(nIntValue8);
            memcpy(pFieldValue, &nIntValue8, LEN_INT8);
        }
    }

    return 0;
}

