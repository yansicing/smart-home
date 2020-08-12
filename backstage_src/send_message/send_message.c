#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <pthread.h>
#include "gb2312_ucs2.h"
#include "send_message.h"

#include "uart_init.h"//uart init
#include "gprs_config.h"//gprs init

#define CENTER_NUM "8613800755500"

static char *serial_dev[]={"/dev/s3c2410_serial0",
							"/dev/s3c2410_serial1",
							"/dev/s3c2410_serial2",
							"/dev/s3c2410_serial3"};

int gprs_send_msg(int uart_fd, char *tel, char *msg, char *center_num)
{
	SM_PARAM pdu ;
	char tel_temp[1024] = "";
	char tel_len[100] = "";
	write(uart_fd,"AT+CMGF=0\n",10);//将AT指令AT+CMGF=0按照文件描述符uart_fd写入到前面打开的文件中
	sleep(3);	//等待系统的回应
	gsm_mg323_pdu_init(&pdu, center_num, tel, msg);	//初始化以pdu格式编码的电话号码和信息
	int length = gsm_mg323_encode_pdu(&pdu, tel_temp);//检测你组包的号码和信息的长度
	sprintf(tel_len, "AT+CMGS=%d\n", length/2);	//将AT指令AT+CMGS=和号码长度的1/2组包
	uart_send_str(uart_fd, tel_len);//将AT指令AT+CMGS=和号码长度的1/2组的包按照文件描述符uart_fd写入到前面打开的文件中
	sleep(3);	//等待系统的回应
	uart_send_str(uart_fd, tel_temp);//将要发送的电话号码和信息组的包
	sleep(1);	//等待系统的回应
	char c=0x1a;	//写完信息需要发送“Ctrl+Z”来结束本次信息发送，0x1a为Ctrl+Z的ASCII码
	write(uart_fd, &c, sizeof(c));//将“Ctrl+Z”对应的ASCII码按照文件描述符uart_fd写入到

	return 0;
}


int gprs_call(int uart_fd, char *tel)
{
	char tel_cmd[64] = "";
	snprintf(tel_cmd, sizeof(tel_cmd), "ATD%s;\n", tel);//将AT指令ATD和电话号码带上分号“;”组包
	uart_send_str(uart_fd, tel_cmd);//将AT指令ATD和电话号码带上分号“;”组的包按照文件描述符uart_fd写入到前面打开的文件中
	
	return 0;
}


/*************************************************************
* ¹¦ÄÜ£º	gprs¹Òµç»°
* ²ÎÊý£º	´®¿ÚÎÄ¼þÃèÊö·û
* ·µ»ØÖµ£º	ÎÞ
**************************************************************/
int gprs_hang_up()
{
	int uart_fd = uart_init(serial_dev[2]);
	uart_send_str(uart_fd, "ATH\n");
	close(uart_fd);
	return 1;
}


int get_gprs_control_mode(char *data, int *mode)
{
	if(data==NULL || mode==NULL)
		return -1;
	return sscanf(data, "%d", mode);
}

int get_gprs_control_tel(char *data, char *tel)
{
	if(data==NULL || tel==NULL)
		return -1;
	return sscanf(data, "%*[^&]&%[^&]&", tel);
}

static char *get_str_addr(char *str, char c, int p_size)
{
	char *p = str;
	int i = 0;
	for(i=0;i<p_size;++i)
	{
		if((p = strchr(p, c)) == NULL){
			break;
		}
		p++;
	}
	return p;	
}

int get_gprs_control_msg(char *data, char *msg)
{
	char *p = NULL;
	p = get_str_addr(data, '&', 2);
	strcpy(msg, p);
	return 1;
}

int gprs_control(char *data, char *return_data)
{
	int mode = -1;
	int return_mode = -1;

	if(data==NULL || return_data==NULL)
		return -1;

	return_mode = get_gprs_control_mode(data, &mode);
	
	if(mode == GPRS_SEND_MSG){
		char tel[20] = "";
		char msg[20] = "";
		get_gprs_control_tel(data, tel);//获取需要发送信息的电话号码
	get_gprs_control_msg(data, msg);//获取需要发送的信息
	int uart_fd = uart_init(serial_dev[2]);//初始化第二个串口
	gprs_send_msg(uart_fd, tel, msg, CENTER_NUM);//将信息按照指定的电话号码发送出去
	close(uart_fd);	//关闭文件描述符
	
		
		
	}else if(mode == GPRS_CALL){
		char tel[20] = "";
		get_gprs_control_tel(data, tel);//获取需要拨打的电话号码
	int uart_fd = uart_init(serial_dev[2]);//初始化第二个串口
	gprs_call(uart_fd, tel);//拨打电话
	close(uart_fd);//关闭文件描述符
		
		
	}else if(mode == GPRS_HANG_UP){
		gprs_hang_up();
	}else{
		return -1;
	}

	return -1;
}


