#ifndef _MAIN_WIN_H_
#define _MAIN_WIN_H_

#include <gtk/gtk.h>
#include <string.h>

#define BGPIC "./image/chessboard.jpg"		// 背景图片路径
#define W 800	// 窗口宽度
#define H 480	// 窗口高度

enum TipState{TBLACK, TWHITE, TSTOP};// 提示状态，TBLACK:黑提示，TWHITE:白提示，TSTOP:不提示
enum ButtonState{INIT, PVP, PVM};    // 按钮状态

// 按钮控件
struct MyButton					
{
	GtkWidget *pvp_button;		// 人人
	GtkWidget *pvm_button;		// 人机
	GtkWidget *goback_button;	// 悔棋
	GtkWidget *redo_button;		// 重来
	GtkWidget *exit_button;		// 退出
}button;

GtkWidget *create_main_window();	// 创建主窗口
GtkWidget *design_main_window();    // 设置窗口，主要加一些控制控件

// 创建带图片的按钮
GtkWidget *create_button(const char *path, int w, int h, int flag); 
// 设置一张图片控件，相当于label显示图片
GtkWidget *set_widget_with_image(const char *image_path, int w, int h);
// 改变控件字体的大小
void set_widget_font_size(GtkWidget *widget, int size, gboolean is_button);
void set_tip_state(enum TipState flag);	        // 设置角色提示状态
void set_button_state(enum ButtonState flag);	// 按钮使能状态设置
void set_time_label(int temp);			        // 设置倒计时显示
void set_num_label(int b_num, int w_num);	    // 设置黑白棋个数显示

gboolean deal_show_tip(gpointer data);			// 定时器处理函数

void show_win(GtkWidget *temp);			        // 显示主窗口部件
void show_num_label();					        // 显示个数统计控件
void hide_num_label();					        // 隐藏个数统计控件
void show_time_label();							// 显示倒计时控件
void hide_time_label();							// 隐藏倒计时控件
void show_info_dialog(const gchar *temp);	    // 信息对话框

GtkWidget *get_mian_window();		// 得到主窗口
GtkWidget *get_draw_area();			// 得到画图区域
GtkWidget *get_time_label();		// 得到倒计时显示控件


#endif