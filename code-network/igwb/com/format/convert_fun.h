#if !defined(_FORMAT_CONVERTFUN_H_)
#define _FORMAT_CONVERTFUN_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CConvertFun
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"
#include "ace/Synch.h"

class CFormat;

#define MAX_PARA_LEN  255  //����һ��������������󳤶�

/****************** The CConvertFun class Defining ******************/
// ����������ת�������࣬������һ�������ࣨUtility Class����������������
//           �ͷ������Ǿ�̬��
//
/********************************************************************/
class CConvertFun
{
public:

    static void AddRef(void);     //���ü�������1����
    static void ReleaseRef(void); //���ü�������1����

    /************ The ConvertAnalysis Defining *********************/
    // �����������Ӹ�ʽ��ת�������з�����������������������б���ȱʡֵ
    //
    // ���������
    // sConvertFun��ת����������ӦSFieldInfo�е�szConvertFun
    //
    // ���������
    //        sLib�������������ʹ�ö�̬���ӿ⣬�򷵻ص�sLibΪ���ַ�����
    //        sFun��ת��������
    //    sDefault����ȱʡֵ
    //    ParaList�������б���������������������
    //
    //   ����ֵ���������sConvertFun���ó�����������-1�����򷵻�0
    //
    /***************************************************************/
    static int ConvertAnalysis(const STRING& sConvertFun, STRING& sLib, 
                               STRING& sFun, STRING& sDefault, 
                               LIST<STRING>& ParaList);
    /************ End of The ConvertAnalysis Defining **************/

    /************ The BlockCopy Defining ***************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�����ݿ�Ŀ���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ��Դ���ݿ���ڴ棬
    //           nSrcFieldLen[0]��ʾԴ���ݿ�ĳ���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int BlockCopy(void* pCurField, const UINT4 nSrcFields,
                         const void* pSrcFields[],
                         const UINT4 nSrcFieldLen[],
                         const SFieldInfo* pFieldInfo,
                         const void* pDefault, const UINT4 nDefaultLen,
                         int nIsQuery = 0);
    /************ End of The BlockCopy Defining ********************/

    //�鿽��������֧��Intel/�����ֽ���ͬ���������Ļ��࿽�� ZhengYuqun 2004-04-28
    static int BlockCopyEx(void* pCurField, const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);

    /************ The GetBinYear Defining **************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֽ�BYTE�ͻ���SHORT�͵���λ��ת���ɶ�������λ��
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ����������ΪBYTE��SHORT�͵�2λ���ꡱ��
    //           ���nSrcFieldLen[0]Ϊ1����ʾpSrcFields[0]ΪBYTE�͵��꣬
    //           ���nSrcFieldLen[0]Ϊ2����ʾpSrcFields[0]ΪSHORT�͵���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int GetBinYear(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[], 
                          const UINT4 nSrcFieldLen[], 
                          const SFieldInfo* pFI, 
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery = 0);
    /************ End of The GetBinYear Defining *******************/

    /************ The GetTextYear Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֽ�BYTE��SHORT�͵���λ��ת�����ı���ʽ����λ��
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ����������ΪBYTE��SHORT�͵�2λ���ꡱ��
    //           ���nSrcFieldLen[0]Ϊ1����ʾpSrcFields[0]ΪBYTE�͵��꣬
    //           ���nSrcFieldLen[0]Ϊ2����ʾpSrcFields[0]ΪSHORT�͵���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int GetTextYear(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The GetTextYear Defining ******************/

    /************ The GetTextTime Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ���ꡢ�¡��ա�ʱ���֡���ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ6��
    //           pSrcFields[0]ָ����������ΪBYTE��SHORT�͵�2λ���ꡱ��
    //           pSrcFields[1]��pSrcFields[2]��pSrcFields[3]��pSrcFields[4]��
    //           pSrcFields[5]�ֱ�ָ��BYTE���¡��ա�ʱ���֡��룬
    //           ���nSrcFieldLen[0]Ϊ1����ʾpSrcFields[0]ΪBYTE�͵��꣬
    //           ���nSrcFieldLen[0]Ϊ2����ʾpSrcFields[0]ΪSHORT�͵���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int GetTextTime(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The GetTextTime Defining ******************/

	/************ The GetTextTimeEx Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ���ꡢ�¡��ա�ʱ���֡���ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ6����7
    //           pSrcFields[0]ָ����������ΪBYTE��SHORT�͵�2λ���ꡱ��
    //           pSrcFields[1]��pSrcFields[2]��pSrcFields[3]��pSrcFields[4]��
    //           pSrcFields[5]�ֱ�ָ��BYTE���¡��ա�ʱ���֡���
    //           pSrcFields[6]������ڣ���Ϊ���� 
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int GetTextTimeEx(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The GetTextTime Defining ******************/

    /************ The Int2Text Defining ****************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֽ�������������ת�����ı�
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           nSrcFieldLen[0]Ϊ1��2��4��8ʱ�ֱ��ʾpSrcFields[0]
    //           ָ����������ΪBYTE��SHORT��LONG��INT8�͵�����
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int Int2Text(void* pCurField, const UINT4 nSrcFields, 
                        const void* pSrcFields[],
                        const UINT4 nSrcFieldLen[],
                        const SFieldInfo* pFI,
                        const void* pDefault, const UINT4 nDefaultLen,
                        int nIsQuery = 0);
    /************ End of The Int2Text Defining *********************/

    //add by zkq 2004-09-28
    /************ The BCD2RBCD Defining ****************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           1.������BCD��ת������BCD�룬
    //           ת��ʱ�������������ΪBCD���������
    //           1��BCD�����0xF��2���ﵽBCD���������󳤶�
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           pSrcFields[0]ָ������BCD�����飬 
    //           nSrcFieldLen[0]ָʾ������BCD������ĳ���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
/***************************************************************/
static int BCD2RBCD(void* pCurField, const UINT4 nSrcFields, 
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery);
    /************ End of The BCD2RBCD Defining *********************/

    /************ The BCD2String Defining **************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           1.������BCD��ת�����ַ�����
	//           modify by zkq 2004-05-27
    //           2.������BCD����B,Cת�����ַ�'*','#'��
    //           add end
    //           ת��ʱ�������������ΪBCD���������
    //           1��BCD�����0xF��2���ﵽBCD���������󳤶�
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ������BCD�����飬 
    //           nSrcFieldLen[0]ָʾ������BCD������ĳ���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int BCD2String(void* pCurField, const UINT4 nSrcFields, 
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery = 0);
    /************ End of The BCD2String Defining *******************/

    /************ The BCD2String2 Defining **************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ��n.5���ֽڵ�����BCD��ת�����ַ�����
    //           ת��ʱ�������������ΪBCD���������
    //           1��BCD�����0xF��2���ﵽBCD���������󳤶�
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ������BCD�����飬 
    //           nSrcFieldLen[0]ָʾ������BCD������ĳ���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int BCD2String2(void* pCurField, const UINT4 nSrcFields, 
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery = 0);
    /************ End of The BCD2String2 Defining *******************/

    /************ The SBCD2String Defining **************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ������SBCD��ת�����ַ�����
    //           ת��ʱ�������������ΪBCD���������
    //           1��SBCD����ڵ���0xA��2���ﵽBCD���������󳤶�
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ������BCD�����飬 
    //           nSrcFieldLen[0]ָʾ������BCD������ĳ���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int SBCD2String(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The BCD2String Defining *******************/

    /************ The RBCD2String Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ������BCD��ת�����ַ�����
    //           ת��ʱ�������������ΪBCD���������
    //           1��BCD�����0xF��2���ﵽBCD���������󳤶�
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ����BCD�����飬 
    //           nSrcFieldLen[0]ָʾ�÷���BCD������ĳ���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int RBCD2String(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The RBCD2String Defining ******************/

    /************ The ZXRBCD2String Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ������BCD��ת�����ַ�����
    //           ת��ʱ�������������ΪBCD���������
    //           1��BCD�����0xF��2���ﵽBCD���������󳤶�
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ����BCD�����飬 
    //           nSrcFieldLen[0]ָʾ�÷���BCD������ĳ���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int ZXRBCD2String(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery = 0);
    /************ End of The ZXRBCD2String Defining ******************/
    
    /************ The GetEnumString Defining ***********************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ����һ����ʽ���ö��ֵ������Ӧ��ö���ַ���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ��ö���͵ĸ�ʽ���ȡֵ
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int GetEnumString(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery = 0);
    /************ End of The GetEnumString Defining ****************/

    //Add by zkq for TT&T 2004-05-27

	/************ The BCDTime2Sec Defining **************************/
	// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
	//           ������BCD��ת�����ַ�����������ʱ�����BCD��ת����������ʾ���ַ���
	//           ת��ʱ�������������ΪBCD���������
	//           1��BCD�����0xF��2���ﵽBCD���������󳤶�
	//
	// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
	//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
	//           pSrcFields[0]ָ������BCD�����飬 
	//           nSrcFieldLen[0]ָʾ������BCD������ĳ���
	//
	// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
	//
	//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
	//
	/***************************************************************/
    static int CConvertFun::BCDTime2Sec(void* pCurField, const UINT4 nSrcFields, 
		                                const void* pSrcFields[],
		                                const UINT4 nSrcFieldLen[],
		                                const SFieldInfo* pFI,
		                                const void* pDefault, const UINT4 nDefaultLen,
		                                int nIsQuery);
   /************ End of The BCDTime2Sec Defining *******************/

	/************ The EBCD2String Defining **************************/
	// ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
	//           ������BCD��ת�����ַ�����
	//           ת��ʱ�������������ΪBCD���������
	//           1��BCD�����0xE��2���ﵽBCD���������󳤶�
	//
	// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
	//           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
	//           pSrcFields[0]ָ������BCD�����飬 
	//           nSrcFieldLen[0]ָʾ������BCD������ĳ���
	//
	// ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
	//
	//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
	//
	/***************************************************************/
	static int CConvertFun::EBCD2String(void* pCurField, const UINT4 nSrcFields, 
                                        const void* pSrcFields[],
                                        const UINT4 nSrcFieldLen[],
                                        const SFieldInfo* pFI,
                                        const void* pDefault, const UINT4 nDefaultLen,
                                        int nIsQuery);
    /************ End of The EBCD2String Defining *******************/
	
	//End by zkq for TT&T 2004-05-27
	//Added by maimaoshi at 2002-10-16

	/************ The GetTextTimeInBCD Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ְ�BCD��������ʱ����ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
	static int GetTextTimeInBCD(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery = 0);
	/************ End of The GetTextTimeInBCD Defining **********************/

	/************ The GetTextTimeInBCD_4Y Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ְ�4λ���BCD��������ʱ����ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
	static int GetTextTimeInBCD_4Y(void* pCurField, const UINT4 nSrcFields, 
                                 const void* pSrcFields[],
                                 const UINT4 nSrcFieldLen[],
                                 const SFieldInfo* pFI,
                                 const void* pDefault, const UINT4 nDefaultLen,
                                 int nIsQuery = 0);
	/************ End of The GetTextTimeInBCD_4Y Defining **********************/

	/************ The GetTextTimeInBCD_T Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ְ�6.5���ֽڵ�BCD��ʱ��ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS.T
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
	static int GetTextTimeInBCD_T(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery = 0);
	/************ End of The GetTextTimeInBCD_T Defining **********************/

	/************ The Int2IpText Defining ****************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֽ�������������ת�����ı�
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           nSrcFieldLen[0]Ϊ1��2��4��8ʱ�ֱ��ʾpSrcFields[0]
    //           ָ����������ΪBYTE��SHORT��LONG��INT8�͵�����
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
	static int Int2IpText(void* pCurField, const UINT4 nSrcFields, 
                        const void* pSrcFields[],
                        const UINT4 nSrcFieldLen[],
                        const SFieldInfo* pFI,
                        const void* pDefault, const UINT4 nDefaultLen,
                        int nIsQuery = 0);
	/************ End of The Int2IpText Defining **********************/

	//end by maimaoshi at 2002-05-18

    //by ldf 2003-01-08 ��Ӧ���ⵥD05886 D05556��ͬD05415 D5150��
    /************ The Int2IpTextDirect Defining ****************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֽ�������������ת�����ı�
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           nSrcFieldLen[0]Ϊ1��2��4��8ʱ�ֱ��ʾpSrcFields[0]
    //           ָ����������ΪBYTE��SHORT��LONG��INT8�͵�����
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
	static int Int2IpTextDirect(void* pCurField, const UINT4 nSrcFields, 
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery);
    /************ End of The Int2IpTextDirect Defining **********************/

    /************ The Array2Text Defining ****************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֽ�������������ת�����ı�
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ָ����������ΪBYTE�͵�����
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int Array2Text(void* pCurField, const UINT4 nSrcFields, 
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery);
    /************ End of The Array2Text Defining **********************/
    //�޸����


	/************ The BCDToSecond Defining **************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ������BCD��ת�����ַ�����
    //           ת��ʱ�������������ΪBCD���������
    //           1��BCD�����0xF��2���ﵽBCD���������󳤶�
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ������BCD�����飬 
    //           nSrcFieldLen[0]ָʾ������BCD������ĳ���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int BCDToSecond(void* pCurField, const UINT4 nSrcFields, 
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery = 0);
    /************ End of The BCDToSecond Defining *******************/

	/************ The GetTextTimeNoSp Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ���ꡢ�¡��ա�ʱ���֡���ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ6��
    //           pSrcFields[0]ָ����������ΪBYTE��SHORT�͵�2λ���ꡱ��
    //           pSrcFields[1]��pSrcFields[2]��pSrcFields[3]��pSrcFields[4]��
    //           pSrcFields[5]�ֱ�ָ��BYTE���¡��ա�ʱ���֡��룬
    //           ���nSrcFieldLen[0]Ϊ1����ʾpSrcFields[0]ΪBYTE�͵��꣬
    //           ���nSrcFieldLen[0]Ϊ2����ʾpSrcFields[0]ΪSHORT�͵���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int GetTextTimeNoSp(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The GetTextTimeNoSp Defining ******************/

	/************ The FillFF Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֶ�ĳ�ֶ���"FF"
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ����������ΪBYTE��
    //           nSrcFieldLen[0]Ϊ1
    //           
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int FillFF(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The FillFF Defining ******************/

   	/************ The ChangeGSVNandCallType Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֶ�SoftX300������ϸ�����ĺ�������ҵ�����ͽ���ת��
    //           ҵ������Ϊ���س�;�Ļ�����ҵ�����Զ�Ҫ��Ϊ���֣�
    //           ��������ԭ���ǳ��ָ�Ϊ���֣�ԭ���ǻ�ӵĸ�Ϊ���
	//           �˺���ʵ��ת��Ϊ�����ƻ���  
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ2��
    //          
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int ChangeGSVNandCallType(void* pCurField, 
                           const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery);
    /************ End of The ChangeGSVNandCallType Defining ******************/

	/************ The ChangeGSVNandCallType Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֶ�SoftX300������ϸ�����ĺ�������ҵ�����ͽ���ת��
    //           ҵ������Ϊ���س�;�Ļ�����ҵ�����Զ�Ҫ��Ϊ���֣�
    //           ��������ԭ���ǳ��ָ�Ϊ���֣�ԭ���ǻ�ӵĸ�Ϊ���
    //           �˺���ʵ��ת��Ϊ
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ2��
    //          
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int ChangeGSVNandCallType2Text(void* pCurField, 
                           const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery);
    /************ End of The ChangeGSVNandCallType Defining ******************/

	/************ The Array2HexString Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֶ��ֽ�����ת��Ϊʮ�������ַ���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //          
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int Array2HexString(void* pCurField, 
                           const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery);
    /************ End of The Array2HexString Defining ******************/

	/************ The FillInvaidValue Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֶ��ֽ����FF
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //          
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int FillInvaidValue(void* pCurField, 
                           const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery);
    /************ End of The FillInvaidValue Defining ******************/
    
    	//Added by maimaoshi at 2002-10-16
	
	//added maimaoshi,2003-03-10
	/************ The ToLRLN Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֶ�ĳ�ֶ���"\r\n"
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ����������ΪBYTE��
    //           nSrcFieldLen[0]Ϊ1
    //           
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
	static int ToLRLN(void* pCurField, const UINT4 nSrcFields, 
                       const void* pSrcFields[],
                       const UINT4 nSrcFieldLen[],
                       const SFieldInfo* pFI,
                       const void* pDefault, const UINT4 nDefaultLen,
                       int nIsQuery = 0);
	/************ End of The ToLRLN Defining ****************/

	/************ The ToLR Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֶ�ĳ�ֶ���"\r"
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ����������ΪBYTE��
    //           nSrcFieldLen[0]Ϊ1
    //           
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
	static int ToLR(void* pCurField, const UINT4 nSrcFields, 
                       const void* pSrcFields[],
                       const UINT4 nSrcFieldLen[],
                       const SFieldInfo* pFI,
                       const void* pDefault, const UINT4 nDefaultLen,
                       int nIsQuery = 0);
	/************ End of The ToLR Defining ****************/

	/************ The ToLN Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֶ�ĳ�ֶ���"\n"
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ����������ΪBYTE��
    //           nSrcFieldLen[0]Ϊ1
    //           
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
	static int ToLN(void* pCurField, const UINT4 nSrcFields, 
                   const void* pSrcFields[],
                   const UINT4 nSrcFieldLen[],
                   const SFieldInfo* pFI,
                   const void* pDefault, const UINT4 nDefaultLen,
                   int nIsQuery = 0);
	/************ End of The ToLN Defining ****************/

	/************ The StringCopy Defining ***************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ���ֶΰ����ַ�������
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ��Դ���ݿ���ڴ棬
    //           nSrcFieldLen[0]��ʾԴ���ݿ�ĳ���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int StringCopy(void* pCurField, const UINT4 nSrcFields,
                         const void* pSrcFields[],
                         const UINT4 nSrcFieldLen[],
                         const SFieldInfo* pFieldInfo,
                         const void* pDefault, const UINT4 nDefaultLen,
                         int nIsQuery = 0);
    /************ End of The StringCopy Defining ********************/
    
    /************ The ByteText2 Defining ****************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֽ�������������ת�����ı�
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           nSrcFieldLen[0]Ϊ1��2��4��8ʱ�ֱ��ʾpSrcFields[0]
    //           ָ����������ΪBYTE��SHORT��LONG��INT8�͵�����
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int ByteText2(void* pCurField, const UINT4 nSrcFields, 
                        const void* pSrcFields[],
                        const UINT4 nSrcFieldLen[],
                        const SFieldInfo* pFI,
                        const void* pDefault, const UINT4 nDefaultLen,
                        int nIsQuery = 0);
    /************ End of The ByteText2 Defining *********************/

	/************ The GetTextTimeInString Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ���ꡢ�¡��ա�ʱ���֡���ת�����ı���ʽ��YYYY-MM-DD HH:NN:SS
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ6��
    //           pSrcFields[0]ָ����������ΪBYTE��SHORT�͵�2λ���ꡱ��
    //           pSrcFields[1]��pSrcFields[2]��pSrcFields[3]��pSrcFields[4]��
    //           pSrcFields[5]�ֱ�ָ��BYTE���¡��ա�ʱ���֡��룬
    //           ���nSrcFieldLen[0]Ϊ1����ʾpSrcFields[0]ΪBYTE�͵��꣬
    //           ���nSrcFieldLen[0]Ϊ2����ʾpSrcFields[0]ΪSHORT�͵���
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int GetTextTimeInString(void* pCurField, const UINT4 nSrcFields, 
                                   const void* pSrcFields[],
                                   const UINT4 nSrcFieldLen[],
                                   const SFieldInfo* pFI,
                                   const void* pDefault, const UINT4 nDefaultLen,
                                   int nIsQuery = 0);
    /************ End of The GetTextTimeInString Defining ******************/
    
    //end maimaoshi
    //by ldf 2003-04-01
    /************ The CheckSum Defining *****************************/
    // ����������У�麯��
    /***************************************************************/
	static int CheckSum(void* pCurField, const UINT4 nSrcFields,
		                const void* pSrcFields[], 
		                const UINT4 nSrcFieldLen[], 
		                const SFieldInfo* pFI, 
		                const void* pDefault, const UINT4 nDefaultLen,
		                int nIsQuery);
    //end
    //���м�Ⱥ�ŵ��绰�����ת��.
    static int TrkGrp2CallNo(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery = 0);  
    
    /************ The FindFun Defining *****************************/
    // �������������ݸ�ʽ������ת���������ҵ���Ӧ��ת������ָ��
    //
    // ���������
    // szConvertFun����ʽ���е�ת�������ַ���
    //      pSrcFmt��Դ��ʽ����
    //
    // �����������
    //
    //   ����ֵ��ת������ָ��
    //
    /***************************************************************/
    static CONVERTFUN FindFun(const char* szConvertFun, 
                              const CFormat *pSrcFmt );
    /************ End of The FindFun Defining **********************/

    /************ The FillWithStr Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ�ֶ�ĳ�ֶ����һ�������ַ�����ʵ�ֶԻ�����ʽ���������
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ1��
    //           pSrcFields[0]ָ����������ΪBYTE��
    //           nSrcFieldLen[0]Ϊ1
    //           
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int FillWithStr(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    /************ End of The FillWithStr Defining ******************/
    
    /************ The FillByIniFile Defining *************************/
    // ����������������������CConvert���ṩ��CONVERTFUN���͵Ĺ��ߺ�����
    //           ʵ������ʽ�������һ��INI�ļ��е�ָ����ֵ
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵����
    //           ��Ҫ������ǵ��ô˺���ʱnSrcFields����Ϊ2��
    //           pSrcFields[0]ָ����������Ϊchar��
    //           pSrcFields[1]ָ����������Ϊchar
    //           
    //
    // ����������������˵����μ�CONVERTFUN�������͵Ĳ���˵��
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /***************************************************************/
    static int FillByIniFile(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);    
    
    // ������������ָ��ĺ���
    static CONDITIONFUN FindConFun(const STRING& sFun);


    //��ȡ�绰�����ǰnλ����ת������Ӧ������,
    //�ú�����CONDITIONFUN���͵Ĺ��ߺ���
    static int Prefix(const UINT4 nParasNum, 
                      const void* pParas[],
                      const UINT4 nParasLen[], 
                      const STRING& pDefault,
                      SVariant& stVar);

    //ʵ�ִ����к����з�������ID
    //�ú�����CONDITIONFUN���͵Ĺ��ߺ���
    static int GetAreaID(const UINT4 nParasNum, 
                         const void* pParas[],
                         const UINT4 nParasLen[], 
                         const STRING& pDefault,
                         SVariant& stVar);

	/************ The AddFillerSeparator Defining ******************/
    // ����������Ϊ�ı���ʽ��Ŀ���ʽ�����������ͷָ���
    //
    // ���������
    //   pszField��δ���������ͷָ�����Ŀ���ʽ������
    //        pFI����Ŀ���ʽ�������
    //
    // ���������
    //   pszField���Ѿ��������ͷָ������Ŀ���ʽ������
    //
    //   ����ֵ���ɹ�����0��ʧ�ܷ���-1
    //
    /****************************************************************/
    static int AddFillerSeparator(char* pszField, 
                                  const SFieldInfo* pFI);
    /************ End of The AddFillerSeparator Defining ************/

    //add by lucy 2005-5-25
    /************ The LinkText Defining *****************************/
    // ���������������ӷ����������ֶ�
    /***************************************************************/
    static int LinkText(void* pCurField, const UINT4 nSrcFields,
                        const void* pSrcFields[], 
                        const UINT4 nSrcFieldLen[], 
                        const SFieldInfo* pFI, 
                        const void* pDefault, const UINT4 nDefaultLen,
                        int nIsQuery);
    /************ End of The LinkText Defining *********************/


    //Added By Lucy 2004-07-12
    /************ The GetRecordType  Defining *********************/
    // ��������������TM��ʽת�����󣬴�������Ϊ��
    // 1.   ��ԭʼ�����е�free_indicatorΪ0�ǣ�Record Type�ֶ���дΪ04
    // 2.   ����ԭʼ�����е�gsvn��ͨ��ö��ת��
    // 3.   ֧��û��ȡֵʱ����Ĭ��ֵ����
    // ת��������GetRecordType(free_ind, gsvn, "0")
    /***************************************************************/    
    static int GetRecordType(void* pCurField, const UINT4 nSrcFields,
                            const void* pSrcFields[], 
                            const UINT4 nSrcFieldLen[], 
                            const SFieldInfo* pFI, 
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery);    
    /************ End of The GetRecordType Defining ****************/
    

    //add by lucy 2004-07-09
    /************ The GetSubCategory  Defining *********************/
    // ��������������TM��ʽת�����󣬴�������Ϊ��
    // 1.	���ԭʼ�����е�caller_ctx_ number�ֶ���Ч������ֶ���дΪ03��
    // 2��	���ԭʼ�����е�caller_didΪH.323����SIP������trunk_group _in
    //      ������0xFFFF������ֶ���дΪ05��
    // 3��	�������������ԭʼ�����е�caller_category�ֶ���ת����ö��ȡֵ        
    /***************************************************************/
    static int GetSubCategory(void* pCurField, const UINT4 nSrcFields,
                              const void* pSrcFields[], 
                              const UINT4 nSrcFieldLen[], 
                              const SFieldInfo* pFI, 
                              const void* pDefault, const UINT4 nDefaultLen,
                              int nIsQuery);
    /************ End of The GetSubCategory Defining ****************/
    
    //add by lucy 2004-07-09
    /************ The ISUPChargeNumber Defining *********************/
    // ��������������TM��ʽת�����󣬴�������Ϊ��
    //     ����ԭʼ�����е�ISUP charge number indicator�ֶε�ȡֵת����
    //      1��	���ȡֵΪ0����caller_numberת�������ֶΣ�
    //      2��	���ȡֵΪ1����called_numberת�������ֶΣ�
    //      3��	���ȡֵΪ2����connected_numberת�������ֶΡ�
    //      ���������дΪ���ַ�����
    static int ISUPChargeNumber(void* pCurField, const UINT4 nSrcFields,
                                const void* pSrcFields[], 
                                const UINT4 nSrcFieldLen[], 
                                const SFieldInfo* pFI, 
                                const void* pDefault, const UINT4 nDefaultLen,
                                int nIsQuery); 
    /************ End of The ISUPChargeNumber Defining ****************/
    
    //add by lucy 2004-07-09
    /************ The ISUPChargeNumberNOA Defining *********************/
    // ��������������TM��ʽת�����󣬴�������Ϊ��
    //     ����ԭʼ�����е�ISUP charge number indicator�ֶε�ȡֵת����
    //      1��	���ȡֵΪ0����caller_address_natureת�������ֶΣ�
    //      2��	���ȡֵΪ1����called_address_natureת�������ֶΣ�
    //      3��	���ȡֵΪ2����connected_address_natureת�������ֶΡ�
    //      ���������дΪ���ַ�����
    static int ISUPChargeNumberNOA(void* pCurField, const UINT4 nSrcFields,
                                   const void* pSrcFields[], 
                                   const UINT4 nSrcFieldLen[], 
                                   const SFieldInfo* pFI, 
                                   const void* pDefault, const UINT4 nDefaultLen,
                                   int nIsQuery); 
    /************ End of The ISUPChargeNumberNOA Defining ****************/  
    
    //add by lucy 2004-08-05
    /************ The VideoTime Defining *********************/
    // ������������ͨ���ڸ�ʽת�����󣬴�������Ϊ��
    //     ������Ƶ������ֶ��жϣ��������Ƶ����룬��ʹ��"Ӧ������ʱ��"
    // ʹ��ʾ����
    // VideoTime(video_code_type, ans_year,ans_mon,ans_day,ans_hour,ans_min,ans_sec)
    /********************************************************/
    static int VideoTime(void* pCurField, const UINT4 nSrcFields,
                        const void* pSrcFields[], 
                        const UINT4 nSrcFieldLen[], 
                        const SFieldInfo* pFI, 
                        const void* pDefault, const UINT4 nDefaultLen,
                        int nIsQuery); 

    //add by lucy 2004-08-05
    /************ The VideoDuration Defining *********************/
    // ������������ͨ���ڸ�ʽת�����󣬴�������Ϊ��
    //     ������Ƶ������ֶ��жϣ��������Ƶ����룬��ʹ��"ͨ��ʱ��"
    // ʹ��ʾ����
    // VideoDuration(video_code_type,conversation_time)
    /********************************************************/
    static int VideoDuration(void* pCurField, const UINT4 nSrcFields,
                        const void* pSrcFields[], 
                        const UINT4 nSrcFieldLen[], 
                        const SFieldInfo* pFI, 
                        const void* pDefault, const UINT4 nDefaultLen,
                        int nIsQuery); 
    
    //added by lucy 2004-08-16
    //ʵ�� ������������������
    static int Add(void* pCurField, const UINT4 nSrcFields,
                    const void* pSrcFields[], 
                    const UINT4 nSrcFieldLen[], 
                    const SFieldInfo* pFI, 
                    const void* pDefault, const UINT4 nDefaultLen,
                    int nIsQuery);
private:

    /************ The GetParas Defining *****************************/
    // ������������ת���������õĲ����ַ����в�ֳ���������������
    //           ����֮����','�ָ�
    //
    // ���������
    //     sParas�������ַ���
    //
    // ���������
    //   ParaList�������б�����������˳������
    //
    //   ����ֵ������������ó�����������-1�����򷵻�0
    //
    /****************************************************************/
    static int GetParas(const STRING& sParas, LIST<STRING>& ParaList);
    /************ End of The GetParas Defining **********************/

    /************ The ConvertFunDiv Defining ************************/
    // �����������Ӹ�ʽ��ת�������з�����������������б���ȱʡֵ
    //
    // ���������
    //    sSubStr��ת�������ַ���
    //
    // ���������
    //       sFun��ת��������
    //   sDefault����ȱʡֵ
    //   ParaList�������б���������������������
    //
    //   ����ֵ������������ó�����������-1�����򷵻�0
    //
    /****************************************************************/
    static int ConvertFunDiv(const STRING& sSubStr, STRING& sFun, 
                             STRING& sDefault, LIST<STRING>& ParaList);
    /************ End of The CConvertFunDiv Defining ****************/

    //Added by ZhengYuqun 2003-08-27 SWPD01198
    static int PadAreacode(void* pCurField, const UINT4 nSrcFields,
                         const void* pSrcFields[],
                         const UINT4 nSrcFieldLen[],
                         const SFieldInfo* pFieldInfo,
                         const void* pDefault, const UINT4 nDefaultLen,
                         int nIsQuery = 0);
    //End

    //���µ�ǰ׺�滻�绰�����оɵ�ǰ׺
    static int ReplacePrefix(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFieldInfo,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery);

    //added by lucy 2004-06-25 
    //���ܣ���ԭʼ������ĳ���ֶδ������滻���ַ���,���ֺ��ַ�����Ӧ��ϵ���ļ�������
    static int ReplaceNumByStr(void* pCurField, const UINT4 nSrcFields,
                               const void* pSrcFields[],
                               const UINT4 nSrcFieldLen[],
                               const SFieldInfo* pFieldInfo,
                               const void* pDefault, const UINT4 nDefaultLen,
                               int nIsQuery);


    //����ƽ 2004-02-14 ���ת������ Duration2HHmmSS(...)
    static int Duration2HHmmSS(void* pCurField, 
                               const UINT4 nSrcFields,
                               const void* pSrcFields[],
                               const UINT4 nSrcFieldLen[],
                               const SFieldInfo* pFI,
                               const void* pDefault, 
                               const UINT4 nDefaultLen,
                               int nIsQuery);

    /************ The Convert10ms Defining ************************/
    // ������������10msΪ��λ��ʱ��ת����S, ��HHMMSS
    //
    // ת��HHMMSS�����ʽ�����磺12800, ����"HH:MM:SS" -> 02:08:00
    //
    // ����ֵ���������ת��ʧ�ܣ���������-1�����򷵻�0
    //
    // Added By Lucy 2004-07-16 
    /****************************************************************/
    static int Convert10ms(void* pCurField, 
                           const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, 
                           const UINT4 nDefaultLen,
                           int nIsQuery);
    /*************** End of Convert10ms Defining ********************/

    static int Int2TextEx(void* pCurField, 
                          const UINT4 nSrcFields, 
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, 
                          const UINT4 nDefaultLen,
                          int nIsQuery);

    //Added by ZhengYuqun SWPD03631 2004-02-18
    //MSoftX3000�Ļ�������ת��Ϊ2G�Ļ�������ʱ��Ҫ���ݻ������ͺ��м仰��ԭ��ֵ�����Ժ���������
    static int CDRType2G(void* pCurField, 
        const UINT4 nSrcFields, 
        const void* pSrcFields[],
        const UINT4 nSrcFieldLen[],
        const SFieldInfo* pFI,
        const void* pDefault, 
        const UINT4 nDefaultLen,
        int nIsQuery);

    static int IsInNumSeg(const UINT4 nParasNum, 
                      const void* pParas[],
                      const UINT4 nParasLen[], 
                      const STRING& pDefault,
                      SVariant& stVar);

    static int IsInEnum(const UINT4 nParasNum, 
                        const void* pParas[],
                        const UINT4 nParasLen[], 
                        const STRING& pDefault,
                        SVariant& stVar);

   static int PadNumber(void* pCurField, 
                        const UINT4 nSrcFields, 
                        const void* pSrcFields[],
                        const UINT4 nSrcFieldLen[],
                        const SFieldInfo* pFI,
                        const void* pDefault, 
                        const UINT4 nDefaultLen,
                        int nIsQuery);
                        
    //������Ϣ�е������к���ת��Ϊ�ı�  Added by ZhengYuqun
    static int SMSNum2Text(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);

    static int TermCause2G(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);

    static int PeerMnc2G(void* pCurField, const UINT4 nSrcFields, 
                         const void* pSrcFields[],
                         const UINT4 nSrcFieldLen[],
                         const SFieldInfo* pFI,
                         const void* pDefault, const UINT4 nDefaultLen,
                         int nIsQuery = 0);

    static int VarLen2Text(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);
    
    static int VarValue2Text(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery = 0);

    //Added by ZhengYuqun SWPD06279 2004-06-26
    static int GetVarValue(void* pCurField, const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery = 0);

    static int GetVarLen(void* pCurField, const UINT4 nSrcFields, 
                        const void* pSrcFields[],
                        const UINT4 nSrcFieldLen[],
                        const SFieldInfo* pFI,
                        const void* pDefault, const UINT4 nDefaultLen,
                        int nIsQuery = 0);

    //���ݺ���ĳ�����ת��BCD��  //SWPD06703 Zheng Yuqun 2004-06-19
    static int BCD2StringByLen(void* pCurField, const UINT4 nSrcFields,
                                const void* pSrcFields[],
                                const UINT4 nSrcFieldLen[],
                                const SFieldInfo* pFI,
                                const void* pDefault, const UINT4 nDefaultLen,
                                int nIsQuery = 0);

    static int GetArgSpecField(void* pCurField, const UINT4 nSrcFields, 
                                const void* pSrcFields[],
                                const UINT4 nSrcFieldLen[],
                                const SFieldInfo* pFI,
                                const void* pDefault, const UINT4 nDefaultLen,
                                int nIsQuery = 0);    
                                                   
private:

    static int m_nRefCount;	      //���ü����������뻥�����
    static ACE_Mutex m_hMutex;    //ACE������������ͬ����m_nRefCount�ķ���

};
/****************** End of The CConvertFun class Defining ***********/

#endif  //!defined(_FORMAT_CONVERTFUN_H_)
