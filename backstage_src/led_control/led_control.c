#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include"led_control.h"

/*打开设备文件*/
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

/*配置相应的I/O口*/
void gpio_init(int led_fd)
{
	ioctl(led_fd, GPIO_SET_PIN_OUT, 0x1);//将第一个灯引脚设置成输出
	ioctl(led_fd, GPIO_SET_PIN_OUT, 0x2);//将第二个灯引脚设置成输出
	ioctl(led_fd, GPIO_SET_PIN_OUT, 0x3);//将第三个灯引脚设置成输出
	
}

/*获取LED灯的状态*/
int get_led_status(int fd)
{
	int led_status = 0;
	read(fd,&led_status,sizeof(led_status));//
	led_status &= 0x0f;//只取第四位有效
	return led_status;
}

/*************************************************************
* 函数功能：	设置LED灯的状态
* 参数类型：	第几个灯
* 返回类型：	
**************************************************************/
int set_led(int num)
{
	int led_status = 0;
	int fd = gpio_open();
	
	read(fd,&led_status,sizeof(led_status));
	switch(num)
	{
	
	case 1:
		led_status ^= 0x02;//位异或，取反，将第一盏灯的状态改变
		break;

	case 2:
		led_status ^= 0x04;	//将第二盏灯的状态改变
		break;

	case 3:
		led_status ^= 0x08;//将第三盏灯的状态改变
		break;

	case 4:
		if((led_status & 0x0f) != 0x0f)
			led_status |= 0x0f;	//如果三盏灯不是全亮就变成全亮，
		else
			led_status &= (~0x0f);	//如果三盏灯全亮就变成全灭
	default:
		printf("set_led num error\n");//其他，报错

	
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
	gpio_init(fd);	//初始化led灯的引脚
	status = get_led_status(fd);//获取led灯的状态
	close(fd);	//关闭文件描述符

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

	get_led_cmd(data, &mode);	//获取命令
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

