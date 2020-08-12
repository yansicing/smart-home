cc = gcc  
src = main.c
src += sungtk_interface.c
target = demo

obj = $(src:%.c=%.o)

cflags = `pkg-config --cflags --libs gtk+-2.0` -Wall

$(target):$(obj)
	$(cc) $^ -o $@ $(cflags)
%.o:%.c
	$(cc) -c $< -o $@  $(cflags)
clean:
	rm *.o $(targer) -rf
