#include <gtk/gtk.h>
#include"sungtk_interface.h"

void callback1(GtkWidget *button, gpointer data)
{
	sungtk_button_set_image(button, "./image/play.bmp", 0, 0);//按钮修改图片
}

void callback2(GtkWidget *button, gpointer data)
{
	sungtk_image_load_picture(GTK_WIDGET(data),"./image/previous.bmp", 50, 50);//image控件修改图片
}

gboolean callback_press( GtkWidget *widget,GdkEventButton *event,gpointer data )
{
	printf("abcdef\n");
	return TRUE;
}

gboolean callback_motion( GtkWidget *widget,GdkEventMotion *event,gpointer data )
{
	printf("11111111111\n");
	static double old_y = 0;
	double new_y = event->y;
	if(new_y - old_y > 5){
		old_y = new_y;
		sungtk_scrolled_window_set_vadjustment_value(GTK_WIDGET(data),5);
	}else if(old_y - new_y > 5){
		old_y = new_y;
		sungtk_scrolled_window_set_vadjustment_value(GTK_WIDGET(data),-5);
	}
	return TRUE;
}


int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title((GtkWindow *)window, "Hello");
	gtk_widget_set_size_request(window, 400,300);
	//gtk_window_set_resizable((GtkWindow *)window, FALSE);	
	
	sungtk_background_set_picture(window, "./image/1.jpg", 400, 300);
	
	GtkWidget *table = gtk_table_new(5, 5, TRUE);
	gtk_container_add(GTK_CONTAINER(window), table);

	//滚动窗口
	GtkWidget *s_window = sungtk_scrolled_window_new();
	gtk_table_attach_defaults(GTK_TABLE(table), s_window, 2, 5, 0, 5);
	GtkWidget *label = gtk_label_new("ssssssssss\nasdfas\ndfjka\nsjd\nlkh\nnkas\ndnfk\nsahdfk\nasdfkash\ndkfh\nksdhfe\nwiflkd\nsdf\nafa;skfa\nksfas\n54f\n6a4s\nfas\nodj\nfa\ns\nkje9\nrajskfdj\nsf.zms\ndifue\nfzkdi\nfp;awm\nfsl\nd\njfaiejf\nalsjdei'\nakfa\nsuo\ngn\nad\nfwi\noljf\nlaj\nf9ow");
	GtkWidget *event = gtk_event_box_new();//事件盒子
	gtk_container_add(GTK_CONTAINER(event), label);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(s_window), event);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(s_window), GTK_POLICY_NEVER, GTK_POLICY_NEVER);//设置滚动条不出现
	//滚动窗口移动事件无效，点击事件有效，所以添加了事件盒子
	//类似的clist支持移动事件，所以也可以设置clist的移动事件
	gtk_widget_add_events(s_window, GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_MOTION_MASK);
	g_signal_connect(s_window, "button-press-event", G_CALLBACK(callback_press), NULL);
	g_signal_connect(event, "motion-notify-event", G_CALLBACK(callback_motion), s_window);



	//带图片按钮
	GtkWidget *button1 = sungtk_button_new_with_image("./image/next.bmp", 0, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), button1, 0, 1, 0, 1);
	g_signal_connect(button1, "clicked", G_CALLBACK(callback1), NULL);

	//image控件
	GtkWidget *image = sungtk_image_new_from_file("./image/pause.bmp", 50, 50);
	gtk_table_attach_defaults(GTK_TABLE(table), image, 0, 1, 1, 2);

	// 按钮设置颜色和大小
	GtkWidget *button2 = gtk_button_new_with_label("button2");
	sungtk_widget_set_font_color(button2, "red", TRUE);
	sungtk_widget_set_font_size(button2, 12, TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), button2, 0, 1, 2, 3);
	g_signal_connect(button2, "clicked", G_CALLBACK(callback2), image);

	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}


