src += main.c
src += home_init.c
src += home_display.c
src += agreement.c
src += ./sungtk_interface/sungtk_interface.c
src += ./keyboard/gtk_keyboard.c
#src += ./encoding/gb2312_ucs2.c
target = ../execute/home_gtk 

cc  = arm-linux-gcc 
cflags = -lpthread -Wall -g -lgthread-2.0 `pkg-config --cflags --libs gtk+-2.0` `pkg-config --cflags --libs libglade-2.0`
prefix = /opt/gtkdfb
ldflags = -L$(prefix)/lib -Wl,-rpath,$(prefix)/lib 
cppflags = -I$(prefix)/include/gtk-2.0/ 

export PKG_CONFIG_PATH = /opt/gtkdfb/lib/pkgconfig

obj = $(src:%.c=%.o)

$(target):$(obj)
	$(cc) $^ -o $@ $(cflags) $(ldflags)
%.o:%.c
	$(cc) -c $< -o $@ $(cflags) $(cppflags)
	
.phony:clean
clean:
	rm *.o	$(target) -fr

