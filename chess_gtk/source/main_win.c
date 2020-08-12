#include "main_win.h"

static GtkWidget *window;		  		 // 主窗口
static GtkWidget * area;		  		 // 画图区域
static GtkWidget *black_tip, *white_tip; // 黑白棋角色提示控件 
static GtkWidget *time_label;			 // 倒计时显示控件
static GtkWidget *black_num_label, *white_num_label;	// 黑白棋个数显示控件
static int tip_timer_id;				// 定时器id，为了实现提示一闪一闪的效果
static enum TipState tip_role;		    // 提示状态

/*
 *   功能：创建主窗口
 * 返回值：创建的主窗口
 */
GtkWidget *create_main_window()
{
	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);    // 创建
	// 标题，g_locale_to_utf8：使用UTF-8的编码来显示中文
	gtk_window_set_title(GTK_WINDOW(win), 
				g_locale_to_utf8("黑白棋-GTK版",-1,0,0,0)); 
	//固定窗口的大小, W, H为宏定义
	gtk_widget_set_usize(win, W, H);	  
	gtk_window_set_resizable(GTK_WINDOW(win), FALSE);	// 窗口不可伸缩

	//设置窗口在显示器中的位置为居中，GTK_WIN_POS_CENTER：中心
	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
    // 有了下面的connect，按关闭按钮窗口才能关闭
	g_signal_connect (G_OBJECT(win), "destroy",
                        G_CALLBACK(gtk_main_quit), NULL);
	
	//////////////////设置窗口的背景图
	// 让窗口设置为可画
    gtk_widget_set_app_paintable(win, TRUE);
    gtk_widget_realize(win);
	
    GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file (BGPIC, NULL);
    // w, h是指定图片的宽度和高度
    GdkPixbuf *dst_pixbuf = gdk_pixbuf_scale_simple (src_pixbuf, 
					W, H, GDK_INTERP_BILINEAR);
	GdkPixmap *pixmap = NULL;
    gdk_pixbuf_render_pixmap_and_mask(dst_pixbuf, &pixmap, NULL, 256);
    gdk_window_set_back_pixmap(win->window, pixmap, FALSE);		// 设置背景图
    g_object_unref (src_pixbuf);
    g_object_unref (dst_pixbuf);
    g_object_unref (pixmap);
	
	///////////////////////给窗口增加鼠标点击事件
	gtk_widget_add_events(win, GDK_BUTTON_PRESS_MASK);
	
	return win;
}	

/*
 *	  功能：设置主窗口，加一些必要的控件
 *  返回值：主窗口
 */
GtkWidget *design_main_window()
{
	window = create_main_window();  // 先创建窗口
	
	////////////////////// 网格布局
	GtkWidget *table = gtk_table_new(11, 8, TRUE);
	gtk_container_set_border_width(GTK_CONTAINER(table), 0);
	gtk_container_add(GTK_CONTAINER(window), table);
	// 设置布局控件之间的间隔
	gtk_table_set_row_spacings(GTK_TABLE(table), 0);
	gtk_table_set_col_spacings(GTK_TABLE(table), 0);
	
	/////////////////////// 画图区域
    area = gtk_fixed_new();
	// 这里需要注意，无论如何布局，画图区域一直都是布满整个窗口 2,5,1,8
	gtk_table_attach_defaults(GTK_TABLE(table), area, 2, 5, 1, 8);
	
	/////////////////////角色提示部件
	GtkWidget *tip_fixed = gtk_fixed_new(); 
	gtk_table_attach_defaults(GTK_TABLE(table), tip_fixed, 0, 8, 0, 1);
	
	black_tip = set_widget_with_image("./image/black.png", 50, 50);
	gtk_fixed_put(GTK_FIXED(tip_fixed), black_tip, 224, 5);
	
	white_tip = set_widget_with_image("./image/white.png", 50, 50);
	gtk_fixed_put(GTK_FIXED(tip_fixed), white_tip, 527, 5);
	// 定时器，让显示达到一闪一闪的效果
	set_tip_state(TSTOP);		// 开始时不提示
	// 增加定时器，每个500毫秒执行一次deal_show_tip函数
	tip_timer_id = g_timeout_add(500, (GSourceFunc)deal_show_tip, NULL);
	
	///////////////////////倒计时显示控件
	time_label = gtk_label_new(NULL);
	gtk_fixed_put(GTK_FIXED(tip_fixed), time_label, 360, 2);
	//gtk_misc_set_alignment(GTK_MISC(time_label), 0.5, 0.5);
	gtk_widget_set_usize(time_label, 75, 60);	 // 固定控件大小
	set_widget_font_size(time_label, 40, FALSE); // 设置控件字体大小
	// 改变控件的字体为白色
	GdkColor color;
	gdk_color_parse("white",&color);
	gtk_widget_modify_fg(time_label, GTK_STATE_NORMAL, &color); 
	
	///////////////////////黑子个数显示控件
	GtkWidget *num_fixed = gtk_fixed_new(); 
	gtk_table_attach_defaults(GTK_TABLE(table), num_fixed, 0, 2, 1, 3);
	black_num_label = gtk_label_new(NULL);
	gtk_fixed_put(GTK_FIXED(num_fixed), black_num_label, 70, 10);
	gtk_misc_set_alignment(GTK_MISC(black_num_label), 0.5, 0.5);
	gtk_widget_set_usize(black_num_label, 100, 80);	 // 固定控件大小
	gtk_widget_modify_fg(black_num_label, GTK_STATE_NORMAL, &color); 
	set_widget_font_size(black_num_label, 50, FALSE); // 设置控件字体大小
	
	///////////////////////白子个数显示控件
	num_fixed = gtk_fixed_new(); 
	gtk_table_attach_defaults(GTK_TABLE(table), num_fixed, 6, 8, 1, 3);
	
	white_num_label = gtk_label_new(NULL);
	gtk_fixed_put(GTK_FIXED(num_fixed), white_num_label, 40, 10);
	gtk_widget_set_usize(white_num_label, 100, 80);	 // 固定控件大小
	gtk_misc_set_alignment(GTK_MISC(white_num_label), 0.5, 0.5);
	gtk_widget_modify_fg(white_num_label, GTK_STATE_NORMAL, &color); 
	set_widget_font_size(white_num_label, 50, FALSE); // 设置控件字体大小
	
	//////////////////////增加按钮
	// 操作按钮
	GtkWidget *fixed = gtk_fixed_new(); 
	gtk_container_set_border_width(GTK_CONTAINER(fixed), 0); 
	gtk_table_attach_defaults(GTK_TABLE(table), fixed, 0, 8, 9, 11);
	
	// 双人
	button.pvp_button = create_button("./image/pvp.png", 90, 70, TRUE);
	gtk_fixed_put (GTK_FIXED (fixed), button.pvp_button, 120, 10);
	
	// 单人
	button.pvm_button = create_button("./image/pvm.png", 90, 70, TRUE);
	gtk_fixed_put (GTK_FIXED (fixed), button.pvm_button, 240, 10);
	
	// 悔棋
	button.goback_button = create_button("./image/goback.png", 90, 70, TRUE);
	gtk_fixed_put (GTK_FIXED (fixed), button.goback_button, 360, 10);
	
	// 重来
	button.redo_button = create_button("./image/redo.png", 90, 70, TRUE);
	gtk_fixed_put (GTK_FIXED (fixed), button.redo_button, 480, 10);
	
	// 退出
	button.exit_button = create_button("./image/exit.png", 90, 70, TRUE);
	gtk_fixed_put (GTK_FIXED (fixed), button.exit_button, 590, 10);
	
	return window;
}

/*
 *   功能: 创建按钮
 *   path: 按钮图片的路径
 *   w, h: 图片的宽度和高度
 *   flag: TRUE代表按钮按下后会有焦点
 * 返回值：按钮
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
 *       功能: 创建带图片的控件，相当于label
 * image_path: 图片路径
 *          w: 指定图片的宽度
 *          h: 指定图片的高度
 *     返回值：图片控件
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
 *      功能: 设置控件字体大小
 *    widget: 需要改变字体的控件
 *      size: 字体大小
 * is_button: TRUE代表控件为按钮，FALSE为其它控件
 */
void set_widget_font_size(GtkWidget *widget, int size, gboolean is_button)
{
	GtkWidget *labelChild;  
	PangoFontDescription *font;  
	gint fontSize = size;  
  
	font = pango_font_description_from_string("Sans");          //"Sans"字体名   
	pango_font_description_set_size(font, fontSize*PANGO_SCALE);//设置字体大小   
	
	if(is_button){
		labelChild = gtk_bin_get_child(GTK_BIN(widget));//取出GtkButton里的label  
	}else{
		labelChild = widget;
	}
	
	//设置label的字体，这样这个GtkButton上面显示的字体就变了
	gtk_widget_modify_font(GTK_WIDGET(labelChild), font);
	pango_font_description_free(font);
}

/*
 * 功能：设置角色提示状态
 * flag：TBLACK:黑子提示，TWHITE:白子提示，TSTOP:不提示
 */ 
void set_tip_state(enum TipState flag)
{
	tip_role = flag;
}

/*
 * 功能：按钮使能状态设置
 * flag：INIT：初始化；PVP：人人；PVM：人机
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
 *   功能：设置显示的时间
 *   temp：需要显示的时间
 */
void set_time_label(int temp)
{
	char buf[10];
	sprintf(buf, "%d", temp);
	gtk_label_set_text(GTK_LABEL(time_label), buf);
}

/*
 *   功能：设置黑白子显示的个数
 *  b_num：黑子个数
 *  w_num：白字个数
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
 * 	功能：定时器处理函数，角色提示，达到一闪一闪的效果
 *  data：这里没用到
 */
gboolean deal_show_tip(gpointer data)
{
	static int is_black_show = FALSE, is_white_show = FALSE;
	if(TBLACK == tip_role){				// 黑子角色提示
		if(FALSE == is_black_show){
			gtk_widget_show(black_tip);
			is_black_show = TRUE;
		}else{
			gtk_widget_hide(black_tip);
			is_black_show = FALSE;
		}
		
		gtk_widget_show(white_tip);
		is_white_show = FALSE;
		
	}else if(TWHITE == tip_role){		// 白子角色提示
		if(FALSE == is_white_show){
			gtk_widget_show(white_tip);
			is_white_show = TRUE;
		}else{
			gtk_widget_hide(white_tip);
			is_white_show = FALSE;
		}
		
		gtk_widget_show(black_tip);
		is_black_show = TRUE;
		
	}else if(TSTOP == tip_role){		// 不提示提示角色
		gtk_widget_show(black_tip);
		gtk_widget_show(white_tip);
		is_black_show = FALSE, is_white_show = FALSE;
	}
	
	return TRUE;
}

// 显示主窗口所有部件
void show_win(GtkWidget *temp)
{
	gtk_widget_show_all(temp);
}

// 显示黑白子个数统计控件
void show_num_label()
{
	gtk_widget_show(black_num_label);
	gtk_widget_show(white_num_label);
}

// 隐藏黑白子个数统计控件
void hide_num_label()
{
	gtk_widget_hide(black_num_label);
	gtk_widget_hide(white_num_label);
}

// 显示倒计时控件
void show_time_label()
{
	gtk_widget_show(time_label);
}

// 隐藏倒计时控件
void hide_time_label()
{
	gtk_widget_hide(time_label);
}

/*
 * 	功能：信息对话框
 *  temp：需要显示的信息
 */
void show_info_dialog(const gchar *temp)
{
	GtkWidget* dialog;
    dialog = gtk_message_dialog_new(
            GTK_WINDOW(window),
            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            g_locale_to_utf8(temp,-1,0,0,0));// g_locale_to_utf8:使用UTF-8的编码来显示中文
	gtk_window_set_title(GTK_WINDOW(dialog),  g_locale_to_utf8("提示",-1,0,0,0));
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// 得到主窗口
GtkWidget *get_mian_window()
{
	return window;
}

// 返回画图的区域
GtkWidget *get_draw_area()
{
	return area;
}

// 得到倒计时显示控件
GtkWidget *get_time_label()
{
	return time_label;
}


