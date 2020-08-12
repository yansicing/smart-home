#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "s5pv210-gpio.h"

//Ӳ������
//GPH0_6	RED

#define RED_BYTE 0x40	//����I/O��λ��
#define BEE_BYTE 0x08	//������I/O��λ��


static void red_deal(int red_fd, int bee_fd)
{
	int red = 0;
	char bee = 0;
	double bee_num = 0;
	int bee_flag = 1;
	while(1)
	{
		read(red_fd, &red, sizeof(red));//��I/O������
		if(!(red & RED_BYTE)){			//�͵�ƽ��ʾ������
			bee_flag = 1;
			bee_num++;
			bee |= BEE_BYTE;
			write(bee_fd, &bee, sizeof(bee));//������I/Oд�ߵ�ƽ
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
			bee_num -= 0.02;//����һ�£���5��
		}
		
		usleep(1000*100);
		
	}
}

//��ȡ����̽ͷI/O���߳�
void *pthread_gpio_red(void *arg)
{
	int red_fd = 0;
	int bee_fd = 0;
	red_fd = open("/dev/gpH0", O_RDONLY);//����Թ�
	if(red_fd < 0){
		perror("open gpH0 error");
		return NULL;
	}
	ioctl(red_fd, GPIO_SET_PIN_IN, 0x6);//����Ϊ����
	ioctl(red_fd, GPIO_SET_PIN, 0x6);
	
	bee_fd = open("/dev/gpA0", O_RDWR);//������
	if(bee_fd < 0){
		perror("open gpA0 error");
		return NULL;
	}
	ioctl(bee_fd, GPIO_SET_PIN_OUT, 0x3);//����Ϊ���
	ioctl(bee_fd, GPIO_CLR_PIN, 0x3);
	
	red_deal(red_fd, bee_fd);

	close(red_fd);
	close(bee_fd);
	return NULL;
}


