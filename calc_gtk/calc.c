#include<gtk/gtk.h>
#include<string.h>
#include<stdlib.h>

char *other[] = {"Backspace", "CE", "=", "0", "exit", ".", "/", "*", "-", "+"};

GtkWidget *entry = NULL;//行编辑

gboolean flag = FALSE;//是否点击了+-*/
char total_data[200] = "";
char operation[100] = "";

int sungtk_widget_set_font_size(GtkWidget *widget, int size, gboolean is_button)
{
	GtkWidget *labelChild;  
	PangoFontDescription *font;  
	gint fontSize = size;  
	if(widget == NULL)
		return -1;
  
	font = pango_font_description_from_string("Sans");          //"Sans"字体名   
	pango_font_description_set_size(font, fontSize*PANGO_SCALE);//设置字体大小   
	
	if(is_button){
		labelChild = gtk_bin_get_child(GTK_BIN(widget));//取出GtkButton里的label  
	}else{
		labelChild = widget;
	}
	
	//设置label的字体，这样这个GtkButton上面显示的字体就变了
	gtk_widget_modify_font(GTK_WIDGET(labelChild), font);
	pango_font_description_free(font);

	return 0;
}

void back_fun(const gchar *entry_text, const gchar *btn_text)
{
	if(entry_text[0] != '\0')
	{
		gchar temp[100] = "";
		strncpy(temp, entry_text, strlen(entry_text)-1);
		gtk_entry_set_text(GTK_ENTRY(entry), temp);
	}
}

void clear_fun(const gchar *entry_text, const gchar *btn_text)
{
	gtk_entry_set_text(GTK_ENTRY(entry), "");
	memset(total_data, 0, sizeof(total_data));
	memset(operation, 0, sizeof(operation));
	flag = 0;
}

void equal_fun(const gchar *entry_text, const gchar *btn_text)
{
  	char *data[100] = {NULL};
	memset(data, 0, sizeof(data));
	int i = 0;
	float result = 0.0;
	float num = 0.0;
	int j = 0;
	//strcpy(total_data, entry_text);
	strcat(total_data, entry_text);
	data[0] = total_data;
	//printf("entry_text=======%s-----\n", total_data);
	while((data[i] = strtok(data[i], "+-/*")) != NULL)
	{
		num = atof(data[i]);
		//printf("num===%f\n", num);
		if(i == 0){
			result = num;
			i++;
			continue;
		}
		if(operation[j] == '+'){
			//printf("re===%f, num==%f\n", result, num);
			result += num;
			j++;
		}else if(operation[j] == '-'){
			result -= num;
			j++;
		}else if(operation[j] == '*'){
			result *= num;
			j++;
		}else if(operation[j] == '/'){
			if(num != 0){
				result /= num;
			}
			j++;
		}
		i++;
	}
	char show_data[100] = "";
	sprintf(show_data, "%f", result);
	gtk_entry_set_text(GTK_ENTRY(entry), show_data);
	memset(total_data, 0, sizeof(total_data));
	memset(operation, 0, sizeof(operation));
}

void sign_fun(const gchar *entry_text, const gchar *btn_text)
{
	gtk_main_quit();
}

void operation_fun(const gchar *entry_text, const gchar *btn_text)
{
	operation[strlen(operation)] = btn_text[0];
	//gtk_entry_append_text(GTK_ENTRY(entry), btn_text);
	strcat(total_data, entry_text);
	strcat(total_data, btn_text);
	flag = TRUE;
}

typedef void (*FUNC)(const gchar *entry_text, const gchar *btn_text);
typedef struct
{
	char text[20];
	FUNC fun;
}CALC;

CALC calc[] = {
		{"Backspace", back_fun},
		{"CE", clear_fun},
		{"=", equal_fun},
		{"exit", sign_fun},
		{"/", operation_fun},
		{"*", operation_fun},
		{"-", operation_fun},
		{"+", operation_fun}
};


void callback_pressed(GtkWidget *widget, gpointer data)
{
	const gchar *text = gtk_button_get_label(GTK_BUTTON(widget));
	const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
	int i;
	for(i=0; i<sizeof(calc)/sizeof(CALC); i++)
	{
		if(strcmp(text, calc[i].text)==0)
		{
			calc[i].fun(entry_text, text);
			return;
		}
	}
	if(flag == TRUE){
		gtk_entry_set_text(GTK_ENTRY(entry), "");
		flag = 0;
	}
	gtk_entry_append_text(GTK_ENTRY(entry), text);
}

void button_num_add(GtkWidget *table)
{
 	int i, j;
	int index = 1;
	for(i=3; i>0; i--)
	{
		for(j=0; j<3; j++)
		{
			char num_buf[3] = "";
			sprintf(num_buf, "%d", index++);
			GtkWidget *button = gtk_button_new_with_label(num_buf);
			sungtk_widget_set_font_size(button, 13, TRUE);
			gtk_table_attach_defaults(GTK_TABLE(table), button, j, j+1, i-1, i);
			
			g_signal_connect(button, "pressed", G_CALLBACK(callback_pressed), NULL);
		}
	}
}


int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window, 800, 480);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	GtkWidget *table_bg = gtk_table_new(6, 4, TRUE);
	gtk_table_set_row_spacings(GTK_TABLE(table_bg), 3);
	gtk_table_set_col_spacings(GTK_TABLE(table_bg), 3);
	gtk_container_add(GTK_CONTAINER(window), table_bg);
	
	entry = gtk_entry_new();
	gtk_widget_set_size_request(entry, 400, 50);
	sungtk_widget_set_font_size(entry, 15, FALSE);
	gtk_entry_set_alignment(GTK_ENTRY(entry), 1.0);
	gtk_table_attach_defaults(GTK_TABLE(table_bg), entry, 0, 4, 0, 1);
	
	GtkWidget *button[10] = {NULL};
	int i;
	for(i=0;i<10;i++)
	{
		button[i] = gtk_button_new_with_label(other[i]);
		sungtk_widget_set_font_size(button[i], 13, TRUE);
		g_signal_connect(button[i], "pressed", G_CALLBACK(callback_pressed), NULL);
	}
	gtk_table_attach_defaults(GTK_TABLE(table_bg), button[0], 0, 2, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(table_bg), button[1], 2, 3, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(table_bg), button[2], 3, 4, 1, 2);
	
	gtk_table_attach_defaults(GTK_TABLE(table_bg), button[3], 0, 1, 5, 6);
	gtk_table_attach_defaults(GTK_TABLE(table_bg), button[4], 1, 2, 5, 6);
	gtk_table_attach_defaults(GTK_TABLE(table_bg), button[5], 2, 3, 5, 6);
	
	gtk_table_attach_defaults(GTK_TABLE(table_bg), button[6], 3, 4, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE(table_bg), button[7], 3, 4, 3, 4);
	gtk_table_attach_defaults(GTK_TABLE(table_bg), button[8], 3, 4, 4, 5);
	gtk_table_attach_defaults(GTK_TABLE(table_bg), button[9], 3, 4, 5, 6);
	
	GtkWidget *table_num = gtk_table_new(3, 3, TRUE);//
	gtk_table_set_row_spacings(GTK_TABLE(table_num), 3);//表格行间距
	gtk_table_set_col_spacings(GTK_TABLE(table_num), 3);
	gtk_table_attach_defaults(GTK_TABLE(table_bg), table_num, 0, 3, 2, 5);
	button_num_add(table_num);

	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}
