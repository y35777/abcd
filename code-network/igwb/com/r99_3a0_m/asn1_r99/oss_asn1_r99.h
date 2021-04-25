
extern "C"
{
//for motolola encoding and deccoding
int DeCodeBill_ini_M_mt(void* pGlobal,
                        char* pVal,     /* ����������� */
                        int* pValLen,   /* �������ݳ���,����������ĳ��� */
                        char* pBuf,     /* ����������Ļ����� */
                        int nBufLen);   /* �������ĳ��� */

char* EnCodeBill_ini_M_mt(void* pGlobal,
                          char* pVal,      /* �����������ָ�� */
                          char* pBuf,      /* ��������������ʹ�õĻ����� */
                          int* pBufLen);   /* ���뻺����pBuf�ĳ��ȣ�*/
                                           /* �����ر������ĳ��� (in,out) */

int DeCodeBill_final_M_mt(void* pGlobal,
                          char* pVal,     /* ����������� */
                          int* pValLen,   /* �������ݳ���,����������ĳ��� */
                          char* pBuf,     /* ����������Ļ����� */
                          int nBufLen);   /* �������ĳ��� */

char* EnCodeBill_final_M_mt(void* pGlobal,
                            char* pVal,      /* �����������ָ�� */
                            char* pBuf,      /* ��������������ʹ�õĻ����� */
                            int* pBufLen);   /* ���뻺����pBuf�ĳ��ȣ�*/
                                             /* �����ر������ĳ��� (in,out) */

void* Init_OSS_Lib_Ini_M();
void Release_OSS_Lib_Ini_M(void* pGlobal);

void* Init_OSS_Lib_Final_M();
void Release_OSS_Lib_Final_M(void* pGlobal);
}



