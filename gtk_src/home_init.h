#ifndef _HOME_INIT_H
#define _HOME_INIT_H

#define WIN_BACK_TABLE_ROW 5
#define WIN_BACK_TABLE_COL 15

//glade中名字
#define GLADE_FILE "../gtk_glade/SmartHome.glade"
#define WIN_BACK "window_back"
#define WIN_EVENT_BOX "eventbox_back"
#define FIXED_BACK "fixed_back"
#define NOTEBOOK_BACK "notebook_back"
#define FIXED_PAGE0 "fixed_page0"
#define FIXED_PAGE1 "fixed_page1"
#define FIXED_PAGE2 "fixed_page2"
#define FIXED_PAGE3 "fixed_page3"
#define FIXED_PAGE4 "fixed_page4"
#define FIXED_PAGE5 "fixed_page5"
#define FIXED_PAGE6 "fixed_page6"
#define FIXED_PAGE7 "fixed_page7"
#define FIXED_PAGE8 "fixed_page8"


//screen
#define SCREEN_BRIGHTNEWW "/sys/devices/platform/s3c24xx-pwm.3/pwm-backlight/backlight/pwm-backlight/brightness"
#define SCREEN_BL_POWER "/sys/devices/platform/s3c24xx-pwm.3/pwm-backlight/backlight/pwm-backlight/bl_power"
#define SCREEN_ASH_BRIGHTNESS_NUM 80
#define SCREEN_BREGHTNESS_FULLLIGHT_NUM 255
#define SCREEN_CHANGE_ASH_SECONDS 5
#define SCREEN_ALARM_SECONDS 1000

//主页
#define BACK_BUTTON_WIDTH 80
#define BACK_BUTTON_HEIGHT 80
#define BACK_HOME_BUTTON_WIDTH 40
#define BACK_HOME_BUTTON_HEIGHT 40
#define BACK_HOME_PIC "../picture/background/home_back.jpg"
#define BACK_HOME_BUTTON_PIC "../picture/logo_button.png"
#define BACK_LED_PIC "../picture/light.png"
#define BACK_TEMP_HUMI_PIC "../picture/temp.png"
#define BACK_GPRS_PIC "../picture/phone.png"
#define BACK_MPLAYER_PIC "../picture/mplayer.png"
#define BACK_CALENDAR_PIC "../picture/calendar.jpg"
#define BACK_GAME_PIC "../picture/game1.jpg"
#define BACK_CALC_PIC "../picture/calc.png"

//小按钮
#define LITTLE_BUTTON_WIDTH 70
#define LITTLE_BUTTON_HEIGHT 70
#define LITTLE_WIN_WIDTH 250
#define LITTLE_WIN_HEIGHT 250
#define LITTLE_TO_HOME_PIC "../picture/home_button.png"
#define LITTLE_TO_LOCK_PIC "../picture/lock_button.png"
#define LITTLE_TO_EXIT_PIC "../picture/exit_close.png"

//第0页锁屏
#define MOTIONLOCK_SIZE 320
#define MOTIONLOCK_IMG_SIZE 65
#define MOTIONLOCK_LABEL_SIZE 15
#define MOTIONLOCK_IMAGE_PIC "../picture/background/lock_back.jpg"
#define MOTIONLOCK_PRESS_PIC "../picture/press.png"
#define MOTIONLOCK_UNPRESS_PIC "../picture/unpress.png"
#define HAND_PWD "14789"



//第1页led灯
#define LED_BUTTON_WIDTH 80
#define LED_BUTTON_HEIGHT 80
#define LED_BUTTON_POS_Y 150
#define LED_IMAGE_PIC "../picture/background/led_back.png"
#define LED_BUTTON_LIGHT_PIC "../picture/light.png"
#define LED_BUTTON_LIGHTOFF_PIC "../picture/lightoff.png"


//第2页温湿度
#define TEMP_PIC_WIDTH 80
#define TEMP_PIC_HEIGHT 80
#define TEMP_FONT_SIZE 15
#define TEMP_FONT_COLOR "black"
#define TEMP_IMAGE_PIC "../picture/background/temp_back.jpg"
#define TEMP_PIC "../picture/temp.png"
#define TEMP_WARN_PIC "../picture/temp_warn.png"
#define HUMI_PIC "../picture/hum.png"
#define HUMI_WARN_PIC "../picture/hum_warn.png"


//第3页GPRS
#define GPRS_IMAGE_PIC "../picture/background/send_msg_back.png"
#define GPRS_SEND_PIC "../picture/send_1.jpg"
#define GPRS_CALL_PIC "../picture/call_1.jpg"
#define GPRS_SEND_WIDTH 80
#define GPRS_SEND_HEIGHT 50
#define GPRS_LABEL_FONT_SIZE 13
#define GPRS_LABEL_FONT_COLOR "balck"
#define GPRS_BOARD_WIDTH 450
#define GPRS_BOARD_HEIGHT 210


//第四页mplayer
#define MPALYER_EXE_PATH "./play"


//第五页calendar
#define CALENDAR_IMAGE_PATH "../picture/background/calendar_back.jpg"
#define CALENDAR_IMAGE_WIDTH 800
#define CALENDAR_IMAGE_HEIGHT 480


//第六页game
#define GAME_EXE_PATH "../chess_gtk/chess_a8"


//第七页calc
#define CALC_EXE_PATH "../calc_gtk/calc"

//子进程介绍处理，回收资源
extern void son_process_killed_deal(int signum);

//根据后台命令，启动mplayer
extern void start_mplayer_with_cmd(smarthome_t *home);

//窗口初始化
extern void win_back_init(smarthome_t *home);


#endif
