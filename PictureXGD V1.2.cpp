/*2021.2.21*/

#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <string.h>
#include <string>

const int Width_Max = 80;
const int Width_Min = 4;
const int Height_Max = 80;
const int Height_Min = 4;
const int Select_menu_count = 2;
const char* Menu[Select_menu_count] = { " (1) Open a .xgd picture",
                                        " (2) Create a .xgd picture" };// Select_menu_count must pair to Menu[?]

const char K_space = ' ';
const char K_up = 'w';
const char K_down = 's';
const char K_right = 'd';
const char K_left = 'a';
const char K_quit = 'q';
const char K_save = 'm';
const std::string File_name_extern = ".xgd";
FILE* fp_savefile;

enum {            
	NON,
	BLOCK_BOUNDARY,
	BLOCK_CIRCLE_BLACK,
	BLOCK_CIRCLE_WHITE,
	BLOCK_SQUARE_BLACK,
	BLOCK_SQUARE_WHITE,
	BLOCK_TRIANGLE_BLACK,
	BLOCK_TRIANGLE_WHITE,
	START
};

struct MapSize {
	int Width;
	int Height;
};

int SelectMenu(void);
void SetBoundary(const MapSize mapsize, char* const mapstart);
char* AllocatePictureMem(const MapSize mapsize);
void WelcomeDrawPicture(void);
void PrintTips(void);
void GetMapSize(MapSize&);
bool Move_Draw(char* const map_start, const int start, const MapSize mapsize);
void CleanStdIn(void);
void OutPicture(const char*, const int, const int);
std::string GetFileName(void);

int main(void)
{
	MapSize map;
	int menu = 0;
	menu = SelectMenu();

	if (menu == 1)
	{



	}
	else if (menu == 2)
	{
		WelcomeDrawPicture();
		GetMapSize(map);
		char* ptr_map = AllocatePictureMem(map);
		SetBoundary(map, ptr_map);
		while (Move_Draw(ptr_map, map.Width + 1, map));
		free((char*)ptr_map);
	}


	return 0;
}

int SelectMenu(void)
{
	char user_enter;
	int ptr_now = 0;

	char* ptr_menu = (char*)malloc(sizeof(char) * Select_menu_count);
	if (!ptr_menu)
	{
		std::cout << "Fail to create menu.";
		exit(EXIT_FAILURE);
	}
	memset(ptr_menu, ' ', sizeof(char) * Select_menu_count);

	printf("Enter \'%c\' and \'%c\' to up and down, enter key to select.\n", K_up, K_down);
	ptr_menu[0] = '*';
	for (int n = 0; n < Select_menu_count; n++)
		std::cout << ptr_menu[n] << Menu[n] << std::endl;
	
	while ((user_enter = getchar()) != '\n')
	{
		if (user_enter == K_down)
		{
			if (ptr_now >= Select_menu_count - 1)
			{
				CleanStdIn();
				continue;
			}
			ptr_menu[ptr_now] = ' ';
			ptr_menu[++ptr_now] = '*';
		}
		else if (user_enter == K_up)
		{
			if (ptr_now <= 0)
			{
				CleanStdIn();
				continue;
			}
			ptr_menu[ptr_now] = ' ';
			ptr_menu[--ptr_now] = '*';
		}

		system("cls");
		CleanStdIn();
		printf("Enter \'%c\' and \'%c\' to up and down, enter key to select.\n", K_up, K_down);
		for (int n = 0; n < Select_menu_count; n++)
			std::cout << ptr_menu[n] << Menu[n] << std::endl;
	}
	free((char*)ptr_menu);

	// user enter enter_key
	return ++ptr_now;
}

char* AllocatePictureMem(const MapSize mapsize)
{
	char* ptr_map = (char*)malloc(sizeof(char) * mapsize.Width * mapsize.Height);
	if (!ptr_map)
	{
		std::cout << "Fail to allocate the memory.";
		exit(EXIT_FAILURE);
	}
	memset(ptr_map, NON, mapsize.Width * mapsize.Height * sizeof(char));

	return ptr_map;
}

void SetBoundary(const MapSize mapsize, char* const mapstart)
{
	for (int n = 0; n < mapsize.Width; n++)	//Set top and down boundary 
		mapstart[n] = mapstart[mapsize.Height * mapsize.Width - 1 - n] = BLOCK_BOUNDARY;
	for (int n = 1; n <= mapsize.Height - 2; n++)//Set two side boundary
		mapstart[n * mapsize.Width] = mapstart[(n + 1) * mapsize.Width - 1] = BLOCK_BOUNDARY;
	mapstart[mapsize.Width + 1] = START;//Set start location,  map.Width+1 is START
}

void WelcomeDrawPicture(void)
{
	system("cls");
	std::cout << "Welcome to PictureXGD V1.0\n";
	PrintTips();
	std::cout << "\nFirst enter the  picture width and height. :-)\n";
}

void PrintTips(void)
{
	printf("       __________________________________________________________________________\n");
	printf("Tips:  |  use \'%c\', \'%c\', \'%c\', \'%c\' to move up, left, down, right.\n", K_up, K_left, K_down, K_right);
	printf("       | use space key to change the block.\n");
	printf("       | use \'%c\' to quit, and \'%c\' to save.\n", K_quit, K_save);
	printf("       __________________________________________________________________________\n");
}

void GetMapSize(MapSize& mapsize)
{
	using std::cout;
	using std::endl;
	cout << "Enter the map width(" << Width_Max << "~" << Width_Min << "): ";
	while (scanf("%d", &mapsize.Width) != 1 || mapsize.Width < Width_Min || mapsize.Width>Width_Max)
	{
		cout << "\nWrong! Enter map width again: ";
		CleanStdIn();
	}
	cout << "Map width is " << mapsize.Width << ". Then enter the map height(" << Height_Min << "~" << Height_Max << "): ";
	while (scanf("%d", &mapsize.Height) != 1 || mapsize.Height<Height_Min || mapsize.Height>Height_Max)
	{
		cout << "\nWrong! Enter map height again: ";
		CleanStdIn();
	}

	cout << "\nMap width:" << mapsize.Width << " Map height:" << mapsize.Height << endl;
}

bool Move_Draw(char* const map_start, const int start, const MapSize mapsize)//map_star: the map address, star:～ location, mapsize: width,height
{
	char usr_enter = 0;
	int address_now = start;//record the start, then use address_now to record address and move it
	
	while ((usr_enter = getchar()) != K_quit && usr_enter != K_save)
	{
		if (usr_enter == K_up)
		{
			if (map_start[address_now] == BLOCK_BOUNDARY || (address_now > mapsize.Width&& address_now < mapsize.Width * 2 - 1))//cant move block up
				continue;
			else
			{
				address_now -= mapsize.Width;
				continue;
			}

		}
		else if (usr_enter == K_down)
		{
			if (map_start[address_now] == BLOCK_BOUNDARY
				|| (address_now<mapsize.Width * (mapsize.Height - 1) - 1 && address_now>mapsize.Width* (mapsize.Height - 2)))
				continue;
			else
			{
				address_now += mapsize.Width;
				continue;
			}
		}
		else if (usr_enter == K_left)
		{
			if (map_start[address_now] == BLOCK_BOUNDARY || map_start[address_now - 1] == BLOCK_BOUNDARY)
				continue;
			else
			{
				address_now--;
				continue;
			}

		}
		else if (usr_enter == K_right)
		{
			if (map_start[address_now] == BLOCK_BOUNDARY || map_start[address_now + 1] == BLOCK_BOUNDARY)
				continue;
			else
			{
				address_now++;
				continue;
			}

		}
		else if (usr_enter == K_space)
		{
			if (map_start[address_now] == BLOCK_BOUNDARY)
				continue;
			else
			{
				switch (map_start[address_now])
				{
				case NON:map_start[address_now] = BLOCK_CIRCLE_BLACK; break;
				case BLOCK_CIRCLE_BLACK:map_start[address_now] = BLOCK_CIRCLE_WHITE; break;
				case BLOCK_CIRCLE_WHITE:map_start[address_now] = BLOCK_SQUARE_BLACK; break;
				case BLOCK_SQUARE_BLACK:map_start[address_now] = BLOCK_SQUARE_WHITE; break;
				case BLOCK_SQUARE_WHITE:map_start[address_now] = BLOCK_TRIANGLE_BLACK; break;
				case BLOCK_TRIANGLE_BLACK:map_start[address_now] = BLOCK_TRIANGLE_WHITE; break;
				case BLOCK_TRIANGLE_WHITE:map_start[address_now] = BLOCK_CIRCLE_BLACK; break;
				}
				continue;	
			}
		}
		else if (usr_enter == K_quit)
			break;
		
		system("cls");
		PrintTips();
		OutPicture(map_start, mapsize.Width, mapsize.Height);
		//CleanStdIn();//while user enter like "sssaq", but enter key must ENTER TWICE!!!
	}
	CleanStdIn();
	// usr_enter=K_quit
	printf("If you want to save the picture, enter \'%c\'\nOther key will quit with no save.\n", K_save);
	//if enter K_save, save the data
	if (getchar() == K_save)
	{
		CleanStdIn();
		const std::string S_filename = GetFileName();
		const char* C_filename = S_filename.c_str();
		if ((fp_savefile = fopen(C_filename, "wb")) == NULL)
		{
			std::cout << "Faile to creat file \"" << C_filename << "\" ." << std::endl;
			return false;
		}
		fwrite(map_start, sizeof(char), mapsize.Height * mapsize.Width, fp_savefile);
		fclose(fp_savefile);
	}

	return false;
}

std::string GetFileName(void)
{
	std::string filename;
	std::cout << "Enter file name: ";

Enter_Again:
	while (!(std::cin >> filename))
	{
		std::cout << "Filed! Enter anain!";
		std::cin.clear();
	}
	if (filename.length() >= FILENAME_MAX - File_name_extern.length() - 1)
	{
		printf("\nFile name is too long ! File Name MAX is %d. ", FILENAME_MAX - File_name_extern.length() - 1);
		std::cout << "Enter file name again: ";
		goto Enter_Again;
	}
	filename += File_name_extern;

	return filename;
}

void OutPicture(const char* map_address, const int w, const int h)
{
	using std::cout;

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
			switch (*map_address++)
			{	
			case NON:cout << "  "; break;
			case BLOCK_BOUNDARY: cout << "＊"; break;
			case BLOCK_CIRCLE_BLACK:cout << "●"; break;
			case BLOCK_CIRCLE_WHITE:cout << "○"; break;
			case BLOCK_SQUARE_BLACK:cout << "■"; break;
			case BLOCK_SQUARE_WHITE:cout << "□"; break;
			case BLOCK_TRIANGLE_BLACK:cout << "▲"; break;
			case BLOCK_TRIANGLE_WHITE:cout << "△"; break;
			case START:cout << "～"; break;
			}
		cout << "\n";
	}
}

void CleanStdIn(void)
{
	while (getchar() != '\n')
		continue;
}
