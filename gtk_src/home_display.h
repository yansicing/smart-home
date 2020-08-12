#ifndef _HOME_DISPLAY_H
#define _HOME_DISPLAY_H

//发送命令给后台
extern int send_cmd_to_backstage(int cmd, const char *other_data);

//发送命令给后台控制led灯
extern int send_cmd_get_led_status(void);

//温湿度获取线程
extern void *pthread_temp_humi_get(void *arg);

//gtk获取后台数据线程
extern void *pthread_recv_data(void *arg);

#endif
