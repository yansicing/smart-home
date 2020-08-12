#include<gtk/gtk.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include"types.h"
#include"./sungtk_interface/sungtk_interface.h"
#include"agreement.h"
#include"home_init.h"

//发送命令给后台
int send_cmd_to_backstage(int cmd, char *other_data)
{
	char bag[256] = "";
	group_sunhome_local_client_to_server_bag(bag, cmd, other_data);
	sunhome_send_bag_to_client(bag, LOOPBACK_ADDR_IP, SERVER_LOCAL_RECV_PORT);
	return 0;
}
//发送命令给后台控制led灯
int send_cmd_get_led_status(void)
{
	char bag[256] = "";
	group_sunhome_local_client_to_server_bag(bag, SUNHOME_LED_GET_CMD, NULL);
	sunhome_send_bag_to_client(bag, LOOPBACK_ADDR_IP, SERVER_LOCAL_RECV_PORT);
	return 0;
}

//显示本地IP地址
static int display_local_ipaddr(smarthome_t *home, char *other_data)
{
	char ip[SUNHOME_IPADDR_LENGTH] = "";
	sscanf(other_data, "%s", ip);
	//printf("show111 ip========%s======\n", ip);
	gdk_threads_enter();
	gtk_label_set_text(GTK_LABEL(home->label_ip), ip);  // 设置内容
	gdk_threads_leave();

	return 0;
}

//显示led灯的状态
static int display_led_status(smarthome_t *home, char *other_data)
{
	int status = -1;
	int flag = -1;
	int i;
	flag = sscanf(other_data, "%d", &status);
	if(flag == -1)
		return -1;

	gdk_threads_enter();
	for(i=0;i<3;i++){
		if(status & (0x02<<i)){
			sungtk_button_set_image(home->switch_ctl.led[i], 
				LED_BUTTON_LIGHT_PIC, LED_BUTTON_WIDTH, LED_BUTTON_HEIGHT);
		}else{
			sungtk_button_set_image(home->switch_ctl.led[i], 
				LED_BUTTON_LIGHTOFF_PIC, LED_BUTTON_WIDTH, LED_BUTTON_HEIGHT);
		}
	}
	gdk_threads_leave();
	return 0;
}


//显示温湿度
int display_temp_humi(smarthome_t *home, char *other_data)
{
	char temp_str[16] = "";
	char humi_str[16] = "";
	sscanf(other_data, "%[^&]&%s", temp_str, humi_str);
 	gdk_threads_enter();
	gtk_label_set_text(GTK_LABEL(home->temp_humi.temp), temp_str);
	gtk_label_set_text(GTK_LABEL(home->temp_humi.humi), humi_str);
	gdk_threads_leave();

	return 1;
}

//温湿度获取线程
void *pthread_temp_humi_get(void *arg)
{
	while(1)
	{
		sleep(3);
		send_cmd_to_backstage(SUNHOME_TEMP_HUMI_GET_CMD, NULL);
	}
}

//处理后台数据
static int deal_recv_bag(const char *recv_bag, smarthome_t *home)
{
	int client_mode = 0;
	unsigned short port = 0;
	int cmd_word = 0;
	char data[128] = "";
	//解析协议数据
	get_sunhome_bag_all(recv_bag, &client_mode,
		&port, &cmd_word, data);
	//printf("cmd_word===%d\n", cmd_word);
	switch(cmd_word)
	{
		case SUNHOME_LOGIN_CMD:
			//display_login_success(home, data);
			break;
			
		case SUNHOME_LED_SET_CMD:
		case SUNHOME_LED_GET_CMD:
			display_led_status(home, data);
			break;

		case SUNHOME_TEMP_HUMI_GET_CMD:
			display_temp_humi(home, data);
			break;

		case SUNHOME_LOCAL_IP_SET_CMD:
		case SUNHOME_LOCAL_IP_GET_CMD:
			display_local_ipaddr(home, data);
			break;


		case SUNHOME_MPLAYER_START_CMD:
			start_mplayer_with_cmd(home);
			break;
	}

	return 1;
}


//gtk获取后台数据线程
void *pthread_recv_data(void *arg)
{
	smarthome_t *home = (smarthome_t *)arg;
	int sockfd = sunhome_get_socket_fd();
	sunhome_bind_socket_fd(sockfd, CLIENT_LOCAL_RECV_PORT);
	while(1)
	{
		int flag = 0;
		char recv_bag[256] = "";
		flag = sunhome_recv_data_with_sockfd(sockfd, recv_bag, NULL);
		printf("local client recv ===%s===\n", recv_bag);

		if(flag < 0)
			continue;

		deal_recv_bag(recv_bag, home);
	}

	return NULL;
}

