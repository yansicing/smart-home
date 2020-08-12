SRC := #
#SRC += send_message.c
SRC += uart_init.c
SRC += gprs_config.c
SRC += gb2312_ucs2.c
#OBJ := $(subst .c,.o,$(SRC))
OBJ = $(SRC:%.c=%.o)


CC = arm-linux-gcc
FLAG = -Wall 
OPTION = -lpthread
EXEC_NAME = send_message.o
EXEC_PATH = ./

.PHONY:clean call

send_message:$(OBJ)
	@echo make ...
	#$(CC) -c $(OBJ) -o send_message.o $(FLAG) $(OPTION)
	#$(CC) $^ -o $(EXEC_PATH)/$(EXEC_NAME) $(FLAG) $(OPTION)
	@echo make over
	@echo Execute target is $(EXEC_PATH)/$(EXEC_NAME)
%.o:%.c
	$(CC) -c $< -o $@ $(FLAG) $(OPTION)
clean:
	@echo clean ...
	rm $(EXEC_PATH)/$(EXEC_NAME) *.o -rf
	@echo clean over
	
