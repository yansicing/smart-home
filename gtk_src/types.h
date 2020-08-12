#ifndef _TYPES_H_
#define _TYPES_H_

#include<gtk/gtk.h>
#include<glade/glade.h>  
#include<sys/types.h>
#include"./sungtk_interface/sungtk_interface.h"


#define WIN_BACK_WIDTH	800
#define WIN_BACK_HEIGHT	480

//#define BACK_PIC_PATH "../picture/background/5.jpg"



/*******主页控制*******/
typedef struct
{
	GtkWidget *little_event_box;
	GtkWidget *little_hbox;
	GtkWidget *little_button;
	GtkWidget *event_box;
	GtkWidget *fixed;
	GtkWidget *to_home;
	GtkWidget *to_lock;
	GtkWidget *to_exit;
}homepage_t;


/****************滑动解锁*******************************************/
typedef struct 
{
	GtkWidget *page0_fixed;
	GtkWidget *pwd_label;
	GtkWidget *image;
	SungtkMotionLock *lock;
}motionlock_t;



/*********led灯控制界面***************/
typedef struct
{
	GtkWidget *page1_fixed;
	GtkWidget *image;
	GtkWidget *led[3];
//	GtkWidget *switch_led1;
//	GtkWidget *switch_led2;
//	GtkWidget *switch_led3;
}switch_t;

/***********温湿度********************************/
typedef struct
{
	GtkWidget *page2_fixed;
	GtkWidget *image;
	GtkWidget *temp;
	GtkWidget *humi;
	GtkWidget *temp_image;
	GtkWidget *humi_image;
}temp_humi_t;


/********GPRS界面*************/
typedef struct
{
	GtkWidget *page3_fixed;
	GtkWidget *image;
	GtkWidget *tel;
	GtkWidget *msg;
	GtkWidget *tel_label;
	GtkWidget *msg_label;
	GtkWidget *btn_call;
	GtkWidget *btn_send;
}gprs_t;


/*************mplayer**********************/
typedef struct
{
	pid_t pid;
	gint start_flag;
}mplayer_t;



/*************黑白棋*******************/
typedef struct
{
	pid_t pid;
}chess_t;


/***************日历******************/
typedef struct
{
	GtkWidget *calendar;
	GtkWidget *image;
}calendar_t;

/*************计算器*************/
typedef struct 
{
	pid_t pid;
}calc_t;

typedef struct 
{
	pid_t pid;
}backstage_t;


/***********************************************************/
typedef struct
{
	GladeXML *gxml;  

	GtkWidget *win_back;		/*主窗口*/
	GtkWidget *win_event_box;
	GtkWidget *win_back_image;
	GtkWidget *win_back_fixed;
	GtkWidget *win_back_table;
	GtkWidget *button_led;
	GtkWidget *button_temp;
	GtkWidget *button_gprs;
	GtkWidget *button_mplayer;
	GtkWidget *button_calendar;
	GtkWidget *button_game;
	GtkWidget *button_calc;
	GtkWidget *label_ip;
	GtkWidget *notebook;
	GtkWidget *fixed_page[9];
	
	
	homepage_t homepage;
	motionlock_t motionlock;
	switch_t switch_ctl;
	temp_humi_t temp_humi;
	gprs_t gprs;
	mplayer_t mplayer;
	chess_t chess;
	calendar_t calendar;
	calc_t calc;
	backstage_t back;

	gint cur_page;	//当前显示页面标志位
	gint little_button_flag;//小按钮窗口显示标志位
	gint screen_bright;//屏幕亮度值
	gint screen_count;//屏幕变暗秒数
}smarthome_t;

#endif

