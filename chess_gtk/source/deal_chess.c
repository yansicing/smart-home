#include "deal_chess.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static GtkWidget *win;			// ������
static GtkWidget *win_area;		// ��ͼ����
static int width, height;		// ��ͼ����Ŀ�Ⱥ͸߶�
// start_x��start_y������������ꣻgrid�����̿��(�߶�)
static int start_x, start_y, grid;		
static int chess[64][8][8];    // ��ά���飬�����־����״̬
static int machine_timer_id;	// ��ʱ��id
static int timer_id;			// ����ʱid
static int count_time;			// ����ʱʱ��
static enum GridState current_role;	// ö�ٱ���
static gboolean is_press_mode;	// �Ƿ�ť��սģʽ
static gboolean is_pvp;         // �Ƿ����˶�սģʽ
static int step;               	// ��־��ǰ����Ϊ�ڼ���



int cursor_x = 3;
int cursor_y = 3;




/*
 * ���ܣ����ã���Ҫ�õ����ڣ���ͼ�������ݱ�Ҫ��ʼ��
 * temp��������
 */
void set_control_chess(GtkWidget *temp)
{
	win = temp;					// ���������
	win_area = get_draw_area(); // ��û�ͼ����
	// �趨���̵�������꣬���̵Ŀ��(�߶�)
	start_x = 202, start_y = 62, grid = 50;
	is_press_mode = FALSE;		// Ĭ����û�а���ѡ��ģʽ
	
	// ���ӣ��ô���ʵ��������¼�
	g_signal_connect(G_OBJECT(win), "button_press_event",
								  G_CALLBACK(win_button_pressed), NULL);
	// ���ӣ���ͼ�¼�������
    g_signal_connect(G_OBJECT(win_area), "expose_event",
                        G_CALLBACK(draw_chess_event), NULL);			
}

/*
 * ���ܣ���ʼ������״̬
 */
void init_chess(gboolean is_begin)
{	
	step = 0;				// ��0��
	current_role = BLACK;	// Ĭ�Ϻ�������
	count_time = 21;		// ����ʱ��ʼʱ��
	hide_time_label();		// ���ص���ʱ�ؼ�
	show_num_label();		// ��ʾͳ�Ƹ����ؼ�
	set_num_label(2, 2);
	int i, j;
	// 64����ȫ����־ΪEmpty����־λ����ChessBoard�����ö�ٳ�����8��ֵΪ8
    for(i=0; i<8; i++){
        for(j=0; j<8; j++){
            chess[step][i][j] = EMPTY;
		}
	}
	
	if(TRUE == is_begin){
		// �������м��4���ռ�ֱ��־λ2����2����
		chess[step][3][3]= BLACK;  // a[3][3]
		chess[step][3][4]= WHITE;  // a[3][4]
		chess[step][4][3]= WHITE;  // a[4][3]
		chess[step][4][4]= BLACK;  // a[4][4]
	}
	
	update();
}

/*
 * ���ܣ��ı��ɫ
 */
void change_role()
{	
	count_time = 21;		// ���õ���ʱʱ��
	if(BLACK == current_role){
		current_role = WHITE;
		set_tip_state(TWHITE);	// ��ɫ��ʾ����
	}else{
		current_role = BLACK;
		set_tip_state(TBLACK);	// ��ɫ��ʾ����
	}
	
	if(1 == step){   // ����Ѿ�����һ���ӣ����԰����壬������ť
		gtk_widget_set_sensitive(button.redo_button, TRUE);   // ������ťʹ��
		gtk_widget_set_sensitive(button.goback_button, TRUE); // ���尴ťʹ��
	}
	
	judge_gameover();	// �ж�ʤ��
	
	// ������˼ʶ�սģʽ�����ҵ���������
	if(FALSE == is_pvp && WHITE == current_role){
		// ��ʱ����1s�����������
		machine_timer_id = g_timeout_add(1000, (GSourceFunc)machine_chess, NULL);
	}
}

/*
 * ���ܣ�����·���õ�ͼ�񣬶��ҿ���ָ�����С
 * path: ͼƬ·��
 * w, h: ָ��ͼƬ�Ŀ�Ⱥ͸߶�
 */
GdkPixmap *get_pixmap(const char *path, int w, int h)
{
	GdkPixbuf  *src_pixbuf;
    GdkPixbuf  *dst_pixbuf;
    GdkPixmap *pixmap;
	
	src_pixbuf = gdk_pixbuf_new_from_file (path, NULL);
    dst_pixbuf = gdk_pixbuf_scale_simple (src_pixbuf, w-2, h-2, 							GDK_INTERP_BILINEAR);
    gdk_pixbuf_render_pixmap_and_mask (dst_pixbuf, &pixmap, NULL, 128);
	
	g_object_unref (src_pixbuf);
    g_object_unref (dst_pixbuf);
	
    return pixmap;
}

/*
 * ���ܣ�������¼�������
 */
gboolean win_button_pressed (GtkWidget *widget,
						   GdkEventButton *event,
						   gpointer data)
{
	gint x = event->x, y = event->y;	// ��õ��������
	//printf("x=%d, y=%d\n", x, y);

    // Ҫ��֤����������̷�Χ����
    if( (x <= start_x) || (x >= (start_x + grid*8)) ||
       (y <= start_y) || (y >= (start_y + grid*8)) ){
		return FALSE;
	}
	
	if(FALSE == is_press_mode){ // ���û�а���սģʽ���������ʾ��
		show_info_dialog("��ѡ����սģʽ(���˻���)������");
		return FALSE;
	}
	
	// ����˻�ģʽ�������ǻ������꣬�˲�����
	if(FALSE == is_pvp && WHITE == current_role){
        return  FALSE;
	}
	   
	// ������ֵת��Ϊ������̵������±�
	// ������a[i][j]��i��j
	x = (x - start_x)/grid;
	y = (y - start_y)/(grid-10);
	printf("x=%d, y=%d\n", x, y);

	cursor_x = x;
	cursor_y = y;
	
	if(EMPTY == chess[step][x][y]){ // ��ǰ����λ��Ϊ��ʱ���ܲ���
		// �ж������ܷ����֮ǰ�������̵�״̬��������
		memcpy( (void *)chess[step+1], (void *)chess[step], 64*sizeof(int) );
		
		if(0 < judgeRule(x,y,8, (void *)chess[step+1], current_role, TRUE)){
			step++;          // ��1����ʱstep�Ѿ��ı��ˣ�����1
			update();
			change_role();   // �ı����ӵĽ�ɫ
		}
	}
	
	return FALSE;
}

/*
 * ���ܣ���ͼ�¼�����Ҫ�ǻ��ڰ���
 */
gboolean draw_chess_event (GtkWidget* widget, GdkEventExpose* event, gpointer data)
{
    GdkGC* gc = gdk_gc_new (widget->window);
	// �õ���ͼ����Ŀ�Ⱥ͸߶�
    gdk_drawable_get_size (widget->window, &width, &height);
	
	GdkPixmap *black_pic = get_pixmap("./image/black.jpg", grid, grid-10);
	GdkPixmap *white_pic = get_pixmap("./image/white.jpg", grid, grid-10);
	
	//gdk_draw_drawable(widget->window, gc, black_pic,
    //            0, 0, start_x, start_y, grid-5, grid);
				
	int i, j;
	for(i=0; i<8; i++){
        for(j=0; j<8; j++){
            if(chess[step][i][j] == BLACK){
                gdk_draw_drawable(widget->window, gc, black_pic,
                0, 0, start_x+grid*i, start_y+(grid-10)*j, grid-5, grid);
            }else if(chess[step][i][j] == WHITE){
				gdk_draw_drawable(widget->window, gc, white_pic,
                0, 0, start_x+grid*i, start_y+(grid-10)*j, grid-5, grid);
			
             }
        }
    }

	g_object_unref(black_pic);				
    g_object_unref(white_pic);

	int xx = start_x + cursor_x*grid;
	int yy = start_y + cursor_y*(grid-10);
	cairo_t * cr = gdk_cairo_create(widget->window);
	cairo_set_line_width(cr, 4);
	cairo_set_source_rgb(cr, 1, 0, 0);
	cairo_rectangle(cr, xx, yy, grid-5, grid-10);
	cairo_stroke(cr);

	cairo_destroy(cr);

    return TRUE;
}

// ����ʱ������
gboolean deal_count_time(gpointer data)
{
	count_time--;
	set_time_label(count_time);	// ��ʾ
	if(0 == count_time){	// ʱ�䵽���ı��ɫ
		change_role();
	}
	
	return TRUE;
}

// ��������Ĵ�����
gboolean machine_chess(gpointer data)
{
	g_source_remove(machine_timer_id);	// ֹͣ��ʱ��
	int num = 0;
    int max = 0;
    int px, py;
	int i, j;
	
	// �ж���������Щλ�����ܹ����ӵ�
    for(i= 0;i<8;i++){
        for(j=0;j<8;j++){
            if(chess[step][i][j] == EMPTY){  // Ϊ���̿�ʱ����������
                // �ж��ܲ��ܳ��ӣ�ֻ���жϣ����ı�����״̬��ע��false����
                num = judgeRule(i,j,8,(void*)chess[step],current_role,FALSE);
                if(num > max){   // ��һ���������Ŀո񣬲������걣������
                    max = num;  // �ı�max��ֵ
                    px = i;
                    py = j;
                }
            }
        }
    }
	
	if(0 == max){      // ����һ��maxΪ0��˵�������ﶼ���ܳ��ӣ����Ǿ͸ı����ӵĽ�ɫ
        change_role(); // �ı����ӵĽ�ɫ
        return FALSE;       // �жϴ˺���
    }
	
	memcpy( (void *)chess[step+1], (void *)chess[step], 64*sizeof(int) );
	
	if(0 < judgeRule(px, py,8, (void *)chess[step+1], current_role, TRUE)){
			step++;          // ��1����ʱstep�Ѿ��ı��ˣ�����1
			update();
			change_role();   // �ı����ӵĽ�ɫ
			//gtk_widget_queue_draw_area(win_area, 0, 0, width, height); // update
			
	}
	
	return FALSE;
}

// ���»�ͼ�¼�
void update()
{
	gtk_widget_queue_draw_area(win_area, 0, 0, width, height); // update
}

// ��������ʱ
void start_count_time()
{
	timer_id = g_timeout_add(1000, (GSourceFunc)deal_count_time, NULL);
}

// ֹͣ����ʱ
void stop_count_time()
{
	g_source_remove(timer_id);
	set_time_label(20);
}

// ֹͣ����ʱ��
void stop_machine_time()
{
	g_source_remove(machine_timer_id);
}

// �����Ƿ�ť��ս��ť
void set_press_mode(gboolean flag)
{
	is_press_mode = flag;
}

// �õ��Ƿ�ť��ս��ť״̬
gboolean get_press_mode()
{
	return is_press_mode;
}

// ������սģʽ
void set_pvp_mode(gboolean flag)	
{
	is_pvp = flag;
}

// �õ���սģʽ
gboolean get_pvp_mode()
{
	return is_pvp;
}

// ���û���Ĳ���
void set_goback_step(int temp)
{
	step -= temp;
}	

// �õ���ǰ�ǵڼ�����	
int get_chess_step()
{
	return step;
}
		
// �жϺ��ӻ�����ܷ����
gboolean judge_is_can_eat(enum GridState currentRole)
{
	int i, j;
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			if(chess[step][i][j] != EMPTY){
				break;
			}
			
			if( 0 < judgeRule(i,j,8, (void *)chess[step], current_role, FALSE) ){
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

// �ж�ʤ��
void judge_gameover()
{
	int black_num=0;
	int white_num=0;
	gboolean is_over = TRUE;
	
	int i, j;
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			// ͳ�ƺڰ������
			if(chess[step][i][j] == WHITE){      // ����
				white_num++;
			}else if(chess[step][i][j] == BLACK){ // ����
               black_num++;
			}
			
			 if(0 < judgeRule(i,j,8, (void *)chess[step],WHITE, FALSE) ||
               0 < judgeRule(i,j,8, (void *)chess[step],BLACK, FALSE)){
					is_over = FALSE;
            }
		}
	}
	
	 // �ֱ���ʾ�ڰ������
	 set_num_label(black_num, white_num);
	 
    if(FALSE == is_over || 0 == step){	// ��û�е��ж���Ӯ��ʱ��
        return;
    }
	
	////////////////////////////////////�ж���Ӯ
	////////////////////////////redo
	set_button_state(INIT);	// ���³�ʼ����ť
	set_press_mode(FALSE);	// û�а�����սģʽ
	init_chess(FALSE);		// ����״̬���
	set_tip_state(TSTOP);	// ֹͣ��ʾ��ɫ
	stop_count_time();		// ֹͣ����ʱ
	hide_time_label();		// ���ص���ʱ��ʾ�ؼ�
	hide_num_label();		// ������ʾ�����ؼ�
	stop_machine_time();	// ֹͣ����ʱ��
	
    // �ж���Ӯ
    if(black_num > white_num){
        show_info_dialog("����Ӯ�ˣ�����");
    }else if(black_num < white_num){
        show_info_dialog("����Ӯ�ˣ�����");
    }else{
        show_info_dialog("ƽ����^_^������"); 
    }
	
	
}

/*
 *          ���ܣ����ӹ���
 *          x, y: ���̵�λ�ã���ά������±�
 *         chess: ��λ�����׵�ַ( (void *) chess[step] )
 *   currentRole����ǰ��ɫ(���ӻ����)
 *      eatChess: TRUE����Ҫ���ӣ�FALSE��ֻ���ж��ܲ��ܳ���
 *  ����ֵeatNum�����Ӹ���
 */
int judgeRule(int x, int y, int gridNum, void *chess, enum GridState currentRole,gboolean eatChess)
{
    // ���̵İ˸�����
    int dir[8][2]={{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    int temp_x = x, temp_y = y;             // ��ʱ����������������λ��
    int i = 0, eatNum = 0;                  // ��ʼ������
    typedef int (*p)[gridNum];             // �Զ�������
    p chessBoard = (p)chess;                // ����ת��

    if(chessBoard[temp_x][temp_y] != EMPTY)  // ����˷������������ӣ�����;
        return 0;

    // ���̵�8������
    for(i = 0 ; i <8; i++)
    {
        temp_x += dir[i][0]; temp_y += dir[i][1];	// ׼���ж���������
        // ���û�г��磬�����������ǶԷ����ӣ����г��ӵĿ��ܣ�
        if((temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)
            && (chessBoard[temp_x][temp_y] != currentRole) && (chessBoard[temp_x][temp_y] != EMPTY) )
        {
            temp_x += dir[i][0]; temp_y += dir[i][1];	            // �����ж���һ������ǰ��һ��
            while(temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)
            {
                if(chessBoard[temp_x][temp_y] == EMPTY) // ������λ����
                        break;
                if(chessBoard[temp_x][temp_y] == currentRole)       // �ҵ��Լ������ӣ�������Գ���
                {
                    if(eatChess == TRUE)                            // ȷ������
                    {

                        chessBoard[x][y] = currentRole;          // ��ʼ���־Ϊ�Լ�������
                        temp_x -= dir[i][0]; temp_y -= dir[i][1];// ����һ��
                        while((temp_x != x )||(temp_y != y))	 // ֻҪû�лص���ʼ��λ�þ�ִ��
                        {
                            chessBoard[temp_x][temp_y] = currentRole;   // ��־Ϊ�Լ�������
                            temp_x -= dir[i][0]; temp_y -= dir[i][1];   // ��������һ��
                            eatNum ++;  // �ۼ�
                        }
                    }
                    else    // �����ӣ�ֻ���ж����λ���ܲ��ܳ���
                    {

                        temp_x -= dir[i][0]; temp_y -= dir[i][1];   // ����һ��
                        while((temp_x != x )||(temp_y != y))        // ֻ������Գ��ӵĸ���
                        {
                            temp_x -= dir[i][0]; temp_y -= dir[i][1];// ��������һ��
                            eatNum ++;
                        }
                    }
                    break;  // ����ѭ��
                }// û���ҵ��Լ������ӣ�����ǰ��һ��
                temp_x += dir[i][0]; temp_y += dir[i][1];   // ��ǰ��һ��
            }
        }// �����������ܳ��ӣ��ͻ�һ������
        temp_x = x; temp_y = y;
    }

    return eatNum;              // �����ܳ��ӵĸ���
}
















/*
 * ���ܣ�����
 */
gboolean chess_cmd (int x, int y)
{
	if(x>=8 || x<0 || y>=8 || y<0)
		return FALSE;
	
	if(FALSE == is_press_mode){ // ���û�а���սģʽ���������ʾ��
		//show_info_dialog("��ѡ����սģʽ(���˻���)������");
		return FALSE;
	}
	
	// ����˻�ģʽ�������ǻ������꣬�˲�����
	if(FALSE == is_pvp && WHITE == current_role){
        return  FALSE;
	}
	if(EMPTY == chess[step][x][y]){ // ��ǰ����λ��Ϊ��ʱ���ܲ���
		// �ж������ܷ����֮ǰ�������̵�״̬��������
		memcpy( (void *)chess[step+1], (void *)chess[step], 64*sizeof(int) );
		if(0 < judgeRule(x,y,8, (void *)chess[step+1], current_role, TRUE)){
			step++;          // ��1����ʱstep�Ѿ��ı��ˣ�����1
			update();
			change_role();   // �ı����ӵĽ�ɫ
		}
	}
	
	return FALSE;
}

/*
* ��ȡ�ⲿ���Ȼ���������
*/
void *pthread_chess_recv_cmd(void *arg)
{
	int fd = open(CHESS_RECV_FIFO, O_RDWR);//����
	if(fd < 0){
		perror("open chess_recv_fifo error");
		return NULL;
	}
/*	while(1)
	{
		char buf[100] = "";//chesspos:3,4
		int x, y;
		if(read(fd, buf, sizeof(buf)) > 0){
			//printf("buf====%s===\n", buf);
			if(sscanf(buf, "%d,%d", &x, &y) == 2){
				printf("cmd x==%d,y==%d\n", x, y);
				chess_cmd(x, y);
			}
		}
		
	}
*/
	while(1)
	{
		char buf[32] = "";//chesspos:3,4
		int key;
		if(read(fd, buf, sizeof(buf)) > 0){
			//printf("buf====%s===\n", buf);
			if(sscanf(buf, "key:%d", &key) == 1){	//�����ʽ
				//printf("key====%d\n", key);
				if(key==0){		//ȷ��
					chess_cmd(cursor_x, cursor_y);
				}else if(key==1){	//��
					cursor_y>0 ? (cursor_y--) : 0;
				}else if(key==2){	//��
					cursor_y<7 ? (cursor_y++) : 0;
				}else if(key==3){	//��
					cursor_x>0 ? (cursor_x--) : 0;
				}else if(key==4){	//��
					cursor_x<7 ? (cursor_x++) : 0;
				}
				//printf("cursor_x==%d,cursor_y==%d\n", cursor_x, cursor_y);
				update();//���»�ͼ���򣬰�ѡ�п���ʾ
			}
		}
	}

	return NULL;
}








