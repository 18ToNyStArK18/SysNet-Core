#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include "../include/include.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

int job_count = 0;
// find relative path
char *FindPath(char *home_path) {
    char *working_dir = (char *)malloc(sizeof(char) * PATH_MAX);
    if (getcwd(working_dir, PATH_MAX) == NULL) {
        perror("Error finding the curr_dir\n");
    }
    char *req_dir = (char *)malloc(sizeof(char) * PATH_MAX);
    req_dir[0] = '~';
    int h = strlen(home_path), w = strlen(working_dir);
    if (strncmp(working_dir, home_path, h) == 0) {
        for (int i = h; i < w; i++) {
            req_dir[i - h + 1] = working_dir[i];
        }
        req_dir[w - h + 1] = '\0';
        return req_dir;
    } else {
        return working_dir;
    }
}

int main() {
	// ignore Ctrl-C in the shell itself
//	pid_t shell_pgid = getpid();
  //  if (setpgid(shell_pgid, shell_pgid) < 0) {
    //    perror("Couldn't put the shell in its own process group");
      //  exit(1);
    //}
	//tcsetpgrp(STDIN_FILENO, shell_pgid);
	signal(SIGINT, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
	char *username = getlogin();
	char *sysname = (char *)malloc(1025);

	gethostname(sysname,1024);
	char *home_path = (char *)malloc(PATH_MAX);
	if (getcwd(home_path, PATH_MAX) == NULL)
		perror("Error finding home dir\n");
	char  file_path[1024];
	sprintf(file_path,"%s/jobs_list.txt",home_path);
	FILE *fp = fopen(file_path,"w");
	fclose(fp);

	char *path_req = FindPath(home_path);
	char *prev = (char *)malloc(PATH_MAX);
	prev[0] = '\0';

	while (1) {
		// shell prompt
		printf("<%s", username);
		printf("@%s:", sysname);
		printf("%s> ", path_req);

		char *command = (char *)malloc(4097);
		char *cmd_refined = (char *)malloc(4097);
		
		if (fgets(command, 4096,stdin) == NULL) {
            // This now only triggers on actual Ctrl+D (EOF)
            kill_jobs(home_path);
            printf("\nlogout\n");
            exit(0);
        }
		command[strcspn(command, "\n")] = 0;
		// refine command (remove extra spaces)
		int counter = 0, i = 0;
		int n = strlen(command);
		int found_space = 0;
		while (i < n) {
			found_space = 0;
			if (command[i] == '"') {
				cmd_refined[counter++] = '"';
				i++;
				while (i < n && command[i] != '"')
					cmd_refined[counter++] = command[i++];
				if (i != n)
					cmd_refined[counter++] = command[i++];
			}
			while (command[i] == ' ') {
				i++;
				found_space = 1;
			}
			if (found_space)
				cmd_refined[counter++] = ' ';
			else
				cmd_refined[counter++] = command[i++];
		}
		if (cmd_refined[counter - 1] == ' ')
			cmd_refined[counter - 1] = '\0';
		else
			cmd_refined[counter] = '\0';

		// clear jobs.txt before executing new command
		char *temp_path = (char *)malloc(1025);
		char *idk = (char *)malloc(1025);
		strcpy(idk, "cat ");
		temp_path[0] = '\0';
		strcat(temp_path, home_path);
		strcat(temp_path, "/jobs.txt");
		strcat(idk, temp_path);

		int fd = fork();
		if (fd == 0) {
			FILE *fp = fopen(temp_path, "r");
			if (fp == NULL)
				exit(0);
			fclose(fp);
			execlp("bash", "bash", "-c", idk, NULL);
			exit(127);
		} else {
			waitpid(fd, NULL, 0);
		}
		FILE *fp = fopen(temp_path, "w");
		fclose(fp);

		// validate command
		int temp = validate(cmd_refined);
		if (!temp && temp != 118) {
			printf("Invalid Syntax!\n");
			continue;
		}
		if (temp == 118)
			temp = 1;
		else
			temp = 0;

		// execute command
		int temp_jobs = my_exec(cmd_refined, prev, home_path, path_req, temp);
		if (temp_jobs == -1)
			printf("Error running command\n");

		// update path
		path_req = FindPath(home_path);
	}
}

