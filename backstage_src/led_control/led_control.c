#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include"led_control.h"

/*���豸�ļ�*/
int gpio_open()
{
	int fd = open("/dev/gpH0",O_RDWR);
	if(fd<0)
	{
		perror("gpid open error");
		return -1;
	}
	return fd;
}

/*������Ӧ��I/O��*/
void gpio_init(int led_fd)
{
	ioctl(led_fd, GPIO_SET_PIN_OUT, 0x1);//����һ�����������ó����
	ioctl(led_fd, GPIO_SET_PIN_OUT, 0x2);//���ڶ������������ó����
	ioctl(led_fd, GPIO_SET_PIN_OUT, 0x3);//�����������������ó����
	
}

/*��ȡLED�Ƶ�״̬*/
int get_led_status(int fd)
{
	int led_status = 0;
	read(fd,&led_status,sizeof(led_status));//
	led_status &= 0x0f;//ֻȡ����λ��Ч
	return led_status;
}

/*************************************************************
* �������ܣ�	����LED�Ƶ�״̬
* �������ͣ�	�ڼ�����
* �������ͣ�	
**************************************************************/
int set_led(int num)
{
	int led_status = 0;
	int fd = gpio_open();
	
	read(fd,&led_status,sizeof(led_status));
	switch(num)
	{
	
	case 1:
		led_status ^= 0x02;//λ���ȡ��������һյ�Ƶ�״̬�ı�
		break;

	case 2:
		led_status ^= 0x04;	//���ڶ�յ�Ƶ�״̬�ı�
		break;

	case 3:
		led_status ^= 0x08;//������յ�Ƶ�״̬�ı�
		break;

	case 4:
		if((led_status & 0x0f) != 0x0f)
			led_status |= 0x0f;	//�����յ�Ʋ���ȫ���ͱ��ȫ����
		else
			led_status &= (~0x0f);	//�����յ��ȫ���ͱ��ȫ��
	default:
		printf("set_led num error\n");//����������

	
	}
	
	write(fd,&led_status,sizeof(led_status));
	close(fd);

	return led_status;
}

int set_led_control(int num)
{
	int fd = gpio_open();
	int led_status = 0;
	gpio_init(fd);
	close(fd);
	
	led_status = set_led(num);
	led_status &= 0x0f;
	return led_status;
}

int get_led_control()
{
	int fd = gpio_open();
	int status = 0;
	gpio_init(fd);	//��ʼ��led�Ƶ�����
	status = get_led_status(fd);//��ȡled�Ƶ�״̬
	close(fd);	//�ر��ļ�������

	return status;
}


int get_led_cmd(char *data, int *mode)
{
	sscanf(data, "%d", mode);
	return *mode;
}

int get_led_other_data(char *data, int *other)
{
	sscanf(data, "%*[^&]&%d", other);
	return *other;
}

int led_control(char *data, char *return_data)
{
	int mode = -1;
	int other = -1;
	int led_status = -1;
	int return_mode = -1;
	if(data==NULL || return_data==NULL)
		return -1;

	get_led_cmd(data, &mode);	//��ȡ����
	return_mode = get_led_other_data(data, &other);	
	if(mode == LED_SET_FLAG){
		led_status = set_led_control(other);
	}else if(mode == LED_GET_FALG){
		led_status = get_led_control();
	}else{
		return -1;
	}
	return sprintf(return_data, "%d", led_status);
}

