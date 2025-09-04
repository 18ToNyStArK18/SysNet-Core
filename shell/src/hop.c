#include "../include/include.h"
#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <linux/limits.h>
int hop(char *command, char *prev, char *home_path)
{
	char *temp = (char *)malloc(sizeof(char) * PATH_MAX + 1);
	if (getcwd(temp, PATH_MAX) == NULL)
	{
		perror("ERROR FINDING THE CURR_PATH");
	}
	char *before_hop = (char *)malloc(PATH_MAX + 1);
	strcpy(before_hop, temp);
	int n = strlen(command);
	n--;
	while (command[n] == ' ')
		n--;
	n++;
	char *Home_dir = (char *)malloc(sizeof(char) * PATH_MAX);
	strcpy(Home_dir, home_path);
	int temp_ = 3;
	if (temp_ < n && command[temp_] == ' ') // trailing spaces
		temp_++;
	if (temp_ == n)
	{

		strcpy(prev, temp); // input is just hop so go to home directory
		if (chdir(Home_dir) == -1)
			return -1;
	}
	for (int i = 3; i < n; i++)
	{
		if (i < n && command[i] == ' ') // spaces
			i++;
		if (i < n && command[i] == '~')
		{

			if (chdir(Home_dir) == -1){
				chdir(before_hop);
				return -1;
			}
		}
		else if (i < n && command[i] == '-' && (i + 1 == n || command[i + 1] == ' '))
		{
			if (strlen(prev) == 0)
			{
				return 1;
			}
			chdir(prev);
		}
		else
		{
			if (i < n && command[i] == '.' && (i + 1 == n || command[i + 1] == ' '))
			{
				if (chdir(".") == -1)
					return -1;
			}
			else if (i + 1 < n && command[i] == '.' && command[i + 1] == '.' && (i + 2 == n || command[i + 2] == ' '))
			{
				if (chdir("..") == -1)
				{
					chdir(before_hop);
					return -1;
				}
			}
			else
			{
				char *temp = malloc(sizeof(char) * n);
				int temp_counter = 0;
				while (i < n && command[i] != ' ')
				{
					temp[temp_counter++] = command[i++];
				}
				temp[temp_counter] = '\0';
				if (chdir(temp) == -1)
				{
					chdir(before_hop);
					return -1;
				}
			}
		}
	}
	strcpy(prev, temp);
	return 1;
}
