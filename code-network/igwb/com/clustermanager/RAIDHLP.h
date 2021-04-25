///////////////////////////////////////////////////////////////////////////////////////
// RAIDHLP.h -- ����������а�����
// chenghui, 2000.8
// �ÿ��ṩ���°���������:
//     * PercRAID�������п�����(SC/DC), ����:
//          * ��׼��Open/Close����;
//			* ����״̬��ѯ����;
// ע��: 
//    ÿ����������һ��Open������һ��Close���������ɸ�Get������ɡ�Open�����������ȱ�
//    �����Է���һ�����������Get������Close�����ĵ�һ���������Ǹþ����

#ifndef _RAIDHLP_H
#define _RAIDHLP_H

#ifdef  __cplusplus
extern "C" {
#endif

////////////////////////////////// PercRAID�������� //////////////////////////////////
HANDLE __stdcall PercRAIDOpen(LPVOID pOpenParam);	// ��һ��PercRAID���
void __stdcall PercRAIDClose(HANDLE hSession);		// �ر�һ��PercRAID���

#define PERCRAID_PDS_ERROR		-1	//״̬��ѯ����

// ���º�����PercRAID�������п��ƿ���������American Megatrends Inc���壬�Դ�������
// ���̵�״̬
#define PERCRAID_PDS_READY		1	//������������δ��ʼ��
#define PRECRAID_PDS_ONLINE		3	//�����������Ѿ���ʼ��
#define PRECRAID_PDS_FAILED		4	//����ʧЧ
#define PERCRAID_PDS_REBUILD	5	//���������ؽ�
#define PRECRAID_PDS_HOTSPARE	6	//��ʾ�ȱ��ݴ���
#define PRECRAID_PDS_NONDISK	20	//�Ǵ���

// PercRAIDPhysicalDriveQueryState -- ��ȡ�������״̬
// ����:
//     hSession -- ��PercRAIDOpen���ص�Session���
//     nChannel -- ͨ���š����ڵ�ͨ����ӦΪ�㡣�����ͨ����ӦΪ���һ��
//     nDrive   -- ���̺š�������һ�����̡�
// ����ֵ:
//     ���������������򷵻�PERCRAID_PDS_ERROR�����򷵻�����״ֵ̬��
int __stdcall PercRAIDGetPhysicalDriveState(HANDLE hSession, int nChannel, int nDrive);

#ifdef  __cplusplus
}
#endif

#endif