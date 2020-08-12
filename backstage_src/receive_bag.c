#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include"receive_bag.h"
#include"thread_pool.h"
#include"agreement.h"
#include"./login/login.h"
#include"./led_control/led_control.h"
#include"./get_humiture/get_humiture.h"
#include"./send_message/send_message.h"
#include"./deal_IP/deal_IP.h"
#include"./mplayer_control/mplayer_control.h"


/***************************************
函数功能:处理命令并且返回附加信息
参数类型:cmd_word=处理命令字，data=接收的附加信息，
			  deal_data=返回的附加信息
返回描述:失败返回-1
****************************************/
int deal_data_and_get_data(int cmd_word, char *data, char *deal_data)
{
	int return_flag = -1;
	char hard_bag[128] = "";	//与硬件控制的组包
	if(data==NULL || deal_data==NULL)
		return -1;
	
	switch(cmd_word)
	{
		case SUNHOME_LOGIN_CMD:
			sprintf(hard_bag, "1&%s", data);
			return_flag = login_deal(hard_bag, deal_data);
			break;
		
		case SUNHOME_LED_SET_CMD://设置led
			sprintf(hard_bag, "1&%s", data);
			return_flag = led_control(hard_bag, deal_data);
			break;

		case SUNHOME_LED_GET_CMD://获取led值
			sprintf(hard_bag, "2&%s", data);
			return_flag = led_control(hard_bag, deal_data);
			break;

		case SUNHOME_TEMP_HUMI_GET_CMD://温湿度获取
			sprintf(hard_bag, "1&%s", data);
			return_flag = get_humiture(hard_bag, deal_data);
			break;

		case SUNHOME_GPRS_SEND_MSG_CMD://GPRS发送短信
			sprintf(hard_bag, "1&%s", data);
			return_flag = gprs_control(hard_bag, deal_data);
			break;

		case SUNHOME_GPRS_CALL_CMD://GPRS拨打电话
			sprintf(hard_bag, "2&%s", data);
			printf("---------======================\n");
			return_flag = gprs_control(hard_bag, deal_data);
			break;

		case SUNHOME_GPRS_HANG_UP_CMD://GPRS挂电话
			sprintf(hard_bag, "3&%s", data);
			return_flag = gprs_control(hard_bag, deal_data);
			break;
			
		case SUNHOME_LOCAL_IP_GET_CMD://本机IP地址获取
			sprintf(hard_bag, "1&%s", data);
			return_flag = deal_ip(hard_bag, deal_data);
			break;

		case SUNHOME_LOCAL_IP_SET_CMD://本机IP地址设置
			sprintf(hard_bag, "2&%s", data);
			return_flag = deal_ip(hard_bag, deal_data);
			break;

		case SUNHOME_MPLAYER_START_CMD://mplayer启动
			return_flag = 1;
			break;

		default:
			break;
	}

	return return_flag;
}


/***************************************
函数功能:处理客户端发送的请求数据，并且返回数据给客户端
参数类型:协议包
返回描述:
****************************************/
void *deal_client_request(void *arg)
{
	struct bag_and_ip_t *bag_ip = (struct bag_and_ip_t *)arg;
	if(arg == NULL)
		return NULL;
	
	int client_mode = 0;//客户端模式
	unsigned short port = 0;//接收端口
	int cmd_word = 0;//命令字
	char data[128] = "";//附加信息
	int return_flag = 0;

	//printf("recv_bag===%s======\n", bag_ip->bag);
	if(sunhome_check_bag_is_right(bag_ip->bag) != 0)
		goto bag_free;

	//解析协议包，得到相应数据
	return_flag = get_sunhome_bag_all(bag_ip->bag, &client_mode, 
											&port, &cmd_word, data);
	if(return_flag == -1)
		goto bag_free;
	
	char deal_data[128] = "";
	//根据命令字处理数据，并且返回相应的附加信息
	return_flag = deal_data_and_get_data(cmd_word, data, deal_data);
	//printf("deal_data======%s====\n", deal_data);
	if(return_flag < 0){
		printf("deal_data from client is error\n");
		goto bag_free;
	}


	char send_bag[256] = "";//组包回送包
	group_sunhome_bag(send_bag, client_mode,
			port, cmd_word, deal_data);

	
	if(client_mode == CLIENT_LOCAL_HOST){
		goto bag_answer;
	}else if(client_mode == CLIENT_WEB){
		goto bag_local_answer;
	}else if(client_mode == CLIENT_REMOTE_CONTROL){
		goto bag_local_answer;
	}
	goto bag_free;
	

bag_local_answer:
	if(cmd_word == SUNHOME_LED_SET_CMD){
		sunhome_send_bag_to_client(send_bag, bag_ip->ip, CLIENT_WEB_RECV_PORT);
	}else if(cmd_word == SUNHOME_MPLAYER_START_CMD){
		sunhome_send_bag_to_client(send_bag, bag_ip->ip, CLIENT_LOCAL_RECV_PORT);
	}
	
bag_answer:
	sunhome_send_bag_to_client(send_bag, bag_ip->ip, port);//将处理后的结果返回个请求客户端

bag_free:
	free(bag_ip->bag);//释放申请的空间
	free(bag_ip->ip);
	free(bag_ip);
	return NULL;	
}


/***************************************
函数功能:等待客户端发送协议数据包
参数类型:sockfd=绑定好的套接字,pool=线程池结构图
返回描述:
****************************************/
int wait_recvfrom_client(int sockfd, pool_t *pool)
{
	if(sockfd == -1){
		printf("sockfd error in wait_recvfrom_client\n");
		return -1;
	}
	while(1){			
		char recv_bag[256] = "";
		char client_ip[16] = "";
		struct bag_and_ip_t *bag_ip;

		if(sunhome_recv_data_with_sockfd(sockfd, recv_bag, client_ip) < 0)
			continue;

		printf("recv_bag1111=======%s=======\n", recv_bag);

		bag_ip = (struct bag_and_ip_t *)malloc(sizeof(struct bag_and_ip_t));
		bag_ip->bag = (char *)malloc(strlen(recv_bag)+1);
		memset(bag_ip->bag, 0, strlen(recv_bag)+1);
		strcpy(bag_ip->bag, recv_bag);//

		bag_ip->ip = (char *)malloc(sizeof(client_ip));
		memset(bag_ip->ip, 0, sizeof(client_ip));
		strcpy(bag_ip->ip, client_ip);//

		pool_add_task(pool, deal_client_request, (void *)bag_ip);//启动一个任务
	}
	return 0;
}




