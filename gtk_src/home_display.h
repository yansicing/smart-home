#ifndef _HOME_DISPLAY_H
#define _HOME_DISPLAY_H

//�����������̨
extern int send_cmd_to_backstage(int cmd, const char *other_data);

//�����������̨����led��
extern int send_cmd_get_led_status(void);

//��ʪ�Ȼ�ȡ�߳�
extern void *pthread_temp_humi_get(void *arg);

//gtk��ȡ��̨�����߳�
extern void *pthread_recv_data(void *arg);

#endif
