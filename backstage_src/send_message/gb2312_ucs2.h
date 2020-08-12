#ifndef __GB2312_UCS2_H__
#define __GB2312_UCS2_H__
#include "gprs_config.h"

/*************************************************************
* 文件：gb2312_ucs2.c gb2312_ucs2.h
* 功能：GB2312码与uncode 码相互转换。
* 		utf8码与uncode 码相互转换。
* 		gb2312码与uft8 码相互转换。
**************************************************************/

/*************************************************************
* 语法格式：unsigned short gb2312_to_ucs2(unsigned short ucs2)
* 实现功能：gb2312转换为ucs2
* 参数：	gb2312 待转换的gb2312编码
* 返回值：	ucs2编码
**************************************************************/
unsigned short gb2312_to_ucs2(unsigned short gb2312);

/*************************************************************
* 语法格式：unsigned short ucs2_to_gb2312(unsigned short ucs2)
* 实现功能：ucs2转gb2312
* 参数：	ucs2: 待转换的ucs2编码
* 返回值：	gb2312编码
**************************************************************/
unsigned short ucs2_to_gb2312(unsigned short ucs2);

/*************************************************************
* 语法格式：unsigned char *utf8_to_gb2312(
							const unsigned char *utf, unsigned char *gb2312)
* 实现功能：utf-8字符串转换为gb2312字符串
* 参数：	utf utf-8字符串
* 			gb2312 gb2312字符串
* 返回值：	
**************************************************************/
unsigned char *utf8_to_gb2312(const unsigned char *utf, unsigned char *gb2312);

/*************************************************************
* 语法格式：unsigned char *gb2312_to_utf8(
							const unsigned char *gb2312, unsigned char *utf)
* 实现功能：gb2312字符串转换为utf-8字符串
* 参数：	gb2312 gb2312字符串
* 			utf utf-8字符串
* 返回值：	
**************************************************************/
unsigned char *gb2312_to_utf8(const unsigned char *gb2312, unsigned char *utf);

/*************************************************************
* 语法格式： unsigned short ucs2_to_utf8(unsigned short *ucs2, unsigned char *utf8)
* 实现功能： ucs2转换为utf8
* 参数：	 ucs2待转换的ucs2字符指针，utf8转换后的字符指针
* 返回值：	 utf8指针
**************************************************************/
unsigned char *ucs2_to_utf8(unsigned char *ucs2, unsigned char *utf8,int length_ucs2);

/*************************************************************
* 语法格式： unsigned short utf8_to_ucs2( const unsigned char *utf8,unsigned short *ucs2)
* 实现功能： utf8转换为ucs2
* 参数：	 utf8  待转换的utf8指针，ucs2转换后的ucs2字符
* 返回值：	 ucs2指针
**************************************************************/
int utf8_to_ucs2( unsigned char *utf8, char *unicode);

/*************************************************************
 *gb2312 or utf8 字符转换为unicode码
 *返回值：
		数据长度
 *************************************************************/
int str_to_unicode(char *psrc, char *pdst, int dstlength);

#endif 
