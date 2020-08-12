#ifndef _MPLAYER_CONTROL_H_
#define	_MPLAYER_CONTROL_H_

#include<pthread.h>
#include<semaphore.h>


#define MPLAYER_PAUSE_FLAG			1
#define MPLAYER_VOLUME_FLAG			2
#define MPLAYER_CHANGE_SONG_FLAG	3
#define MPLAYER_GET_LIST_FLAG		4
#define MPLAYER_STATR_FLAG			5


#define SONG_PATH "../song/"
#define FIFO_PATH "../fifo/"
#define LRC_PATH "../lyrics/"
#define TOTAL_SONG_NUM 200

#define PATH_SIZE 64						/*路径大小*/
#define SONG_SIZE 64						/*歌名大小*/

/******mplayer发送命令号******/
enum player_cmd_num
{
	CMD_BACK,//上一曲0
	CMD_FRONT,//下一曲1
	CMD_BCKWORD,//快退2
	CMD_FORWORD,//快进3
	CMD_PAUSE,//暂停4
	CMD_VOLUME,//控制音量5
	CMD_MUTE1,//静音6
    CMD_PLAY_MODEL,//播放模式7
	CMD_CHANGESONG//切换歌曲8
};





/******歌曲状态信息*****/
typedef struct
{
	char song_cur[SONG_SIZE];		//当前播放的歌曲
	int ispause;               	 	//mplayer暂停标志位；0：未暂停，1：已暂停
	int ismute;						//静音标志位
	int play_change_flag;			//切换歌曲标志位
	int volume;                 	//mplayer音量范围（0-100）
	int temp_volume;
	int play_model;					//播放模式
	int song_cur_num;				//当前播放歌曲编号
	int percent;					//播放进度百分比
	int time_position;				//播放进度时间
	int song_length;				//播放时间总长度
}SONG_STATUS;

/******歌曲列表*******/
typedef struct 
{
	char **song_list;				//歌曲列表指针
	int song_num;               	//歌曲总数

	pthread_rwlock_t list_rwlock;	//列表读写锁
}SONG_LIST;

/*********路径相关*********/
typedef struct
{
	char song_path[PATH_SIZE];				//歌曲路径
	char fifo_path[PATH_SIZE];				//存放命名管道路径名
	char lrc_path[PATH_SIZE];				//存放歌词路径名

	pthread_rwlock_t path_rwlock;			//路径读写锁
}PATH;

/*******mplayer进程启动*******/
typedef struct
{
	pid_t pid;						//mplayer启动进程号
	int play_flag;					//mplayer启动标志位;0:未启动,1:已启动

	pthread_rwlock_t process_rwlock;
}PLAYER_PROCESS;


/*******打开的命名管道文件描述符********/
typedef struct
{
	int cmd_fifo_fd;				//向mplayer发送命令的文件描述符
	int msg_fifo_fd;				//接收mplayer信息的文件描述符
	pthread_mutex_t write_cmd_mutex;//写命令的命名管道互斥锁
}FIFO_FD;



/*******整个mplayer启动结构体*********/
typedef struct 
{
	SONG_STATUS status;				//歌曲状态
	SONG_LIST list;					//歌曲列表
	PATH path;						//各种路径
	FIFO_FD fifo;					//命名管道
}PLAYER;


extern int mplayer_control(char *data, char *return_data);


#endif
