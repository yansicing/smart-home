#ifndef _DEAL_BUTTON_H_
#define _DEAL_BUTTON_H_

#include "deal_chess.h"	

// ��ť�¼��봦����������
void connect_button();
// ���˶�ս��ť������
gboolean deal_pvp_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data);
// �˻���ս��ť������
gboolean deal_pvm_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data);
// ���尴ť������
gboolean deal_goback_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data);
// ������ť������
gboolean deal_redo_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data);
// �˳���ť������
gboolean deal_exit_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data);

#endif