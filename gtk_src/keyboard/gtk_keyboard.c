#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "gtk_keyboard.h"
#include"../sungtk_interface/sungtk_interface.h"

/****************************************************
��������:�������̰�ť
��������:�Զ�����̽ṹ��
����ֵ:��ť����
*****************************************************/
static int create_keyboard_window(KEYBOARD *p_keyboard)
{
	p_keyboard->table = gtk_table_new(KEYBOARD_ROW, KEYBOARD_COL, TRUE);/*�������̱��*/
	
	int i;
	char text[50] = "";
	for(i=0;i<10;++i)
	{
		memset(text, 0, sizeof(text));
		snprintf(text, sizeof(text), "%d", i);
		//snprintf(text, sizeof(text), "../picture/keyboard/%d.jpg", i);
		//printf("text----%s--\n", text);
		p_keyboard->button[i] = gtk_button_new_with_label(text);/*����0~9��ť������*/
		//p_keyboard->button[i] = sungtk_button_new_with_image(text, 40, 40);
	}
	 
	char j;
	for(j='a';j<='z';++j)
	{
		memset(text, 0, sizeof(text));
		snprintf(text, sizeof(text), "%c", j);
		//snprintf(text, sizeof(text), "../picture/keyboard/%c.jpg", j);
		p_keyboard->button[i++] = gtk_button_new_with_label(text);/*����a~z��ť������*/
		//p_keyboard->button[i++] = sungtk_button_new_with_image(text, 40, 40);
	}
	
	p_keyboard->button[i++] = gtk_button_new_with_label(".");/*���õ�'.'��ť������*/
	p_keyboard->button[i++] = gtk_button_new_with_label("clear");/*������հ�ť������*/
	p_keyboard->button[i++] = gtk_button_new_with_label("backspace");/*����ɾ��һ���ַ���ť������*/
	//p_keyboard->button[i++] = sungtk_button_new_with_image("../picture/keyboard/point.jpg", 40, 40);
	//p_keyboard->button[i++] = sungtk_button_new_with_image("../picture/keyboard/clear.jpg", 40, 40);
	//p_keyboard->button[i++] = sungtk_button_new_with_image("../picture/keyboard/backspace.jpg", 80, 40);
	return i;
}

/****************************************************
��������:���ü��̴�С�ͼ��
��������:�Զ�����̽ṹ��
����ֵ:
*****************************************************/
static void config_keyboard_window(KEYBOARD *p_keyboard)
{
	gtk_widget_set_usize(p_keyboard->table, KEYBOARD_USIZE_WIDE, KEYBOARD_USIZE_HEIGHT );/*���ü��̴�С*/
	gtk_table_set_row_spacings(GTK_TABLE(p_keyboard->table), KEY_ROW_SPACINGS);/*�����м��*/
	gtk_table_set_col_spacings(GTK_TABLE(p_keyboard->table), KEY_COL_SPACINGS);/*�����м��*/
}

/****************************************************
��������:���̰�ť����
��������:�Զ�����̽ṹ��
����ֵ:��ť����
*****************************************************/
static void layout_keyboard_window(KEYBOARD *p_keyboard)
{
	int i;
	for(i=1;i<=9;++i)
	{
		gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[i], i-1, i, 0, 1);/*���1~9*/
	}
	--i;
	gtk_table_attach_defaults(GTK_TABLE(p_keyboard->table), p_keyboard->button[0], i, i+1, 0, 1);/*��0��ӽ�ȥ*/
	
	/*��ӵڶ������񲼾�*/
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

	/*��ӵ��������񲼾�*/
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

	/*��ӵ��������񲼾�*/
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
��������:���̰�ť�ź��趨
��������:�Զ�����̽ṹ�壬�ص�����
����ֵ:��ť����
*****************************************************/
static int signal_keyboard_set(KEYBOARD *p_keyboard, int key_num)
{
	int i;
	for(i=0;i<key_num;++i)
	{
		g_signal_connect(p_keyboard->button[i], "clicked",
				G_CALLBACK(p_keyboard->callback), (gpointer)p_keyboard);/*�źźͻص���������*/
	}
	return i;
}

/****************************************************
��������:���̰�ť���ý���
��������:�Զ�����̽ṹ�壬ȡ��
����ֵ:��ť����
*****************************************************/
static int set_keyboard_button_focus(KEYBOARD *p_keyboard, gboolean focus, int key_num)
{
	int i;
	for(i=0;i<key_num;++i)
	{
		gtk_button_set_focus_on_click(GTK_BUTTON(p_keyboard->button[i]), focus);/*����û�н���*/
	}
	return i;
}

void set_keyboard_size(KEYBOARD *p_keyboard, const int width, const int height)
{
	gtk_widget_set_size_request(p_keyboard->table, width, height);
}

/****************************************************
��������:��ʼ������
��������:�Զ�����̽ṹ��
����ֵ:
*****************************************************/
void init_keyboard(KEYBOARD *p_keyboard)
{
	create_keyboard_window(p_keyboard);/*������񲼾ֺͰ�ť*/
	config_keyboard_window(p_keyboard);/*���ü��̴�С�ͼ��*/
	layout_keyboard_window(p_keyboard);/*��ť����*/
	set_keyboard_button_focus(p_keyboard, FALSE, KEY_NUM);/*���ð�ťû�н���*/
	signal_keyboard_set(p_keyboard, KEY_NUM);/*��ť�ź��趨*/
}