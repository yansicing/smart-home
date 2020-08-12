#include <gtk/gtk.h>
#include"sungtk_interface.h"


gboolean callback_press( GtkWidget *widget,GdkEventButton *event,gpointer data )
{
	printf("abcdef\n");
	return TRUE;
}

gboolean callback_motion( GtkWidget *widget,GdkEventMotion *event,gpointer data )
{
	printf("11111111111\n");
	static double old_y = 0;
	double new_y = event->y;
	if(new_y - old_y > 5){
		old_y = new_y;
		sungtk_scrolled_window_set_vadjustment_value(GTK_WIDGET(data),5);
	}else if(old_y - new_y > 5){
		old_y = new_y;
		sungtk_scrolled_window_set_vadjustment_value(GTK_WIDGET(data),-5);
	}
	return TRUE;
}
int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title((GtkWindow *)window, "Hello");
	gtk_widget_set_size_request(window, 400,300);
	

	GtkWidget *table = gtk_table_new(5, 5, TRUE);
	gtk_container_add(GTK_CONTAINER(window), table);
	//��������
	GtkWidget *s_window = sungtk_scrolled_window_new();
	gtk_table_attach_defaults(GTK_TABLE(table), s_window, 2, 5, 0, 5);
	GtkWidget *label = gtk_label_new("ssssssssss\nasdfas\ndfjka\nsjd\nlkh\nnkas\ndnfk\nsahdfk\nasdfkash\ndkfh\nksdhfe\nwiflkd\nsdf\nafa;skfa\nksfas\n54f\n6a4s\nfas\nodj\nfa\ns\nkje9\nrajskfdj\nsf.zms\ndifue\nfzkdi\nfp;awm\nfsl\nd\njfaiejf\nalsjdei'\nakfa\nsuo\ngn\nad\nfwi\noljf\nlaj\nf9ow");
	GtkWidget *event = gtk_event_box_new();//�¼�����
	gtk_container_add(GTK_CONTAINER(event), label);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(s_window), event);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(s_window), GTK_POLICY_NEVER, GTK_POLICY_NEVER);//���ù�����������
	//���������ƶ��¼���Ч������¼���Ч������������¼�����
	//���Ƶ�clist֧���ƶ��¼�������Ҳ��������clist���ƶ��¼�
	gtk_widget_add_events(s_window, GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_MOTION_MASK);
	g_signal_connect(s_window, "button-press-event", G_CALLBACK(callback_press), NULL);
	g_signal_connect(event, "motion-notify-event", G_CALLBACK(callback_motion), s_window);


	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}


