#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main()
{
	mkfifo("../fifo/chess_recv_fifo", 0666);
	int fd = open("../fifo/chess_recv_fifo", O_RDWR);
	if(fd < 0){
		perror("open error");
		return -1;
	}
	while(1)
	{
		char buf[100] = "";
		gets(buf);
		int len = write(fd, buf, strlen(buf));
		printf("len===%d\n", len);
	}
	
	close(fd);
	return 0;
}