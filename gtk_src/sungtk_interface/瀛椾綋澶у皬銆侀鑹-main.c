#include <gtk/gtk.h>
#include"sungtk_interface.h"

void callback1(GtkWidget *button, gpointer data)
{
	sungtk_button_set_image(button, "./image/play.bmp", 0, 0);//按钮修改图片
}


int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title((GtkWindow *)window, "Hello");
	gtk_widget_set_size_request(window, 400,300);
	

	GtkWidget *table = gtk_table_new(5, 5, TRUE);
	gtk_container_add(GTK_CONTAINER(window), table);

	//带图片按钮
	GtkWidget *button1 = sungtk_button_new_with_image("./image/next.bmp", 0, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), button1, 0, 1, 0, 1);
	g_signal_connect(button1, "clicked", G_CALLBACK(callback1), NULL);

	//image控件
	GtkWidget *label = gtk_label_new("sunplusedu");
	sungtk_widget_set_font_color(label, "red", FALSE);
	sungtk_widget_set_font_size(label, 15, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 1, 2);

	// 按钮设置颜色和大小
	GtkWidget *button2 = gtk_button_new_with_label("button2");
	sungtk_widget_set_font_color(button2, "red", TRUE);
	sungtk_widget_set_font_size(button2, 12, TRUE);
	gtk_table_attach_defaults(GTK_TABLE(table), button2, 0, 1, 2, 3);
	
	
	//带label和image按钮,label不能用提供的接口这是大下和颜色
	GtkWidget *button3 = sungtk_button_new_with_label_and_image("./image/pause.bmp", "sunplusedu", 80, 80);
	gtk_table_attach_defaults(GTK_TABLE(table), button3, 2, 4, 2, 4);

	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}


