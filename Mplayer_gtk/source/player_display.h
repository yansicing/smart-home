#ifndef _PLAYER_DISPLAY_H_
#define _PLAYER_DISPLAY_H_

#include"types.h"

#define WIN_BACK_WIDTH 800
#define WIN_BACK_HEIGTH 480

#define BACK_TABLE_ROW 20	/*背景表格布局行数*/
#define BACK_TABLE_COL 20	/*背景表格布局列数*/

#define IMAGE_BACKGROUND_PATH "../picture/background/11.jpg"

#define IMAGE_BUTTON_PAUSE_PATH "../picture/button_style/play.png"
#define IMAGE_BUTTON_PLAY_PATH "../picture/button_style/pause.png"
#define IMAGE_BUTTON_BACK_PATH "../picture/button_style/back.png"
#define IMAGE_BUTTON_FRONT_PATH "../picture/button_style/front.png"
#define IMAGE_BUTTON_EXIT_PATH "../picture/button_style/exit.jpg"


#define IMAGE_MUSIC_COMMON_PATH "../picture/music.png"
#define IMAGE_LRC_BACK_PATH "../picture/music.png"
#define IMAGE_LIST_BUTTON_SHOW_PATH "../picture/jiantou.jpg"



#define BUTTON_PAUSE_WIDTH 70
#define BUTTON_PAUSE_HEIGTH 70
#define BUTTON_NEXT_WIDTH 70
#define BUTTON_NEXT_HEIGHT 70

#define LIST_ROW_HEIGTH 40
#define LIST_COL_WIDTH 210
#define LIST_FONT_SIZE 13
#define LIST_FONT_BRIGHT_COLOR "white"
#define LIST_START_POS_X 30
#define LIST_START_POS_Y 5

#define LRC_STATR_POS_X 30
#define LRC_STATR_POS_Y 10

#define LRC_FONT_SIZE 15
#define LRC_FONT_COLOR "white"

#define SONG_NAME_FONT_SIZE 16
#define SONG_NAME_FONT_COLOR "white"

#define SINGER_FONT_SIZE 16
#define SINGER_FONT_COLOR "white"

#define SONG_TIME_FONT_SIZE 12
#define SONG_TIME_FONT_COLOR "black"

#define SONG_LENGTH_FONT_SIZE 12
#define SONG_LENGTH_FONT_COLOR "black"


#define IMAGE_BUTTON_MUTE_PATH "../picture/mute.jpg"
#define IMAGE_BUTTON_MUTEOFF_PATH "../picture/muteoff.jpg"
#define VOL_RATE_WIDTH 7
#define VOL_RATE_HEIGHT 67
#define VOL_MUTE_WIDTH 40
#define VOL_MUTE_HEIGHT 40
#define VOL_MUTE_POS_Y 65


extern void player_song_cur_display(PLAYER *player);

extern void player_current_time_display(PLAYER *player, const int time);

extern void player_percent_display(PLAYER *player, const int percent);

extern void player_song_length_display(PLAYER *player, const int length);

extern void player_artist_display(PLAYER *player, const char *artist);

extern void player_song_list_display(PLAYER *player);

extern void player_auto_play(PLAYER *player, int cmd);


extern void player_window_init(PLAYER *player);




extern void thread_gtk_init(void);

#endif
