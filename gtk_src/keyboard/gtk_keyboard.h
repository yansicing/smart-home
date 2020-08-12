#ifndef __GTK_KEYBOARD_H__
#define __GTK_KEYBOARD_H__

#define KEYBOARD_ROW 4				/*键盘函数*/
#define KEYBOARD_COL 10				/*键盘列数*/

#define KEYBOARD_USIZE_WIDE 300		/*键盘宽度*/
#define KEYBOARD_USIZE_HEIGHT 150	/*键盘高度*/

#define KEY_ROW_SPACINGS 2			/*按钮行间距*/
#define KEY_COL_SPACINGS 2			/*按钮列间距*/

#define CLEAR "clear"
#define BACKSPACE "backspace"

#define KEY_NUM 39					/*按键个数*/
#define RECV_NUM 10					/*接收者的个数*/

typedef void (*callback_t)(GtkWidget *widget, gpointer data);

typedef struct keyboard
{
	GtkWidget *table;				/*键盘布局*/
	GtkWidget *button[KEY_NUM];		/*按键*/
	callback_t callback;			/*按键回调函数*/
	GtkWidget *recv[RECV_NUM];		/*键盘接收者*/
}KEYBOARD;

extern void set_keyboard_size(KEYBOARD *p_keyboard, const int width, const int height);

/*键盘初始化*/
extern void init_keyboard(KEYBOARD *p_keyboard);

#endif

/*****************************EOF**********************************/
