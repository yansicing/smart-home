#include <gtk/gtk.h>
#include<string.h>
#include"sungtk_interface.h"

#define MOTIONLOCK_SIZE 320			//整个大小
#define MOTIONLOCK_IMG_SIZE 65		//一个按钮的大小
#define MOTIONLOCK_PRESS_PIC "../picture/press.png"	//被按下照片路径
#define MOTIONLOCK_UNPRESS_PIC "../picture/unpress.png"	//未按下的照片路径
#define HAND_PWD "14789"	//密码

static gboolean callback_motionlock_release_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	SungtkMotionLock *lock = (SungtkMotionLock *)data;

	char get_pwd[10] = "";
	sungtk_motion_lock_get_password(lock, get_pwd, sizeof(get_pwd));//获取密码
	printf("get_pwd===%s\n", get_pwd);
	
	if(strcmp(get_pwd, HAND_PWD)==0){
		printf("right\n");
	}else{
		printf("Error ! ! !\n");
	}
	
	sungtk_motion_lock_release_deal(lock);//最后必须要调用
	return TRUE;
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title((GtkWindow *)window, "Hello");
	gtk_widget_set_size_request(window, 400,300);
	
	GtkWidget *table = gtk_table_new(5, 5, TRUE);
	gtk_container_add(GTK_CONTAINER(window), table);

	
	//创建密码滑动锁，需要知道大小和图片路径
	SungtkMotionLock *lock = sungtk_motion_lock_new(MOTIONLOCK_SIZE, MOTIONLOCK_IMG_SIZE, 
		MOTIONLOCK_PRESS_PIC, MOTIONLOCK_UNPRESS_PIC);
	gtk_table_attach_defaults(GTK_TABLE(table), lock->event_box, 0, 5, 0, 5);
		
	//设置在滑动抬起后的回调函数
	sungtk_motion_lock_signal_release(lock, callback_motionlock_release_event);

	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}


