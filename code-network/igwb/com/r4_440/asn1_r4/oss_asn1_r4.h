
extern "C"
{

int DeCodeBill_ini_mt(void* pGlobal,
                      char* pVal,     /* ����������� */
                      int* pValLen,   /* �������ݳ���, ����������ĳ��� */
                      char* pBuf,     /* ����������Ļ����� */
                      int nBufLen);   /* �������ĳ��� */

char* EnCodeBill_ini_mt(void* pGlobal,
                        char* pVal,      /* �����������ָ�� */
                        char* pBuf,      /* ��������������ʹ�õĻ����� */
                        int* pBufLen);   /* ���뻺����pBuf�ĳ���,*/
                                         /* �����ر������ĳ��� (in,out) */

int DeCodeBill_final_mt(void* pGlobal,
                        char* pVal,     /* ����������� */
                        int* pValLen,   /* �������ݳ���,����������ĳ��� */
                        char* pBuf,     /* ����������Ļ����� */
                        int nBufLen);   /* �������ĳ��� */

char* EnCodeBill_final_mt(void* pGlobal,
                          char* pVal,      /* �����������ָ�� */
                          char* pBuf,      /* ��������������ʹ�õĻ����� */
                          int* pBufLen);   /* ���뻺����pBuf�ĳ��ȣ�*/
                                           /* �����ر������ĳ��� (in,out) */

void* Init_OSS_Lib_Ini();
void Release_OSS_Lib_Ini(void* pGlobal);

void* Init_OSS_Lib_Final();
void Release_OSS_Lib_Final(void* pGlobal);

}



