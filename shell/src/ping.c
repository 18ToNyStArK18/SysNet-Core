#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include"../include/include.h"

int str_int(char *str){
	int ans = 0;
	int i = 0;
	while( i < (int)strlen(str) && str[i] != ' '){
		ans = ans*10 + str[i] - '0';
		i++;
	}
	return ans;
}

int ping(char * cmd,char * home_path){

	int i=5;
	int pid = str_int(cmd+5);
	while(i <(int) strlen(cmd) && cmd[i] != ' ')
		i++;
	i++;
	int signal = str_int(cmd+i);
	signal =signal % 32;
	if(kill(pid,signal) == -1){
		return 0;
	}
	if (signal == SIGSTOP || signal == SIGTSTP) {
		char jobs_file_path[1024];
		char temp_file_path[1024];

		snprintf(jobs_file_path, sizeof(jobs_file_path), "%s/jobs_list.txt", home_path);
		snprintf(temp_file_path, sizeof(temp_file_path), "%s/jobs_list.tmp", home_path);

		FILE *jobs_file = fopen(jobs_file_path, "r");
		FILE *temp_file = fopen(temp_file_path, "w");

		if (!jobs_file || !temp_file) {
			perror("Error opening jobs file for status update");
			if (jobs_file) fclose(jobs_file);
			if (temp_file) fclose(temp_file);
			return pid;
		}

		char *line = NULL;
		size_t len = 0;

		while (getline(&line, &len, jobs_file) != -1) {
			int file_job_id, file_pid, file_state;
			char command_part[512];

			if (sscanf(line, "%d %d %d %511[^\n]", &file_job_id, &file_pid, &file_state, command_part) >= 3) {
				if (file_pid == pid) {
					fprintf(temp_file, "%d %d %d %s\n", file_job_id, file_pid, 2, command_part);
				} else {
					fputs(line, temp_file);
				}
			}
		}

		free(line);
		fclose(jobs_file);
		fclose(temp_file);

		remove(jobs_file_path);
		rename(temp_file_path, jobs_file_path);
	}
	return pid;
}
