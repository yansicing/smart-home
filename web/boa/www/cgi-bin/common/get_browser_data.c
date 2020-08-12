#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define GET "GET"
#define POST "POST"

int get_browser_data(char *data)
{
	char *request_type = NULL;
	char *tmp = NULL;
	int size = 0;
	int i = 0;
	
	request_type = getenv("REQUEST_METHOD");//获取请求的类型，GET或POST
	if(request_type==NULL)
	{
		printf("getenv REQUEST_METHOD error\n");
		return -1;
	}
	//printf("request_type===%s\n",request_type);
	if(strcmp(request_type,GET)==0)//GET方式
	{
		tmp = getenv("QUERY_STRING");//GET方式，直接获取到数据
		if(tmp != NULL)
		{
			strncpy(data, tmp, strlen(tmp));
		}
		else
		{
			printf("getenv QUERY_STRING error\n");
			return -1;
		}
	}
	else if(strcmp(request_type,POST)==0)//POST方式
	{
		tmp = getenv("CONTENT_LENGTH");//POST方式，先获取长度
		if(tmp!=NULL)
		{
			size = atoi(tmp);//数据大小转换为整形
			for(i=0;i<size;++i)
			{
				data[i] = getc(stdin);//从标准输入获取数据
			}
		}
		else
		{
			printf("getenv CONTENT_LENGTH error\n");
			return -1;
		}
	}
	else
	{
		printf("request is not GET or POST");
		return -1;
	}
	
	return 0;
}