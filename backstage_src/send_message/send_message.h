#ifndef __SEND_MESSAGE_H__
#define __SEND_MESSAGE_H__

#define S3C2410_SERIAL0 0
#define S3C2410_SERIAL1 1 
#define S3C2410_SERIAL2 2
#define S3C2410_SERIAL3 3

#define GPRS_SEND_MSG	1
#define GPRS_CALL		2
#define GPRS_HANG_UP	3

#define SEND_MSG_UTF8_TO_UCS2	0
#define SEND_MSG_GB2312_TO_UCS2	1


extern int gprs_hang_up();

extern int gprs_control(char *data, char *return_data);

#endif