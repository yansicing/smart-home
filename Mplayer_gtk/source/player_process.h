#ifndef _PLAYER_PROCESS_H_
#define _PLAYER_PROCESS_H_

#define SONG_PATH "../song/"
#define SQLITE_PATH "../sqlite/"
#define FIFO_PATH "../execute/"
#define LRC_PATH "../lyrics/"
#define PIC_PATH "../picture/"
#define TOTAL_SONG_NUM 200

/******���ĳ�ʼ��******/
extern void lock_init(PLAYER *player);

//*******************��ǰ���Ÿ���
extern int player_set_status_song_cur(PLAYER *player, const char *new_song);

extern int player_get_status_song_cur(PLAYER *player, char *song_cur, int size);

//*********************��ͣ��־λ
extern int player_set_status_ispause(PLAYER *player, const int ispause);

extern int player_get_status_ispause(PLAYER *player, int *ispause);

//********************������־λ
extern int player_set_status_ismute(PLAYER *player, const int ismute);

extern int player_get_status_ismute(PLAYER *player, int *ismute);

//****************�����л���־λ
extern int player_set_status_play_change_flag(PLAYER *player, const int play_change_flag);

extern int player_get_status_play_change_flag(PLAYER *player, int *play_change_flag);

//******************��������
extern int player_set_status_volume(PLAYER *player, const int volume);

extern int player_get_status_volume(PLAYER *player, int *volume);


//**************������ǰ���ŵı��
extern int player_set_status_song_cur_num(PLAYER *player, const int song_cur_num);

extern int player_get_status_song_cur_num(PLAYER *player, int *song_cur_num);


//*********************�ٷֱ�
extern int player_set_status_percent(PLAYER *player, const int percent);

extern int player_get_status_percent(PLAYER *player, int *percent);

//*********************��ǰ����ʱ��
extern int player_set_status_time_position(PLAYER *player, const int time_position);

extern int player_get_status_time_position(PLAYER *player, int *time_position);

//*********************�����ܳ���
extern int player_set_status_song_length(PLAYER *player, const int song_length);

extern int player_get_status_song_length(PLAYER *player, int *song_length);


//**********************��������
extern int player_set_song_list_num(PLAYER *player, const int song_num);

extern int player_get_song_list_num(PLAYER *player, int *song_num);


//****************�������Ŀ¼
extern int player_set_song_path(PLAYER *player, const char *song_path);

extern int player_get_song_path(PLAYER *player, char *song_path, int size);

//**********************�����ܵ�·��
extern int player_set_fifo_path(PLAYER *player, const char *fifo_path);

extern int player_get_fifo_path(PLAYER *player, char *fifo_path, int size);

//********************���·��
extern int player_set_lrc_path(PLAYER *player, const char *lrc_path);

extern int player_get_lrc_path(PLAYER *player, char *lrc_path, int size);

//******************ͼƬ·��
extern int player_set_pic_path(PLAYER *player, const char *pic_path);

extern int player_get_pic_path(PLAYER *player, char *pic_path, int size);












/********·����ʼ��*********/
extern void path_init(PLAYER *player);

/********�����ܵ���ʼ��********/
extern void fifo_fd_init(PLAYER *player);


extern void song_status_init(PLAYER *player);

extern int player_start(PLAYER *player);

extern void player_lrc_init(PLAYER *player);

#endif

