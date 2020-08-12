#ifndef _DEAL_CHESS_H_
#define _DEAL_CHESS_H_
#include "main_win.h"


// Empty标志为空，Black标志为黑子，White标志为白子
enum GridState{EMPTY, BLACK, WHITE};

// 设置，主要得到窗口，画图区域
void set_control_chess(GtkWidget *);
// 棋盘的初始化
void init_chess(gboolean is_begin);
// 改变角色
void change_role();
// 根据路径得到图片
GdkPixmap *get_pixmap(const char *path, int w, int h);

// 窗口鼠标按下事件
gboolean win_button_pressed(GtkWidget *widget, GdkEventButton *event, gpointer data);
// 画图事件
gboolean draw_chess_event(GtkWidget* widget, GdkEventExpose* event, gpointer data);
// 倒计时处理函数
gboolean deal_count_time(gpointer data);
// 机器下棋的处理函数
gboolean machine_chess(gpointer data);

// 更新画图事件
void update();
// 启动倒计时
void start_count_time();
// 停止倒计时
void stop_count_time();
// 停止机器时间
void stop_machine_time();

void set_press_mode(gboolean flag);	// 设置是否按钮作战按钮
gboolean get_press_mode();			// 得到是否按钮作战按钮状态
void set_pvp_mode(gboolean flag);	// 设置作战模式
gboolean get_pvp_mode();			// 得到作战模式
void set_goback_step(int temp);		// 设置悔棋的步数
int get_chess_step();				// 得到当前是第几步棋

// 判断胜负
void judge_gameover();
// 判断黑子或白字能否吃子
gboolean judge_is_can_eat(enum GridState currentRole);
// 吃子规则
int judgeRule(int x, int y, int gridNum, void *chess, enum GridState currentRole,gboolean eatChess);








#define CHESS_RECV_FIFO "./fifo/chess_recv_fifo"

extern void *pthread_chess_recv_cmd(void *arg);


#endif
