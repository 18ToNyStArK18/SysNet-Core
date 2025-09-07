#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include "../include/include.h"
#include <fcntl.h>
#include <unistd.h>
int job_count=0;
char *FindPath(char *home_path)
{
	char *working_dir = (char *)malloc(sizeof(char) * PATH_MAX);
	if (getcwd(working_dir, PATH_MAX) == NULL)
	{
		perror("Error finding the curr_dir\n");
	}
	char *req_dir = (char *)malloc(sizeof(char *) * PATH_MAX);
	req_dir[0] = '~';
	int h = strlen(home_path), w = strlen(working_dir);
	if (strncmp(working_dir, home_path, h) == 0)
	{
		for (int i = h; i < w; i++)
		{
			req_dir[i - h + 1] = working_dir[i];
		}
		req_dir[w - h + 1] = '\0';
		return req_dir;
	}
	else
		return working_dir;
}

int main()
{
	char *username = getlogin();
	struct utsname sys; // for sysname and all
	char *home_path = (char *)malloc(PATH_MAX);
	if (getcwd(home_path, PATH_MAX) == NULL) // the curr_directory when the code is run is considered as home directory
		perror("Error finding home dir\n");
	char *path_req = FindPath(home_path);
	char *prev = (char *)malloc(sizeof(char) * PATH_MAX);
	prev[0] = '\0';
	while (1)
	{
		// prompting of our shell
		printf("<%s", username);
		if (uname(&sys) == 0)
		{
			printf("@%s:", sys.sysname);
		}
		printf("%s>", path_req);
		char *command = (char *)malloc(sizeof(char) * 4097);
		char *cmd_refined = (char *)malloc(sizeof(char) * 4097);

		if(scanf(" %[^\n]", command)==EOF){
			kill_jobs(home_path);
			printf("logout\n");
				exit(0);
		}
		int counter = 0, i = 0;
		int n = strlen(command);
		int found_space = 0;
		while (i < n)
		{
			found_space = 0;
			if (command[i] == '"')
			{
				cmd_refined[counter++] = '"';
				i++;
				while (i < n && command[i] != '"')
					cmd_refined[counter++] = command[i++];
				if (i != n)
					cmd_refined[counter++] = command[i++];
			}
			while (command[i] == ' ')
			{
				i++;
				found_space = 1;
			}
			if (found_space) // found atlest one space => one space
				cmd_refined[counter++] = ' ';
			else
				cmd_refined[counter++] = command[i++];
		}
		if (cmd_refined[counter - 1] == ' ') // if end is space negelect it
		{
			cmd_refined[counter - 1] = '\0';
		}
		else
			cmd_refined[counter] = '\0';
		char *temp_path = (char *)malloc(1025);
		char *idk = (char *)malloc(1025);
		strcpy(idk,"cat ");
        temp_path[0]='\0';
        strcat(temp_path,home_path); 
        strcat(temp_path,"/jobs.txt");
		strcat(idk,temp_path);
		int fd = fork();
		if(fd == 0){
			FILE *fp = fopen(temp_path,"r");
			if(fp == NULL)
				exit(0);
			fclose(fp);
			execlp("bash","bash","-c",idk,NULL);
			exit(127);
		}
		else{
			waitpid(fd,NULL,0);
		}
		FILE *fp = fopen(temp_path,"w");
		fclose(fp); 
		int temp = validate(cmd_refined);
		if (!temp && temp != 118 ) // checking the string
		{
			printf("Invalid Syntax!\n");
			continue;
		}
		if(temp == 118)
			temp =1;
		else
			temp =0;
		int temp_jobs = my_exec(cmd_refined, prev, home_path, path_req,temp); // executing

		if (temp_jobs == -1)
		{
			printf("Error running cmf\n");
		}
		path_req = FindPath(home_path); // updating the path every time bcz path may change when we use hop
	}
}
