#ifndef _UART_INIT_H_
#define _UART_INIT_H_

#include <termios.h>

void uart_print_attr(struct termios *options);

/*************************************************************
* ���ܣ�	���ô������Խṹ��
* ������	fd�������豸�ļ�������
			speed�����ڲ����ʣ���ѡ2400��4800��9600��115200�����Լ������޸ĳ�������֧�֣�
			data_bits������λ������ѡ5��6��7��8��
			stop_bits��ֹͣλ����ѡ1��2��
			check����żУ��λ����ѡ'N'��'O'��'E'���ֱ�Ӧ������żУ�顢��У�顢żУ�飩
			options���������ṹ�壬
* ����ֵ��	�������Խṹ��
**************************************************************/
struct termios *uart_set_attr(int fd,
					int speed,
					int data_bits,
					int stop_bits,
					int check, 
					struct termios *options);

/*************************************************************
* ���ܣ�	���ڳ�ʼ������
* ������	�����豸�ļ���
* ����ֵ��	�����豸�ļ�������
**************************************************************/
int uart_init(char *devname);

/*************************************************************
* ���ܣ�	���ڷ���ʼ������
* ������	�����豸�ļ�������
* ����ֵ��	��
**************************************************************/
void uart_uninit(int uart_fd);

/*************************************************************
* ���ܣ�	���ڷ����ַ���
* ������	uart_fd�������豸�ļ�������
			str�������͵��ַ�
* ����ֵ��	��
**************************************************************/
void uart_send_str(int uart_fd, char *str);

/*************************************************************
* ���ܣ�	���ڶ�һ���ַ��������趨��ʱ���ڶ�����������������
* ������	uart_fd�������豸�ļ�������
			buffer��������ݵ��ڴ���׵�ַ
			len��������ݵ��ڴ�ռ�Ĵ�С
			timeout_ms����ʱʱ��(��λ��ms)
* ����ֵ��	
			�ɹ���ʵ�ʶ������ַ���
			ʧ�ܣ�-1
**************************************************************/
int uart_readline(int uart_fd, char *buffer, int len, int timeout_ms);

/*************************************************************
* ���ܣ�	
* ������	uart_fd�������豸�ļ�������
			buffer��������ݵ��ڴ���׵�ַ
			buffer_size ���ݴ�С			
* ����ֵ��	
			�ɹ���ʵ��д����ַ���
			ʧ�ܣ�-1
**************************************************************/
int uart_send(int uart_fd, const void * buffer, int buffer_size);
#endif