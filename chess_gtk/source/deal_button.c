#include "deal_button.h"	

// ��ť�¼��봦����������
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
	set_button_state(INIT);	// ��ʼ����ťʹ�ܿ���״̬
}

// ���˶�ս��ť������
gboolean deal_pvp_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	set_button_state(PVP);
	
	init_chess(TRUE);		// ���ð�ť
	set_press_mode(TRUE);	// ������ս��ť
	set_pvp_mode(TRUE);		// ���˶�սģʽ
	set_tip_state(TBLACK);	// ������ʾ
	start_count_time();		// ����ʱ��ʼ
	show_time_label();		// ��ʾ����ʱ
	show_num_label();		// ��ʾ˫������ͳ�ƿؼ�
	
	return TRUE;
}

// �˻���ս��ť������
gboolean deal_pvm_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	set_button_state(PVM);	// ���ð�ť
	init_chess(TRUE);		// ��ʼ������
	set_press_mode(TRUE);	// ������ս��ť
	set_pvp_mode(FALSE);	// �˻���սģʽ
	set_tip_state(TBLACK);	// ������ʾ
	start_count_time();		// ����ʱ��ʼ
	show_time_label();		// ��ʾ����ʱ
	show_num_label();		// ��ʾ˫������ͳ�ƿؼ�
	
	return TRUE;
}

// ���尴ť������
gboolean deal_goback_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    if( TRUE == get_pvp_mode() && 1 <= get_chess_step()){	// ��������˶�ս�������Ѿ�������
        change_role(); // �ı����ӵĽ�ɫ
        set_goback_step(1);        // ��һ��
    }else if( FALSE == get_pvp_mode() && 2 <= get_chess_step() ){ // ������˻���ս
        set_goback_step(2);   
	}
	
	update();
	
	if(0 == get_chess_step()){	// ����ص���һ��
		gtk_widget_set_sensitive(button.goback_button, FALSE); // ���尴ť��ʹ��
		init_chess(TRUE);	// ��ʼ������
		show_time_label();  // ��ʾʱ��							
	}
	
	return TRUE;
}

// ������ť������
gboolean deal_redo_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	set_button_state(INIT);	// ���³�ʼ����ť
	set_press_mode(FALSE);	// û�а�����սģʽ
	init_chess(FALSE);		// ����״̬���
	set_tip_state(TSTOP);	// ֹͣ��ʾ��ɫ
	stop_count_time();		// ֹͣ����ʱ
	hide_time_label();		// ���ص���ʱ��ʾ�ؼ�
	hide_num_label();		// ������ʾ�����ؼ�
	stop_machine_time();	// ֹͣ����ʱ��
	
	return TRUE;
}

// �˳���ť������
gboolean deal_exit_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	gtk_main_quit();	// �˳�
	
	return TRUE;
}

