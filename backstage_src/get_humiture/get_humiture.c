#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include"get_humiture.h"

/*************************************************************
* �������ܣ�	��am2311��ʪ���豸�ļ�
* �������ͣ�	
* �������ͣ�	�ļ�������
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
* �������ܣ�	��ȡam2311��ʪ�ȵ�ֵ
* �������ͣ�	fd=�ļ���������temp=����¶ȣ�humi=���ʪ��
* �������ͣ�	�ɹ�����1
**************************************************************/
int read_am2311_data(const int fd, float *temp, float *humi)
{
int data[2] = {0};
	if(read(fd,data,sizeof(data)) <= 0)//��ȡ��ʽΪһ��int������
		return -1;
	
	*temp = data[1]/10.0;//�¶�
	*humi = data[0]/10.0;//ʪ��

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

