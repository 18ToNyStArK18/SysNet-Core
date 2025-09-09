#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include <sys/wait.h>
#include <unistd.h>
#include"../include/include.h"
typedef struct proc{
	pid_t pid;
	char command[1024];
}proc;
int comp(const void *a,const void*b){
	proc A = *(proc *)a;
	proc B = *(proc *)b;
	return strcasecmp(A.command,B.command);

}
// ################ took ai help for this function
void activ(char *home_path) {
    char path[1025];
    snprintf(path, sizeof(path), "%s/proc.txt", home_path);

    FILE *fp = fopen(path, "r");
    if (!fp) return;

    proc data[1000];
    int count = 0;

    while (fscanf(fp, "%d %[^\n]", &data[count].pid, data[count].command) == 2) {
        count++;
    }
    fclose(fp);

    qsort(data, count, sizeof(proc), comp);

    fp = fopen(path, "w");
    if (!fp) return;

    for (int i = 0; i < count; i++) {
        pid_t pid = data[i].pid;
        char statpath[256];
        snprintf(statpath, sizeof(statpath), "/proc/%d/stat", pid);

        FILE *statfp = fopen(statpath, "r");
        if (!statfp) {
			//printf("[%d] : %s - Finished\n", pid, data[i].command);
            continue;
        }

        char comm[256], state;
        fscanf(statfp, "%*d %s %c", comm, &state);
        fclose(statfp);

        if (state == 'T') {
            printf("[%d] : %s - Stopped\n", pid, data[i].command);
            fprintf(fp, "%d %s\n", pid, data[i].command);
        } else if (state == 'Z') {
    		//printf("[%d] : %s - Finished\n", pid, data[i].command);
		} else {
            printf("[%d] : %s - Running\n", pid, data[i].command);
            fprintf(fp, "%d %s\n", pid, data[i].command);
        }
    }

    fclose(fp);
}


void add_proc(char *cmd,pid_t pid,char *home_path){
	char *path = (char *)malloc(1025);
	path[0]='\0';
	strcat(path,home_path);
	strcat(path,"/proc.txt");
	FILE *fp = fopen(path,"a");
	fprintf(fp,"%d %s\n",pid,cmd);
	fclose(fp);
	return;

}
