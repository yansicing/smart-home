#ifndef _DEAL_CHESS_H_
#define _DEAL_CHESS_H_
#include "main_win.h"


// Empty��־Ϊ�գ�Black��־Ϊ���ӣ�White��־Ϊ����
enum GridState{EMPTY, BLACK, WHITE};

// ���ã���Ҫ�õ����ڣ���ͼ����
void set_control_chess(GtkWidget *);
// ���̵ĳ�ʼ��
void init_chess(gboolean is_begin);
// �ı��ɫ
void change_role();
// ����·���õ�ͼƬ
GdkPixmap *get_pixmap(const char *path, int w, int h);

// ������갴���¼�
gboolean win_button_pressed(GtkWidget *widget, GdkEventButton *event, gpointer data);
// ��ͼ�¼�
gboolean draw_chess_event(GtkWidget* widget, GdkEventExpose* event, gpointer data);
// ����ʱ������
gboolean deal_count_time(gpointer data);
// ��������Ĵ�����
gboolean machine_chess(gpointer data);

// ���»�ͼ�¼�
void update();
// ��������ʱ
void start_count_time();
// ֹͣ����ʱ
void stop_count_time();
// ֹͣ����ʱ��
void stop_machine_time();

void set_press_mode(gboolean flag);	// �����Ƿ�ť��ս��ť
gboolean get_press_mode();			// �õ��Ƿ�ť��ս��ť״̬
void set_pvp_mode(gboolean flag);	// ������սģʽ
gboolean get_pvp_mode();			// �õ���սģʽ
void set_goback_step(int temp);		// ���û���Ĳ���
int get_chess_step();				// �õ���ǰ�ǵڼ�����

// �ж�ʤ��
void judge_gameover();
// �жϺ��ӻ�����ܷ����
gboolean judge_is_can_eat(enum GridState currentRole);
// ���ӹ���
int judgeRule(int x, int y, int gridNum, void *chess, enum GridState currentRole,gboolean eatChess);








#define CHESS_RECV_FIFO "./fifo/chess_recv_fifo"

extern void *pthread_chess_recv_cmd(void *arg);


#endif
