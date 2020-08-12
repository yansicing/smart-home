//=============================================================
// 文件名称：	gprs_config.c
// 功能描述:	config gprs转换
//=============================================================
#ifndef __GPRS_CONFIG_MG323_HUAWEI_h__
#define __GPRS_CONFIG_MG323_HUAWEI_h__

typedef struct __mg323_msg_type{
	char TP_SCA;	//短消息中心地址长度，决定着号码类型+号码的长度  1字节
	char PDU_type;	//短消息中心号码类型	1字节
	char *TP_SC;	//短消息中心号码	不确定几个字节  手机号12字节
	
	char TP_HEADER;	//文件头字节--设置短信息发送接收数据
	char TP_MR;		//信息类型
	char TP_CLE;	//被叫号码长度
	char TP_CPT;	//被叫PDU_type
	char *TP_CN;//
	
	char TP_PID;	//协议表示
	char TP_DCS;	//数据编码标准
	char TP_VP;		//允许时间
	char TP_UDL;	//用户数据长度
	char*TP_UD;		//用户数据指针
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
