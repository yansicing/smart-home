#ifndef __GB2312_UCS2_H__
#define __GB2312_UCS2_H__
/*************************************************************
* �ļ���gb2312_ucs2.c gb2312_ucs2.h
* ���ܣ�GB2312����uncode ���໥ת����
**************************************************************/

/*************************************************************
* �﷨��ʽ��unsigned short gb2312_to_ucs2(unsigned short ucs2)
* ʵ�ֹ��ܣ�gb2312ת��Ϊucs2
* ������	gb2312 ��ת����gb2312����
* ����ֵ��	ucs2����
**************************************************************/
unsigned short gb2312_to_ucs2(unsigned short gb2312);

/*************************************************************
* �﷨��ʽ��unsigned short ucs2_to_gb2312(unsigned short ucs2)
* ʵ�ֹ��ܣ�ucs2תgb2312
* ������	ucs2: ��ת����ucs2����
* ����ֵ��	gb2312����
**************************************************************/
unsigned short ucs2_to_gb2312(unsigned short ucs2);

/*************************************************************
*���ܣ�
* ˵��:
* UTF-8���ֽ�Ϊ��λ��Unicode���б��롣
* ��Unicode��UTF-8�ı��뷽ʽ���£�
* Unicode����(16����) --> UTF-8 �ֽ���(������) 
* U-00000000 ~ U-0000007F --> 0xxxxxxx  
* U-00000080 ~ U-000007FF --> 110xxxxx 10xxxxxx  
* U-00000800 ~ U-0000FFFF --> 1110xxxx 10xxxxxx 10xxxxxx  
* U-00010000 ~ U-001FFFFF --> 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
* U-00200000 ~ U-03FFFFFF --> 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
* U-04000000 ~ U-7FFFFFFF --> 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
* �ʸ���ÿ�����ֵĵ�һ���ֽڣ������֪��utf-8�����ɼ����ֽ����
*************************************************************/
int get_utf8_nbytes(unsigned char first_char);

/*************************************************************
* �﷨��ʽ��int utf8_to_ucs2(const char *utf8)
* ʵ�ֹ��ܣ�utf-8�ַ���ת��Ϊunicode��
* ������	utf utf-8�ַ���
* ����ֵ��	unicode����ֵ
*************************************************************/
int utf8_to_ucs2(const char *utf8);

/*************************************************************
* �﷨��ʽ��char *utf8str_to_ucs2str(char *dest, char *source)
* ���ܣ�	utf8�ַ���תΪucs2�ַ���
* ������	source��utf8�ַ���
			dest��ucs2�ַ���
* ����ֵ��	ucs2�ַ����׵�ַ
**************************************************************/
char *utf8str_to_ucs2str(char *dest, const char *source);

/*************************************************************
* �﷨��ʽ��unsigned char *utf8_to_gb2312(
							const unsigned char *utf, unsigned char *gb2312)
* ʵ�ֹ��ܣ�utf-8�ַ���ת��Ϊgb2312�ַ���
* ������	utf utf-8�ַ���
* 			gb2312 gb2312�ַ���
* ����ֵ��	
**************************************************************/
unsigned char *utf8_to_gb2312(const unsigned char *utf, unsigned char *gb2312);

/*************************************************************
* �﷨��ʽ��unsigned char *gb2312_to_utf8(
							const unsigned char *gb2312, unsigned char *utf)
* ʵ�ֹ��ܣ�gb2312�ַ���ת��Ϊutf-8�ַ���
* ������	gb2312 gb2312�ַ���
* 			utf utf-8�ַ���
* ����ֵ��	
**************************************************************/
	char *gb2312_to_utf8(const char *gb2312, char *utf);


#endif 