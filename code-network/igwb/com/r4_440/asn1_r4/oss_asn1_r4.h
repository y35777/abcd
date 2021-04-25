
extern "C"
{

int DeCodeBill_ini_mt(void* pGlobal,
                      char* pVal,     /* 待解码的数据 */
                      int* pValLen,   /* 输入数据长度, 输出解码结果的长度 */
                      char* pBuf,     /* 存放输出结果的缓冲区 */
                      int nBufLen);   /* 缓冲区的长度 */

char* EnCodeBill_ini_mt(void* pGlobal,
                        char* pVal,      /* 待编码的数据指针 */
                        char* pBuf,      /* 编码结果函数运算使用的缓冲区 */
                        int* pBufLen);   /* 输入缓冲区pBuf的长度,*/
                                         /* 并返回编码结果的长度 (in,out) */

int DeCodeBill_final_mt(void* pGlobal,
                        char* pVal,     /* 待解码的数据 */
                        int* pValLen,   /* 输入数据长度,输出解码结果的长度 */
                        char* pBuf,     /* 存放输出结果的缓冲区 */
                        int nBufLen);   /* 缓冲区的长度 */

char* EnCodeBill_final_mt(void* pGlobal,
                          char* pVal,      /* 待编码的数据指针 */
                          char* pBuf,      /* 编码结果函数运算使用的缓冲区 */
                          int* pBufLen);   /* 输入缓冲区pBuf的长度，*/
                                           /* 并返回编码结果的长度 (in,out) */

void* Init_OSS_Lib_Ini();
void Release_OSS_Lib_Ini(void* pGlobal);

void* Init_OSS_Lib_Final();
void Release_OSS_Lib_Final(void* pGlobal);

}



