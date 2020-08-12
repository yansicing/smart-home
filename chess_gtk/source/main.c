#include "deal_button.h"
#include "deal_chess.h"
#include <pthread.h>
#include <gtk/gtk.h>

static void thread_create_init(void)
{
	pthread_t tid;
	pthread_create(&tid, NULL, pthread_chess_recv_cmd, (void *)NULL);//���պ�̨��Ϣ
	pthread_detach(tid);
}


/********gtk�̳߳�ʼ��*************/
void threads_gtk_init(void)
{
  	if( FALSE == g_thread_supported() ){  // ����߳�û�г�ʼ��
		g_thread_init(NULL); // ��ʼ���̣߳�g_thread_init()�������gdk_threads_init()ǰ��
	} 
	gdk_threads_init();
}

int main(int argc, char **argv)
{
	threads_gtk_init();
	gtk_init (&argc, &argv);

	thread_create_init();
	
	GtkWidget *window = design_main_window();	// ���ô���
	
	set_control_chess(window); 	// control chess
	
	connect_button();			// ������ť
	
	show_win(window);			// ��ʾ
	
	gtk_main();
	
	return 0;
}