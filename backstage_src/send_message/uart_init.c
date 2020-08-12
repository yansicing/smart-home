#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <sys/select.h>

static struct termios option_old;

/*************************************************************
* 功能：	打开串口设备文件
* 参数：	串口设备文件名
* 返回值：	串口设备文件描述符
**************************************************************/
static int uart_open(char *dev_name)
{
	int uart_fd;
	
	uart_fd = open(dev_name, O_RDWR);
	if(uart_fd < 0)
	{
		perror("open_dev");
		_exit(-1);
	}
	return uart_fd;
}

void uart_print_attr(struct termios *options)
{
	printf("c_iflag = %#x\n", options->c_iflag);
	printf("c_oflag = %#x\n", options->c_oflag);
	printf("c_cflag = %#x\n", options->c_cflag);
	printf("c_lflag = %#x\n", options->c_lflag);
	int i;
	for(i=VINTR;i<=VEOL2;i++)
	{
		printf("c_cc[%d] = %d\n", i, options->c_cc[i]);
	}
}

/*************************************************************
* 功能：	串口默认属性
* 参数：	无
* 返回值：	无
**************************************************************/
static struct termios *uart_default_attr(void)
{
	struct termios *options = NULL;
	
	options = (struct termios *)calloc(1, sizeof(struct termios));
	if(options == NULL)
	{
		perror("calloc");
		_exit(-1);
	}
	/*
		IXOFF：使起动/停止输入控制流起作用
		IXON：使起动/停止输出控制流起作用
		ICRNL：将输入的CR转换为NL
	*/
	options->c_iflag |= IXOFF|IXON|ICRNL;
	/*
		ONLCR：将NL转换为CR-NL
		OPOST：执行输出处理
	*/
	options->c_oflag = ONLCR|OPOST;
	/*
		CSIZE：设置数据位时先关闭历史设置
		B115200：波特率为115200
		CS8：数据宽度为8位
		CREAD：启用接收装置，可以接收字符
		CLOCAL：忽略调制解调器状态行，否则一直阻塞到和调制解调器建立连接
	*/
	options->c_cflag &= ~CSIZE;	
	options->c_cflag = B115200|CS8|CREAD|CLOCAL;
	
	/*
		ICANON：标准模式
		ISIG：启用终端产生的信号
		IEXTEN：启用扩充的输入字符处理
		ECHOKE：kill的可见擦除
		ECHOCTL：回显控制字符为^(Char) 
		ECHOK：回显kill符
		ECHOE：可见擦除符
		ECHO：回显
	*/
	//options->c_lflag = ICANON|ISIG|IEXTEN|ECHOKE|ECHOCTL|ECHOK|ECHOE|ECHO;
	options->c_lflag = ISIG|IEXTEN|ECHOKE|ECHOCTL|ECHOK|ECHOE;
	options->c_cc[VINTR]    = 3;	/* ^c：Ctrl+c，中断信号 */ 
	options->c_cc[VQUIT]    = 28;	/* ^\：Ctrl+\，退出信号 */
	options->c_cc[VERASE]   = 127;	/* ^H：Backspace，向前擦除一个字符 */
	options->c_cc[VKILL]    = 21;	/* ^U：，擦行 */
	options->c_cc[VEOF]     = 4;	/* ^D：Ctrl+d，文件结束 */
	options->c_cc[VTIME]    = 0;	/* 设置超时计时器，单位：deciseconds十分之一秒 */
	options->c_cc[VMIN]     = 1;	/* 串口的缓冲区的大小 */
	options->c_cc[VSWTC]    = 0;	/* '\0' */
	options->c_cc[VSTART]   = 17;	/* ^Q：Ctrl+q，恢复输出 */ 
	options->c_cc[VSTOP]    = 19;	/* ^S：Ctrl+s，停止输出 */
	options->c_cc[VSUSP]    = 26;	/* ^Z：Ctrl+z，挂起信号（SIGTSTP） */
	options->c_cc[VEOL]     = 0;	/* '\0'，行结束 */
	options->c_cc[VREPRINT] = 18;	/* ^R：Ctrl-r，再打印全部输入 */
	options->c_cc[VDISCARD] = 15;	/* ^O：Ctrl-u，擦除全部输出 */
	options->c_cc[VWERASE]  = 23;	/* ^W：Ctrl-w，擦除一个字 */
	options->c_cc[VLNEXT]   = 22;	/* ^V：Ctrl-v，下一个字面字符 */
	options->c_cc[VEOL2]    = 0;	/* 供替换的行结束 */
	
	//uart_print_attr(options);
	return options;
}

/*************************************************************
* 功能：	设置串口属性结构体
* 参数：	fd：串口设备文件描述符
			speed：串口波特率（可选2400、4800、9600、115200。可自己酌情修改程序添加支持）
			data_bits：数据位宽（可选5、6、7、8）
			stop_bits：停止位（可选1、2）
			check：奇偶校验位（可选'N'、'O'、'E'；分别应着无奇偶校验、奇校验、偶校验）
			options：串口属结构体，
* 返回值：	串口属性结构体
**************************************************************/
struct termios * uart_set_attr(int fd,
					int speed,
					int data_bits,
					int stop_bits,
					int check, 
					struct termios *options)
{
	int baud_rate = 0;
	
	if(options == NULL)
	{
		options = uart_default_attr();
	}
	//options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|IGNCR|ICRNL|IXON);
	/***********数据位选择****************/      
	switch(data_bits)     
	{   
		case 5:     
			options->c_cflag |= CS5;     
			break; 
		case 6:     
			options->c_cflag |= CS6;     
			break; 			
		case 7:     
			options->c_cflag |= CS7;     
			break;     
		case 8:     
			options->c_cflag |= CS8;     
			break;
		default:
			options->c_cflag |= CS8;
			break;
	}
	/***********校验位选择****************/    
	switch(check)     
	{     
		case 'O':
			options->c_cflag |= PARENB;//允许输出产生奇偶信息以及输入的奇偶校验
			options->c_iflag |= (INPCK|ISTRIP);//INPCK:启用输入奇偶检测;ISTRIP:去掉第八位(传输时只传7位)
			options->c_cflag |= PARODD;//输入和输出是奇校验(ODD)
			break;     
		case 'E':
			options->c_cflag |= PARENB;
			options->c_iflag |= (INPCK|ISTRIP);
			options->c_cflag &= ~PARODD;//输入和输出是偶校验(ECC)
			break;     
		case 'N':     
			options->c_cflag &= ~PARENB;//无奇偶校验位
	}
	/***********停止位选择****************/    
	if(stop_bits == 1)
	{     
		options->c_cflag &= ~CSTOPB;     
	}     
	else if(stop_bits ==2)
	{     
		options->c_cflag |= CSTOPB;     
	}
	/***********波特率选择****************/     
	switch(speed)     
	{
		case 0:     
			baud_rate = B0;    
			break;
		case 50:     
			baud_rate = B50;    
			break;
		case 75:     
			baud_rate = B75; 
			break;
		case 110:     
			baud_rate = B110; 
			break;
		case 134:     
			baud_rate = B134; 
			break;
		case 150:     
			baud_rate = B150; 
			break;
		case 200:     
			baud_rate = B200; 
			break;
		case 300:     
			baud_rate = B300; 
			break;
		case 600:     
			baud_rate = B600; 
			break;
		case 1200:     
			baud_rate = B1200; 
			break;
		case 1800:     
			baud_rate = B1800; 
			break;
		case 2400:     
			baud_rate = B2400; 
			break;
		case 4800:     
			baud_rate = B4800; 
			break;
		case 9600:     
			baud_rate = B9600; 
			break;
		case 19200:     
			baud_rate = B19200; 
			break;
		case 38400:     
			baud_rate = B38400; 
			break;
		case 57600:     
			baud_rate = B57600; 
			break;
		case 115200:     
			baud_rate = B115200; 
			break;
		case 230400:     
			baud_rate = B230400; 
			break;
		case 460800:     
			baud_rate = B460800; 
			break;
		case 500000:     
			baud_rate = B500000; 
			break;
		case 576000:     
			baud_rate = B576000; 
			break;
		case 921600:     
			baud_rate = B921600; 
			break;
		case 1000000:     
			baud_rate = B1000000; 
			break;
		case 1152000:     
			baud_rate = B1152000; 
			break;
		case 1500000:     
			baud_rate = B1500000; 
			break;
		case 2000000:     
			baud_rate = B2000000; 
			break;
		case 2500000:     
			baud_rate = B2500000; 
			break;
		case 3000000:     
			baud_rate = B3000000; 
			break;
		case 3500000:     
			baud_rate = B3500000; 
			break;
		case 4000000:     
			baud_rate = B4000000; 
			break;
		default:     
			baud_rate = B9600;    
			break;
	}
	cfsetispeed(options, baud_rate);     
	cfsetospeed(options, baud_rate);  
	tcflush(fd, TCIFLUSH);
	// fcntl(fd, F_SETFL, O_NONBLOCK);	//read时不阻塞
	fcntl(fd, F_SETFL, 0);			//read时阻塞
	return options;
}
//---------------------------------------------------------
//---------------------------------------------------------
int Uart_Init(int num)
{
	int fd;
	struct termios options;
	// char *uart[3]={"/dev/s3c2410_serial0","/dev/s3c2410_serial1","/dev/s3c2410_serial2"};
	char *uart[3]={"/dev/s3c2410_serial0","/dev/s3c2410_serial1","/dev/ttyS0"};
	fd = open(uart[num], O_RDWR);
	printf("uart_fd=%d************************************************\n",fd);
	if(fd<0)
		perror("open !!!!!!");
	tcgetattr(fd, &options);
	
	cfsetispeed(&options, B115200);//uart in speed
	cfsetospeed(&options, B115200);//uart out speed
	//cfsetispeed(&options, B9600);//uart in speed
	//cfsetospeed(&options, B9600);//uart out speed

	options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|IGNCR|ICRNL|IXON);
	//
	options.c_cflag &= ~PARENB; //无奇偶校验位
	options.c_cflag &= ~CSTOPB;	//停止位为1位
	options.c_cflag &= ~CSIZE;	
	options.c_cflag |= CS8;		//数据位为8位
	
	options.c_lflag   &=   ~(ICANON   |   ECHO   |   ECHOE   |   ISIG)   ;
	
	tcsetattr(fd,TCSANOW,&options);
	return fd;
}
/*************************************************************
* 功能：	串口初始化程序
* 参数：	串口设备文件名
* 返回值：	串口设备文件描述符
**************************************************************/
int uart_init(char *devname)
{
	int uart_fd;
	struct termios *options;

	uart_fd = uart_open(devname);
	tcgetattr(uart_fd, &option_old);	//保存串口属性
	options = uart_set_attr(uart_fd, 115200, 8, 1, 'N', NULL);

	//options->c_lflag |= ECHO;				/* 开回显 */
	options->c_lflag &= ~ECHO;				/* 关回显 */
	tcsetattr(uart_fd, TCSANOW,  options);	/* 设置串口属性 */
	free(options);
	return uart_fd;
}

/*************************************************************
* 功能：	串口反初始化程序
* 参数：	串口设备文件描述符
* 返回值：	无
**************************************************************/
void uart_uninit(int uart_fd)
{
	/*还原串口属性*/
	tcsetattr(uart_fd, TCSANOW, &option_old);
	
	/*关闭串口*/
	close(uart_fd);
}

/*************************************************************
* 功能：	串口发送字符串
* 参数：	uart_fd：串口设备文件描述符
			str：待发送的字符
* 返回值：	无
**************************************************************/
void uart_send_str(int uart_fd, char *str)
{
	int ret;
	
	ret = write(uart_fd, str, strlen(str));
	if(ret < 0)
	{
		perror("write");
	}
}

/*************************************************************
* 功能：	串口读一行字符串，在设定的时间内读不到数据则函数返回
* 参数：	uart_fd：串口设备文件描述符
			buffer：存放数据的内存的首地址
			len：存放数据的内存空间的大小
			timeout_ms：超时时间(单位：ms)
* 返回值：	
			成功：实际读到的字符数
			失败：-1
**************************************************************/
int uart_readline(int uart_fd, char *buffer, int len, int timeout_ms)
{
	char c = '\0';
	fd_set fds;
	struct timeval tv;
	int i;
	int ret;

	memset(buffer, 0, len);
	for(i=0; i<len && c != '\n'; i++){
		tv.tv_sec = 0;
		tv.tv_usec = timeout_ms*1000;
		FD_ZERO(&fds);
		FD_SET(uart_fd, &fds);
		ret = select(FD_SETSIZE, &fds, NULL, NULL, &tv);
		if(ret < 0){
			perror("seclect");
			return -1;
		}else if(ret > 0){
			ret = read(uart_fd, &c, 1);
			if(ret < 0)
			{
				perror("read");
			}
		}else{
			return -1;
		}
		*buffer++ = c;
		//printf("c=%c\n", c);
	}
	return i;
}

/*************************************************************
* 功能：	
* 参数：	uart_fd：串口设备文件描述符
			buffer：存放数据的内存的首地址
			buffer_size 数据大小			
* 返回值：	
			成功：实际写入的字符数
			失败：-1
**************************************************************/
int uart_send(int uart_fd, const void * buffer, int buffer_size)
{
	printf("1buffer = %s",(char *)buffer);
	if(write(uart_fd, buffer, buffer_size)==buffer_size)
	{
		printf("2buffer = %s",(char *)buffer);
		return buffer_size;
	}
	else
	{
		return -1;
	}
	
}
