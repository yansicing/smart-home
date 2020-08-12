#include<gtk/gtk.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/types.h>
#include"types.h"
#include"./deal_song_list/deal_song_list.h"
#include"player_display.h"
#include"./sungtk_interface/sungtk_interface.h"
#include"./encoding/gb2312_ucs2.h"
#include"player_process.h"
#include"player_control.h"

static gboolean callback_list_release(GtkWidget *widget, GdkEventButton *event, gpointer data);


static PLAYER *_player = NULL;

static int _list_pos_y = LIST_START_POS_Y;
static int _list_pos_x = LIST_START_POS_X;
static double _press_x = 0;
static double _press_y = 0;


enum list_press_state {NOTHING, AROUND, FLUCTUATE, SWITCH};
static enum list_press_state _list_flag = NOTHING;


void player_song_cur_show_from_text(PLAYER *player, const char *text)
{
	gtk_label_set_text(GTK_LABEL(player->win.song), text);
}

void player_song_cur_display(PLAYER *player)
{
	const char *text = sungtk_clist_get_row_data(player->win.win_list, player->status.song_cur_num);
	player_song_cur_show_from_text(player, text);
	sungtk_clist_select_row(player->win.win_list, player->status.song_cur_num, LIST_FONT_BRIGHT_COLOR);
}

void player_current_time_display(PLAYER *player, const int time)
{
	gchar show_time[32] = "";
	snprintf(show_time, sizeof(show_time), "%02d:%02d", 
		time/60, time%60);

	gdk_threads_enter();
	gtk_label_set_text(GTK_LABEL(player->win.time), show_time);
	gdk_threads_leave();
}

void player_percent_display(PLAYER *player, const int percent)
{
	double per = (double)percent/100;
	gdk_threads_enter();
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(player->win.rate), per);
	gdk_threads_leave();
}

void player_song_length_display(PLAYER *player, const int length)
{
	gchar show_time[32] = "";
	snprintf(show_time, sizeof(show_time), "%02d:%02d", 
		length/60, length%60);

	gdk_threads_enter();
	gtk_label_set_text(GTK_LABEL(player->win.length), show_time);
	gdk_threads_leave();
}

void player_artist_display(PLAYER *player, const char *artist)
{
	char show_buf[128] = "";
	gb2312_to_utf8((unsigned char *)artist, (unsigned char *)show_buf);	

	gdk_threads_enter();
	gtk_label_set_text(GTK_LABEL(player->win.singer), show_buf);
	gdk_threads_leave();
}

void player_song_list_display(PLAYER *player)
{
	if(player->list.song_list != NULL)
		player->list.song_list = release_song_list(player->list.song_list, player->list.song_num);

	player->list.song_list = get_song_list(player->path.song_path, &player->list.song_num);

	int i;
	for(i=0;i<player->list.song_num;++i)
	{
		//printf("%s\n", player->list.song_list[i]);
		sungtk_clist_append(player->win.win_list, player->list.song_list[i]);
	}
}

static void player_pause_state_display(PLAYER *player)
{
	char *pic[2] = {IMAGE_BUTTON_PLAY_PATH, IMAGE_BUTTON_PAUSE_PATH};
	sungtk_button_set_image(player->win.pause, pic[player->status.ispause], 
		BUTTON_PAUSE_WIDTH, BUTTON_PAUSE_HEIGTH);
}



static void callback_pause(GtkWidget *widget, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	send_cmd_to_player(player, CMD_PAUSE, NULL);
	player_pause_state_display(player);
}

static void callback_next(GtkWidget *widget, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	send_cmd_to_player(player, CMD_FRONT, NULL);
	player_song_cur_display(player);
	player_pause_state_display(player);
}

static void callback_back(GtkWidget *widget, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	send_cmd_to_player(player, CMD_BACK, NULL);
	player_song_cur_display(player);
	player_pause_state_display(player);
}

static gboolean callback_press_rate(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	double right_press_x = event->x-(widget->allocation.width - player->win.rate->allocation.width)/2;
	int seek_num = player->status.song_length*right_press_x/player->win.rate->allocation.width - player->status.time_position;
	//printf("seek_num====%d\n", seek_num);
	send_cmd_to_player(player, CMD_FORWORD, &seek_num);
	return TRUE;
}


static void list_comeback(PLAYER *player)
{
	int list_min_y = A8_SCREEN_HEIGHT - player->win.win_list->vbox->allocation.height;
	if(_list_pos_y > LIST_START_POS_Y || list_min_y >= 0){
		_list_pos_y = LIST_START_POS_Y;
		gtk_fixed_move(GTK_FIXED(player->win.win_lrc_fixed), player->win.win_list->fixed, LIST_START_POS_X, _list_pos_y);
	}else if(_list_pos_y < (0-list_min_y)){
		_list_pos_y = list_min_y;
		gtk_fixed_move(GTK_FIXED(player->win.win_lrc_fixed), player->win.win_list->fixed, LIST_START_POS_X, _list_pos_y);
	}
}


static gboolean callback_list_release(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	list_comeback(_player);
	//printf("list press x==%lf, y===%lf\n", event->x, event->y);
	
	if(_list_flag == NOTHING){
		int row = (int)data;
		//printf("presss x==%lf, y===%lf, row====%d\n", event->x, event->y, row);
		send_cmd_to_player(_player, CMD_CHANGESONG, &row);
		player_song_cur_display(_player);
		player_pause_state_display(_player);
	}
	
	_list_flag = NOTHING;
	return TRUE;
}


static gboolean callback_list_motion_release(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	PLAYER *player = (PLAYER *)data;

	list_comeback(player);

	printf("motion release x==%lf, y===%lf\n", event->x, event->y);
	_list_flag = NOTHING;
	return TRUE;
}

static gboolean callback_list_motion_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	_press_x = event->x;
	_press_y = event->y;
	_list_flag = NOTHING;
	printf("motion press x==%lf, y===%lf\n", event->x, event->y);
	return TRUE;
}


static gboolean callback_list_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	double new_y = event->y;
	double new_x = event->x;

	//printf("motion x==%lf, y===%lf\n", event->x, event->y);

	if((_list_flag == NOTHING)){
		
		if(abs(new_y - _press_y) >= player->win.win_list->row_height){
			_list_flag = FLUCTUATE;
			printf("2222222\n");
		}else if(abs(new_x - _press_x) > 10){
			_list_flag = FLUCTUATE;
		}
	}
	if((_list_flag == FLUCTUATE) && (abs(new_y - _press_y) > 6)){
		_list_pos_y += (new_y > _press_y) ? 17 : -17;
		//printf("pos y===%d\n", _list_pos_y);
		_press_y = new_y;
		gtk_fixed_move(GTK_FIXED(player->win.win_lrc_fixed), player->win.win_list->fixed, _list_pos_x, _list_pos_y);
	}

	return TRUE;
}


static gboolean callback_volume_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	if(event->y > VOL_MUTE_POS_Y)
		gtk_widget_show_all(player->win.volume_rate_fixed);
	return TRUE;
}

static gboolean callback_volume_release(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	gtk_widget_hide_all(player->win.volume_rate_fixed);
	return TRUE;
}

static gboolean callback_volume_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	if(VOL_MUTE_POS_Y-event->y > 0){
		double percent = (double)(VOL_MUTE_POS_Y-event->y)/VOL_RATE_HEIGHT;
		int volume = percent*100;
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(player->win.volume_rate), percent);
		send_cmd_to_player(player, CMD_VOLUME, &volume);
	/*	if(volume > 2){
			sungtk_image_load_picture(player->win.mute, IMAGE_BUTTON_MUTE_PATH, 40, 40);
		}else{
			sungtk_image_load_picture(player->win.mute, IMAGE_BUTTON_MUTEOFF_PATH, 40, 40);
		}*/
	}

	return TRUE;
}

static void callback_exit(GtkWidget *widget, gpointer data)
{
	PLAYER *player = (PLAYER *)data;
	kill(player->process.pid, SIGKILL);
	//pid_t pid = waitpid(-1, NULL, WNOHANG);
	//printf("pid===%d\n", pid);
	gtk_main_quit();
}

void player_auto_play(PLAYER *player, int cmd)
{
	printf("cmd====%d\n", cmd);
	send_cmd_to_player(player, cmd, &player->status.volume);
	gdk_threads_enter();
	player_song_cur_display(player);
	player_pause_state_display(player);
	gdk_threads_leave();
}



void player_window_init(PLAYER *player)
{
	_player = player;
	
	/*创建背景窗口*/
	player->win.win_back = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(player->win.win_back), "Mplayer");/*设置标题*/
    gtk_widget_set_size_request(player->win.win_back, WIN_BACK_WIDTH, WIN_BACK_HEIGTH);/*设置窗口大小*/
    g_signal_connect(G_OBJECT(player->win.win_back), "destroy",
                        G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show(player->win.win_back);
	sungtk_background_set_picture(player->win.win_back, IMAGE_BACKGROUND_PATH, WIN_BACK_WIDTH, WIN_BACK_HEIGTH);

	//窗口固定布局
	player->win.win_back_fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(player->win.win_back), player->win.win_back_fixed);
	gtk_widget_show(player->win.win_back_fixed);

	/********创建背景表格布局*********/
	player->win.win_back_tabel = gtk_table_new(BACK_TABLE_ROW, BACK_TABLE_COL, TRUE);
	gtk_widget_set_size_request(player->win.win_back_tabel, WIN_BACK_WIDTH, WIN_BACK_HEIGTH);
	gtk_container_add(GTK_CONTAINER(player->win.win_back_fixed), player->win.win_back_tabel);
	gtk_widget_show(player->win.win_back_tabel);

	/*********创建退出按钮*********/
	player->win.button_exit = sungtk_button_new_with_image(IMAGE_BUTTON_EXIT_PATH, 50, 50);
	gtk_fixed_put(GTK_FIXED(player->win.win_back_fixed), player->win.button_exit, 750, 430);
	g_signal_connect(player->win.button_exit, "clicked", G_CALLBACK(callback_exit), player);

	//创建歌词和歌曲列表事件盒子
	player->win.win_lrc_eventbox = gtk_event_box_new();
	gtk_event_box_set_visible_window(GTK_EVENT_BOX(player->win.win_lrc_eventbox), FALSE);  
	gtk_widget_add_events(player->win.win_lrc_eventbox, GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_MOTION_MASK);
	g_signal_connect(player->win.win_lrc_eventbox, "motion-notify-event", G_CALLBACK(callback_list_motion), player);
	g_signal_connect(player->win.win_lrc_eventbox, "button-release-event", G_CALLBACK(callback_list_motion_release), player);
	g_signal_connect(player->win.win_lrc_eventbox, "button-press-event", G_CALLBACK(callback_list_motion_press), player);
	gtk_table_attach_defaults(GTK_TABLE(player->win.win_back_tabel), player->win.win_lrc_eventbox, 
		12, 20, 0, 20);
	

	//创建歌词歌曲列表存放的固定布局
	player->win.win_lrc_fixed = gtk_fixed_new();	
	gtk_container_add(GTK_CONTAINER(player->win.win_lrc_eventbox), player->win.win_lrc_fixed);
	gtk_widget_show(player->win.win_lrc_fixed);
	
	//歌曲列表滚动创建
	player->win.win_list = sungtk_clist_new();
	sungtk_clist_set_row_height(player->win.win_list, LIST_ROW_HEIGTH);
	sungtk_clist_set_col_width(player->win.win_list, LIST_COL_WIDTH);
	sungtk_clist_set_text_size(player->win.win_list, LIST_FONT_SIZE);
	sungtk_clist_set_select_row_signal(player->win.win_list, "button-release-event", callback_list_release);
	gtk_fixed_put(GTK_FIXED(player->win.win_lrc_fixed), player->win.win_list->fixed, LIST_START_POS_X, LIST_START_POS_Y);
	gtk_widget_show(player->win.win_list->fixed);


	//歌词显示label创建
	player->win.lrc_light = gtk_label_new("");
	sungtk_widget_set_font_size(player->win.lrc_light, LRC_FONT_SIZE, FALSE);
	sungtk_widget_set_font_color(player->win.lrc_light, LRC_FONT_COLOR, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(player->win.win_back_tabel), player->win.lrc_light, 
		1, 11, 12, 13);


	//设置进度条  
	GtkWidget *event_rate = gtk_event_box_new();//事件盒子，透明
	gtk_event_box_set_visible_window(GTK_EVENT_BOX(event_rate), FALSE);
	gtk_table_attach_defaults(GTK_TABLE(player->win.win_back_tabel), event_rate, 
		0, 12, 18, 19);
	g_signal_connect(event_rate, "button-press-event", G_CALLBACK(callback_press_rate), player);
	GtkWidget *rate_table = gtk_table_new(3, 20, TRUE);//进度条存放表格
	gtk_container_add(GTK_CONTAINER(event_rate), rate_table);
	
	player->win.rate = gtk_progress_bar_new();//进度条创建
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(player->win.rate), 0);
	gtk_table_attach_defaults(GTK_TABLE(rate_table), player->win.rate, 
		1, 19, 1, 2);

	//当前播放时间
	player->win.time = gtk_label_new("01:12");
	sungtk_widget_set_font_size(player->win.time, SONG_TIME_FONT_SIZE, FALSE);
	sungtk_widget_set_font_color(player->win.time, SONG_TIME_FONT_COLOR, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(player->win.win_back_tabel), player->win.time, 
		0, 2, 17, 18);

	//歌曲总长时间
	player->win.length = gtk_label_new("04:16");
	sungtk_widget_set_font_size(player->win.length, SONG_LENGTH_FONT_SIZE, FALSE);
	sungtk_widget_set_font_color(player->win.length, SONG_LENGTH_FONT_COLOR, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(player->win.win_back_tabel), player->win.length, 
		10, 12, 17, 18);


	//播放、暂停按钮
	player->win.pause = sungtk_button_new_with_image(IMAGE_BUTTON_PLAY_PATH, BUTTON_PAUSE_WIDTH, BUTTON_PAUSE_HEIGTH);
	gtk_table_attach_defaults(GTK_TABLE(player->win.win_back_tabel), player->win.pause, 
		5, 7, 14, 17);
	g_signal_connect(player->win.pause, "pressed", G_CALLBACK(callback_pause), player);

	//上一曲按钮
	player->win.next = sungtk_button_new_with_image(IMAGE_BUTTON_BACK_PATH, BUTTON_NEXT_WIDTH, BUTTON_NEXT_HEIGHT);
	gtk_table_attach_defaults(GTK_TABLE(player->win.win_back_tabel), player->win.next, 
		2, 4, 14, 17);
	g_signal_connect(player->win.next, "pressed", G_CALLBACK(callback_back), player);

	//下一曲按钮
	player->win.back = sungtk_button_new_with_image(IMAGE_BUTTON_FRONT_PATH, BUTTON_NEXT_WIDTH, BUTTON_NEXT_HEIGHT);
	gtk_table_attach_defaults(GTK_TABLE(player->win.win_back_tabel), player->win.back, 
		8, 10, 14, 17);
	g_signal_connect(player->win.back, "pressed", G_CALLBACK(callback_next), player);

	//歌名显示label创建
	player->win.song = gtk_label_new("");
	sungtk_widget_set_font_size(player->win.song, SONG_NAME_FONT_SIZE, FALSE);
	sungtk_widget_set_font_color(player->win.song, SONG_NAME_FONT_COLOR, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(player->win.win_back_tabel), player->win.song, 
		6, 12, 5, 6);

	//歌唱者显示label创建
	player->win.singer = gtk_label_new("mike");
	sungtk_widget_set_font_size(player->win.singer, SINGER_FONT_SIZE, FALSE);
	sungtk_widget_set_font_color(player->win.singer, SINGER_FONT_COLOR, FALSE);
	gtk_table_attach_defaults(GTK_TABLE(player->win.win_back_tabel), player->win.singer, 
		6, 12, 8, 9);

	//歌唱者头像
	player->win.image_artist = sungtk_image_new_from_file(IMAGE_MUSIC_COMMON_PATH, 150, 150);
	gtk_table_attach_defaults(GTK_TABLE(player->win.win_back_tabel), player->win.image_artist, 
		1, 5, 3, 9);

	//音量调节
	player->win.volume_event_box = gtk_event_box_new();
	gtk_event_box_set_visible_window(GTK_EVENT_BOX(player->win.volume_event_box), FALSE);
	gtk_table_attach_defaults(GTK_TABLE(player->win.win_back_tabel), player->win.volume_event_box, 
		10, 12, 12, 17);
	g_signal_connect(player->win.volume_event_box, "button-press-event", G_CALLBACK(callback_volume_press), player);
	g_signal_connect(player->win.volume_event_box, "button-release-event", G_CALLBACK(callback_volume_release), player);
	g_signal_connect(player->win.volume_event_box, "motion-notify-event", G_CALLBACK(callback_volume_motion), player);

	player->win.volume_fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(player->win.volume_event_box), player->win.volume_fixed);
	
	player->win.mute = sungtk_image_new_from_file(IMAGE_BUTTON_MUTE_PATH, VOL_MUTE_WIDTH, VOL_MUTE_HEIGHT);
	gtk_fixed_put(GTK_FIXED(player->win.volume_fixed), player->win.mute, 20, VOL_MUTE_POS_Y);

	player->win.volume_rate_fixed = gtk_fixed_new();
	gtk_fixed_put(GTK_FIXED(player->win.volume_fixed), player->win.volume_rate_fixed, 34, 0);

	player->win.volume_rate = gtk_progress_bar_new();
	gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(player->win.volume_rate), GTK_PROGRESS_BOTTOM_TO_TOP);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(player->win.volume_rate), 0.3);
	gtk_widget_set_size_request(player->win.volume_rate, VOL_RATE_WIDTH, VOL_RATE_HEIGHT);
	gtk_fixed_put(GTK_FIXED(player->win.volume_rate_fixed), player->win.volume_rate, 0, 0);
}


/********gtk线程初始化*************/
void thread_gtk_init(void)
{
  	if( FALSE == g_thread_supported() ){  // 如果线程没有初始化
		g_thread_init(NULL); // 初始化线程，g_thread_init()必须放在gdk_threads_init()前面
	} 
	gdk_threads_init();
}

