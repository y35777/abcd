
extern "C"
{

int DeCodeCMCCCSBill_final_mt(void* pGlobal,
                           char* pVal,     /* ����������� */
                           int* pValLen,   /* �������ݳ���,����������ĳ��� */
                           char* pBuf,     /* ����������Ļ����� */
                           int nBufLen,   /* �������ĳ��� */
                           int& nPDU);

char* EnCodeCMCCCSBill_final_mt(void* pGlobal,
                            char* pVal,      /* �����������ָ�� */
                            char* pBuf,      /* ��������������ʹ�õĻ����� */
                            int* pBufLen,    /* ���뻺����pBuf�ĳ��ȣ�*/
                            int& nPDU);  
                                             /* �����ر������ĳ��� (in,out) */

void* Init_OSS_Lib_Final_CMCC_CS();
void Release_OSS_Lib_Final_CMCC_CS(void* pGlobal);

}



