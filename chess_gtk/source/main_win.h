#ifndef _MAIN_WIN_H_
#define _MAIN_WIN_H_

#include <gtk/gtk.h>
#include <string.h>

#define BGPIC "./image/chessboard.jpg"		// ����ͼƬ·��
#define W 800	// ���ڿ��
#define H 480	// ���ڸ߶�

enum TipState{TBLACK, TWHITE, TSTOP};// ��ʾ״̬��TBLACK:����ʾ��TWHITE:����ʾ��TSTOP:����ʾ
enum ButtonState{INIT, PVP, PVM};    // ��ť״̬

// ��ť�ؼ�
struct MyButton					
{
	GtkWidget *pvp_button;		// ����
	GtkWidget *pvm_button;		// �˻�
	GtkWidget *goback_button;	// ����
	GtkWidget *redo_button;		// ����
	GtkWidget *exit_button;		// �˳�
}button;

GtkWidget *create_main_window();	// ����������
GtkWidget *design_main_window();    // ���ô��ڣ���Ҫ��һЩ���ƿؼ�

// ������ͼƬ�İ�ť
GtkWidget *create_button(const char *path, int w, int h, int flag); 
// ����һ��ͼƬ�ؼ����൱��label��ʾͼƬ
GtkWidget *set_widget_with_image(const char *image_path, int w, int h);
// �ı�ؼ�����Ĵ�С
void set_widget_font_size(GtkWidget *widget, int size, gboolean is_button);
void set_tip_state(enum TipState flag);	        // ���ý�ɫ��ʾ״̬
void set_button_state(enum ButtonState flag);	// ��ťʹ��״̬����
void set_time_label(int temp);			        // ���õ���ʱ��ʾ
void set_num_label(int b_num, int w_num);	    // ���úڰ��������ʾ

gboolean deal_show_tip(gpointer data);			// ��ʱ��������

void show_win(GtkWidget *temp);			        // ��ʾ�����ڲ���
void show_num_label();					        // ��ʾ����ͳ�ƿؼ�
void hide_num_label();					        // ���ظ���ͳ�ƿؼ�
void show_time_label();							// ��ʾ����ʱ�ؼ�
void hide_time_label();							// ���ص���ʱ�ؼ�
void show_info_dialog(const gchar *temp);	    // ��Ϣ�Ի���

GtkWidget *get_mian_window();		// �õ�������
GtkWidget *get_draw_area();			// �õ���ͼ����
GtkWidget *get_time_label();		// �õ�����ʱ��ʾ�ؼ�


#endif