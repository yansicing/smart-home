#ifndef _DEAL_BUTTON_H_
#define _DEAL_BUTTON_H_

#include "deal_chess.h"	

// 按钮事件与处理函数相连接
void connect_button();
// 人人对战按钮处理函数
gboolean deal_pvp_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data);
// 人机对战按钮处理函数
gboolean deal_pvm_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data);
// 悔棋按钮处理函数
gboolean deal_goback_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data);
// 重来按钮处理函数
gboolean deal_redo_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data);
// 退出按钮处理函数
gboolean deal_exit_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data);

#endif