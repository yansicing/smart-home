#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "gtk_keyboard.h"
#include"../sungtk_interface/sungtk_interface.h"

/****************************************************
函数功能:创建键盘按钮
参数类型:自定义键盘结构体
返回值:按钮个数
*****************************************************/
static int create_keyboard_window(KEYBOARD *p_keyboard)
{
	p_keyboard->table = gtk_table_new(KEYBOARD_ROW, KEYBOARD_COL, TRUE);/*创建键盘表格*/
	
	int i;
	char text[50] = "";
	for(i=0;i<10;++i)
	{
		memset(text, 0, sizeof(text));
		snprintf(text, sizeof(text), "%d", i);
		//snprintf(text, sizeof(text), "../picture/keyboard/%d.jpg", i);
		//printf("text----%s--\n", text);
		p_keyboard->button[i] = gtk_button_new_with_label(text);/*设置0~9按钮的内容*/
		//p_keyboard->button[i] = sungtk_button_new_with_image(text, 40, 40);
	}
	 
	char j;
	for(j='a';j<='z';++j)
	{
		memset(text, 0, sizeof(text));
		snprintf(text, sizeof(text), "%c", j);
		//snprintf(text, sizeof(text), "../picture/keyboard/%c.jpg", j);
		p_keyboard->button[i++] = gtk_button_new_with_label(text);/*设置a~z按钮的内容*/
		//p_keyboard->button[i++] = sungtk_button_new_with_image(text, 40, 40);
	}
	
	p_keyboard->button[i++] = gtk_button_new_with_label(".");/*设置点'.'按钮的内容*/
	p_keyboard->button[i++] = gtk_button_new_with_label("clear");/*设置清空按钮的内容*/
	p_keyboard->button[i++] = gtk_button_new_with_label("backspace");/*设置删除一个字符按钮的内容*/
	//p_keyboard->button[i++] = sungtk_button_new_with_image("../picture/keyboard/point.jpg", 40, 40);
	//p_keyboard->button[i++] = sungtk_button_new_with_image("../picture/keyboard/clear.jpg", 40, 40);
	//p_keyboard->button[i++] = sungtk_button_new_with_image("../picture/keyboard/backspace.jpg", 80, 40);
	return i;
}

/****************************************************
函数功能:设置键盘大小和间距
参数类型:自定义键盘结构体
返回值:
*****************************************************/
static void config_keyboard_window(KEYBOARD *p_keyboard)
{
	gtk_widget_set_usize(p_keyboard->table, KEYBOARD_USIZE_WIDE, KEYBOARD_USIZE_HEIGHT );/*设置键盘大小*/
	gtk_table_set_row_spacings(GTK_TABLE(p_keyboard->table), KEY_ROW_SPACINGS);/*设置行间距*/
	gtk_table_set_col_spacings(GTK_TABLE(p_keyboard->table), KEY_COL_SPACINGS);/*设置列间距*/
}

/****************************************************
函数功能:键盘按钮布局
参数类型:自定义键盘结构体
返回值:按钮个数
*****************************************************/
static void layout_keyboard_window(KEYBOARD *p_keyboard)
{
	int i;
	for(i=1;i<=9;++i)
	{
		gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[i], i-1, i, 0, 1);/*添加1~9*/
	}
	--i;
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[0], i, i+1, 0, 1);/*把0添加进去*/
	
	/*添加第二行入表格布局*/
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[26], 0, 1, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[32], 1, 2, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[14], 2, 3, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[27], 3, 4, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[29], 4, 5, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[34], 5, 6, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[30], 6, 7, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[18], 7, 8, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[24], 8, 9, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[25], 9, 10, 1, 2);

	/*添加第三行入表格布局*/
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[10], 0, 1, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[28], 1, 2, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[13], 2, 3, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[15], 3, 4, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[16], 4, 5, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[17], 5, 6, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[19], 6, 7, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[20], 7, 8, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[21], 8, 9, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[37], 9, 10, 2, 3);

	/*添加第四行入表格布局*/
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[35], 0, 1, 3, 4);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[33], 1, 2, 3, 4);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[12], 2, 3, 3, 4);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[31], 3, 4, 3, 4);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[11], 4, 5, 3, 4);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[23], 5, 6, 3, 4);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[22], 6, 7, 3, 4);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[36], 7, 8, 3, 4);
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[38], 8, 10, 3, 4);
}

/****************************************************
函数功能:键盘按钮信号设定
参数类型:自定义键盘结构体，回调函数
返回值:按钮个数
*****************************************************/
static int signal_keyboard_set(KEYBOARD *p_keyboard, int key_num)
{
	int i;
	for(i=0;i<key_num;++i)
	{
		g_signal_connect(p_keyboard->button[i], "clicked",
				G_CALLBACK(p_keyboard->callback), (gpointer)p_keyboard);/*信号和回调函数连接*/
	}
	return i;
}

/****************************************************
函数功能:键盘按钮设置焦点
参数类型:自定义键盘结构体，取消
返回值:按钮个数
*****************************************************/
static int set_keyboard_button_focus(KEYBOARD *p_keyboard, gboolean focus, int key_num)
{
	int i;
	for(i=0;i<key_num;++i)
	{
		gtk_button_set_focus_on_click(GTK_BUTTON(p_keyboard->button[i]), focus);/*设置没有焦点*/
	}
	return i;
}

void set_keyboard_size(KEYBOARD *p_keyboard, const int width, const int height)
{
	gtk_widget_set_size_request(p_keyboard->table, width, height);
}

/****************************************************
函数功能:初始化键盘
参数类型:自定义键盘结构体
返回值:
*****************************************************/
void init_keyboard(KEYBOARD *p_keyboard)
{
	create_keyboard_window(p_keyboard);/*创建表格布局和按钮*/
	config_keyboard_window(p_keyboard);/*设置键盘大小和间距*/
	layout_keyboard_window(p_keyboard);/*按钮布局*/
	set_keyboard_button_focus(p_keyboard, FALSE, KEY_NUM);/*设置按钮没有焦点*/
	signal_keyboard_set(p_keyboard, KEY_NUM);/*按钮信号设定*/
}