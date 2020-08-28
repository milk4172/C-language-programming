#pragma warning(disable : 4996 )

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#define AREA_WIDTH 72
#define AREA_HEIGHT 42
#define FILE_CHS_MAX 50
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define DRAW ' '
#define NODRAW 'o'
#define CLOSE 'Q'
#define FILENMME_EXT ".xgd"

void set_boundary(void);
void output_display(void);
void rand_set(void);
int block_move_draw(int x, int y);
int save_and_quit(void);
char* s_gets(char* st, int n);

char draw_area[AREA_HEIGHT][AREA_WIDTH];
int set_j = 0, set_k = 0;
char filename[FILE_CHS_MAX];
FILE* fp;

int main(void)
{
	memset(draw_area, 0, sizeof(draw_area));
	fputs("Enter Q to quit.", stdout);
	set_boundary();
	rand_set();
	if (block_move_draw(set_j, set_k))
	{
		if (!save_and_quit())
			puts("Result:Failed.");
	}

	return 0;
}
void set_boundary(void)
{
	for (int j = 0; j < AREA_HEIGHT; j++)
		draw_area[j][0] = draw_area[j][AREA_WIDTH - 1] = 1;
	for (int k = 0; k < AREA_WIDTH; k++)
		draw_area[0][k] = draw_area[AREA_HEIGHT - 1][k] = 1;
}
void rand_set(void)
{
	srand((unsigned int)time(NULL));

get_j_again: set_j = rand() % AREA_HEIGHT + 1;
	if (set_j > AREA_HEIGHT - 2)
		goto get_j_again;

get_k_again: set_k = rand() % AREA_WIDTH + 1;
	if (set_k > AREA_WIDTH - 2)
		goto get_k_again;

	draw_area[set_j][set_k] = 1;
}
void output_display(void)
{
	fputs("Enter Q to quit.\n", stdout);
	for (int x = 0; x < AREA_HEIGHT; x++)
	{
		for (int y = 0; y < AREA_WIDTH; y++)
		{
			if (draw_area[x][y] == 1)
				printf("■");
			else if (draw_area[x][y] == 2)
				printf("◎");
			else
				printf("  ");
		}
		putchar('\n');
	}
}
int block_move_draw(int x, int y)
{
	char ch;

draw_again:

	if ((ch = getch()) == 'w')
	{
		if (x == 1)
			goto move_end;
		x--;
	}
	else if (ch == 's')
	{
		if (x == AREA_HEIGHT - 2)
			goto move_end;
		x++;
	}
	else if (ch == 'a')
	{
		if (y == 1)
			goto move_end;
		y--;
	}
	else if (ch == 'd')
	{
		if (y == AREA_WIDTH - 2)
			goto move_end;
		y++;
	}
move_end:
	draw_area[x][y] = 2;
	output_display();
	switch (getch())
	{
	case DRAW:draw_area[x][y] = 1; break;
	case NODRAW:draw_area[x][y] = 0; break;
	case CLOSE:return 1; break;
	}
	system("cls");
	output_display();
	goto draw_again;
}
int save_and_quit(void)
{
	fputs("Enter the file name:", stdout);
	s_gets(filename, FILE_CHS_MAX - strlen(FILENMME_EXT) - 5);
	strcat(filename, FILENMME_EXT);
	if ((fp = fopen(filename, "wb")) == NULL)
	{
		fprintf(stderr, "Faile to create the picture.\n");
		return 0;
	}
	fwrite(draw_area, sizeof(char), sizeof(draw_area), fp);

	return 1;
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
