#ifndef __GTK_KEYBOARD_H__
#define __GTK_KEYBOARD_H__

#define KEYBOARD_ROW 4				/*���̺���*/
#define KEYBOARD_COL 10				/*��������*/

#define KEYBOARD_USIZE_WIDE 300		/*���̿��*/
#define KEYBOARD_USIZE_HEIGHT 150	/*���̸߶�*/

#define KEY_ROW_SPACINGS 2			/*��ť�м��*/
#define KEY_COL_SPACINGS 2			/*��ť�м��*/

#define CLEAR "clear"
#define BACKSPACE "backspace"

#define KEY_NUM 39					/*��������*/
#define RECV_NUM 10					/*�����ߵĸ���*/

typedef void (*callback_t)(GtkWidget *widget, gpointer data);

typedef struct keyboard
{
	GtkWidget *table;				/*���̲���*/
	GtkWidget *button[KEY_NUM];		/*����*/
	callback_t callback;			/*�����ص�����*/
	GtkWidget *recv[RECV_NUM];		/*���̽�����*/
}KEYBOARD;

extern void set_keyboard_size(KEYBOARD *p_keyboard, const int width, const int height);

/*���̳�ʼ��*/
extern void init_keyboard(KEYBOARD *p_keyboard);

#endif

/*****************************EOF**********************************/
