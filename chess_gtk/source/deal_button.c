#include "deal_button.h"	

// 按钮事件与处理函数相连接
void connect_button()
{
	g_signal_connect (G_OBJECT(button.pvp_button), "button_press_event",
                        G_CALLBACK(deal_pvp_button_press), NULL);
	g_signal_connect (G_OBJECT(button.pvm_button), "button_press_event",
                        G_CALLBACK(deal_pvm_button_press), NULL);
	g_signal_connect (G_OBJECT(button.goback_button), "button_press_event",
                        G_CALLBACK(deal_goback_button_press), NULL);
	g_signal_connect (G_OBJECT(button.redo_button), "button_press_event",
                        G_CALLBACK(deal_redo_button_press), NULL);
	g_signal_connect (G_OBJECT(button.exit_button), "button_press_event",
                        G_CALLBACK(deal_exit_button_press), NULL);
	set_button_state(INIT);	// 初始化按钮使能控制状态
}

// 人人对战按钮处理函数
gboolean deal_pvp_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	set_button_state(PVP);
	
	init_chess(TRUE);		// 设置按钮
	set_press_mode(TRUE);	// 按下作战按钮
	set_pvp_mode(TRUE);		// 人人对战模式
	set_tip_state(TBLACK);	// 黑子提示
	start_count_time();		// 倒计时开始
	show_time_label();		// 显示倒计时
	show_num_label();		// 显示双方个数统计控件
	
	return TRUE;
}

// 人机对战按钮处理函数
gboolean deal_pvm_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	set_button_state(PVM);	// 设置按钮
	init_chess(TRUE);		// 初始化棋盘
	set_press_mode(TRUE);	// 按下作战按钮
	set_pvp_mode(FALSE);	// 人机对战模式
	set_tip_state(TBLACK);	// 黑子提示
	start_count_time();		// 倒计时开始
	show_time_label();		// 显示倒计时
	show_num_label();		// 显示双方个数统计控件
	
	return TRUE;
}

// 悔棋按钮处理函数
gboolean deal_goback_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if( TRUE == get_pvp_mode() && 1 <= get_chess_step()){	// 如果是人人对战，并且已经下子了
        change_role(); // 改变棋子的角色
        set_goback_step(1);        // 退一步
    }else if( FALSE == get_pvp_mode() && 2 <= get_chess_step() ){ // 如果是人机对战
        set_goback_step(2);   
	}
	
	update();
	
	if(0 == get_chess_step()){	// 如果回到第一步
		gtk_widget_set_sensitive(button.goback_button, FALSE); // 悔棋按钮非使能
		init_chess(TRUE);	// 初始化棋盘
		show_time_label();  // 显示时间							
	}
	
	return TRUE;
}

// 重来按钮处理函数
gboolean deal_redo_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	set_button_state(INIT);	// 重新初始化按钮
	set_press_mode(FALSE);	// 没有按下作战模式
	init_chess(FALSE);		// 棋盘状态清空
	set_tip_state(TSTOP);	// 停止提示角色
	stop_count_time();		// 停止倒计时
	hide_time_label();		// 隐藏倒计时显示控件
	hide_num_label();		// 隐藏显示个数控件
	stop_machine_time();	// 停止机器时间
	
	return TRUE;
}

// 退出按钮处理函数
gboolean deal_exit_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	gtk_main_quit();	// 退出
	
	return TRUE;
}

