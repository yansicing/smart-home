src += main.c
src += thread_pool.c
src += receive_bag.c
src += agreement.c
src += ./login/login.c
src += ./led_control/led_control.c
src += ./get_humiture/get_humiture.c

src += ./send_message/gprs_config.c
src += ./send_message/uart_init.c
src += ./send_message/gb2312_ucs2.c
src += ./send_message/send_message.c
#src += ./send_msg_encoding/gb2312_ucs2.c

src += ./deal_IP/deal_IP.c
src += ./mplayer_control/mplayer_control.c
src += ./mplayer_control/deal_song_list.c
src += ./infrared/infrared.c
src += ./gpio_red/gpio_red.c
target = ../execute/home_back

cc  = arm-linux-gcc 
cflags = -lpthread -Wall

obj = $(src:%.c=%.o)

$(target):$(obj)
	$(cc) $^ -o $@ $(cflags)
%.o:%.c
	$(cc) -c $< -o $@ $(cflags)
	
.phony:clean
clean:
	rm *.o	$(target) -fr

