#include<gtk/gtk.h>
#include<glade/glade.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"types.h"
#include"home_init.h"
#include"./sungtk_interface/sungtk_interface.h"
#include"./keyboard/gtk_keyboard.h"
#include"agreement.h"
#include"home_display.h"

static smarthome_t *_home;

static int set_screen_brightness(int num);
static int screen_brightness_is_light(smarthome_t *home);

/*************根据cur_page标志位，显示相应窗口********************/
void show_page(smarthome_t *home, gint cur_page)
{
	if(cur_page == 100){	//主页
		gtk_widget_hide_all(home->notebook);
		gtk_widget_show_all(home->win_back_image);
		gtk_widget_show_all(home->win_back_table);
	}else{
		gtk_widget_hide_all(home->notebook);
		gtk_widget_hide_all(home->win_back_image);
		gtk_widget_hide_all(home->win_back_table);
		gtk_widget_show_all(home->fixed_page[cur_page]);
		gtk_widget_show(home->notebook);
	}
	gtk_widget_show_all(home->homepage.little_hbox);
}

/*****************启动Mplayer**********************/
static int start_mplayer(smarthome_t *home)
{
	if(home->mplayer.start_flag == 1)
		return -1;
	
	home->mplayer.start_flag = 1;
	
	home->mplayer.pid = vfork();
	if(home->mplayer.pid < 0){
		return -1;
	}else if(home->mplayer.pid == 0){
		close(0);
		//printf("getpgid=====%d\n", getpgid(getpid()));
		chdir("../Mplayer_gtk/execute/");
		printf("Mplayer_gtk=====%s====\n", getcwd(NULL, 0));
		execl(MPALYER_EXE_PATH, MPALYER_EXE_PATH, NULL);
		perror("execl Mplayer_gtk");
		exit(-1);
	}
	return 0;
}

/*****************启动黑白棋********************/
static int start_chess_game(smarthome_t *home)
{
	home->chess.pid = vfork();
	if(home->chess.pid < 0){
		return -1;
	}else if(home->chess.pid == 0){
		close(0);
		chdir("../chess_gtk/");
		//printf("getcwd=====%s====\n", getcwd(NULL, 0));
		execl(GAME_EXE_PATH, GAME_EXE_PATH, NULL);
		perror("execl chess game");
		exit(-1);
	}
	return 0;
}

/***************启动计算器********************/
static int start_calc(smarthome_t *home)
{
	home->calc.pid = vfork();
	if(home->calc.pid < 0){
		return -1;
	}else if(home->calc.pid == 0){
		close(0);
		chdir("../calc_gtk/");
		//printf("getcwd=====%s====\n", getcwd(NULL, 0));
		execl(CALC_EXE_PATH, CALC_EXE_PATH, NULL);
		perror("execl calc");
		exit(-1);
	}
	return 0;
}

/***************设置显示窗口标志位*******************/
static void cur_page_set_value(smarthome_t *home, gint cur_page)
{
	home->cur_page = cur_page;
}

/*************主窗口按钮回调函数*****************/
static void callback_back_button(GtkWidget *widget, gpointer data)
{
	smarthome_t *home = (smarthome_t *)data;
	screen_brightness_is_light(home);
	
	if(widget == home->button_led){	//led
		home->cur_page = 1;
	}else if(widget == home->button_temp){ //温湿度
		home->cur_page = 2;
	}else if(widget == home->button_gprs){	//gprs
		home->cur_page = 3;
	}else if(widget == home->button_mplayer){//mplayer
		start_mplayer(home);	
		//gtk_widget_hide_all(home->win_back);
		gtk_widget_hide_all(home->win_back_table);
		return;
	}else if(widget == home->button_calendar){	//日历
		home->cur_page = 5;
	}else if(widget == home->button_game){	//黑白棋
		gtk_widget_hide_all(home->win_back);
		start_chess_game(home);
		return;
	}else if(widget == home->button_calc){	//计算器
		start_calc(home);
		gtk_widget_hide_all(home->win_back);
		return;
	}
	show_page(home, home->cur_page);
}

/*************发送停止命令给mplayer****************/
#define MPLAYER_MSG_FIFO_PATH "../Mplayer_gtk/execute/player_msg_fifo"
static void send_cmd_mplayer_exit()
{
	int fd = open(MPLAYER_MSG_FIFO_PATH, O_RDWR);
	if(fd < 0){
		perror("open msg fifo");
		return;
	}
	char *str = "MPLAYER_EXIT";
	write(fd, str, strlen(str));
	close(fd);
}

/**********启动或停止mplayer***************/
void start_mplayer_with_cmd(smarthome_t *home)
{
	if(home->mplayer.start_flag == 0){
		if(home->mplayer.pid==0 && home->calc.pid==0 && home->chess.pid==0)
			callback_back_button(home->button_mplayer, home);
	}else{
		//printf("kill mplayer pid == %d\n", home->mplayer.pid);
		send_cmd_mplayer_exit();
		//kill(home->mplayer.pid, SIGINT);
		//kill(home->mplayer.pid, SIGKILL);
	}
	
	//callback_back_button(home->button_mplayer, home);
}


/***********设置小按钮窗口显示标志位*************/
static inline void little_button_set_flag(smarthome_t *home, gint little_flag)
{
	home->little_button_flag = little_flag;
}
/*********小按钮窗口显示标志位置反*************/
static inline void little_button_flag_turn(smarthome_t *home)
{
	home->little_button_flag = (home->little_button_flag==0) ? 1 : 0;
}

void set_sensitive_after_little_button(smarthome_t *home, gboolean sensitive)
{
	if(home->cur_page == 100){
		gtk_widget_set_sensitive(home->win_back_table, sensitive);
	}else{
		gtk_widget_set_sensitive(home->fixed_page[home->cur_page], sensitive);
	}
}

/***********小按钮窗口使能、显示设置***************/
static void little_button_press_deal(smarthome_t *home)
{
	if(home->little_button_flag == 0){
	//	gtk_widget_set_sensitive(home->win_back_table, TRUE);
	//	gtk_widget_set_sensitive(home->notebook, TRUE);
		set_sensitive_after_little_button(home, TRUE);
		gtk_widget_hide_all(home->homepage.event_box);
	}else{
	//	gtk_widget_set_sensitive(home->notebook, FALSE);
	//	gtk_widget_set_sensitive(home->win_back_table, FALSE);
		set_sensitive_after_little_button(home, FALSE);
		gtk_widget_show_all(home->homepage.event_box);
	}
}

/***********小按钮窗口是否显示******************/
static void callback_little_button(GtkWidget *widget, gpointer data)
{
	smarthome_t *home = (smarthome_t *)data;
	little_button_flag_turn(home);
	little_button_press_deal(home);
//	screen_brightness_is_light(home);
}

/*
static void callback_little_button(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	smarthome_t *home = (smarthome_t *)data;
	printf("eeeeeeeee\n");
	little_button_flag_turn(home);
	little_button_press_deal(home);
}
*/

/***************回到主页********************/
static void callback_little_button_to_home(GtkWidget *widget, gpointer data)
{
	smarthome_t *home = (smarthome_t *)data;
	little_button_set_flag(home, 0);
	little_button_press_deal(home);

	cur_page_set_value(home, 100);
	show_page(home, home->cur_page);
}

/***************回调滑动密码锁**********************/
static void callback_little_button_to_lock(GtkWidget *widget, gpointer data)
{
	smarthome_t *home = (smarthome_t *)data;
	little_button_set_flag(home, 0);
	little_button_press_deal(home);

	show_page(home, 0);
	gtk_widget_hide_all(home->homepage.little_hbox);
}

/************退出*****************/
static void callback_little_button_to_exit(GtkWidget *widget, gpointer data)
{
	smarthome_t *home = (smarthome_t *)data;
	gtk_widget_hide_all(home->win_back);
	kill(home->back.pid, SIGKILL);
	gtk_main_quit();
}

/*******************滑动密码锁抬起时，处理结果***********************/
static gboolean callback_motionlock_release_event(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	SungtkMotionLock *lock = (SungtkMotionLock *)data;

	char get_pwd[10] = "";
	sungtk_motion_lock_get_password(lock, get_pwd, sizeof(get_pwd));//获取滑动后的值
	//printf("get_pwd===%s\n", get_pwd);
	if(strcmp(get_pwd, HAND_PWD)==0){	//密码正确
		printf("right\n");
		//printf("get_pwd===%s\n", get_pwd);
		gtk_label_set_text(GTK_LABEL(_home->motionlock.pwd_label), "Password");
		show_page(_home, _home->cur_page);//
	}else{
		gtk_label_set_text(GTK_LABEL(_home->motionlock.pwd_label), "Error ! ! !");
	}
	
	sungtk_motion_lock_release_deal(lock);//最后一定要调用
	return TRUE;
}

/***************键盘按下回调函数********************/
static void callback_gprs_keyboard(GtkWidget *widget, gpointer data)
{
	const gchar *key = NULL;
	const gchar *temp = NULL;
	gchar now[100] = "";	/*字符的个数不能超过100，有待完善*/
	int i;
	KEYBOARD *board = (KEYBOARD *)data;
	key = gtk_button_get_label(GTK_BUTTON(widget));/*获得点击按钮的内容*/
	//printf("key====%s====\n", key);

	for(i=0;board->recv[i]!=NULL;++i)	/*判断接收者的个数*/
	{
		if(gtk_widget_is_focus(board->recv[i]))/*焦点在哪*/
		{
			if(strcmp(key, CLEAR)==0)/*清空*/
			{
				gtk_entry_set_text(GTK_ENTRY(board->recv[i]), "");
			}
			else if(strcmp(key, BACKSPACE)==0)/*删除一个字符*/
			{
				temp = gtk_entry_get_text(GTK_ENTRY(board->recv[i]));
				if(strcmp(temp, "") != 0)/*输入框内不为空*/
				{
					strncpy(now, temp, strlen(temp)-1);
					gtk_entry_set_text(GTK_ENTRY(board->recv[i]), now);
				}
			}
			else	/*追加一个字符*/
			{
				gtk_entry_append_text(GTK_ENTRY(board->recv[i]), key);
			}
			temp = gtk_entry_get_text(GTK_ENTRY(board->recv[i])); 
			gtk_entry_set_position(GTK_ENTRY(board->recv[i]), strlen(temp));/*重新设置光标的位置*/
			break;	/*焦点找到后退出循环*/
		}
	}
}

/**************背景被点击后的处理*********************/
static gboolean callback_back_eventbox_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	smarthome_t *home = (smarthome_t *)data;
	//printf("back eventbox press x==%lf, y==%lf\n", event->x, event->y);
	if(home->little_button_flag == 1){
		little_button_set_flag(home, 0);//小按钮窗口显示标志设置
		little_button_press_deal(home);//

		cur_page_set_value(home, home->cur_page);
		show_page(home, home->cur_page);//当前页显示
	}

	screen_brightness_is_light(home);//设置屏幕亮
	return TRUE;
}

static gboolean callback_back_eventbox_release(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	//printf("back eventbox release x==%lf, y==%lf\n", event->x, event->y);
	return TRUE;
}



/**********led灯点击回调函数***********/
static void callback_led_clicked(GtkWidget *widget, gpointer data)
{
	smarthome_t *home = (smarthome_t *)data;
	char bag[256] = "";
	char other_data[32] = "";
	int led_status = 0;
	int i;
	
	for(i=0;i<3;i++){
		if(widget == home->switch_ctl.led[i]){
			led_status = i+1;
			break;
		}
	}
	sprintf(other_data, "%d", led_status);

	group_sunhome_local_client_to_server_bag(bag, SUNHOME_LED_SET_CMD, other_data);
	//printf("bag=====%s======\n", bag);
	
	sunhome_send_bag_to_client(bag, LOOPBACK_ADDR_IP, SERVER_LOCAL_RECV_PORT);
}


static void callback_send_message(GtkWidget *widget, gpointer data)
{
	smarthome_t *home = (smarthome_t *)data;
	const char *tel = gtk_entry_get_text(GTK_ENTRY(home->gprs.tel));
	const char *msg = gtk_entry_get_text(GTK_ENTRY(home->gprs.msg));

	char bag[256] = "";
	char other_data[32] = "";
	sprintf(other_data, "%s&%s", tel, msg);

	group_sunhome_local_client_to_server_bag(bag, 
			SUNHOME_GPRS_SEND_MSG_CMD, other_data);
		
	sunhome_send_bag_to_client(bag, LOOPBACK_ADDR_IP, SERVER_LOCAL_RECV_PORT);

	printf("tel=====%s====\n", tel);
	printf("msg=====%s====\n", msg);
}

static void callback_call(GtkWidget *widget, gpointer data)
{
	smarthome_t *home = (smarthome_t *)data;
	const char *tel = gtk_entry_get_text(GTK_ENTRY(home->gprs.tel));
	printf("tel=====%s====\n", tel);

	char bag[256] = "";
	char other_data[32] = "";
	
	sprintf(other_data, "%s", tel);
	group_sunhome_local_client_to_server_bag(bag, 
			SUNHOME_GPRS_CALL_CMD, other_data);
		
	sunhome_send_bag_to_client(bag, LOOPBACK_ADDR_IP, SERVER_LOCAL_RECV_PORT);
	//printf("call=======%s=======\n", bag);



}



/**********但gtk中创建的子进程介绍，则回收期资源*****************/
void son_process_killed_deal(int signum)
{
	printf("son is killed\n");
	pid_t pid;
	pid = waitpid(-1, NULL, 0); //
	if(pid > 0){
		if(_home->mplayer.pid == pid){
			_home->mplayer.start_flag = 0;
			_home->mplayer.pid = 0;
		}else if(_home->chess.pid == pid){
			_home->chess.pid = 0;
		}else if(_home->calc.pid == pid){
			_home->calc.pid = 0;
		}
		
		gtk_widget_show_all(_home->win_back);
		//gtk_widget_show_all(_home->win_back_table);
		gtk_widget_hide_all(_home->homepage.event_box);
		show_page(_home, _home->cur_page);
	}
	
	//printf("====kill pid=====%d====%d==\n", pid, _home->mplayer.pid);
}


/************主窗口中的按钮*********************/
static void back_button_init(smarthome_t *home)
{
	//led control
	home->button_led = sungtk_button_new_with_image(BACK_LED_PIC, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);
	gtk_table_attach_defaults(GTK_TABLE(home->win_back_table), home->button_led, 2, 4, 1, 2);
	g_signal_connect(home->button_led, "clicked", G_CALLBACK(callback_back_button), home);

	//temp and humi
	home->button_temp = sungtk_button_new_with_image(BACK_TEMP_HUMI_PIC, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);
	gtk_table_attach_defaults(GTK_TABLE(home->win_back_table), home->button_temp, 5, 7, 1, 2);
	g_signal_connect(home->button_temp, "clicked", G_CALLBACK(callback_back_button), home);

	//gprs send msg
	
	home->button_gprs = sungtk_button_new_with_image(BACK_GPRS_PIC, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);
	gtk_table_attach_defaults(GTK_TABLE(home->win_back_table), home->button_gprs, 8, 10, 1, 2);
	g_signal_connect(home->button_gprs, "clicked", G_CALLBACK(callback_back_button), home);
	//mplayer
	home->button_mplayer = sungtk_button_new_with_image(BACK_MPLAYER_PIC, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);
	gtk_table_attach_defaults(GTK_TABLE(home->win_back_table), home->button_mplayer, 11, 13, 1, 2);
	g_signal_connect(home->button_mplayer, "clicked", G_CALLBACK(callback_back_button), home);
	


	//calendar
	home->button_calendar = sungtk_button_new_with_image(BACK_CALENDAR_PIC, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);
	gtk_table_attach_defaults(GTK_TABLE(home->win_back_table), home->button_calendar,2, 4, 3, 4);
	g_signal_connect(home->button_calendar, "clicked", G_CALLBACK(callback_back_button), home);
	

	//game
	home->button_game = sungtk_button_new_with_image(BACK_GAME_PIC, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT);
	gtk_table_attach_defaults(GTK_TABLE(home->win_back_table), home->button_game,5, 7, 3, 4);
	g_signal_connect(home->button_game, "clicked", G_CALLBACK(callback_back_button), home);
	

	//calc
	
	home->button_calc = sungtk_button_new_with_image(BACK_CALC_PIC, BACK_BUTTON_WIDTH+15, BACK_BUTTON_HEIGHT+15);
	gtk_table_attach_defaults(GTK_TABLE(home->win_back_table), home->button_calc,8, 10, 3, 4);
	g_signal_connect(home->button_calc, "clicked", G_CALLBACK(callback_back_button), home);
	
}

/****************主页小按钮初始化**********************/
static void little_button_init(smarthome_t *home)
{
	//little hbox
	//home->homepage.little_event_box = gtk_event_box_new();
	home->homepage.little_hbox = gtk_hbox_new(TRUE, 0);
	gtk_widget_set_size_request(home->homepage.little_hbox, BACK_HOME_BUTTON_WIDTH+20, BACK_HOME_BUTTON_HEIGHT+20);
	//gtk_event_box_set_visible_window(GTK_EVENT_BOX(home->homepage.little_event_box), FALSE);
	gtk_fixed_put(GTK_FIXED(home->win_back_fixed), home->homepage.little_hbox, 200, 428);
	
	//little button
	home->homepage.little_button = sungtk_button_new_with_image(BACK_HOME_BUTTON_PIC, BACK_HOME_BUTTON_WIDTH, BACK_HOME_BUTTON_HEIGHT);
	gtk_container_add(GTK_CONTAINER(home->homepage.little_hbox), home->homepage.little_button);
	g_signal_connect(home->homepage.little_button, "clicked", G_CALLBACK(callback_little_button), home);
	gtk_widget_show_all(home->homepage.little_hbox);


	//little button press show
	home->homepage.event_box = gtk_event_box_new();
	gtk_event_box_set_visible_window(GTK_EVENT_BOX(home->homepage.event_box), FALSE);
	gtk_widget_set_size_request(home->homepage.event_box, LITTLE_WIN_WIDTH, LITTLE_WIN_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->win_back_fixed), home->homepage.event_box, 300, 150);
	
	home->homepage.fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(home->homepage.event_box), home->homepage.fixed);

	//to homepage
	home->homepage.to_home = sungtk_button_new_with_image(LITTLE_TO_HOME_PIC, LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->homepage.fixed), home->homepage.to_home, 80, 20);
	g_signal_connect(home->homepage.to_home, "clicked", G_CALLBACK(callback_little_button_to_home), home);

	//to motion lock
	home->homepage.to_lock = sungtk_button_new_with_image(LITTLE_TO_LOCK_PIC, LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->homepage.fixed), home->homepage.to_lock, 10, 105);
	g_signal_connect(home->homepage.to_lock, "clicked", G_CALLBACK(callback_little_button_to_lock), home);

	//to exit
	home->homepage.to_exit = sungtk_button_new_with_image(LITTLE_TO_EXIT_PIC, LITTLE_BUTTON_WIDTH, LITTLE_BUTTON_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->homepage.fixed), home->homepage.to_exit, 150, 105);
	g_signal_connect(home->homepage.to_exit, "clicked", G_CALLBACK(callback_little_button_to_exit), home);
}


/****************锁屏界面初始化**********************/
static void motionlock_page0_init(smarthome_t *home)
{
	_home = home;
	home->fixed_page[0] = glade_xml_get_widget(home->gxml, FIXED_PAGE0);

	//锁屏背景图
	home->motionlock.image = sungtk_image_new_from_file(MOTIONLOCK_IMAGE_PIC, WIN_BACK_WIDTH, WIN_BACK_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[0]), home->motionlock.image, 0, 0);

	home->motionlock.pwd_label = gtk_label_new("Password");
	sungtk_widget_set_font_size(home->motionlock.pwd_label, MOTIONLOCK_LABEL_SIZE, FALSE);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[0]), home->motionlock.pwd_label,
		316, 50);

	//初始化自定义滑动锁
	home->motionlock.lock = sungtk_motion_lock_new(MOTIONLOCK_SIZE, MOTIONLOCK_IMG_SIZE, 
		MOTIONLOCK_PRESS_PIC, MOTIONLOCK_UNPRESS_PIC);
	//设置挑起时的回调函数
	sungtk_motion_lock_signal_release(home->motionlock.lock, callback_motionlock_release_event);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[0]), home->motionlock.lock->event_box,
		200, 85);
}

/***************led灯界面初始化****************/
static void led_page1_init(smarthome_t *home)
{
	home->fixed_page[1] = glade_xml_get_widget(home->gxml, FIXED_PAGE1);
	home->switch_ctl.image = sungtk_image_new_from_file(LED_IMAGE_PIC, WIN_BACK_WIDTH, WIN_BACK_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[1]), home->switch_ctl.image, 0, 0);

	//led1
	home->switch_ctl.led[0] = sungtk_button_new_with_image(LED_BUTTON_LIGHT_PIC, LED_BUTTON_WIDTH, LED_BUTTON_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[1]), home->switch_ctl.led[0],
		100, LED_BUTTON_POS_Y);
	g_signal_connect(home->switch_ctl.led[0], "pressed", G_CALLBACK(callback_led_clicked), home);

	//led2
	home->switch_ctl.led[1] = sungtk_button_new_with_image(LED_BUTTON_LIGHT_PIC, LED_BUTTON_WIDTH, LED_BUTTON_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[1]), home->switch_ctl.led[1],
		300, LED_BUTTON_POS_Y);
	g_signal_connect(home->switch_ctl.led[1], "pressed", G_CALLBACK(callback_led_clicked), home);

	//led3
	home->switch_ctl.led[2] = sungtk_button_new_with_image(LED_BUTTON_LIGHT_PIC, LED_BUTTON_WIDTH, LED_BUTTON_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[1]), home->switch_ctl.led[2],
		500, LED_BUTTON_POS_Y);
	g_signal_connect(home->switch_ctl.led[2], "pressed", G_CALLBACK(callback_led_clicked), home);
}

/*************温湿度显示**************************/
static void temp_page2_init(smarthome_t *home)
{
	home->fixed_page[2] = glade_xml_get_widget(home->gxml, FIXED_PAGE2);
	home->temp_humi.image = sungtk_image_new_from_file(TEMP_IMAGE_PIC, WIN_BACK_WIDTH, WIN_BACK_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[2]), home->temp_humi.image, 0, 0);

	home->temp_humi.temp_image = sungtk_image_new_from_file(TEMP_PIC, TEMP_PIC_WIDTH, TEMP_PIC_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[2]), home->temp_humi.temp_image,
		100, 60);

	home->temp_humi.humi_image = sungtk_image_new_from_file(HUMI_PIC, TEMP_PIC_WIDTH, TEMP_PIC_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[2]), home->temp_humi.humi_image,
		100, 180);

	home->temp_humi.temp = gtk_label_new("13.C");
	sungtk_widget_set_font_size(home->temp_humi.temp, TEMP_FONT_SIZE, FALSE);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[2]), home->temp_humi.temp,
		220, 90);

	home->temp_humi.humi = gtk_label_new("45%");
	sungtk_widget_set_font_size(home->temp_humi.humi, TEMP_FONT_SIZE, FALSE);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[2]), home->temp_humi.humi,
		220, 210);
}

/****************发送短信界面初始************************/
static void gprs_page3_init(smarthome_t *home)
{
	home->fixed_page[3] = glade_xml_get_widget(home->gxml, FIXED_PAGE3);
	home->gprs.image = sungtk_image_new_from_file(GPRS_IMAGE_PIC, WIN_BACK_WIDTH, WIN_BACK_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[3]), home->gprs.image, 0, 0);

	home->gprs.tel_label = gtk_label_new("Tel:");
	sungtk_widget_set_font_size(home->gprs.tel_label, GPRS_LABEL_FONT_SIZE, FALSE);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[3]), home->gprs.tel_label, 230, 64);
	home->gprs.tel = gtk_entry_new();
	sungtk_widget_set_font_size(home->gprs.tel, GPRS_LABEL_FONT_SIZE, FALSE);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[3]), home->gprs.tel, 280, 60);

	
	home->gprs.msg_label = gtk_label_new("Msg:");
	sungtk_widget_set_font_size(home->gprs.msg_label, GPRS_LABEL_FONT_SIZE, FALSE);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[3]), home->gprs.msg_label, 230, 124);
	home->gprs.msg = gtk_entry_new();
	sungtk_widget_set_font_size(home->gprs.msg, GPRS_LABEL_FONT_SIZE, FALSE);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[3]), home->gprs.msg, 280, 120);

	home->gprs.btn_send = sungtk_button_new_with_image(GPRS_SEND_PIC, GPRS_SEND_WIDTH, GPRS_SEND_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[3]), home->gprs.btn_send, 220, 160);
	home->gprs.btn_call = sungtk_button_new_with_image(GPRS_CALL_PIC, GPRS_SEND_WIDTH, GPRS_SEND_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[3]), home->gprs.btn_call, 400, 160);

	g_signal_connect(home->gprs.btn_send, "pressed", G_CALLBACK(callback_send_message), home);
	g_signal_connect(home->gprs.btn_call, "pressed", G_CALLBACK(callback_call), home);

	static KEYBOARD board;//自定义键盘
	memset(&board, 0, sizeof(KEYBOARD));
	board.callback = callback_gprs_keyboard;/*设置回调函数*/
	board.recv[0] = home->gprs.tel;
	board.recv[1] = home->gprs.msg;
	init_keyboard(&board);
	set_keyboard_size(&board, GPRS_BOARD_WIDTH, GPRS_BOARD_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[3]), board.table, 140, 225);
}

static void mplayer_page4_init(smarthome_t *home)
{
	home->fixed_page[4] = glade_xml_get_widget(home->gxml, FIXED_PAGE4);

	
}

/*****************日历界面初始化******************/
static void calendar_page5_init(smarthome_t *home)
{
	home->fixed_page[5] = glade_xml_get_widget(home->gxml, FIXED_PAGE5);

	home->calendar.image = sungtk_image_new_from_file(CALENDAR_IMAGE_PATH, CALENDAR_IMAGE_WIDTH, CALENDAR_IMAGE_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->fixed_page[5]), home->calendar.image, 0, 0);
	//home->calendar.calendar = gtk_calendar_new();
	//gtk_widget_set_size_request(home->calendar.calendar, 800, 190);
	//gtk_fixed_put(GTK_FIXED(home->fixed_page[5]), home->calendar.calendar, 0, 0);

	//gtk_calendar_display_options(GTK_CALENDAR(home->calendar.calendar), 
	//	GTK_CALENDAR_SHOW_DAY_NAMES|GTK_CALENDAR_SHOW_HEADING|GTK_CALENDAR_SHOW_WEEK_NUMBERS);
}


static void game_page5_init(smarthome_t *home)
{
	home->fixed_page[6] = glade_xml_get_widget(home->gxml, FIXED_PAGE6);

	
}

/**************ip显示初始化***************/
static void label_ip_init(smarthome_t *home)
{
	home->label_ip = gtk_label_new("NULL");
	gtk_widget_set_size_request(home->label_ip, 150, 40);
	gtk_misc_set_alignment(GTK_MISC(home->label_ip), 1.0, 0.5);
	sungtk_widget_set_font_size(home->label_ip, 13, FALSE);
	gtk_fixed_put(GTK_FIXED(home->win_back_fixed), home->label_ip, 640, 0);
}

/***************最开始窗口显示状态****************/
void win_init_show(smarthome_t *home)
{
	gtk_widget_show_all(home->win_back);
	show_page(home, 0);
	gtk_widget_hide_all(home->homepage.event_box);
	gtk_widget_hide_all(home->homepage.little_hbox);
}

/***************设置屏幕亮度*****************/
static int set_screen_brightness(int num)
{
	int fd = open(SCREEN_BRIGHTNEWW, O_WRONLY);
	if(fd < 0 ){
		perror("open error");
		return -1;
	}
	char buf[10] = "";
	snprintf(buf, sizeof(buf), "%d", num);
	write(fd, buf, strlen(buf));
	//printf("len===%d\n", len);

	close(fd);
	return 0;
}

/****************是否设置亮*****************/
static int screen_brightness_is_light(smarthome_t *home)
{
	static int press_first_flag = 0;
	home->screen_count = SCREEN_CHANGE_ASH_SECONDS;
	if(home->screen_bright == SCREEN_ASH_BRIGHTNESS_NUM || 
		press_first_flag == 0){
		press_first_flag = 1;
		home->screen_bright = SCREEN_BREGHTNESS_FULLLIGHT_NUM;
		set_screen_brightness(home->screen_bright);
	}
	return 0;
}

/**************屏幕定时回调函数，每一秒计数减一**********************/
static gboolean callback_screen_deal(gpointer data)
{
	smarthome_t *home = (smarthome_t *)data;
	home->screen_count--;
	if(home->screen_count == -1){
		home->screen_bright = SCREEN_ASH_BRIGHTNESS_NUM;
		set_screen_brightness(home->screen_bright);
	}
	return TRUE;
}

/**************屏幕亮度初始化*****************/
static void srceen_brightness_init(smarthome_t *home)
{
	home->screen_bright = SCREEN_BREGHTNESS_FULLLIGHT_NUM;
	home->screen_count = SCREEN_CHANGE_ASH_SECONDS;
	g_timeout_add(SCREEN_ALARM_SECONDS, (GSourceFunc)callback_screen_deal, home);
}

/**************整个窗口初始化********************/
void win_back_init(smarthome_t *home)
{
	home->cur_page = 100;//最开始在主页
	
	home->gxml = glade_xml_new(GLADE_FILE, NULL, NULL);//打开glade
	home->win_back = glade_xml_get_widget(home->gxml, WIN_BACK);//获取glade定义的控件
	g_signal_connect(home->win_back, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	//窗口上的事件盒子，用于点击
	home->win_event_box = glade_xml_get_widget(home->gxml, WIN_EVENT_BOX);
	g_signal_connect(home->win_event_box, "button-press-event", G_CALLBACK(callback_back_eventbox_press), home);
	g_signal_connect(home->win_event_box, "button-release-event", G_CALLBACK(callback_back_eventbox_release), home);
	//g_signal_connect(home->win_event_box, "motion-notify-event", G_CALLBACK(callback_back_eventbox_motion), home);

	home->win_back_fixed = glade_xml_get_widget(home->gxml, FIXED_BACK);
	home->win_back_image = sungtk_image_new_from_file(BACK_HOME_PIC, WIN_BACK_WIDTH, WIN_BACK_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->win_back_fixed), home->win_back_image, 0, 0);
	
	home->win_back_table = gtk_table_new(WIN_BACK_TABLE_ROW, WIN_BACK_TABLE_COL, TRUE);
	gtk_widget_set_size_request(home->win_back_table, WIN_BACK_WIDTH, WIN_BACK_HEIGHT);
	gtk_fixed_put(GTK_FIXED(home->win_back_fixed), home->win_back_table, 0, 0);
	
	//主页按钮初始化
	back_button_init(home);
	gtk_widget_show_all(home->win_back);

	//ip get and show
	label_ip_init(home);

	//notebook
	home->notebook = glade_xml_get_widget(home->gxml, NOTEBOOK_BACK);
	gtk_widget_hide_all(home->notebook);

	//little button
	little_button_init(home);
	

	//page0,motion lock
	motionlock_page0_init(home);

	//page1，led control
	led_page1_init(home);

	//page2, temp and humi
	temp_page2_init(home);

	//page3, gprs send message
	gprs_page3_init(home);

	//page4, mplayer
	mplayer_page4_init(home);

	//page5, calendar
	calendar_page5_init(home);

	//page6, game
	game_page5_init(home);

	//最开始窗口显示的状态
	win_init_show(home);

	//屏幕亮度初始化
	srceen_brightness_init(home);
}

