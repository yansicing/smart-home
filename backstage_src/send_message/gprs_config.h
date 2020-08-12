//=============================================================
// �ļ����ƣ�	gprs_config.c
// ��������:	config gprsת��
//=============================================================
#ifndef __GPRS_CONFIG_MG323_HUAWEI_h__
#define __GPRS_CONFIG_MG323_HUAWEI_h__

typedef struct __mg323_msg_type{
	char TP_SCA;	//����Ϣ���ĵ�ַ���ȣ������ź�������+����ĳ���  1�ֽ�
	char PDU_type;	//����Ϣ���ĺ�������	1�ֽ�
	char *TP_SC;	//����Ϣ���ĺ���	��ȷ�������ֽ�  �ֻ���12�ֽ�
	
	char TP_HEADER;	//�ļ�ͷ�ֽ�--���ö���Ϣ���ͽ�������
	char TP_MR;		//��Ϣ����
	char TP_CLE;	//���к��볤��
	char TP_CPT;	//����PDU_type
	char *TP_CN;//
	
	char TP_PID;	//Э���ʾ
	char TP_DCS;	//���ݱ����׼
	char TP_VP;		//����ʱ��
	char TP_UDL;	//�û����ݳ���
	char*TP_UD;		//�û�����ָ��
}SM_PARAM;

#define GSM_7BIT  0x00
#define GSM_8BIT  0x01
#define GSM_UCS2  0x08

// extern int gprs_mg323_encode_7bit(const char* pSrc, unsigned char* pDst, int nSrcLength);
// extern int gprs_mg323_decode_7bit(const char* pSrc, unsigned char* pDst, int nSrcLength);
extern int gprs_mg323_encode_ucs2(  char* psrc, char * pdst, int nsrclength);
extern int gsm_mg323_encode_pdu(SM_PARAM *pdu, char* pDst);
extern int gsm_mg323_pdu_init(SM_PARAM *pdu,  char * psrc_num,  char * pdst_num,  char *content);

#endif
