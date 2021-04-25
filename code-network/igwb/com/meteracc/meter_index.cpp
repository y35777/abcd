#include "meter_index.h"
#include "meter_acc.h"
#include "resource.h"

CMeterAccIndex::CMeterAccIndex()
{
    m_pFile             = NULL;
    m_szFileName[0]   = '\0';
}

CMeterAccIndex::~CMeterAccIndex()
{
    Close();
}

void CMeterAccIndex::Close(void)
{
    if (m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}

/*
  ��������	�û��ƴα��������ļ���
  �������	
        pszFileName : ��������ļ���

  �������	
        ��

  ����ֵ  	
        0           : ��ʼ���ɹ�
        -1          : ��ʼ��ʧ��
*/
int CMeterAccIndex::Init(const char* pszFileName)
{
    if (NULL == pszFileName)
    {
        return -1;
    }

    strncpy(m_szFileName, pszFileName, MAX_PATH);
    m_szFileName[MAX_PATH - 1] = '\0';

    Close();

    //�Ȱ��ļ��Ѵ��ڵķ�ʽ��
    m_pFile = fopen(pszFileName, "rb+");
    if (NULL == m_pFile)
    {
        //����ļ������ڣ��򴴽����ļ�
        if (NULL == (m_pFile = fopen(pszFileName, "wb+")))
        {
            return -1;
        }
    }

    SMeterLocation ml;
    while (!feof(m_pFile))
    {
        if (1 > fread(&ml, sizeof(ml), 1, m_pFile))
        {
            return 0;
        }

        m_lstMeterLoc.push_back(ml);
    }

    return 0;
}

/*
  ��������	ͨ���û������ȡ���û��ƴα��¼�ڱ���ļ��е�λ��
  �������	
        pSubscriber : �û�����
        nLength     : pSubscriber�ĳ���

  �������	
        nLocation   : �û��ƴα��¼�ڱ���ļ���ƫ����

  ����ֵ  	
        0           : ���û�����
        -1          : ���û�Ŀǰû�б�׼�¼  
*/
int CMeterAccIndex::GetIndex(const unsigned char* pSubscriber, 
                                  const int nLength,
                                  long& nLocation)
{
    if (m_lstMeterLoc.size() == 0)
    {
        return -1;
    }

    METER_LOCATION_INDEX::iterator it = m_lstMeterLoc.begin();
    while (it != m_lstMeterLoc.end())
    {
        if (memcmp(pSubscriber, (*it).ySubscriber, nLength) == 0)
        {
            nLocation = (*it).nLocation;
            return 0;
        }

        it++;
    }

    return -1;
}

/*
  ��������	��״̬�ļ�������һ���û���¼����
  �������	
        pSubscriber : �û�����
        nLength     : pSubscriber�ĳ���
        nLocation   : �û��ƴα��¼�ڱ���ļ���ƫ����

  �������	
        ��

  ����ֵ  	
        0           : ��ӳɹ�
        -1          : ���ʧ��  
*/
int CMeterAccIndex::AddIndex(const unsigned char* pSubscriber, 
                                const int nLength, 
                                const long nLocation)
{
    SMeterLocation ml;

    memcpy(ml.ySubscriber, pSubscriber, nLength);
    fseek(m_pFile, 0, SEEK_END);
    ml.nLocation = nLocation;
    if (1 != fwrite(&ml, sizeof(ml), 1, m_pFile))
    {
        TRACE(MTS_METERACC, S_BS_FAIL_WRITE_FILE, m_szFileName);

        return -1;
    }  
	
    fflush(m_pFile);      
    m_lstMeterLoc.push_back(ml);

    return 0;
}
