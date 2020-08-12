#include<stdio.h>
#include<string.h>
#include<gtk/gtk.h>
#include<pthread.h>
#include<signal.h>
#include<sys/wait.h>
#include<stdlib.h>
#include"types.h"
#include"player_display.h"
#include"player_process.h"
#include"debug.h"
#include"player_control.h"

static PLAYER *_player;

static void signal_son_killed(int signum)
{
	pid_t pid = waitpid(-1, NULL, WNOHANG);
	printf("Mplayer son pid == %d\n", pid);
}

static void signal_callback(int signum)
{
	if(_player->process.pid != 0){
		//kill(_player->process.pid, SIGINT);
		kill(_player->process.pid, SIGKILL);
	}
	gtk_main_quit();
}

static void signal_init(PLAYER *player)
{
	_player = player;
	signal(SIGINT, signal_callback);
	signal(SIGQUIT, signal_callback);
	signal(SIGHUP, signal_callback);
	signal(SIGCHLD, signal_son_killed);
}


static void player_init(PLAYER *player)
{
	path_init(player);
	lock_init(player);
	fifo_fd_init(player);
	
	player_window_init(player);/*���ڳ�ʼ��*/
	player_song_list_display(player);
	song_status_init(player);

	player_song_cur_display(player);
	
	signal_init(player);
}

/***************************************
��������:�����߳�
��������:PLayer�ṹ��
��������:
****************************************/
static void create_pthread(PLAYER *player)
{
	pthread_t pthread_send_cmd, pthread_rcv_msg;
	pthread_t pthread_show_lrc;
	
	pthread_create(&pthread_send_cmd,NULL,pthread_send_player_cmd,(void *)player);//��������
	pthread_create(&pthread_rcv_msg,NULL,pthread_rcv_player_msg,(void *)player);//������Ϣ
	pthread_create(&pthread_show_lrc,NULL,pthread_show_lrc_msg,(void *)player);//�����ʾ

	pthread_detach(pthread_send_cmd);
	pthread_detach(pthread_rcv_msg);
	pthread_detach(pthread_show_lrc);
}


int main(int argc, char *argv[])
{
	static PLAYER player;/*��������Mplayer�ṹ��*/
	memset(&player, 0, sizeof(player));

	thread_gtk_init();
	gtk_init (&argc, &argv);
	player_init(&player);/*Mplayer��ʼ��*/

	player_start(&player);/*����Mplayer*/

	create_pthread(&player);/*�̴߳�����ʼ��*/

	gtk_widget_show_all(player.win.win_back);
	gtk_widget_hide(player.win.volume_rate);
	gtk_main();
	exit(0);
	return 0;
}




