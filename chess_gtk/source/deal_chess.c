#include "deal_chess.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static GtkWidget *win;			// 主窗口
static GtkWidget *win_area;		// 画图区域
static int width, height;		// 画图区域的宽度和高度
// start_x，start_y：棋盘起点坐标；grid：棋盘宽度(高度)
static int start_x, start_y, grid;		
static int chess[64][8][8];    // 三维数组，方便标志棋盘状态
static int machine_timer_id;	// 定时器id
static int timer_id;			// 倒计时id
static int count_time;			// 倒计时时间
static enum GridState current_role;	// 枚举变量
static gboolean is_press_mode;	// 是否按钮作战模式
static gboolean is_pvp;         // 是否人人对战模式
static int step;               	// 标志当前下子为第几步



int cursor_x = 3;
int cursor_y = 3;




/*
 * 功能：设置，主要得到窗口，画图区域，数据必要初始化
 * temp：主窗口
 */
void set_control_chess(GtkWidget *temp)
{
	win = temp;					// 获得主窗口
	win_area = get_draw_area(); // 获得画图区域
	// 设定棋盘的起点坐标，棋盘的宽度(高度)
	start_x = 202, start_y = 62, grid = 50;
	is_press_mode = FALSE;		// 默认是没有按下选择模式
	
	// 连接，让窗口实现鼠标点击事件
	g_signal_connect(G_OBJECT(win), "button_press_event",
								  G_CALLBACK(win_button_pressed), NULL);
	// 连接，画图事件处理函数
    g_signal_connect(G_OBJECT(win_area), "expose_event",
                        G_CALLBACK(draw_chess_event), NULL);			
}

/*
 * 功能：初始化棋盘状态
 */
void init_chess(gboolean is_begin)
{	
	step = 0;				// 第0步
	current_role = BLACK;	// 默认黑子先下
	count_time = 21;		// 倒计时开始时间
	hide_time_label();		// 隐藏倒计时控件
	show_num_label();		// 显示统计个数控件
	set_num_label(2, 2);
	int i, j;
	// 64个格全部标志为Empty，标志位是在ChessBoard定义的枚举常量，8的值为8
    for(i=0; i<8; i++){
        for(j=0; j<8; j++){
            chess[step][i][j] = EMPTY;
		}
	}
	
	if(TRUE == is_begin){
		// 将棋盘中间的4个空间分别标志位2黑子2白子
		chess[step][3][3]= BLACK;  // a[3][3]
		chess[step][3][4]= WHITE;  // a[3][4]
		chess[step][4][3]= WHITE;  // a[4][3]
		chess[step][4][4]= BLACK;  // a[4][4]
	}
	
	update();
}

/*
 * 功能：改变角色
 */
void change_role()
{	
	count_time = 21;		// 重置倒计时时间
	if(BLACK == current_role){
		current_role = WHITE;
		set_tip_state(TWHITE);	// 角色提示白子
	}else{
		current_role = BLACK;
		set_tip_state(TBLACK);	// 角色提示黑子
	}
	
	if(1 == step){   // 如果已经下了一个子，可以按悔棋，重来按钮
		gtk_widget_set_sensitive(button.redo_button, TRUE);   // 重来按钮使能
		gtk_widget_set_sensitive(button.goback_button, TRUE); // 悔棋按钮使能
	}
	
	judge_gameover();	// 判断胜负
	
	// 如果是人际对战模式，并且到机器下了
	if(FALSE == is_pvp && WHITE == current_role){
		// 定时器，1s后机器才下子
		machine_timer_id = g_timeout_add(1000, (GSourceFunc)machine_chess, NULL);
	}
}

/*
 * 功能：根据路径得到图像，而且可以指定其大小
 * path: 图片路径
 * w, h: 指定图片的宽度和高度
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
 * 功能：鼠标点击事件，下子
 */
gboolean win_button_pressed (GtkWidget *widget,
						   GdkEventButton *event,
						   gpointer data)
{
	gint x = event->x, y = event->y;	// 获得点击的坐标
	//printf("x=%d, y=%d\n", x, y);

    // 要保证点击点在棋盘范围里面
    if( (x <= start_x) || (x >= (start_x + grid*8)) ||
       (y <= start_y) || (y >= (start_y + grid*8)) ){
		return FALSE;
	}
	
	if(FALSE == is_press_mode){ // 如果没有按作战模式，最好有提示框
		show_info_dialog("请选择作战模式(人人或单人)！！！");
		return FALSE;
	}
	
	// 如果人机模式，必须是机器下完，人才能下
	if(FALSE == is_pvp && WHITE == current_role){
        return  FALSE;
	}
	   
	// 将坐标值转换为相对棋盘的数组下标
	// 类似于a[i][j]的i和j
	x = (x - start_x)/grid;
	y = (y - start_y)/(grid-10);
	printf("x=%d, y=%d\n", x, y);

	cursor_x = x;
	cursor_y = y;
	
	if(EMPTY == chess[step][x][y]){ // 当前棋盘位置为空时才能操作
		// 判断棋子能否吃子之前，把棋盘的状态保存起来
		memcpy( (void *)chess[step+1], (void *)chess[step], 64*sizeof(int) );
		
		if(0 < judgeRule(x,y,8, (void *)chess[step+1], current_role, TRUE)){
			step++;          // 加1，这时step已经改变了，多了1
			update();
			change_role();   // 改变棋子的角色
		}
	}
	
	return FALSE;
}

/*
 * 功能：画图事件，主要是画黑白子
 */
gboolean draw_chess_event (GtkWidget* widget, GdkEventExpose* event, gpointer data)
{
    GdkGC* gc = gdk_gc_new (widget->window);
	// 得到画图区域的宽度和高度
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

// 倒计时处理函数
gboolean deal_count_time(gpointer data)
{
	count_time--;
	set_time_label(count_time);	// 显示
	if(0 == count_time){	// 时间到，改变角色
		change_role();
	}
	
	return TRUE;
}

// 机器下棋的处理函数
gboolean machine_chess(gpointer data)
{
	g_source_remove(machine_timer_id);	// 停止定时器
	int num = 0;
    int max = 0;
    int px, py;
	int i, j;
	
	// 判断棋盘上哪些位置是能够吃子的
    for(i= 0;i<8;i++){
        for(j=0;j<8;j++){
            if(chess[step][i][j] == EMPTY){  // 为棋盘空时，才能下子
                // 判断能不能吃子，只是判断，不改变棋盘状态，注意false参数
                num = judgeRule(i,j,8,(void*)chess[step],current_role,FALSE);
                if(num > max){   // 挑一个吃子最多的空格，并把坐标保存起来
                    max = num;  // 改变max的值
                    px = i;
                    py = j;
                }
            }
        }
    }
	
	if(0 == max){      // 到这一步max为0，说明方格里都不能吃子，这是就改变棋子的角色
        change_role(); // 改变棋子的角色
        return FALSE;       // 中断此函数
    }
	
	memcpy( (void *)chess[step+1], (void *)chess[step], 64*sizeof(int) );
	
	if(0 < judgeRule(px, py,8, (void *)chess[step+1], current_role, TRUE)){
			step++;          // 加1，这时step已经改变了，多了1
			update();
			change_role();   // 改变棋子的角色
			//gtk_widget_queue_draw_area(win_area, 0, 0, width, height); // update
			
	}
	
	return FALSE;
}

// 更新画图事件
void update()
{
	gtk_widget_queue_draw_area(win_area, 0, 0, width, height); // update
}

// 启动倒计时
void start_count_time()
{
	timer_id = g_timeout_add(1000, (GSourceFunc)deal_count_time, NULL);
}

// 停止倒计时
void stop_count_time()
{
	g_source_remove(timer_id);
	set_time_label(20);
}

// 停止机器时间
void stop_machine_time()
{
	g_source_remove(machine_timer_id);
}

// 设置是否按钮作战按钮
void set_press_mode(gboolean flag)
{
	is_press_mode = flag;
}

// 得到是否按钮作战按钮状态
gboolean get_press_mode()
{
	return is_press_mode;
}

// 设置作战模式
void set_pvp_mode(gboolean flag)	
{
	is_pvp = flag;
}

// 得到作战模式
gboolean get_pvp_mode()
{
	return is_pvp;
}

// 设置悔棋的步数
void set_goback_step(int temp)
{
	step -= temp;
}	

// 得到当前是第几步棋	
int get_chess_step()
{
	return step;
}
		
// 判断黑子或白字能否吃子
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

// 判断胜负
void judge_gameover()
{
	int black_num=0;
	int white_num=0;
	gboolean is_over = TRUE;
	
	int i, j;
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			// 统计黑白棋个数
			if(chess[step][i][j] == WHITE){      // 白子
				white_num++;
			}else if(chess[step][i][j] == BLACK){ // 黑子
               black_num++;
			}
			
			 if(0 < judgeRule(i,j,8, (void *)chess[step],WHITE, FALSE) ||
               0 < judgeRule(i,j,8, (void *)chess[step],BLACK, FALSE)){
					is_over = FALSE;
            }
		}
	}
	
	 // 分别显示黑白棋个数
	 set_num_label(black_num, white_num);
	 
    if(FALSE == is_over || 0 == step){	// 还没有到判断输赢的时候
        return;
    }
	
	////////////////////////////////////判读输赢
	////////////////////////////redo
	set_button_state(INIT);	// 重新初始化按钮
	set_press_mode(FALSE);	// 没有按下作战模式
	init_chess(FALSE);		// 棋盘状态清空
	set_tip_state(TSTOP);	// 停止提示角色
	stop_count_time();		// 停止倒计时
	hide_time_label();		// 隐藏倒计时显示控件
	hide_num_label();		// 隐藏显示个数控件
	stop_machine_time();	// 停止机器时间
	
    // 判断输赢
    if(black_num > white_num){
        show_info_dialog("黑子赢了！！！");
    }else if(black_num < white_num){
        show_info_dialog("白子赢了！！！");
    }else{
        show_info_dialog("平局了^_^！！！"); 
    }
	
	
}

/*
 *          功能：吃子规则
 *          x, y: 棋盘的位置，二维数组的下标
 *         chess: 二位数组首地址( (void *) chess[step] )
 *   currentRole：当前角色(黑子或白子)
 *      eatChess: TRUE，需要吃子；FALSE，只是判断能不能吃子
 *  返回值eatNum：吃子个数
 */
int judgeRule(int x, int y, int gridNum, void *chess, enum GridState currentRole,gboolean eatChess)
{
    // 棋盘的八个方向
    int dir[8][2]={{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    int temp_x = x, temp_y = y;             // 临时保存棋盘数组坐标位置
    int i = 0, eatNum = 0;                  // 初始化数据
    typedef int (*p)[gridNum];             // 自定义类型
    p chessBoard = (p)chess;                // 类型转换

    if(chessBoard[temp_x][temp_y] != EMPTY)  // 如果此方格内已有棋子，返回;
        return 0;

    // 棋盘的8个方向
    for(i = 0 ; i <8; i++)
    {
        temp_x += dir[i][0]; temp_y += dir[i][1];	// 准备判断相邻棋子
        // 如果没有出界，且相邻棋子是对方棋子，才有吃子的可能．
        if((temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)
            && (chessBoard[temp_x][temp_y] != currentRole) && (chessBoard[temp_x][temp_y] != EMPTY) )
        {
            temp_x += dir[i][0]; temp_y += dir[i][1];	            // 继续判断下一个，向前走一步
            while(temp_x < gridNum && temp_x >=0 && temp_y < gridNum && temp_y >= 0)
            {
                if(chessBoard[temp_x][temp_y] == EMPTY) // 遇到空位跳出
                        break;
                if(chessBoard[temp_x][temp_y] == currentRole)       // 找到自己的棋子，代表可以吃子
                {
                    if(eatChess == TRUE)                            // 确定吃子
                    {

                        chessBoard[x][y] = currentRole;          // 开始点标志为自己的棋子
                        temp_x -= dir[i][0]; temp_y -= dir[i][1];// 后退一步
                        while((temp_x != x )||(temp_y != y))	 // 只要没有回到开始的位置就执行
                        {
                            chessBoard[temp_x][temp_y] = currentRole;   // 标志为自己的棋子
                            temp_x -= dir[i][0]; temp_y -= dir[i][1];   // 继续后退一步
                            eatNum ++;  // 累计
                        }
                    }
                    else    // 不吃子，只是判断这个位置能不能吃子
                    {

                        temp_x -= dir[i][0]; temp_y -= dir[i][1];   // 后退一步
                        while((temp_x != x )||(temp_y != y))        // 只计算可以吃子的个数
                        {
                            temp_x -= dir[i][0]; temp_y -= dir[i][1];// 继续后退一步
                            eatNum ++;
                        }
                    }
                    break;  // 跳出循环
                }// 没有找到自己的棋子，就向前走一步
                temp_x += dir[i][0]; temp_y += dir[i][1];   // 向前走一步
            }
        }// 如果这个方向不能吃子，就换一个方向
        temp_x = x; temp_y = y;
    }

    return eatNum;              // 返回能吃子的个数
}
















/*
 * 功能：下子
 */
gboolean chess_cmd (int x, int y)
{
	if(x>=8 || x<0 || y>=8 || y<0)
		return FALSE;
	
	if(FALSE == is_press_mode){ // 如果没有按作战模式，最好有提示框
		//show_info_dialog("请选择作战模式(人人或单人)！！！");
		return FALSE;
	}
	
	// 如果人机模式，必须是机器下完，人才能下
	if(FALSE == is_pvp && WHITE == current_role){
        return  FALSE;
	}
	if(EMPTY == chess[step][x][y]){ // 当前棋盘位置为空时才能操作
		// 判断棋子能否吃子之前，把棋盘的状态保存起来
		memcpy( (void *)chess[step+1], (void *)chess[step], 64*sizeof(int) );
		if(0 < judgeRule(x,y,8, (void *)chess[step+1], current_role, TRUE)){
			step++;          // 加1，这时step已经改变了，多了1
			update();
			change_role();   // 改变棋子的角色
		}
	}
	
	return FALSE;
}

/*
* 获取外部命令，然后进行落子
*/
void *pthread_chess_recv_cmd(void *arg)
{
	int fd = open(CHESS_RECV_FIFO, O_RDWR);//打开与
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
			if(sscanf(buf, "key:%d", &key) == 1){	//命令格式
				//printf("key====%d\n", key);
				if(key==0){		//确定
					chess_cmd(cursor_x, cursor_y);
				}else if(key==1){	//上
					cursor_y>0 ? (cursor_y--) : 0;
				}else if(key==2){	//下
					cursor_y<7 ? (cursor_y++) : 0;
				}else if(key==3){	//左
					cursor_x>0 ? (cursor_x--) : 0;
				}else if(key==4){	//右
					cursor_x<7 ? (cursor_x++) : 0;
				}
				//printf("cursor_x==%d,cursor_y==%d\n", cursor_x, cursor_y);
				update();//更新绘图区域，把选中框显示
			}
		}
	}

	return NULL;
}








