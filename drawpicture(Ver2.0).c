#pragma warning(disable : 4996 )

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>

#define AREA_WIDTH_MAX 72
#define AREA_HEIGHT_MAX 42
#define FILE_CHS_MAX 50
#define FILENMME_EXT ".xgd"
#define SQUARE 1
#define DOUBLESPACE 0
#define CIRCLE 2
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define DRAW ' '
#define NODRAW 'o'
#define CLOSE 'Q'

static int area_width;
static int area_height;
long int move_len = 0;
char* move_ptr;
char* record_ptr_start;
char filename[FILE_CHS_MAX];
FILE* fp;

void get_area_width_height(void);
char* create_area_by_malloc(const int width, const int height);
void set_area_boundary_ran_first(const int width, const int height, char* ptr_area);
void move_block(char ch, const int width, const int height);
void output_display(const int width, const int height, const char* ptr_area);
char* s_gets(char* st, int n);
int save_and_quit(const int width, const int height);


int main(void)
{
	get_area_width_height();
	record_ptr_start = create_area_by_malloc(area_width, area_height);
	move_ptr = record_ptr_start;
	set_area_boundary_ran_first(area_width, area_height, record_ptr_start);
	while (1)
	{
		output_display(area_width, area_height, record_ptr_start);
		move_block(getch(), area_width, area_height);
		system("cls");
	}

	


	return 0;
}
void get_area_width_height(void)
{
	fputs("Enter the picture width (between 4~72):", stdout);
	while (scanf("%d", &area_width) != 1 || area_width < 4 || area_width>72 || area_width % 2 != 0)
	{
		fputs("Wrong! Enter width again:", stdout);
		while (getchar() != '\n')
			continue;
	}

	fputs("Enter the picture height (between 4~42):", stdout);
	while (scanf("%d", &area_height) != 1 || area_height < 4 || area_height>42 || area_height % 2 != 0)
	{
		fputs("Wrong! Enter height again:", stdout);
		while (getchar() != '\n')
			continue;
	}
}
char* create_area_by_malloc(const int width, const int height)
{
	char* ptr_area;
	ptr_area = (char*)malloc(sizeof(char) * width * height);
	if (ptr_area == NULL)
	{
		fprintf(stderr, "Faile to create memory of picture.\n");
		exit(EXIT_FAILURE);
	}
	memset(ptr_area, 0, sizeof(char) * width * height);

	return (ptr_area);
}
void set_area_boundary_ran_first(const int width, const int height, char* ptr_area)
{
	for (int j = 0; j < width; j++)
		ptr_area[j] = 1;
	for (int k = 0; k < height - 2; k++)
	{
		if (k == 0)
		{
			ptr_area[width] = ptr_area[width + width - 1] = 1;
			continue;
		}
		ptr_area[width * (k + 1)] = ptr_area[width * (k + 1) + width - 1] = 1;
	}
	for (int j_2 = 0; j_2 < width; j_2++)
		ptr_area[width * height - 1 - j_2] = 1;

	ptr_area[width + 1] = 2;
}
void output_display(const int width, const int height, const char* ptr_area)
{
	for (long int n = 0; n < width * height; n++)
	{
		if ((n % width ) == 0 && n != 0)
			putchar('\n');
		switch (ptr_area[n])
		{
		case DOUBLESPACE:printf("  "); break;
		case SQUARE:printf("■"); break;
		case CIRCLE:printf("◎"); break;
		default:printf("  "); break;
		}
	}
}
void move_block(char ch, const int width, const int height)
{
	
	const long int move_left_max = -(width + 1);
	const long int move_right_max = +(width * height - (width + 2));
	move_ptr += (width + 1);
	
	if (ch == UP)
	{
		move_len += (-(width));
		if (move_len<move_left_max || move_len>move_right_max)
		{
			move_len -= (-(width));
			goto move_end;
		}
		move_ptr += (-(width));
	}
	else if (ch == DOWN)
	{
		move_len += width;
		if (move_len<move_left_max || move_len>move_right_max)
		{
			move_len -= width;
			goto move_end;
		}
		move_ptr += width;
	}
	else if (ch == LEFT)
	{
		move_len += (-(1));
		if (move_len<move_left_max || move_len>move_right_max)
		{
			move_len -= (-(1));
			goto move_end;
		}
		move_ptr += (-(1));
	}
	else if (ch == RIGHT)
	{
		move_len += 1;
		if (move_len<move_left_max || move_len>move_right_max)
		{
			move_len -= 1;
			goto move_end;
		}
		move_ptr += 1;
	}

move_end:
	*move_ptr = CIRCLE;
	switch (getch())
	{
	case DRAW:*move_ptr = 1; break;
	case NODRAW:*move_ptr = 0; break;
	case CLOSE:exit(1); break;
	}
}
char* s_gets(char* st, int n)
{
	char* ret_val;
	char* find;

	ret_val = fgets(st, n, stdin);
	if (ret_val)
	{
		find = strchr(st, '\n');
		if (find)
			*find = '\0';
		else
			while (getchar() != '\n')
				continue;
	}

	return ret_val;
}
int save_and_quit(const int width, const int height)
{
	fputs("Enter the file name:", stdout);
	s_gets(filename, FILE_CHS_MAX - strlen(FILENMME_EXT) - 5);
	strcat(filename, FILENMME_EXT);
	if ((fp = fopen(filename, "wb")) == NULL)
	{
		fprintf(stderr, "Faile to create the picture.\n");
		return 0;
	}
	fwrite(record_ptr_start, sizeof(char), width * height, fp);

	return 1;
}
