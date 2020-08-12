#ifndef _INFRARED_H
#define _INFRARED_H


#define INFRARDE_DEV_PATH "/dev/hs38b_ir"
#define MPLAYER_FIFO_PATH "../Mplayer_gtk/execute/player_cmd_fifo"
#define MPLAYER_MSG_FIFO_PATH "../Mplayer_gtk/execute/player_msg_fifo"
#define MUSIC_WAV "../backstage_src/music/test.wav"

#define CHESS_FIFO_PATH "../chess_gtk/fifo/chess_recv_fifo"

void *pthread_infrared_recv(void *arg);


#endif


