#ifndef _RECEIVE_BAG_H_
#define _RECEIVE_BAG_H_

#include"thread_pool.h"

struct bag_and_ip_t
{
	char *bag;
	char *ip;
};


/***************************************
函数功能:处理命令并且返回附加信息
参数类型:cmd_word=处理命令字，data=接收的附加信息，
			  deal_data=返回的附加信息
返回描述:失败返回-1
****************************************/
int deal_data_and_get_data(int cmd_word, char *data, char *deal_data);


/***************************************
函数功能:处理客户端发送的请求数据，并且返回数据给客户端
参数类型:协议包
返回描述:
****************************************/
void *deal_client_request(void *arg);

/***************************************
函数功能:等待客户端发送协议数据包
参数类型:sockfd=绑定好的套接字,pool=线程池结构图
返回描述:
****************************************/
int wait_recvfrom_client(int sockfd, pool_t *pool);	

#endif
