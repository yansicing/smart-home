#ifndef _RECEIVE_BAG_H_
#define _RECEIVE_BAG_H_

#include"thread_pool.h"

struct bag_and_ip_t
{
	char *bag;
	char *ip;
};


/***************************************
��������:��������ҷ��ظ�����Ϣ
��������:cmd_word=���������֣�data=���յĸ�����Ϣ��
			  deal_data=���صĸ�����Ϣ
��������:ʧ�ܷ���-1
****************************************/
int deal_data_and_get_data(int cmd_word, char *data, char *deal_data);


/***************************************
��������:����ͻ��˷��͵��������ݣ����ҷ������ݸ��ͻ���
��������:Э���
��������:
****************************************/
void *deal_client_request(void *arg);

/***************************************
��������:�ȴ��ͻ��˷���Э�����ݰ�
��������:sockfd=�󶨺õ��׽���,pool=�̳߳ؽṹͼ
��������:
****************************************/
int wait_recvfrom_client(int sockfd, pool_t *pool);	

#endif
