#include<stdio.h>
#include<string.h>
#include"login.h"

#define USERNAME	"root"
#define PASSWORD	"111111"

#define RIGHT				0	
#define USER_ERR			1
#define PWD_ERR				2
#define USER_AND_PWD_ERR	3

int get_login_mode(const char *data, int *mode)
{
	return sscanf(data, "%d&", mode);
}

int get_login_user_and_pwd(const char *data, char *user, char *pwd)
{
	return sscanf(data, "%*[^&]&%[^&]&%s", user, pwd);
}

int login_deal(const char *data, char *return_data)
{
	int mode = -1;
	char user[64] = "";
	char pwd[64] = "";
	int temp = -1;
	
	get_login_mode(data, &mode);
	get_login_user_and_pwd(data, user, pwd);
	printf("user==%s,pwd==%s===\n", user, pwd);
	if(mode == LOGIN_JUDGE_FLAG){
		if(strcmp(user, USERNAME) == 0){
			if(strcmp(pwd, PASSWORD) == 0){
				temp = RIGHT;
			}else{
				temp = PWD_ERR;
			}
		}else if(strcmp(pwd, PASSWORD) == 0){
			if(strcmp(user, USERNAME) == 0){
				temp = RIGHT;
			}else{
				temp = USER_ERR;
			}
		}else{
			temp = USER_AND_PWD_ERR;
		}
	}
	else
		return -1;
	sprintf(return_data, "%d", temp);
	return 0;
}

