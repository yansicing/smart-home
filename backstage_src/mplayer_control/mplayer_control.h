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

#define PATH_SIZE 64						/*·����С*/
#define SONG_SIZE 64						/*������С*/

/******mplayer���������******/
enum player_cmd_num
{
	CMD_BACK,//��һ��0
	CMD_FRONT,//��һ��1
	CMD_BCKWORD,//����2
	CMD_FORWORD,//���3
	CMD_PAUSE,//��ͣ4
	CMD_VOLUME,//��������5
	CMD_MUTE1,//����6
    CMD_PLAY_MODEL,//����ģʽ7
	CMD_CHANGESONG//�л�����8
};





/******����״̬��Ϣ*****/
typedef struct
{
	char song_cur[SONG_SIZE];		//��ǰ���ŵĸ���
	int ispause;               	 	//mplayer��ͣ��־λ��0��δ��ͣ��1������ͣ
	int ismute;						//������־λ
	int play_change_flag;			//�л�������־λ
	int volume;                 	//mplayer������Χ��0-100��
	int temp_volume;
	int play_model;					//����ģʽ
	int song_cur_num;				//��ǰ���Ÿ������
	int percent;					//���Ž��Ȱٷֱ�
	int time_position;				//���Ž���ʱ��
	int song_length;				//����ʱ���ܳ���
}SONG_STATUS;

/******�����б�*******/
typedef struct 
{
	char **song_list;				//�����б�ָ��
	int song_num;               	//��������

	pthread_rwlock_t list_rwlock;	//�б��д��
}SONG_LIST;

/*********·�����*********/
typedef struct
{
	char song_path[PATH_SIZE];				//����·��
	char fifo_path[PATH_SIZE];				//��������ܵ�·����
	char lrc_path[PATH_SIZE];				//��Ÿ��·����

	pthread_rwlock_t path_rwlock;			//·����д��
}PATH;

/*******mplayer��������*******/
typedef struct
{
	pid_t pid;						//mplayer�������̺�
	int play_flag;					//mplayer������־λ;0:δ����,1:������

	pthread_rwlock_t process_rwlock;
}PLAYER_PROCESS;


/*******�򿪵������ܵ��ļ�������********/
typedef struct
{
	int cmd_fifo_fd;				//��mplayer����������ļ�������
	int msg_fifo_fd;				//����mplayer��Ϣ���ļ�������
	pthread_mutex_t write_cmd_mutex;//д����������ܵ�������
}FIFO_FD;



/*******����mplayer�����ṹ��*********/
typedef struct 
{
	SONG_STATUS status;				//����״̬
	SONG_LIST list;					//�����б�
	PATH path;						//����·��
	FIFO_FD fifo;					//�����ܵ�
}PLAYER;


extern int mplayer_control(char *data, char *return_data);


#endif
