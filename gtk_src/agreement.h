#ifndef _AGREEMENT_H_
#define _AGREEMENT_H_


/*************************************
Э���ʽ:$SUNHOME:2:9550:1:other_data
$SUNHOME	=	Э��ͷ��(�Զ���)
2			=	�ͻ���ģʽ(������web��)
9549		=	�ͻ��˽��ն˿�
1			=	������
other_data	=	������Ϣ
**************************************/

#define SUNHOME_HEAD			"$SUNHOME"


#define SERVER_LOCAL_HOST			1
#define CLIENT_LOCAL_HOST			2			/*����*/
#define CLIENT_WEB					3			/*��ҳ*/
#define CLIENT_REMOTE_CONTROL		4			/*ң����*/

#define SERVER_LOCAL_RECV_PORT		9549		/*��̨���ն˿�*/
#define CLIENT_LOCAL_RECV_PORT		9550		/*���ؿͻ��˽��ն˿�*/
#define CLIENT_WEB_RECV_PORT		9551		/*web��cgi������ն˿�*/
#define CLIENT_CONTROL_PORT			9552		/*�Զ���*/


#define SUNHOME_MAX_LENGTH			256
#define SUNHOME_IPADDR_LENGTH		16


#define LOOPBACK_ADDR_IP			"127.0.0.1"	/*�ػ�ip��ַ*/	


#define SUNHOME_LOGIN_CMD					1

#define SUNHOME_LED_SET_CMD					101		/*����led������*/
#define SUNHOME_LED_GET_CMD					102		/*��ȡled��״̬����*/

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



/*******����ת��Ϊ�ַ���*******/
extern char *sunhome_int_to_str(int num, char *str);

/*******������ת��Ϊ�ַ���*******/
extern char *sunhome_float_to_str(float num, char *str);

/*******�����������********/
extern char *group_sunhome_bag(char *bag, int client_mode,
			unsigned short port, int cmd_word, char *data);

/*******���ؿͻ������********/
extern char *group_sunhome_local_client_to_server_bag(char *bag, 
					int cmd_word, char *data);

/*******��ȡ����ͷ��********/
extern char *get_sunhome_bag_head(const char *bag, char *head);

/*******��ȡ�ͻ��˵�ģʽ********/
extern int get_sunhome_bag_client_mode(const char *bag, int *client_mode);

/*******��ȡ�������ݵĶ˿�********/
extern unsigned short get_sunhome_bag_port(const char *bag, unsigned short *port);

/*******��ȡ������********/
extern int get_sunhome_bag_cmd_word(const char *bag, int *cmd_word);

/*******��ȡ������Ϣ********/
extern char *get_sunhome_bag_other_data(const char *bag, char *other_data);

/*******��ȡ������������********/
extern int get_sunhome_bag_all(const char *bag, int *client_mode,
		unsigned short *port, int *cmd_word, char *other_data);


/*******��ȡUDP�׽���********/
extern int sunhome_get_socket_fd(void);

/*******�󶨶˿�********/
extern int sunhome_bind_socket_fd(int sockfd, unsigned short port);

/*******����IP�Ͷ˿ڣ���������********/
extern int sunhome_send_bag_to_client(const char *send_bag, char *ip, unsigned short port);

/*********************************************
��������:���տͻ�������
��������:recv_bag=�洢��������,port=���ն˿�,msec=�ȴ����յ�ʱ���Ժ���Ϊ��λ,msec=0��ʾ��Զ�ȴ�,
*********************************************/
extern int sunhome_recv_bag_from_client(char *recv_bag, unsigned short port, long msec);

/******************************************
��������:���ݰ󶨵��׽��ֽ�����Ӧ����
��������:recv_data=�洢��������,ip=�ͻ���ip
******************************************/
extern int sunhome_recv_data_with_sockfd(int sockfd, char *recv_data, char *ip);

#endif

