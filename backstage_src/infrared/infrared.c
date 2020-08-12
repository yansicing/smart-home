#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pthread.h>
#include<string.h>
#include"infrared.h"
#include"../agreement.h"

//����Э�飬����led��
int send_cmd_set_led_status(int num)
{
	char bag[256] = "";
	char other_data[10] = "";
	//num=1�����Ƶ�һ���ƣ�һ������
	snprintf(other_data, sizeof(other_data), "%d", num);
	
	group_sunhome_bag(bag, CLIENT_REMOTE_CONTROL, 
		IGNORE_PORT, SUNHOME_LED_SET_CMD, other_data);
	sunhome_send_bag_to_client(bag, LOOPBACK_ADDR_IP, SERVER_LOCAL_RECV_PORT);
	return 0;
}

//����mplayer�����͸�gtk����Э��
void send_cmd_start_mplayer(int cmd_word)
{
	char bag[256] = "";
	group_sunhome_bag(bag, CLIENT_REMOTE_CONTROL, 
		IGNORE_PORT, cmd_word, NULL);//CLIENT_REMOTE_CONTROL������Ʒ�ʽ
	sunhome_send_bag_to_client(bag, LOOPBACK_ADDR_IP, SERVER_LOCAL_RECV_PORT);
}

//mplayer����ͣ����һ��������++,����--
static void control_mplayer(int cmd)
{
	int fd = open(MPLAYER_MSG_FIFO_PATH, O_RDWR);//д��mplayer�ڶ���Ϣ�������ܵ���
	if(fd < 0){
		perror("open msg fifo");
		return;
	}
	char *str[] = {"ROMOTE_CMD=0\n", "ROMOTE_CMD=1\n", 
		"ROMOTE_CMD=2\n","ROMOTE_CMD=3\n"};
	
	write(fd, str[cmd], strlen(str[cmd]));
	close(fd);
}

static void control_chess(int cmd)
{
	int fd = open(CHESS_FIFO_PATH, O_RDWR);
	if(fd < 0){
		perror("open chsee fifo");
		return;
	}
	char *str[] = {"key:0", "key:1", "key:2", "key:3", "key:4"};
	write(fd, str[cmd], strlen(str[cmd]));
	close(fd);
}

//��������̣߳����϶�ȡ
void *pthread_infrared_recv(void *arg)
{
	int fd = open(INFRARDE_DEV_PATH, O_RDWR);
	if(fd < 0){
		perror("open infrared error\n");
		return NULL;
	}
	unsigned int key;
	int ret;
	while(1)
	{
		ret = read(fd, &key, sizeof(key)); 
		if(ret == sizeof(key)){
			printf("the key = %u\n",key);
			switch(key){
				case 1:
					control_mplayer(1);//next
					break;
				case 2:
					control_mplayer(2);//vol ++
					break;
				case 3:
					send_cmd_set_led_status(key);//led��3���ƿ���
					break;
				case 4:
					send_cmd_start_mplayer(SUNHOME_MPLAYER_START_CMD);//����Mplayer��ֹͣmplayer
					break;

				case 5:
					//control_mplayer(0);//pause
					control_chess(1);
					break;
				case 6:
					//control_mplayer(1);//next
					control_chess(0);
					break;
				case 7:
					//control_mplayer(2);//vol ++
					control_chess(3);
					break;
				case 8:
					//control_mplayer(3);//vol --
					control_chess(2);
					break;
				case 9:
					control_chess(4);
					break;
				default:
					break;
			}
		}
	}
	return NULL;
}


