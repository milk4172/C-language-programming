#pragma warning(disable : 4996 )

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define USR_NAME 11
#define USR_PASSWORDS 11

void get_usr_enter(void);
int check_usr_name_passwords(const char*, const char*);
long int get_lines(void);
char* create_file_copy_to_str(void);
char** create_ptr_str_loc(long int, char*);

static FILE* fp_usr_and_passwords;
static char usr_enter_name[USR_NAME];
static char usr_enter_passwords[USR_PASSWORDS];

int main(void)
{
	long int lines = 0;
	char* ptr_start_copyed_file;
	char** ptr_usr_psaawords_str_loc;

	get_usr_enter();
	while (!check_usr_name_passwords(usr_enter_name, usr_enter_passwords))
	{
		puts("Wrong!");
		get_usr_enter();
	}
		
	if ((fp_usr_and_passwords = fopen("USR_AND_PASSWORDS.txt", "r")) == NULL)
	{
		fprintf(stderr, "Fail to open the file.\n");
		exit(EXIT_FAILURE);
	}

	lines = get_lines();
	fseek(fp_usr_and_passwords, 0L, SEEK_SET);
	ptr_start_copyed_file = create_file_copy_to_str();
	ptr_usr_psaawords_str_loc = create_ptr_str_loc(lines, ptr_start_copyed_file);

	for (long int seek = 0; seek < lines; seek++)
	{
		if (!strcmp(usr_enter_name, ptr_usr_psaawords_str_loc[seek]) && seek < lines - 1)
		{
			if (!strcmp(usr_enter_passwords, ptr_usr_psaawords_str_loc[++seek]))
			{
				puts("There is a result!");
				break;
			}
			else
			{
				seek--;
				continue;
			}
		}
		if (seek == lines - 1)
			puts("There is no result!");
	}

	fclose(fp_usr_and_passwords);
	

	return 0;
}
void get_usr_enter(void)
{
	fputs("Enter the user name:", stdout);
	fgets(usr_enter_name, USR_NAME, stdin);
	while (getchar() != '\n')
		continue;

	fputs("Enter the user passwords:", stdout);
	fgets(usr_enter_passwords, USR_PASSWORDS, stdin);
	while (getchar() != '\n')
		continue;
}
int check_usr_name_passwords(const char* name, const char* passwords)
{
	int result = 0;

	for (int n = 0; n < USR_NAME - 1; n++)
	{
		if (!isalnum(name[n]) || !isalnum(passwords[n]))
		{
			result = 0;
			goto end;
		}
	}

	result = 1;

	end:
	return (result);
}
long int get_lines(void)
{
	long int lines = 0;
	int ch;

	while ((ch = getc(fp_usr_and_passwords)) != EOF)
	{
		if (ch == '\n')
			lines++;
	}

	return (lines);
}
char* create_file_copy_to_str(void)
{
	int ch;
	long int ch_count = 0;
	char* ptr_start_copyed_file;

	while ((ch = getc(fp_usr_and_passwords)) != EOF)
		ch_count++;
	ptr_start_copyed_file = (char*)malloc(ch_count * sizeof(char));//*********:free!!!
	if (ptr_start_copyed_file == NULL)
	{
		fprintf(stderr, "Fail to create memory(USR_PASS).\n");
		exit(EXIT_FAILURE);
	}
	rewind(fp_usr_and_passwords);

	get_ch_again:	while ((ch = getc(fp_usr_and_passwords)) != EOF)
	{
		if ((char)ch == '\n')
		{
			*ptr_start_copyed_file++ = '\0';
			goto get_ch_again;
		}
		*ptr_start_copyed_file++ = (char)ch;
	}
	ptr_start_copyed_file -= ch_count;

	return (ptr_start_copyed_file);
}
char** create_ptr_str_loc(long int lines, char* ptr_start_copyed_file)
{
	char** ptr_usr_psaawords_str_loc;
	int ch;
	long int ch_count = 0;

	fseek(fp_usr_and_passwords, 0L, SEEK_SET);
	ptr_usr_psaawords_str_loc = (char**)malloc(lines * sizeof(char**));//**************free!!!
	if (ptr_usr_psaawords_str_loc == NULL)
	{
		fprintf(stderr, "Fail to create memory(pointer_to_usr_pass).\n");
		exit(EXIT_FAILURE);
	}
	memset(ptr_usr_psaawords_str_loc, 0, lines * sizeof(char**));
	while ((ch = getc(fp_usr_and_passwords)) != EOF)
		ch_count++;

	for (long int n = 0; n < ch_count / USR_NAME; n++)
	{
		*ptr_usr_psaawords_str_loc = ptr_start_copyed_file;
		ptr_usr_psaawords_str_loc++;
		ptr_start_copyed_file += USR_NAME;
	}
	ptr_usr_psaawords_str_loc -= ch_count / USR_NAME;

	return (ptr_usr_psaawords_str_loc);
}