#include <gtk/gtk.h>	// 头文件
#include "sungtk_interface.h"
int main( int argc,char *argv[] )
{
	gtk_init(&argc, &argv);		// 初始化

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);	// 创建顶层窗口
	// 设置窗口边框的宽度(窗口里的控件与窗口边框间隔为15)
	gtk_container_set_border_width(GTK_CONTAINER(window), 15);

	GtkWidget *button = sungtk_button_new_with_label_and_image("1.png", "hello", 100, 100);

	// 把按钮放入窗口(窗口也是一种容器)
	gtk_container_add(GTK_CONTAINER(window), button);


	// 显示控件有两种方法：逐个显示，全部显示
	// gtk_widget_show(button);
	// gtk_widget_show(window);
	gtk_widget_show_all(window);	// 显示窗口全部控件

	gtk_main();		// 主事件循环

	return 0;
}


