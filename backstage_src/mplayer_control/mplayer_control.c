#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include"mplayer_control.h"
#include"deal_song_list.h"

/*******************************************************
*功能：		带空格的文件及路径的转换
*参数：		dest：转换目标地址
			length：转换目标地址空间大小
			src：文件或路径名地址
*返回值：	成功：转换目标地址：dest；失败：NULL
********************************************************/
char *pathname_space_dispose(char *dest, int length, char *src)
{
	char *tmp;
	int src_length = 0;
	tmp = dest;
	
//	printf("src=[%s]\n", src);
	memset(dest, 0, length);
	while(*src != 0)
	{
		src_length++;
		if(src_length > (length-1))
		{
			return NULL;
		}
		if(*src == ' ')
		{
			*dest = '\\';
			dest++;
		}
		*dest = *src;
		dest++;
		src++;
	}
	//printf("dest=[%s]\n", tmp);
	//printf("length=%d\n", dest-tmp);
	return tmp;
}

/***************************************
函数功能:写命令给mplayer
参数类型:写入的命名(完整的一条命令)
返回描述:
****************************************/
int write_player_cmd(FIFO_FD *fifo, SONG_STATUS *status, const char *cmd)
{
	pthread_mutex_lock(&fifo->write_cmd_mutex);
	int wr_len = write(fifo->cmd_fifo_fd,cmd,strlen(cmd));
	pthread_mutex_unlock(&fifo->write_cmd_mutex);
	return wr_len;
}

/***************************************
函数功能:通过命令号实现发送发送相应的命令控制mplayer
参数类型:PLAYER结构体，命令号，设置了一个枚举
返回描述:
****************************************/
int send_cmd_to_player(PLAYER *player, int cmd_num, void *data)
{
	char cmd[128] = "";
	char song_cur[64] = "";
	
//	printf("=====%d========\n", cmd_num);
	switch(cmd_num)
	{
		case CMD_PAUSE:		/*4 暂停*/
			strcpy(cmd, "pause\n");
			break;

		case CMD_VOLUME:	/*5 音量控制*/
			snprintf(cmd, sizeof(cmd), "volume %d 1\n",
					player->status.volume);
			
			break;


		case CMD_CHANGESONG:/*7 切换歌曲*/
			if(data!=NULL)
			{
				memset(player->status.song_cur, 0, sizeof(player->status.song_cur));
				strcpy(player->status.song_cur, (char *)data);
				pathname_space_dispose(song_cur, sizeof(song_cur), player->status.song_cur);
				snprintf(cmd, sizeof(cmd), "loadfile %s%s\n",
						player->path.song_path, song_cur);
			}
			break;

		default:
				printf("cmd not found!!\n");
			break;
		
	}
	int wr_len;
	wr_len = write_player_cmd(&player->fifo, &player->status, cmd);

	return wr_len;
}



/***************************************
函数功能:创建进程，启动mplayer
参数类型:PLAYER结构体
返回描述:
****************************************/
int player_start(PLAYER *player)
{
	pid_t pid = 0;

	if(player->list.song_list==NULL){
		printf("------No song can player,song_list is NULL-----\n");
		return -1;
	}
	pid = vfork();
	if(pid<0){
		perror("player start fork error");
		return -1;
	}
	else if(pid==0)
	{
		char pathname[200] = "";
		char fifo_pathname[64] = "";
	
		snprintf(pathname,sizeof(pathname),"%s%s",
			player->path.song_path, player->list.song_list[0]);

		snprintf(fifo_pathname,sizeof(fifo_pathname), "file=%s%s",
			player->path.fifo_path, "player_cmd_fifo");
		close(0);
		printf("===pathname=====%s=====\n", pathname);
		dup2(player->fifo.msg_fifo_fd, STDOUT_FILENO);/*重定向标准输出*/
		execl("../execute/mplayer",
				"mplayer", "-ac", "mad",
				"-slave","-idle", "-quiet",
				"-input", fifo_pathname,
				pathname, NULL);

		
		perror("execl error");
		_exit(-1);
	}

	
	
	return pid;
}


/********路径初始化*********/
void path_init(PATH *path)
{
	strcpy(path->song_path, SONG_PATH);
	strcpy(path->fifo_path, FIFO_PATH);
}

/*******歌曲列表初始化***********/
char **song_list_init(PLAYER *player)
{
	if(player->list.song_list != NULL){
		player->list.song_list = release_song_list(player->list.song_list, player->list.song_num);
	}
	player->list.song_list = NULL;
	player->list.song_list = get_song_list(player->path.song_path, &player->list.song_num);

	return player->list.song_list;
}



/***********歌曲状态初始化**************/
void song_status_init(SONG_STATUS *status, SONG_LIST *list)
{
	if(list->song_list != NULL){
		strcpy(status->song_cur, list->song_list[0]);
	}
	status->ispause = 0;
	status->ismute = 0;
	status->play_change_flag = 1;
	status->volume = 40;
	status->play_model = 0;
	status->song_cur_num = 0;
	status->percent = 0;
	status->time_position = 0;
}

/******读写锁的初始化******/
void lock_init(PLAYER *player)
{
	pthread_mutex_init(&player->fifo.write_cmd_mutex, NULL);//创建写mplayer命令的互斥锁
}


/***************************************
函数功能:创建mplayer接收命令和传输数据的命名管道
参数类型:命名管道，路径
返回描述:
****************************************/
int create_cmd_msg_fifo(FIFO_FD *fifo, PATH *path)
{
	char cmd_pathname[128] = "";
	//char msg_pathname[128] = "";
	strcpy(cmd_pathname, path->fifo_path);
	strcat(cmd_pathname, "player_cmd_fifo");

	//strcpy(msg_pathname, path->fifo_path);
	//strcat(msg_pathname, "player_msg_fifo");
	
	//unlink(cmd_pathname);
	//unlink(msg_pathname);

	mkfifo(cmd_pathname,0666);
	//mkfifo(msg_pathname,0666);

	fifo->cmd_fifo_fd = open(cmd_pathname,O_RDWR);
	//fifo->msg_fifo_fd = open(msg_pathname,O_RDWR);
	if((fifo->cmd_fifo_fd<0))
	{
		perror("open fifo error");
		return -1;
	}

	return 1;
}

/********命名管道初始化********/
void fifo_fd_init(PATH *path, FIFO_FD *fifo)
{
	create_cmd_msg_fifo(fifo, path);
}

void fifo_fd_close(FIFO_FD *fifo)
{
	close(fifo->cmd_fifo_fd);
	close(fifo->msg_fifo_fd);
}

int get_mplayer_control_mode(char *data, int *mode)
{
	if(data==NULL || mode==NULL)
		return -1;

	return sscanf(data, "%d&", mode);
}

static char *get_str_addr(char *str, char c, int p_size)
{
	char *p = str;
	int i = 0;
	for(i=0;i<p_size;++i)
	{
		if((p = strchr(p, c)) == NULL){
			break;
		}
		p++;
	}
	return p;	
}

int get_mplayer_control_volume(char *data, int *volume)
{
	if(data==NULL || volume==NULL)
		return -1;

	return sscanf(data, "%*[^&]&%d", volume);
}


int get_mplayer_control_song_name(char *data, char *msg)
{
	if(data==NULL || msg==NULL)
		return -1;

	char *p = get_str_addr(data, '&', 1);
	if(p==NULL)
		return -1;
	strcpy(msg, p);
	return 1;
}


int mplayer_control(char *data, char *return_data)
{
	int mode = -1;
	int return_mode = -1;
	PLAYER player;
	char song[2048] = "";
	static int play_flag = 0;
	memset(&player, 0, sizeof(PLAYER));

	if(data==NULL || return_data==NULL)
		return -1;


	return_mode = get_mplayer_control_mode(data, &mode);
	//printf("mode==%d\n", mode);
	if(mode == MPLAYER_PAUSE_FLAG){
		path_init(&player.path);
		fifo_fd_init(&player.path, &player.fifo);

		//send_cmd_to_player(&player, CMD_PAUSE, NULL);

		send_cmd_to_player(&player, CMD_PAUSE, NULL);
	
		fifo_fd_close(&player.fifo);

	}else if(mode == MPLAYER_VOLUME_FLAG){
		path_init(&player.path);
		fifo_fd_init(&player.path, &player.fifo);

		get_mplayer_control_volume(data, &player.status.volume);
		send_cmd_to_player(&player, CMD_VOLUME, NULL);
			
		fifo_fd_close(&player.fifo);

	}else if(mode == MPLAYER_CHANGE_SONG_FLAG){
		path_init(&player.path);
		fifo_fd_init(&player.path, &player.fifo);

		char song_name[128] = "";
		get_mplayer_control_song_name(data, song_name);
		send_cmd_to_player(&player, CMD_CHANGESONG, song_name);

		fifo_fd_close(&player.fifo);

	}else if(mode == MPLAYER_GET_LIST_FLAG){
		int i;
		
		path_init(&player.path);
		player.list.song_list = get_song_list(player.path.song_path, &player.list.song_num);
		if(player.list.song_list == NULL)
			return -1;
		memset(song, 0, sizeof(song));
		for(i=0; i<player.list.song_num; ++i)
		{
			if(strlen(song) > 2048-64){
				break;
			}
			strcat(song, player.list.song_list[i]);
			if(i<player.list.song_num-1)
				strcat(song, "&");
		}
		//printf("song======%s======\n", song);

		sprintf(return_data, "%s", song);

		return 1;
	}else if(mode == MPLAYER_STATR_FLAG){
		if(play_flag != 0)
			return -1;
		
		play_flag = 1;

		path_init(&player.path);
		fifo_fd_init(&player.path, &player.fifo);
		player.list.song_list = get_song_list(player.path.song_path, &player.list.song_num);
		if(player.list.song_list == NULL)
			return -1;
		
		player_start(&player);

		//send_cmd_to_player(&player, CMD_PAUSE, NULL);//首先暂停

		fifo_fd_close(&player.fifo);
		release_song_list(player.list.song_list, player.list.song_num);
	}
	return -1;
}
