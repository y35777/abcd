#ifndef _BACKUP_RESOURCE_H_
#define _BACKUP_RESOURCE_H_
	
#ifdef _PLATFORM_WIN32
#define S_ISDIR(mode)   (((mode) & S_IFMT) == S_IFDIR)
#endif


#define MAX_HOST_LEN 24
#define MAX_USERNAME_LEN 255
#define MAX_PASSWORD_LEN 255
#define BK_MAX_INFO_LEN     500
#define ONE_DAY_IN_SEC	86400

#define NAMELIST_END_CHAR       '~'
#define MAX_CLIENTNO            255

typedef LIST<STRING> STRINGLIST;


//���ݵ��ļ���Χ���͵�ö�ٶ���
enum BakRangeType
{
    DATE_DIR  = 0,     //ĳ���ڷ�Χ�ڵĻ���
    FILE_NAME = 1      //ĳ�ļ���Χ�ڵĻ���
};

typedef struct _FilesListNode
{
    time_t tFileTime;               //�ļ��޸�ʱ��
    char szFileName[MAX_PATH];      //�����ļ�����(��·����Ϣ)
    bool operator < (_FilesListNode &rhs)
	{
		if(tFileTime == rhs.tFileTime)
        {
            return strncmp(szFileName, rhs.szFileName, sizeof(szFileName)) < 0;
        }
		
        return tFileTime < rhs.tFileTime;
	}

    bool operator == (_FilesListNode& rhs)
    {
        if(strncmp(szFileName, rhs.szFileName, sizeof(szFileName)) != 0)
        {
            return FALSE;
        }
        
        return tFileTime == rhs.tFileTime;
    }

} SFilesListNode, *PFilesListNode;


#define LISTFILELISTNODE LIST<SFilesListNode>

#endif /* _BACKUP_RESOURCE_H_ */
