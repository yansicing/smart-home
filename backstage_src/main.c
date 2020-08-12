#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>
#include"thread_pool.h"
#include"receive_bag.h"
#include"agreement.h"
#include"./infrared/infrared.h"
#include"./gpio_red/gpio_red.h"

#define THREADS_LIMIT	3

void system_thread_pool_init(pool_t *pool)
{
	pool_init(pool, THREADS_LIMIT);
}


//红外探头接收
void infrared_init(void *arg)
{
	pthread_t tid;
	pthread_create(&tid, NULL, pthread_infrared_recv, arg);
	pthread_detach(tid);
}


//红外对管触发及蜂鸣器
void gpio_red_init(void *arg)
{
	pthread_t tid;
	pthread_create(&tid, NULL, pthread_gpio_red, arg);
	pthread_detach(tid);
}



int main(int argc, char *argv[])
{
	static pool_t pool;
	int sockfd = 0;
	int bind_flag = 0;

	infrared_init(NULL);
	//gpio_red_init(NULL);
	
	memset(&pool, 0, sizeof(pool));
	system_thread_pool_init(&pool);//线程池初始化

	sockfd = sunhome_get_socket_fd();//获取套接字
	if(sockfd == -1){
		printf("sockfd-----%d\n", sockfd);
		goto err_socker;
	}
	bind_flag = sunhome_bind_socket_fd(sockfd, SERVER_LOCAL_RECV_PORT);
	if(bind_flag ==-1){
		printf("sunhome bind error\n");
		goto err_bind;
	}

#if 0	
	pid_t pid;
	pid = fork();
	if(pid==0)
	{
		usleep(1000);
		char *msg = "$SUNHOME:2:9550:505:NULL";
		sunhome_send_bag_to_client(msg, "127.0.0.1", SERVER_LOCAL_RECV_PORT);
		//sendto_client(sockfd, msg);
		
		_exit(1);
	}
#endif 
	
	printf("SUNHOME is starting!!!\n");
	wait_recvfrom_client(sockfd, &pool);//等待命令接收


err_bind:
	close(sockfd);

err_socker:
	return 0;
}
