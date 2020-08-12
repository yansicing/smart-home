//=============================================================
// 文件名称：	gprs_config.c
// 功能描述:	config gprs转换
//=============================================================
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

//#include "uni.h"//str to uniconde
#include "gb2312_ucs2.h"//str to uniconde
//send AT\n
void gprs_mg323_sent_at(int uart_fd)
{
	write(uart_fd,"AT\n",sizeof("AT\n"));
}

/*
 *转换电话号码到pdu格式号码
 */
int gprs_mg323_covert_phonenuber_to_pdu(const char *psrc,char *pdst)
{
	int length = strlen(psrc);
	printf("psrc = %s\n",psrc);
	
	
	return length;
}

//7位编码
// 输入: psrc - 源字符串指针
//       nSrcLength - 源字符串长度
// 输出: pdst - 目标编码串指针
// 返回: 目标编码串长度
int gprs_mg323_encode_7bit(const char* psrc, unsigned char* pdst, int nsrclength)
{
	printf("gprs_mg323_encode_7bit\n");
	int nSrc;        // 源字符串的计数值
    int nDst;        // 目标编码串的计数值
    int nChar;        // 当前正在处理的组内字符字节的序号，范围是0-7
    unsigned char nLeft;    // 上一字节残余的数据

    // 计数值初始化
    nSrc = 0;
    nDst = 0;

    // 将源串每8个字节分为一组，压缩成7个字节
    // 循环该处理过程，直至源串被处理完
    // 如果分组不到8字节，也能正确处理
    while (nSrc < nsrclength)
    {
        // 取源字符串的计数值的最低3位
        nChar = nSrc & 7;

        // 处理源串的每个字节
        if(nChar == 0)
        {
            // 组内第一个字节，只是保存起来，待处理下一个字节时使用
            nLeft = *psrc;
        }
        else
        {
            // 组内其它字节，将其右边部分与残余数据相加，得到一个目标编码字节
            *pdst = (*psrc << (8-nChar)) | nLeft;

            // 将该字节剩下的左边部分，作为残余数据保存起来
            nLeft = *psrc >> nChar;

            // 修改目标串的指针和计数值
            pdst++;
            nDst++;
        }

        // 修改源串的指针和计数值
        psrc++;
        nSrc++;
    }

    // 返回目标串长度
    return nDst;
}
//7位解码
// 输入: psrc - 源编码串指针
//       nSrcLength - 源编码串长度
// 输出: pdst - 目标字符串指针
// 返回: 目标字符串长度
int gprs_mg323_decode_7bit(const char* psrc, unsigned char* pdst, int nsrclength)
{
	printf("gprs_mg323_decode_7bit\n");
	int nSrc;        // 源字符串的计数值
    int nDst;        // 目标解码串的计数值
    int nByte;        // 当前正在处理的组内字节的序号，范围是0-6
    unsigned char nLeft;    // 上一字节残余的数据

    // 计数值初始化
    nSrc = 0;
    nDst = 0;
    
    // 组内字节序号和残余数据初始化
    nByte = 0;
    nLeft = 0;

    // 将源数据每7个字节分为一组，解压缩成8个字节
    // 循环该处理过程，直至源数据被处理完
    // 如果分组不到7字节，也能正确处理
    while(nSrc<nsrclength)
    {
        // 将源字节右边部分与残余数据相加，去掉最高位，得到一个目标解码字节
        *pdst = ((*psrc << nByte) | nLeft) & 0x7f;

        // 将该字节剩下的左边部分，作为残余数据保存起来
        nLeft = *psrc >> (7-nByte);

        // 修改目标串的指针和计数值
        pdst++;
        nDst++;

        // 修改字节计数值
        nByte++;
        // 到了一组的最后一个字节
        if(nByte == 7)
        {
            // 额外得到一个目标解码字节
            *pdst = nLeft;

            // 修改目标串的指针和计数值
            pdst++;
            nDst++;

            // 组内字节序号和残余数据初始化
            nByte = 0;
            nLeft = 0;
        }
        // 修改源串的指针和计数值
        psrc++;
        nSrc++;
    }
    // 输出字符串加个结束符
    *pdst = '\0';
    // 返回目标串长度
    return nDst;
}
//8位编码
// 输入: psrc - 源字符串指针
//       nSrcLength - 源字符串长度
// 输出: pdst - 目标编码串指针
// 返回: 目标编码串长度
int gprs_mg323_encode_8bit(const unsigned char* psrc, char* pdst, int nsrclength)
{
	// 简单复制
    memcpy(pdst, psrc, nsrclength);	
    return nsrclength;
}
//8位解码
// 输入: psrc - 源编码串指针
//       nSrcLength - 源编码串长度
// 输出: pdst - 目标字符串指针
// 返回: 目标字符串长度
int gprs_mg323_decode_8bit(const unsigned char* psrc, char* pdst, int nsrclength)
{
	// printf("gprs_mg323_encode_8bit\n");
	// 简单复制
    memcpy(pdst, psrc, nsrclength);
    // 输出字符串加个结束符
    *pdst = '\0';
    return nsrclength;
}

//UCS2编码
// 输入: psrc - 源字符串指针
//       nSrcLength - 源字符串长度
// 输出: pdst - 目标编码串指针
// 返回: 目标编码串长度
int gprs_mg323_encode_ucs2(  char* psrc, char * pdst, int nsrclength)
{
	printf("gprs_mg323_encode_ucs2\n");	
	int ndstlength = nsrclength;  // UNICODE宽字符数目
    // 字符串-->UNICODE串
	ndstlength = str_to_unicode(psrc,pdst,nsrclength);
    // 返回目标编码串长度
    return ndstlength ;
}
//UCS2解码
// 输入: psrc - 源编码串指针
//       nSrcLength - 源编码串长度
// 输出: pdst - 目标字符串指针
// 返回: 目标字符串长度
int gprs_mg323_decode_ucs2(  char* pSrc,  char* pDst, int nSrcLength)
{
	printf("gprs_mg323_decode_ucs2\n");
	int nDstLength = 0;      // UNICODE宽字符数目

	// printf()
    // UNICODE串-->字符串
	ucs2_to_utf8(pSrc,pDst,nSrcLength);
    // 输出字符串加个结束符
    pDst[nDstLength] = '\0';

    // 返回目标字符串长度
    return nDstLength;
}

// 字节数据转换为可打印字符串
// 如：{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01" 
// 输入: pSrc - 源数据指针
//       nSrcLength - 源数据长度
// 输出: pDst - 目标字符串指针
// 返回: 目标字符串长度
int gprs_mg323_byte_to_string(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
    const char tab[]="0123456789ABCDEF";    // 0x0-0xf的字符查找表
	int i = 0;
    for (i = 0; i < nSrcLength; i++)
    {
        *pDst++ = tab[*pSrc >> 4];      // 输出高4位
        *pDst++ = tab[*pSrc & 0x0f];    // 输出低4位
        pSrc++;
    }

    // 输出字符串加个结束符
    *pDst = '\0';

    // 返回目标字符串长度
    return (nSrcLength * 2);
}



// 可打印字符串转换为字节数据
// 如："C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
// 输入: pSrc - 源字符串指针
//       nSrcLength - 源字符串长度
// 输出: pDst - 目标数据指针
// 返回: 目标数据长度
int gprs_mg323_string_to_byte(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	int i = 0;
    for (i = 0; i < nSrcLength; i += 2)
    {
        // 输出高4位
        if ((*pSrc >= '0') && (*pSrc <= '9'))
        {
            *pDst = (*pSrc - '0') << 4;
        }
        else
        {
            *pDst = (*pSrc - 'A' + 10) << 4;
        }

        pSrc++;

        // 输出低4位
        if ((*pSrc>='0') && (*pSrc<='9'))
        {
            *pDst |= *pSrc - '0';
        }
        else
        {
            *pDst |= *pSrc - 'A' + 10;
        }

        pSrc++;
        pDst++;
    }

    // 返回目标数据长度
    return (nSrcLength / 2);
}
// 正常顺序的字符串转换为两两颠倒的字符串，若长度为奇数，补'F'凑成偶数
// 如："8613851872468" --> "683158812764F8"
// 输入: pSrc - 源字符串指针
//       nSrcLength - 源字符串长度
// 输出: pDst - 目标字符串指针gsmInvertNumbers
// 返回: 目标字符串长度
int gprs_mg323_invert_number(const char* pSrc, char* pDst, int nSrcLength)
{
    int nDstLength;        // 目标字符串长度
    char ch;            // 用于保存一个字符
	int i = 0;
    // 复制串长度
    nDstLength = nSrcLength;

    // 两两颠倒
    for(i =0; i<nSrcLength;i+=2)
    {
        ch = *pSrc++;        // 保存先出现的字符
        *pDst++ = *pSrc++;    // 复制后出现的字符
        *pDst++ = ch;        // 复制先出现的字符
    }

    // 源串长度是奇数吗？
    if(nSrcLength & 1)
    {
        *(pDst-2) = 'F';    // 补'F'
        nDstLength++;        // 目标串长度加1
    }

    // 输出字符串加个结束符
    *pDst = '\0';

    // 返回目标字符串长度
    return nDstLength;
}
// PDU编码，用于编制、发送短消息
// 输入: psrc_num - 信息中心目标地址指针
// pdst_num - 发送短信目标地址
// 输出: 
// 返回: 用户PDU串长度

int gsm_mg323_encode_pdu(SM_PARAM *pdu, char* pDst)
{
    int nLength;               // 内部用的串长度
    int nDstLength;            // 目标PDU串长度
	int usrDstlength;
    unsigned char buf[256];    // 内部用的缓冲区

    // SMSC地址信息段
    nLength = strlen(pdu->TP_SC);    // SMSC地址字符串的长度    
    buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;    // SMSC地址信息长度
    buf[1] = pdu->PDU_type;        // 固定: 用国际格式号码
    nDstLength = gprs_mg323_byte_to_string(buf, pDst, 2);        						// 转换2个字节到目标PDU串
	nDstLength += gprs_mg323_invert_number(pdu->TP_SC, &pDst[nDstLength], nLength); 	// 转换SMSC号码到目标PDU串
	usrDstlength = nDstLength;
//	printf("nDstLength = %d\n",nDstLength);
	
    // TPDU段基本参数、目标地址等
    nLength = strlen(pdu->TP_CN);    		// TP-DA地址字符串的长度
    buf[0] = pdu->TP_HEADER;            // 是发送短信(TP-MTI=01)，TP-VP用相对格式(TP-VPF=10)
    buf[1] = pdu->TP_MR ;               // TP-MR=0
    buf[2] = (char)nLength;            	// 目标地址数字个数(TP-DA地址字符串真实长度)
    buf[3] =  pdu->TP_CPT;              // 固定: 用国际格式号码
	
    nDstLength += gprs_mg323_byte_to_string(buf, &pDst[nDstLength], 4);        // 转换4个字节到目标PDU串
    nDstLength += gprs_mg323_invert_number(pdu->TP_CN, &pDst[nDstLength], nLength);    // 转换TP-DA到目标PDU串

	// TPDU段协议标识、编码方式、用户信息等
    nLength = strlen(pdu->TP_UD);//pdu->TP_UD);    	// 用户信息字符串的长度
    buf[0] =  pdu->TP_PID;            	// 协议标识(TP-PID)
    buf[1] =  pdu->TP_DCS;            	// 用户信息编码方式(TP-DCS)
    buf[2] =  pdu->TP_VP;                 // 有效期(TP-VP)为5分钟

	if(pdu->TP_DCS == GSM_7BIT)    
    {
        // 7-bit编码方式
        buf[3] = nLength;            // 编码前长度
        // nLength = gsmEncode7bit(pdu->TP_UD, &buf[4], nLength+1) + 4;    // 转换TP-DA到目标PDU串
    }
    else if(pdu->TP_DCS == GSM_UCS2)
    {
        // UCS2编码方式
		buf[3] = gprs_mg323_encode_ucs2(pdu->TP_UD, &buf[4], nLength);    // 转换TP-DA到目标PDU串
		nLength = buf[3] + 4;        // nLength等于该段数据长度
    }
    else
    {
        // 8-bit编码方式
        // buf[3] = gsmEncode8bit(pSrc->TP_UD, &buf[4], nLength);    // 转换TP-DA到目标PDU串
        nLength = buf[3] + 4;        // nLength等于该段数据长度
    }	
    nDstLength += gprs_mg323_byte_to_string(buf, &pDst[nDstLength], nLength);        // 转换该段数据到目标PDU串
//	printf("nLength = %d\n",nLength);
//	printf("nDstLength = %d\n",nDstLength);
	
	return nDstLength-usrDstlength;//nDstLength--PDU数据总长度，usrDstlength--数据中心PDU数据长度
}
//用户pdu短信结构体初始化
//参数：用户数据结构体指针
//返回值:void
/*结构体类型定义
struct __mg323_msg_type{
	char TP_SCA;	//短消息中心地址长度，决定着号码类型+号码的长度  1字节
	char PDU_type;	//短消息中心号码类型	1字节
	char TP_SC[12];	//短消息中心号码	不确定几个字节  手机号12字节
	
	char TP_HEADER;	//文件头字节--设置短信息发送接收数据
	char TP_MR;		//信息类型
	char TP_CLE;	//被叫号码长度
	char TP_CPT;	//被叫PDU_type
	char TP_CN[12];//
	
	char TP_PID;	//协议表示
	char TP_DCS;	//数据编码标准
	char TP_VP;		//允许时间
	char TP_UDL;	//用户数据长度
	char*TP_UD;
}SM_PARAM;
*/
int gsm_mg323_pdu_init(SM_PARAM *pdu,  char * psrc_num,  char * pdst_num,  char *content)
{
	 // pdu->TP_SCA = 0;	//短消息中心地址长度，决定着号码类型+号码的长度  1字节
	 if((psrc_num==NULL)||(pdst_num==NULL)||(content==NULL))
		return -1;
	 pdu->PDU_type = 0x91;	//短消息中心号码类型	1字节
	 pdu->TP_SC = psrc_num;	//短消息中心号码指针
	
	 pdu->TP_HEADER = 0x11;	//文件头字节--设置短信息发送接收数据--是发送短信(TP-MTI=01)，TP-VP用相对格式(TP-VPF=10)
	 pdu->TP_MR = 0;		//信息类型--TP-MR=0
	 pdu->TP_CPT = 0x91;	//被叫号码类型
	 pdu->TP_CN = pdst_num;	//被叫号码指针

	 pdu->TP_PID = 0;		//协议表示
	 pdu->TP_DCS = 0x08;	//用户信息编码方式(TP-DCS)
	 pdu->TP_VP = 0;		//有效期(TP-VP)为5分钟
	 pdu->TP_UD = content;	//内容指针
	 
	 return 0;
}
