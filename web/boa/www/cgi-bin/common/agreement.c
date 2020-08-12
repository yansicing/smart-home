#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/un.h>

#include"agreement.h"

/***************************************
函数功能:整形数字转换为字符串
参数类型:数字，str=存储字符串
返回描述:存储字符串
****************************************/
char *sunhome_int_to_str(int num, char *str)
{
	if(str == NULL)
		return NULL;
	if(sprintf(str, "%d", num) == -1)
		return NULL;
	return str;
}

/***************************************
函数功能:浮点数字转换为字符串
参数类型:数字，str=字符串
返回描述:字符串
****************************************/
char *sunhome_float_to_str(float num, char *str)
{
	if(str == NULL)
		return NULL;
	if(sprintf(str, "%f", num) == -1)
		return NULL;
	return str;
}

/*******所有数据组包********/
char *group_sunhome_bag(char *bag, int client_mode,
			unsigned short port, int cmd_word, char *data)
{
	int s_flag = -1;
	if(bag == NULL)
		return NULL;

	if(data == NULL)
		data = "NULL";

	s_flag = sprintf(bag, "%s:%d:%d:%d:%s",
		SUNHOME_HEAD, client_mode, port, cmd_word, data);
	if(s_flag == -1)
		return NULL;
	
	return bag;
}

/*******本地客户端组包,客户端的模式、反馈信息端口固定********/
char *group_sunhome_local_client_to_server_bag(char *bag, 
					int cmd_word, char *data)
{
	int s_flag = -1;
	if(bag==NULL)
		return NULL;

	if(data==NULL)
		data = "NULL";

	s_flag = sprintf(bag, "%s:%d:%d:%d:%s",
		SUNHOME_HEAD, CLIENT_LOCAL_HOST,
		CLIENT_LOCAL_RECV_PORT, cmd_word, data);
	if(s_flag == -1)
		return NULL;
	
	return bag;
}


//===================================
/*******获取包的头部********/
char *get_sunhome_bag_head(const char *bag, char *head)
{
	if(bag==NULL || head==NULL){
		return NULL;
	}
	if(sscanf(bag, "%[^:]", head) == -1)
		return NULL;
	return head;
}

/*******获取客户端的模式********/
int get_sunhome_bag_client_mode(const char *bag, int *client_mode)
{
	if(bag == NULL || client_mode==NULL){
		return -1;
	}
	if(sscanf(bag, "%*[^:]:%d:", client_mode) == -1)
		return -1;
	
	return *client_mode;
}


/*******获取回馈数据的端口********/
unsigned short get_sunhome_bag_port(const char *bag, unsigned short *port)
{
	unsigned int temp = -1;
	if(bag == NULL || port==NULL){
		return -1;
	}
	if(sscanf(bag, "%*[^:]:%*[^:]:%u:", &temp) == -1)
		return -1;
	
	*port = (unsigned short)temp;
	return *port;
}

/*******获取命令字********/
int get_sunhome_bag_cmd_word(const char *bag, int *cmd_word)
{
	if(bag == NULL || cmd_word == NULL){
		return -1;
	}
	if(sscanf(bag, "%*[^:]:%*[^:]:%*[^:]:%d:", cmd_word) == -1)
		return -1;
	
	return *cmd_word;	
}

/*******获取附加信息********/
char *get_sunhome_bag_other_data(const char *bag, char *other_data)
{
	if(bag==NULL || other_data==NULL){
		return NULL;
	}
	sscanf(bag, "%*[^:]:%*[^:]:%*[^:]:%*[^:]:%s", other_data);
	return other_data;
}

/*******获取包的所有数据********/
int get_sunhome_bag_all(const char *bag, int *client_mode, 
		unsigned short *port, int *cmd_word, char *other_data)
{
	unsigned int temp = -1;
	int s_flag = -1;
	if(bag==NULL || client_mode==NULL ||
		port==NULL || cmd_word==NULL || other_data==NULL)
	{
		return -1;
	}
	s_flag = sscanf(bag, "%*[^:]:%d:%d:%d:%s", client_mode,
		 &temp, cmd_word, other_data);
	*port = (unsigned short)temp;
	return s_flag;
}

//====================================
/*******获取UDP套接字********/
int sunhome_get_socket_fd(void)
{
	int sockfd = 0;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1)
	{
		perror("socket error");
		return -1;
	}
	
	return sockfd;
}

/*******绑定端口********/
int sunhome_bind_socket_fd(int sockfd, unsigned short port)
{
	int err_log = 0;
	struct sockaddr_in bindAddr;

	bzero(&bindAddr, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port   = htons(port);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	err_log = bind(sockfd, (struct sockaddr *)&bindAddr, sizeof(bindAddr));
	if(err_log == -1)
	{
		perror("bind error");
		close(sockfd);
		return err_log;
	}
	return sockfd;
}


/*******根据IP和端口，发送数据********/
int sunhome_send_bag_to_client(const char *send_bag, char *client_ip, unsigned short port)
{
	int send_len = 0;
	int sockfd = 0;
	struct sockaddr_in dest_addr;

	sockfd = sunhome_get_socket_fd();
	
	bzero(&dest_addr, sizeof(dest_addr));	
	dest_addr.sin_family = AF_INET;	
	dest_addr.sin_port = htons(port);

	inet_pton(AF_INET, client_ip, &dest_addr.sin_addr);				

	send_len = sendto(sockfd, send_bag, strlen(send_bag), 0, 
				(struct sockaddr*)&dest_addr, sizeof(dest_addr));
	close(sockfd);
	return send_len;
}



/*********************************************
函数功能:接收客户端数据
参数类型:recv_bag=存储接收数据,port=接收端口,msec=等待接收的时间以毫秒为单位,msec=0表示永远等待,
*********************************************/
int sunhome_recv_bag_from_client(char *recv_bag, unsigned short port, long msec)
{
	int sockfd = 0;
	int err_log = -1;

	if(recv_bag == NULL)
		return -1;

	sockfd = sunhome_get_socket_fd();
	err_log = sunhome_bind_socket_fd(sockfd, port);
	if(err_log == -1){
		printf("bind port error\n");
		close(sockfd);
		return -1;
	}

	//=============
	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);
	struct timeval timeout;
	struct timeval *s_timeout = NULL;

	if(msec == 0){
		s_timeout = NULL;
	}else{
		timeout.tv_sec = msec/1000;
		timeout.tv_usec = (msec%1000)*1000;
		s_timeout = &timeout;
	}

	if(select(sockfd+1, &rset, NULL, NULL, s_timeout) > 0){
		if(FD_ISSET(sockfd, &rset)){
			struct sockaddr_in client_addr;					
			char recv_buf[256] = "";
			socklen_t cliaddr_len = sizeof(client_addr);	

			memset(&client_addr, 0, sizeof(client_addr));
			recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, 
					(struct sockaddr*)&client_addr, &cliaddr_len);	

			strcpy(recv_bag, recv_buf);
		}
	}else{
		close(sockfd);
		return -1;
	}
	
	close(sockfd);
	return 0;
		
}

/******************************************
函数功能:根据绑定的套接字接收相应数据
参数类型:recv_data=存储接收数据,ip=客户端ip
******************************************/
int sunhome_recv_data_with_sockfd(int sockfd, char *recv_data, char *ip)
{
	struct sockaddr_in client_addr;				
	int recv_len = 0;
	char recv_bag[SUNHOME_MAX_LENGTH] = "";
	socklen_t cliaddr_len = sizeof(client_addr);

	if(recv_data == NULL)
		return -1;

	memset(&client_addr, 0, sizeof(client_addr));
	recv_len = recvfrom(sockfd, recv_bag, sizeof(recv_bag), 0, 
				(struct sockaddr*)&client_addr, &cliaddr_len);//接收	

	
	if(ip != NULL){
		inet_ntop(AF_INET, &client_addr.sin_addr, ip, 16);
	}
	strcpy(recv_data, recv_bag);
	return recv_len;
}



