#include "main_win.h"

static GtkWidget *window;		  		 // ������
static GtkWidget * area;		  		 // ��ͼ����
static GtkWidget *black_tip, *white_tip; // �ڰ����ɫ��ʾ�ؼ� 
static GtkWidget *time_label;			 // ����ʱ��ʾ�ؼ�
static GtkWidget *black_num_label, *white_num_label;	// �ڰ��������ʾ�ؼ�
static int tip_timer_id;				// ��ʱ��id��Ϊ��ʵ����ʾһ��һ����Ч��
static enum TipState tip_role;		    // ��ʾ״̬

/*
 *   ���ܣ�����������
 * ����ֵ��������������
 */
GtkWidget *create_main_window()
{
	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);    // ����
	// ���⣬g_locale_to_utf8��ʹ��UTF-8�ı�������ʾ����
	gtk_window_set_title(GTK_WINDOW(win), 
				g_locale_to_utf8("�ڰ���-GTK��",-1,0,0,0)); 
	//�̶����ڵĴ�С, W, HΪ�궨��
	gtk_widget_set_usize(win, W, H);	  
	gtk_window_set_resizable(GTK_WINDOW(win), FALSE);	// ���ڲ�������

	//���ô�������ʾ���е�λ��Ϊ���У�GTK_WIN_POS_CENTER������
	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
    // ���������connect�����رհ�ť���ڲ��ܹر�
	g_signal_connect (G_OBJECT(win), "destroy",
                        G_CALLBACK(gtk_main_quit), NULL);
	
	//////////////////���ô��ڵı���ͼ
	// �ô�������Ϊ�ɻ�
    gtk_widget_set_app_paintable(win, TRUE);
    gtk_widget_realize(win);
	
    GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file (BGPIC, NULL);
    // w, h��ָ��ͼƬ�Ŀ�Ⱥ͸߶�
    GdkPixbuf *dst_pixbuf = gdk_pixbuf_scale_simple (src_pixbuf, 
					W, H, GDK_INTERP_BILINEAR);
	GdkPixmap *pixmap = NULL;
    gdk_pixbuf_render_pixmap_and_mask(dst_pixbuf, &pixmap, NULL, 256);
    gdk_window_set_back_pixmap(win->window, pixmap, FALSE);		// ���ñ���ͼ
    g_object_unref (src_pixbuf);
    g_object_unref (dst_pixbuf);
    g_object_unref (pixmap);
	
	///////////////////////����������������¼�
	gtk_widget_add_events(win, GDK_BUTTON_PRESS_MASK);
	
	return win;
}	

/*
 *	  ���ܣ����������ڣ���һЩ��Ҫ�Ŀؼ�
 *  ����ֵ��������
 */
GtkWidget *design_main_window()
{
	window = create_main_window();  // �ȴ�������
	
	////////////////////// ���񲼾�
	GtkWidget *table = gtk_table_new(11, 8, TRUE);
	gtk_container_set_border_width(GTK_CONTAINER(table), 0);
	gtk_container_add(GTK_CONTAINER(window), table);
	// ���ò��ֿؼ�֮��ļ��
	gtk_table_set_row_spacings(GTK_TABLE(table), 0);
	gtk_table_set_col_spacings(GTK_TABLE(table), 0);
	
	/////////////////////// ��ͼ����
    area = gtk_fixed_new();
	// ������Ҫע�⣬������β��֣���ͼ����һֱ���ǲ����������� 2,5,1,8
	gtk_table_attach_defaults(GTK_TABLE(table), area, 2, 5, 1, 8);
	
	/////////////////////��ɫ��ʾ����
	GtkWidget *tip_fixed = gtk_fixed_new(); 
	gtk_table_attach_defaults(GTK_TABLE(table), tip_fixed, 0, 8, 0, 1);
	
	black_tip = set_widget_with_image("./image/black.png", 50, 50);
	gtk_fixed_put(GTK_FIXED(tip_fixed), black_tip, 224, 5);
	
	white_tip = set_widget_with_image("./image/white.png", 50, 50);
	gtk_fixed_put(GTK_FIXED(tip_fixed), white_tip, 527, 5);
	// ��ʱ��������ʾ�ﵽһ��һ����Ч��
	set_tip_state(TSTOP);		// ��ʼʱ����ʾ
	// ���Ӷ�ʱ����ÿ��500����ִ��һ��deal_show_tip����
	tip_timer_id = g_timeout_add(500, (GSourceFunc)deal_show_tip, NULL);
	
	///////////////////////����ʱ��ʾ�ؼ�
	time_label = gtk_label_new(NULL);
	gtk_fixed_put(GTK_FIXED(tip_fixed), time_label, 360, 2);
	//gtk_misc_set_alignment(GTK_MISC(time_label), 0.5, 0.5);
	gtk_widget_set_usize(time_label, 75, 60);	 // �̶��ؼ���С
	set_widget_font_size(time_label, 40, FALSE); // ���ÿؼ������С
	// �ı�ؼ�������Ϊ��ɫ
	GdkColor color;
	gdk_color_parse("white",&color);
	gtk_widget_modify_fg(time_label, GTK_STATE_NORMAL, &color); 
	
	///////////////////////���Ӹ�����ʾ�ؼ�
	GtkWidget *num_fixed = gtk_fixed_new(); 
	gtk_table_attach_defaults(GTK_TABLE(table), num_fixed, 0, 2, 1, 3);
	black_num_label = gtk_label_new(NULL);
	gtk_fixed_put(GTK_FIXED(num_fixed), black_num_label, 70, 10);
	gtk_misc_set_alignment(GTK_MISC(black_num_label), 0.5, 0.5);
	gtk_widget_set_usize(black_num_label, 100, 80);	 // �̶��ؼ���С
	gtk_widget_modify_fg(black_num_label, GTK_STATE_NORMAL, &color); 
	set_widget_font_size(black_num_label, 50, FALSE); // ���ÿؼ������С
	
	///////////////////////���Ӹ�����ʾ�ؼ�
	num_fixed = gtk_fixed_new(); 
	gtk_table_attach_defaults(GTK_TABLE(table), num_fixed, 6, 8, 1, 3);
	
	white_num_label = gtk_label_new(NULL);
	gtk_fixed_put(GTK_FIXED(num_fixed), white_num_label, 40, 10);
	gtk_widget_set_usize(white_num_label, 100, 80);	 // �̶��ؼ���С
	gtk_misc_set_alignment(GTK_MISC(white_num_label), 0.5, 0.5);
	gtk_widget_modify_fg(white_num_label, GTK_STATE_NORMAL, &color); 
	set_widget_font_size(white_num_label, 50, FALSE); // ���ÿؼ������С
	
	//////////////////////���Ӱ�ť
	// ������ť
	GtkWidget *fixed = gtk_fixed_new(); 
	gtk_container_set_border_width(GTK_CONTAINER(fixed), 0); 
	gtk_table_attach_defaults(GTK_TABLE(table), fixed, 0, 8, 9, 11);
	
	// ˫��
	button.pvp_button = create_button("./image/pvp.png", 90, 70, TRUE);
	gtk_fixed_put (GTK_FIXED (fixed), button.pvp_button, 120, 10);
	
	// ����
	button.pvm_button = create_button("./image/pvm.png", 90, 70, TRUE);
	gtk_fixed_put (GTK_FIXED (fixed), button.pvm_button, 240, 10);
	
	// ����
	button.goback_button = create_button("./image/goback.png", 90, 70, TRUE);
	gtk_fixed_put (GTK_FIXED (fixed), button.goback_button, 360, 10);
	
	// ����
	button.redo_button = create_button("./image/redo.png", 90, 70, TRUE);
	gtk_fixed_put (GTK_FIXED (fixed), button.redo_button, 480, 10);
	
	// �˳�
	button.exit_button = create_button("./image/exit.png", 90, 70, TRUE);
	gtk_fixed_put (GTK_FIXED (fixed), button.exit_button, 590, 10);
	
	return window;
}

/*
 *   ����: ������ť
 *   path: ��ťͼƬ��·��
 *   w, h: ͼƬ�Ŀ�Ⱥ͸߶�
 *   flag: TRUE����ť���º���н���
 * ����ֵ����ť
 */
GtkWidget *create_button(const char *path, int w, int h, int flag)
{
	GtkWidget *image = set_widget_with_image(path, w, h);
	
	GtkWidget *button = gtk_button_new();
	gtk_container_add(GTK_CONTAINER(button), image);
	gtk_widget_set_usize(button, w+6, h+6);
	gtk_button_set_relief(GTK_BUTTON(button),GTK_RELIEF_NONE);
	gtk_button_set_focus_on_click(GTK_BUTTON(button), flag);
	
	return button;
}

/* 
 *       ����: ������ͼƬ�Ŀؼ����൱��label
 * image_path: ͼƬ·��
 *          w: ָ��ͼƬ�Ŀ��
 *          h: ָ��ͼƬ�ĸ߶�
 *     ����ֵ��ͼƬ�ؼ�
 */
GtkWidget *set_widget_with_image(const char *image_path, int w, int h)
{
	GtkWidget *image = NULL;
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file(image_path, NULL);
    GdkPixbuf *dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h, GDK_INTERP_BILINEAR);
	image = gtk_image_new_from_pixbuf(dst_pixbuf);
	
	g_object_unref (src_pixbuf);
    g_object_unref (dst_pixbuf);
	
	if(NULL == image){
		return NULL;
	}else{
		return image;
	}
}

/*
 *      ����: ���ÿؼ������С
 *    widget: ��Ҫ�ı�����Ŀؼ�
 *      size: �����С
 * is_button: TRUE����ؼ�Ϊ��ť��FALSEΪ�����ؼ�
 */
void set_widget_font_size(GtkWidget *widget, int size, gboolean is_button)
{
	GtkWidget *labelChild;  
	PangoFontDescription *font;  
	gint fontSize = size;  
  
	font = pango_font_description_from_string("Sans");          //"Sans"������   
	pango_font_description_set_size(font, fontSize*PANGO_SCALE);//���������С   
	
	if(is_button){
		labelChild = gtk_bin_get_child(GTK_BIN(widget));//ȡ��GtkButton���label  
	}else{
		labelChild = widget;
	}
	
	//����label�����壬�������GtkButton������ʾ������ͱ���
	gtk_widget_modify_font(GTK_WIDGET(labelChild), font);
	pango_font_description_free(font);
}

/*
 * ���ܣ����ý�ɫ��ʾ״̬
 * flag��TBLACK:������ʾ��TWHITE:������ʾ��TSTOP:����ʾ
 */ 
void set_tip_state(enum TipState flag)
{
	tip_role = flag;
}

/*
 * ���ܣ���ťʹ��״̬����
 * flag��INIT����ʼ����PVP�����ˣ�PVM���˻�
 */
void set_button_state(enum ButtonState flag)
{
	if(INIT == flag){
		gtk_widget_set_sensitive(button.pvp_button, TRUE);
		gtk_widget_set_sensitive(button.pvm_button, TRUE);
		gtk_widget_set_sensitive(button.goback_button, FALSE);
		gtk_widget_set_sensitive(button.redo_button, FALSE);
	}else if(PVP == flag || PVM == flag){
		gtk_widget_set_sensitive(button.pvp_button, FALSE);
		gtk_widget_set_sensitive(button.pvm_button, FALSE);
	}
}

/*
 *   ���ܣ�������ʾ��ʱ��
 *   temp����Ҫ��ʾ��ʱ��
 */
void set_time_label(int temp)
{
	char buf[10];
	sprintf(buf, "%d", temp);
	gtk_label_set_text(GTK_LABEL(time_label), buf);
}

/*
 *   ���ܣ����úڰ�����ʾ�ĸ���
 *  b_num�����Ӹ���
 *  w_num�����ָ���
 */
void set_num_label(int b_num, int w_num)
{
	char buf[10];
	sprintf(buf, "%d", b_num);
	gtk_label_set_text(GTK_LABEL(black_num_label), buf);
	sprintf(buf, "%d", w_num);
	gtk_label_set_text(GTK_LABEL(white_num_label), buf);
}

/*
 * 	���ܣ���ʱ������������ɫ��ʾ���ﵽһ��һ����Ч��
 *  data������û�õ�
 */
gboolean deal_show_tip(gpointer data)
{
	static int is_black_show = FALSE, is_white_show = FALSE;
	if(TBLACK == tip_role){				// ���ӽ�ɫ��ʾ
		if(FALSE == is_black_show){
			gtk_widget_show(black_tip);
			is_black_show = TRUE;
		}else{
			gtk_widget_hide(black_tip);
			is_black_show = FALSE;
		}
		
		gtk_widget_show(white_tip);
		is_white_show = FALSE;
		
	}else if(TWHITE == tip_role){		// ���ӽ�ɫ��ʾ
		if(FALSE == is_white_show){
			gtk_widget_show(white_tip);
			is_white_show = TRUE;
		}else{
			gtk_widget_hide(white_tip);
			is_white_show = FALSE;
		}
		
		gtk_widget_show(black_tip);
		is_black_show = TRUE;
		
	}else if(TSTOP == tip_role){		// ����ʾ��ʾ��ɫ
		gtk_widget_show(black_tip);
		gtk_widget_show(white_tip);
		is_black_show = FALSE, is_white_show = FALSE;
	}
	
	return TRUE;
}

// ��ʾ���������в���
void show_win(GtkWidget *temp)
{
	gtk_widget_show_all(temp);
}

// ��ʾ�ڰ��Ӹ���ͳ�ƿؼ�
void show_num_label()
{
	gtk_widget_show(black_num_label);
	gtk_widget_show(white_num_label);
}

// ���غڰ��Ӹ���ͳ�ƿؼ�
void hide_num_label()
{
	gtk_widget_hide(black_num_label);
	gtk_widget_hide(white_num_label);
}

// ��ʾ����ʱ�ؼ�
void show_time_label()
{
	gtk_widget_show(time_label);
}

// ���ص���ʱ�ؼ�
void hide_time_label()
{
	gtk_widget_hide(time_label);
}

/*
 * 	���ܣ���Ϣ�Ի���
 *  temp����Ҫ��ʾ����Ϣ
 */
void show_info_dialog(const gchar *temp)
{
	GtkWidget* dialog;
    dialog = gtk_message_dialog_new(
            GTK_WINDOW(window),
            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            g_locale_to_utf8(temp,-1,0,0,0));// g_locale_to_utf8:ʹ��UTF-8�ı�������ʾ����
	gtk_window_set_title(GTK_WINDOW(dialog),  g_locale_to_utf8("��ʾ",-1,0,0,0));
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// �õ�������
GtkWidget *get_mian_window()
{
	return window;
}

// ���ػ�ͼ������
GtkWidget *get_draw_area()
{
	return area;
}

// �õ�����ʱ��ʾ�ؼ�
GtkWidget *get_time_label()
{
	return time_label;
}


