#ifndef __GB2312_UCS2_H__
#define __GB2312_UCS2_H__
#include "gprs_config.h"

/*************************************************************
* �ļ���gb2312_ucs2.c gb2312_ucs2.h
* ���ܣ�GB2312����uncode ���໥ת����
* 		utf8����uncode ���໥ת����
* 		gb2312����uft8 ���໥ת����
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
unsigned char *gb2312_to_utf8(const unsigned char *gb2312, unsigned char *utf);

/*************************************************************
* �﷨��ʽ�� unsigned short ucs2_to_utf8(unsigned short *ucs2, unsigned char *utf8)
* ʵ�ֹ��ܣ� ucs2ת��Ϊutf8
* ������	 ucs2��ת����ucs2�ַ�ָ�룬utf8ת������ַ�ָ��
* ����ֵ��	 utf8ָ��
**************************************************************/
unsigned char *ucs2_to_utf8(unsigned char *ucs2, unsigned char *utf8,int length_ucs2);

/*************************************************************
* �﷨��ʽ�� unsigned short utf8_to_ucs2( const unsigned char *utf8,unsigned short *ucs2)
* ʵ�ֹ��ܣ� utf8ת��Ϊucs2
* ������	 utf8  ��ת����utf8ָ�룬ucs2ת�����ucs2�ַ�
* ����ֵ��	 ucs2ָ��
**************************************************************/
int utf8_to_ucs2( unsigned char *utf8, char *unicode);

/*************************************************************
 *gb2312 or utf8 �ַ�ת��Ϊunicode��
 *����ֵ��
		���ݳ���
 *************************************************************/
int str_to_unicode(char *psrc, char *pdst, int dstlength);

#endif 
