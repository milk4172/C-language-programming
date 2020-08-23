#pragma warning(disable : 4996 )

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#define USR_NAME 11
#define USR_PASSWORD 11
#define PASSWORDS "AKB480204"
#define PASSWORDS_LEN 10

int get_check_passwords(void);
int get_usr_num(void);
void create_rand_usr_pass(char*, int);
char* s_gets(char* st, int n);

int main(void)
{
	int usr_num;
	char* ptr_usr_passwords;
	FILE* newfile;
	int check = 3;
	
	while (check >= 0)
	{
		if (get_check_passwords())
			break;
		fprintf(stdout, "Passwords is wrong! enter again:");
		check--;
		if (check == 0)
		{
			fprintf(stdout, "You can't enter anain.(Enter to quit)");
			getchar();
			exit(EXIT_FAILURE);
		}
	}

	if ((newfile = fopen("USR_AND_PASSWORDS.txt", "w+")) == NULL)
	{
		fprintf(stderr, "Fail to create \"USR_AND_PASSWORDS.txt\"!");
		exit(EXIT_FAILURE);
	}
	usr_num = get_usr_num();
	ptr_usr_passwords = (char*)malloc((USR_NAME + USR_PASSWORD) * sizeof(char) * usr_num);
	if (ptr_usr_passwords != NULL)
	{
		memset(ptr_usr_passwords, '*', (USR_NAME + USR_PASSWORD) * sizeof(char) * usr_num);
		create_rand_usr_pass(ptr_usr_passwords, usr_num);
	}
	else
	{
		fprintf(stderr, "Fail to create memory.");
		exit(EXIT_FAILURE);
	}

	for (int count = 0; count < (USR_NAME + USR_PASSWORD) * sizeof(char) * usr_num; count++)
		putc(*ptr_usr_passwords++, newfile);
	ptr_usr_passwords -= (USR_NAME + USR_PASSWORD) * sizeof(char) * usr_num;
	
	fclose(newfile);
	free(ptr_usr_passwords);
	ptr_usr_passwords = NULL;

	return 0;
}

int get_check_passwords(void)
{
	char passwords[PASSWORDS_LEN];
	int result;

	fputs("Enter the passwords:", stdout);
	s_gets(passwords, PASSWORDS_LEN);
	passwords[PASSWORDS_LEN - 1] = '\0';
	if (!strcmp(passwords, PASSWORDS))
		result = 1;
	else
		result = 0;

	return result;
}
int get_usr_num(void)
{
	int user_num;
	fputs("Enter the number of user(passwords) you want to create:", stdout);
	while (scanf("%d", &user_num) != 1 || user_num <= 0)
	{
		puts("Wrong! enter again:");
		while (getchar() != '\n')
			continue;
	}
	while (getchar() != '\n')
		continue;

	return user_num;
}
void create_rand_usr_pass(char* ptr, int user_num)
{
	char ch;

	srand((unsigned int)time(NULL));
	for (int n = 0; n < user_num; n++)
	{
		for (int x = 0; x < USR_NAME - 1; x++)
		{
			while (!isalnum(ch = rand() % 'z' + 1))
				continue;
			*ptr = ch;
			ptr++;
		}
		*ptr++ = '\n';
		for (int y = 0; y < USR_PASSWORD - 1; y++)
		{
			while (!isalnum(ch = rand() % 'z' + 1))
				continue;
			*ptr = ch;
			ptr++;
		}
		*ptr++ = '\n';
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