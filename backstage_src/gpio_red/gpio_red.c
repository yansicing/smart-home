#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "s5pv210-gpio.h"

//硬件连接
//GPH0_6	RED

#define RED_BYTE 0x40	//红外I/O口位置
#define BEE_BYTE 0x08	//蜂鸣器I/O口位置


static void red_deal(int red_fd, int bee_fd)
{
	int red = 0;
	char bee = 0;
	double bee_num = 0;
	int bee_flag = 1;
	while(1)
	{
		read(red_fd, &red, sizeof(red));//读I/O非阻塞
		if(!(red & RED_BYTE)){			//低电平表示触发了
			bee_flag = 1;
			bee_num++;
			bee |= BEE_BYTE;
			write(bee_fd, &bee, sizeof(bee));//蜂鸣器I/O写高电平
			sleep(1);//
			continue;
		}
		
		if(bee_num <= 0){
			if(bee_flag == 1){
				bee_flag = 0;
				bee &= (~BEE_BYTE);
				write(bee_fd, &bee, sizeof(bee));
			}
		}else{
			bee_num -= 0.02;//触发一下，响5秒
		}
		
		usleep(1000*100);
		
	}
}

//读取红外探头I/O口线程
void *pthread_gpio_red(void *arg)
{
	int red_fd = 0;
	int bee_fd = 0;
	red_fd = open("/dev/gpH0", O_RDONLY);//红外对管
	if(red_fd < 0){
		perror("open gpH0 error");
		return NULL;
	}
	ioctl(red_fd, GPIO_SET_PIN_IN, 0x6);//设置为输入
	ioctl(red_fd, GPIO_SET_PIN, 0x6);
	
	bee_fd = open("/dev/gpA0", O_RDWR);//蜂鸣器
	if(bee_fd < 0){
		perror("open gpA0 error");
		return NULL;
	}
	ioctl(bee_fd, GPIO_SET_PIN_OUT, 0x3);//设置为输出
	ioctl(bee_fd, GPIO_CLR_PIN, 0x3);
	
	red_deal(red_fd, bee_fd);

	close(red_fd);
	close(bee_fd);
	return NULL;
}


