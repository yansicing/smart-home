#ifndef _SUNGTK_INTERFACE_H_
#define _SUNGTK_INTERFACE_H_

#include<gtk/gtk.h>


#define GET_WIDGET_WIDTH(widget)	widget->allocation.width	//widget���
#define GET_WIDGET_HEIGHT(widget)	widget->allocation.height	//widget�߶�

/*****************************************************
* ��������:���ñ���ͼ
* ��������:	widget: ������
* 				w, h:   ͼƬ�Ĵ�С
* 				path��  ͼƬ·��
* ����ֵ:�ɹ�����0��ʧ�ܷ���-1
* ά����¼:2013-12-18 by lihuibo
******************************************************/
extern int sungtk_background_set_picture(GtkWidget *widget, const gchar *path, const int w, const int h);


/**************************************************
 * ��������:�������õ�image�������һ��ͼƬ
 * ��������:
 *				image:�Ѵ����õ�image�ؼ�
 * 				file_path:ͼƬ·��
 *				 w��h:ͼƬ���ȺͿ������w==0&&h==0,��ʹ��ͼƬĬ�ϴ�С
 * ��������:�ɹ�����0,ʧ�ܷ���-1
 * ά����¼:2013-12-18 by lihuibo
 **************************************************/
extern int sungtk_image_load_picture(GtkWidget *image, const char *file_path, const int w, const int h );

/**************************************************
 * ��������:����һ����ͼƬ��image�ؼ�
 * ��������:
 *				image:�Ѵ����õ�image�ؼ�
 * 				file_path:ͼƬ·��
 *				 w��h:ͼƬ���ȺͿ������w==0&&h==0,��ʹ��ͼƬĬ�ϴ�С
 * ��������:�ɹ�����0,ʧ�ܷ���-1
 * ά����¼:2013-12-18 by lihuibo
 **************************************************/
GtkWidget *sungtk_image_new_from_file(const char *file_path, const int w, const int h);


/**************************************************
 * ��������:����ͼƬ·������һ���°�ť��ͬʱָ��ͼƬ��С
 * ��������:
 * 				file_path:ͼƬ·��
 * 				w��h:ͼƬ���ȺͿ������w==0&&h==0,��ʹ��ͼƬĬ�ϴ�С
 * ��������:�ɹ����ش���button��ַ
 * ά����¼:2013-12-18 by lihuibo
 **************************************************/
extern GtkWidget *sungtk_button_new_with_image(const char *file_path, const int w, const int h);

/**************************************************
 * ��������:�������ð�ť����Ƭ
 * ��������:button:Ҫ���õİ�ť�ؼ�
 * 				file_path:ͼƬ·��
 * 				w��h:ͼƬ���ȺͿ������w==0&&h==0,��ʹ��ͼƬĬ�ϴ�С
 * ��������:
 * ά����¼:2013-12-18 by lihuibo
 **************************************************/
int sungtk_button_set_image(GtkWidget *button, const gchar *file_path, const int w, const int h);



/**************************************************
 * ��������: ���ÿؼ������С
 * ��������:
 * 				widget: ��Ҫ�ı�����Ŀؼ�
 * 				size: �����С
 *				is_button: TRUE����ؼ�Ϊ��ť��FALSEΪ�����ؼ�
 * ��������:�ɹ�����0��ʧ�ܷ���-1
 * ά����¼:2013-12-18 by lihuibo
 **************************************************/
extern int sungtk_widget_set_font_size(GtkWidget *widget, int size, gboolean is_button);

/***********************************************
* ��������:��ȡһ��GdkColor����
* ��������:color_buf:����������ɫ����"red"��"blue"
				color:���GdkColor���͵�ַ
* ��������:�ɹ�����0��ʧ�ܷ���-1
* ά����¼:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_color_get(const char *color_buf, GdkColor *color);

/***********************************************
* ��������:���ÿؼ�������ɫ
* ��������:widget:��Ҫ�ı���ɫ�Ŀؼ�
				color_buf:����������ɫ����"red"��"blue"
				is_button:�Ƿ�Ϊbutton
* ��������:�ɹ�����0��ʧ�ܷ���-1
* ά����¼:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_widget_set_font_color(GtkWidget *widget, const char *color_buf, gboolean is_button);

/***********************************************
* ��������:����һ����������
* ��������:
* ��������:�ɹ�����0��ʧ�ܷ���-1
* ά����¼:2013-12-18 by lihuibo
***********************************************/
extern GtkWidget *sungtk_scrolled_window_new(void);

/***********************************************
* ��������:���ù������ڵ�ˮƽֵ
* ��������:widget:�������ڵ�ַ
				add_val:ԭ����֮�Ͻ�����ӣ�����ֵ�����෴����
* ��������:�ɹ�����0��ʧ�ܷ���-1
* ά����¼:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_scrolled_window_set_hadjustment_value(GtkWidget *s_window, const gdouble add_val);

/***********************************************
* ��������:���ù������ڵĴ�ֱ����ֵ
* ��������:widget:�������ڵ�ַ
				add_val:ԭ����֮�Ͻ�����ӣ�����ֵ�����෴����
* ��������:�ɹ�����0��ʧ�ܷ���-1
* ά����¼:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_scrolled_window_set_vadjustment_value(GtkWidget *s_window, const gdouble add_val);


/***********************************************
* ��������:clist���һ�У���clistֻ��һ��
* ��������:clist:�б��ַ
				data:׷���е�����
* ��������:�ɹ�����0��ʧ�ܷ���-1
* ά����¼:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_clist_append_only_a_column(GtkCList *clist, char *data);


/***********************************************
* ��������:entry�б༭ɾ�����һ���ַ�
* ��������:entry�б༭��ַ
* ��������:�ɹ�����0��ʧ�ܷ���-1
* ά����¼:2013-12-18 by lihuibo
***********************************************/
extern int sungtk_entry_backspace(GtkEntry *entry);





/************************�Զ����б�************************************/
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


