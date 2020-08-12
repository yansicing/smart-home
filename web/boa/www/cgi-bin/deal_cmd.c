#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include"./common/agreement.h"
#include"./common/get_browser_data.h"

void *pthread_send_bag(void *arg)
{
	char *bag = (char *)arg;
	usleep(1000);
	sunhome_send_bag_to_client(bag, LOOPBACK_ADDR_IP, SERVER_LOCAL_RECV_PORT);
	return NULL;
}

int main(int argc, char *argv[])
{
	printf("Content-type: text/html \n\n");
	char browser_data[256] = "";
	if(get_browser_data(browser_data) < 0)
		return -1;
		
	char bag[256] = "";
	int cmd_word = 0;
	char other_data[128] = "";
	if(sscanf(browser_data, "%d:%s", &cmd_word, other_data) < 0)
		return -1;
		
	group_sunhome_bag(bag, CLIENT_WEB, CLIENT_WEB_RECV_PORT,
		cmd_word, other_data);

	
	pthread_t tid;
	pthread_create(&tid, NULL, pthread_send_bag, (void *)bag);
	pthread_detach(tid);

	char recv_bag[256] = "";
	if(sunhome_recv_bag_from_client(recv_bag, CLIENT_WEB_RECV_PORT, 200) < 0 )
			return -1;


	memset(other_data, 0, sizeof(other_data));
	get_sunhome_bag_cmd_word(recv_bag, &cmd_word);
	get_sunhome_bag_other_data(recv_bag, other_data);
	//printf("recv_bag======%s==\n", recv_bag);
	printf("%d&%s", cmd_word, other_data);
	 
	return 0;
}
