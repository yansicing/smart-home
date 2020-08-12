#include "deal_button.h"
#include "deal_chess.h"
#include <pthread.h>
#include <gtk/gtk.h>

static void thread_create_init(void)
{
	pthread_t tid;
	pthread_create(&tid, NULL, pthread_chess_recv_cmd, (void *)NULL);//接收后台信息
	pthread_detach(tid);
}


/********gtk线程初始化*************/
void threads_gtk_init(void)
{
  	if( FALSE == g_thread_supported() ){  // 如果线程没有初始化
		g_thread_init(NULL); // 初始化线程，g_thread_init()必须放在gdk_threads_init()前面
	} 
	gdk_threads_init();
}

int main(int argc, char **argv)
{
	threads_gtk_init();
	gtk_init (&argc, &argv);

	thread_create_init();
	
	GtkWidget *window = design_main_window();	// 设置窗口
	
	set_control_chess(window); 	// control chess
	
	connect_button();			// 操作按钮
	
	show_win(window);			// 显示
	
	gtk_main();
	
	return 0;
}