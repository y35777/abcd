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
  功能描述	用户计次表表底索引文件。
  输入参数	
        pszFileName : 表底索引文件名

  输出参数	
        无

  返回值  	
        0           : 初始化成功
        -1          : 初始化失败
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

    //先按文件已存在的方式打开
    m_pFile = fopen(pszFileName, "rb+");
    if (NULL == m_pFile)
    {
        //如果文件不存在，则创建新文件
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
  功能描述	通过用户号码获取该用户计次表记录在表底文件中的位置
  输入参数	
        pSubscriber : 用户号码
        nLength     : pSubscriber的长度

  输出参数	
        nLocation   : 用户计次表记录在表底文件的偏移量

  返回值  	
        0           : 该用户存在
        -1          : 该用户目前没有表底记录  
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
  功能描述	向状态文件中增加一条用户记录索引
  输入参数	
        pSubscriber : 用户号码
        nLength     : pSubscriber的长度
        nLocation   : 用户计次表记录在表底文件的偏移量

  输出参数	
        无

  返回值  	
        0           : 添加成功
        -1          : 添加失败  
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
