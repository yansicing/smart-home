#ifndef _AGREEMENT_H_
#define _AGREEMENT_H_


/*************************************
协议格式:$SUNHOME:2:9550:1:other_data
$SUNHOME	=	协议头部(自定义)
2			=	客户端模式(本机，web等)
9549		=	客户端接收端口
1			=	命令字
other_data	=	附加信息
**************************************/

#define SUNHOME_HEAD			"$SUNHOME"


#define SERVER_LOCAL_HOST			1
#define CLIENT_LOCAL_HOST			2			/*本机*/
#define CLIENT_WEB					3			/*网页*/
#define CLIENT_REMOTE_CONTROL		4			/*遥控器*/

#define SERVER_LOCAL_RECV_PORT		9549		/*后台接收端口*/
#define CLIENT_LOCAL_RECV_PORT		9550		/*本地客户端接收端口*/
#define CLIENT_WEB_RECV_PORT		9551		/*web中cgi程序接收端口*/
#define CLIENT_CONTROL_PORT			9552		/*自定义*/


#define SUNHOME_MAX_LENGTH			256
#define SUNHOME_IPADDR_LENGTH		16


#define LOOPBACK_ADDR_IP			"127.0.0.1"	/*回环ip地址*/	


#define SUNHOME_LOGIN_CMD					1

#define SUNHOME_LED_SET_CMD					101		/*设置led灯命令*/
#define SUNHOME_LED_GET_CMD					102		/*获取led灯状态命令*/

#define SUNHOME_TEMP_HUMI_GET_CMD			201

#define SUNHOME_GPRS_SEND_MSG_CMD			301
#define SUNHOME_GPRS_CALL_CMD				302
#define SUNHOME_GPRS_HANG_UP_CMD			303

#define SUNHOME_LOCAL_IP_SET_CMD			401
#define SUNHOME_LOCAL_IP_GET_CMD			402

#define SUNHOME_MPLAYER_PAUSE_CMD			501
#define SUNHOME_MPLAYER_VOLUME_CMD			502
#define SUNHOME_MPLAYER_CHANGESONG_CMD		503
#define SUNHOME_MPLAYER_GET_SONGLIST_CMD	504
#define SUNHOME_MPLAYER_START_CMD			505



/*******整数转换为字符串*******/
extern char *sunhome_int_to_str(int num, char *str);

/*******浮点数转换为字符串*******/
extern char *sunhome_float_to_str(float num, char *str);

/*******所有数据组包********/
extern char *group_sunhome_bag(char *bag, int client_mode,
			unsigned short port, int cmd_word, char *data);

/*******本地客户端组包********/
extern char *group_sunhome_local_client_to_server_bag(char *bag, 
					int cmd_word, char *data);

/*******获取包的头部********/
extern char *get_sunhome_bag_head(const char *bag, char *head);

/*******获取客户端的模式********/
extern int get_sunhome_bag_client_mode(const char *bag, int *client_mode);

/*******获取回馈数据的端口********/
extern unsigned short get_sunhome_bag_port(const char *bag, unsigned short *port);

/*******获取命令字********/
extern int get_sunhome_bag_cmd_word(const char *bag, int *cmd_word);

/*******获取附加信息********/
extern char *get_sunhome_bag_other_data(const char *bag, char *other_data);

/*******获取包的所有数据********/
extern int get_sunhome_bag_all(const char *bag, int *client_mode,
		unsigned short *port, int *cmd_word, char *other_data);


/*******获取UDP套接字********/
extern int sunhome_get_socket_fd(void);

/*******绑定端口********/
extern int sunhome_bind_socket_fd(int sockfd, unsigned short port);

/*******根据IP和端口，发送数据********/
extern int sunhome_send_bag_to_client(const char *send_bag, char *ip, unsigned short port);

/*********************************************
函数功能:接收客户端数据
参数类型:recv_bag=存储接收数据,port=接收端口,msec=等待接收的时间以毫秒为单位,msec=0表示永远等待,
*********************************************/
extern int sunhome_recv_bag_from_client(char *recv_bag, unsigned short port, long msec);

/******************************************
函数功能:根据绑定的套接字接收相应数据
参数类型:recv_data=存储接收数据,ip=客户端ip
******************************************/
extern int sunhome_recv_data_with_sockfd(int sockfd, char *recv_data, char *ip);

#endif

