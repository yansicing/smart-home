#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<net/if.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/ioctl.h>

#include<regex.h>
#include"deal_IP.h"

#define ETH_NAME "eth0"

/*************************************************************
* �������ܣ�	ͨ��������ʽ����Ƿ�ΪIP��ַ
* �������ͣ�	��Ҫ����IP��ַ
* �������ͣ�	�ɹ�����0��ʧ�ܷ���-1
**************************************************************/
int check_right_ip(const char *ip)
{
    int status = 0;
	int cflags = REG_EXTENDED;
	regmatch_t pmatch[1];
	const size_t nmatch = 1;
	regex_t reg;
	char str_ip[30] = "";
	const char *pattern = "[0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}[.][0-9]{1,3}";//���ھ���

	if(ip == NULL)
		return -1;

    strcpy(str_ip,ip);
	regcomp(&reg, pattern, cflags);
	status = regexec(&reg,str_ip,nmatch,pmatch,0);

	if(status==REG_NOMATCH)
	{
        printf("No match\n");
		return -1;
	}
	else if(status==0)
	{
        return 0;
	}
	regfree(&reg);

	return 0;
}

/*************************************************************
* �������ܣ�	��ȡIP��ַ
* �������ͣ�	IP��ַ���λ��
* �������ͣ�	
**************************************************************/
int get_ip(char *ip)
{
    int sock;
    struct sockaddr_in sin;
    struct ifreq ifr;
	char *temp_ip = NULL;
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        perror("socket");
        return -1;                
    }
    strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ-1);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;
    
    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
    {
        perror("ioctl");
		close(sock);
        return -1;
    }

    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
	temp_ip = inet_ntoa(sin.sin_addr);
	strcpy(ip,temp_ip);
    fprintf(stdout, "eth0: %s\n", temp_ip);
	close(sock);
	return 1;
}

/*************************************************************
* �������ܣ�	�Զ�����IP��ַ
* �������ͣ�	������õ�IP��ַ
* �������ͣ�	
**************************************************************/
int set_auto_ip(char *ip)
{
	pid_t pid = fork();//����һ�����̣����ڵ���exec������ִ��udhcpc�Զ�����IP
	if(pid<0)
	{
        perror("get auto ip fork error");
		return -1;
	}
	else if(pid==0)
	{
        execlp("udhcpc","udhcpc",NULL);
		perror("udhcpc error");
	}
	else
	{
        sleep(2);//��ʱ2�룬�ȴ�����IP
		int flag = kill(pid,SIGKILL);//���3����ӽ��̻�û��������ɱ����
		wait(NULL);//�����ӽ�����Դ

		if(flag==0)//��ȡʧ��
		{
			printf("get auto ip error\n");
            return -1;
		}
		else if(flag!=0)//�ɹ�����ȥ��ȡIP 
		{
            get_ip(ip);
		}
	}
	
	return 1;
}

/*************************************************************
* �������ܣ�	�ֶ�����IP��ַ
* �������ͣ�	Ҫ���õ�IP��ַ
* �������ͣ�	�ɹ�����0��ʧ�ܷ���-1
**************************************************************/
int set_hand_ip(const char *ipaddr)
{
	int sock_set_ip;     
    struct sockaddr_in sin_set_ip;     
    struct ifreq ifr_set_ip;     

    memset( &ifr_set_ip,0,sizeof(ifr_set_ip));     
    if( ipaddr == NULL )     {
        return -1;     
    }
   
	sock_set_ip = socket( AF_INET, SOCK_STREAM, 0 );
	//printf("sock_set_ip=====%d\n",sock_set_ip);
    if(sock_set_ip<0)  
    {     
        perror("socket create failse...SetLocalIp!");     
        return -1;     
    }     

    memset( &sin_set_ip, 0, sizeof(sin_set_ip));     
    strncpy(ifr_set_ip.ifr_name, "eth0", sizeof(ifr_set_ip.ifr_name)-1);        

    sin_set_ip.sin_family = AF_INET;     
    sin_set_ip.sin_addr.s_addr = inet_addr(ipaddr);     
    memcpy( &ifr_set_ip.ifr_addr, &sin_set_ip, sizeof(sin_set_ip));     
    printf("ipaddr===%s\n",ipaddr);
    if( ioctl( sock_set_ip, SIOCSIFADDR, &ifr_set_ip) < 0 )     
    {     
        perror( "Not setup interface");  
		close( sock_set_ip );
        return -1;     
    }     

    //���ü����־      
    ifr_set_ip.ifr_flags |= IFF_UP |IFF_RUNNING;      

    //get the status of the device      
    if( ioctl( sock_set_ip, SIOCSIFFLAGS, &ifr_set_ip ) < 0 )     
    {     
         perror("SIOCSIFFLAGS");   
		 close( sock_set_ip );
         return -1;     
    }     
    
    close( sock_set_ip );     
	return 0;
}

int get_deal_ip_mode(char *data, int *mode)
{
	if(data==NULL || mode==NULL)
		return -1;
	return sscanf(data, "%d&", mode);
}

int get_deal_ip_ipaddr(char *data, char *ip)
{
	if(data==NULL || ip==NULL)
		return -1;
	return sscanf(data, "%*[^&]&%s", ip);
}

int deal_ip(char *data, char *return_data)
{
	int mode = -1;
	int return_mode = -1;
	char ip[16] = "";
	if(data==NULL || return_data==NULL){
		sprintf(return_data, "-1&NULL");
		return -1;
	}

	return_mode = get_deal_ip_mode(data, &mode);
	if(mode == GET_IP_FLAG){
		return_mode = get_ip(ip);
		if(return_mode == -1){
			return_mode = set_auto_ip(ip);
		}
	}else if(mode == SET_IP_FLAG){
		return_mode = get_deal_ip_ipaddr(data, ip);
		return_mode = check_right_ip(ip);
		//if(return_mode != -1)
			//return_mode = set_hand_ip(ip);
	}else{
		return -1;
	}
	
	return sprintf(return_data, "%s", ip);;
}

