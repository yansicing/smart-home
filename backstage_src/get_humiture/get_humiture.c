#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include"get_humiture.h"

/*************************************************************
* 函数功能：	打开am2311温湿度设备文件
* 参数类型：	
* 返回类型：	文件描述符
**************************************************************/
int am2311_open()
{
    int fd;
	fd = open("/dev/am2311", O_RDWR);
	if(fd < 0)
	{
		perror("/dev/am2311 open");
		return -1;
	}
	
	
	return fd;
}

/*************************************************************
* 函数功能：	读取am2311温湿度的值
* 参数类型：	fd=文件描述符，temp=存放温度，humi=存放湿度
* 返回类型：	成功返回1
**************************************************************/
int read_am2311_data(const int fd, float *temp, float *humi)
{
int data[2] = {0};
	if(read(fd,data,sizeof(data)) <= 0)//读取格式为一个int型数字
		return -1;
	
	*temp = data[1]/10.0;//温度
	*humi = data[0]/10.0;//湿度

    return 1;
}

int get_temp_and_humi(float *temp, float *humi)
{
	int fd = am2311_open();
	if(fd < 0)
		return -1;
	if(read_am2311_data(fd, temp, humi) <= 0)
		return -1;

	close(fd);
	
	return 1;
}

int get_humiture_mode(char *data, int *mode)
{
	if(data==NULL || mode==NULL)
		return -1;
	return sscanf(data, "%d", mode);;
}

int get_humiture(char *data, char *return_data)
{
	int mode = -1;
	float temp = -1, humi = -1;
	int s_flag = -1;
	int return_mode = -1;
	
	if(data==NULL || return_data==NULL)
		return -1;
	return_mode = get_humiture_mode(data, &mode);
	if(mode == HUMITURE_GET){
		return_mode = get_temp_and_humi(&temp, &humi);
	}else{
		return -1;
	}
	s_flag = sprintf(return_data, "%f&%f", temp, humi);
	printf("temp===%f, humi===%f\n", temp, humi);
	

	return s_flag;
}

