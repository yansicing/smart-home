#ifndef _SUNGTK_INTERFACE_H_
#define _SUNGTK_INTERFACE_H_

#include<gtk/gtk.h>


#define GET_WIDGET_WIDTH(widget)	widget->allocation.width	//widget宽度
#define GET_WIDGET_HEIGHT(widget)	widget->allocation.height	//widget高度

/*****************************************************
* 函数功能:设置背景图
* 参数类型:	widget: 主窗口
* 				w, h:   图片的大小
* 				path：  图片路径
* 返回值:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
******************************************************/
extern int sungtk_background_set_picture(GtkWidget *widget, const gchar *path, const int w, const int h);


/**************************************************
 * 函数功能:给创建好的image重新设计一张图片
 * 参数类型:
 *				image:已创建好的image控件
 * 				file_path:图片路径
 *				 w、h:图片长度和宽带，如w==0&&h==0,则使用图片默认大小
 * 返回类型:成功返回0,失败返回-1
 * 维护记录:2013-12-18 by lihuibo
 **************************************************/
extern int sungtk_image_load_picture(GtkWidget *image, const char *file_path, const int w, const int h );

/**************************************************
 * 函数功能:创建一个带图片的image控件
 * 参数类型:
 *				image:已创建好的image控件
 * 				file_path:图片路径
 *				 w、h:图片长度和宽带，如w==0&&h==0,则使用图片默认大小
 * 返回类型:成功返回0,失败返回-1
 * 维护记录:2013-12-18 by lihuibo
 **************************************************/
GtkWidget *sungtk_image_new_from_file(const char *file_path, const int w, const int h);


/**************************************************
 * 函数功能:根据图片路径创建一个新按钮，同时指定图片大小
 * 参数类型:
 * 				file_path:图片路径
 * 				w、h:图片长度和宽带，如w==0&&h==0,则使用图片默认大小
 * 返回类型:成功返回创建button地址
 * 维护记录:2013-12-18 by lihuibo
 **************************************************/
extern GtkWidget *sungtk_button_new_with_image(const char *file_path, const int w, const int h);

/**************************************************
 * 函数功能:重新设置按钮的照片
 * 参数类型:button:要设置的按钮控件
 * 				file_path:图片路径
 * 				w、h:图片长度和宽带，如w==0&&h==0,则使用图片默认大小
 * 返回类型:
 * 维护记录:2013-12-18 by lihuibo
 **************************************************/
int sungtk_button_set_image(GtkWidget *button, const gchar *file_path, const int w, const int h);



/**************************************************
 * 函数功能: 设置控件字体大小
 * 参数类型:
 * 				widget: 需要改变字体的控件
 * 				size: 字体大小
 *				is_button: TRUE代表控件为按钮，FALSE为其它控件
 * 返回类型:成功返回0，失败返回-1
 * 维护记录:2013-12-18 by lihuibo
 **************************************************/
extern int sungtk_widget_set_font_size(GtkWidget *widget, int size, gboolean is_button);

/***********************************************
* 函数功能:获取一个GdkColor类型
* 参数类型:color_buf:设置字体颜色，如"red"、"blue"
				color:存放GdkColor类型地址
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_color_get(const char *color_buf, GdkColor *color);

/***********************************************
* 函数功能:设置控件字体颜色
* 参数类型:widget:需要改变颜色的控件
				color_buf:设置字体颜色，如"red"、"blue"
				is_button:是否为button
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_widget_set_font_color(GtkWidget *widget, const char *color_buf, gboolean is_button);

/***********************************************
* 函数功能:创建一个滚动窗口
* 参数类型:
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
extern GtkWidget *sungtk_scrolled_window_new(void);

/***********************************************
* 函数功能:设置滚动窗口的水平值
* 参数类型:widget:滚动窗口地址
				add_val:原基础之上进行相加，正负值滚动相反方向
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_scrolled_window_set_hadjustment_value(GtkWidget *s_window, const gdouble add_val);

/***********************************************
* 函数功能:设置滚动窗口的垂直方向值
* 参数类型:widget:滚动窗口地址
				add_val:原基础之上进行相加，正负值滚动相反方向
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_scrolled_window_set_vadjustment_value(GtkWidget *s_window, const gdouble add_val);


/***********************************************
* 函数功能:clist添加一行，该clist只有一列
* 参数类型:clist:列表地址
				data:追加行的数据
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_clist_append_only_a_column(GtkCList *clist, char *data);


/***********************************************
* 函数功能:entry行编辑删除最后一个字符
* 参数类型:entry行编辑地址
* 返回类型:成功返回0，失败返回-1
* 维护记录:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_entry_backspace(GtkEntry *entry);





/************************自定义列表************************************/
typedef gboolean (*SunGtkSelectRow)(GtkWidget *widget, GdkEventButton *event, gpointer data);

typedef struct _clist_link
{
	GtkWidget *event_box;
	GtkWidget *label;
	struct _clist_link *next;
}CListLink;

typedef struct _sungtk_clist
{
	GtkWidget *fixed;
	GtkWidget *vbox;
	gint select_row;
	gint font_size;
	gchar font_color[30];
	gint row_height;
	gint col_width;
	gint total_num;
	gchar sig_str[30];
	SunGtkSelectRow callback;
	CListLink *head;
}SunGtkCList;

#define SUNGTK_CLIST(x) (SunGtkCList *)x


extern SunGtkCList *sungtk_clist_new();


extern int sungtk_clist_unref(SunGtkCList *clist);

extern int sungtk_clist_append(SunGtkCList *clist, const gchar *text);

extern int sungtk_clist_set_foreground(SunGtkCList *clist, const gchar *color_buf);

extern int sungtk_clist_set_row_color(SunGtkCList *clist, gint row, const gchar *color_buf);

extern int sungtk_clist_select_row(SunGtkCList *clist, gint row, const gchar *color_buf);

extern int sungtk_clist_set_row_height(SunGtkCList *clist, gint height);

extern int sungtk_clist_set_col_width(SunGtkCList *clist, gint width);

extern int sungtk_clist_set_text_size(SunGtkCList *clist, int size);

extern const gchar *sungtk_clist_get_row_data(SunGtkCList *clist, gint row);

extern int sungtk_clist_set_row_data(SunGtkCList *clist, gint row, const gchar *text);


extern int sungtk_clist_set_select_row_signal(SunGtkCList *clist, const gchar *detailed_signal, SunGtkSelectRow callback);


#endif


