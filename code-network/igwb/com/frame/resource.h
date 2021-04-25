
#ifndef _FRAME_RESOURCE_H_
#define _FRAME_RESOURCE_H_


    /* �������Ƴ����ַ��� */

    #define PTS_UNKNOWN                     "PT_UNKNOWN"    //δ֪  
    #define PTS_OUTSIDE                     "PT_OUTSIDE"    //�Ʒ�����֮��Ľ���
    #define PTS_SELF                        "PT_SELF"       //������
    #define PTS_KERNEL                      "PT_KERNEL"     //KERNEL
    #define PTS_CLSTR                       "PT_CLSTR"      //˫������
    #define PTS_OM                          "PT_OM"         //����ά������
    #define PTS_AP_BASE                     "PT_AP_BASE"    //��������
    #define PTS_CO_BASE                     "PT_CO_BASE"    //��������
    #define PTS_BROADCAST                   "PT_BROADCAST"  //�㲥��Ϣ�Ľ��ս���
    #define PTS_CFGSVR                      "PT_CFGSVR"     //Զ�̲������÷���˽���

    //GTP'�Ʒѽӿ����� Z30971 2004/06  
    #define PTS_BS                          "PT_BS"         //�Ʒѽӿڽ���

    /* ģ�����Ƴ����ַ��� */

    #define MTS_UNKNOWN                     "MT_UNKNOWN"        //δ֪ģ��
    #define MTS_MANAGER                     "MT_MANAGER"        //����ģ��
    #define MTS_COMM                        "MT_COMM"           //����ͨ�Ų�
    #define MTS_NETWORK                     "MT_NETWORK"        //����ģ��
    #define MTS_FRONTSAVE                   "MT_FRONTSAVE"      //ǰ����ģ��
    #define MTS_BILLPROCESSOR               "MT_BILLPROCESSOR"  //��������ģ��
    #define MTS_BACKSAVE                    "MT_BACKSAVE"       //�����ģ��
    #define MTS_MONITOR                     "MT_MONITOR"        //���ģ��
    #define MTS_CLSTRINTERFACE              "MT_CLSTRINTERFACE" //˫���ӿ�ģ��
    #define MTS_ALARM                       "MT_ALARM"          //�澯ģ��
    #define MTS_BACKUP                      "MT_BACKUP"         //����ģ��
    #define MTS_MMLSERVER                   "MT_MMLSERVER"      //MML SERVERģ��
    #define MTS_MMLDEBUG                    "MT_MMLDEBUG"       //MML���е���̨
    #define MTS_BILLBROWSER                 "MT_BILLBROWSER"    //���������ѯģ��
    #define MTS_LOG                         "MT_LOG"            //��־����ģ��
    #define MTS_MSGDIST                     "MT_MSGDIST"        //��Ϣ��תģ��
    #define MTS_CLSTR                       "MT_CLSTR"          //˫��ģ��
    #define MTS_PERF                        "MT_PERF"           //����ģ��
    #define MTS_MMLDEBUG                    "MT_MMLDEBUG"       //MML���е���̨
    #define MTS_BILLTASK                    "MT_BILLTASK"       //SCP�����ָ�ģ��
	#define MTS_CDRSTAT						"MT_CDRSTAT"		//����ͳ��ģ��
    #define MTS_CFGSVR                      "MT_CFGSVR"         //Զ�̲�������ģ��
	#define MTS_LICENSE_MANAGER				"MT_LICENSEMANGER"	//License����ģ��
    #define MTS_COLLECTOR                   "MT_COLLECTOR"      //�ɼ�ģ��
    #define MTS_COLAGENT                    "MT_COLAGENT"       //�ɼ�MML����ģ��
    #define MTS_COLCONVERTOR                "MT_COLCONVERTOR"   //�ɼ�����ת��ģ��
    #define MTS_FORMAT                      "MT_FORMAT"         //��ʽ����ģ��
    #define MTS_METERACC                    "MT_METERACC"

    //GTP'�Ʒѽӿ����� Z30971 2004/06    
    #define MTS_CDRSENDER                   "MT_CDRSENDER"      //��������ģ��

    /* OMCSERVER �Ľӿ�ģ�� */

    #define MTS_OMCSERVERINTERFACE          "MT_OMCSERVERINTERFACE"

#endif

