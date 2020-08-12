#ifndef _GET_IP_H
#define _GET_IP_H

#define GET_IP_FLAG 1
#define SET_IP_FLAG 2

extern int get_ip(char *ip);
extern int set_auto_ip(char *ip);
extern int set_hand_ip(const char *ip);
extern int check_right_ip(const char *ip);

extern int deal_ip(char *data, char *return_data);

#endif
