#include<stdio.h>
#include<gtk/gtk.h>
#include<glade/glade.h>  
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<signal.h>
#include<sys/wait.h>
#include"types.h"
#include"home_init.h"
#include"agreement.h"
#include"home_display.h"

static smarthome_t* _home;

static void signal_callback(int signum)
{
	//结束，则将子进程也接收
	if(_home->mplayer.pid != 0){
		kill(_home->mplayer.pid, SIGINT);
		kill(_home->mplayer.pid, SIGKILL);
		//waitpid(_home->mplayer.pid, NULL, WNOHANG);
	}else if(_home->chess.pid != 0){
		kill(_home->chess.pid, SIGINT);
		kill(_home->chess.pid, SIGKILL);
	}

	gtk_main_quit();
}

//信号初始化，
static void signal_init(smarthome_t *home)
{
	_home = home;
	signal(SIGINT, signal_callback);
	signal(SIGQUIT, signal_callback);
	signal(SIGCHLD, son_process_killed_deal);//子进程结束，父进程收到SIGCHLD信号
}



void start_data_init(smarthome_t *home)
{
	send_cmd_to_backstage(SUNHOME_LOCAL_IP_GET_CMD, NULL);//获取IP
	send_cmd_to_backstage(SUNHOME_LED_GET_CMD, NULL);//获取led灯状态
}

//线程初始化
void pthread_create_init(smarthome_t *home)
{
	pthread_t tid;
	pthread_create(&tid, NULL, pthread_recv_data, (void *)home);//接收后台信息
	pthread_detach(tid);

	pthread_t tid_temp;
	pthread_create(&tid_temp, NULL, pthread_temp_humi_get, (void *)home);
	pthread_detach(tid_temp);
}



void smarthome_init(smarthome_t *home)
{
	pthread_create_init(home);

	win_back_init(home);

	start_data_init(home);

	signal_init(home);
}

/********gtk线程初始化*************/
void threads_gtk_init(void)
{
  	if( FALSE == g_thread_supported() ){  // 如果线程没有初始化
		g_thread_init(NULL); // 初始化线程，g_thread_init()必须放在gdk_threads_init()前面
	} 
	gdk_threads_init();
}

static void start_home_back(smarthome_t *home)
{
	home->back.pid = vfork();
	if(home->back.pid == 0){
		execl("./home_back", "home_back", NULL);
		perror("execl home_back error");
		_exit(-1);
	}
}

int main(int argc, char *argv[])
{
	/*创建背景窗口*/
	static smarthome_t home;
	memset(&home, 0, sizeof(home));

	start_home_back(&home);

	threads_gtk_init();
	gtk_init(&argc, &argv);

	smarthome_init(&home);

	gtk_main();
	return 0;
}



