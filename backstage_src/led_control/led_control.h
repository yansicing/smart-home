#ifndef __LED_CONTROL_H__
#define __LED_CONTROL_H__

#define GPIO_SET_PIN_OUT 1

#define LED_SET_FLAG 1
#define LED_GET_FALG 2


extern int gpio_open();
extern void gpio_init(int led_fd);
extern int set_led(int num);
extern int get_led_status(int fd);

extern int set_led_control(int status);

extern int get_led_control();

extern int led_control(char *bag, char *recv_bag);

#endif