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
* ���ܣ�	�򿪴����豸�ļ�
* ������	�����豸�ļ���
* ����ֵ��	�����豸�ļ�������
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
* ���ܣ�	����Ĭ������
* ������	��
* ����ֵ��	��
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
		IXOFF��ʹ��/ֹͣ���������������
		IXON��ʹ��/ֹͣ���������������
		ICRNL���������CRת��ΪNL
	*/
	options->c_iflag |= IXOFF|IXON|ICRNL;
	/*
		ONLCR����NLת��ΪCR-NL
		OPOST��ִ���������
	*/
	options->c_oflag = ONLCR|OPOST;
	/*
		CSIZE����������λʱ�ȹر���ʷ����
		B115200��������Ϊ115200
		CS8�����ݿ��Ϊ8λ
		CREAD�����ý���װ�ã����Խ����ַ�
		CLOCAL�����Ե��ƽ����״̬�У�����һֱ�������͵��ƽ������������
	*/
	options->c_cflag &= ~CSIZE;	
	options->c_cflag = B115200|CS8|CREAD|CLOCAL;
	
	/*
		ICANON����׼ģʽ
		ISIG�������ն˲������ź�
		IEXTEN����������������ַ�����
		ECHOKE��kill�Ŀɼ�����
		ECHOCTL�����Կ����ַ�Ϊ^(Char) 
		ECHOK������kill��
		ECHOE���ɼ�������
		ECHO������
	*/
	//options->c_lflag = ICANON|ISIG|IEXTEN|ECHOKE|ECHOCTL|ECHOK|ECHOE|ECHO;
	options->c_lflag = ISIG|IEXTEN|ECHOKE|ECHOCTL|ECHOK|ECHOE;
	options->c_cc[VINTR]    = 3;	/* ^c��Ctrl+c���ж��ź� */ 
	options->c_cc[VQUIT]    = 28;	/* ^\��Ctrl+\���˳��ź� */
	options->c_cc[VERASE]   = 127;	/* ^H��Backspace����ǰ����һ���ַ� */
	options->c_cc[VKILL]    = 21;	/* ^U�������� */
	options->c_cc[VEOF]     = 4;	/* ^D��Ctrl+d���ļ����� */
	options->c_cc[VTIME]    = 0;	/* ���ó�ʱ��ʱ������λ��decisecondsʮ��֮һ�� */
	options->c_cc[VMIN]     = 1;	/* ���ڵĻ������Ĵ�С */
	options->c_cc[VSWTC]    = 0;	/* '\0' */
	options->c_cc[VSTART]   = 17;	/* ^Q��Ctrl+q���ָ���� */ 
	options->c_cc[VSTOP]    = 19;	/* ^S��Ctrl+s��ֹͣ��� */
	options->c_cc[VSUSP]    = 26;	/* ^Z��Ctrl+z�������źţ�SIGTSTP�� */
	options->c_cc[VEOL]     = 0;	/* '\0'���н��� */
	options->c_cc[VREPRINT] = 18;	/* ^R��Ctrl-r���ٴ�ӡȫ������ */
	options->c_cc[VDISCARD] = 15;	/* ^O��Ctrl-u������ȫ����� */
	options->c_cc[VWERASE]  = 23;	/* ^W��Ctrl-w������һ���� */
	options->c_cc[VLNEXT]   = 22;	/* ^V��Ctrl-v����һ�������ַ� */
	options->c_cc[VEOL2]    = 0;	/* ���滻���н��� */
	
	//uart_print_attr(options);
	return options;
}

/*************************************************************
* ���ܣ�	���ô������Խṹ��
* ������	fd�������豸�ļ�������
			speed�����ڲ����ʣ���ѡ2400��4800��9600��115200�����Լ������޸ĳ������֧�֣�
			data_bits������λ����ѡ5��6��7��8��
			stop_bits��ֹͣλ����ѡ1��2��
			check����żУ��λ����ѡ'N'��'O'��'E'���ֱ�Ӧ������żУ�顢��У�顢żУ�飩
			options���������ṹ�壬
* ����ֵ��	�������Խṹ��
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
	/***********����λѡ��****************/      
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
	/***********У��λѡ��****************/    
	switch(check)     
	{     
		case 'O':
			options->c_cflag |= PARENB;//�������������ż��Ϣ�Լ��������żУ��
			options->c_iflag |= (INPCK|ISTRIP);//INPCK:����������ż���;ISTRIP:ȥ���ڰ�λ(����ʱֻ��7λ)
			options->c_cflag |= PARODD;//������������У��(ODD)
			break;     
		case 'E':
			options->c_cflag |= PARENB;
			options->c_iflag |= (INPCK|ISTRIP);
			options->c_cflag &= ~PARODD;//����������żУ��(ECC)
			break;     
		case 'N':     
			options->c_cflag &= ~PARENB;//����żУ��λ
	}
	/***********ֹͣλѡ��****************/    
	if(stop_bits == 1)
	{     
		options->c_cflag &= ~CSTOPB;     
	}     
	else if(stop_bits ==2)
	{     
		options->c_cflag |= CSTOPB;     
	}
	/***********������ѡ��****************/     
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
	// fcntl(fd, F_SETFL, O_NONBLOCK);	//readʱ������
	fcntl(fd, F_SETFL, 0);			//readʱ����
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
	options.c_cflag &= ~PARENB; //����żУ��λ
	options.c_cflag &= ~CSTOPB;	//ֹͣλΪ1λ
	options.c_cflag &= ~CSIZE;	
	options.c_cflag |= CS8;		//����λΪ8λ
	
	options.c_lflag   &=   ~(ICANON   |   ECHO   |   ECHOE   |   ISIG)   ;
	
	tcsetattr(fd,TCSANOW,&options);
	return fd;
}
/*************************************************************
* ���ܣ�	���ڳ�ʼ������
* ������	�����豸�ļ���
* ����ֵ��	�����豸�ļ�������
**************************************************************/
int uart_init(char *devname)
{
	int uart_fd;
	struct termios *options;

	uart_fd = uart_open(devname);
	tcgetattr(uart_fd, &option_old);	//���洮������
	options = uart_set_attr(uart_fd, 115200, 8, 1, 'N', NULL);

	//options->c_lflag |= ECHO;				/* ������ */
	options->c_lflag &= ~ECHO;				/* �ػ��� */
	tcsetattr(uart_fd, TCSANOW,  options);	/* ���ô������� */
	free(options);
	return uart_fd;
}

/*************************************************************
* ���ܣ�	���ڷ���ʼ������
* ������	�����豸�ļ�������
* ����ֵ��	��
**************************************************************/
void uart_uninit(int uart_fd)
{
	/*��ԭ��������*/
	tcsetattr(uart_fd, TCSANOW, &option_old);
	
	/*�رմ���*/
	close(uart_fd);
}

/*************************************************************
* ���ܣ�	���ڷ����ַ���
* ������	uart_fd�������豸�ļ�������
			str�������͵��ַ�
* ����ֵ��	��
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
* ���ܣ�	���ڶ�һ���ַ��������趨��ʱ���ڶ�����������������
* ������	uart_fd�������豸�ļ�������
			buffer��������ݵ��ڴ���׵�ַ
			len��������ݵ��ڴ�ռ�Ĵ�С
			timeout_ms����ʱʱ��(��λ��ms)
* ����ֵ��	
			�ɹ���ʵ�ʶ������ַ���
			ʧ�ܣ�-1
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
* ���ܣ�	
* ������	uart_fd�������豸�ļ�������
			buffer��������ݵ��ڴ���׵�ַ
			buffer_size ���ݴ�С			
* ����ֵ��	
			�ɹ���ʵ��д����ַ���
			ʧ�ܣ�-1
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
