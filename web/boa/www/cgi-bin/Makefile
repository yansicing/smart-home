src += deal_cmd.c
src += ./common/get_browser_data.c
src += ./common/agreement.c
target = mplayer.cgi

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

