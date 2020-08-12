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
	//���������ӽ���Ҳ����
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

//�źų�ʼ����
static void signal_init(smarthome_t *home)
{
	_home = home;
	signal(SIGINT, signal_callback);
	signal(SIGQUIT, signal_callback);
	signal(SIGCHLD, son_process_killed_deal);//�ӽ��̽������������յ�SIGCHLD�ź�
}



void start_data_init(smarthome_t *home)
{
	send_cmd_to_backstage(SUNHOME_LOCAL_IP_GET_CMD, NULL);//��ȡIP
	send_cmd_to_backstage(SUNHOME_LED_GET_CMD, NULL);//��ȡled��״̬
}

//�̳߳�ʼ��
void pthread_create_init(smarthome_t *home)
{
	pthread_t tid;
	pthread_create(&tid, NULL, pthread_recv_data, (void *)home);//���պ�̨��Ϣ
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

/********gtk�̳߳�ʼ��*************/
void threads_gtk_init(void)
{
  	if( FALSE == g_thread_supported() ){  // ����߳�û�г�ʼ��
		g_thread_init(NULL); // ��ʼ���̣߳�g_thread_init()�������gdk_threads_init()ǰ��
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
	/*������������*/
	static smarthome_t home;
	memset(&home, 0, sizeof(home));

	start_home_back(&home);

	threads_gtk_init();
	gtk_init(&argc, &argv);

	smarthome_init(&home);

	gtk_main();
	return 0;
}



