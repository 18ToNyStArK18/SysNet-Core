#include<stdio.h>
#include<regex.h>
#include<string.h>
#include<stdlib.h>
#include<pwd.h>
#include<unistd.h>
#include<sys/utsname.h>
#include<linux/limits.h>
#include<sys/wait.h>
#include <fcntl.h>  
#include <unistd.h>
#include<signal.h>
#include"../include/include.h"


int find_job_by_jid(int jid, char* home_path, Job* found_job) {
    char file_path[1024];
    sprintf(file_path, "%s/jobs_list.txt", home_path);
    FILE *fp = fopen(file_path, "r");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    Job current_job;
    while (fscanf(fp, "%d %d %d %[^\n]", &current_job.jid, &current_job.pid, &current_job.state, current_job.data) != EOF) {
        if (current_job.jid == jid && current_job.state != 0) { // Find active or stopped jobs
            *found_job = current_job;
			printf("%s\n",current_job.data);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0; // Not found
}

void remove_job_by_jid(int jid, char* home_path) {
    Job temp_jobs[1000];
    int count = 0;

    char file_path[1024];
    sprintf(file_path, "%s/jobs_list.txt", home_path);
    FILE *fp = fopen(file_path, "r");
    if (!fp) return;

    while (fscanf(fp, "%d %d %d %[^\n]", &temp_jobs[count].jid, &temp_jobs[count].pid, &temp_jobs[count].state, temp_jobs[count].data) != EOF) {
        count++;
    }
    fclose(fp);

    fp = fopen(file_path, "w");
    if (!fp) return;

    for (int i = 0; i < count; i++) {
        if (temp_jobs[i].jid != jid) { // Write back all jobs except the one to be removed
            fprintf(fp, "%d %d %d %s\n", temp_jobs[i].jid, temp_jobs[i].pid, temp_jobs[i].state, temp_jobs[i].data);
        }
    }
    fclose(fp);
}

void fg_command(char *command, char *home_path) {
		
	int n = strlen(command);
	n--;
	while(command[n]==' ')
		n--;
	n++;
	command[n]='\0';
	int jid;
	if(strcmp(command,"fg")==0)
		jid = job_count-1;
	else
		sscanf(command+3,"%d",&jid);
    Job job_to_fg;
    int find_result = find_job_by_jid(jid, home_path, &job_to_fg);

    if (find_result == 0) {
        printf("No such job\n");
        return;
    }
    if (find_result == -1) {
        printf("fg: error accessing job list.\n");
        return;
    }
    pid_t pgid = job_to_fg.pid;

    if (tcsetpgrp(STDIN_FILENO, pgid) == -1) {
        perror("tcsetpgrp");
        return;
    }

    if ( job_to_fg.state == 2 && kill(-pgid, SIGCONT) < 0) {
        perror("kill (SIGCONT)");
        return;
    }

    int status;
    waitpid(pgid, &status, WUNTRACED);
    tcsetpgrp(STDIN_FILENO, getpgrp());

    if (WIFEXITED(status) || WIFSIGNALED(status)) {
        remove_job_by_jid(jid, home_path);
    } else if (WIFSTOPPED(status)) {
        printf("\nJob [%d] stopped.\n", jid);
    }
}

void bg_command(char *command, char *home_path) {
	int n = strlen(command);
	n--;
	while(command[n]==' ')
		n--;
	n++;
	command[n]='\0';
	int jid;
	if(strcmp(command,"bg")==0)
		jid = job_count-1;
	else
		sscanf(command+3,"%d",&jid);
	Job job_to_bg;
	if (find_job_by_jid(jid, home_path, &job_to_bg) <= 0) {
		printf("No such job\n");
		return;
	}

	if (job_to_bg.state == 1) {
		printf("Job already running\n");
		return;
	}

	if (job_to_bg.state == 2) {
		if (kill(-job_to_bg.pid, SIGCONT) < 0) {
			perror("bg: kill (SIGCONT) failed");
			return;
		}
		printf("[%d] %s &\n", job_to_bg.jid, job_to_bg.data);
	}
}


