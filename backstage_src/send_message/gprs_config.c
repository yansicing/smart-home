//=============================================================
// �ļ����ƣ�	gprs_config.c
// ��������:	config gprsת��
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
 *ת���绰���뵽pdu��ʽ����
 */
int gprs_mg323_covert_phonenuber_to_pdu(const char *psrc,char *pdst)
{
	int length = strlen(psrc);
	printf("psrc = %s\n",psrc);
	
	
	return length;
}

//7λ����
// ����: psrc - Դ�ַ���ָ��
//       nSrcLength - Դ�ַ�������
// ���: pdst - Ŀ����봮ָ��
// ����: Ŀ����봮����
int gprs_mg323_encode_7bit(const char* psrc, unsigned char* pdst, int nsrclength)
{
	printf("gprs_mg323_encode_7bit\n");
	int nSrc;        // Դ�ַ����ļ���ֵ
    int nDst;        // Ŀ����봮�ļ���ֵ
    int nChar;        // ��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7
    unsigned char nLeft;    // ��һ�ֽڲ��������

    // ����ֵ��ʼ��
    nSrc = 0;
    nDst = 0;

    // ��Դ��ÿ8���ֽڷ�Ϊһ�飬ѹ����7���ֽ�
    // ѭ���ô�����̣�ֱ��Դ����������
    // ������鲻��8�ֽڣ�Ҳ����ȷ����
    while (nSrc < nsrclength)
    {
        // ȡԴ�ַ����ļ���ֵ�����3λ
        nChar = nSrc & 7;

        // ����Դ����ÿ���ֽ�
        if(nChar == 0)
        {
            // ���ڵ�һ���ֽڣ�ֻ�Ǳ�����������������һ���ֽ�ʱʹ��
            nLeft = *psrc;
        }
        else
        {
            // ���������ֽڣ������ұ߲��������������ӣ��õ�һ��Ŀ������ֽ�
            *pdst = (*psrc << (8-nChar)) | nLeft;

            // �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
            nLeft = *psrc >> nChar;

            // �޸�Ŀ�괮��ָ��ͼ���ֵ
            pdst++;
            nDst++;
        }

        // �޸�Դ����ָ��ͼ���ֵ
        psrc++;
        nSrc++;
    }

    // ����Ŀ�괮����
    return nDst;
}
//7λ����
// ����: psrc - Դ���봮ָ��
//       nSrcLength - Դ���봮����
// ���: pdst - Ŀ���ַ���ָ��
// ����: Ŀ���ַ�������
int gprs_mg323_decode_7bit(const char* psrc, unsigned char* pdst, int nsrclength)
{
	printf("gprs_mg323_decode_7bit\n");
	int nSrc;        // Դ�ַ����ļ���ֵ
    int nDst;        // Ŀ����봮�ļ���ֵ
    int nByte;        // ��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6
    unsigned char nLeft;    // ��һ�ֽڲ��������

    // ����ֵ��ʼ��
    nSrc = 0;
    nDst = 0;
    
    // �����ֽ���źͲ������ݳ�ʼ��
    nByte = 0;
    nLeft = 0;

    // ��Դ����ÿ7���ֽڷ�Ϊһ�飬��ѹ����8���ֽ�
    // ѭ���ô�����̣�ֱ��Դ���ݱ�������
    // ������鲻��7�ֽڣ�Ҳ����ȷ����
    while(nSrc<nsrclength)
    {
        // ��Դ�ֽ��ұ߲��������������ӣ�ȥ�����λ���õ�һ��Ŀ������ֽ�
        *pdst = ((*psrc << nByte) | nLeft) & 0x7f;

        // �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
        nLeft = *psrc >> (7-nByte);

        // �޸�Ŀ�괮��ָ��ͼ���ֵ
        pdst++;
        nDst++;

        // �޸��ֽڼ���ֵ
        nByte++;
        // ����һ������һ���ֽ�
        if(nByte == 7)
        {
            // ����õ�һ��Ŀ������ֽ�
            *pdst = nLeft;

            // �޸�Ŀ�괮��ָ��ͼ���ֵ
            pdst++;
            nDst++;

            // �����ֽ���źͲ������ݳ�ʼ��
            nByte = 0;
            nLeft = 0;
        }
        // �޸�Դ����ָ��ͼ���ֵ
        psrc++;
        nSrc++;
    }
    // ����ַ����Ӹ�������
    *pdst = '\0';
    // ����Ŀ�괮����
    return nDst;
}
//8λ����
// ����: psrc - Դ�ַ���ָ��
//       nSrcLength - Դ�ַ�������
// ���: pdst - Ŀ����봮ָ��
// ����: Ŀ����봮����
int gprs_mg323_encode_8bit(const unsigned char* psrc, char* pdst, int nsrclength)
{
	// �򵥸���
    memcpy(pdst, psrc, nsrclength);	
    return nsrclength;
}
//8λ����
// ����: psrc - Դ���봮ָ��
//       nSrcLength - Դ���봮����
// ���: pdst - Ŀ���ַ���ָ��
// ����: Ŀ���ַ�������
int gprs_mg323_decode_8bit(const unsigned char* psrc, char* pdst, int nsrclength)
{
	// printf("gprs_mg323_encode_8bit\n");
	// �򵥸���
    memcpy(pdst, psrc, nsrclength);
    // ����ַ����Ӹ�������
    *pdst = '\0';
    return nsrclength;
}

//UCS2����
// ����: psrc - Դ�ַ���ָ��
//       nSrcLength - Դ�ַ�������
// ���: pdst - Ŀ����봮ָ��
// ����: Ŀ����봮����
int gprs_mg323_encode_ucs2(  char* psrc, char * pdst, int nsrclength)
{
	printf("gprs_mg323_encode_ucs2\n");	
	int ndstlength = nsrclength;  // UNICODE���ַ���Ŀ
    // �ַ���-->UNICODE��
	ndstlength = str_to_unicode(psrc,pdst,nsrclength);
    // ����Ŀ����봮����
    return ndstlength ;
}
//UCS2����
// ����: psrc - Դ���봮ָ��
//       nSrcLength - Դ���봮����
// ���: pdst - Ŀ���ַ���ָ��
// ����: Ŀ���ַ�������
int gprs_mg323_decode_ucs2(  char* pSrc,  char* pDst, int nSrcLength)
{
	printf("gprs_mg323_decode_ucs2\n");
	int nDstLength = 0;      // UNICODE���ַ���Ŀ

	// printf()
    // UNICODE��-->�ַ���
	ucs2_to_utf8(pSrc,pDst,nSrcLength);
    // ����ַ����Ӹ�������
    pDst[nDstLength] = '\0';

    // ����Ŀ���ַ�������
    return nDstLength;
}

// �ֽ�����ת��Ϊ�ɴ�ӡ�ַ���
// �磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01" 
// ����: pSrc - Դ����ָ��
//       nSrcLength - Դ���ݳ���
// ���: pDst - Ŀ���ַ���ָ��
// ����: Ŀ���ַ�������
int gprs_mg323_byte_to_string(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
    const char tab[]="0123456789ABCDEF";    // 0x0-0xf���ַ����ұ�
	int i = 0;
    for (i = 0; i < nSrcLength; i++)
    {
        *pDst++ = tab[*pSrc >> 4];      // �����4λ
        *pDst++ = tab[*pSrc & 0x0f];    // �����4λ
        pSrc++;
    }

    // ����ַ����Ӹ�������
    *pDst = '\0';

    // ����Ŀ���ַ�������
    return (nSrcLength * 2);
}



// �ɴ�ӡ�ַ���ת��Ϊ�ֽ�����
// �磺"C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
// ����: pSrc - Դ�ַ���ָ��
//       nSrcLength - Դ�ַ�������
// ���: pDst - Ŀ������ָ��
// ����: Ŀ�����ݳ���
int gprs_mg323_string_to_byte(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	int i = 0;
    for (i = 0; i < nSrcLength; i += 2)
    {
        // �����4λ
        if ((*pSrc >= '0') && (*pSrc <= '9'))
        {
            *pDst = (*pSrc - '0') << 4;
        }
        else
        {
            *pDst = (*pSrc - 'A' + 10) << 4;
        }

        pSrc++;

        // �����4λ
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

    // ����Ŀ�����ݳ���
    return (nSrcLength / 2);
}
// ����˳����ַ���ת��Ϊ�����ߵ����ַ�����������Ϊ��������'F'�ճ�ż��
// �磺"8613851872468" --> "683158812764F8"
// ����: pSrc - Դ�ַ���ָ��
//       nSrcLength - Դ�ַ�������
// ���: pDst - Ŀ���ַ���ָ��gsmInvertNumbers
// ����: Ŀ���ַ�������
int gprs_mg323_invert_number(const char* pSrc, char* pDst, int nSrcLength)
{
    int nDstLength;        // Ŀ���ַ�������
    char ch;            // ���ڱ���һ���ַ�
	int i = 0;
    // ���ƴ�����
    nDstLength = nSrcLength;

    // �����ߵ�
    for(i =0; i<nSrcLength;i+=2)
    {
        ch = *pSrc++;        // �����ȳ��ֵ��ַ�
        *pDst++ = *pSrc++;    // ���ƺ���ֵ��ַ�
        *pDst++ = ch;        // �����ȳ��ֵ��ַ�
    }

    // Դ��������������
    if(nSrcLength & 1)
    {
        *(pDst-2) = 'F';    // ��'F'
        nDstLength++;        // Ŀ�괮���ȼ�1
    }

    // ����ַ����Ӹ�������
    *pDst = '\0';

    // ����Ŀ���ַ�������
    return nDstLength;
}
// PDU���룬���ڱ��ơ����Ͷ���Ϣ
// ����: psrc_num - ��Ϣ����Ŀ���ַָ��
// pdst_num - ���Ͷ���Ŀ���ַ
// ���: 
// ����: �û�PDU������

int gsm_mg323_encode_pdu(SM_PARAM *pdu, char* pDst)
{
    int nLength;               // �ڲ��õĴ�����
    int nDstLength;            // Ŀ��PDU������
	int usrDstlength;
    unsigned char buf[256];    // �ڲ��õĻ�����

    // SMSC��ַ��Ϣ��
    nLength = strlen(pdu->TP_SC);    // SMSC��ַ�ַ����ĳ���    
    buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;    // SMSC��ַ��Ϣ����
    buf[1] = pdu->PDU_type;        // �̶�: �ù��ʸ�ʽ����
    nDstLength = gprs_mg323_byte_to_string(buf, pDst, 2);        						// ת��2���ֽڵ�Ŀ��PDU��
	nDstLength += gprs_mg323_invert_number(pdu->TP_SC, &pDst[nDstLength], nLength); 	// ת��SMSC���뵽Ŀ��PDU��
	usrDstlength = nDstLength;
//	printf("nDstLength = %d\n",nDstLength);
	
    // TPDU�λ���������Ŀ���ַ��
    nLength = strlen(pdu->TP_CN);    		// TP-DA��ַ�ַ����ĳ���
    buf[0] = pdu->TP_HEADER;            // �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
    buf[1] = pdu->TP_MR ;               // TP-MR=0
    buf[2] = (char)nLength;            	// Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
    buf[3] =  pdu->TP_CPT;              // �̶�: �ù��ʸ�ʽ����
	
    nDstLength += gprs_mg323_byte_to_string(buf, &pDst[nDstLength], 4);        // ת��4���ֽڵ�Ŀ��PDU��
    nDstLength += gprs_mg323_invert_number(pdu->TP_CN, &pDst[nDstLength], nLength);    // ת��TP-DA��Ŀ��PDU��

	// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
    nLength = strlen(pdu->TP_UD);//pdu->TP_UD);    	// �û���Ϣ�ַ����ĳ���
    buf[0] =  pdu->TP_PID;            	// Э���ʶ(TP-PID)
    buf[1] =  pdu->TP_DCS;            	// �û���Ϣ���뷽ʽ(TP-DCS)
    buf[2] =  pdu->TP_VP;                 // ��Ч��(TP-VP)Ϊ5����

	if(pdu->TP_DCS == GSM_7BIT)    
    {
        // 7-bit���뷽ʽ
        buf[3] = nLength;            // ����ǰ����
        // nLength = gsmEncode7bit(pdu->TP_UD, &buf[4], nLength+1) + 4;    // ת��TP-DA��Ŀ��PDU��
    }
    else if(pdu->TP_DCS == GSM_UCS2)
    {
        // UCS2���뷽ʽ
		buf[3] = gprs_mg323_encode_ucs2(pdu->TP_UD, &buf[4], nLength);    // ת��TP-DA��Ŀ��PDU��
		nLength = buf[3] + 4;        // nLength���ڸö����ݳ���
    }
    else
    {
        // 8-bit���뷽ʽ
        // buf[3] = gsmEncode8bit(pSrc->TP_UD, &buf[4], nLength);    // ת��TP-DA��Ŀ��PDU��
        nLength = buf[3] + 4;        // nLength���ڸö����ݳ���
    }	
    nDstLength += gprs_mg323_byte_to_string(buf, &pDst[nDstLength], nLength);        // ת���ö����ݵ�Ŀ��PDU��
//	printf("nLength = %d\n",nLength);
//	printf("nDstLength = %d\n",nDstLength);
	
	return nDstLength-usrDstlength;//nDstLength--PDU�����ܳ��ȣ�usrDstlength--��������PDU���ݳ���
}
//�û�pdu���Žṹ���ʼ��
//�������û����ݽṹ��ָ��
//����ֵ:void
/*�ṹ�����Ͷ���
struct __mg323_msg_type{
	char TP_SCA;	//����Ϣ���ĵ�ַ���ȣ������ź�������+����ĳ���  1�ֽ�
	char PDU_type;	//����Ϣ���ĺ�������	1�ֽ�
	char TP_SC[12];	//����Ϣ���ĺ���	��ȷ�������ֽ�  �ֻ���12�ֽ�
	
	char TP_HEADER;	//�ļ�ͷ�ֽ�--���ö���Ϣ���ͽ�������
	char TP_MR;		//��Ϣ����
	char TP_CLE;	//���к��볤��
	char TP_CPT;	//����PDU_type
	char TP_CN[12];//
	
	char TP_PID;	//Э���ʾ
	char TP_DCS;	//���ݱ����׼
	char TP_VP;		//����ʱ��
	char TP_UDL;	//�û����ݳ���
	char*TP_UD;
}SM_PARAM;
*/
int gsm_mg323_pdu_init(SM_PARAM *pdu,  char * psrc_num,  char * pdst_num,  char *content)
{
	 // pdu->TP_SCA = 0;	//����Ϣ���ĵ�ַ���ȣ������ź�������+����ĳ���  1�ֽ�
	 if((psrc_num==NULL)||(pdst_num==NULL)||(content==NULL))
		return -1;
	 pdu->PDU_type = 0x91;	//����Ϣ���ĺ�������	1�ֽ�
	 pdu->TP_SC = psrc_num;	//����Ϣ���ĺ���ָ��
	
	 pdu->TP_HEADER = 0x11;	//�ļ�ͷ�ֽ�--���ö���Ϣ���ͽ�������--�Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
	 pdu->TP_MR = 0;		//��Ϣ����--TP-MR=0
	 pdu->TP_CPT = 0x91;	//���к�������
	 pdu->TP_CN = pdst_num;	//���к���ָ��

	 pdu->TP_PID = 0;		//Э���ʾ
	 pdu->TP_DCS = 0x08;	//�û���Ϣ���뷽ʽ(TP-DCS)
	 pdu->TP_VP = 0;		//��Ч��(TP-VP)Ϊ5����
	 pdu->TP_UD = content;	//����ָ��
	 
	 return 0;
}
