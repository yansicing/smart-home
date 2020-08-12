#ifndef __GET_HUMITURE_H__
#define __GET_HUMITURE_H__

#define HUMITURE_GET	1

extern int am2311_open();
extern int read_am2311_data(const int fd, float *temp, float *humi);
extern int get_temp_and_humi(float *temp, float *humi);
extern int get_humiture(char *bag, char *recv_bag);

#endif